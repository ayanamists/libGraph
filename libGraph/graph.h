/*
 * Author -- ayanamists
 * Email -- ayanamists@gmail.com
 * FOR EDUCATIONAL PURPOSE ONLY
 */

#pragma once
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <iostream>
#include <stdint.h>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include "distanct_set.h"

template<class Vertex>
class Graph {
public:
	virtual void addVertex(Vertex& v) = 0;
	virtual void addEdge(Vertex& v_1, Vertex& v_2) = 0;
	virtual void removeVertex(Vertex& v) = 0;
	virtual void removeEdge(Vertex& v_1, Vertex & v_2) = 0;
	virtual std::vector<Vertex> getNeighbors(Vertex& v) = 0;
	virtual uint32_t getNumberOfVertices() = 0;
	virtual uint32_t getNumberOfEdges() = 0;
	virtual void clearGraph() = 0;
	virtual ~Graph() {};
};

template<class value>
class stdVertex {
public:
	value val;
	stdVertex() {}
	stdVertex(value val) {
		this->val = val;
	}
	bool operator ==(stdVertex<value> other) {
		return this->val == other.val;
	}
};

/* 
 * We use IndexMapping to convert Vertex to index(an uint32)
 */
template<class value, class Vertex = stdVertex<value>>
class IndexMapping {
public:
	uint32_t getIndex(Vertex& v) {
		return this->valueToIndex.at(v.val);
	}
	value getValue(uint32_t index) {
		return this->indexToValue.at(index);
	}
	uint32_t addVertexToMap(Vertex& v) {
		this->indexToValue.push_back(v.val);
		this->valueToIndex.insert(
			std::pair<value, uint32_t>(v.val, this->indexToValue.size() - 1));
		return this->indexToValue.size() - 1;
	}
	uint32_t removeVertexFromMap(Vertex& v) {
		int index_v = this->getIndex(v);
		this->indexToValue.erase(indexToValue.begin() + index_v);
		for (auto i = valueToIndex.begin(); i != valueToIndex.end();) {
			if ((*i).second == index_v) {
				i = valueToIndex.erase(i);
			}
			else {
				if ((*i).second > index_v) {
					(*i).second--;
				}
				i++;
			}
		}
		return index_v;
	}
private:
	std::map<value, uint32_t> valueToIndex;
	std::vector<value> indexToValue;
};

class IndexEdge {
public:
	IndexEdge() { this->edge = std::pair<uint32_t, uint32_t>(0, 0); value = 0; }
	IndexEdge(uint32_t v_1 , uint32_t v_2, int value ) {
		this->edge = std::pair<uint32_t, uint32_t>(v_1, v_2);
		this->value = value;
	}
	std::pair<uint32_t, uint32_t> edge;
	int value;
	bool operator<(IndexEdge& other) {
		return this->value < other.value;
	}
};

template<class value, class Vertex = stdVertex<value>>
struct PathNode
{
	PathNode(Vertex _from, Vertex _to, int _value) {
		from = _from;
		to = _to;
		value = _value;
	}
	Vertex from;
	Vertex to;
	int value;
};

template<class value, class Vertex = stdVertex<value> >
class IndexGraph : public Graph<Vertex>, public IndexMapping<value, Vertex> {
public:
	/* Depth First Search */
	void DFS(std::function<void(Vertex)> do_something, Vertex start) {
		this->help_vec_8 = std::vector<uint8_t>(this->getNumberOfVertices(), false);
		uint32_t start_index = this->getIndex(start);
		this->STD_DFS(do_something, start_index);
	}
	
	/* Board Fisrt Search */
	void BFS(std::function<void(Vertex)> do_something, Vertex start) {
		auto _start = this->getIndex(start);
		this->help_vec_8 = std::vector<uint8_t>(this->getNumberOfVertices(), 0);
		std::queue<uint32_t> help_quene;
		help_quene.push(_start);
		for (;!help_quene.empty();) {
			uint32_t now = help_quene.front();
			help_quene.pop();
			help_vec_8[now] = true;
			do_something(Vertex(this->getValue(now)));
			auto neighbors = this->_get_neighbors(now);
			for (auto i : neighbors) {
				if (help_vec_8[i] != true) {
					help_quene.push(i);
				}
			}
		}
	}
	
	/*
	 * Minimal Spanning Tree algorithm can only be apply on undirected graph
	 */

