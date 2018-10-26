#include "Nodes.h"
#include "Branch.h"
#include <iostream>
std::ostream& operator<<(std::ostream& out, std::complex<double> c);
Nodes::Nodes()
{
	for (int i = 0; i < max; i++) {
		nodes[i] = NULL;
	}
}


Nodes::~Nodes()
{
	for (int i = 0; i < max; i++) {
		if (nodes[i])
			delete nodes[i];
	}
}

Node& Nodes::operator[](int n) {
	if (n < 0) {
		std::cout << "Node number can't be a negative value" << std::endl;
		throw 0;
	}
	else if (n > max - 1) {
		std::cout << "Node number can't be a greater than " << max - 1 << std::endl;
		throw 0;
	}
	if (nodes[n])
		return *nodes[n];
	nodes[n] = new Node(n);
	return *nodes[n];

}

void Nodes::applyKCL(Eigen::MatrixXcd& m1, Eigen::MatrixXcd& m2, int & row)
{
	int cn = 0;
	bool first = true;
	for (int i = 0; i < max; i++) {
		if (!nodes[i] || nodes[i]->isSimple())
			continue;
		std::vector<Branch*> bs = nodes[i]->getBranches();
		for (int j = 0; j < bs.size(); j++) {
			bs[j]->assumeCurrent(cn);

		}
		if (first) {
			first = false;
			continue;
		}
		nodes[i]->applyKCL(m1, m2, row);
	}
}

void Nodes::calcV()
{
	Node* node = getFirstNode();
	if (!node)
		return;
	node->setVoltage(0);
	std::vector<Node*> list;
	list.push_back(node);
	std::vector<Branch*> bs;
	while (list.size() != 0) {
		node = list.back();
		list.pop_back();
		bs = node->getBranches();
		for (int i = 0; i < bs.size(); i++) {
			if (bs[i]->has(CS) || bs[i]->getEnd()->isCalculated())
				continue;
			bs[i]->getEnd()->setVoltage(node->getVoltage() - bs[i]->getVoltageValue());
			list.push_back(bs[i]->getEnd());
		}
	}

}

void Nodes::display() const
{
	for (int i = 0; i < max; i++) {
		if (nodes[i])
			std::cout << "V(" << i << ") " << nodes[i]->getVoltage() << std::endl;
	}
}

Node* Nodes::getFirstNode()
{
	for (int i = 0; i < max; i++) {
		if (nodes[i])
			return nodes[i];
	}
	return NULL;
}
