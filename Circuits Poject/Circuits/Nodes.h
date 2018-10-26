#ifndef NODES
#define NODES
#include "Node.h"
#define max 100
class Nodes
{
private:
	Node * nodes[max];
public:
	Nodes();
	~Nodes();
	Node& operator[](int n);
	void applyKCL(Eigen::MatrixXcd& m1, Eigen::MatrixXcd& m2, int& row);
	void calcV();
	void display() const;
	Node* getFirstNode();
};
#endif

