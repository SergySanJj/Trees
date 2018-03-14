#pragma once

#ifndef SJtree_H_
#define SJtree_H_

#include "SJstack.h" // my version of stack class

namespace Tree
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
		SearchNode<keyType, dataType> *_parent = nullptr; // parent of current node, if this node is root => _parent must stay nullptr
		char _side = 'n';                                 // l - is left son, r - is right son, n - is not a son

		SearchNode()
		{
			key = keyType();
			data = dataType();
		}

		~SearchNode()
		{
			this->_left = nullptr;
			this->_right = nullptr;
			this->_parent = nullptr;
		}

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

#define sTree SearchTree<keyType, dataType>
#define sNode SearchNode<keyType, dataType>
#define dataPair pair<keyType, dataType>

	template <typename keyType, class dataType>
	class SearchTree
	{
	public:
		SearchTree();
		~SearchTree();

		void add(keyType addKey, dataType addElement);          // interface to add element
		bool find(keyType searchKey, dataType &resultVariable); // search first element with searchKey key, return 1 and change resultVariable if such element was found,
																//return 0 and don't change resultVariable if such element doesn't exist
		
		void buildFromStack(SJstack<dataPair> &pairStack);      // add new row of elements from stack to the tree
		void del(keyType deleteKey);                            // delete first element with deleteKey key
		void clear();                                           // delete all nodes from tree
		bool rotateRight();                                     // tree rotation to the right
		bool rotateLeft();                                      // tree rotation to the left
		void printPostOrder();                                  // wrapper function for backend print
		void printInOrder();
		
	private:
		//Data//
		sNode *Root = nullptr;

		//Methods//
		bool cmp(keyType addKey, sNode *subTree);                                             // 0 == left, 1 == right, subTree is a sub-tree on chosen level
		void recAdd(keyType addKey, dataType addElement, sNode *subTree);                     // recursive adding of element to the sub-tree by rule: go left if key<=currentKey, otherwise go right
		void recDelete(sNode *subTree);                                                       // recursive deletting all inheritor nodes, used as destructor
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack);                    // traverse tree in post-order and add each node into stack
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack, keyType deleteKey); // modification that won't include elements with set key into the stack
		void BE_printPostOrder(sNode *subTree);
		void delElement(sNode *curNode, keyType deleteKey);									  // delets element with deleteKey could be found from chosen node

		void traverseInOrder(sNode *subTree);

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

	template <typename keyType, class dataType>
	void SearchTree<keyType, dataType>::add(keyType addKey, dataType addElement)
	{
		if (this->Root == nullptr)
		{
			sNode *newElem = new sNode();
			newElem->data = addElement;
			newElem->key = addKey;
			newElem->_side = 'n';
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
				subTree->_right->_side = 'r';
				subTree->_right->_parent = subTree;
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
			subTree->_left->_side = 'l';
			subTree->_left->_parent = subTree;
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
			if (subTree->_side == 'l')
				subTree->_parent->_left = nullptr;
			else if (subTree->_side == 'r')
				subTree->_parent->_right = nullptr;
			else if (subTree->_side == 'n')
			{
				if (subTree->_left != nullptr)
				{
					this->Root = subTree->_left;
				}
				else if (subTree->_right != nullptr)
				{
					this->Root = subTree->_right;
				}
				else
				{
					this->Root = nullptr;
				}
			}
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

		delElement(this->Root, deleteKey);
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

	template <typename keyType, class dataType>
	void SearchTree<keyType, dataType>::clear()
	{
		recDelete(this->Root);
	}

	template<typename keyType, class dataType>
	inline bool SearchTree<keyType, dataType>::rotateRight()
	{
		if (this->Root == nullptr)
			return 0;
		else
		{
			if (this->Root->_left == nullptr)
				return 0;

			sNode* newRoot = this->Root->_left;
			newRoot->_parent = nullptr;
			newRoot->_side = 'n';
			
			this->Root->_left = newRoot->_right;
			if (newRoot->_right != nullptr)
			{
				newRoot->_right->_side = 'l';
				newRoot->_right->_parent = this->Root;
			}
			
			newRoot->_right = this->Root;
			this->Root->_parent = newRoot;
			this->Root->_side = 'r';

			this->Root = newRoot;

			return 1;
		}
	}

	template<typename keyType, class dataType>
	inline bool SearchTree<keyType, dataType>::rotateLeft()
	{
		if (this->Root == nullptr)
			return 0;
		else
		{
			if (this->Root->_right == nullptr)
				return 0;

			sNode* newRoot = this->Root->_right;
			newRoot->_parent = nullptr;
			newRoot->_side = 'n';

			this->Root->_right = newRoot->_left;
			if (newRoot->_left != nullptr)
			{
				newRoot->_left->_side = 'r';
				newRoot->_left->_parent = this->Root;
			}
			newRoot->_left = this->Root;
			this->Root->_parent = newRoot;
			this->Root->_side = 'l';

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
		traverseInOrder(this->Root);
		cout << '\n';
	}

	template<typename keyType, class dataType>
	inline void SearchTree<keyType, dataType>::traverseInOrder(sNode * subTree)
	{
		if (subTree->_left == nullptr)
		{
			cout << subTree->key << ' ';
			return;
		}
		traverseInOrder(subTree->_left);
		cout << subTree->key << ' ';
		if (subTree->_right == nullptr)
		{
			cout << subTree->_right->key << ' ';
		}
	}

	template <typename keyType, class dataType>
	void SearchTree<keyType, dataType>::delElement(sNode *curNode, keyType deleteKey)
	{
		if (curNode == nullptr)
			return;

		if (deleteKey < curNode->key)
			return delElement(curNode->_left, deleteKey);
		else if (deleteKey > curNode->key)
			return delElement(curNode->_right, deleteKey);
		else {
			if (curNode->_left == nullptr && curNode->_right == nullptr) {
				if (curNode->_parent->_left == curNode)
					curNode->_parent->_left = nullptr;
				else
					curNode->_parent->_right = nullptr;
				delete curNode;
			}
			else {
				sNode * newnode = nullptr;
				if (curNode->_left != nullptr)
				{
					newnode = rightMost(curNode->_left);
				}
				else
					newnode = leftMost(curNode->_right);

				if (curNode->_parent->_left == curNode)
					curNode->_parent->_left = newnode;
				else
					curNode->_parent->_right = newnode;

				newnode->_parent = curNode->_parent;
				newnode->_right = curNode->_right;
				newnode->_left = curNode->_left;

				delete curNode;
			}
		}
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