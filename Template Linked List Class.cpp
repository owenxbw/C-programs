/**	@file Template Linked List Class.cpp
	@author Bowen Xuan
	@date May 30th 

	This program contains template List, Node and Iterator class. Every node contains an int type data
	and with private variable Node* previous and after; List class inludes public member functions
	to sort, reverse, swap, merge lists. The Iterator class has all the operator overloaded that
	can be used to iterator through list.	
*/


#include <iostream>    
#include <iomanip>      
#include <string>
#include <cassert>
#include <algorithm>


template <typename T>
class List;
template <typename T>
class Iterator;


/** This class Node contains data(template type) two Node pointers: previous and next
	@param previous and next 
	return
*/
template <typename T>
class Node {
	friend class List<T>;
	friend class Iterator<T>;
	//friend void sort2(List<T>& a);
public:
	Node() :data(),previous(NULL), next(NULL){}
	Node(T s) :data(s), previous(NULL), next(NULL){}
	T operator*() const;
private:
	T data;
	Node<T>* previous;
	Node<T>* next;
};

/** This class List contains three Node pointers: special,first,last. Special is the one after last.
	It also contains several member functions: insert(),erase(),reverse(),swap(),sort() and push_front etc. 
	@param special,first,last 
	return
*/
template<typename T>
class List {
	friend class Iterator<T>;
	friend void sort(List<T>& a);
	friend void swap(List<T>& a, List<T>& b);
public:
	List();
	~List();
	Iterator<T> insert(Iterator<T> pos, T s);
	void push_back(T data);
	Iterator<T> erase(Iterator<T> pos);
	Iterator<T> end();
	Iterator<T> begin();
	void reverse();
	void swap(List<T>& other);
	void sort();
	void reverse2();
	void reverse3();
	void push_front();
	//void sort();
	int get_size() const;
	Iterator<T> List<T>::maximum_pos(Iterator<T> from, Iterator<T> to);
private:
	Node<T>* special;
	Node<T>* first;
	Node<T>* last;
	unsigned int size;
};

\
/** This class Iterator contains one Node pointers: position and one List pointer container 
	It also contains several member functions: ++,--,*(),next(), previous()
	@param position, container
	return
*/
template<typename T>
class Iterator {	//serve as pointer
	friend class List<T>;
	friend void sort(List<T>& a);
public:
	//Iterator(List& contain) : container(contain) { }
	Iterator(Node<T>* pos, List<T>* contain);
	T get() const;
	Iterator<T>& operator++();
	Iterator<T> operator++(int a);
	Iterator<T>& operator--();
	Iterator<T> operator--(int a);
	T operator*() const;
	bool operator==(const Iterator<T>& b) const;
	void next();
	void previous();
private:
	Node<T>* position;
	//const List& container;  // List* container?
	List<T>* container;
};


