#include<queue>
#include<cstring>
#include "huffmanTree.h"
using namespace std;

struct Compare      //had to tweak the compare function a bit to account for same letters
{
    bool operator()(node *left, node *right)
    {

        if(left->frequency == right->frequency)     
        {
            if(left->let == right->let)
            {
                return left < right;
            }
            return left->let > right->let;
        }
        return left->frequency > right->frequency;
    } // take in to account for null let
};

node* huffmanTree::codeHuffmanTree(unordered_map<char,int> code)    //referred to geeksforgeeks on how a huffman could be built, priority queue seems to be the way everyone is going.
{
    //set a priority queue since we'll need the frequency in order
    priority_queue<node*,vector<node*>, Compare >pq;

    for(auto p : code)
    {
        pq.push(new node(p.first,p.second));
    }

    // create a while loop that goes through the priority queue that adds up the frequency and creates parent nodes
    // in each iteration
    while (pq.size() > 1)
    {
        node *left = pq.top();
        pq.pop();                   //just pop two nodes, add them and onto the next
        node *right = pq.top();
        pq.pop();

        node *parent = new node('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;


        pq.push(parent);
    }

    return pq.top();
}

void huffmanTree::print(node* base, string code)        //basically traversing through a regular tree like normal
{
    if(base == nullptr) //check to see if the node is empty
    {
        return;
    }
    if(base->right == nullptr && base->left == nullptr) //if we reach a leaf, print the  code
    {
        cout << "Symbol: " << base->let << ", Frequency: " << base->frequency << ", Code: " << code << "\n";
        base->code = code;
        return;
    }

    print(base->left, code + "0"); //in this assignment, left is 0 despite the number being greater or not
    print(base->right, code+ "1"); //and right will always be 1
}
