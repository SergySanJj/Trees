#include <iostream>
#include <Windows.h>
#include "SJtree.h"
#include "SJstack.h"
#include "AVLtree.h"

using namespace AVLTree;

int main()
{
	AVLtree<int, int> tree;
	SJstack< pair<int, int> > stackk;
	int a = 0;
	int b;
	char c= '0';
	while (c != 'E')
	{
		cin >> c;
		if (c == '1')
		{
			cin >> a >> b;
			tree.add(a, b);
		}
		else if (c == '0')
		{
			cin >> a;
			tree.del(a);
		}
		tree.printInOrder();
	}
	//stackk.sPush(pair<int, int>(2, 29));
	//cout << '\n';
	//tree.buildFromStack(stackk);
	
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
15 6
8 0
15 10
10 4
50 2
50 1
-1 -1
*/