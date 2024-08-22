/**
 * @file tree.hpp
 * @brief Implementation of the dynamic segment tree data structure.
 */

#ifndef TREE_HPP_
#define TREE_HPP_

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
 * @tparam _type The type of the values stored in the tree indices.
 * @tparam _index_type The type of the indices used in the tree, which can be different from the type of the values but must be integral.
 * @tparam _functor The functor used to aggregate the values of the tree.
 */
template<typename _type, typename _index_type, class _functor>
class tree {
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
		_type _value;
		std::pair<_index_type, _index_type> _range;

		node* _parent;
		node* _left;
		node* _right;
	
	public:
		node(std::pair<_index_type, _index_type> range, _type value, node* p, node* l, node* r)
			: _range(range), _value(value), _parent(p), _left(l), _right(r) {}
		node(std::pair<_index_type, _index_type> range, _type value)
			: node(range, value, nullptr, nullptr, nullptr) {}
		node(std::pair<_index_type, _index_type> range)
			: node(range, _type()) {}

		node(_index_type index, _type value) : node(std::make_pair(index, index), value) {}
		node(_index_type index) : node(std::make_pair(index, index)) {}

		_type& value() { return _value; }
		std::pair<_index_type, _index_type> range() { return _range; }

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
	node* _extend(node* cur, _index_type index);

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
	node* _insert(node* cur, _index_type index, _type value);

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
	node* _erase(node* cur, _index_type index);

	/**
	 * @brief Internal function to query the aggregate value of a given range in the tree.
	 * 
	 * This method aggregates the values of the indices covered by the given range which exist within the tree.
	 * 
	 * @param cur The current node.
	 * @param segment The range to query.
	 * @return The aggregate value of the range.
	 */
	_type _query(node* cur, std::pair<_index_type, _index_type> segment);

	/**
	 * @brief Internal function to clear the tree.
	 * 
	 * This method clears the tree by deleting all the nodes.
	 * 
	 * @param cur The current node.
	 */
	void _clear(node* cur);

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
	void insert(_index_type index, _type value);

	/**
	 * @brief Remove an index (with its value) from the tree.
	 * @param index The index to be removed.
	 */
	void erase(_index_type index);

	/**
	 * @brief Aggregate the values in the given range for which the indices exist in the tree. The range is inclusive.
	 * @param start The start of the range to query.
	 * @param end The end of the range to query.
	 * @return The aggregate value of the range.
	 */
	_type query(_index_type start, _index_type end);

	/**
	 * @brief Aggregate the values in the given range for which the indices exist in the tree. The range is inclusive.
	 * @param segment The range to query.
	 * @return The aggregate value of the range.
	 */
	_type query(std::pair<_index_type, _index_type> segment);

	/**
	 * @brief Clear the tree by deleting all the nodes.
	 */
	void clear();

	/**
	 * @brief Destructor for the tree.
	 */
	~tree();
};

/**
 ************************************** Special member functions **************************************
 */

// Constructor
template<typename _type, typename _index_type, class _functor>
tree<_type, _index_type, _functor>::tree() : _root(nullptr) {}

// Destructor
template<typename _type, typename _index_type, class _functor>
tree<_type, _index_type, _functor>::~tree() {
	clear();
}

/**
 ******************************************* Public methods *******************************************
 */

// Insert
template<typename _type, typename _index_type, class _functor>
void tree<_type, _index_type, _functor>::insert(_index_type index, _type value) {
	_insert(_root, index, value);
}

// Erase
template<typename _type, typename _index_type, class _functor>
void tree<_type, _index_type, _functor>::erase(_index_type index) {
	_erase(_root, index);
}

// Endpoints query
template<typename _type, typename _index_type, class _functor>
_type tree<_type, _index_type, _functor>::query(_index_type start, _index_type end) {
	return _query(_root, std::make_pair(start, end));
}

// Pair query
template<typename _type, typename _index_type, class _functor>
_type tree<_type, _index_type, _functor>::query(std::pair<_index_type, _index_type> range) {
	return _query(_root, range);
}

// Clear
template<typename _type, typename _index_type, class _functor>
void tree<_type, _index_type, _functor>::clear() {
	_clear(_root);
	_root = nullptr;
}

/**
 ******************************************* Private methods ******************************************
 */

// Internal extend
template<typename _type, typename _index_type, class _functor>
typename tree<_type, _index_type, _functor>::node*
tree<_type, _index_type, _functor>::_extend(node* cur, _index_type index) {

	// Range extension

	std::pair<_index_type, _index_type> range;

	if(cur->parent() == nullptr) { // Very unfortunate, we extend manually
		range = cur->range();

		_index_type dist = (index < range.first) ? range.second - index : index - range.first;
		_index_type resolution = 1;

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

// Internal insert
template<typename _type, typename _index_type, class _functor>
typename tree<_type, _index_type, _functor>::node*
tree<_type, _index_type, _functor>::_insert(node* cur, _index_type index, _type value) {
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

// Internal erase
template<typename _type, typename _index_type, class _functor>
typename tree<_type, _index_type, _functor>::node*
tree<_type, _index_type, _functor>::_erase(node* cur, _index_type index) {
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

// Internal query
template<typename _type, typename _index_type, class _functor>
_type tree<_type, _index_type, _functor>::_query(node* cur, std::pair<_index_type, _index_type> segment) {
	if(cur == nullptr) return _type();

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

	return _type();
}

// Internal clear
template<typename _type, typename _index_type, class _functor>
void tree<_type, _index_type, _functor>::_clear(node* cur) {
	if(cur == nullptr) return;
	_clear(cur->left());
	_clear(cur->right());
	delete cur;
}

}

#endif