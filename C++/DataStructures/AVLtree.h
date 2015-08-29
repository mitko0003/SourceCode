#ifndef AVLTREE_H
#define AVLTREE_H

#include <cassert>
			
template <typename K, typename V>
class AVLtree
{
public:
	AVLtree() : root(nullptr) {}	 
	AVLtree(const AVLtree&);		 
	~AVLtree();						 
	AVLtree& operator=(const AVLtree&); 

	bool insert(const K&, const V&);	
	bool remove(const K&);
	bool find(const K&, V&);		

private:
	struct Node
	{
		Node(const K& key, const V& value, const int& balance = 0) : key(key), value(value), balance(balance), left(nullptr), right(nullptr), parent(nullptr)	{}
		K key;
		V value;
		int balance;
		Node* left, *right, *parent;
	};
	Node* root;

	void delSubTree(Node*);					 
	void copySubTree(const Node*, Node*&);	 

	void rotateLeft(Node*);
	void rotateRight(Node*);

	void retraceInsert(Node*);
	void retraceRemove(Node*);

	inline void fixLeftParent(Node* parent) { if (parent->left) { parent->left->parent = parent; } }
	inline void fixRightParent(Node* parent) { if (parent->right) { parent->right->parent = parent; } }
	void fixParentChild(Node*, Node*, const int& = 0);
	void fixBalance(Node*, Node*, const int&);

	Node** maxNodeInSub(Node**);
	Node** findNode(const K&);
};

template <typename K, typename V>
AVLtree<K, V>::~AVLtree()
{
	this->delSubTree(root);
}

template <typename K, typename V>
AVLtree<K, V>::AVLtree(const AVLtree<K, V>& other) : root(nullptr)
{
	this->copySubTree(other.root, root);
}

template <typename K, typename V>
AVLtree<K, V>& AVLtree<K, V>::operator=(const AVLtree<K, V>& other)
{
	if (this != &other)
	{
		this->delSubTree(root);
		root = nullptr;
		this->copySubTree(other.root, root);
	}
	return *this;
}

template <typename K, typename V>
bool AVLtree<K, V>::insert(const K& key, const V& value)
{
	Node * newNode = new Node(key, value), **subRoot = &root, *prevRoot = nullptr;
	assert(newNode);

	do {
		if (!*subRoot){
			newNode->parent = prevRoot;
			*subRoot = newNode;
			this->retraceInsert(newNode);
			return true;
		}
		else if (key < (*subRoot)->key) {
			prevRoot = *subRoot;
			subRoot = &((*subRoot)->left);
		}
		else if (key > (*subRoot)->key) {
			prevRoot = *subRoot;
			subRoot = &((*subRoot)->right);
		}
		else
			return false;
	} while (true);

}

template <typename K, typename V>
void AVLtree<K, V>::retraceInsert(Node* leaf)
{
	while (leaf->parent) {
		if (leaf == leaf->parent->left) {
			if (leaf->parent->balance == 1) {
				leaf->parent->balance = 2;
				if (leaf->balance == -1) {
					this->rotateLeft(leaf);
					leaf = leaf->parent;
				}
				this->rotateRight(leaf->parent);
				break;
			}
			if (leaf->parent->balance == -1) {
				leaf->parent->balance = 0;
				break;
			}
			leaf->parent->balance = 1;
		}
		else {
			if (leaf->parent->balance == -1) {
				leaf->parent->balance = -2;
				if (leaf->balance == 1) {
					this->rotateRight(leaf);
					leaf = leaf->parent;
				}
				this->rotateLeft(leaf->parent);
				break;
			}
			if (leaf->parent->balance == 1) {
				leaf->parent->balance = 0;
				break;
			}
			leaf->parent->balance = -1;
		}
		leaf = leaf->parent;
	}
}

template <typename K, typename V>
typename AVLtree<K, V>::Node** AVLtree<K, V>::maxNodeInSub(Node** root)
{
	while ((*root)->right){
		root = &(*root)->right;
	}
	return root;
}

template <typename K, typename V>
bool AVLtree<K, V>::remove(const K& key)
{
	Node** curr = this->findNode(key);

	if (!curr) return false;

	if ((*curr)->left && (*curr)->right) {
		Node ** succ = this->maxNodeInSub(&(*curr)->left);
		std::swap((*succ)->key, (*curr)->key);
		std::swap((*succ)->value, (*curr)->value);
		curr = succ;
	}

	Node* toRemove = *curr;

	if ((*curr)->left) {
		*curr = (*curr)->left;
		(*curr)->parent = (*curr)->parent->parent;
		this->retraceRemove(*curr);
	}
	else if ((*curr)->right) {
		*curr = (*curr)->right;
		(*curr)->parent = (*curr)->parent->parent;
		this->retraceRemove(*curr);
	}
	else {
		this->retraceRemove(toRemove);
		*curr = nullptr;
	}

	delete toRemove;
	return true;
}

