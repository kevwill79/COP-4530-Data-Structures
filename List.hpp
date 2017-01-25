#ifndef DL_LIST_CPP
#define DL_LIST_CPP

//-----------nested const_iterator class-----------

//default zero parameter constuctor
//book function
template <typename T>
List<T>::const_iterator::const_iterator():current{nullptr} {}

//operator*() to return element
//book function
template <typename T>
const T & List<T>::const_iterator::operator*() const 
{
	return retrieve();
}

//book function
template <typename T>
typename List<T>::const_iterator & List<T>::const_iterator::operator++() 
{
	current = current->next;
	return *this;
}

//book function
template <typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator++(int) 
{
	const_iterator old = *this;
	++(*this);
	return old;
}

//prefix --
template <typename T>
typename List<T>::const_iterator & List<T>::const_iterator::operator--() 
{
	current = current->prev;	//current becomes prev (same as --)
	return *this;
}


//postfix --
template <typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator--(int) 
{
	const_iterator old = *this;	
	--(*this);
	return old;
}

//book function
template <typename T>
bool List<T>::const_iterator::operator==(const const_iterator & rhs) const 
{
	return current == rhs.current;
}

//book function
template <typename T>
bool List<T>::const_iterator::operator!=(const const_iterator &rhs) const 
{
	return !(*this == rhs);
}

//retrieve the element refers to
//book function
template <typename T>
T & List<T>::const_iterator::retrieve() const 
{
	return current->data;
}

//protected one parameter constructor
//book function
template <typename T>
List<T>::const_iterator::const_iterator(Node *p):current{p} {}


//-----------------nested iterator class----------------

//default parameter
//book function
template <typename T>
List<T>::iterator::iterator() {}

//book function
template <typename T>
T & List<T>::iterator::operator*() 
{
	return const_iterator::retrieve();
}

//book function
template <typename T>
const T & List<T>::iterator::operator*() const 
{
	return const_iterator::operator*();
}

//book function
template <typename T>
typename List<T>::iterator & List<T>::iterator::operator++() 
{
	this->current = this->current->next;
	return *this;
}

//book function
template <typename T>
typename List<T>::iterator List<T>::iterator::operator++(int) 
{
	iterator old = *this;
	++(*this);
	return old;
}

//prefix --
template <typename T>
typename List<T>::iterator & List<T>::iterator::operator--() 
{
	this->current = this->current->prev;
	return *this;
}

//postfix --
template <typename T>
typename List<T>::iterator List<T>::iterator::operator--(int) 
{
	iterator old = *this;
	--(*this);
	return old;
}

//protected one parameter constructor
//book function
template <typename T>
List<T>::iterator::iterator(Node *p):List<T>::const_iterator::const_iterator{p} {}

//-------------------list class-------------------

//default zero parameter constructor
//book function
template <typename T>
List<T>::List() 
{
	init();
}

//copy constructor
//book function
template <typename T> 
List<T>::List(const List & rhs) 
{
	init();	
	for(auto & x : rhs)
		push_back(x);
}

//move constructor
//book function
template <typename T>
List<T>::List(List && rhs) 
	: theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail} 
	{
		rhs.theSize = 0;
		rhs.head = nullptr;
		rhs.tail = nullptr;
	}

//num elements with value of val
template <typename T>
List<T>::List(int num, const T & val)
{
	//initialize the list
	init();						
	
	//num is the size, all elements contain whatever val is
	for(auto i = 0; i < num; i++)
		push_back(val);
}

//constructs elements with elements [start, end)
template <typename T>
List<T>::List(const_iterator start, const_iterator end) 
{
	//intialize the list
	init();

	//begin with element at "start", does not include the "end" element
	for(auto itr = start; itr != end; ++itr)
		push_back(*itr);
}

//constructs with a copy of each of the elements in the initializer_list
template <typename T>
List<T>::List(std::initializer_list<T> iList)
{
	//initialize the list
	init();

	//pushes the elements from iList into the list
	for(auto itr = iList.begin(); itr != iList.end(); ++itr)
		push_back(*itr);
}

//destructor
//book function
template <typename T>
List<T>::~List() 
{
	clear();
	delete head;
	delete tail;
}

//copy assignment operator
//book function
template <typename T>
const List<T> & List<T>::operator=(const List & rhs) 
{
	List copy = rhs;
	std::swap(*this, copy);
	return *this;
}

//move assignment operator
//book function
template <typename T>
List<T> & List<T>::operator=(List && rhs) 
{
	std::swap(theSize, rhs.theSize);
	std::swap(head, rhs.head);
	std::swap(tail, rhs.tail);

	return *this;
}

//sets list to the elements of the initializer_list
template <typename T>
List<T>& List<T>::operator=(std::initializer_list<T> iList)
{
	//initialize the list
	init();

	//pushes the elements from iList into the list
	for(auto itr = iList.begin(); itr != iList.end(); ++itr)
		push_back(*itr);
}

//number of elements
//book function
template <typename T>
int List<T>::size() const 
{
	return theSize;
}

