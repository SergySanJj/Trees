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
		sNode *pRoot = NULL;

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

		sNode *pCurNode = this->pRoot;
		if (this->pRoot->key != deleteKey)
		{
			while (deleteKey != pCurNode->key && (pCurNode->pLeft != NULL || pCurNode->pRight != NULL))
			{
				if (deleteKey < pCurNode->key)
				{
					if (pCurNode->pLeft != NULL)
					{
						pCurNode = pCurNode->pLeft;
						continue;
					}
					else
						return;
				}
				else
				{
					if (pCurNode->pRight != NULL)
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
		SJstack<sNode> treeStack;
		
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
}

/*
void Tree::remove(Node *node, Node *parent)//удаление узла
{
if (node->rightPtr == 0 && node->leftPtr == 0)//если это лист
{
if (parent == 0)//если он единственный в дереве
root = 0;//дерево пустое
else if (parent->leftPtr == node)//иначе если родитель есть и наш узел стоит слева от родителя
parent->leftPtr = 0;//мы его выкидываем
else
parent->rightPtr = 0;//если был справа тоже выкидываем
}
else if (node->rightPtr == 0)//если наш узел имеет только потомка слева
{
if (parent == 0)//если это корень дерева
root = node->leftPtr;//потомок слева становится корнем
else if (parent->leftPtr == node)//иначе если наш узел расположен слева у своего родителя
parent->leftPtr = node->leftPtr;//потомок узла становится на его место
else
parent->rightPtr = node->leftPtr;//значит узел находится справа у родителя, удаляем его
}
else if (node->leftPtr == 0)//аналогично
{
if (parent == 0)
root = node->rightPtr;
else if (parent->leftPtr == node)
parent->leftPtr = node->rightPtr;
else
parent->rightPtr = node->rightPtr;
}
else //у него есть 2 потомка
{
Node *c, *p = node;//2 указателя на удаляемый узел
for (c = node->rightPtr; c->leftPtr != NULL; c = c->leftPtr) p = c;//ищем в правой ветви крайний левый узел
if (p != node)
{
p->leftPtr = c->rightPtr;
c->rightPtr = node->rightPtr;
}
c->leftPtr = node->leftPtr;
if (parent == 0)//если это случай, когда узел -это корень
root = c;//просто крайний левый ставим в корень
else if (parent->leftPtr == node)//если есть родитель, то теперь родитель указывает слева на крайний левый узел
parent->leftPtr = c;
else
parent->rightPtr = c;//аналогично
}
delete node;//до этого расставляли верно указатели, что бы не разрушить структуру дерева, а сейчас удаляем узел.
*/