template <typename K, typename V>
void AVLtree<K, V>::retraceRemove(Node* curr)
{
	while (curr->parent) {
		if (curr == curr->parent->right) {
			if (curr->parent->balance == 1) {
				if (curr->parent->left->balance == -1) {
					this->rotateLeft(curr->parent->left);
				}
				this->rotateRight(curr->parent);
				if (curr->parent->parent->balance == 0) break;
			}
			if (curr->parent->balance == 0) {
				curr->parent->balance = 1;
				break;
			}
			curr->parent->balance = 0;
		}
		else {
			if (curr->parent->balance == -1) {
				if (curr->parent->right->balance == 1) {
					this->rotateRight(curr->parent->right);
				}
				this->rotateLeft(curr->parent);
				if (curr->parent->parent->balance == 0) break;
			}
			if (curr->parent->balance == 0) {
				curr->parent->balance = -1;
				break;
			}	   
			curr->parent->balance = 0;
		}
		curr = curr->parent;
	}
}

template <typename K, typename V>
bool AVLtree<K, V>::find(const K& key, V& foundValue)
{
	Node** searched = findNode(key);
	if (searched) {
		foundValue = (*searched)->value;
		return true;
	}
	return false;
}

template <typename K, typename V>
typename AVLtree<K, V>::Node** AVLtree<K, V>::findNode(const K& key)
{
	Node ** currentNode = &root;

	while (*currentNode){
		if ((*currentNode)->key == key){
			return currentNode;
		}
		else if (key < (*currentNode)->key){
			currentNode = &(*currentNode)->left;
		}
		else {
			currentNode = &(*currentNode)->right;
		}
	}
	return nullptr;
}

template <typename K, typename V>
void AVLtree<K, V>::rotateLeft(Node* root)
{
	Node* pivot = root->right;
	root->right = pivot->left;
	pivot->left = root;

	pivot->parent = root->parent;
	root->parent = pivot;

	if (!pivot->parent) this->root = pivot;
	else fixParentChild(root, pivot);

	this->fixRightParent(root);
									
	this->fixBalance(root, pivot, -1);
}

template <typename K, typename V>
void AVLtree<K, V>::rotateRight(Node* root)
{
	Node* pivot = root->left;
	root->left = pivot->right;
	pivot->right = root;

	pivot->parent = root->parent;
	root->parent = pivot;

	if (!pivot->parent) this->root = pivot;
	else fixParentChild(root, pivot);

	this->fixLeftParent(root);

	this->fixBalance(root, pivot, 1);
}

template <typename K, typename V>
void AVLtree<K, V>::fixBalance(Node* root, Node* pivot, const int& direction)
{
	root->balance -= 1 * direction;
	if (pivot->balance * direction > 0)
		root->balance -= pivot->balance;
	pivot->balance -= 1 * direction;
	if (root->balance * direction < 0)
		pivot->balance += root->balance;
}

template <typename K, typename V>
void AVLtree<K, V>::fixParentChild(Node* root, Node* pivot, const int& balance)
{
	if (!pivot->parent)
		return;

	if (pivot->parent->left == root) {
		pivot->parent->left = pivot;
		pivot->parent->balance += balance;
	}
	else{
		pivot->parent->right = pivot;
		pivot->parent->balance -= balance;
	}
}

template <typename K, typename V>
void AVLtree<K, V>::copySubTree(const Node* rootFrom, Node*& rootTo)
{
	if (rootFrom){
		rootTo = new Node(rootFrom->key, rootFrom->value, rootFrom->balance);
		this->copySubTree(rootFrom->left, rootTo->left);
		this->fixLeftParent(rootTo);
		this->copySubTree(rootFrom->right, rootTo->right);
		this->fixRightParent(rootTo);
	}
}

template <typename K, typename V>
void AVLtree<K, V>::delSubTree(Node* root)
{
	if (!root) return;
	if (root->left)
		this->delSubTree(root->left);
	if (root->right)
		this->delSubTree(root->right);
	delete root;
}

#endif
