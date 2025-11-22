// Author : B133040043 鄭凱謚
// Date : Sept. 20, 2025
// Purpose : 以C++製作一個「集合」類別 (set class)，
//           使其能進行「聯集」、「交集」、「差集」、「包含」、「屬於」等五種運算。

#include<iostream>
#include<string>    // for std::getline()
#define NUM 256     // 廣義 ASCII code 的範圍
using namespace std;

class TSet{
    public:
        TSet();
        friend istream& operator>>(istream& in,TSet& t);
        friend ostream& operator<<(ostream& out,const TSet& t);
        TSet operator+(const TSet& t) const;    // 聯集（union）
        TSet operator*(const TSet& t) const;    // 交集（intersection）
        TSet operator-(const TSet& t) const;    // 差集（difference）
        bool operator==(const TSet& t) const;   // 相等 (equal)
        bool operator>=(const TSet& t) const;   // 包含（contain）
        bool in(char x);    // 屬於（belong to）
    private:
        int bitstring[NUM]; // 用 1 0 儲存此字元是否出現過
};

TSet::TSet(){
    // Initialize
    for(int i = 0;i < NUM;bitstring[i++] = 0);
}

istream& operator>>(istream& in,TSet& t){
    string input;
    getline(cin,input);
    for(const auto &i : input)
        t.bitstring[(int)i] = 1;
    return in;
}

ostream& operator<<(ostream& out,const TSet& t){
    out << '{';
    for(int i = 0;i < NUM;++i)
        if(t.bitstring[i]) out << (char)i;
    out << '}';
    return out;
}

TSet TSet::operator+(const TSet& t) const{
    TSet ans;
    // x1 + x2 = {a|a 屬於 x1 或 a 屬於 x2}
    for(int i = 0;i < NUM;++i)
        ans.bitstring[i] = bitstring[i] | t.bitstring[i];
    return ans;
}

TSet TSet::operator*(const TSet& t) const{
    TSet ans;
    // x1 * x2 = {a|a 屬於 x1 且 a 屬於 x2}
    for(int i = 0;i < NUM;++i)
        ans.bitstring[i] = bitstring[i] & t.bitstring[i];
    return ans;
}

TSet TSet::operator-(const TSet& t) const{
    TSet ans;
    // x1 - x2 = {a|a 屬於 x1 且 a 不屬於 x2}
    for(int i = 0;i < NUM;++i)
        ans.bitstring[i] = bitstring[i] & (!t.bitstring[i]);
    return ans;
}

bool TSet::operator==(const TSet& t) const{
    for(int i = 0;i < NUM;++i)
        if(bitstring[i] != t.bitstring[i]) return false;
    return true;
}

bool TSet::operator>=(const TSet& t) const{
    // x1 >= x2 <=> x1 * x2 = x2
    if((*this) * t == t) return true;
    return false;
}

bool TSet::in(char x){
    return bitstring[(int)x];
}

int main(){
    int n;
    cin >> n;
    for(int i = 0;i < n;++i){
        TSet A,B;
        char x;

        cin.ignore();   // 吸收換行符號
        cin >> A >> B;
        cin.get(x);

        TSet C,D;
        C = A+B;
        D = A*B;

        // output
        cout << "Test Case " << (i+1) << ':' << endl;
        cout << "A: " << A << endl;
        cout << "B: " << B << endl;
        cout << "A+B: " << C << endl;
        cout << "A*B: " << D << endl;
        cout << "A-B: " << A-B << endl;
        cout << "B-A: " << B-A << endl;
        cout << (A >= B ? "A contains B" : "A does not contain B") << endl;
        cout << (B >= A ? "B contains A" : "B does not contain A") << endl;
        cout << '\'' << x << '\'' << (A.in(x) ? " is in A" : " is not in A") << endl;
        cout << '\'' << x << '\'' << (B.in(x) ? " is in B" : " is not in B") << endl;
        cout << endl;
    }
    return 0;    
}