#include "AVLTree.h"
#include <iostream>
using namespace std;

template <typename TYPE>
int cmp(const TYPE& item1, const TYPE& item2)
{
	if (item1 < item2)
		return -1;
	if (item1 == item2)
		return 0;
	return 1;
}

int main()
{
    AVLTree<int> testTree(cmp);
    cout << "----------Testing insert()----------" << endl;
    cout << "insert: 50, 90, 100 (right-right case)" << endl;
    testTree.insert(50);
    testTree.insert(90);
    testTree.insert(100);
    testTree.printTree();
    cout << "insert: 40, 30 (left-left case)" << endl;
    testTree.insert(40);
    testTree.insert(30);
    testTree.printTree();
    cout << "insert: 95, 96 (left-right case)" << endl;
    testTree.insert(95);
    testTree.insert(96);
    testTree.printTree();
    cout << "insert: 110, 105 (right-left case)" << endl;
    testTree.insert(110);
    testTree.insert(105);
    testTree.printTree();
    cout << "insert: 110 (duplicate case)" << endl;
    testTree.insert(110);

    cout << "----------Testing remove()----------" << endl;
    cout << "Note: adding 20, 35 to buff tree" << endl;
    testTree.insert(20);
    testTree.insert(35);
    testTree.printTree();
    cout << "remove: 50 (left-left case)" << endl;
    testTree.remove(50);
    testTree.printTree();
    cout << "remove: 100, 95 (right-right case)" << endl;
    testTree.remove(100);
    testTree.remove(95);
    testTree.printTree();
    cout << "remove: 20 (right-left case)" << endl;
    testTree.remove(20);
    testTree.printTree();
    cout << "remove: 30, 96, 110, 105 (left-right case)" << endl;
    testTree.remove(30);
    testTree.remove(96);
    testTree.remove(110);
    testTree.remove(105);
    testTree.printTree();
    cout << "remove: 30 (non-existent node case)" << endl;
    testTree.remove(30);

    cout << "----------Testing search()----------" << endl;
    cout << "search(35) returns: " << testTree.search(35) << endl;
    cout << "search(100) (non-existent node case) returns: " << testTree.search(100) << endl;

    cout << "----------Testing count()----------" << endl;
    cout << "count() returns: " << testTree.count() << endl;

    return 0;
}
