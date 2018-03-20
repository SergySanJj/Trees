#pragma once

#ifndef SJstack_H
#define SJstack_H

template <class T>
class TNode
{
<<<<<<< HEAD
	TNode() {}
	~TNode(){}
=======
public:
	TNode()
	{

	}
	~TNode()
	{

	}
>>>>>>> origin/Dev
	TNode<T> *prev = nullptr; // previous elem, nullptr at the begining
	T data;
};

template <class T>
class SJstack
{
public:
	SJstack();
	~SJstack();                 // deletes all elements from top to nullptr

	size_t size() { return (this->_size); }

	bool sPush(const T);        // add new elem at the end
	const T sPop();             // return value of last elem and delete it
	const T sWatch();           // return value of last elem
	bool watch(T &result);      // 0 - not found, 1 - found
	bool empty();               // 0 - empty , 1 - not empty
	
private:
	TNode<T> *pTop = nullptr;      // pointer on the top element
	size_t _size = 0;
};

template <class T>
SJstack<T>::SJstack()
{
	this->pTop = nullptr;
}

template <class T>
SJstack<T>::~SJstack()
{
	TNode<T> *pTmp = nullptr;
	while (this->pTop != nullptr)
	{
		pTmp = this->pTop;
		pTop = pTop->prev;
		delete pTmp;
	}
	pTop = nullptr;
	_size = 0;
}

template <class T>
inline bool SJstack<T>::sPush(T pushData)
{
	TNode<T> *pNew = new TNode<T>();
	pNew->data = pushData;
	pNew->prev = this->pTop;
	this->pTop = pNew;
	this->_size++;
	return 0;
}

template <class T>
inline const T SJstack<T>::sPop() // methods that return variables probably won't be "inlined" in disassembed code
{
	if (this->pTop == nullptr)
		exit(1);           /// value of default constructor or exit
	T res = this->pTop->data;
	TNode<T> *pTmp = this->pTop;
	pTop = pTop->prev;
	delete pTmp;
	this->_size--;
	return res;
}

template <class T>
inline const T SJstack<T>::sWatch()
{
	if (this->pTop == nullptr)
	{
		exit(1);            /// value of default constructor or exit
	}
	else
	{
		return (this->pTop->data);
	}
}

template <class T>
inline bool SJstack<T>::watch(T &result)
{
	if (this->pTop == nullptr)
	{
		return 0;             // no elements in stack
	}
	else
	{
		result = (this->pTop->data);
		return 1;
	}
}

template<class T>
inline bool SJstack<T>::empty()
{
	if (this->pTop != nullptr)
		return 0;
	else
		return 1;
}

#endif // !SJstack_H