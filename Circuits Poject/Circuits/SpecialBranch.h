#ifndef SPECIALBRANCH
#define SPECIALBRANCH
#include "Branch.h"
#include "Loop.h"
class SpecialBranch
{
private:
	SpecialBranch(Branch* start, Node* lostNode);
	int expand(); // 1 (reached) / -1 (hasn't reached yet) / 0 can't reach
	void formLoop(Loop &loop, SpecialBranch sb) const;
	bool inSpecialStage;
	std::vector<Branch*> bs;
	std::vector<SpecialBranch> specialBs;
	bool reached;
	Node* lostNode;
public:
	SpecialBranch();
	Loop getLoop(Branch* start);
	void reset();
};
#endif

