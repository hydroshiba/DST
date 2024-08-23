/**
 * @file purs.cpp
 * @brief Example of Point Update Range Sum use. Tested on https://judge.yosupo.jp/problem/point_add_range_sum
 */

#include <iostream>
#include "dst.hpp"

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, query;
	std::cin >> size >> query;

	dst::tree<long long, int> tree;

	for(int i = 0; i < size; ++i) {
		long long value;
		std::cin >> value;
		tree.insert(i, value);
	}

	while(query--) {
		int type;
		std::cin >> type;

		if(type) {
			int start, end;
			std::cin >> start >> end;
			std::cout << tree.query(start, end - 1) << '\n';
		}
		else {
			int index;
			long long value;
			std::cin >> index >> value;
			tree.insert(index, tree[index] + value);
		}
	}
}