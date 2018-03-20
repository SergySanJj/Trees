#include <iostream>
#include <Windows.h>
#include "SJtree.h"
#include "SJstack.h"

using namespace Tree;

int main()
{
	SearchTree<int, int> tree;
	SJstack< pair<int, int> > stackk;
	int a = 0;
	int b;
	while (a != -1)
	{
		cin >> a >> b;
		if (a == -1)
			break;
		stackk.sPush(pair<int, int>(a, b));
	}
	//stackk.sPush(pair<int, int>(2, 29));
	cout << '\n';
	tree.buildFromStack(stackk);

	tree.del(15);

	tree.printPostOrder();
	tree.printInOrder();
	tree.printPreOrder();
	cout << '\n';
	while (tree.rotateRight())
	{
		tree.printPostOrder();
		tree.printInOrder();
		tree.printPreOrder();
		cout << '\n';
	}
	//stackk.sPush(pair<int, int>(1, 8));
	system("pause");
	return 0;
}
// examples :
/*
50 1
25 1
65 1
1 0
28 100
5 4 
8 0
10 4
50 2
50 1
-1 -1
*/