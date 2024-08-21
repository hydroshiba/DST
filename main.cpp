#include <iostream>
#include <functional>
#include <utility>
#include <cstdlib>
#include <set>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

#include "tree.hpp"
#include "aggregate_set.hpp"

int main() {
	dst::tree<float, float, std::plus<float>> tree;
	std::vector<float> list(10);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(1.0, 10.0);

    for(auto& i : list) i = dis(gen);
	for(float i : list) std::cout << i << " ";
	std::cout << std::endl;

	std::cout << std::fixed;
	std::cout.precision(2);

	for(float i : list) {
		std::cout << "Insert " << i << std::endl;
		tree.insert(i, i);
	}

	tree.traverse();
	std::cout << std::endl;

	// for(int i = -5; i <= 5; ++i) {
	// 	std::cout << "Erase " << i << std::endl;
	// 	tree.erase(i);
	// 	tree.traverse();
	// 	std::cout << std::endl;
	// }

	// std::cout << std::endl;

	// std::sort(list.begin(), list.end());
	// for(int i : list) std::cout << i << " ";
	// std::cout << std::endl;

	// for(int i = 1; i <= 10; ++i) {
	// 	std::cout << "Query " << i << " " << tree.query(std::make_pair(1, i)) << std::endl;
	// }

	// dst::aggregate_set<int, std::plus<int>> set;
	// for(int i = -5; i <= 5; ++i) {
	// 	std::cout << "Insert " << i << std::endl;
	// 	set.insert(i);
	// }

	// std::cout << "All: " << set.all() << std::endl;
}