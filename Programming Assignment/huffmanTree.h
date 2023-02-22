#include<iostream>
#include<unordered_map>
using namespace std;

struct node{
    char let;
    int frequency;
    string code;
    node* right;
    node* left;
    node(char chara, int freq)
    {
        let = chara;
        frequency = freq;
        left = right = nullptr;
    }
};

class huffmanTree{
private:
    node* tree;
public:
    huffmanTree()
    {
        tree = nullptr;
    }
    node* codeHuffmanTree(unordered_map<char,int>);
    void print(node*,string);
    void decode(unordered_map<int, vector<int>>,node*);
};
