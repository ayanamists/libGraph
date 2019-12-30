#include "graph.h"
#include <iostream>
int main() {
	// stdVertex<std::string> v_0("v_0");
	stdVertex<std::string> v_1("v_1");
	stdVertex<std::string> v_2("v_2");
	stdVertex<std::string> v_3("v_3");
	stdVertex<std::string> v_4("v_4");
	stdVertex<std::string> v_5("v_5");
	stdVertex<std::string> v_6("v_6");
	stdVertex<std::string> v_7("v_7");
	stdVertex<std::string> v_8("v_8");

	ValuedListGraph < std::string > graph;
	// graph.addVertex(v_0);
	graph.addVertex(v_1);
	graph.addVertex(v_2);
	graph.addVertex(v_3);
	graph.addVertex(v_4);
    graph.addVertex(v_5);
	graph.addVertex(v_6);
	graph.addVertex(v_7);
	graph.addVertex(v_8);
	
	graph.addEdge(v_1, v_2, 8);
	graph.addEdge(v_2, v_4, 14);
	graph.addEdge(v_1, v_3, 12);
	graph.addEdge(v_3, v_4, 10);
	graph.addEdge(v_4, v_5, 6);
	graph.addEdge(v_3, v_6, 28);
	graph.addEdge(v_4, v_6, 8);
	graph.addEdge(v_5, v_7, 18);
	graph.addEdge(v_6, v_7, 6);
	graph.addEdge(v_7, v_8, 12);

	graph.show_AOE();
	/*
	graph.addEdge(v_0, v_1, 4);
	graph.addEdge(v_0, v_2, 3);
	graph.addEdge(v_1, v_3, 1);
	graph.addEdge(v_2, v_3, 3);
	graph.addEdge(v_3, v_4, 9);
	graph.addEdge(v_4, v_5, 10);
	graph.addEdge(v_5, v_6, 2);
	graph.addEdge(v_6, v_7, 1);
	graph.addEdge(v_7, v_0, 8);
	graph.addEdge(v_7, v_1, 11);
	graph.addEdge(v_7, v_8, 7);
	graph.addEdge(v_2, v_8, 2);
	graph.addEdge(v_6, v_8, 6);
	graph.addEdge(v_2, v_5, 4);
	graph.addEdge(v_1, v_7, 11);
	graph.addEdge(v_3, v_5, 14);
	*/

	/*
	auto ret =
		graph.Dijkstra("v_0");
		for (auto i : ret) {
		std::cout << i.first.val << " <-> " << i.second.val <<  std::endl;
		}
		std::cout << std::endl;
	ret = graph.fastPrim();
		for (auto i : ret) {
			std::cout << i.first.val << " <-> " << i.second.val << std::endl;
		}
	*/
	/*
	std::vector<PathNode<std::string>> ret = graph.Dijkstra(v_0);
	for (auto i : ret) {
		std::cout << i.from.val << " -> " << i.to.val << " : " << i.value <<  std::endl;
	}
	std::cout << std::endl;
	ret = graph.fastDijkstra(v_0);
	for (auto i : ret) {
		std::cout << i.from.val << " -> " << i.to.val << " : " << i.value << std::endl;
	}*/

	/*
	auto ret = graph.TopoSort();
	for (auto i : ret) {
		std::cout << i.val << " ";
	}
	std::cout << std::endl;

	ret = graph.TopoSort_Another();
	for (auto i : ret) {
		std::cout << i.val << " ";
	}
	std::cout << std::endl;
	*/
}