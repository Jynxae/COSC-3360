#include <iostream>
#include <pthread.h>    //needed to do threading
#include <fstream>
#include <sstream>  //string strean
#include <vector>
#include "huffmanTree.cpp"
#include <unordered_map>
using namespace std;

struct ThreadArgs
{
    node *root;
    string binaryCode; // found out how to pass multiple arguments into a thread function like this from stack overflow and https://w3.cs.jmu.edu/
    vector<int> position;
    vector<char> message; // changed to vector, messy to deal with as string. can just print out indexes with no spaces
};

void *threadFunction(void *args) // here is where we actually decode, referred to https://iq.opengenus.org/ with best way of traversing
{
    ThreadArgs *arg = (ThreadArgs *)args;

    node *curr = arg->root;
    for (int x = 0; x < arg->binaryCode.length(); x++) // traverse the BINARY SYMBOL string, character by character.
    {
        if (arg->binaryCode[x] == '0')  //think like we are doing a huffman tree by hand, we go left when zero
        {
            curr = curr->left;
        }
        else if (arg->binaryCode[x] == '1') //and we go right when one, stop once we reach the wanted node, in our case, once we finish reading the string character by character
        {
            curr = curr->right;
        }
    }

    char letterResult = curr->let; // gives me the final letter of the node i landed on from above^^

    for (int x = 0; x < arg->position.size(); x++)
    {
        arg->message.at(arg->position.at(x)) = letterResult;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    string inputFile, compressedFile, line;
    //  cin >> inputFile;
    //  cin >> compressedFile;

    ifstream fin("input2");
    ifstream cfin("cInput2");
    
    int mSize = 0;
    int argSize = 0;
    string binary = "";
    unordered_map<char, int> input;
    unordered_map<int, vector<int>> code;
    static vector<pthread_t> th(mSize); // remove static if it messes with code possibly
    static vector<ThreadArgs *> passer;

    while (getline(fin, line))
    {
        char let;
        int num;
        let = line[0];
        num = int(line[2]) - '0';    //or we can do - 48, the ascii value of zero
        input[let] = num;
        mSize += line[2] - '0'; // plus the frequency
    }

    huffmanTree call;
    vector<char> message(mSize); // DECLARE VECTOR HERE INSTEAD AND KEEP MSIZE UP TOP. Since im still adjusting size. Do not give size to the vector until its the final size
    node *root = call.codeHuffmanTree(input);
    call.print(root, "");

    while (getline(cfin, line))
    {
        pthread_t thread1; // decalring a new thread each time i get a line
        ThreadArgs *argument = new ThreadArgs();
        argument->root = root;
        argument->message = message;
        stringstream str(line); // stringstream allows me to read in individual values after i getline, REMEMBER TO USE IN FUTURE ASSIGNMENTS!!!!!

        str >> binary; // after i read in a whole line, str allows me to read one at a time.
        vector<int> spot;
        int value;
        argSize++;
        while (str >> value)
        {
            spot.push_back(value);
        }

        argument->binaryCode = binary;
        argument->position = spot;

        pthread_create(&thread1, NULL, threadFunction, (void *)argument); // remember for exam to DEREFERENCE THE THREAD!!!! must also cast the argument as void*
        passer.push_back(argument);
        th.push_back(thread1);
    }

    for (int x = 0; x < th.size(); x++)
    { // referenced from rincons code
        pthread_join(th.at(x), NULL);
    }

    // add another for loop here for something??

    for(int x = 0; x < passer.size(); x++)
    {
        vector<int> pos = passer.at(x)->position;
        for(int i = 0; i < mSize; i++)
        {
            message.at(i) = passer.at(x)->message.at(i);
        }
    }

    for (auto arg : passer) // basically arg takes the value of the first value of passer and when it loops again, it takes the next value
    {
        for (auto spot : arg->position)
        {
            message.at(spot) = arg->message.at(spot);
        }
    }

    cout << "Original message: ";

    string decoded = "";
    for (int x = 0; x < message.size(); x++)
    {
        decoded += message.at(x); // doesnt output in terminal while debugging if i cout from here??
    }

    cout << decoded; // YAYAYYA

    return 0;
}
