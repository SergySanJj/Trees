#pragma once

#ifndef AVLtree_H_
#define AVLtree_H_

#include "SJstack.h" // my version of stack class

namespace Trees
{
	using namespace std;

	template <typename keyType, class dataType>
	class AVLNode
	{
	public:
		keyType  key;  // standart type key so we can do operations between them like <,>,==
		dataType data; // so any type data can be stored
		AVLNode<keyType, dataType> *_left = nullptr;   // left sub-tree
		AVLNode<keyType, dataType> *_right = nullptr;  // right sub-tree
		unsigned char _height;
		AVLNode() { key = keyType(); data = dataType(); }

		~AVLNode() { _left = nullptr; _right = nullptr; }

		AVLNode(keyType key, dataType data)  // copy constructor
		{
			this->key = key;
			this->data = data;
			this->_height = 1;
		}

		AVLNode<keyType, dataType> operator = (const AVLNode<keyType, dataType> &AVLNode2)
		{
			return (AVLNode<keyType, dataType>(this->key = AVLNode2.key, this->data = AVLNode2.data));
		}
	};


#define sAVLTree    AVLtree<keyType, dataType>
#define aNode    AVLNode<keyType, dataType>
#define dataPair pair<keyType, dataType>

	template <typename keyType, class dataType>
	class AVLtree
	{
	public:
		AVLtree();
		~AVLtree();

		aNode* getRoot();
		size_t getHeight();
		void add(keyType addKey, dataType addElement);          // Interface to add element.
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
		aNode * Root = nullptr;

		//Methods//

		void recDelete(aNode *subTree);                                                       // recursive deletting all inheritor nodes, used as destructor
		void traversePostOrder(aNode *subTree, SJstack<aNode> &treeStack);                    // traverse tree in post-order and add each node into stack
		void traversePostOrder(aNode *subTree, SJstack<aNode> &treeStack, keyType deleteKey); // modification that won't include elements with set key into the stack
		void BE_printPostOrder(aNode *subTree);
		void BE_printInOrder(aNode *subTree);
		void BE_printPreOrder(aNode *subTree);

		aNode *leftMost(aNode *curNode);
		aNode *rightMost(aNode *curNode);

		aNode* rotateRight(aNode* subTree);                                    // Tree rotation to the right.
		aNode* rotateLeft(aNode* subTree);                                     // Tree rotation to the left.

		aNode* recInsert(keyType addKey, dataType addElement, aNode *subTree); // recursive adding of element to the sub-tree by rule: go left 
																			   //if key<=currentKey, otherwise go right

		aNode* rightLeast(aNode* subTree);                                     // returns least node of right son's sub-tree
		aNode* delLeast(aNode* subTree);

		aNode* recDelete(aNode* subTree, keyType val);                         // deletes element with deleteKey could be found from chosen node
		
		unsigned char height(aNode* subTree);
		int bFactor(aNode* subTree);                                           // diff between right and left son's heights
		void normalizeHeight(aNode* subTree);
		aNode* balance(aNode* subTree);
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename keyType, class dataType>
	AVLtree<keyType, dataType>::AVLtree()
	{
	}

	template <typename keyType, class dataType>
	AVLtree<keyType, dataType>::~AVLtree()
	{
		recDelete(this->Root);
	}

	template<typename keyType, class dataType>
	inline aNode * AVLtree<keyType, dataType>::getRoot()
	{
		return this->Root;
	}

	template<typename keyType, class dataType>
	inline size_t AVLtree<keyType, dataType>::getHeight()
	{
		if (this->Root == nullptr)
			return 0;
		return this->Root->_height;
	}

	template <typename keyType, class dataType>
	void AVLtree<keyType, dataType>::add(keyType addKey, dataType addElement)
	{
		this->Root = recInsert(addKey, addElement, this->Root);
	}

	template <typename keyType, class dataType>
	void AVLtree<keyType, dataType>::recDelete(aNode *subTree)
	{
		if (subTree != nullptr)
		{
			recDelete(subTree->_left);
			recDelete(subTree->_right);
			balance(subTree);
			delete subTree;
		}
	}