	/* Minimal Spanning Tree -- Kruskal */
	std::vector<std::pair<Vertex, Vertex>> Kruskal() {
		std::vector<IndexEdge> vec = this->_get_all_edges();
		std::sort(vec.begin(), vec.end());
		auto i = vec.begin();
		auto count = 0U;
		std::vector<std::pair<Vertex, Vertex>> ret;
		IndexDSet<> set(this->getNumberOfVertices());
		for (; count < this->getNumberOfVertices() - 1;) {
			auto v_1 = (*i).edge.first;
			auto v_2 = (*i).edge.second;
			/* retrun true means we don't have a ring after merge */
			if (set.mergeTwoSet(v_1, v_2)) {
				ret.push_back(std::pair<Vertex, Vertex>( 
					Vertex(this->getValue(v_1)), Vertex(this->getValue(v_2))));
				count++;
			}
			i++;
		}
		return ret;
	}     

	/* Minimal Spanning Tree -- Prim */
	std::vector<std::pair<Vertex, Vertex>> Prim() {
		std::vector<bool> add_list(this->getNumberOfVertices(), false);
		std::vector<uint32_t> father(this->getNumberOfVertices(), INT32_MAX);
		std::vector<int> distance(this->getNumberOfVertices(), INT32_MAX);
		std::vector<std::pair<int, int>> result_vec;

		distance[0] = 0;
		father[0] = 0;
		for (auto i = 0u; i < this->getNumberOfVertices(); ++i) {
			auto pos = 0u;
			auto min = INT32_MAX;
			for (auto j = 0u; j < distance.size(); ++j) {
				if ((distance[j] < min)&& !add_list[j]) {
					pos = j;
					min = distance[j];
				}
			}
			add_list[pos] = true;
			for (auto j = 0u; j < distance.size(); ++j) {
				if ((distance[j] > _get_value(pos, j)) && !add_list[j]) {
					father[j] = pos;
					distance[j] = _get_value(pos, j);
				}
			}
		}

		std::vector<std::pair<Vertex, Vertex>> ret;
		for (auto i = 1u; i < add_list.size();++i) {
			Vertex v_1(this->getValue(i));
			Vertex v_2(this->getValue(father[i]));
			std::pair<Vertex, Vertex> pair(v_1, v_2);
			ret.push_back(pair);
		}
		return ret;
	}


	/* Shortest path algorithm -- Dijkatra */
	std::vector<PathNode<value>> Dijkstra(Vertex& start) {
		std::vector<uint32_t> father(this->getNumberOfVertices(), 0xffffffff);
		std::vector<int> shortest(this->getNumberOfVertices(), INT_MAX);
		std::vector<bool> mark(this->getNumberOfVertices(), false);
		auto v = this->getIndex(start);
		bool end = false;
		shortest[v] = 0;
		father[v] = v;
		auto pos = 0;
		for (; !end;) {
			auto min = INT_MAX;
			end = true;
			for (auto i = 0u; i < shortest.size(); ++i) {
				if (mark[i] == false && shortest[i] != INT_MAX) {
					end = false;
				}
				if (shortest[i] < min && mark[i] == false) {
					pos = i;
					min = shortest[i];
				}
			}
			mark[pos] = true;
			for (auto i = 0u; i < shortest.size(); ++i) {
				if (_get_value(pos, i) == INT_MAX) {
					continue;
				}
				if (shortest[i] > shortest[pos] + _get_value(pos, i) && mark[i] == false) {
					father[i] = pos;
					shortest[i] = shortest[pos] + _get_value(pos, i);
				}
			}
		}
		std::vector<PathNode<value>> ret;
		for (auto i = 0u; i < this->getNumberOfVertices();++i) {
			PathNode<value> p(Vertex(this->getValue(father[i])), Vertex(this->getValue(i)), shortest[i]);
			ret.push_back(p);
		}
		return ret;
	}

	
	
