#ifndef NODE
#define NODE
#include <vector>
#include<Eigen/Eigen>
class Branch;
class Node
{
private:
	int n;
	bool simple;
	bool calculated;
	std::vector<Branch*> branches;
	std::complex<double> voltage;
public:
	static int nonSimple;
	Node(int n);
	~Node();
	int getNumber() const;
	void addBranch(Branch* branch);
	std::vector<Branch*> getBranches(Branch* exclude = NULL);
	bool isSimple() const;
	void applyKCL(Eigen::MatrixXcd& m1, Eigen::MatrixXcd & m2, int& row);
	void allOut(Branch* exclude = NULL);
	void setSimple(bool b);
	void setVoltage(std::complex<double> v);
	std::complex<double> getVoltage();
	bool isCalculated() const;
};
#endif