//check if list is empty
//book function
template <typename T>
bool List<T>::empty() const 
{
	return size() == 0;
}

//delete all elemnts
//book function
template <typename T>
void List<T>::clear() 
{
	while(!empty())
		pop_front();
}

//reverse the order of the elements
template <typename T>
void List<T>::reverse() 
{
	//set current to first element
	auto current = head;

	do
	{
		//swap the next element with the previous element
		std::swap(current->next, current->prev);
		current = current->prev;
	}
	while(current != nullptr);	//nullptr signals the end of list

	std::swap(head,tail);	//lastly swap the head and the tail
}

//reference to the first element
//book function
template <typename T>
T & List<T>::front() 
{
	return *begin();
}

//book function
template <typename T>
const T& List<T>::front() const 
{
	return *begin();
}

//reference to the last element
//book function
template <typename T>
T & List<T>::back() 
{
	return *--end();
}

//book function
template <typename T>
const T & List<T>::back() const
{
	return *--end();
} 

//insert to the beginning
//book function
template <typename T>
void List<T>::push_front(const T & val) 
{
	insert(begin(), val);
}

//move version of insert
//book function
template <typename T>
void List<T>::push_front(T && val) 
{
	insert(begin(), std::move(val));
}

//insert to the end
//book function
template <typename T>
void List<T>::push_back(const T & val) 
{
	insert(end(), val);
}

//move version of insert
//book function
template <typename T>
void List<T>::push_back(T && val) 
{
	insert(end(), std::move(val));
}

//delete first element
//book function
template <typename T>
void List<T>::pop_front() 
{
	erase(begin());
}

//delete last element
//book function
template <typename T>
void List<T>::pop_back() 
{
	erase(--end());
}

//remove all elements with value = val
template <typename T>
void List<T>::remove(const T &val) 
{
	for(auto itr = begin(); itr != end(); ++itr)
	{
		if(*itr == val)
			erase(itr);
	}
}

//remove all elements for which Predicate pred
//returns true. pred can take in a function object
template <typename T>
template <typename PREDICATE>
void List<T>::remove_if(PREDICATE pred)
{
	for(auto itr = begin(); itr != end(); ++itr)
	{
		//if pred's val = the list's val erase it
		if(pred(*itr) == true)
			erase(itr);
	}
}

//print out all elements. ofc is deliminitor
template <typename T>
void List<T>::print(std::ostream& os, char ofc) const 
{
	//iterate through the stream
	for(auto itr = begin(); itr != end(); ++itr)
		os << *itr << ofc;
}

//iterator to first element
//book function
template <typename T>
typename List<T>::iterator List<T>::begin() 
{
	return{head->next};
}

//book function
template <typename T>
typename List<T>::const_iterator List<T>::begin() const 
{
	return{head->next};
}

//end marker iterator
//book function
template <typename T>
typename List<T>::iterator List<T>::end() 
{
	return{tail};
}

//book function
template <typename T>
typename List<T>::const_iterator List<T>::end() const 
{
	return{tail};
}

//insert val ahead of itr
//book function
template <typename T>
typename List<T>::iterator List<T>::insert(iterator itr, const T & val) 
{
	Node *p = itr.current;
	theSize++;
	
	return{p->prev = p->prev->next = new Node{val, p->prev, p}};
}

//move version of insert
//book function
template <typename T>
typename List<T>::iterator List<T>::insert(iterator itr, T && val) 
{
	Node *p = itr.current;
	theSize++;
	return{p->prev = p->prev->next = new Node{std::move(val), p->prev, p}};
}

//erase one element
//book function
template <typename T>
typename List<T>::iterator List<T>::erase(iterator itr) 
{
	Node *p = itr.current;
	iterator retVal{p->next};
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p;
	theSize--;

	return retVal;
}

//erase [start, end)
//book function
template <typename T>
typename List<T>::iterator List<T>::erase(iterator start, iterator end) 
{
	for(iterator itr = start; itr != end;)
		itr = erase(itr);

	return end;
}


//initialization
//book function
template <typename T>
void List<T>::init() 
{
	theSize = 0;
	head = new Node;
	tail = new Node;
	head->next = tail;
	tail->prev = head;
}

//----------overloading comparison operators-----------

template <typename T>
bool operator==(const List<T> & lhs, const List<T> & rhs) 
{
	//check the size first
	if(lhs.size() != rhs.size())
		return false;
	
	//start itrs for lhs and rhs, then check to see if *itrs match
	for(auto lItr = lhs.begin(), rItr = rhs.begin(); lItr != lhs.end(); ++lItr, ++rItr)
	{
		if(*lItr != *rItr)
			return false;
	}

	return true;
}

template <typename T>
bool operator!=(const List<T> & lhs, const List<T> &rhs) 
{
	return !(lhs == rhs);	//return if the do not match
}

//comment
template <typename T>
std::ostream & operator<<(std::ostream & os, const List<T> & l) 
{
	l.print(os);

	return os;
}

#endif