	/* Shortest path algorithm -- Floyd */
	std::vector<PathNode<value>> Floyd(Vertex v_1) {
		auto temp = std::vector<std::vector<int>>(this->getNumberOfVertices(), 
			std::vector<int>(this->getNumberOfVertices(), INT_MAX));
		
		for (auto i = 0u; i < this->getNumberOfVertices(); ++i) {
			for (auto j = 0u; j < this->getNumberOfVertices(); ++j) {
				temp[i][j] = this->_get_value(i, j);
			}
		}

		for (auto k = 0u; k < this->getNumberOfVertices(); ++k) {
			for (auto i = 0u; i < this->getNumberOfVertices(); ++i) {
				for (auto j = 0u; j < this->getNumberOfVertices(); ++j) {
					if (temp[i][k] == INT_MAX || temp[k][j] == INT_MAX) {
						continue;
					}
					if (temp[i][j] > temp[i][k] + temp[k][j]) {
						temp[i][j] = temp[i][k] + temp[k][j];
					}
				}
			}
		}

		auto v = this->getIndex(v_1);
		std::vector<PathNode<value>> ret;
		for (auto i = 0u; i < this->getNumberOfVertices(); ++i) {
			PathNode<value> temp_node(Vertex(this->getValue(v)), Vertex(this->getValue(i)), temp[v][i]);
			ret.push_back(temp_node);
		}
		return ret;
	}
#define RED 1
#define WHITE 0
#define GREEN -1
	/* if the graph have a ring */
	bool ifRing() {
		this->help_vec_8 = std::vector<uint8_t>(this->getNumberOfVertices(), WHITE);
		uint32_t index = 0;
		this->RING_DFS(index);
		return this->ring_hit;
	}

	/* Topological Sort */
	std::vector<Vertex> TopoSort() {
		if (ifRing()) { throw std::runtime_error("can't sort because of ring!"); }
		this->help_vec_8 = std::vector<uint8_t>(this->getNumberOfVertices(), 0);
		for (auto i = 0u; i < this->getNumberOfVertices(); ++i) {
			if (help_vec_8[i] != 1) {
				SORT_DFS(i);
			}
		}
		std::reverse(this->sort_result_vec.begin(), this->sort_result_vec.end());
		std::vector<Vertex> result_vec;
		for (auto i : sort_result_vec) {
			result_vec.push_back(Vertex(this->getValue(i)));
		}
		this->sort_result_vec.clear();
		return result_vec;
	}

	std::vector<Vertex> TopoSort_Another() {
		if (this->ifRing()) {
			throw std::runtime_error("can't sort because of ring!");
		}
		this->help_vec_32 = std::vector<uint32_t>(this->getNumberOfVertices(), 0);
		for (uint32_t i = 0; i < this->getNumberOfVertices(); ++i) {
			auto vec = this->_get_neighbors(i);
			for (auto j : vec) {
				this->help_vec_32[j]++;
			}
		}

		std::queue<uint32_t> que;
		for (auto i = 0; i < this->help_vec_32.size(); ++i) {
			if (this->help_vec_32[i] == 0) {
				que.push(i);
			}
		}

		while (!que.empty()) {
			auto now = que.front();
			que.pop();
			this->sort_result_vec.push_back(now);

			auto neighbors = this->_get_neighbors(now);
			for (auto i : neighbors) {
				if (--this->help_vec_32[i] == 0) {
					que.push(i);
				}
			}
		}

		std::vector<Vertex> result_vec;
		for (auto i : sort_result_vec) {
			result_vec.push_back(Vertex(this->getValue(i)));
		}
		this->sort_result_vec.clear();
		return result_vec;
	}

