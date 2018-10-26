#include "SpecialBranch.h"



SpecialBranch::SpecialBranch(Branch *start, Node* lostNode) : lostNode(lostNode)
{
	reached = false;
	inSpecialStage = false;
	bs.push_back(start);
}


int SpecialBranch::expand() {
	if (inSpecialStage) {
		int hope = 0;
		int state = 0;
		for (int i = 0; i < specialBs.size(); i++) {
			state = specialBs[i].expand();
			hope += state;
			if (state == 1) {
				reached = true;
				return 1;
			}
		}
		if (hope == 0)
			return 0;
	}
	else {
		bs.back()->setCanPass(false);
		std::vector<Branch*> nbs = bs.back()->nextBranches();
		size_t size = nbs.size();
		if (size == 1) {
			bs.push_back(nbs[0]);
			if (nbs[0]->has(lostNode)) {
				reached = true;
				return 1;
			}
		}
		else if (size > 1) {
			inSpecialStage = true;
			for (int i = 0; i < size; i++) {
				specialBs.push_back(SpecialBranch(nbs[i], lostNode));
				if (nbs[i]->has(lostNode)) {
					specialBs.back().reached = true;
					reached = true;
					return 1;
				}
			}
		}
		else {
			return 0;
		}
	}
	return -1;
}

void SpecialBranch::formLoop(Loop & loop, SpecialBranch sb) const
{
	loop.addBranches(sb.bs);
	for (int i = 0; i < sb.specialBs.size(); i++) {
		if (sb.specialBs[i].reached) {
			formLoop(loop, sb.specialBs[i]);
			break;
		}
	}
}

SpecialBranch::SpecialBranch()
{
}

Loop SpecialBranch::getLoop(Branch* start)
{
	reached = false;
	inSpecialStage = false;
	bs.push_back(start);
	lostNode = start->getStart();
	int state;
	while ((state = expand()) == -1) {}
	if (state == 0) {
		reset();
		return Loop();
	}
	Loop loop;
	formLoop(loop, *this);
	reset();
	return loop;
}

void SpecialBranch::reset()
{
	std::vector<SpecialBranch*> v;
	v.push_back(this);
	SpecialBranch* r;
	int j = 0;
	int size = 1;
	while (j < size) {
		r = v[j];
		r->reached = false;
		for (int i = 0; i < (r->bs.size()); i++) {
			r->bs[i]->setCanPass(true);
		}
		j++;
		for (int i = 0; i < (r->specialBs.size()); i++) {
			v.push_back(&r->specialBs[i]);
			size++;
		}
	}
	for (int i = size - 1; i >= 0; i--) {
		v[i]->bs.clear();
		v[i]->specialBs.clear();
	}
}

