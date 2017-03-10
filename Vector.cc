// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Jacob Ashmore

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
	//cout << "Default Vector constructor Vector() has been called" << endl;
	elements = NULL; count=0; reserved=0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
	//LHS == RHS count and reserved. Then allocate memory for the elements and copy them to LHS
	count = rhs.count;
	reserved = rhs.reserved;
	
	elements = (T*)malloc(sizeof(T)*count);
	for(size_t i=0; i < count; i++) {
		new (elements + i) T(rhs.elements[i]);
	}
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
	if(elements!=0) {
		for(size_t i=0; i<count; i++){
			elements[i].~T;
		}
		free(elements);
	}
	count=rhs.count;
	reserved=rhs.reserved;
	
	elements = (T*)malloc(sizeof(T)*reserved);
	
	for(size_t i=0; i< count; i++) {
		new (elements[i]) T(rhs.elements[i]);
	}
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"
	new (elements) T(t)
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ // Reserve extra memory for n elements of size_t
}

#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
	Clear();
	free(elements);
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
	
	if(count < reserved) {		//there is already enough memory allocated
		
		new (&elements[count]) T(rhs);//new (&elements[count] T(rhs));
		
	} else {					//there is not enough memory allocated already
		T* newElements = (T*)malloc(sizeof(T)*(count+1));
		reserved = count+1;
		for(size_t i=0; i<count; i++) {
			new (&newElements[i]) T(elements[i]);//new (&newElements[i] T(elements[i])) //call copy constructor to deep copy old elements[i] into newElements[i]
			elements[i].~T();					 //explicitly call destructor on old element values
		}
		
		new (&newElements[count]) T(rhs);//new &newElements[count] T(rhs);		//add in the new data at the end position
		count=count+1;
		free(elements);							//Already freed the memory when I explicitly called the destructor, so this should be redundant
		elements = newElements;
		
	}
	
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
	
	if(count < reserved) {
		for(size_t i=count; i>0; i--) {
			elements[i]=elements[i-1];									//go from end to beginning and move values up by one
		}
		
		new (&elements[0]) T(rhs);										//add in the new data at the first position
		count++;
	} else {
		T* newElements = (T*)malloc(sizeof(T)*(count+1));
		for(size_t i=count; i>0; i--) {
			new (newElements + i) T(elements[i-1]);
			elements[i-1].~T();
		}
		free(elements);
		elements=newElements;
		new (&elements[0]) T(rhs);
		count++;
		reserved = count; 
	}
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
	if(count>0) {
		elements[count-1].~T();
		count=count-1;//count--;
	}
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
	if(count>0) {
		elements[0].~T();
		for(size_t i=0; i<count-1;i++) {
			new (&elements[i]) T(elements[i+1]);
			elements[i+1].~T();
		}
		count=count-1;//count--;
	}
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
	return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
	return elements[count-1];
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{ // const element access
	return elements[i];
}

template <typename T>
T& Vector<T>::operator[](size_t i)
{//nonconst element access
	return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
	return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
	//Return true if there are no elements in the vector and false if there are elements
	if(count==0){
		return 1;
	} else {
		return 0;
	}
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
	//get rid of all of the elements in the vector
	reserved=count;
	for(size_t i=0; i<count; i++){
		elements[count - 1 - i].~T();	//recursively remove elements from the vector
	}
	count=0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{ 
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
	return VectorIterator<T>(elements+count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
	//initialize an empty vector
	current=NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
	current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
	current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
	//return a pointer to the current element
	return *current;
}

// Prefix incremen t
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
	return VectorIterator<T>(++current);
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
	return VectorIterator<T>(current++);
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
	return current!=rhs.current;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
	return current==rhs.current;
}




