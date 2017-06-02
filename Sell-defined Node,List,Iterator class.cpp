/**	@file Operator Overloading.cpp
	@author Bowen Xuan
	@date May 16th 

	This program contains List, Node and Iterator class. Every node contains an int type data
	and with private variable Node* previous and after; List class inludes public member functions
	to sort, reverse, swap, merge lists. The Iterator class has all the operator overloaded that
	can be used to iterator through list.	
*/


#include <iostream>    
#include <iomanip>      
#include <string>
#include <cassert>
#include <algorithm>
/**Every node contains an int type data and with private variable Node* previous and after; 
*/
class Node {
	friend class List;
	friend class Iterator;
	friend void sort2(List&a);
public:
	Node();
	Node(int s);
	int operator*() const;
private:
	int data;
	Node* previous;
	Node* next;
};
class List {
	friend class Iterator;
	friend void sort(List& a);
	friend void sort2(List& a);
	friend void swap(List& a, List& b);
public:
	List();
	~List();
	Iterator insert(Iterator pos, int s);
	void push_back(int data);
	Iterator erase(Iterator pos);
	Iterator end();
	Iterator begin();
	void reverse();
	void reverse2();
	void reverse3();
	void push_front();
	void sort();
	int get_size() const;
	Iterator List::maximum_pos(Iterator from, Iterator to);
private:
	Node* special;
	Node* first;
	Node* last;
	unsigned int size;
};
class Iterator {	//serve as pointer
	friend class List;
	friend void sort2(List&a);
public:
	//Iterator(List& contain) : container(contain) { }
	Iterator(Node* pos, List* contain);
	int get() const;
	Iterator& operator++();
	Iterator operator++(int a);
	Iterator& operator--();
	Iterator operator--(int a);
	int operator*() const;
	bool operator==(const Iterator& b) const;
	void next();
	void previous();
private:
	Node* position;
	//const List& container;  // List* container?
	List* container;
};