	void show_AOE() {
		this->TopoSort_AOE();
		std::cout << "VE : " << std::endl;
		for (auto i : this->sort_result_vec) {
			std::cout << this->getValue(i) << " : " << this->aoe_ve[i] << std::endl;
		}
		
		auto aoe_vl = std::vector<uint32_t>(this->getNumberOfVertices(), UINT32_MAX);
		aoe_vl[this->getNumberOfVertices() - 1] = aoe_ve[this->getNumberOfVertices() - 1];
		for (auto i = this->sort_result_vec.rbegin(); i != this->sort_result_vec.rend(); ++i) {
			auto now = *i;
			auto neighbor = this->_get_neighbors(now);

			auto& now_vl = aoe_vl[now];
			for (auto j : neighbor) {
				if (now_vl > aoe_vl[j] - _get_value(now, j)) {
					now_vl = aoe_vl[j] - _get_value(now, j);
				}
			}
		}

		std::cout << "VL : " << std::endl;
		for (auto i : this->sort_result_vec) {
			std::cout << this->getValue(i) << " : " << aoe_vl[i] << std::endl;
		}
	}
protected:
	virtual std::vector<uint32_t> _get_neighbors(uint32_t index) = 0;
	virtual int _get_value(uint32_t, uint32_t) { return INT_MAX; }
	virtual std::vector<IndexEdge> _get_all_edges() = 0;
private:
	void STD_DFS(std::function<void(Vertex)> do_something, uint32_t vertex) {
		do_something(Vertex(this->getValue(vertex)));
		help_vec_8[vertex] = true;
		std::vector<uint32_t> list = this->_get_neighbors(vertex);
		for (auto i : list) {
			if (help_vec_8[i] == false) {
				STD_DFS(do_something, i);
			}
		}
	}
	void RING_DFS(uint32_t index) {
		if (ring_hit == true) {
			return;
		}
		if (help_vec_8[index] == RED) {
			ring_hit = true;
			return;
		}
		else {
			help_vec_8[index] = RED;
			std::vector<uint32_t> list = this->_get_neighbors(index);
			for (auto i : list) {
				if (help_vec_8[i] != GREEN)
				{
					RING_DFS(i);
				}
			}
			help_vec_8[index] = GREEN;
		}
	}
	void SORT_DFS(uint32_t index){
		this->help_vec_8[index] = 1;
		std::vector<uint32_t> neighbor = this->_get_neighbors(index);
		for (auto i : neighbor) {
			if (help_vec_8[i] != 1) {
				SORT_DFS(i);
			}
		}
		sort_result_vec.push_back(index);
	}
	void TopoSort_AOE() {
		if (this->ifRing()) {
			throw std::runtime_error("can't sort because of ring!");
		}

		this->help_vec_32 = std::vector<uint32_t>(this->getNumberOfVertices(), 0);
		for (uint32_t i = 0; i < this->getNumberOfVertices(); ++i) {
			auto vec = this->_get_neighbors(i);
			for (auto j : vec) {
				this->help_vec_32[j]++;
			}
		}

		this->aoe_ve = std::vector<uint32_t>(this->getNumberOfVertices(), 0);

		std::queue<uint32_t> que;
		for (auto i = 0; i < this->help_vec_32.size(); ++i) {
			if (this->help_vec_32[i] == 0) {
				que.push(i);
			}
		}

		while (!que.empty()) {
			auto now = que.front();
			que.pop();
			this->sort_result_vec.push_back(now);

			auto neighbors = this->_get_neighbors(now);
			for (auto i : neighbors) {
				if (--this->help_vec_32[i] == 0) {
					que.push(i);
				}
				if (this->aoe_ve[i] < this->aoe_ve[now] + _get_value(now, i)) {
					this->aoe_ve[i] = this->aoe_ve[now] + _get_value(now, i);
				}
			}
		}
	}
	std::vector<uint8_t> help_vec_8;
	std::vector<uint32_t> sort_result_vec;
	std::vector<uint32_t> help_vec_32;
	std::vector<uint32_t> aoe_ve;
	bool ring_hit;
};

/*
 * here we use Vertex class as node,
 * to correctly implement this class, you should follow these rules:
 * 1. overload =(), or don't use anything could cause deep copy/shallow copy problem.
 * 2. implement these methods:
 *    1. getValue()
 */
template<class value, class Vertex = stdVertex<value>, class VertexInner = stdVertex<uint32_t>>
class ListGraph : public IndexGraph<value, Vertex>{
public:
	void addVertex(Vertex& v) {
		std::list<VertexInner> vec_list;
		this->addVertexToMap(v);
		this->adjList.push_back(vec_list);
	}

	void addEdge(Vertex& v_1, Vertex& v_2) {
		auto index = this->getIndex(v_1);
		VertexInner v_in;
		v_in.val = this->getIndex(v_2);
		this->adjList[index].push_back(v_in);
	}

	void removeVertex(Vertex& v) {
		auto index = this->getIndex(v);
		this->adjList.erase(this->adjList.begin() + index);
		for (auto i = this->adjList.begin(); i != this->adjList.end(); ++i) {
			for (auto j = (*i).begin(); j != (*i).end();) {
				if ((*j).val == index) {
					j = (*i).erase(j);
				}
				else {
					if ((*j).val > index) {
						(*j).val--;
					}
					j++;
				}
			}
		}
		this->removeVertexFromMap(v);
	}

	void removeEdge(Vertex& v_1, Vertex& v_2) {
		auto index_1 = this->getIndex(v_1);
		auto index_2 = this->getIndex(v_2);
		auto list = this->adjList[index_1];
		for (auto i = list.begin(); i != list.end();) {
			if ((*i).val == index_2) {
				i = list.erase(i);
			}
			else {
				i++;
			}
		}
	}

