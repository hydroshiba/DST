#ifndef TREE_HPP_
#define TREE_HPP_

#include <iostream>
#include <cstdint>

namespace dst {

template<typename _type, typename _index_type, class _function>
class tree {
private:
	class node {
	private:
		_type _value;
		std::pair<_index_type, _index_type> _range;

		node* _parent;
		node* _left;
		node* _right;
	
	public:
		node(std::pair<_index_type, _index_type> range, _type value, node* p, node* l, node* r) : _range(range), _value(value), _parent(p), _left(l), _right(r) {}
		node(std::pair<_index_type, _index_type> range, _type value) : node(range, value, nullptr, nullptr, nullptr) {}
		node(std::pair<_index_type, _index_type> range) : node(range, _type()) {}

		node(_index_type index, _type value) : node(std::make_pair(index, index), value) {}
		node(_index_type index) : node(std::make_pair(index, index)) {}

		_type& value() { return _value; }
		std::pair<_index_type, _index_type> range() { return _range; }

		node*& parent() { return _parent; }
		node*& left() { return _left; }
		node*& right() { return _right; }
	};

	node* _root;
	_function _func;

	void _traverse(node* cur);
	node* _extend(node* cur, _index_type index);

	node* _insert(node* cur, _index_type index, _type value);
	node* _erase(node* cur, _index_type index);
	_type _query(node* cur, std::pair<_index_type, _index_type> range);

public:
	tree();
	void traverse() { _traverse(_root); }

	void insert(_index_type index, _type value);
	void erase(_index_type index);
	_type query(std::pair<_index_type, _index_type> range);

	// ~tree();
};

// Public methods

template<typename _type, typename _index_type, class _function>
tree<_type, _index_type, _function>::tree() : _root(nullptr) {}

template<typename _type, typename _index_type, class _function>
void tree<_type, _index_type, _function>::insert(_index_type index, _type value) {
	_insert(_root, index, value);
}

template<typename _type, typename _index_type, class _function>
void tree<_type, _index_type, _function>::erase(_index_type index) {
	_erase(_root, index);
}

template<typename _type, typename _index_type, class _function>
_type tree<_type, _index_type, _function>::query(std::pair<_index_type, _index_type> range) {
	return _query(_root, range);
}

// Private methods

template<typename _type, typename _index_type, class _function>
void tree<_type, _index_type, _function>::_traverse(node* cur) {
	if(cur == nullptr) return;

	std::cout << '[' << cur->range().first << ',' << cur->range().second << ')';
	std::cout << ' ';

	if(cur->left()) std::cout << '[' << cur->left()->range().first << ',' << cur->left()->range().second << ')' << ' ';
	if(cur->right()) std::cout << '[' << cur->right()->range().first << ',' << cur->right()->range().second << ')' << ' ';

	std::cout << std::endl;

	_traverse(cur->left());
	_traverse(cur->right());
}

template<typename _type, typename _index_type, class _function>
typename tree<_type, _index_type, _function>::node*
tree<_type, _index_type, _function>::_extend(node* cur, _index_type index) {
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

			// That weird case where you consider a single-element interval
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
			auto mid = (range.first + range.second) / 2;

			if(index < mid && cur->range().first >= mid) break;
			if(index >= mid && cur->range().second < mid) break;

			if(index < mid) range.second = mid;
			else range.first = mid;
		}
	}

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

template<typename _type, typename _index_type, class _function>
typename tree<_type, _index_type, _function>::node*
tree<_type, _index_type, _function>::_insert(node* cur, _index_type index, _type value) {
	if(cur == nullptr) {
		cur = new node(index, value);
		if(_root == nullptr) _root = cur;
		return cur;
	}

	auto range = cur->range();

	if(index < range.first || index >= range.second) {
		node* par = _extend(cur, index);
		cur->parent() = par;
		return _insert(par, index, value);
	}
	else {
		auto mid = (range.first + range.second) / 2;
		if(index < mid) {
			cur->left() = _insert(cur->left(), index, value);
			cur->left()->parent() = cur;
		}
		else {
			cur->right() = _insert(cur->right(), index, value);
			cur->right()->parent() = cur;
		}
	}

	cur->value() = _func(cur->left()->value(), cur->right()->value());
	return cur;
}

template<typename _type, typename _index_type, class _function>
typename tree<_type, _index_type, _function>::node*
tree<_type, _index_type, _function>::_erase(node* cur, _index_type index) {
	if(cur == nullptr) return nullptr;
	auto range = cur->range();

	if(range.first == range.second) {
		if(cur == _root) _root = nullptr;
		delete cur;
		return nullptr;
	}

	auto mid = (range.first + range.second) / 2;
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

template<typename _type, typename _index_type, class _function>
_type tree<_type, _index_type, _function>::_query(node* cur, std::pair<_index_type, _index_type> range) {
	auto [l, r] = range;
	auto [cl, cr] = cur->range();
	if(l <= cl && cr <= r) return cur->value();
	
	auto mid = (cl + cr) >> 1;

	if(r <= mid && l > mid) return _func(_query(cur->left(), range), _query(cur->right(), range));
	if(r <= mid) return _query(cur->left(), range);
	if(l > mid) return _query(cur->right(), range);

	return _type();
}

}

#endif