#pragma once
#include <vector>
#include <stdint.h>

#define IndexType uint32_t
class DSNode {
public:
	IndexType getParent() {
		return parent;
	}
	void setParent(IndexType& index) {
		this->parent = index;
	}
protected:
	IndexType parent;
};

template <class Node = DSNode>
class IndexDSet {
public:
	IndexDSet(){}
	IndexDSet(IndexType count) {
		vec = std::vector<Node>(count);
		for (auto i = 0u; i < vec.size(); ++i) {
			this->vec[i].setParent(i);
		}
	}
	IndexType getParent(IndexType target) {
		if (this->vec[target].getParent() == target) {
			return target;
		}
		else {
			return getParent(this->vec[target].getParent());
		}
	}
	bool ifSame(IndexType ele_a, IndexType ele_b) {
		IndexType parent_a = getParent(ele_a);
		IndexType parent_b = getParent(ele_b);
		
		return parent_a == parent_b;
	}
	bool mergeTwoSet(IndexType ele_a, IndexType ele_b){
		IndexType parent_a = getParent(ele_a);
		IndexType parent_b = getParent(ele_b);
		if (parent_a == parent_b) {
			return false;
		}
		else {
			this->vec[parent_a].setParent(parent_b);
			return true;
		}
	}
protected:
	std::vector<Node> vec;
};