	std::vector<Vertex> getNeighbors(Vertex& v_1) {
		auto index = this->getIndex(v_1);
		auto& list = this->adjList[index];
		std::vector<Vertex> ret;
		for (auto i : list) {
			ret.push_back(Vertex(this->getValue(i.val)));
		}
		return ret;
	}

	uint32_t getNumberOfVertices() {
		return this->adjList.size();
	}
	
	uint32_t getNumberOfEdges() {
		auto sum = 0;
		std::for_each(this->adjList.begin(), this->adjList.end(),
			[&sum](auto i) {sum += i.size(); });
		return sum;
	}

	void clearGraph () {
		adjList.clear();
	}
	
	std::vector<IndexEdge> _get_all_edges() {
		throw std::runtime_error("can't apply Krustal in list graph!");
	}
protected:
	std::vector<std::list<VertexInner> > adjList;
	std::vector<uint32_t> _get_neighbors(uint32_t index) {
		std::vector<uint32_t> ret;
		auto& list = this->adjList[index];
		for (auto i : list) {
			ret.push_back(i.val);
		}
		return ret;
	}
};

class ValuedVertex : public stdVertex<uint32_t> {
	/* edge_value represents the value of edge 
	 * between the head vertex and that vertex
	 * if this vertex is the first vertex, 
	 * then edge_value MUST be INT32_MAX
	 */
public:
	ValuedVertex(){
		val = 0;
		edge_value = 0;
	}
	uint32_t edge_value;
};

template<class value, class Vertex = stdVertex<value> >
class ValuedListGraph : public ListGraph<value, stdVertex<value>, ValuedVertex> {
public:
	void addEdge(Vertex& v_1, Vertex& v_2, uint32_t _value) {
		auto& list = this->adjList.at(this->getIndex(v_1));
		auto index_2 = this->getIndex(v_2);
		ValuedVertex vex;
		vex.val = index_2;
		vex.edge_value = _value; 
		list.push_back(vex);
	}	

	typedef std::pair<uint32_t, int> P;
	/* only when use listGraph, fastDijkstra is faster than normal Dijkstra */
	std::vector<PathNode<value>> fastDijkstra(Vertex& start) {
		std::vector<uint32_t> father(this->getNumberOfVertices(), 0xffffffff);
		std::vector<int> shortest(this->getNumberOfVertices(), INT_MAX);

		// pair.first means index, pair.second means distance
		auto cmp = [](auto i, auto j) { return i.second > j.second; };
		std::priority_queue < P, std::vector<P>, std::function<bool(P, P)>> que(cmp);

		auto start_index = this->getIndex(start);
		que.push(P(start_index, 0));
		shortest[start_index] = 0;
		father[start_index] = start_index;

		while (!que.empty())
		{
			auto now = que.top();
			que.pop();

			int distance_now = now.second;
			for (auto i : this->adjList[now.first]) {
				if (shortest[i.val] > i.edge_value + distance_now) {
					shortest[i.val] = i.edge_value + distance_now;
					father[i.val] = now.first;
					que.push(P(i.val, shortest[i.val]));
				}
			}
		}
		std::vector<PathNode<value>> ret;
		for (auto i = 0u; i < this->getNumberOfVertices(); ++i) {
			PathNode<value> p(Vertex(this->getValue(father[i])), Vertex(this->getValue(i)), shortest[i]);
			ret.push_back(p);
		}
		return ret;
	}
protected:
	int _get_value(uint32_t pos_1, uint32_t pos_2) {
		if (pos_1 == pos_2) {
			return 0;
		}
		for (auto i : this->adjList[pos_1]) {
			if (pos_2 == i.val) {
				return i.edge_value;
			}
		}
		return INT32_MAX;
	}
};

#define UNCONNECTED INT_MAX
template <class value, class Vertex = stdVertex<value>>
class MatrixGraph : public IndexGraph<value, Vertex> {
public:
	MatrixGraph(){}
	void addVertex(Vertex& v) {
		/* add colunm */
		for (auto i = matrix.begin(); i < matrix.end(); ++i) {
			(*i).push_back(UNCONNECTED);
		}

		/* add row */
		size_t vec_size = matrix.size() + 1;
		std::vector<uint32_t> vec(vec_size, UNCONNECTED);
		vec[vec_size - 1] = 0;
		matrix.push_back(vec);

		this->addVertexToMap(v);
	}
	
	void addEdge(Vertex& v_1, Vertex& v_2) { addEdge(v_1, v_2, 1); }

