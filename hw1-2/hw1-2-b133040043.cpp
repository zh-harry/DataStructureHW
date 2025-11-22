// Author : B133040043 鄭凱謚
// Date : Sept. 10, 2025
// Purpose : 印出 1 到 N 之間所有整數之階乘，where N <= 50

#include<iostream>
#include<vector>    // for std::vector
using namespace std;

// factorial 函數根據輸入參數 n 產生 1! ~ n! 的結果
void factorial(int n){

    // 宣告一個 vector 存放每個位數的數字，且初始為 1 位數，其值為 1
    vector<int> v(1,1);

    // 產生 i! 的答案，i = 1 ~ n
    for(int i = 1;i <= n;++i){

        // 宣告 carry 當作進位的數值
        int carry = 0;

        // 進行每一位數乘上 i 的運算
        for(int j = 0;j < v.size();++j){
            v[j] = v[j]*i + carry;
            carry = v[j] / 10;
            v[j] = v[j] % 10;
        }

        // 處理每一位數運算完仍有進位值的情況
        while(carry > 0){
            v.push_back(carry%10);
            carry /= 10;
        }

        // 輸出 i! 的答案
        cout << i << "! = ";
        for(int k = v.size()-1;k >= 0;--k)
            cout << v[k];
        cout << endl;
    }

    cout << endl;
    
    return;
}

int main(){
    
    int n;  // 宣告 n 作為承接輸入的變數

    // 持續輸入 n 獲得 1! ~ n! 的結果直到 n == 0
    while(cin >> n && n != 0)
        factorial(n);

    return 0;
}