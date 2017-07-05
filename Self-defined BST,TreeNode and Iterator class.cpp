/**	@file Self-defined BST,TreeNode and Iterator class.cpp
	@author Bowen Xuan
	@date May 24th 

	This program contains BST, TreeNode and Iterator class. Every TreeNode contains an int type data
	and with private variable TreeNode* left and right; BST class inludes public member functions
	to insert,erase TreeNodes. The Iterator class has all the operator overloaded that
	can be used to iterator through BST.	
*/

#include <iostream>    
#include <iomanip>      
#include <string>

class TreeNode {
	friend class BinarySearchTree;
	friend class TreeIterator;
public:
	TreeNode(int input);
	~TreeNode();
	void insert_node(TreeNode* new_node);
	int operator*();
	void print_nodes() const;
	bool find(int data) const;
private:
	int data;
	TreeNode* parent;
	TreeNode* left;
	TreeNode* right;
};
class BinarySearchTree {
	friend class TreeIterator;
public:
	BinarySearchTree();
	~BinarySearchTree();
	TreeIterator begin();
	TreeIterator end();
	void insert(int data);
	void erase(int data);
	int smallest();
	int largest();
	void print() const;
private:
	TreeNode* root;
	TreeNode* end_node;

};

class TreeIterator {
	friend class BinarySearchTree;
public:
	TreeIterator(TreeNode* pos, BinarySearchTree* con);
	void operator++();
	int operator*();
	bool operator==(TreeIterator& rhs) const;
	bool operator!=(TreeIterator& rhs) const;

private:
	TreeNode* position;
	BinarySearchTree* container;
};

TreeNode::TreeNode(int input) {
	parent = left = right = NULL;
	data = input;
}
TreeNode::~TreeNode() {
	delete left;
	delete right;
}
BinarySearchTree::BinarySearchTree() {
	root = NULL;
	end_node = new TreeNode(0);
}
BinarySearchTree::~BinarySearchTree() {
	//for (TreeIterator temp = begin(); temp != end();) {
	//	TreeIterator a = temp;
	//	++temp;
	//	delete a.position;
	//	std::cout << "deleted" << std::endl;
	//}
	delete root;
	delete end_node;
}
TreeIterator::TreeIterator(TreeNode* pos, BinarySearchTree* con) {
	position = pos;
	container = con;
}

