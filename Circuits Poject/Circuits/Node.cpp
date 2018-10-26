#include "Node.h"
#include "Branch.h"
#include <complex>

int Node::nonSimple = 0;
Node::Node(int number)
{
	n = number;
	simple = true;
}


Node::~Node()
{
}

int Node::getNumber() const
{
	return n;
}

void Node::addBranch(Branch* branch)
{
	branches.push_back(branch);
	if (branches.size() == 3) {
		simple = false;
		nonSimple++;
	}
}

std::vector<Branch*> Node::getBranches(Branch* exclude) 
{
	allOut(exclude);
	std::vector<Branch*> bs;
	for (int i = 0; i < branches.size(); i++) {
		if (exclude && branches[i] == exclude) {
			exclude = NULL;
			continue;
		}
		bs.push_back(branches[i]);
	}
	return bs;
}

bool Node::isSimple() const
{
	return simple;
}

void Node::applyKCL(Eigen::MatrixXcd & m1, Eigen::MatrixXcd & m2, int& row)
{
	allOut();
	for (int i = 0; i < branches.size(); i++) {
		if (branches[i]->isKnown()) {
			std::complex<double> cv = -branches[i]->getCurrentValue();
			m2(row, 0).real(m2(row, 0).real() + cv.real());
			m2(row, 0).imag(m2(row, 0).imag() + cv.imag());
		}
		else {
			m1(row, branches[i]->getCN()).real(branches[i]->getDirection());
		}
	}
	row++;
}

void Node::allOut(Branch* exclude)
{
	for (int i = 0; i < branches.size(); i++) {
		if (exclude && branches[i] == exclude) {
			exclude = NULL;
			continue;
		}
		if (this != branches[i]->getStart())
			branches[i]->swap();
	}
}

void Node::setSimple(bool b)
{
	if (simple && !b) {
		nonSimple++;
	}
	else if (!simple && b) {
		nonSimple--;
	}
	simple = b;
}

void Node::setVoltage(std::complex<double> v)
{
	voltage = v;
	calculated = true;
}

std::complex<double> Node::getVoltage()
{
	return voltage;
}

bool Node::isCalculated() const
{
	return calculated;
}


