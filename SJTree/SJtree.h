#pragma once
#include "SJstack.h" // my version of stack class

namespace Tree
{
	using namespace std;

	template <typename keyType, class dataType>
	struct searchNode
	{
	public:
		keyType  key;  // standart type key so we can do operations between them like <,>,==
		dataType data; // so any type data can be stored
		searchNode<keyType, dataType> *pLeft = NULL;  // left sub-tree
		searchNode<keyType, dataType> *pRight = NULL; // right sub-tree
		searchNode<keyType, dataType> *pParent = NULL;// parent of current node, if this node is root => pParent must stay NULL
		char side = 'n';                              // l - is left son, r - is right son, n - is not a son

		searchNode()
		{
			key = keyType();
			data = dataType();
		}

		~searchNode()
		{
			this->pLeft = NULL;
			this->pRight = NULL;
			this->pParent = NULL;
		}

		searchNode(keyType key, dataType data)  // copy constructor
		{
			this->key = key;
			this->data = data;
		}

		searchNode<keyType, dataType> operator = (const searchNode<keyType, dataType> &searchNode2)
		{
			return searchNode<keyType, dataType>(this->key = searchNode2.key, this->data = searchNode2.data);
		}
	};

#define sTree searchTree<keyType, dataType>
#define sNode searchNode<keyType, dataType>

	template <typename keyType, class dataType>
	class searchTree
	{
	public:
		searchTree();
		~searchTree();

		void add(keyType addKey, dataType addElement);           // interface to add element
		bool find(keyType searchKey, dataType &resultVariable); // search first element with searchKey key, return 1 and change resultVariable if such element was found,
																//return 0 and don't change resultVariable if such element doesn't exist
		void del(keyType deleteKey);                            // delete first element with deleteKey key
		void clear();                                           // delete all nodes from tree
	private:
		//Data//
		sNode *pRoot = NULL;

