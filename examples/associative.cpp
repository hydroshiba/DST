/**
 * @file associative.cpp
 * @brief Example of indexed associative use. Tested on https://judge.yosupo.jp/problem/associative_array
 */

#include <iostream>
#include "dst.hpp"

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int queries;
	std::cin >> queries;

	dst::tree<long long, long long> tree;

	while(queries--) {
		int type;
		std::cin >> type;

		if(type) {
			long long index;
			std::cin >> index;
			std::cout << tree[index] << std::endl;
		}
		else {
			long long index, value;
			std::cin >> index >> value;
			tree.insert(index, value);
		}
	}
}