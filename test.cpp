#include <iostream>
#include "avlTree.h"
#include <list>

using namespace std;

void print_list(list<int>& lis) {
    list<int>::iterator it = lis.begin();
    while (it != lis.end()) {
        cout << *it++ << " ";
    }
    cout << endl;
}

void tra_3_orders(AVLTree<int>& tree1) {
    cout << "preorder ";
    list<int> preorder;
    tree1.preorder_traversal(preorder);
    print_list(preorder);

    cout << "inorder  ";
    list<int> inorder;
    tree1.inorder_traversal(inorder);
    print_list(inorder);

    cout << "posorder ";
    list<int> posorder;
    tree1.posorder_traversal(posorder);
    print_list(posorder);
    cout << "******" << endl;
}

int main() {
    AVLTree<int> tree1;
    AVLTree<int> tree2(3);

    int arr[] = {3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9};
    int k, size = sizeof(arr)/sizeof(int);
    for (k = 0; k < size; k++) {
        cout << "insert: " << arr[k] << " result: " << tree1.insert(arr[k]) << endl;
    }
    cout << "insert: " << 16 << " result: " << tree1.insert(16) << endl;
    cout << "max: "<< tree1.get_max()->val << endl;
    cout << "min: " << tree1.get_min()->val << endl;

    tra_3_orders(tree1);

    tree1.remove(9);
    tra_3_orders(tree1);

    cout << "tree height "<< tree1.tree_height() << endl;
    tree1.empty_tree();
    cout << "tree height "<< tree1.tree_height() << endl;
    return 0;
}
