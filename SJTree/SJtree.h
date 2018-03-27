#pragma once

#ifndef SJtree_H_
#define SJtree_H_

#include "SJstack.h" // my version of stack class

namespace Trees
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

		SearchNode() { key = keyType(); data = dataType(); }

		~SearchNode() { _left = nullptr; _right = nullptr; }

		SearchNode(keyType key, dataType data)  // copy constructor
		{
			this->key = key;
			this->data = data;
		}

		SearchNode<keyType, dataType> operator = (const SearchNode<keyType, dataType> &searchNode2)
		{
			return (SearchNode<keyType, dataType>(this->key = searchNode2.key, this->data = searchNode2.data));
		}
	};

#define sTree    SearchTree<keyType, dataType>
#define sNode    SearchNode<keyType, dataType>
#define dataPair pair<keyType, dataType>

	template <typename keyType, class dataType>
	class SearchTree
	{
	public:
		SearchTree();
		~SearchTree();

		sNode* getRoot();
		void add(keyType addKey, dataType addElement);          // Interface to add element.
		bool find(keyType searchKey, dataType &resultVariable); // Search first element with searchKey key, return 1 and change resultVariable if such element was found,
																//return 0 and don't change resultVariable if such element doesn't exist.
		
		void buildFromStack(SJstack<dataPair> &pairStack);      // Add new row of elements from stack to the tree.
		void del(keyType deleteKey);                            // Delete closest to root element with deleteKey key.
		void clear();                                           // Delete all nodes from tree.
		bool rotateRight(sNode* subTree);                                     // Tree rotation to the right.
		bool rotateLeft(sNode* subTree);                                      // Tree rotation to the left.
		void printPostOrder();                                  // Wrapper function for backend print.
		void printInOrder();
		void printPreOrder();
		

	private:
		//Data//
		sNode *Root = nullptr;

		//Methods//
		bool cmp(keyType addKey, sNode *subTree);                                             // 0 == left, 1 == right, subTree is a sub-tree on chosen level
		void recAdd(keyType addKey, dataType addElement, sNode *subTree);                     // recursive adding of element to the sub-tree by rule: go left 
		                                                                                      //if key<=currentKey, otherwise go right
		void recDelete(sNode *subTree);                                                       // recursive deletting all inheritor nodes, used as destructor
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack);                    // traverse tree in post-order and add each node into stack
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack, keyType deleteKey); // modification that won't include elements with set key into the stack
		void BE_printPostOrder(sNode *subTree);
		void BE_printInOrder(sNode *subTree);
		void BE_printPreOrder(sNode *subTree);
		sNode* DeleteNode(sNode* subNode, dataType val);                                      // delets element with deleteKey could be found from chosen node

		sNode *leftMost(sNode *curNode);
		sNode *rightMost(sNode *curNode);
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename keyType, class dataType>
	SearchTree<keyType, dataType>::SearchTree()
	{
	}

	template <typename keyType, class dataType>
	SearchTree<keyType, dataType>::~SearchTree()
	{
		recDelete(this->Root);
	}

	template<typename keyType, class dataType>
	inline sNode * SearchTree<keyType, dataType>::getRoot()
	{
		return this->Root;
	}

	template <typename keyType, class dataType>
	void SearchTree<keyType, dataType>::add(keyType addKey, dataType addElement)
	{
		if (this->Root == nullptr)
		{
			sNode *newElem = new sNode();
			newElem->data = addElement;
			newElem->key = addKey;
			this->Root = newElem;
			return;
		}
		recAdd(addKey, addElement, this->Root);
	}

	template <typename keyType, class dataType>
	bool SearchTree<keyType, dataType>::cmp(keyType addKey, sNode *subTree)
	{
		return (addKey > subTree->key);
	}

	template <typename keyType, class dataType>
	void SearchTree<keyType, dataType>::recAdd(keyType addKey, dataType addElement, sNode *subTree)
	{
		if (cmp(addKey, subTree))
		{
			if (subTree->_right == nullptr)
			{
				subTree->_right = new sNode();
				subTree->_right->key = addKey;
				subTree->_right->data = addElement;
				return;
			}
			recAdd(addKey, addElement, subTree->_right);
			return;
		}
		if (subTree->_left == nullptr)
		{
			subTree->_left = new sNode();
			subTree->_left->key = addKey;
			subTree->_left->data = addElement;
			return;
		}
		recAdd(addKey, addElement, subTree->_left);
	}

	template <typename keyType, class dataType>
	void SearchTree<keyType, dataType>::recDelete(sNode *subTree)
	{
		if (subTree != nullptr)
		{
			recDelete(subTree->_left);
			recDelete(subTree->_right);
			delete subTree;
		}
	}

	template <typename keyType, class dataType>
	bool SearchTree<keyType, dataType>::find(keyType searchKey, dataType &resultVariable) // ln(n) travers search using pre-order search
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
	void SearchTree<keyType, dataType>::del(keyType deleteKey)
	{
		if (this->Root == nullptr) // stop if empty
			return;

		DeleteNode(this->Root, deleteKey);
	}

	template <typename keyType, class dataType>
	void SearchTree<keyType, dataType>::buildFromStack(SJstack<dataPair> &pairStack)
	{
		dataPair tmpPair;
		while (!pairStack.empty())
		{
			tmpPair = pairStack.sPop();
			add(tmpPair.first, tmpPair.second);
		}
	}

	template <typename keyType, class dataType>
	void SearchTree<keyType, dataType>::traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack)
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
	void SearchTree<keyType, dataType>::traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack, keyType deleteKey)
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
	inline void SearchTree<keyType, dataType>::BE_printPostOrder(sNode * subTree)
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
	inline void SearchTree<keyType, dataType>::BE_printInOrder(sNode * subTree)
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
	inline void SearchTree<keyType, dataType>::BE_printPreOrder(sNode * subTree)
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
	void SearchTree<keyType, dataType>::clear()
	{
		recDelete(this->Root);
	}

	template<typename keyType, class dataType>
	inline bool SearchTree<keyType, dataType>::rotateRight(sNode* subTree)
	{
		if (subTree == nullptr)
			return 0;
		else
		{
			if (subTree->_left == nullptr)
				return 0;

			sNode* newRoot = subTree->_left;

			
			subTree->_left = newRoot->_right;
			
			newRoot->_right = subTree;

			if (this->Root == subTree)
				this->Root = newRoot;

			return 1;
		}
	}

	template<typename keyType, class dataType>
	inline bool SearchTree<keyType, dataType>::rotateLeft(sNode* subTree)
	{
		if (subTree == nullptr)
			return 0;
		else
		{
			if (subTree->_right == nullptr)
				return 0;

			sNode* newRoot = subTree->_right;

			subTree->_right = newRoot->_left;

			newRoot->_left = this->Root;

			if (this->Root == subTree)
				this->Root = newRoot;

			return 1;
		}
	}

	template<typename keyType, class dataType>
	inline void SearchTree<keyType, dataType>::printPostOrder()
	{
		BE_printPostOrder(this->Root);
		cout << '\n';
	}

	template<typename keyType, class dataType>
	inline void SearchTree<keyType, dataType>::printInOrder()
	{
		BE_printInOrder(this->Root);
		cout << '\n';
	}

	template<typename keyType, class dataType>
	inline void SearchTree<keyType, dataType>::printPreOrder()
	{
		BE_printPreOrder(this->Root);
		cout << '\n';
	}


	template <typename keyType, class dataType>
	sNode* SearchTree<keyType, dataType>::DeleteNode(sNode* subNode, dataType val) {
		if (subNode == nullptr)
			return subNode;
		//If element is at the root of subTree & has 2 sons -> 
		//replace it with min el of right sub-tree and del min el
		if (val == subNode->key) {

			sNode* tmp;
			if (subNode->_right == nullptr)
				tmp = subNode->_left;
			else {

				sNode* ptr = subNode->_right;
				if (ptr->_left == nullptr) {
					ptr->_left = subNode->_left;
					tmp = ptr;
				}
				else {

					sNode* pmin = leftMost(ptr->_left);

					ptr->_left = pmin->_right;
					pmin->_left = subNode->_left;
					pmin->_right = subNode->_right;

					tmp = pmin;
				}
			}

			delete subNode;
			return tmp;
		}
		else if (val < subNode->key)
			subNode->_left = DeleteNode(subNode->_left, val);
		else
			subNode->_right = DeleteNode(subNode->_right, val);
		return subNode;
	}

	template <typename keyType, class dataType>
	sNode * SearchTree<keyType, dataType>::leftMost(sNode *curNode) 
	{
		if (curNode == nullptr)
			return nullptr;
		if (curNode->_left != nullptr) {
			return leftMost(curNode->_left);
		}
		return curNode;
	}

	template <typename keyType, class dataType>
	sNode * SearchTree<keyType, dataType>::rightMost(sNode *curNode)
	{
		if (curNode == nullptr)
			return nullptr;
		if (curNode->_right != nullptr) {
			return rightMost(curNode->_right);
		}
		return curNode;
	}
}


#endif // !SJtree_H_