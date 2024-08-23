/**
 * @file salary.cpp
 * @brief Solves the problem Salary Queries (https://cses.fi/problemset/task/1144/) on CSES, but TLE due to
 * strict time limits and the data structure is not (yet) rigorously optimized.
 */

#include <iostream>
#include <vector>
#include "dst.hpp"

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, query;
	std::cin >> size >> query;

	std::vector<int> arr(size);
	dst::tree<int, int> tree;

	for(int& value : arr) {
		std::cin >> value;
		tree.insert(value, tree[value] + 1);
	}

	while(query--) {
		char type;
		std::cin >> type;

		if(type == '?') {
			int start, end;
			std::cin >> start >> end;
			std::cout << tree.query(start, end) << '\n';
		}
		else {
			int employee, salary;
			std::cin >> employee >> salary;
			--employee;

			int old_salary = arr[employee];
			arr[employee] = salary;

			tree.apply(old_salary, -1);
			tree.apply(salary, 1);
		}
	}
}