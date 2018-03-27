#pragma once

#ifndef RNDtree_H_
#define RNDtree_H_

#include "SJstack.h" // my version of stack class

namespace Trees
{
	using namespace std;

	template <typename keyType, class dataType>
	class RndNode
	{
	public:
		keyType  key;  // standart type key so we can do operations between them like <,>,==
		dataType data; // so any type data can be stored
		RndNode<keyType, dataType> *_left = nullptr;   // left sub-tree
		RndNode<keyType, dataType> *_right = nullptr;  // right sub-tree
		int _size = 1;

		RndNode() { key = keyType(); data = dataType(); }

		~RndNode() { _left = nullptr; _right = nullptr; }

		RndNode(keyType key, dataType data)  // copy constructor
		{
			this->key = key;
			this->data = data;
		}

		RndNode<keyType, dataType> operator = (const RndNode<keyType, dataType> &RndNode2)
		{
			return (RndNode<keyType, dataType>(this->key = RndNode2.key, this->data = RndNode2.data));
		}
	};


#define sRNDtree    RNDtree<keyType, dataType>
#define rNode    RndNode<keyType, dataType>
#define dataPair pair<keyType, dataType>

	template <typename keyType, class dataType>
	class RNDtree
	{
	public:
		RNDtree();
		~RNDtree();

		rNode* getRoot();
		size_t getHeight();
		int getSize(rNode* subTree);
		void add(keyType addKey, dataType addElement);          // Interface to add element.
		bool find(keyType searchKey);
		bool find(keyType searchKey, dataType &resultVariable); // Search first element with searchKey key, return 1 and change resultVariable if such element was found,
																//return 0 and don't change resultVariable if such element doesn't exist.
		void del(keyType deleteKey);                            // Delete closest to root element with deleteKey key.
		void buildFromStack(SJstack<dataPair> &pairStack);      // Add new row of elements from stack to the tree.
		void clear();                                           // Delete all nodes from tree.

		void printPostOrder();                                  // Wrapper function for backend print.
		void printInOrder();
		void printPreOrder();

	private:
		//Data//
		rNode * Root = nullptr;

		//Methods//
		void maxHeight(rNode* subTree, size_t &etalon, size_t curr);
		void resize(rNode* subTree);
		void recDelete(rNode *subTree);                                                       // recursive deletting all inheritor nodes, used as destructor
		void traversePostOrder(rNode *subTree, SJstack<rNode> &treeStack);                    // traverse tree in post-order and add each node into stack
		void traversePostOrder(rNode *subTree, SJstack<rNode> &treeStack, keyType deleteKey); // modification that won't include elements with set key into the stack
		void BE_printPostOrder(rNode *subTree);
		void BE_printInOrder(rNode *subTree);
		void BE_printPreOrder(rNode *subTree);

		rNode* rotateRight(rNode* subTree);
		rNode* rotateLeft(rNode* subTree);

		rNode* simpleInsert(rNode* subTree, keyType addKey, dataType addElement); // standart insert in search tree
		rNode* inserToRoot(rNode* subTree, keyType addKey, dataType addElement);  // insert into the root of chosen sub-tree
		
		rNode* rndInsert(rNode* subTree, keyType addKey, dataType addElement);    // wrapper that chooses between simpleInsert & inserToRoot
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename keyType, class dataType>
	RNDtree<keyType, dataType>::RNDtree()
	{
	}

	template <typename keyType, class dataType>
	RNDtree<keyType, dataType>::~RNDtree()
	{
		recDelete(this->Root);
	}

	template<typename keyType, class dataType>
	inline rNode * RNDtree<keyType, dataType>::getRoot()
	{
		return this->Root;
	}

	template<typename keyType, class dataType>
	inline size_t RNDtree<keyType, dataType>::getHeight()
	{
		size_t res = 0;
		if (this->Root == nullptr)
			return res;
		maxHeight(this->Root, res, 1);
		return res;
	}

	template<typename keyType, class dataType>
	inline int RNDtree<keyType, dataType>::getSize(rNode* subTree)
	{
		if (subTree == nullptr)
			return 0;
		return subTree->_size;
	}

