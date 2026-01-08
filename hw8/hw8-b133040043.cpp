// Author : B133040043 鄭凱謚
// Date : Dec. 14, 2025
// Purpose : 以 Huffman 演算法製作一個檔案壓縮與解壓縮之軟體

#include<iostream>
#include<fstream>
#include<cstring>
#include<iomanip>
#include<cctype>
#include<vector>
#include<utility>
#include<queue>
#define ull unsigned long long
#define NUM 256
using namespace std;

int FrequencyCnt[NUM];  // 壓縮時紀錄每個字元的出現頻率
string EncodeTable[NUM];// 壓縮時產生每個字元的編碼

// Huffman Tree 中的節點
struct Node {
    unsigned char Character;
    int Frequency;
    Node *LeftChild;
    Node *RightChild;
    Node();
    Node(unsigned char c,int f,Node *lc,Node *rc);
};

Node::Node():Character('\0'),Frequency(0),LeftChild(nullptr),RightChild(nullptr){};

Node::Node(unsigned char c,int f,Node *lc,Node *rc):Character(c),Frequency(f),LeftChild(lc),RightChild(rc){};

// 實現 MinHeap 比較函數
struct MinHeapComparator{
    bool operator()(Node *left, Node *right){
        if(left->Frequency == right->Frequency)
            return left->Character > right->Character;
        return left->Frequency > right->Frequency;
    }
};

void init(){
    for(int i = 0;i < NUM;++i){
        FrequencyCnt[i] = 0;
        EncodeTable[i] = "";
    }
}

// 建立 Huffman Tree
void Huffman(priority_queue<Node*,vector<Node*>,MinHeapComparator>& MinHeap,int n){
    for(int i = 0;i < n-1;++i){
        Node *left = MinHeap.top();
        MinHeap.pop();

        Node *right = MinHeap.top();
        MinHeap.pop();

        if(left->Character > right->Character)
            swap(left,right);

        Node *parent = new Node(left->Character,left->Frequency + right->Frequency,left,right);

        MinHeap.push(parent);
    }
}

// 產生字元編碼表
void GenerateEncodeTable(Node *root,string code){
    if(!root) return;
    // Leaf Node 代表字元
    if(!root->LeftChild && !root->RightChild){
        EncodeTable[root->Character] = code;
        return;
    }
    GenerateEncodeTable(root->LeftChild,code + "0");
    GenerateEncodeTable(root->RightChild,code + "1");
}

// 由已知字元編碼重建 Huffman Tree
void RebuildHuffmanTree(Node *root,unsigned char ch,string code){
    Node *current = root;
    for(const auto& bit : code){
        if(bit == '0'){
            if(!current->LeftChild)
                current->LeftChild = new Node();
            current = current->LeftChild;
        }
        else if(bit == '1'){
            if(!current->RightChild)
                current->RightChild = new Node();
            current = current->RightChild;
        }
    }
    current->Character = ch;
}

