#include "Branch.h"
#include "Node.h"

#include <iostream>
using namespace std;
ostream& operator<<(ostream& out, complex<double> c);
Branch::Branch(std::string name, Node* start, Node* end, Type type, complex<double> component) :start(start), end(end)
{
	compName = name;
	start->addBranch(this);
	end->addBranch(this);
	pass = true;
	inLoop = false;
	polarity = 1;
	direction = 0;
	currentName = -1;
	known = false;
	TYPE = type;
	this->component = component;
	if (type == CS) {
		swap();
		setCurrentValue(component);
		setCN(-1);
	}
	else if (type == VS) {
		setVoltageValue(component);
	}
}


Branch::~Branch()
{
}

bool Branch::has(Node* node) const
{
	return start == node || end == node;
}

bool Branch::has(Type type) const
{
	return TYPE == type;
}

std::vector<Branch*> Branch::nextBranches(bool checkPass)
{
	std::vector<Branch*> bs = end->getBranches(this);
	if (!checkPass)
		return bs;
	int size = bs.size() - 1;
	for (int i = size; i >= 0; i--) {
		if (!bs[i]->pass)
			bs.erase(bs.begin() + i);

	}
	return bs;
}

void Branch::assumeCurrent(int & cn)
{
	if (getCN() != -1)
		return;
	bool alone = false;
	bool currentSource = false;
	complex<double> currentValue;
	std::vector<Branch*> longBranch;
	std::vector<Branch*> bs;
	bs.push_back(this);
	bool stop = false;
	while (!stop) {
		if (bs[0]->has(CS)) {
			currentSource = true;
			currentValue = bs[0]->getCurrentValue();
		}
		longBranch.push_back(bs[0]);
		stop = !bs[0]->getEnd()->isSimple();
		bs = bs[0]->nextBranches(false);
		if (bs.size() == 0) {
			alone = true;
			stop = true;
		}
	}
	for (int i = 0; i < longBranch.size(); i++) {
		if (alone) {
			longBranch[i]->setCurrentValue(0);
			longBranch[i]->setCN(-2);
			longBranch[i]->setVoltageValue(0);
		}
		else if (currentSource) {
			longBranch[i]->setCurrentValue(currentValue);
			longBranch[i]->setCN(-2);
		}
		else {
			longBranch[i]->setCN(cn);
		}
	}
	if (!currentSource && !alone)
		cn++;
}

Node * Branch::getStart()
{
	return start;
}

Node * Branch::getEnd()
{
	return end;
}

bool Branch::canPass() const
{
	return pass;
}

void Branch::setCanPass(bool b)
{
	pass = b;
}

void Branch::setInLoop(bool b)
{
	inLoop = b;
}

bool Branch::isInLoop()
{
	return inLoop;
}

void Branch::display() const
{
	cout << compName << ": I(" << start->getNumber() << "," << end->getNumber() << ") = " << getCurrentValue() <<endl;
}

void Branch::displayName() const
{
	cout << "(" << start->getNumber() << ", " << end->getNumber() << ")";
}

void Branch::swap()
{
	Node* tmp = start;
	start = end;
	end = tmp;
	polarity *= -1;
	direction *= -1;
}

Node * Branch::getCommon(Branch * branch)
{
	if (start == branch->start || start == branch->end)
		return start;
	else if (end == branch->start || end == branch->end)
		return end;
	return NULL;
}

void Branch::centerCommon(Branch * branch)
{
	if (end == branch->end)
		branch->swap();
	else if (start == branch->start)
		swap();
	else if (start == branch->end) {
		swap();
		branch->swap();
	}
}

void Branch::setCN(int currentName)
{
	this->currentName = currentName;
	direction = 1;
}

int Branch::getCN() const
{
	return currentName;
}

int Branch::getDirection() const
{
	return direction;
}

std::complex<double> Branch::getVs()
{
	if (TYPE == VS)
		return polarity == 1 ? component : -component;
	return 0;
}

std::complex<double> Branch::getZ()
{
	if (TYPE == Z)
		return direction == 1 ? component : -component;
	return 0;
}

std::complex<double> Branch::getCs()
{
	if (TYPE == CS)
		return direction == 1 ? component : -component;
	return 0;
}

void Branch::setCurrentValue(std::complex<double> cv)
{
	currentValue = cv;
	if (abs(currentValue.real()) < 10e-12)  currentValue.real(0);
	if (abs(currentValue.imag()) < 10e-12) currentValue.imag(0);
	if (TYPE == Z) {
		voltageValue = currentValue * component;
	}
	known = true;
}

void Branch::setVoltageValue(std::complex<double> vv)
{
	voltageValue = vv;
	if (abs(voltageValue.real()) < 10e-12)  voltageValue.real(0);
	if (abs(voltageValue.imag()) < 10e-12) voltageValue.imag(0);
}

std::complex<double> Branch::getCurrentValue() const
{
	return direction == 1 ? currentValue : -currentValue;
}

std::complex<double> Branch::getVoltageValue() const
{
	if (TYPE == VS)
		return polarity == 1 ? voltageValue : -voltageValue;
	return direction == 1 ? voltageValue : -voltageValue;
}

bool Branch::isKnown() const
{
	return known;
}
