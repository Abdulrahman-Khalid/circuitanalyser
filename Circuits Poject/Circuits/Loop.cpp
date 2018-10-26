#include "Loop.h"
#include <iostream>
using namespace std;

int Loop::C_N = 0;

void Loop::addToMatrix(Eigen::MatrixXcd & m1, Eigen::MatrixXcd & m2, Branch * b, int r, int c)
{
	m1(r, c).real(m1(r, c).real() + b->getZ().real());
	m1(r, c).imag(m1(r, c).imag() + b->getZ().imag());
	m2(r, 0).real(m2(r, 0).real() - b->getVs().real());
	m2(r, 0).imag(m2(r, 0).imag() - b->getVs().imag());

}

Loop::Loop()
{
}


Loop::~Loop()
{
}

void Loop::addBranch(Branch * branch)
{
	branch->setInLoop(true);
	bs.push_back(branch);
}

void Loop::addBranches(std::vector<Branch*> branches)
{
	for (int i = 0; i < branches.size(); i++) {
		addBranch(branches[i]);
	}
}

void Loop::display() const
{
	for (int i = 0; i < bs.size(); i++) {
		bs[i]->displayName();
	}
	cout << endl;
}

void Loop::applyKVL(Eigen::MatrixXcd & m1, Eigen::MatrixXcd & m2, int row)
{
	if (bs.size() == 0)
		return;
	arrange();
	for (int i = 0; i < bs.size(); i++) {
		addToMatrix(m1, m2, bs[i], row, bs[i]->getCN());
	}
}

bool Loop::isEmpty() const
{
	return bs.size() == 0;
}

void Loop::arrange()
{
	bs[0]->centerCommon(bs[1]);
	for (int i = 1; i < bs.size() - 1; i++) {
		if (bs[i]->getEnd() != bs[i + 1]->getStart())
			bs[i + 1]->swap();
	}
}