int main(int argc,char *argv[]){
    
    string Operation = argv[1]; // 決定操作
    string InFile = argv[3];    // 輸入檔案
    string OutFile = argv[5];   // 輸出檔案
    
    ifstream ifs;
    ofstream ofs;
    
    init();

    // 壓縮操作
    if(Operation == "-c"){

        // 紀錄原始檔案資料
        vector<unsigned char> UncompressedData;
        ifs.open(InFile, ios::in | ios::binary);
        unsigned char uc;
        while(ifs.read(reinterpret_cast<char*>(&uc),sizeof(uc))){
            UncompressedData.push_back(uc);
            ++FrequencyCnt[uc];
        }
        ifs.close();

        // 建立 MinHeap
        priority_queue<Node*,vector<Node*>,MinHeapComparator> MinHeap;
        for(int i = 0;i < NUM;++i){
            if(FrequencyCnt[i] > 0){
                Node* PushNode = new Node(i,FrequencyCnt[i],nullptr,nullptr);
                MinHeap.push(PushNode);
            }
        }

        // 建立 Huffman Tree
        Huffman(MinHeap,MinHeap.size());
        Node *HuffmanTreeRoot = MinHeap.top();
        MinHeap.pop();

        // 建立編碼表
        GenerateEncodeTable(HuffmanTreeRoot,"");

        // 建立需要輸出到終端與 File Header 的編碼表形式：
        // (字元)(編碼長度)(編碼)
        vector<unsigned char> PresentTable;
        int TableRow = 0;
        for(int i = 0;i < NUM;++i){
            if(EncodeTable[i] != ""){
                ++TableRow;
                PresentTable.push_back(i);
                PresentTable.push_back(EncodeTable[i].size()); 
                for(const auto& j : EncodeTable[i])
                    PresentTable.push_back(j);
            }
        }

        // 紀錄壓縮後的資料（盡量塞進一個 byte 裡）
        vector<unsigned char> CompressedData;
        unsigned char buffer = 0;
        int BitCnt = 0;
        for(const auto& byte : UncompressedData){
            for(const auto& bit : EncodeTable[byte]){
                buffer = buffer << 1;
                if(bit == '1') buffer = buffer | 1;
                ++BitCnt;
                if(BitCnt == 8){
                    CompressedData.push_back(buffer);
                    buffer = 0;
                    BitCnt = 0;
                }
            }
        }
        if(BitCnt > 0){ // 將最後一個 byte 補滿 0
            buffer = buffer << (8-BitCnt);
            CompressedData.push_back(buffer);
        }

        // 輸出 File Header 資訊
        ull UncompressedSize = UncompressedData.size();
        ull CompressedSize = sizeof(int) + sizeof(double) + 2*sizeof(ull) + PresentTable.size() + CompressedData.size();
        double CompressionRatio = (double)UncompressedSize/CompressedSize;
        ofs.open(OutFile, ios::out | ios::binary);
        ofs.write(reinterpret_cast<char*>(&UncompressedSize),sizeof(UncompressedSize));
        ofs.write(reinterpret_cast<char*>(&CompressedSize),sizeof(CompressedSize));
        ofs.write(reinterpret_cast<char*>(&CompressionRatio),sizeof(CompressionRatio));
        ofs.write(reinterpret_cast<char*>(&TableRow),sizeof(TableRow));
        if(!PresentTable.empty())
            ofs.write(reinterpret_cast<char*>(PresentTable.data()),PresentTable.size());
        if(!CompressedData.empty())
            ofs.write(reinterpret_cast<char*>(CompressedData.data()),CompressedData.size());
        ofs.close();

        // 輸出到終端資訊
        cout << "Uncompressed Size: " << UncompressedSize << endl;
        cout << "Compressed Size: " << CompressedSize << endl;
        cout << "Compression Ration: " << CompressionRatio << endl;
        for(int i = 0,j = 0;i < TableRow;++i){
            unsigned char PrintChar = PresentTable[j++];
            if(isprint(PrintChar))
                cout << '\'' << PrintChar << '\'';
            else
                cout << '(' << (int)PrintChar << ')';
            cout << " = ";
            int CodeLength = PresentTable[j++];
            for(int k = 0;k < CodeLength;++k)
                cout << PresentTable[j++];
            cout << endl;
        }
    }
    else if(Operation == "-u"){

        // 讀入 File Header 資訊
        ull UncompressedSize,CompressedSize;
        double CompressionRate;
        int TableRow;
        ifs.open(InFile, ios::in | ios::binary);
        ifs.read(reinterpret_cast<char*>(&UncompressedSize),sizeof(UncompressedSize));
        ifs.read(reinterpret_cast<char*>(&CompressedSize),sizeof(CompressedSize));
        ifs.read(reinterpret_cast<char*>(&CompressionRate),sizeof(CompressionRate));
        ifs.read(reinterpret_cast<char*>(&TableRow),sizeof(TableRow));
        Node* HuffmanTreeRoot = new Node();
        for(int i = 0;i < TableRow;++i){// 讀入編碼表並重建 Huffman Tree
            unsigned char Key,Length;
            ifs.read(reinterpret_cast<char*>(&Key),sizeof(Key));
            ifs.read(reinterpret_cast<char*>(&Length),sizeof(Length));
            string Code = "";
            for(int j = 0;j < Length;++j){
                unsigned char bit;
                ifs.read(reinterpret_cast<char*>(&bit),sizeof(bit));
                Code += bit;
            }
            RebuildHuffmanTree(HuffmanTreeRoot,Key,Code);
        }

        // 讀取壓縮後資料並解壓縮至輸出檔案
        ofs.open(argv[5], ios::out | ios::binary);
        Node *current = HuffmanTreeRoot;
        unsigned char buffer;
        ull DecodeCnt = 0;
        while(ifs.read(reinterpret_cast<char*>(&buffer),sizeof(buffer))){
            for(int i = 7;i >= 0;--i){
                int bit = (buffer >> i) & 1;// 逐位取出位元
                if(bit)
                    current = current->RightChild;
                else
                    current = current->LeftChild;
                if(!current->LeftChild && ! current->RightChild){
                    unsigned char DecompressedChar = current->Character;
                    ofs.write(reinterpret_cast<char*>(&DecompressedChar),sizeof(DecompressedChar));
                    ++DecodeCnt;
                    if(DecodeCnt == UncompressedSize) 
                        break;
                    current = HuffmanTreeRoot;
                }
            }
        }
        ofs.close();
        ifs.close();
    }
    return 0;
}