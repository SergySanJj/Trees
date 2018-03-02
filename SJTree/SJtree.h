#pragma once


namespace Tree
{
	template <typename keyType, class dataType>
	struct searchNode
	{
		keyType  key;  // standart type key so we can doo operations between them like <,>,==
		dataType data; // so any type data can be stored 
		searchNode< keyType, dataType > *pLeft = nullptr;  // left sub-tree
		searchNode< keyType, dataType > *pRight = nullptr; // right sub-tree
	};


	template <typename keyType, class dataType>
	class searchTree
	{
	public:
		searchTree();
		~searchTree();

		void add(dataType addElement);
	private:
		//Data//
		searchNode< keyType, dataType > *pRoot = NULL;

		//Methods//


	};

	template <typename keyType, class dataType>
	searchTree::searchTree()
	{

	}

	template <typename keyType, class dataType>
	searchTree::~searchTree()
	{

	}

	template <typename keyType, class dataType>
	void searchTree::add(dataType addElementd)
	{

	}
}