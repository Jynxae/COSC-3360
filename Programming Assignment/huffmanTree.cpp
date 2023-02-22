#include<iostream>
#include<unordered_map>
#include<queue>
#include<cstring>
#include "huffmanTree.h"
using namespace std;

struct Compare                                  //referred to geeksforgeeks
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

node* huffmanTree::codeHuffmanTree(unordered_map<char,int> code)        //also referred to geeksforgeeks
{
    //set a priority queue since we'll need the frequency in order
    priority_queue<node*,vector<node*>, Compare >pq;

    for(auto p : code)
    {
        pq.push(new node(p.first,p.second));
    }

    // while(!pq.empty())   //made to check and see if sorting is done correctly
    // {
    //     node* result = pq.top();
    //     pq.pop();
    //     cout << result->frequency << " : " << result->let << endl;
    // }
    // create a while loop that goes through the priority queue that adds up the frequency and creates parent nodes
    // in each iteration
    while (pq.size() > 1)
    {
        node *left = pq.top();
        pq.pop();
        node *right = pq.top();
        pq.pop();

        // Create a new node with the combined frequency of the left and right nodes
        node *parent = new node('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        // Insert the new node back into the priority queue
        pq.push(parent);
    }

    return pq.top();
}

void huffmanTree::decode(unordered_map<int, vector<int>>, node*)
{
    
}

void huffmanTree::print(node* base, string code)
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