	void addEdge(Vertex& v_1, Vertex& v_2, int value) {
		uint32_t i = this->getIndex(v_1);
		uint32_t j = this->getIndex(v_2);
		matrix[i][j] = value;
	}

	void removeVertex(Vertex& v) {
	}

	void removeEdge(Vertex& v_1, Vertex& v_2){}

	std::vector<Vertex> getNeighbors(Vertex& v) {
		std::vector<Vertex> ret;
		uint32_t i = this->getIndex(v);
		for (auto j = 0u; j < this->matrix.size();++j) {
			if (this->_if_connected(i, j)) {
				ret.push_back(Vertex(this->getValue(j)));
			}
		}
		return ret;
	}

	uint32_t getNumberOfVertices() {
		return matrix.size();
	}

	uint32_t getNumberOfEdges() {
		uint32_t ret = 0;
		for (auto i = 0u; i < matrix.size(); ++i) {
			for (auto j = 0u; j < matrix.size(); ++j) {
				if (_if_connected(i, j)) {
					ret++;
				}
			}
		}
		return ret;
	}

	void clearGraph() {
		matrix.clear();
	}

	

protected:
	std::vector<std::vector<uint32_t>> matrix;
	
	bool _if_connected(uint32_t index_1, uint32_t index_2) {
		return this->matrix[index_1][index_2] != UNCONNECTED && index_1 != index_2;
	}

	std::vector<uint32_t> _get_neighbors(uint32_t index) {
		std::vector<uint32_t> ret;
		for (auto i = 0u; i < matrix[index].size(); ++i) {
			if (_if_connected(index, i)) {
				ret.push_back(i);
			}
		}
		return ret;
	}
	int _get_value(uint32_t pos_1, uint32_t pos_2) {
		return matrix[pos_1][pos_2];
	}
	std::vector<IndexEdge> _get_all_edges() {
		std::vector<IndexEdge> ret;
		for (auto i = 0u; i < this->matrix.size(); ++i) {
			for (auto j = i + 1; j < this->matrix.size(); ++j) {
				if (matrix[i][j] != UNCONNECTED) {
					ret.push_back(IndexEdge(i, j, matrix[i][j]));
				}
			}
		}
		return ret;
	}
};

template<class value, class Vertex = stdVertex<value>>
class UndirectedListGraph : public ValuedListGraph<value, Vertex> {
public:
	void addEdge(Vertex v_1, Vertex v_2, int _value) {
		ValuedListGraph<value, Vertex>::addEdge(v_1, v_2, _value);
		ValuedListGraph<value, Vertex>::addEdge(v_2, v_1, _value);
	}

	typedef std::pair<uint32_t, int> P;
	class temp_edge {
	public:
		temp_edge(uint32_t _father, int _value) {
			father = _father;
			value = _value;
			mark = false;
		}
		uint32_t father;
		int value;
		bool mark;
	};
	std::vector<std::pair<Vertex, Vertex>> fastPrim() {
		std::vector<temp_edge> vec(this->getNumberOfVertices(), temp_edge(0xfffffff, INT32_MAX));
		auto begin = 0;
		
		auto cmp = [](auto i, auto j) { return i.second > j.second; };
		std::priority_queue<P, std::vector<P>, std::function<bool(P, P)> > pq(cmp);

		pq.push(P(begin, 0));
		while (!pq.empty()) {
			auto now = pq.top();
			pq.pop();
			vec[now.first].mark = true;
			for (auto i : this->adjList[now.first]) {
				if (vec[i.val].value > i.edge_value && vec[i.val].mark == false) {
					vec[i.val].father = now.first;
					vec[i.val].value = i.edge_value;
					pq.push(P(i.val, i.edge_value));
				}
			}
		}

		std::vector<std::pair<Vertex, Vertex>> ret;
		for (auto i = 1u; i < vec.size(); ++i) {
			Vertex v_1(this->getValue(i));
			Vertex v_2(this->getValue(vec[i].father));
			std::pair<Vertex, Vertex> pair(v_1, v_2);
			ret.push_back(pair);
		}
		return ret;
	}
private:
};

template<class value, class Vertex = stdVertex<value>>
class UndirectedMatrixGraph : public MatrixGraph<value, Vertex> {
public:
	void addEdge(Vertex v_1, Vertex v_2, int _value) {
		MatrixGraph<value, Vertex>::addEdge(v_1, v_2, _value);
		MatrixGraph<value, Vertex>::addEdge(v_2, v_1, _value);
	}
};