		//Methods//
		bool cmp(keyType addKey, sNode *subTree);                                             // 0 == left, 1 == right, subTree is a sub-tree on chosen level
		void recAdd(keyType addKey, dataType addElement, sNode *subTree);                     // recursive adding of element to the sub-tree by rule: go left if key<=currentKey, otherwise go right
		void recDelete(sNode *subTree);                                                       // recursive deletting all inheritor nodes, used as destructor
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack);                    // traverse tree in post-order and add each node into stack
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack, keyType deleteKey); // modification that won't include elements with set key into the stack
		void buildFromStack(SJstack<sNode> nodeStack);                                        // add new row of elements from stack to the tree

		void delElement(sNode *curNode, keyType deleteKey);									  // delets element with deleteKey could be found frome chosen node

		sNode *leftMost(sNode *curNode);
		sNode *rightMost(sNode *curNode);
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename keyType, class dataType>
	searchTree<keyType, dataType>::searchTree()
	{
	}

	template <typename keyType, class dataType>
	searchTree<keyType, dataType>::~searchTree()
	{
		recDelete(this->pRoot);
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::add(keyType addKey, dataType addElement)
	{
		if (this->pRoot == NULL)
		{
			sNode *newElem = new sNode();
			newElem->data = addElement;
			newElem->key = addKey;
			newElem->side = 'n';
			this->pRoot = newElem;
			return;
		}
		recAdd(addKey, addElement, this->pRoot);
	}

	template <typename keyType, class dataType>
	bool searchTree<keyType, dataType>::cmp(keyType addKey, sNode *subTree)
	{
		return (addKey > subTree->key);
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::recAdd(keyType addKey, dataType addElement, sNode *subTree)
	{
		if (cmp(addKey, subTree))
		{
			if (subTree->pRight == NULL)
			{
				subTree->pRight = new sNode();
				subTree->pRight->key = addKey;
				subTree->pRight->data = addElement;
				subTree->pRight->side = 'r';
				subTree->pRight->pParent = subTree;
				return;
			}
			recAdd(addKey, addElement, subTree->pRight);
			return;
		}
		if (subTree->pLeft == NULL)
		{
			subTree->pLeft = new sNode();
			subTree->pLeft->key = addKey;
			subTree->pLeft->data = addElement;
			subTree->pLeft->side = 'l';
			subTree->pLeft->pParent = subTree;
			return;
		}
		recAdd(addKey, addElement, subTree->pLeft);
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::recDelete(sNode *subTree)
	{
		if (subTree != NULL)
		{
			recDelete(subTree->pLeft);
			recDelete(subTree->pRight);
			if (subTree->side == 'l')
				subTree->pParent->pLeft = NULL;
			else if (subTree->side == 'r')
				subTree->pParent->pRight = NULL;
			else if (subTree->side == 'n')
			{
				if (subTree->pLeft != NULL)
				{
					this->pRoot = subTree->pLeft;
				}
				else if (subTree->pRight != NULL)
				{
					this->pRoot = subTree->pRight;
				}
				else
				{
					this->pRoot = NULL;
				}
			}
			delete subTree;
		}
	}

	template <typename keyType, class dataType>
	bool searchTree<keyType, dataType>::find(keyType searchKey, dataType &resultVariable) // ln(n) travers search using pre-order search
	{
		if (this->pRoot == NULL) // stop if empty
			return 0;

		sNode *pCurNode = this->pRoot;
		while (searchKey != pCurNode->key && (pCurNode->pLeft != NULL || pCurNode->pRight != NULL))
		{
			if (searchKey < pCurNode->key)
			{
				if (pCurNode->pLeft != NULL)
				{
					pCurNode = pCurNode->pLeft;
					continue;
				}
				else
					return 0;
			}
			else
			{
				if (pCurNode->pRight != NULL)
				{
					pCurNode = pCurNode->pRight;
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
	void searchTree<keyType, dataType>::del(keyType deleteKey)
	{
		if (this->pRoot == NULL) // stop if empty
			return;

		delElement(this->pRoot, deleteKey);
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::buildFromStack(SJstack<sNode> nodeStack)
	{
		sNode tmpRes;
		while (nodeStack.watch(tmpRes))
		{
			add(tmpRes.key, tmpRes.data);
			nodeStack.sPop();
		}
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack)
	{
		if (subTree != NULL)
		{
			traversePostOrder(subTree->pLeft, treeStack);
			traversePostOrder(subTree->pRight, treeStack);
			treeStack.sPush(*subTree);
		}
		else
			return;
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack, keyType deleteKey)
	{
		if (subTree != NULL)
		{
			traversePostOrder(subTree->pLeft, treeStack, deleteKey);
			traversePostOrder(subTree->pRight, treeStack, deleteKey);
			if (subTree->key != deleteKey)
				treeStack.sPush(*subTree);
		}
		else
			return;
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::clear()
	{
		recDelete(this->pRoot);
		//this->pRoot = NULL;
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::delElement(sNode *curNode, keyType deleteKey)
	{
		if (curNode == NULL)
			return;

		if (deleteKey < curNode->key)
			return delElement(curNode->pLeft, deleteKey);
		else if (deleteKey > curNode->key)
			return delElement(curNode->pRight, deleteKey);
		else {
			if (curNode->pLeft == NULL && curNode->pRight == NULL) {
				if (curNode->pParent->pLeft == curNode)
					curNode->pParent->pLeft = NULL;
				else
					curNode->pParent->pRight = NULL;
				delete curNode;
			}
			else {
				sNode * newnode = NULL;
				if (curNode->pLeft != NULL) {
					newnode = rightMost(curNode->pLeft);
				}
				else
					newnode = leftMost(curNode->pRight);

				if (curNode->pParent->pLeft == curNode)
					curNode->pParent->pLeft = newnode;
				else
					curNode->pParent->pRight = newnode;

				newnode->pParent = curNode->pParent;
				newnode->pRight = curNode->pRight;
				newnode->pLeft = curNode->pLeft;

				delete curNode;
			}
		}
	}

	template <typename keyType, class dataType>
	sNode * searchTree<keyType, dataType>::leftMost(sNode *curNode) 
	{
		if (curNode == NULL)
			return NULL;
		if (curNode->pLeft != NULL) {
			return leftMost(curNode->pLeft);
		}
		return curNode;
	}

	template <typename keyType, class dataType>
	sNode * searchTree<keyType, dataType>::rightMost(sNode *curNode)
	{
		if (curNode == NULL)
			return NULL;
		if (curNode->pRight != NULL) {
			return rightMost(curNode->pRight);
		}
		return curNode;
	}
}