List::List() {
	special = new Node(0); 
	first = special;
	last = special;
	size = 0;
}
List::~List() {	//delete all nodes
	//std::cout << "destructor" << std::endl;
	Node* current = first;
	while (current) {   //not NULL
		Node* temp = current->next;
		delete current;
		current = temp;
	}
}
void List::push_back(int data) { //update end,last 
	Node* new_node = new Node(data);
	if (first == NULL) {	//inserting first node
		first = new_node;
		last = new_node;
		special->previous = new_node;
		new_node->next = special;
	}
	else {	//normal pushing back
		new_node->previous = last;
		new_node->next = special;
		last->next = new_node;
		last = new_node;
	}
}
Iterator List::insert(Iterator pos, int s) {
	assert(pos.position != NULL);
	Node* new_node = new Node(s); 
	Node* after = pos.position;
	Node* before = after->previous;
	new_node->previous = before;
	new_node->next = after;
	after->previous = new_node;

	if (after == special)	//inserting at the end
		last = new_node;
	if (before == NULL)		//inserting at the beginning
		first = new_node;	//no before->next
	else
		before->next = new_node;
	++size;
	pos.position = new_node;  //return pos of newly inserted node
	return pos;
}
Iterator List::erase(Iterator pos) {
	assert(pos.position != special);
	Node* remove = pos.position;
	Node* before = pos.position->previous;
	Node* after = pos.position->next;

	if (remove == first)		//if removing first node
		first = after;
	else
		before->next = after;
	if (remove == last)			//if removing last node
		last = before;
	after->previous = before;	//always do this because we have special after
	--size;
	delete remove;
	Iterator r(after, this);
	return r;
}
Iterator List::end(){
	Iterator temp(special, this);
	return temp;
}
Iterator List::begin(){
	Iterator temp(first, this);
	return temp;
}
void List::reverse() {	//copying to a new list
	if (first == last)	//if an empty list
		return;
	List temp;	//an empty list
	while (first != special) {					//until only last and special is in the list
		temp.insert(temp.begin(), *begin());	//insert from the first to the one before last
		erase(begin());							//erase first in this
	}
	swap(*this, temp);		//solved problem with destructor
}
 void List::reverse2() {
	if (first == last)	//if an empty list
		return;
	Node* iter = first;	//first node will be the last, with previous to second last, next pointing to special
	iter->previous = iter->next;
	iter->next = special;
	iter = iter->previous; //moving iter to next node in current order
	while (iter != last) { //iterating through current list, swapping previous and next
		Node* temp = iter->next;
		iter->next = iter->previous;
		iter->previous = temp;
		iter = temp;	//++iter
	}
	iter->next = iter->previous; //iter pointing to last
	iter->previous = NULL;	//last node will be the first node
	special->previous = first;//update the linking special to new last
	last = first;	//swapping first and last
	first = iter; //iter pointing to last
}
void List::reverse3() {  //swapping data
	if (first == last)	 //if an empty list
		return;
	Iterator front(first, this);
	Iterator back(last, this);
	int temp, i = size / 2;
	while (i) {
		temp = *front;
		front.position->data = *back;
		back.position->data = temp;
		--i;
		++front;
		--back;
	}

}
int List::get_size() const{
	return size;
}
void List::sort() {
	//Iterator last_pos(last, this);
	//Iterator max_pos = maximum_pos(begin(), last_pos);
	//Iterator iter = begin();

	//while (!(iter == last_pos)) {
	//	std::swap(*iter, *max_pos);
	//	++iter;
	//	max_pos = maximum_pos(iter, end());
	//}

	////Reverse the list because right now it is sorted from large to small.
	//reverse();
}
Iterator List::maximum_pos(Iterator from, Iterator to) {
	Iterator max_pos = from;
	Iterator iter = from;
	int max = *max_pos;

	//If it is an empty list...
	if (from == to)
		return max_pos;
	else {
		do {
			if (max < *iter)
				max_pos = iter;
			++iter;
		} while (!(iter == to));

		return max_pos;
	}

}
Iterator::Iterator(Node* pos, List* contain) {
	position = pos;
	container = contain;
}
int Iterator::get() const {
	assert(position != NULL);
	return position->data;
}
void Iterator::next() { //stops at NULL
	assert(position != NULL);
	position = position->next;
}
void Iterator::previous() { //stops at first
	assert(position != container->first);
	if (position == NULL)
		position = container->last;
	else
		position = position->previous;

}
Iterator& Iterator::operator++() {
	assert(position != container->special);
	position = position->next;
	return *this;
}
Iterator Iterator::operator++(int a) {
	assert(position != container->special);
	Iterator temp = *this;
	position = position->next;
	return temp;
}
Iterator& Iterator::operator--() {
	assert(position != container->first);
	if (position == container->last)	//???
		position = (container->last)->previous;
	else
		position = position->previous;
	return *this;
}
Iterator Iterator::operator--(int a) {
	assert(position != container->first);
	Iterator temp = *this;
	if (position == container->last)	//???
		position = (container->last)->previous;
	else
		position = position->previous;
	return temp;
}
int Iterator::operator*() const{
	//return *(*position);
	return position->data;
}
bool Iterator::operator==(const Iterator& b) const{
	return position == b.position;	//container??
}

