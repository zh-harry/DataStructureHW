// Author : B133040043 鄭凱謚
// Date : Nov. 4, 2025
// Purpose : 設計一個程式，可以處理兩個多項式之加法與乘法

// 這裡是先將 Linked List 先建立出來，再建立 Polynomial 類別用一個 has-a 的關係將 List
// 視為 private member，因此 List 的部分可能較冗長，先說聲不好意思 ><。
// 或者想要讓 Polymomial 的 class 結構本身就像一個 List 也可以。

#include<iostream>
using namespace std;

// 實作 Linked List(singly)
template<class T>
class List;

template<class T>
class ListNode{
    public:
        ListNode();
        ListNode(const T& x);
        friend class List<T>; 
    private:
        T data;
        ListNode<T>* link;
};

template<class T>
ListNode<T>::ListNode():link(nullptr){}

template<class T>
ListNode<T>::ListNode(const T& x):link(nullptr){data = x;}

template<class T>
class List{
    public:
        List();
        List(const List& other);    // copy constructor
        ~List();
        List& operator=(const List& other); // deep copy

        // Iterator：迭代器
        class ListIterator{
            public:
                ListIterator(ListNode<T>* startNode = nullptr);
                T& operator*() const;
                T* operator->() const;
                ListIterator& operator++();             // prefix
                ListIterator operator++(int ignore);    // postfix
                bool operator!=(const ListIterator& right) const;
                bool operator==(const ListIterator& right) const;
                friend class List<T>;
            private:
                ListNode<T>* current;
        };

        ListIterator begin() const; // 回傳指向 List 開頭（first）的 Iterator
        ListIterator end() const;   // 回傳指向 List 結尾（last 的下一個 Node，即為 nullptr）的 Iterator
        void PushBack(const T& x);  // 由 List 尾端插入元素
        void Insert(ListIterator position,const T& x);  // 根據 Iterator 指向的位置插入元素
        void Clear();   // 清空 List
    private:
        ListNode<T>* first;
        ListNode<T>* last;
};

template<class T>
List<T>::List():first(nullptr),last(nullptr){}

template<class T>
List<T>::List(const List& other):first(nullptr),last(nullptr){
    ListNode<T>* current = other.first;
    while(current){
        PushBack(current->data);
        current = current->link;
    }
}

template<class T>
List<T>::~List(){
    ListNode<T>* current;
    while(first){
        current = first;
        first = first->link;
        delete first;
    }
    last = nullptr;
}

template<class T>
List<T>& List<T>::operator=(const List& other){
    if(this == &other) return *this;

    Clear();

    first = nullptr; last = nullptr;
    ListNode<T>* current = other.first;
    while(current){
        PushBack(current->data);
        current = current->link;
    }
    return *this;
}

template<class T>
List<T>::ListIterator::ListIterator(ListNode<T>* startNode){current = startNode;}

template<class T>
T& List<T>::ListIterator::operator*() const{return current->data;}

template<class T>
T* List<T>::ListIterator::operator->() const{return &(current->data);}

template<class T>
typename List<T>::ListIterator& List<T>::ListIterator::operator++(){
    current = current->link;
    return *this;
}

template<class T>
typename List<T>::ListIterator List<T>::ListIterator::operator++(int ignore){
    ListIterator previous = *this;
    current = current->link;
    return previous;
}

template<class T>
bool List<T>::ListIterator::operator!=(const ListIterator& right) const{return current != right.current;}

template<class T>
bool List<T>::ListIterator::operator==(const ListIterator& right) const{return current == right.current;}

template<class T>
typename List<T>::ListIterator List<T>::begin() const{return ListIterator(first);}

template<class T>
typename List<T>::ListIterator List<T>::end() const{return ListIterator(nullptr);}

template<class T>
void List<T>::PushBack(const T& x){
    if(first){
        last->link = new ListNode<T>(x);
        last = last->link;
    }
    else
        first = last = new ListNode<T>(x);
}

template<class T>
void List<T>::Insert(ListIterator position,const T& x){
    ListNode<T>* insert_node = new ListNode<T>(x);
    
    if(position.current == first){
        insert_node->link = first;
        first = insert_node;
        return;
    }

    if(position.current == nullptr){
        PushBack(x);
        return;
    }

    ListNode<T>* temp = first;
    while(temp && temp->link != position.current) temp = temp->link;
    insert_node->link = temp->link;
    temp->link = insert_node;
}

template<class T>
void List<T>::Clear(){
    ListNode<T>* current;
    while(first){
        current = first;
        first = first->link;
        delete current;
    }
    last = nullptr;
}

