#pragma once

using namespace std;

template <class T>
struct TNode
{
	TNode<T> *prev = NULL; // previous elem, NULL at the begining
	T data;
};

template <class T>
class SJstack
{
public:
	SJstack();
	~SJstack();                 // deletes all elements from top to NULL

	bool sPush(const T);        // add new elem at the end
	const T sPop();             // return value of last elem and delete it
	const T sWatch();           // return value of last elem
	bool watch(T &result);// 0 - not found, 1 - found
private:
	TNode<T> *pTop = NULL;      // pointer on the top element
};

template <class T>
SJstack<T>::SJstack()
{
	this->pTop = NULL;
}

template <class T>
SJstack<T>::~SJstack()
{
	TNode<T> *pTmp;
	while (this->pTop != NULL)
	{
		pTmp = this->pTop;
		pTop = pTop->prev;
		delete pTmp;
	}
}

template <class T>
bool SJstack<T>::sPush(T pushData)
{
	TNode<T> *pNew = new TNode<T>();
	pNew->data = pushData;
	pNew->prev = this->pTop;
	this->pTop = pNew;

	return 0;
}

template <class T>
const T SJstack<T>::sPop()
{
	if (this->pTop == NULL)
		return T();              // value of default constructor
	T res = this->pTop->data;
	TNode<T> *pTmp = this->pTop;
	pTop = pTop->prev;
	delete pTmp;
	return res;
}

template <class T>
const T SJstack<T>::sWatch()
{
	T res;
	if (this->pTop == NULL)
	{
		return T();             // value of default constructor
	}
	else
	{
		res = (this->pTop->data);
	}
	return res;
}

template <class T>
bool SJstack<T>::watch(T &result)
{
	if (this->pTop == NULL)
	{
		return 0;             // no elements in stack
	}
	else
	{
		result = (this->pTop->data);
		return 1;
	}
}
