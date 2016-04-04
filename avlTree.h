#ifndef AVL_TREE
#define AVL_TREE
#include <list>
#include <iostream>

using namespace std;
// 用宏来避免函数调用。
#define HEIGHT(p) ((p == NULL) ? -1 : (((Node<T> *) (p))->height))      // 树高
#define MAX(a, b) (((a) > (b))? (a):(b))                                               // 最大值
/*
 * 树高度的更新？
 * 旋转的时机？
 */

// 树节点
template <class T>
struct  Node {
    T val;
    int height;
    struct Node *left;
    struct Node *right;
};

// 创建节点
template <class T>
Node<T>* create_node(T val) {
    Node<T> *node = new struct Node<T>;
    node->val = val;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// AVL树实现
template <class T>
class AVLTree {
private:
    // 根节点
    Node<T> *TreeRoot;

    void right_rotate(Node<T>*& root);       // 左左，右旋
    void left_rotate(Node<T>*& root);                                   // 右右，左旋
    void left_right_rotate(Node<T>*& root);                          // 左右，左旋，右旋
    void right_left_rotate(Node<T>*& root);                          // 右左，右旋，左旋
    bool inner_insert(Node<T>*& root, T val);
    void inner_preorder(Node<T>*, list<T>&);
    void inner_insert(Node<T>*, list<T>&);
    void inner_posorder(Node<T>*, list<T>&);
    void destory_tree(Node<T>*);
    bool inner_remove(T, Node<T>*&);

public:
    // 初始化一个空AVL树
    AVLTree():TreeRoot(NULL) {}

    // 创建带根节点值的AVL树
    AVLTree(T val):TreeRoot(create_node(val)) {}

    // 析构函数
    ~AVLTree() {destory_tree(TreeRoot);}

    bool insert(T val);
    bool remove(T val);
    
    const Node<T>* get_max();
    const Node<T>* get_min();
    bool query(T val);
    int tree_height();

    void preorder_traversal(list<T>&);
    void inorder_traversal(list<T>&);
    void posorder_traversal(list<T>&);
    void empty_tree() {destory_tree(TreeRoot);}
};

#endif

/**********************************
 * 旋转操作 START
 **********************************/
// 左子节点上提
template <class T>
void AVLTree<T>::right_rotate(Node<T>*& root) {
    Node<T> *left = root->left;

    root->left = left->right;
    left->right = root;

    // 旋转后更新高度
    root->height = MAX(HEIGHT(root->left), HEIGHT(root->right)) + 1;
    left->height = MAX(HEIGHT(left->left), HEIGHT(left->right)) + 1;
    root = left;
}

// 右子节点上提
template <class T>
void AVLTree<T>::left_rotate(Node<T>*& root) {
    Node<T> *right = root->right;

    root->right = right->left;
    right->left = root;
    
    root->height = MAX(HEIGHT(root->left), HEIGHT(root->right)) + 1;
    right->height = MAX(HEIGHT(right->left), HEIGHT(right->right)) + 1;
    root = right;
}

// 左旋，右旋
template <class T>
void AVLTree<T>::left_right_rotate(Node<T>*& root) {
    left_rotate(root->left);
    return right_rotate(root);
}

// 右旋，左旋
template <class T>
void AVLTree<T>::right_left_rotate(Node<T>*& root) {
    right_rotate(root->right);
    return left_rotate(root);
}
/**********************************
 * 旋转操作 END
 **********************************/



/**********************************
 * 查询操作 START
 **********************************/
template <class T>
bool AVLTree<T>::query(T val) {
    AVLTree *root = TreeRoot;

    while (root != NULL) {
        if (val == root->val) return true;
        else if (val < root->val) root = root->left;
        else root = root->right;
    }
    return false;
}

/* 最大值 */
template <class T>
const Node<T>* AVLTree<T>::get_max() {
    const Node<T> *node = TreeRoot;
    if (node == NULL) return node;
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

/* 最小值 */
template <class T>
const Node<T>* AVLTree<T>::get_min() {
    const Node<T> *node = TreeRoot;
    if (node == NULL) return node;
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/* 深度查询 */
template <class T>
int AVLTree<T>::tree_height() {return HEIGHT(TreeRoot);}
/**********************************
 * 查询操作 END
 **********************************/



/**********************************
 * 插入操作 START
 **********************************/
// 公开的插入接口
template <class T>
bool AVLTree<T>::insert(T val) {
    if (TreeRoot == NULL) {
        TreeRoot = create_node(val);
        return true;
    } else {
        return inner_insert(TreeRoot, val);
    }
}


// 插入的内部实现
// 确保@root的值不是NULL
template <class T>
bool AVLTree<T>::inner_insert(Node<T>*& root, T val) {
    bool ret;

    if (root->val == val) {
        return 0;
    } else if (val < root->val) {           // 插入到左子树
        if (root->left == NULL) {           // 左子树不存在，创建左节点并赋值，左子树深度为0，不可能失衡
            root->left = create_node(val);
            ret = true;                                // 返回值
        } else {                                    // 插入到存在的左子树
            ret = inner_insert(root->left, val);
            // 插入到左子树，只有dis==2这一个破坏平衡的可能
            // 注意到左节点在这里必然存在
            if (HEIGHT(root->left) - HEIGHT(root->right) == 2) {
                // 插入左节点左边导致失衡，右旋
                if (val < root->left->val) right_rotate(root);

                // 插入左节点右边导致失衡，左旋，右旋
                else left_right_rotate(root);
            }
        }
    } else {              // root->val < val，插入到右子树
        if (root->right == NULL) {
            root->right = create_node(val);
            ret = true;
        } else {
            ret = inner_insert(root->right, val);

            // 插入到右子树，只有dis==-2这一个破坏平衡的可能
            // 注意到右节点在这里必然存在
            if (HEIGHT(root->left) - HEIGHT(root->right) == -2) {
                // 插入右节点右边导致失衡，左旋
                if (val > root->right->val) left_rotate(root);

                // 插入右节点左边导致失衡，右旋，左旋
                else right_left_rotate(root);
            }
        }
    }

    // 更新高度
    root->height = MAX(HEIGHT(root->left), HEIGHT(root->right))  + 1;
    return ret;
}
/**********************************
 * 插入操作 END
 **********************************/



/**********************************
 * 删除操作 START
 **********************************/
// 外部接口
template <class T>
bool AVLTree<T>::remove(T val) {
    return inner_remove(val, TreeRoot);
 }

// 内部实现
template <class T>
bool AVLTree<T>::inner_remove(T val, Node<T>*& root) {
    if (root == NULL) return false;              // 删除不存在的节点

    bool ret;
    // 当前节点是目标删除节点
    if (root->val == val) {
        ret = true;
        /* 当两边子树都为空时,直接删除节点 */
        if (root->left == NULL && root->right == NULL) {
            delete root;
            root = NULL;
        } else {
        /* 这里选择较高子树中的一个最值替换根节点，然后删除原最值所在节点
         *  通过取巧的方式来保证这一步删除不会造成当前根节点失衡
         */
            if (HEIGHT(root->left) > HEIGHT(root->right)) {                 // 左子树较高
                // 选择左子树的最大值替换根，保证根总是大于左节点，小于右节点
                Node<T> *max = root->left;
                while (max->right != NULL)
                    max = max->right;
                
                root->val = max->val;                               // 用最值替换根的值

                // 交换目标值到最值位置
                // 将最值位置删除后二叉树会重新恢复平衡
                // 随后以左子树为根，删除目标值，注意到左子树此时是满足二叉树的约束的。可以按正常方式删除val。
                max->val = val;
                inner_remove(val, root->left);                  // 这里确认是不会导致当前根节点失衡的。
            } else {                                                        // 右子树较高
                // 选择右子树的最小值替换根。
                Node<T> *min = root->right;
                while (min->left != NULL) min = min->left;

                root->val = min->val;
                min->val = val;
                inner_remove(val, root->right);
            }
        }
    } else if (val < root->val) {                       // 目标删除节点在左子树上,只有dis==-2会破坏平衡
        ret = inner_remove(val, root->left);

        if (HEIGHT(root->left) - HEIGHT(root->right) == -2) {
            // 注意到高度差为-2时，右子树根节点必然存在（高度至少为1吧？），对其进行访问是合法的。
            if (HEIGHT(root->right->left) > HEIGHT(root->right->right)) {
                // 等同右左情况
                left_right_rotate(root);
            } else {
                // 类似右右情况
                left_rotate(root);
            }
        }
    } else {
        // val > root->val 目标节点在右子树上，删除后可能导致右子树变矮，只有dis==2会破坏平衡
        ret = inner_remove(val, root->right);

        if (HEIGHT(root->left) - HEIGHT(root->right) == 2) {
            // 这里注意到当左子树的左右子树相等时，直接把左子树的根节点提上来才能保证平衡
            if (HEIGHT(root->left->left) >= HEIGHT(root->left->right)) {
                // 等同左左情况
                right_rotate(root);
            } else {
                // 类似左右情况
                right_left_rotate(root);
            }
        }
    }

    // 更新高度
    if (root) root->height = MAX(HEIGHT(root->left), HEIGHT(root->right))+1;

    return ret;
}
/**********************************
 * 删除操作 END
 **********************************/

/**********************************
 * 遍历操作 START
 **********************************/

/* START 先序遍历 */
// 外部接口
template <class T>
void AVLTree<T>::preorder_traversal(list<T>& mylist) {
    inner_preorder(TreeRoot, mylist);
}

// 内部实现
template <class T>
void AVLTree<T>::inner_preorder(Node<T>* root, list<T>& mylist) {
    if (root == NULL) return;

    mylist.push_back(root->val);
    inner_preorder(root->left, mylist);
    inner_preorder(root->right, mylist);
}
/* END 先序遍历 */


/* START 中序遍历 */
 // 接口
template <class T>
void AVLTree<T>::inorder_traversal(list<T>& mylist) {
    inner_inorder(TreeRoot, mylist);
}

// 内部实现
template <class T>
void inner_inorder(Node<T>* root, list<T>& mylist) {
    if (root == NULL) return;
    inner_inorder(root->left, mylist);
    mylist.push_back(root->val);
    inner_inorder(root->right, mylist);
}
/* END 中序遍历 */


/* START 后序遍历 */
// 接口
template <class T>
void AVLTree<T>::posorder_traversal(list<T>& mylist) {
    inner_posorder(TreeRoot, mylist);
}

template <class T>
void AVLTree<T>::inner_posorder(Node<T>* root, list<T>& mylist) {
    if (root == NULL) return;
    inner_posorder(root->left, mylist);
    inner_posorder(root->right, mylist);
    mylist.push_back(root->val);
}

/* END 后序遍历 */
 /**********************************
 * 遍历操作 END
 **********************************/


 /**********************************
 * 清空树 START
 **********************************/
template <class T>
 void AVLTree<T>::destory_tree(Node<T>* root) {
    if (root == NULL) return;
    destory_tree(root->left);
    destory_tree(root->right);
    delete root;
    root = NULL;
 }
  /**********************************
 * 清空树 END
 **********************************/