// Author : B133040043 鄭凱謚
// Date : Nov. 12, 2025
// Purpose : 給予一串數字，找出所有出現次數為奇數之數字

#include<iostream>
#include<vector>
using namespace std;

struct Term{
    int c_data;
    int l_data;
    int r_data;
};

class BST;

class TreeNode{
    public:
        TreeNode();
        TreeNode(int c);
        friend class BST;
    private:
        TreeNode *leftchild, *rightchild, *parent;
        int data;
};

TreeNode::TreeNode():leftchild(nullptr),rightchild(nullptr),parent(nullptr){}

TreeNode::TreeNode(int c):leftchild(nullptr),rightchild(nullptr),parent(nullptr){data = c;}

// 實作 Binary Search Tree
class BST{
    public:
        BST();
        TreeNode* Search(int val);  // 查詢 val 是否存在於 Tree 中
        void Insert(int val);   // 插入 val 到 Tree 中
        void Delete(int val);   // 刪除 Tree 中值為 val 的 Node
        void Travel();  // 以中序遍歷整顆 Tree（順序即為小到大）
    private:
        TreeNode *root;
        TreeNode* Leftmost(TreeNode *current);  // 尋找以 current 為 root 的 subtree 中，最左邊的 Node
        TreeNode* Successor(TreeNode *current); // 尋找比 current 大的下一個值的 Node
};

BST::BST():root(nullptr){}

TreeNode* BST::Leftmost(TreeNode *current){
    while(current->leftchild)
        current = current->leftchild;
    return current;
}

// 以中序遍歷的規則尋找 Successor
TreeNode* BST::Successor(TreeNode *current){ 
    if(current->rightchild)
        return Leftmost(current->rightchild);

    TreeNode *s = current->parent;
    while(s && current == s->rightchild){
        current = s;
        s = s->parent;
    }
    return s;
}

// 由 root 往下找到指定 val
TreeNode* BST::Search(int val){
    TreeNode *current = root;

    while(current && val != current->data){
        if(val < current->data)
            current = current->leftchild;
        else
            current = current->rightchild;
    }

    return current;
}

void BST::Insert(int val){
    // c 為插入點，p 為他的 parent
    TreeNode *c = nullptr, *p = nullptr;

    // 由 root 開始往下找，當 c 為 nullptr 時代表找到插入點
    c = root;
    while(c){
        p = c;
        if(val < c->data)
            c = c->leftchild;
        else if(val > c->data)
            c = c->rightchild;
        else{   // 在這題中，遇到相同的數值就必須刪除此節點
            Delete(val);
            return;
        }
    }

    TreeNode *insert_node = new TreeNode(val);
    insert_node->parent = p;

    if(p == nullptr)    // 插入點為 root
        this->root = insert_node;
    else if(val < p->data)  // 較 p 值小，插入至左邊
        p->leftchild = insert_node;
    else if(val > p->data)  // 較 p 值大，插入至右邊
        p->rightchild = insert_node;
    return;
}

void BST::Delete(int val){
    // 確認 val 有在 Tree 中
    TreeNode *delete_node = Search(val);
    if(delete_node == nullptr){
        cout << "data not found" << endl;
        return;
    }

    // y：實際要被刪除釋放記憶體的 Node，x：被刪除之 Node 的 child
    TreeNode *y = nullptr, *x = nullptr;

    // 若 delete_node 有兩個 child，則找出 successor 降至只有零或一個 child，最後兩者 data 再交換
    if(delete_node->leftchild == nullptr || delete_node->rightchild == nullptr)
        y = delete_node;
    else
        y = Successor(delete_node);

    // 將 x 設為 y 的 child（可能為 nullptr）
    if(y->leftchild)
        x = y->leftchild;
    else
        x = y->rightchild;
    
    // 將 x 接上欲刪除節點的 parent
    if(x)
        x->parent = y->parent;

    if(y->parent == nullptr)// 刪除點為 root
        this->root = x;
    else if(y == y->parent->leftchild)  // 欲刪除的是 parent 的 leftchild
        y->parent->leftchild = x;
    else if(y == y->parent->rightchild) // 欲刪除的是 parent 的 rightchild
        y->parent->rightchild = x;

    // 如果是找 successor 代替，則交換 data
    if(y != delete_node)
        delete_node->data = y->data;

    // 釋放刪除點記憶體位置
    delete y;
    y = nullptr;

    return;
}

void BST::Travel(){
    if(root == nullptr){
        cout << "Tree is empty" << endl;
        return;
    }

    // 由 root 找的 Leftmost 一定是最小值
    TreeNode *current = Leftmost(root);

    vector<Term> v;  // 暫存數值

    // 中序遍歷
    while(current){
        int c,l,r;

        c = current->data;
        if(current->leftchild == nullptr)
            l = 0;
        else
            l = current->leftchild->data;
        if(current->rightchild == nullptr)
            r = 0;
        else
            r = current->rightchild->data;
        v.push_back({c,l,r});

        current = Successor(current);
    }

    // output:
    cout << "node:";
    for(const auto &i : v)
        cout << ' ' << i.c_data;
    cout << endl;
    cout << "left:";
    for(const auto &i : v)
        cout << ' ' << i.l_data;
    cout << endl;
    cout << "right:";
    for(const auto &i : v)
        cout << ' ' << i.r_data;
    cout << endl;

    return;
}

int main(){
    int n;
    while(cin >> n){
        BST tree;
        if(n == -1) continue;
        tree.Insert(n);
        while(cin >> n && n != -1)
            tree.Insert(n);
        tree.Travel();
        cout << endl;
    }
    return 0;
}