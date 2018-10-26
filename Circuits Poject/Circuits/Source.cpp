#include <iostream>
#include <string>
#include <fstream>
#include <locale>
#include "Branch.h"
#include "SpecialBranch.h"
#include "Nodes.h"
#define PI 3.14159265359
using namespace std;


vector<Branch*> bs;

Branch* getBranch(); //get a branch which is not in a loop
int lastBranch = 0;

void display(Eigen::MatrixXcd matrix, int row, int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout << "(" << matrix(i, j).real() << " " << matrix(i, j).imag() << ")    ";
		}
		cout << endl;
	}
}

string tolower(string &str) {
	std::locale loc;
	string s;
	for (std::string::size_type i = 0; i < str.length(); ++i)
		s += std::tolower(str[i], loc);
	return s;
}

ostream& operator<<(ostream& out, complex<double> c) {
	if (c.real() != 0)
		out << c.real();
	if (c.imag() != 0)
		out << c.imag() << "j";
	if (c.real() == 0 && c.imag() == 0)
		out << 0;
	//out << "\t" << abs(c) << "<" << arg(c);
	return out;
}

int main(int argc, char* argv[]) {
	if (argc < 2)
		return 0;
	ifstream in(argv[1]);
	//ifstream in("file.txt");
	if (!in.is_open())
		return 0;
	bool flag = false;
	Nodes node;
	double omega;
	in >> omega;
	string name;
	string lname;
	in >> name;
	lname = tolower(name);
	int Vsdefname = 0, Csdefname = 0, Rdefname = 0, Cdefname = 0, Ldefname = 0;
	while(!in.fail()) {
		string temp = lname;
		if (name.size() > 2)
		{
			temp = lname.substr(0, 2);
		}
		if (temp != "vs" &&temp != "cs" && temp[0] != 'r' && temp[0] != 'c' && temp[0] != 'l')
		{
			cout << "The component name (" << name << ") is not supported, Please read the Readme file and try again!!!" << endl;
			in.close();
			return 0;
		}
		Node *n1, *n2;
		try {
			int x, y;
			in >> x >> y;
			n1 = &node[x];
			n2 = &node[y];
		}
		catch (...) {
			return 0;
		}
		if (temp == "vs")
		{
			double r, ph;
			in >> r >> ph;
			if (name.size() == 2)
			{
				name += to_string(Vsdefname++);
			}
			bs.push_back(new Branch(name, n1, n2, VS, polar(r, ph * PI / 180)));
		}
		else if (temp == "cs")
		{
			double r, ph;
			in >> r >> ph;
			if (name.size() == 2)
			{
				name += to_string(Csdefname++);
			}
			bs.push_back(new Branch(name, n1, n2, CS, polar(r, ph * PI / 180)));
			bs.back()->setCanPass(false);
		}
		else if (temp[0] == 'r')
		{
			flag = true;
			double v;
			in >> v;
			if (name.size() == 1)
			{
				name += to_string(++Rdefname);
			}
			bs.push_back(new Branch(name, n1, n2, Z, complex<double>(v, 0)));
		}
		else if (temp[0] == 'c')
		{
			flag = true;
			double v;
			in >> v;
			v = -1 / (v*omega);
			if (name.size() == 1)
			{
				name += to_string(++Cdefname);
			}
			bs.push_back(new Branch(name, n1, n2, Z, complex<double>(0, v)));
		}
		else if (temp[0] == 'l')
		{
			flag = true;
			double v;
			in >> v;
			v = v * omega;
			if (name.size() == 1)
			{
				name += to_string(++Ldefname);
			}
			bs.push_back(new Branch(name, n1, n2, Z, complex<double>(0, v)));
		}
		if (in.eof())
			break;
		in >> name;
		lname = tolower(name);
	}
	if (in.fail())
	{
		cout << "The input is not in the right format. Please read the Readme.txt file" << endl;
		return 0;
	}
	in.close();
	if (!flag)
	{
		cout << "Short circuit is detected, Can't analyse this circuit...";
		return 0;
	}
	SpecialBranch sb;
	Branch* start;
	vector<Loop> loops;
	while (start = getBranch()) {
		Loop loop = sb.getLoop(start);
		if (!loop.isEmpty()) {
			loops.push_back(loop);
		}
	}
	if (Node::nonSimple == 0)
		bs[0]->getStart()->setSimple(false);
	int x = loops.size() + Node::nonSimple - 1;
	Eigen::MatrixXcd m1(x, x);
	Eigen::MatrixXcd m2(x, 1);
	Eigen::MatrixXcd ans(x, 1);
	m1.setZero();
	m2.setZero();
	ans.setZero();
	int row = 0;
	node.applyKCL(m1, m2, row);
	for (int i = 0; i < loops.size(); i++) {
		loops[i].applyKVL(m1, m2, i + row);
		//loops[i].display();
	}
	/*
	display(m1, x, x);
	cout << "----------------" << endl;
	display(m2, x, 1);
	cout << "----------------" << endl;
	display(ans, x, 1);
	*/
	ans = m1.inverse() * m2;
	for (int i = 0; i < bs.size(); i++) {
		if (!bs[i]->isKnown()) {
			bs[i]->setCurrentValue(ans(bs[i]->getCN(), 0));
		}
		bs[i]->display();
	}
	node.calcV();
	node.display();
	for (int i = 0; i < bs.size(); i++) {
		delete bs[i];
	}
	return 0;
}

Branch* getBranch() {
	for (int i = lastBranch; i < bs.size(); i++) {
		if (!bs[i]->isInLoop() && bs[i]->canPass()) {
			lastBranch = i + 1;
			return bs[i];
		}
	}
	return NULL;
}

/*
100
Vs 1 0 10 0
R 1 2 10
R 2 0 10
R 2 0 10
ok

100
Vs 1 0 20 0
R 1 2 3
R 2 3 2
R 3 4 2
Vs 4 0 12 0
R 3 5 1
R 5 6 1
Vs 6 0 9 0
R 5 7 3
R 7 8 2
Vs 0 8 18 0
ok

100
Vs 1 0 20 0
R 1 2 3
R 2 3 2
R 3 4 2
Vs 4 0 12 0
R 3 5 1
R 5 6 1
Vs 6 0 9 0
R 5 7 3
R 7 8 2
Cs 0 8 18 0
ok

1
Vs 1 0 10 0
R 1 2 1
R 2 0 2
Cs 2 0 5 0
ok


1
Cs 1 0 3 0
R 1 0 0.5
R 1 2 0.5
Vs 2 3 1.5 0
Cs 4 3 6 0
R 3 4 0.25
R 4 0 0.5
R 0 3 1
ok


100
Vs 1 0 10 30
L 1 2 .001
C 2 3 0.0001
R 3 0 10
ok

200000
Cs 1 0 8 0
R 1 0 10
R 1 2 6
L 2 0 0.00004
C 1 0 0.000001
ok


Error
1
Cs 0 1 0.025 0
L 2 0 200
R 2 3 100
R 3 4 100
C 4 0 .005
ok
*/