Node::Node() {
	data = 0;
	previous = NULL;
	next = NULL;
}
Node::Node(int s) {
	data = s;
	previous = NULL;
	next = NULL;
}
int Node::operator*() const {
	return data;
}
bool operator!=(Iterator& a, Iterator& b) {
	return !(a == b);
}
void print(List& mylist) {
	Iterator temp = mylist.begin();
	std::cout << "Your list is: \n(" << *temp;
	++temp;
	for (; temp != mylist.end(); ++temp)
		std::cout << "," << *temp;
	std::cout << ")" << std::endl;
}
void print2(List& mylist) {
	for (auto x : mylist)
		std::cout << x << ",";
	std::cout << std::endl;
}
int maximum(List& a) {
	Iterator temp = a.begin();
	int max = *temp;
	while (temp != a.end()) {
		++temp;
		if (*temp > max)
			max = *temp;
	}
	return max;
}
void downsize(List& a) {			
	for(Iterator temp = a.begin(); temp != a.end();){
		++temp;
		if (temp != a.end())      //check temp for every increament!
			temp = a.erase(temp); //always update temp
	}
}
void sort(List& a) {  //creating another list
	List temp;
	int max = *a.begin();
	Iterator max_iter = a.begin();
	Iterator temp_iter = a.begin();
	while (a.first != a.special) {
		for (temp_iter = max_iter = a.begin(), max = *a.begin(); temp_iter != a.end(); ++temp_iter) {
			if (*temp_iter > max) {
				max = *temp_iter;
				max_iter = temp_iter;
			}
		}
		temp.insert(temp.begin(), max); //adding in reverse order
		a.erase(max_iter);
	}
	swap(a, temp);
}
void sort2(List& a) {
	int temp;
	Iterator min_pos = a.begin();
	Iterator temp_iter = a.begin();
	while (temp_iter != a.end()) {
		for (Iterator min_iter = temp_iter; min_iter != a.end(); ++min_iter) { //min_iter iterates from temp_iter to end
			if (*min_iter < *min_pos)
				min_pos = min_iter; //updates min_pos
		}
		//swapping data of temp and min position
		std::swap(temp_iter.position->data, min_pos.position->data);
		++temp_iter; //moves temp_iter to next position
		min_pos = temp_iter;//updates min_pos
	}
}
void swap(List& a, List& b) {
	std::swap(a.first, b.first);
	std::swap(a.last, b.last);
	std::swap(a.special, b.special);
	std::swap(a.size, b.size);
}

int main() {
	List mylist;
	Iterator myiterator = mylist.begin();
	myiterator = mylist.insert(myiterator, 1);
	myiterator = mylist.insert(myiterator, 4);
	myiterator = mylist.insert(myiterator, 3);
	myiterator = mylist.insert(myiterator, 42);
	myiterator = mylist.end();
	myiterator = mylist.insert(myiterator, 11);
	++myiterator;
	myiterator = mylist.insert(myiterator, 7);
	myiterator = mylist.insert(myiterator, 8);
	myiterator = mylist.insert(myiterator, 128);
	myiterator = mylist.insert(myiterator, 7);
	myiterator = mylist.end();
	myiterator = mylist.insert(myiterator, 7);
	myiterator = mylist.end();
	myiterator = mylist.insert(myiterator, 7);
	myiterator = mylist.end();
	myiterator = mylist.insert(myiterator, 7);
	print(mylist);
	myiterator = mylist.erase(myiterator); //erased last 2.
	--myiterator;
	print2(mylist);
	myiterator = mylist.begin();
	//myiterator = mylist.erase(myiterator);
	//mylist.erase(myiterator);
	print(mylist);
	mylist.reverse3();

	std::cout << "After reversing, ";
	print(mylist);
	std::cout << "size of list is: " << mylist.get_size() << std::endl;
	std::cout << "The max num in list is: " << maximum(mylist) << std::endl;
	downsize(mylist);
	std::cout << "After downsize, ";
	print(mylist);
	sort2(mylist);
	std::cout << "After sorting, ";
	print(mylist);
	List mylist2;
	mylist2.insert(mylist2.begin(), 4);
	mylist2.insert(mylist2.begin(), 4);
	std::cout << "For mylist2, ";
	print(mylist2);
	swap(mylist, mylist2);
	std::cout << "After swaping with mylist 2, ";
	print(mylist);
	std::cout << "After swaping with mylist, ";
	print(mylist2);

	//for(auto x: mylist)
	//	std::cout << x << " , ";
	//std::cout << std::endl;
}