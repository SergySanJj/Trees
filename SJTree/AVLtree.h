#pragma once

#ifndef AVLtree_H_
#define AVLtree_H_

#include "SJstack.h" // my version of stack class

namespace AVLTree
{
	using namespace std;

	template <typename keyType, class dataType>
	class SearchNode
	{
	public:
		keyType  key;  // standart type key so we can do operations between them like <,>,==
		dataType data; // so any type data can be stored
		SearchNode<keyType, dataType> *_left = nullptr;   // left sub-tree
		SearchNode<keyType, dataType> *_right = nullptr;  // right sub-tree
		unsigned char _height;
		SearchNode() { key = keyType(); data = dataType(); }

		~SearchNode() { _left = nullptr; _right = nullptr; }

		SearchNode(keyType key, dataType data)  // copy constructor
		{
			this->key = key;
			this->data = data;
			this->_height = 1;
		}

		SearchNode<keyType, dataType> operator = (const SearchNode<keyType, dataType> &searchNode2)
		{
			return (SearchNode<keyType, dataType>(this->key = searchNode2.key, this->data = searchNode2.data));
		}
	};


#define sAVLTree    AVLtree<keyType, dataType>
#define sNode    SearchNode<keyType, dataType>
#define dataPair pair<keyType, dataType>

	template <typename keyType, class dataType>
	class AVLtree
	{
	public:
		AVLtree();
		~AVLtree();

		sNode* getRoot();
		void add(keyType addKey, dataType addElement);          // Interface to add element.
		bool find(keyType searchKey, dataType &resultVariable); // Search first element with searchKey key, return 1 and change resultVariable if such element was found,
																//return 0 and don't change resultVariable if such element doesn't exist.

		void buildFromStack(SJstack<dataPair> &pairStack);      // Add new row of elements from stack to the tree.
		void del(keyType deleteKey);                            // Delete closest to root element with deleteKey key.
		void clear();                                           // Delete all nodes from tree.
		void printPostOrder();                                  // Wrapper function for backend print.
		void printInOrder();
		void printPreOrder();


	private:
		//Data//
		sNode * Root = nullptr;

		//Methods//

