/**
 * @file tree.hpp
 * @brief Implementation of the dynamic segment tree data structure.
 */

#ifndef TREE_HPP_
#define TREE_HPP_

#include <utility>

namespace dst {

/**
 * @brief The core dynamic segment tree data structure.
 *
 * This class implements a dynamic segment tree data structure. A custom functor can be passed which is used to
 * aggregate the values of the tree. The tree supports the following operations:
 * 
 * - Insertion of a value at a given index.
 * 
 * - Deletion of a value at a given index.
 * 
 * - Querying the aggregate value of a given range.
 *
 * @tparam _tvalue The type of the values stored in the tree indices.
 * @tparam _tindex The type of the indices used in the tree, which can be different from the type of the values but must be integral.
 * @tparam _functor The functor used to aggregate the values of the tree.
 */
template<typename _tvalue, typename _tindex, class _functor>
class tree {
public:
	/**
	 * @brief Constructor for the tree.
	 */
	tree();

	/**
	 * @brief Insert a value at a given index in the tree.
	 * @param index The index to insert the value.
	 * @param value The value to insert.
	 */
	void insert(const _tindex& index, const _tvalue& value);

	/**
	 * @brief Remove an index (with its value) from the tree.
	 * @param index The index to be removed.
	 */
	void erase(const _tindex& index);

	/**
	 * @brief Aggregate the values in the given range for which the indices exist in the tree. The range is inclusive.
	 * @param start The start of the range to query.
	 * @param end The end of the range to query.
	 * @return The aggregate value of the range.
	 */
	_tvalue query(const _tindex& start, const _tindex& end);

	/**
	 * @brief Aggregate the values in the given range for which the indices exist in the tree. The range is inclusive.
	 * @param segment The range to query.
	 * @return The aggregate value of the range.
	 */
	_tvalue query(const std::pair<_tindex, _tindex>& range);

	/**
	 * @brief Access the value at a given index in the tree.
	 * @param index The index to access.
	 * @return The value at the index.
	 */
	_tvalue operator[](const _tindex& index);

	/**
	 * @brief Clear the tree by deleting all the nodes.
	 */
	void clear();

	/**
	 * @brief Destructor for the tree.
	 */
	~tree();	

private:
	/**
	 * @brief The node structure of the tree.
	 * 
	 * This structure defines a node of the dynamic segment tree. Each node contains a range of indices, a value,
	 * and pointers to its parent, left child, and right child.
	 * 
	 */
	class node {
	private:
		_tvalue _value;
		std::pair<_tindex, _tindex> _range;

		node* _parent;
		node* _left;
		node* _right;
	
	public:
		node(const std::pair<_tindex, _tindex>& range, const _tvalue& value, node* p, node* l, node* r)
			: _range(range), _value(value), _parent(p), _left(l), _right(r) {}

		node(const std::pair<_tindex, _tindex>& range, const _tvalue& value)
			: node(range, value, nullptr, nullptr, nullptr) {}
		
		node(const std::pair<_tindex, _tindex>& range)
			: node(range, _tvalue()) {}

		node(const _tindex& index, const _tvalue& value)
			: node(std::make_pair(index, index), value) {}

		node(const _tindex& index)
			: node(std::make_pair(index, index)) {}

		_tvalue& value() { return _value; }
		std::pair<_tindex, _tindex> range() { return _range; }

		node*& parent() { return _parent; }
		node*& left() { return _left; }
		node*& right() { return _right; }
	};

	/**
	 * @brief The root node of the tree.
	 */
	node* _root;

	/**
	 * @brief Aggregation functor used by the tree.
	 */
	_functor _func;

	/**
	 * @brief Internal function to extend the range of a node to include a given index.
	 * 
	 * If the index is outside the current range of the node, this method creates a new node with the extended range.
	 * The range is extended to the nearest power of 2 that includes the index such that the endpoints are consistent
	 * with the rest of the tree.
	 * 
	 * @param cur The current node.
	 * @param index The index to include in the range.
	 * @return The new node with the extended range.
	 */
	node* _extend(node* cur, const _tindex& index);