	template <typename keyType, class dataType>
	void RNDtree<keyType, dataType>::add(keyType addKey, dataType addElement)
	{
		this->Root = rndInsert(this->Root, addKey, addElement);
	}

	template<typename keyType, class dataType>
	inline bool RNDtree<keyType, dataType>::find(keyType searchKey)
	{
		if (this->Root == nullptr) // stop if empty
			return 0;

		rNode *pCurNode = this->Root;
		while (searchKey != pCurNode->key && (pCurNode->_left != nullptr || pCurNode->_right != nullptr))
		{
			if (searchKey < pCurNode->key)
			{
				if (pCurNode->_left != nullptr)
				{
					pCurNode = pCurNode->_left;
					continue;
				}
				else
					return 0;
			}
			else
			{
				if (pCurNode->_right != nullptr)
				{
					pCurNode = pCurNode->_right;
					continue;
				}
				else
					return 0;
			}
		}
		if (pCurNode->key == searchKey)
		{
			return 1;
		}
		else
			return 0;
	}

	template <typename keyType, class dataType>
	bool RNDtree<keyType, dataType>::find(keyType searchKey, dataType &resultVariable) // ln(n) travers search using pre-order search
	{
		if (this->Root == nullptr) // stop if empty
			return 0;

		rNode *pCurNode = this->Root;
		while (searchKey != pCurNode->key && (pCurNode->_left != nullptr || pCurNode->_right != nullptr))
		{
			if (searchKey < pCurNode->key)
			{
				if (pCurNode->_left != nullptr)
				{
					pCurNode = pCurNode->_left;
					continue;
				}
				else
					return 0;
			}
			else
			{
				if (pCurNode->_right != nullptr)
				{
					pCurNode = pCurNode->_right;
					continue;
				}
				else
					return 0;
			}
		}
		if (pCurNode->key == searchKey)
		{
			resultVariable = pCurNode->data;
			return 1;
		}
		else
			return 0;
	}

	template <typename keyType, class dataType>
	void RNDtree<keyType, dataType>::del(keyType deleteKey)
	{
		if (this->Root == nullptr) // stop if empty
			return;
		this->Root = recDelete(this->Root, deleteKey);
	}

