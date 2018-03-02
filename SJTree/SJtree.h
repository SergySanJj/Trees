#pragma once

using namespace std;

namespace Tree
{


	template <typename keyType, class dataType>
	struct searchNode
	{
		keyType  key;  // standart type key so we can doo operations between them like <,>,==
		dataType data; // so any type data can be stored 
		searchNode<keyType, dataType> *pLeft = nullptr;  // left sub-tree
		searchNode<keyType, dataType> *pRight = nullptr; // right sub-tree
	};

#define sTree searchTree<keyType, dataType>
#define sNode searchNode<keyType, dataType>

	template <typename keyType, class dataType>
	class searchTree
	{
	public:
		searchTree();
		~searchTree();

		void add(keyType addKey,dataType addElement); // interface to add element
	private:
		//Data//
		sNode *pRoot = nullptr;

		//Methods//
		bool cmp(keyType addKey, sNode *subTree);   // 0 == left, 1 == right, subTree is a sub-tree on chosen level
		void recAdd(keyType addKey, dataType addElement, sNode *subTree);// recursive adding of element to the sub-tree 
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename keyType, class dataType>
	searchTree<keyType, dataType>::searchTree()
	{

	}

	template <typename keyType, class dataType>
	searchTree<keyType, dataType>::~searchTree()
	{

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


}