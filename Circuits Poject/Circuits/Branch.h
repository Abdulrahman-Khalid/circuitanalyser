#ifndef BRANCH
#define BRANCH
#include "Node.h"
#include <complex>
#include <string>
enum Type { CS, VS, Z };
class Branch
{
private:
	Node* start;
	Node* end;
	bool pass;
	bool inLoop;
	int direction;
	int polarity;
	int currentName;
	Type TYPE;
	std::complex<double> component;
	std::complex<double> currentValue;
	std::complex<double> voltageValue;
	std::string compName;
	bool known; //Current
public:
	Branch(std::string name,Node* start, Node* end, Type type, std::complex<double> component);
	~Branch();
	bool has(Node* node) const;
	bool has(Type type) const;
	std::vector<Branch*> nextBranches(bool checkPass = true);
	void assumeCurrent(int &cn);
	Node* getStart();
	Node* getEnd();
	bool canPass() const;
	void setCanPass(bool b);
	void setInLoop(bool b);
	bool isInLoop();
	void display() const;
	void displayName() const;
	void swap();
	Node* getCommon(Branch* branch);
	void centerCommon(Branch* branch);
	void setCN(int currentName);
	int getCN() const;
	int getDirection() const;
	std::complex<double> getVs();
	std::complex<double> getZ();
	std::complex<double>  getCs();
	void setCurrentValue(std::complex<double> cv);
	void setVoltageValue(std::complex<double> vv);
	std::complex<double> getCurrentValue() const;
	std::complex<double> getVoltageValue() const;
	bool isKnown() const;
};
#endif