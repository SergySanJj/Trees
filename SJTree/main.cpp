#include <iostream>
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
	tree.printPostOrder();
	while (tree.rotateRight())
	{
		tree.printPostOrder();
	}
	stackk.sPush(pair<int, int>(1, 8));
	system("pause");
	return 0;
}