	template <typename keyType, class dataType>
	void RNDtree<keyType, dataType>::buildFromStack(SJstack<dataPair> &pairStack)
	{
		dataPair tmpPair;
		while (!pairStack.empty())
		{
			tmpPair = pairStack.sPop();
			add(tmpPair.first, tmpPair.second);
		}
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::maxHeight(rNode * subTree, size_t &etalon, size_t curr)
	{
		if (subTree == nullptr)
		{
			if (etalon < curr)
				etalon = curr;
			return;
		}
		curr++;
		maxHeight(subTree->_left, etalon, curr);
		maxHeight(subTree->_right, etalon, curr);
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::resize(rNode * subTree)
	{
		subTree->_size= getSize(subTree->_left) + getSize(subTree->_right) + 1;
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::recDelete(rNode * subTree)
	{
		if (subTree == nullptr)
			return;
		recDelete(subTree->_left);
		recDelete(subTree->_right);
	}

	template <typename keyType, class dataType>
	void RNDtree<keyType, dataType>::traversePostOrder(rNode *subTree, SJstack<rNode> &treeStack)
	{
		if (subTree != nullptr)
		{
			traversePostOrder(subTree->_left, treeStack);
			traversePostOrder(subTree->_right, treeStack);
			treeStack.sPush(*subTree);
		}
		else
			return;
	}

	template <typename keyType, class dataType>
	void RNDtree<keyType, dataType>::traversePostOrder(rNode *subTree, SJstack<rNode> &treeStack, keyType deleteKey)
	{
		if (subTree != nullptr)
		{
			traversePostOrder(subTree->_left, treeStack, deleteKey);
			traversePostOrder(subTree->_right, treeStack, deleteKey);
			if (subTree->key != deleteKey)
				treeStack.sPush(*subTree);
		}
		else
			return;
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::BE_printPostOrder(rNode * subTree)
	{
		if (subTree != nullptr)
		{
			BE_printPostOrder(subTree->_left);
			BE_printPostOrder(subTree->_right);
			cout << subTree->key << ' '; // only key because there can be no << operator for dataType
		}
		else
			return;
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::BE_printInOrder(rNode * subTree)
	{
		if (subTree != nullptr)
		{
			BE_printInOrder(subTree->_left);
			cout << subTree->key << ' '; // only key because there can be no << operator for dataType
			BE_printInOrder(subTree->_right);
		}
		else
			return;
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::BE_printPreOrder(rNode * subTree)
	{
		if (subTree != nullptr)
		{
			cout << subTree->key << ' '; // only key because there can be no << operator for dataType
			BE_printPreOrder(subTree->_left);
			BE_printPreOrder(subTree->_right);
		}
		else
			return;
	}

	template<typename keyType, class dataType>
	inline rNode * RNDtree<keyType, dataType>::rotateRight(rNode * subTree)
	{
		rNode* newRoot = subTree->_left;
		if (newRoot == nullptr)
			return subTree;

		subTree->_left = newRoot->_right;
		newRoot->_right = subTree;
		newRoot->_size = subTree->_size;
		resize(subTree);
		return newRoot;
	}

	template<typename keyType, class dataType>
	inline rNode * RNDtree<keyType, dataType>::rotateLeft(rNode * subTree)
	{

		rNode* newRoot = subTree->_right;
		if (newRoot == nullptr)
			return subTree;

		subTree->_right = newRoot->_left;
		newRoot->_left = subTree;
		newRoot->_size = subTree->_size;
		resize(subTree);
		return newRoot;

	}

	template<typename keyType, class dataType>
	inline rNode * RNDtree<keyType, dataType>::simpleInsert(rNode * subTree, keyType addKey, dataType addElement)
	{
		if (subTree == nullptr)
		{
			rNode* newNode = new rNode;
			newNode->key = addKey;
			newNode->data = addElement;
			return newNode;
		}
		if (subTree->key > addKey)
			subTree->_left = simpleInsert(subTree->_left, addKey, addElement);
		else
			subTree->_right = simpleInsert(subTree->_right, addKey, addElement);
		resize(subTree);
		return subTree;
	}

	template<typename keyType, class dataType>
	inline rNode * RNDtree<keyType, dataType>::inserToRoot(rNode * subTree, keyType addKey, dataType addElement)
	{
		if (subTree == nullptr)
		{
			rNode* newNode = new rNode;
			newNode->key = addKey;
			newNode->data = addElement;
			return newNode;
		}
		if (addKey < subTree->key)
		{
			subTree->_left = inserToRoot(subTree->_left, addKey, addElement);
			return rotateRight(subTree);
		}
		else
		{
			subTree->_right = inserToRoot(subTree->_right, addKey, addElement);
			return rotateLeft(subTree);
		}
	}

	template<typename keyType, class dataType>
	inline rNode * RNDtree<keyType, dataType>::rndInsert(rNode * subTree, keyType addKey, dataType addElement)
	{
		if (subTree == nullptr)
		{
			rNode* newNode = new rNode;
			newNode->key = addKey;
			newNode->data = addElement;
			return newNode;
		}
		if (rand() % (subTree->_size + 1) == 0)
			return inserToRoot(subTree, addKey, addElement);
		if (subTree->key > addKey)
			subTree->_left = simpleInsert(subTree->_left, addKey, addElement);
		else
			subTree->_right = simpleInsert(subTree->_right, addKey, addElement);
		resize(subTree);
		return subTree;
	}

	template <typename keyType, class dataType>
	void RNDtree<keyType, dataType>::clear()
	{
		recDelete(this->Root);
		this->Root = nullptr;
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::printPostOrder()
	{
		BE_printPostOrder(this->Root);
		cout << '\n';
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::printInOrder()
	{
		BE_printInOrder(this->Root);
		cout << '\n';
	}

	template<typename keyType, class dataType>
	inline void RNDtree<keyType, dataType>::printPreOrder()
	{
		BE_printPreOrder(this->Root);
		cout << '\n';
	}
}


#endif // !RNDtree_H_