// Term：即為一個項，存放係數與次方
struct Term{
    Term(int c = 0,int e = 0);
    int coef;
    int exp;
};

Term::Term(int c,int e):coef(c),exp(e){}

// 以 Linked List 當作 private member 實作 Polynomial 類別
class Polynomial{
    public:
        Polynomial();
        using Iterator = List<Term>::ListIterator;  // 別名：alias
        Iterator begin() const;
        Iterator end() const;
        void Insert(const Term& x);
        void Clear();
        Polynomial operator+(const Polynomial& right) const;
        Polynomial operator*(const Polynomial& right) const;
    private:
        List<Term> poly;
};

Polynomial::Polynomial(){}

Polynomial::Iterator Polynomial::begin() const{return poly.begin();}

Polynomial::Iterator Polynomial::end() const{return poly.end();}

void Polynomial::Clear(){poly.Clear();}

// 讓 Polynomial insert 時可以自動排序成降冪形式，因此與 List insert 行為稍不同，另外實作。
void Polynomial::Insert(const Term& x){
    for(Iterator it = begin();it != end();++it){
        if(x.exp > it->exp){
            poly.Insert(it,x);
            return;
        }
        else if(x.exp == it->exp){
            it->coef += x.coef;
            return;
        }
    }
    poly.PushBack(x);
    return;
}

// 實作 Polynomial 加法運算
Polynomial Polynomial::operator+(const Polynomial& right) const{
    Polynomial ans;
    Iterator left_it = begin(),right_it = right.begin();

    // 高次方項處理到低次方項（按照次方高到低計算，因此用 List<T>::PushBack）
    while((left_it != end()) && (right_it != right.end())){
        // 兩個項次方相等
        if(left_it->exp == right_it->exp){
            int sum = left_it->coef + right_it->coef;
            if(sum) // 和為非 0 數值才要記錄
                ans.poly.PushBack({sum,right_it->exp});
            ++left_it;
            ++right_it;
        }
        // 右邊項較大
        else if(left_it->exp < right_it->exp){
            ans.poly.PushBack({right_it->coef,right_it->exp});
            ++right_it;
        }
        // 左邊項較大
        else{
            ans.poly.PushBack({left_it->coef,left_it->exp});
            ++left_it;
        }
    }
    // 處理剩餘的項（只有其中一邊會剩）
    while(left_it != end()){
        ans.poly.PushBack({left_it->coef,left_it->exp});
        ++left_it;
    }
    while(right_it != right.end()){
        ans.poly.PushBack({right_it->coef,right_it->exp});
        ++right_it;
    }

    return ans;
}

// 實作 Polynomial 乘法運算
Polynomial Polynomial::operator*(const Polynomial& right) const{
    Polynomial ans;

    // 其中一者為 0 多項式，就回傳 0 多項式
    if((begin() == end()) || (right.begin() == right.end()))
        return ans;
    
    // 兩層迴圈跑完所有的乘法組合（計算順序非皆照次方高低順序，因此用 Polynomial::Insert）
    for(const auto& left_it : poly){
        for(const auto& right_it : right.poly){
            int c = left_it.coef * right_it.coef; // 係數相乘
            int e = left_it.exp + right_it.exp;   // 指數相加
            ans.Insert({c,e});
        }
    }
    return ans;
}

int main(){
    Polynomial A,B,C,D;
    int P,Q;
    int cnt = 0;
    while(cin >> P){
        // 輸入 A
        for(int i = 0;i < P;++i){
            int c,e;
            cin >> c >> e;
            if(c)   // 係數非 0 才要紀錄
                A.Insert({c,e});
        }

        cin >> Q;
        if((P+Q) == 0) break; // (P == 0) && (Q == 0)

        // 輸入 B
        for(int i = 0;i < Q;++i){
            int c,e;
            cin >> c >> e;
            if(c)   // 係數非 0 才要紀錄
                B.Insert({c,e});
        }

        C = A + B;
        D = A * B;

        // output:
        ++cnt;
        cout << "Case" << cnt << ':' << endl;

        cout << "ADD" << endl;
        Polynomial::Iterator C_it = C.begin();
        if(C_it == C.end())
            cout << "0 0" << endl;
        else{
            for(;C_it != C.end();++C_it)
                cout << C_it->coef << ' ' << C_it->exp << endl;
        }

        cout << "MULTIPLY" << endl;
        Polynomial::Iterator D_it = D.begin();
        if(D_it == D.end())
            cout << "0 0" << endl;
        else{
            for(;D_it != D.end();++D_it)
                cout << D_it->coef << ' ' << D_it->exp << endl;
        }
        A.Clear(); B.Clear();
    }
    return 0;
}