#ifndef AGGREGATE_SET_HPP_
#define AGGREGATE_SET_HPP_

#include <limits>
#include "tree.hpp"

namespace dst {

struct none_hash {
	template<typename _type>
	std::size_t operator()(_type value) const {
		return 0;
	}
};

template<typename _type, class _function, class _hash = std::hash<_type>>
class aggregate_set {
private:
	using _index_type = decltype(_hash()(_type()));
	tree<_type, _index_type, _function> _tree;

public:
	aggregate_set() {}

	void insert(_type value) {
		_tree.insert(_hash()(value), value);
	}

	void erase(_type value) {
		_tree.erase(_hash()(value));
	}

	_type all() {
		return _tree.query(std::make_pair(std::numeric_limits<_index_type>::min(), std::numeric_limits<_index_type>::max()));
	}
};

template<typename _type, class _function>
class aggregate_set<_type, _function, typename std::enable_if<std::is_integral<_type>::value>::type> {
private:
	tree<_type, _type, _function> _tree;

public:
	aggregate_set() {}

	void insert(_type value) {
		_tree.insert(value, value);
	}

	void erase(_type value) {
		_tree.erase(value);
	}

	_type all() {
		return _tree.query(std::make_pair(std::numeric_limits<_type>::min(), std::numeric_limits<_type>::max()));
	}
};

}

#endif