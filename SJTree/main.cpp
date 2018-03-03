#include <iostream>
#include "SJtree.h"

using namespace Tree;

int main()
{
	searchTree<int, int> tree;
	//tree.add(1, 1);
	tree.add(2, 32);
	tree.add(1, 22);
	//tree.add(33, 2);
	//tree.clear();
	
	//tree.del(2);
	//tree.del(33);
	while (true)
	{
		int a, b = 0;
		cin >> a;
		cout << tree.find(a, b) << '\n';
	}
	system("pause");
	return 0;
}