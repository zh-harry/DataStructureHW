// Author : B133040043 鄭凱謚
// Date : Oct. 8, 2025
// Purpose : 使用 recursion 找出西洋棋騎士(knight)走完棋盤的順序

#include<iostream>
#include<vector>    // for std::vector
#include<iomanip>   // for std::setw()
using namespace std;

int n;
vector<vector<int>> board;

// 共八種可能的移動方式
int move_i[8] = {-2,-1,1,2,2,1,-1,-2};  
int move_j[8] = {1,2,2,1,-1,-2,-2,-1};

// 主要遞迴函式，(i,j)：目前的位置，record：下一步要填入的數字
bool sol(int i,int j,int record){
    
    // 如果棋盤的所有格子都被走訪過
    if(record == n*n+1)
        return true;

    // 嘗試八個方向
    for(int k = 0;k < 8;++k){

        int ti = i + move_i[k];
        int tj = j + move_j[k];

        // 檢查邊界條件和是否被拜訪過
        if(ti >= 0 && ti < n && tj >= 0 && tj < n && board[ti][tj] == 0){
            board[ti][tj] = record;
            if(sol(ti,tj,record+1))
                return true;
            board[ti][tj] = 0; // 回朔 backtrack
        }
    }
    return false;
}

int main(){
    for(n = 1;n <= 6;++n){
        cout << "n = " << n << ':' << endl;

        // 用 n x n vector 做棋盤
        board.assign(n,vector<int>(n,0));

        // 初始 (0,0) = 1
        board[0][0] = 1;

        // 利用遞迴函式解騎士巡禮問題，如果有解就輸出走訪順序
        if(sol(0,0,2)){
            for(const auto& r : board){
                for(const auto& c : r)
                    cout << setw(2) << c << ' ';
                cout << endl;
            }
        }
        else
            cout << "no solution" << endl;
        cout << endl;
    }
    return 0;
}