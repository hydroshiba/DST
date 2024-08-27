#ifndef DST_AGGREGATE_SET_HPP_
#define DST_AGGREGATE_SET_HPP_

#include <limits>
#include <functional>

#include "tree.hpp"

namespace dst {

/**
 * @brief The aggregate set, which is a wrapper structure around the dynamic segment tree.
 * 
 * This class is a wrapper of the dynamic segment tree, which provides a set-like interface for the tree. The set supports
 * the following operations:
 * 
 * - Insertion of a value.
 * 
 * - Deletion of a value.
 * 
 * - Aggregation of all the values in the set.
 * 
 * @tparam _tvalue The type of the values stored in the set. If the type is non-integral a default hashing funtion is used instead.
 * @tparam _functor The functor used to aggregate the values of the set. For non-integral type since hashing breaks the value
 * ordering, if the functor operation is non-commutative the result may be unexpected.
 * @tparam _hash The hash function used to hash the values of the set. Required if the value type does not have a default hashing function.
 */
template<typename _tvalue, class _functor = std::plus<_tvalue>, class _hash = std::hash<_tvalue>>
class aggregate_set {
private:
	using _tindex = decltype(_hash()(_tvalue()));

	/**
	 * @brief The internal dynamic segment tree used by the set.
	 */
	tree<_tvalue, _tindex, _functor> _tree;

public:
	/**
	 * @brief Constructor for the aggregate set.
	 */
	aggregate_set() {}

	/**
	 * @brief Insert a value into the set.
	 * @param value The value to insert.
	 */
	void insert(_tvalue value) {
		_tree.insert(_hash()(value), value);
	}

	/**
	 * @brief Remove a value from the set.
	 * @param value The value to remove.
	 */
	void erase(_tvalue value) {
		_tree.erase(_hash()(value));
	}

	/**
	 * @brief Aggregate the whole set.
	 * @return The aggregate value of all the values of the set.
	 */
	_tvalue all() {
		return _tree.query(std::make_pair(std::numeric_limits<_tindex>::min(), std::numeric_limits<_tindex>::max()));
	}
};

template<typename _tvalue, class _functor>
class aggregate_set<_tvalue, _functor, typename std::enable_if<std::is_integral<_tvalue>::value>::type> {
private:
	tree<_tvalue, _tvalue, _functor> _tree;

public:
	aggregate_set() {}

	void insert(_tvalue value) {
		_tree.insert(value, value);
	}

	void erase(_tvalue value) {
		_tree.erase(value);
	}

	_tvalue all() {
		return _tree.query(std::make_pair(std::numeric_limits<_tvalue>::min(), std::numeric_limits<_tvalue>::max()));
	}
};

}

#endif