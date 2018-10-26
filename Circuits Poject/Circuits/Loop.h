#ifndef LOOP
#define LOOP
#include <vector>
#include "Branch.h"
#include <Eigen/Eigen>
#include <complex>
class Loop
{
	std::vector<Branch*> bs;
	void addToMatrix(Eigen::MatrixXcd & m1, Eigen::MatrixXcd & m2, Branch * b, int r, int c);
	bool check;
public:
	static int C_N;
	Loop();
	~Loop();
	void addBranch(Branch* branch);
	void addBranches(std::vector<Branch*> branches);
	void display() const;
	void applyKVL(Eigen::MatrixXcd & m1, Eigen::MatrixXcd & m2, int row);
	bool isEmpty() const;
	void arrange();
};
#endif

