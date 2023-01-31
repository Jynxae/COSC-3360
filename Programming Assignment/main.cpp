#include<iostream>
#include <pthread.h>
#include<fstream>
#include<sstream>
#include <vector>
#include "huffmanTree.cpp"
#include <unordered_map>
using namespace std;

struct datA{
    node* root;
    string symbol;          //found out how to pass multiple arguments into a thread function like this from stack overflow and https://w3.cs.jmu.edu/
    vector<int> position;
    vector<char> message;
};

void* threadFunction(void* args) //here is where we actually decode
{
    datA* arg = (datA*) args;

    node* curr = arg->root;
    for(auto x : arg->symbol)
    {
        if( x == '0')
        {
            curr = curr->left;
        }
        else if(x == '1')
        {
            curr = curr->right;
        }
    }
    for(auto x : arg->position)
    {
        arg->message.at(x) = curr->let;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    string inputFile, compressedFile, line;
    // cin >> inputFile;
    // cin >> compressedFile;

    ifstream fin("input1");
    ifstream cfin("cInput1");
    char let;
    int num, freq;
    unordered_map<char,int> input;
    unordered_map<int, vector<int>> code;
    
    int mSize = 0;

    while(getline(fin,line))
    {
        let = line[0];
        num = int(line[2]) - 48;
        input[let] = num;
        mSize += line[2] - 48; //plus the frequency
    }
    vector<pthread_t> th (mSize);
    vector<char> message (mSize);
    huffmanTree call;
    node * root = call.codeHuffmanTree(input);
    call.print(root, "");

    string binary = ""; 
    while(getline(cfin,line))
    {
        stringstream str(line);
        str >> binary;
        vector<int> spot;
        int value;
        while(str >> value) {
            spot.push_back(value);
        }
        pthread_t thread1;      //decalring a new thread each time i get a line
        datA *d = new datA();
        d->root = root;
        d->symbol = binary;
        d->position = spot;
        d->message = message;

        pthread_create(&thread1, NULL, threadFunction, (void*) d);
        th.push_back(thread1);
    }
    cout << "done";
    for(int x = 0; x < th.size(); x++){
            pthread_join(th.at(x),NULL);
    }

    //add another for loop here for something??

    for(int x = 0; th.size(); x++)
    {
        cout << th.at(x) << endl;
    }
    cout << "Original message: " << endl;
    return 0;
}
