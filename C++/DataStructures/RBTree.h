#pragma once

#include <cassert>
#include <string>
#include <iostream>

using namespace std;

enum Color {
	Red,
	Black
};

template<typename K, typename V>
struct TRBNode
{
	TRBNode() : left(this), right(this), parentAndColor((uintptr_t) this | Black) {}
	TRBNode(const K& key, const V& value) : left(const_cast<TRBNode*>(&nil)), right(const_cast<TRBNode*>(&nil)), parentAndColor((uintptr_t)&nil | Red), key(key), value(value) {}
	TRBNode(const TRBNode*, const TRBNode*, const K&, const V&, const Color&);

	K key;
	V value;
	TRBNode* left, *right;

	static const TRBNode nil;

	inline Color getColor() const 
	{
		return (Color) (parentAndColor & ColorMask);
	}

	inline TRBNode* getParent() const 
	{ 
		return (TRBNode*) (parentAndColor & ParentMask);
	}

	inline void setColor(const Color& color) 
	{ 
		parentAndColor = ((parentAndColor & ParentMask) | color);
	}

	inline void setParent(const TRBNode* parent) 
	{ 
		parentAndColor = (parentAndColor & ColorMask) | (uintptr_t) parent;
	}

private:
	uintptr_t parentAndColor;
	static const uintptr_t ColorMask = 0b1;
	static const uintptr_t ParentMask = ~0b1;
};

template<typename K, typename V>
class RBTree
{
public:
	typedef TRBNode<K, V> RBNode;
	RBTree() : root(nil) {}
	RBTree(const RBTree&);
	RBTree(RBTree&&);
	~RBTree();
	RBTree& operator=(const RBTree&);
	RBTree& operator=(RBTree&&);

	// TODO use iterators
	bool insert(const K&, const V&);
	void print(string = "root", const RBNode* = nil);
	bool remove(const K&);
	bool find(const K&, V&) const;
private:
	RBNode* root;

	static RBNode* const nil;

	void fixInsertion(RBNode*);
	void fixRemoval(RBNode*);
	void rotateLeft(RBNode*);
	void rotateRight(RBNode*);
	RBNode* maxNode(const RBNode*) const;
	void deleteSubtree(const RBNode*) const;
	RBNode* copySubtree(const RBNode*) const;
};

template<typename K, typename V>
typename RBTree<K, V>::RBNode* const RBTree<K, V>::nil = const_cast<RBNode*>(&RBNode::nil);

template<typename K, typename V>
RBTree<K, V>::RBTree(const RBTree& rhs)
{
	root = copySubtree(rhs->root);
}

template<typename K, typename V>
RBTree<K, V>::RBTree(RBTree&& rhs)
{
	root = rhs.root;
	rhs.root = nil;
}

template<typename K, typename V>
RBTree<K, V>::~RBTree()
{
	deleteSubtree(root);
}

template<typename K, typename V>
RBTree<K, V>& RBTree<K, V>::operator=(const RBTree& rhs)
{
	if (this != &rhs) 
	{
		deleteSubtree(root);
		root = copySubtree(rhs.root);
	}
	return *this;
}

template<typename K, typename V>
RBTree<K, V>& RBTree<K, V>::operator=(RBTree&& rhs)
{
	if (this != &rhs) 
	{
		deleteSubtree(root);
		root = rhs.root;
		rhs.root = nil;
	}
	return *this;
}

template<typename K, typename V>
bool RBTree<K, V>::insert(const K& key, const V& value)
{
	RBNode** current = &root;
	RBNode* prev = nil;

	while (*current != nil) 
	{
		prev = *current;
		if (key < (*current)->key)
			current = &((*current)->left);
		else if (key > (*current)->key)
			current = &((*current)->right);
		else
			return false;
	}

	*current = new RBNode(key, value);
	(*current)->setParent(prev);
	fixInsertion(*current);
	return true;
}

template<typename K, typename V>
void RBTree<K, V>::print(string prefix, const RBNode* root)
{
	if (prefix == "root")
		root = this->root;

	if (root == nil)
	{
		cout << prefix << ": " << "nill" << endl;
		return;
	}

	print(prefix + ", left", root->left);
	print(prefix + ", right", root->right);
	cout << prefix << ": " << root->key << ", " << root->value << ", " << (root->getColor() == Black ? "Black" : "Red") << endl;
}

template<typename K, typename V>
bool RBTree<K, V>::remove(const K& key)
{

	RBNode* current = root;
	while (current != nil)
	{
		if (key < current->key) {
			current = current->left;
		}
		else if (key > current->key) {
			current = current->right;
		}
		else {
			RBNode* leftMax = maxNode(current->left);
			current->key = leftMax->key;
			current->value = leftMax->value;
			fixRemoval(leftMax);
			return true;
		}
	}
	return false;
}

