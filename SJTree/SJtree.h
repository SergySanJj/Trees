#pragma once
#include "SJstack.h" // my version of stack class

namespace Tree
{
	using namespace std;

	template <typename keyType, class dataType>
	struct searchNode
	{
		keyType  key;  // standart type key so we can do operations between them like <,>,==
		dataType data; // so any type data can be stored
		searchNode<keyType, dataType> *pLeft = nullptr;  // left sub-tree
		searchNode<keyType, dataType> *pRight = nullptr; // right sub-tree

		searchNode()
		{
		}

		~searchNode()
		{

		}

		searchNode(keyType key, dataType data)  // copy constructor
		{
			this->key = key;
			this->data = data;
		}

		searchNode<keyType, dataType> operator = (const searchNode<keyType, dataType> &searchNode2)
		{
			
			//searchNode<keyType, dataType> *newNode = new searchNode<keyType, dataType>();
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
		void del(keyType deleteKey);                            // delete all elements with deleteKey key
		void clear();                                           // delete all nodes from tree
	private:
		//Data//
		sNode *pRoot = nullptr;

		//Methods//
		bool cmp(keyType addKey, sNode *subTree);                                         // 0 == left, 1 == right, subTree is a sub-tree on chosen level
		void recAdd(keyType addKey, dataType addElement, sNode *subTree);                 // recursive adding of element to the sub-tree by rule: go left if key<=currentKey, otherwise go right
		void recDelete(sNode *subTree);                                                   // recursive deletting all inheritor nodes, used as destructor
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack);                // traverse tree in post-order and add each node into stack
		void buildFromStack(SJstack<sNode> nodeStack);                                    // add new row of elements from stack
		void traversePostOrder(sNode *subTree, SJstack<sNode> &treeStack, keyType deleteKey); // modification that won't include elements with set key into the stack
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
		if (this->pRoot == nullptr)
		{
			sNode *newElem = new sNode();
			newElem->data = addElement;
			newElem->key = addKey;
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
			if (subTree->pRight == nullptr)
			{
				subTree->pRight = new sNode();
				subTree->pRight->key = addKey;
				subTree->pRight->data = addElement;
				return;
			}
			recAdd(addKey, addElement, subTree->pRight);
			return;
		}
		if (subTree->pLeft == nullptr)
		{
			subTree->pLeft = new sNode();
			subTree->pLeft->key = addKey;
			subTree->pLeft->data = addElement;
			return;
		}
		recAdd(addKey, addElement, subTree->pLeft);
	}

	template <typename keyType, class dataType>
	void searchTree<keyType, dataType>::recDelete(sNode *subTree)
	{
		/*if (subTree->pLeft == nullptr && subTree->pRight == nullptr)
		{
			delete subTree;
			return;
		}
		if (subTree->pLeft!=nullptr)
			recDelete(subTree->pLeft);
		if (subTree->pRight != nullptr)
			recDelete(subTree->pRight);*/
		if (subTree != nullptr)
		{
			recDelete(subTree->pLeft);
			recDelete(subTree->pRight);
			delete subTree;
			subTree = nullptr;
		}
	}

	template <typename keyType, class dataType>
	bool searchTree<keyType, dataType>::find(keyType searchKey, dataType &resultVariable) // ln(n) travers search using pre-order search
	{
		sNode *pCurNode = this->pRoot;
		while (searchKey != pCurNode->key && (pCurNode->pLeft != nullptr || pCurNode->pRight != nullptr))
		{
			if (searchKey < pCurNode->key)
			{
				if (pCurNode->pLeft != nullptr)
				{
					pCurNode = pCurNode->pLeft;
					continue;
				}
				else
					return 0;
			}
			else
			{
				if (pCurNode->pRight != nullptr)
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
		sNode *pCurNode = this->pRoot;
		while (deleteKey != pCurNode->key && (pCurNode->pLeft != nullptr || pCurNode->pRight != nullptr))
		{
			if (deleteKey < pCurNode->key)
			{
				if (pCurNode->pLeft != nullptr)
				{
					pCurNode = pCurNode->pLeft;
					continue;
				}
				else
					return;
			}
			else
			{
				if (pCurNode->pRight != nullptr)
				{
					pCurNode = pCurNode->pRight;
					continue;
				}
				else
					return;
			}
		}
		if (pCurNode->key == deleteKey)
		{
			SJstack<sNode> treeStack;
			traversePostOrder(pCurNode, treeStack, deleteKey);

			recDelete(pCurNode);
			buildFromStack(treeStack);
		}
		else
			return;
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
		if (subTree != nullptr)
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
		if (subTree != nullptr)
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
	}
}