// Author : B133040043 鄭凱謚
// Date : Sept. 9, 2025
// Purpose : 製作 N 階（N 為奇數）的魔方陣

#include<iostream>
#include<vector>    // for std::vector
#include<iomanip>   // for setw() function
using namespace std;

// MagicSquare 函數用來產生魔方陣，傳入參數 n 代表 n 階魔方陣
void MagicSquare(int n){

    // 宣告一個 2d vector 當作方陣，並將所有值初始化為 0
    vector<vector<int>> square(n,vector<int>(n,0));

    // 宣告 i, j 代表位置 (i,j)，並設置初始位置（最上列的中間）和其值
    int i = 0,j = n/2;
    square[i][j] = 1;

    // 宣告變數 k，讓 k 從 2 開始到 n 平方填滿方陣
    for(int k = 2;k <= n*n;++k){

        // 宣告 ti, tj 表示下一個位置
        int ti = ((i-1) + n) % n;
        int tj = ((j-1) + n) % n;

        // 檢查 (ti,tj) 是否還沒被填過（其值為 0）
        if(square[ti][tj] == 0){
            i = ti;
            j = tj;
        }
        else{   // 如果被填過則下個位置換到目前位置的正下方
            i = i+1;
            j = j;
        }
        
        // 更新下個位置的值
        square[i][j] = k;
    }

    // 輸出 n 階魔方陣
    cout << "n = " << n << " : " << endl;
    for(const auto &row : square){
        for(const auto &col : row)
            cout << setw(2) << col << ' ';
        cout << endl;
    }
    cout << endl;

    return;
}

int main(){

    // 製造 1, 3, 5, 7, 9 階魔方陣
    for(int i = 1;i <= 9; i+=2)
        MagicSquare(i);
    
    return 0;
}