	template <typename keyType, class dataType>
	bool AVLtree<keyType, dataType>::find(keyType searchKey, dataType &resultVariable) // ln(n) travers search using pre-order search
	{
		if (this->Root == nullptr) // stop if empty
			return 0;

		aNode *pCurNode = this->Root;
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
	void AVLtree<keyType, dataType>::del(keyType deleteKey)
	{
		if (this->Root == nullptr) // stop if empty
			return;
		this->Root = recDelete(this->Root, deleteKey);
	}

	template <typename keyType, class dataType>
	void AVLtree<keyType, dataType>::buildFromStack(SJstack<dataPair> &pairStack)
	{
		dataPair tmpPair;
		while (!pairStack.empty())
		{
			tmpPair = pairStack.sPop();
			add(tmpPair.first, tmpPair.second);
		}
	}

	template <typename keyType, class dataType>
	void AVLtree<keyType, dataType>::traversePostOrder(aNode *subTree, SJstack<aNode> &treeStack)
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
	void AVLtree<keyType, dataType>::traversePostOrder(aNode *subTree, SJstack<aNode> &treeStack, keyType deleteKey)
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
	inline void AVLtree<keyType, dataType>::BE_printPostOrder(aNode * subTree)
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
	inline void AVLtree<keyType, dataType>::BE_printInOrder(aNode * subTree)
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
	inline void AVLtree<keyType, dataType>::BE_printPreOrder(aNode * subTree)
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


	template <typename keyType, class dataType>
	void AVLtree<keyType, dataType>::clear()
	{
		recDelete(this->Root);
		this->Root = nullptr;
	}

	template<typename keyType, class dataType>
	aNode* AVLtree<keyType, dataType>::rotateRight(aNode* subTree)
	{
		if (subTree == nullptr)
			return nullptr;
		else
		{
			//if (subTree->_left == nullptr)
				//return nullptr;

			aNode* newRoot = subTree->_left; // new sub-tree root

			subTree->_left = newRoot->_right;

			newRoot->_right = subTree;

			normalizeHeight(subTree);
			normalizeHeight(newRoot);

			return newRoot;
		}
	}

	template<typename keyType, class dataType>
	aNode* AVLtree<keyType, dataType>::rotateLeft(aNode* subTree)
	{
		if (subTree == nullptr)
			return nullptr;
		else
		{
			//if (subTree->_right == nullptr)
				//return nullptr;

			aNode* newRoot = subTree->_right;

			subTree->_right = newRoot->_left;

			newRoot->_left = subTree;

			normalizeHeight(subTree);
			normalizeHeight(newRoot);

			return newRoot;
		}
	}

	template<typename keyType, class dataType>
	inline aNode * AVLtree<keyType, dataType>::recInsert(keyType addKey, dataType addElement, aNode * subTree)
	{
		if (subTree == nullptr) 
			return new aNode(addKey, addElement);

		if (addKey < subTree->key)
			subTree->_left = recInsert(addKey, addElement, subTree->_left);
		else
			subTree->_right = recInsert(addKey, addElement, subTree->_right);
		return balance(subTree);
	}

	template<typename keyType, class dataType>
	inline aNode * AVLtree<keyType, dataType>::rightLeast(aNode * subTree)
	{
		if (subTree->_left == nullptr)
			return subTree;
		else
			return rightLeast(subTree->_left);
	}

	template<typename keyType, class dataType>
	inline aNode * AVLtree<keyType, dataType>::delLeast(aNode * subTree)
	{
		if (subTree->_left == nullptr)
			return subTree->_right;
		subTree->_left = delLeast(subTree->_left);
		return balance(subTree);
	}

	template<typename keyType, class dataType>
	inline aNode * AVLtree<keyType, dataType>::recDelete(aNode * subTree, keyType val)
	{
		if (subTree == nullptr)
			return nullptr;
		if (val < subTree->key)
			subTree->_left = recDelete(subTree->_left, val);
		else if (val > subTree->key)
			subTree->_right = recDelete(subTree->_right, val);

		else //  found
		{
			aNode* leftSon = subTree->_left;
			aNode* rightSon = subTree->_right;
			delete subTree;
			if (rightSon == nullptr) 
				return leftSon;
			aNode* min_ = rightLeast(rightSon);
			min_->_right = delLeast(rightSon);
			min_->_left = leftSon;
			return balance(min_);
		}

		return balance(subTree);
	}

	template<typename keyType, class dataType>
	unsigned char AVLtree<keyType, dataType>::height(aNode * subTree)
	{
		if (subTree != nullptr)
			return subTree->_height;
		else
			return 0;
	}

	template<typename keyType, class dataType>
	inline int AVLtree<keyType, dataType>::bFactor(aNode * subTree)
	{
		return (height(subTree->_right) - height(subTree->_left));
	}

	template<typename keyType, class dataType>
	inline void AVLtree<keyType, dataType>::normalizeHeight(aNode * subTree)
	{
		unsigned char hleft = 0;
		unsigned char hright = 0;
		if (subTree->_left != nullptr)
			hleft = height(subTree->_left);
		if (subTree->_right != nullptr)
			hright = height(subTree->_right);

		if (hleft > hright)
			subTree->_height = hleft + 1;
		else
			subTree->_height = hright + 1;
	}

	template<typename keyType, class dataType>
	inline aNode * AVLtree<keyType, dataType>::balance(aNode * subTree)
	{
		normalizeHeight(subTree);
		if (bFactor(subTree) == 2)
		{
			if (bFactor(subTree->_right) < 0)
				subTree->_right = rotateRight(subTree->_right); // double
			return rotateLeft(subTree);                         // single
		}
		if (bFactor(subTree) == -2)
		{
			if (bFactor(subTree->_left) > 0)
				subTree->_left = rotateLeft(subTree->_left);   // double
			return rotateRight(subTree);                       // single
		}
		return subTree;
	}

	template<typename keyType, class dataType>
	inline void AVLtree<keyType, dataType>::printPostOrder()
	{
		BE_printPostOrder(this->Root);
		cout << '\n';
	}

	template<typename keyType, class dataType>
	inline void AVLtree<keyType, dataType>::printInOrder()
	{
		BE_printInOrder(this->Root);
		cout << '\n';
	}

	template<typename keyType, class dataType>
	inline void AVLtree<keyType, dataType>::printPreOrder()
	{
		BE_printPreOrder(this->Root);
		cout << '\n';
	}

	template <typename keyType, class dataType>
	aNode * AVLtree<keyType, dataType>::leftMost(aNode *curNode)
	{
		if (curNode == nullptr)
			return nullptr;
		if (curNode->_left != nullptr) {
			return leftMost(curNode->_left);
		}
		return curNode;
	}

	template <typename keyType, class dataType>
	aNode * AVLtree<keyType, dataType>::rightMost(aNode *curNode)
	{
		if (curNode == nullptr)
			return nullptr;
		if (curNode->_right != nullptr) {
			return rightMost(curNode->_right);
		}
		return curNode;
	}
}


#endif // !AVLTree_H_