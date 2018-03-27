#include <iostream>
#include <random>
#include <Windows.h>
#include "SJtree.h"
#include "SJstack.h"
#include "AVLtree.h"
#include "RNDtree.h"

using namespace Trees;

int main()
{
	RNDtree<int, int> rnd_tree;
	AVLtree<int, int> avl_tree;
	
	SJstack< pair<int, int> > stackk;
	std::random_device rd;
	std::mt19937 mt(rd());
	uniform_int_distribution<int> d{ -20'000,20'000 };

	for (int i = 0; i < 20; i++)
	{
		avl_tree.clear();
		rnd_tree.clear();
		for (int j = 0; j < 500; j++)
		{
			int a = d(mt);
			int b = a;
			avl_tree.add(a, b);
			rnd_tree.add(a, b);
		}
		//avl_tree.printPostOrder();
		//rnd_tree.printPostOrder();
		cout << avl_tree.getHeight() << " " << rnd_tree.getHeight() << "\n";
	}

	/*
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
	}*/

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