int TreeNode::operator*() {
	return data;
}
void BinarySearchTree::insert(int data)
{
	TreeNode* new_node = new TreeNode(data);
	if (root == NULL) {
		root = new_node;
		root->parent = end_node;
	}
	else root->insert_node(new_node);
}
void TreeNode::insert_node(TreeNode* new_node) {
	if (new_node->data < data) {
		if (left == NULL) {
			left = new_node;
			left->parent = this;
		}
		else left->insert_node(new_node);
	}
	else if (data < new_node->data) {
		if (right == NULL) {
			right = new_node;
			right->parent = this;
		}
		else right->insert_node(new_node);
	}
	else
		delete new_node;
}
void BinarySearchTree::erase(int data) {
	//Find the node to be removed
	TreeNode* to_be_removed = root;
	TreeNode* parent_node = NULL;
	bool found = false;
	while (!found&&to_be_removed != NULL) {
		if (data > to_be_removed->data) {
			parent_node = to_be_removed;
			to_be_removed = to_be_removed->right;
		}
		else if (data < to_be_removed->data) {
			parent_node = to_be_removed;
			to_be_removed = to_be_removed->left;
		}
		else found = true;
	}	//if found at root, to_be_removed==root,parent_node==NULL
	if (!found)	return;
	std::cout << "here" << std::endl;

	//if one of the children is empty, use the other
	if (to_be_removed->left == NULL || to_be_removed->right == NULL) {
		TreeNode* new_child;

		if (to_be_removed->left == NULL)	//A
			new_child = to_be_removed->right; //NULL or not
		else                                //B
			new_child = to_be_removed->left; //NULL or not
											 //std::cout << "new_child: " << **(new_child) << std::endl;
		if (parent_node == NULL) {//found at root
								  //to_be_removed = root;
			root->left = NULL;
			root->right = NULL;
			delete root;
			root = new_child; //update root
			if (new_child)
				new_child->parent = end_node;
			//std::cout << "after deleting " << std::endl;
		}
		else if (parent_node->left == to_be_removed) {//1
			parent_node->left->left = NULL;
			parent_node->left->right = NULL;
			delete parent_node->left;
			parent_node->left = new_child; //update
			if (new_child)
				new_child->parent = parent_node;
		}
		else {                             //2
			parent_node->right->left = NULL;
			parent_node->right->right = NULL;
			delete parent_node->right;
			parent_node->right = new_child;//update
			if (new_child)
				new_child->parent = parent_node;
		}
		return;
	}

	//Neither subtree is empty
	//find largest element of the left subtree
	TreeNode* largest_parent = to_be_removed;
	TreeNode* largest = to_be_removed->left;
	std::cout << "here2" << std::endl;
	while (largest->right != NULL) { //keep going to the rightest
		largest_parent = largest;
		largest = largest->right;
	}
	to_be_removed->data = largest->data; //copy data
	if (largest_parent == to_be_removed) {	//if to_be_removed is the parent
		largest_parent->left = largest->left;
		if (largest_parent->left)
			(largest_parent->left)->parent = largest_parent;
	}

	else {
		largest_parent->right = largest->left;
		if (largest_parent->left)
			(largest_parent->right)->parent = largest_parent;
	}
	largest->left = NULL;
	largest->right = NULL;
	delete largest;
	std::cout << "here3" << std::endl;
}
bool TreeNode::find(int input) const {
	if (data < input) {
		if (left == NULL)	return false;
		else return left->find(data);
	}
	else if (data > input) {
		if (right == NULL)	return false;
		else return right->find(input);
	}
	else
		return true;
}

TreeIterator BinarySearchTree::begin() {
	//TreeIterator temp(root,this);
	TreeNode* temp = root;
	while (temp->left)
		temp = temp->left;
	TreeIterator temp_iter(temp, this);
	return temp_iter;
}
TreeIterator BinarySearchTree::end() {
	TreeIterator temp_iter(end_node, this);
	return temp_iter;
}

int TreeIterator::operator*() {
	return position->data;
}
bool TreeIterator::operator==(TreeIterator& rhs) const {
	return (position == rhs.position);
}
bool TreeIterator::operator!=(TreeIterator& rhs) const {
	return !(position == rhs.position);
}
void TreeIterator::operator++() {
	if (position->right != NULL) {
		position = position->right;
		while (position->left != NULL) //go to smallest child in right branch
			position = position->left;
	}
	else {
		TreeNode* temp = position->parent;
		while (position == temp->right && temp != container->end_node) {//go all the way up until not right branch
			position = temp;
			temp = position->parent;
			//if (!temp) break;//break when position is root?
		}
		//if (position != temp) //end at root? empty tree? end()?
		//if(temp)
		position = temp;
	}
}

//bool operator!=(TreeIterator& a, TreeIterator& b) {
//	return !(a == b);
//}

int main() {
	BinarySearchTree bst;
	bst.insert(3); bst.insert(2); bst.insert(1);
	bst.insert(14); bst.insert(6); bst.insert(8);
	bst.insert(3); bst.insert(2); bst.insert(11);
	bst.insert(13); bst.insert(5); bst.insert(17);
	bst.insert(12); bst.insert(5); bst.insert(37);

	for (auto x : bst)	std::cout << x << ",";
	std::cout << std::endl;

	bst.erase(3);
	for (auto x : bst)	std::cout << x << ",";
	std::cout << std::endl;
	bst.erase(12);
	bst.erase(37);

	for (auto x : bst)	std::cout << x << ",";
	std::cout << std::endl;

	auto start = std::begin(bst);
	auto stop = std::end(bst);

	int value = 17;
	bool keep_looking = true;
	while (start != stop && keep_looking) {
		if (*start == value)
			keep_looking = false;
		else
			++start;
	}
	if (start != stop) std::cout << *start << " is in the list." << std::endl;
	return 0;
}