// Author : B133040043 鄭凱謚
// Date : Nov. 28, 2025
// Purpose : 測試五種排序(sorting)演算法的執行效率

#include<iostream>
#include<thread>
#include<vector>
#include<chrono>
#include<iomanip>
#include<fstream>
#include<cstdlib>
#include<algorithm>
using namespace std;

void InsertionSort(vector<int>& data){
    for(int i = 1;i < data.size(); ++i){
        int current = data[i];
        int j = i-1;
        
        // 持續找直到遇見比 current 還小的值
        while(j >= 0 && data[j] > current){
            data[j+1] = data[j];
            --j;
        }
        // 將 current 插入所找好的位置
        data[j+1] = current;
    }
    return;
}

void Merge(vector<int>& data,int left,int mid,int right){
    // 建立左右 subvector
    vector<int> LeftSub(data.begin()+left,data.begin()+mid+1),
                RightSub(data.begin()+mid+1,data.begin()+right+1);
    
    int i = 0,j = 0,k = left;
    // 兩個 vector 倆倆比較，較小的先放
    while(i < LeftSub.size() && j < RightSub.size()){
        if(LeftSub[i] <= RightSub[j]){
            data[k] = LeftSub[i];
            ++i;
        }
        else{
            data[k] = RightSub[j];
            ++j;
        }
        ++k;
    }
    // 處理剩餘元素
    while(i < LeftSub.size()){
        data[k] = LeftSub[i];
        ++i;
        ++k;
    }
    while(j < RightSub.size()){
        data[k] = RightSub[j];
        ++j;
        ++k;
    }
    return;
}

void MergeSort(vector<int>& data,int left,int right){
    // 只要長度為 2 以上就要排序
    if(right > left){
        int mid = (left+right)/2;   // 分成左右兩個 subvector
        MergeSort(data,left,mid);   // 排序好左邊的
        MergeSort(data,mid+1,right);// 排序好右邊的
        Merge(data,left,mid,right); // 合併起來（包括排序）
    }
    return;
}

void QuickSort(vector<int>& data,int left,int right){
    if(left < right){
        // 選定 pivot
        int i = left,j = right+1,pivot = data[left];
        // 排序成 pivot 左邊的都比 pivot 小，右邊的都比 pivot 大
        do{
            do ++i; while(data[i] < pivot);
            do --j; while(data[j] > pivot);
            if(i < j) swap(data[i],data[j]);
        }while(i < j);
        swap(data[left],data[j]);
        // 分成兩邊繼續遞迴排序
        QuickSort(data,left,j-1);
        QuickSort(data,j+1,right);
    }
    return; 
}

int cmp(const void *a,const void *b){
    return *(int *)a - *(int *)b;
}

void CQSort(vector<int>& data){
    int *ptr = &data[0];
    qsort(ptr,data.size(),sizeof(int),cmp);
    return;
}

void CppSort(vector<int>& data){
    sort(data.begin(),data.end());
    return;
}


int main(){
    ifstream ifs;
    ofstream ofs;

    ifs.open("input.txt");
    int N;
    ifs >> N;
    vector<int> vec(N); // 儲存輸入之資料
    for(int i = 0;i < N;++i)
        ifs >> vec[i];
    ifs.close();
    
    vector<int> data;   // 真正要做排序並輸出的資料
    chrono::steady_clock::time_point start,end; // 擷取排序前後的時間
    cout << "Elapsed Time:" << endl;
    
    // Insertion Sort:
    ofs.open("ouputA.txt");
    data = vec;
    start = chrono::steady_clock::now();
    InsertionSort(data);
    end = chrono::steady_clock::now();
    cout << "Insertion Sort:" << endl;
    cout << chrono::duration<double>(end - start).count() << endl;
    ofs << "Insertion Sort:" << endl;
    for(const auto& i : data)
        ofs << i << endl;
    ofs.close();

    // Merge Sort:
    ofs.open("ouputB.txt");
    data = vec;
    start = chrono::steady_clock::now();
    MergeSort(data,0,data.size()-1);
    end = chrono::steady_clock::now();
    cout << "Merge Sort:" << endl;
    cout << chrono::duration<double>(end - start).count() << endl;
    ofs << "Merge Sort:" << endl;
    for(const auto& i : data)
        ofs << i << endl;
    ofs.close();
    
    // Quick Sort:
    ofs.open("ouputC.txt");
    data = vec;
    start = chrono::steady_clock::now();
    QuickSort(data,0,data.size()-1);
    end = chrono::steady_clock::now();
    cout << "Quick Sort:" << endl;
    cout << chrono::duration<double>(end - start).count() << endl;
    ofs << "Quick Sort" << endl;
    for(const auto& i : data)
        ofs << i << endl;
    ofs.close();
    
    // C qsort():
    ofs.open("ouputD.txt");
    data = vec;
    start = chrono::steady_clock::now();
    CQSort(data);
    end = chrono::steady_clock::now();
    cout << "C qsort():" << endl;
    cout << chrono::duration<double>(end - start).count() << endl;
    ofs << "C qsort()" << endl;
    for(const auto& i : data)
        ofs << i << endl;
    ofs.close();
    
    // C++ sort():
    ofs.open("ouputE.txt");
    data = vec;
    start = chrono::steady_clock::now();
    CppSort(data);
    end = chrono::steady_clock::now();
    cout << "C++ sort():" << endl;
    cout << chrono::duration<double>(end - start).count() << endl;
    ofs << "C++ sort()" << endl;
    for(const auto& i : data)
        ofs << i << endl;
    ofs.close();

    return 0;
}