		void recDelete(sNode *subTree);                                                       // recursive deletting all inheritor nodes, used as destructor
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack);                    // traverse tree in post-order and add each node into stack
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack, keyType deleteKey); // modification that won't include elements with set key into the stack
		void BE_printPostOrder(sNode *subTree);
		void BE_printInOrder(sNode *subTree);
		void BE_printPreOrder(sNode *subTree);

		sNode *leftMost(sNode *curNode);
		sNode *rightMost(sNode *curNode);

		sNode* rotateRight(sNode* subTree);                                    // Tree rotation to the right.
		sNode* rotateLeft(sNode* subTree);                                     // Tree rotation to the left.

		sNode* recInsert(keyType addKey, dataType addElement, sNode *subTree); // recursive adding of element to the sub-tree by rule: go left 
																			   //if key<=currentKey, otherwise go right

		sNode* rightLeast(sNode* subTree);                                     // returns least node of right son's sub-tree
		sNode* delLeast(sNode* subTree);

		sNode* recDelete(sNode* subNode, keyType val);                         // deletes element with deleteKey could be found from chosen node
		
		unsigned char height(sNode* subNode);
		int bFactor(sNode* subNode);                                           // diff between right and left son's heights
		void fixHeight(sNode* subNode);
		sNode* balance(sNode* subNode);
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
	inline sNode * AVLtree<keyType, dataType>::getRoot()
	{
		return this->Root;
	}

	template <typename keyType, class dataType>
	void AVLtree<keyType, dataType>::add(keyType addKey, dataType addElement)
	{
		this->Root = recInsert(addKey, addElement, this->Root);
	}

	template <typename keyType, class dataType>
	void AVLtree<keyType, dataType>::recDelete(sNode *subTree)
	{
		if (subTree != nullptr)
		{
			recDelete(subTree->_left);
			recDelete(subTree->_right);
			delete subTree;
		}
	}

	template <typename keyType, class dataType>
	bool AVLtree<keyType, dataType>::find(keyType searchKey, dataType &resultVariable) // ln(n) travers search using pre-order search
	{
		if (this->Root == nullptr) // stop if empty
			return 0;

		sNode *pCurNode = this->Root;
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
	void AVLtree<keyType, dataType>::traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack)
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
	void AVLtree<keyType, dataType>::traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack, keyType deleteKey)
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
	inline void AVLtree<keyType, dataType>::BE_printPostOrder(sNode * subTree)
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
	inline void AVLtree<keyType, dataType>::BE_printInOrder(sNode * subTree)
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
	inline void AVLtree<keyType, dataType>::BE_printPreOrder(sNode * subTree)
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
	}

	template<typename keyType, class dataType>
	sNode* AVLtree<keyType, dataType>::rotateRight(sNode* subTree)
	{
		if (subTree == nullptr)
			return nullptr;
		else
		{
			//if (subTree->_left == nullptr)
				//return nullptr;

			sNode* newRoot = subTree->_left;

			subTree->_left = newRoot->_right;

			newRoot->_right = subTree;

			fixHeight(newRoot);

			return newRoot;
		}
	}

	template<typename keyType, class dataType>
	sNode* AVLtree<keyType, dataType>::rotateLeft(sNode* subTree)
	{
		if (subTree == nullptr)
			return nullptr;
		else
		{
			//if (subTree->_right == nullptr)
				//return nullptr;

			sNode* newRoot = subTree->_right;

			subTree->_right = newRoot->_left;

			newRoot->_left = subTree;

			fixHeight(subTree);
			fixHeight(newRoot);

			return newRoot;
		}
	}

	template<typename keyType, class dataType>
	inline sNode * AVLtree<keyType, dataType>::recInsert(keyType addKey, dataType addElement, sNode * subTree)
	{
		if (subTree == nullptr) 
			return new sNode(addKey, addElement);

		if (addKey < subTree->key)
			subTree->_left = recInsert(addKey, addElement, subTree->_left);
		else
			subTree->_right = recInsert(addKey, addElement, subTree->_right);
		return balance(subTree);
	}

	template<typename keyType, class dataType>
	inline sNode * AVLtree<keyType, dataType>::rightLeast(sNode * subTree)
	{
		if (subTree->_left == nullptr)
			return subTree;
		else
			return rightLeast(subTree->_left);
	}

	template<typename keyType, class dataType>
	inline sNode * AVLtree<keyType, dataType>::delLeast(sNode * subTree)
	{
		if (subTree->_left == nullptr)
			return subTree->_right;
		subTree->_left = delLeast(subTree->_left);
		return balance(subTree);
	}

	template<typename keyType, class dataType>
	inline sNode * AVLtree<keyType, dataType>::recDelete(sNode * subNode, keyType val)
	{
		if (subNode == nullptr)
			return nullptr;
		if (val < subNode->key)
			subNode->_left = recDelete(subNode->_left, val);
		else if (val > subNode->key)
			subNode->_right = recDelete(subNode->_right, val);

		else //  found
		{
			sNode* leftSon = subNode->_left;
			sNode* rightSon = subNode->_right;
			delete subNode;
			if (rightSon == nullptr) 
				return leftSon;
			sNode* min_ = rightLeast(rightSon);
			min_->_right = delLeast(rightSon);
			min_->_left = leftSon;
			return balance(min_);
		}

		return balance(subNode);
	}

	template<typename keyType, class dataType>
	inline unsigned char AVLtree<keyType, dataType>::height(sNode * subNode)
	{
		if (subNode != nullptr)
			return subNode->_height;
		else
			return 0;
	}

	template<typename keyType, class dataType>
	inline int AVLtree<keyType, dataType>::bFactor(sNode * subNode)
	{
		return (height(subNode->_right) - height(subNode->_left));
	}

	template<typename keyType, class dataType>
	inline void AVLtree<keyType, dataType>::fixHeight(sNode * subNode)
	{
		unsigned char hleft = height(subNode->_left);
		unsigned char hright = height(subNode->_right);

		if (hleft > hright)
			subNode->_height = hleft + 1;
		else
			subNode->_height = hright + 1;
	}

	template<typename keyType, class dataType>
	inline sNode * AVLtree<keyType, dataType>::balance(sNode * subNode)
	{
		fixHeight(subNode);
		if (bFactor(subNode) == 2)
		{
			if (bFactor(subNode->_right) < 0)
				subNode->_right = rotateRight(subNode->_right);
			return rotateLeft(subNode);
		}
		if (bFactor(subNode) == -2)
		{
			if (bFactor(subNode->_left) > 0)
				subNode->_left = rotateLeft(subNode->_left);
			return rotateRight(subNode);
		}
		return subNode;
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
	sNode * AVLtree<keyType, dataType>::leftMost(sNode *curNode)
	{
		if (curNode == nullptr)
			return nullptr;
		if (curNode->_left != nullptr) {
			return leftMost(curNode->_left);
		}
		return curNode;
	}

	template <typename keyType, class dataType>
	sNode * AVLtree<keyType, dataType>::rightMost(sNode *curNode)
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