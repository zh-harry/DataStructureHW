// Author : B133040043 鄭凱謚
// Date : Oct. 7, 2025
// Purpose : 使用 stack 找出西洋棋騎士(knight)走完棋盤的順序

#include<iostream>
#include<iomanip>   // for std::setw()
#include<vector>    // for std::vector
using namespace std;

// 共八種可能的移動方式
int move_i[8] = {-2,-1,1,2,2,1,-1,-2};
int move_j[8] = {1,2,2,1,-1,-2,-2,-1};

// 用 array 實作 stack
template <class T>
class Stack{
    public:
        Stack(int stackcapacity = 10);
        void push(const T& x);
        void pop();
        T Top();
        bool IsEmpty();
    private:
        T *arr;
        int top;
        int capacity;
};

template <class T>
Stack<T>::Stack(int stackcapacity):capacity(stackcapacity){
    arr = new T[stackcapacity];
    top = -1;
}

template <class T>
void Stack<T>::push(const T& x){
    if(top == capacity-1)
        throw "stack overflows";
    arr[++top] = x;
}

template <class T>
void Stack<T>::pop(){
    if(IsEmpty())
        throw "The stack is empty.";
    --top;
}

template <class T>
T Stack<T>::Top(){
    if(IsEmpty())
        throw "The stack is empty.";
    return arr[top];
}

template <class T>
bool Stack<T>::IsEmpty(){
    return top == -1;
}

// stack 內每一單位儲存三個數值
struct Term{
    int i;
    int j;
    int k;
};

// n x n 的騎士巡禮問題
void sol(int n){
    cout << "n = " << n  << ':' << endl;

    // 用 n x n vector 做棋盤
    vector<vector<int>> board(n,vector<int>(n,0));

    // 用 stack 實現除錯(backtrack)
    Stack<Term> backtrack(n*n);

    int record = 1; // 騎士在棋盤上的拜訪紀錄
    int i = 0,j = 0;// 騎士當前位置 = (i,j)
    int k = 0;      // 第 k 種移動方式

    board[i][j] = record; // 初始 (0,0) = 1

    // backtracking 直到把棋盤走完
    while(record != n*n){
        for(;k < 8;++k){

            int ti = i + move_i[k];
            int tj = j + move_j[k];

            // 檢查邊界條件和是否被拜訪過
            if(ti >= 0 && ti < n && tj >= 0 && tj < n && board[ti][tj] == 0){
                backtrack.push({i,j,k});
                i = ti;
                j = tj;
                k = 0;
                ++record;
                board[i][j] = record;
                break;
            }
        }
        if(k == 8){ // 八個方向都無法，代表要往回走
            
            // stack 是空的代表目前位置在 (0,0)，已經沒辦法再退了
            if(backtrack.IsEmpty()){
                cout << "no solution" << endl << endl;
                return;
            }

            board[i][j] = 0;
            --record;
            i = backtrack.Top().i;
            j = backtrack.Top().j;
            k = backtrack.Top().k + 1;
            backtrack.pop();
        }
    }

    // output result
    for(int r = 0;r < n;++r){
        for(int c = 0;c < n;++c)
            cout << setw(2) << board[r][c] << ' ';
        cout << endl;
    }
    cout << endl;
}

int main(){
    for(int i = 1;i <= 6;++i)
        sol(i);
    return 0;
}