	/**
	 * @brief Internal function to insert a value at a given index in the tree.
	 * 
	 * This method inserts a value at a given index in the tree. If the index is outside the current range of the node, the range is extended.
	 * 
	 * @param cur The current node.
	 * @param index The index to insert the value.
	 * @param value The value to insert.
	 * @return The root of the tree.
	 */
	node* _insert(node* cur, const _tindex& index, const _tvalue& value);

	/**
	 * @brief Internal function to erase a value at a given index in the tree.
	 * 
	 * This method removes the node at the index (with its value) from the tree. Also removes its direct parent if it has only one child and
	 * connects the other child to the grandparent to maintain O(N) nodes.
	 * 
	 * @param cur The current node.
	 * @param index The index to erase the value.
	 * @return The root of the tree.
	 */
	node* _erase(node* cur, const _tindex& index);

	/**
	 * @brief Internal function to query the aggregate value of a given range in the tree.
	 * 
	 * This method aggregates the values of the indices covered by the given range which exist within the tree.
	 * 
	 * @param cur The current node.
	 * @param segment The range to query.
	 * @return The aggregate value of the range.
	 */
	_tvalue _query(node* cur, const std::pair<_tindex, _tindex>& segment) const;

	/**
	 * @brief Internal function to clear the tree.
	 * 
	 * This method clears the tree by deleting all the nodes.
	 * 
	 * @param cur The current node.
	 */
	void _clear(node* cur);
};

/**
 ************************************** Special member functions **************************************
 */

template<typename _tvalue, typename _tindex, class _functor>
tree<_tvalue, _tindex, _functor>::tree() : _root(nullptr) {}

template<typename _tvalue, typename _tindex, class _functor>
tree<_tvalue, _tindex, _functor>::~tree() {
	clear();
}

/**
 ******************************************* Public methods *******************************************
 */

template<typename _tvalue, typename _tindex, class _functor>
void tree<_tvalue, _tindex, _functor>::insert(const _tindex& index, const _tvalue& value) {
	_insert(_root, index, value);
}

template<typename _tvalue, typename _tindex, class _functor>
void tree<_tvalue, _tindex, _functor>::erase(const _tindex& index) {
	_erase(_root, index);
}

template<typename _tvalue, typename _tindex, class _functor>
_tvalue tree<_tvalue, _tindex, _functor>::query(const _tindex& start, const _tindex& end) {
	return _query(_root, std::make_pair(start, end));
}

template<typename _tvalue, typename _tindex, class _functor>
_tvalue tree<_tvalue, _tindex, _functor>::query(const std::pair<_tindex, _tindex>& range) {
	return _query(_root, range);
}

template<typename _tvalue, typename _tindex, class _functor>
_tvalue tree<_tvalue, _tindex, _functor>::operator[](const _tindex& index) {
	return _query(_root, std::make_pair(index, index));
}

template<typename _tvalue, typename _tindex, class _functor>
void tree<_tvalue, _tindex, _functor>::clear() {
	_clear(_root);
	_root = nullptr;
}

/**
 ******************************************* Private methods ******************************************
 */

template<typename _tvalue, typename _tindex, class _functor>
typename tree<_tvalue, _tindex, _functor>::node*
tree<_tvalue, _tindex, _functor>::_extend(node* cur, const _tindex& index) {

	// Range extension

	std::pair<_tindex, _tindex> range;

	if(cur->parent() == nullptr) { // Very unfortunate, we extend manually
		range = cur->range();

		_tindex dist = (index < range.first) ? range.second - index : index - range.first;
		_tindex resolution = 1;

		while(resolution > dist) resolution /= 2;

		if(index < range.first) { // Left extension
			while(resolution < dist) resolution *= 2;
			range.first = range.second - resolution;

			// That weird single-element interval case
			if(cur->range().first == cur->range().second) range.second += resolution;
		}
		else { // Right extension
			while(resolution <= dist) resolution *= 2;
			range.second = range.first + resolution;
		}
	}
	else { // We could just shrink the parent's range
		range = cur->parent()->range();

		while(true) {
			auto mid = range.first + (range.second - range.first) / 2;
			auto l = cur->range().first, r = cur->range().second;

			if(index < mid) {
				if(l >= mid) break;
				range.second = mid;
			}
			else {
				// That weird single-element interval case again
				if(l == r) {
					if(r < mid) break;
				}
				else if(r <= mid) break;
				range.first = mid;
			}
		}
	}

	// Node creation

	node* par = new node(range);
	
	// Parent assignment
	par->parent() = cur->parent();
	if(cur == _root) _root = par;
	if(par->parent() != nullptr) {
		if(par->parent()->left() == cur) par->parent()->left() = par;
		else par->parent()->right() = par;
	}

	// Children assignment
	if(index < cur->range().first) {
		par->right() = cur;
		par->left() = nullptr;
	}
	else {
		par->left() = cur;
		par->right() = nullptr;
	}

	return par;
}

template<typename _tvalue, typename _tindex, class _functor>
typename tree<_tvalue, _tindex, _functor>::node*
tree<_tvalue, _tindex, _functor>::_insert(node* cur, const _tindex& index, const _tvalue& value) {
	if(cur == nullptr) {
		cur = new node(index, value);
		if(_root == nullptr) _root = cur;
		return cur;
	}

	auto range = cur->range();
	auto mid = range.first + (range.second - range.first) / 2;

	if(range.first == range.second && range.first == index) {
		cur->value() = value;
		return cur;
	}

	if(index < range.first || index >= range.second) {
		node* par = _extend(cur, index);
		cur->parent() = par;
		return _insert(par, index, value);
	}
	
	auto& branch = (index < mid) ? cur->left() : cur->right();
	branch = _insert(branch, index, value);
	branch->parent() = cur;

	cur->value() = _func(cur->left()->value(), cur->right()->value());
	return cur;
}

template<typename _tvalue, typename _tindex, class _functor>
typename tree<_tvalue, _tindex, _functor>::node*
tree<_tvalue, _tindex, _functor>::_erase(node* cur, const _tindex& index) {
	if(cur == nullptr) return nullptr;
	
	auto range = cur->range();
	auto mid = range.first + (range.second - range.first) / 2;

	if(range.first == range.second) {
		if(cur == _root) _root = nullptr;
		delete cur;
		return nullptr;
	}

	if(index < mid) cur->left() = _erase(cur->left(), index);
	else cur->right() = _erase(cur->right(), index);

	if(!cur->left() || !cur->right()) {
		node* child = (cur->left() == nullptr) ? cur->right() : cur->left();
		if(cur == _root) _root = child;
		else child->parent() = cur->parent();

		delete cur;
		return child;
	}

	cur->value() = _func(cur->left()->value(), cur->right()->value());
	return cur;
}

template<typename _tvalue, typename _tindex, class _functor>
_tvalue tree<_tvalue, _tindex, _functor>::_query(node* cur, const std::pair<_tindex, _tindex>& segment) const {
	if(cur == nullptr) return _tvalue();

	auto range = cur->range();
	auto mid = range.first + (range.second - range.first) / 2;

	if(segment.first <= range.first && range.second <= segment.second)
		return cur->value();

	if(segment.first < mid && mid <= segment.second)
		return _func(_query(cur->left(), segment), _query(cur->right(), segment));

	if(segment.second < mid)
		return _query(cur->left(), segment);

	if(mid <= segment.first)
		return _query(cur->right(), segment);

	return _tvalue();
}

template<typename _tvalue, typename _tindex, class _functor>
void tree<_tvalue, _tindex, _functor>::_clear(node* cur) {
	if(cur == nullptr) return;
	_clear(cur->left());
	_clear(cur->right());
	delete cur;
}

}

#endif