template<typename K, typename V>
bool RBTree<K, V>::find(const K& key, V& value) const
{
	RBNode* current = root;
	while (current != nil)
	{
		if (key < current->key) {
			current = current->left;
		} else if (key > current->key) {
			current = current->right;
		} else {
			value = current->value;
			return true;
		}
	}
	return false;
}

template<typename K, typename V>
void RBTree<K, V>::fixInsertion(RBNode* root)
{
	// case: N is the root node, i.e., first node of red–black tree
	if (root == this->root) 
	{
		root->setColor(Black);
		return;
	}

	RBNode* parent = root->getParent();

	// case: N's parent (P) is black
	if (parent->getColor() == Black)
		return;

	RBNode* grandparent = parent->getParent();
	RBNode* uncle = parent == grandparent->left ? grandparent->right : grandparent->left;

	// case: N's parent (P) and uncle (U) are red
	if (parent->getColor() == Red && uncle->getColor() == Red)
	{
		parent->setColor(Black);
		uncle->setColor(Black);
		grandparent->setColor(Red);
		fixInsertion(grandparent);
		return;
	}

	// case: N is added to right of left child of grandparent, or N is added to left of right child of grandparent (P is red and U is black)
	if (parent->right == root && grandparent->left == parent) {
		rotateLeft(parent);
		parent = root;
		root = root->left;
	}
	else if (parent->left == root && grandparent->right == parent) {
		rotateRight(parent);
		parent = root;
		root = root->right;
	}

	parent->setColor(Black);
	grandparent->setColor(Red);

	// case: N is added to left of left child of grandparent, or N is added to right of right child of grandparent (P is red and U is black)
	if (parent->right == root && grandparent->right == parent)
		rotateLeft(grandparent);
	else if (parent->left == root && grandparent->left == parent)
		rotateRight(grandparent);
}

// TODO: Write the cases
template<typename K, typename V>
void RBTree<K, V>::fixRemoval(RBNode* root)
{
	RBNode* parent = root->getParent();
	RBNode* child = root->left;

	Color rootColor = root->getColor();
	child->setParent(parent);
	delete root;

	if (parent != nil)
		parent->right = child;

	if (rootColor == Black) {
		if (child->getColor() == Red) {
			child->setColor(Black)
		}
	}

	// case: N is the new root
	if (parent == nil)
		return;

	RBNode* sibling = parent->left;

	if (sibling.getColor() == Red)
}

template<typename K, typename V>
void RBTree<K, V>::rotateLeft(RBNode* pivot)
{
	if (root == pivot)
		root = pivot->right;

	RBNode* pivotParent = pivot->getParent();
	RBNode* pivotRight = pivot->right;

	pivotRight->setParent(pivotParent);
	pivot->setParent(pivotRight);

	pivot->right = pivotRight->left;
	pivotRight->left = pivot;


	if (pivotParent != nil) {
		if (pivotParent->left == pivot)
			pivotParent->left = pivotRight;
		else
			pivotParent->right = pivotRight;
	}

	if (pivot->right != nil) {
		pivot->right->setParent(pivot);
	}
}

template<typename K, typename V>
void RBTree<K, V>::rotateRight(RBNode* pivot)
{
	if (root == pivot)
		root = pivot->left;

	RBNode* pivotParent = pivot->getParent();
	RBNode* pivotLeft = pivot->left;

	pivotLeft->setParent(pivotParent);
	pivot->setParent(pivotLeft);

	pivot->left = pivotLeft->right;
	pivotLeft->right = pivot;

	if (pivotParent != nil) {
		if (pivotParent->left == pivot)
			pivotParent->left = pivotLeft;
		else
			pivotParent->right = pivotLeft;
	}

	if (pivot->left != nil) {
		pivot->left->setParent(pivot);
	}
}

template<typename K, typename V>
typename RBTree<K, V>::RBNode* RBTree<K, V>::maxNode(const RBNode* root) const
{
	while (root->right != nil)
		root = root->right;
	return root;
}

template<typename K, typename V>
void RBTree<K, V>::deleteSubtree(const RBNode* root) const
{
	if (root == nil)
		return;

	deleteSubtree(root->left);
	deleteSubtree(root->right);
	delete root;
}

template<typename K, typename V>
typename RBTree<K, V>::RBNode* RBTree<K, V>::copySubtree(const RBNode* root) const
{
	if (root == nil)
		return nil;

	RBNode* left = deleteSubtree(root->left);
	RBNode* right = deleteSubtree(root->right);
	return new RBNode(left, right, root->key, root->value, root.getColor());
}

template<typename K, typename V>
const TRBNode<K, V> TRBNode<K, V>::nil;

template<typename K, typename V>
TRBNode<K, V>::TRBNode(const TRBNode* left, const TRBNode* right, const K& key, const V& value, const Color& color) :
	left(left), 
	right(right), 
	parentAndColor((uintptr_t) &nil | color), 
	key(key), 
	value(value) 
{
	left->setParent(this);
	right->setParent(this);
}