template<typename T>
List<T>::List() {
	special = new Node<T>;
	first = special;
	last = special;
	size = 0;
}
template<typename T>
List<T>::~List() 
{	//delete all nodes
	std::cout << "destructor" << std::endl;
	Node<T>* current = first;
	while (current) {   //not NULL
		Node<T>* temp = current->next;
		delete current;
		current = temp;
	}
}
template<typename T>
Iterator<T> List<T>::insert(Iterator<T> pos, T s) {
	assert(pos.position != NULL);
	Node<T>* new_node = new Node<T>(s);
	Node<T>* after = pos.position;
	Node<T>* before = after->previous;
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

template<typename T>
Iterator<T> List<T>::erase(Iterator<T> pos) {
	assert(pos.position != special);
	Node<T>* remove = pos.position;
	Node<T>* before = pos.position->previous;
	Node<T>* after = pos.position->next;

	if (remove == first)		//if removing first node
		first = after;
	else
		before->next = after;
	if (remove == last)			//if removing last node
		last = before;
	after->previous = before;	//always do this because we have special after
	--size;
	delete remove;
	Iterator<T> r(after, this);
	return r;
}

template<typename T>
Iterator<T> List<T>::end() {
	Iterator<T> temp(special, this);
	return temp;
}

template<typename T>
Iterator<T> List<T>::begin() {
	Iterator<T> temp(first, this);
	return temp;
}

template<typename T>
void List<T>::reverse() {	//copying to a new list
	if (first == last)	//if an empty list
		return;
	List<T> temp;	//an empty list
	while (first != special) {					//until only last and special is in the list
		temp.insert(temp.begin(), *begin());	//insert from the first to the one before last
		erase(begin());							//erase first in this
	}
	swap(*this, temp);		//solved problem with destructor
}
template<typename T>
void List<T>::reverse2() {
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
template<typename T>
void List<T>::reverse3() {  //swapping data
	if (first == last)	 //if an empty list
		return;
	Iterator<T> front(first, this);
	Iterator<T> back(last, this);
	T temp;
	int i = size / 2;
	while (i) {
		temp = *front;
		front.position->data = *back;
		back.position->data = temp;
		--i;
		++front;
		--back;
	}

}
template<typename T>
int List<T>::get_size() const {
	return size;
}
template<typename T>
void List<T>::swap(List<T>& other) {
	std::swap(first, other.first);
	std::swap(last, other.last);
	std::swap(special, other.special);
	std::swap(size, other.size);
}
template<typename T>
void List<T>::sort() {
	List<T> temp;
	T max = *begin();
	Iterator<T> max_iter = begin();
	Iterator<T> temp_iter = begin();
	while (first != special) {
		for (temp_iter = max_iter = begin(), max = *begin(); temp_iter != end(); ++temp_iter) {
			if (*temp_iter > max) {
				max = *temp_iter;
				max_iter = temp_iter;
			}
		}
		temp.insert(temp.begin(), max); //adding in reverse order
		erase(max_iter);
	}
	swap(temp);
}
template<typename T>
Iterator<T> List<T>::maximum_pos(Iterator<T> from, Iterator<T> to) {
	Iterator<T> max_pos = from;
	Iterator<T> iter = from;
	T max = *max_pos;

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

template<typename T>
Iterator<T>::Iterator(Node<T>* pos, List<T>* contain) {
	position = pos;
	container = contain;
}

template<typename T>
T Iterator<T>::get() const {
	assert(position != NULL);
	return position->data;
}

template<typename T>
void Iterator<T>::next() { //stops at NULL
	assert(position != NULL);
	position = position->next;
}

template<typename T>
void Iterator<T>::previous() { //stops at first
	assert(position != container->first);
	if (position == NULL)
		position = container->last;
	else
		position = position->previous;
}

template<typename T>
Iterator<T>& Iterator<T>::operator++() {
	assert(position != container->special);
	position = position->next;
	return *this;
}

template<typename T>
Iterator<T> Iterator<T>::operator++(int a) {
	assert(position != container->special);
	Iterator<T> temp = *this;
	position = position->next;
	return temp;
}

template<typename T>
Iterator<T>& Iterator<T>::operator--() {
	assert(position != container->first);
	if (position == container->last)	//???
		position = (container->last)->previous;
	else
		position = position->previous;
	return *this;
}

template<typename T>
Iterator<T> Iterator<T>::operator--(int a) {
	assert(position != container->first);
	Iterator temp = *this;
	if (position == container->last)	//???
		position = (container->last)->previous;
	else
		position = position->previous;
	return temp;
}

template<typename T>
T Iterator<T>::operator*() const {
	//return *(*position);
	return position->data;
}

template<typename T>
bool Iterator<T>::operator==(const Iterator<T>& b) const {
	return position == b.position;	//container??
}

template<typename T>
T Node<T>::operator*() const {
	return data;
}


class Point2D {
	friend std::ostream& operator<<(std::ostream& out, const Point2D&a);
public:
	Point2D() :x(0), y(0){}
	Point2D(double init_x, double init_y);
	bool operator<(const Point2D& rhs) const;
	bool operator==(const Point2D& rhs) const;
private:
	double x, y;
};
Point2D::Point2D(double init_x, double init_y) {
	x = init_x;
	y = init_y;
}
bool Point2D::operator<(const Point2D& rhs) const {
	return (x < rhs.x);
}
bool Point2D::operator==(const Point2D& rhs) const {
	return (x == rhs.x);
}
std::ostream& operator<<(std::ostream& out, const Point2D& a) {
	return out <<"("<< a.x << "," << a.y <<")";
}




/**Non-member functions*/
template<typename T>
bool operator!=(Iterator<T>& a, Iterator<T>& b) {
	return !(a == b);
}
template<typename T>
void print(List<T>& mylist) {
	Iterator<T> temp = mylist.begin();
	std::cout << "Your list is: \n(" << *temp;
	++temp;
	for (; temp != mylist.end(); ++temp)
		std::cout << "," << *temp;
	std::cout << ")" << std::endl;
}
template<typename T>
void print2(List<T>& mylist) {
	for (auto x : mylist)
		std::cout << x << ",";
	std::cout << std::endl;
}
template<typename T>
T maximum(List<T>& a) {
	Iterator<T> temp = a.begin();
	int max = *temp;
	while (temp != a.end()) {
		++temp;
		if (*temp > max)
			max = *temp;
	}
	return max;
}
template<typename T>
void swap(List<T>& a, List<T>& b) {
	std::swap(a.first, b.first);
	std::swap(a.last, b.last);
	std::swap(a.special, b.special);
	std::swap(a.size, b.size);
}
template<typename T>
void downsize(List<T>& a) {
	for (Iterator<T> temp = a.begin(); temp != a.end();) {
		++temp;
		if (temp != a.end())      //check temp for every increament!
			temp = a.erase(temp); //always update temp
	}
}
template<typename T>
void sort(List<T>& a) {  //creating another list
	List<T> temp;
	T max = *a.begin();
	Iterator<T> max_iter = a.begin();
	Iterator<T> temp_iter = a.begin();
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



int main() {
	List<Point2D> mylist;
	Iterator<Point2D> myiterator = mylist.begin();
	myiterator = mylist.insert(myiterator, Point2D(1, 2));
	myiterator = mylist.insert(myiterator, Point2D(2, 2));
	myiterator = mylist.insert(myiterator, Point2D(3, 2));
	myiterator = mylist.insert(myiterator, Point2D(4, 2));
	myiterator = mylist.end();
	myiterator = mylist.insert(myiterator, Point2D(3, 2));
	myiterator = mylist.insert(myiterator, Point2D(4, 2));
	//myiterator = mylist.insert(myiterator, 11);
	//++myiterator;
	//myiterator = mylist.end();
	//myiterator = mylist.insert(myiterator, 7);
	//myiterator = mylist.end();
	//myiterator = mylist.insert(myiterator, 7);
	//myiterator = mylist.end();
	//myiterator = mylist.insert(myiterator, 7);
	print(mylist);
	//myiterator = mylist.erase(myiterator); //erased last 2.
	//--myiterator;
	myiterator = mylist.begin();
	myiterator = mylist.erase(myiterator);
	mylist.erase(myiterator);
	print(mylist);
	mylist.reverse3();

	std::cout << "after reversing, ";
	print(mylist);
	//std::cout << "size of list is: " << mylist.get_size() << std::endl;
	//std::cout << "The max num in list is: " << maximum(mylist) << std::endl;
	//downsize(mylist);
	//std::cout << "After downsize, ";
	//print(mylist);
	//mylist.sort();
	//std::cout << "After sorting, ";
	//print(mylist);
	//List<int> mylist2;
	//mylist2.insert(mylist2.begin(), 4);
	//mylist2.insert(mylist2.begin(), 4);
	//std::cout << "For mylist2, ";
	//print(mylist2);
	//mylist.swap(mylist2);
	//std::cout << "After swaping with mylist 2, ";
	//print(mylist);
	//std::cout << "After swaping with mylist, ";
	//print(mylist2);

	//for(auto x: mylist)
	//	std::cout << x << " , ";
	//std::cout << std::endl;
}