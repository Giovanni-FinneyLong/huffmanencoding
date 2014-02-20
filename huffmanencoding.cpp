/* Project 2 - Huffman Encoding
   Richard Waltman, Giovanni Finney-long
   CS 130A, W14, UCSB, 2/18/14
*/
#include <iostream>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <climits>
#include <iomanip>
using namespace std;

int newNodeLetters = 65;

class node {
private:
   char letter;
   int freq;
   node* lChild;
   node* rChild;

public:
    node(){};
    ~node(){};
    node(char c, int f) {letter = c; freq = f; lChild = NULL; rChild = NULL; }
    node(node* node1, node* node2 )    //combines two deleted nodes into a new one
    {
        freq = ( ( node1->getFreq() ) + ( node2->getFreq() ) );
        letter = newNodeLetters;    //new node labeled starting at 'A'
        rChild = node1;
        lChild = node2;
        newNodeLetters++;
    }
    int getFreq() {return freq;}
    char getChar() {return letter;}
    node* getRight() {return rChild;}
    node* getLeft() {return lChild;}
    };

class Heap {
private:
vector<node*> heap;      //vector representing min heap
   int charsUsed;        //to build the full storage array
   string encodeMap[27]; //stores binary code for each used letter
   char* fileName;
public:
   Heap(){};
    Heap(char* file);     //creates a vector heap using a fileName
   ~Heap();
   void insert(node* element);     //inserts a node into heap
   node* deletemin();	 //deletes index 0 and returns it
   void print();	 //prints out the heap
   void printStorage();            //prints the storage vector
   int size() {return heap.size();}//returns size of vector heap
   void buildTrie();   //builds a trie out of the minHeap and stores it in heap vector
   void huffmanDecode(node* currPtr); //decodes binary and returns a string
private:
   int left(int parent);	 //returns left child given an index
   int right(int parent);	 //returns right child given an index
   int parent(int child);	 //returns the parent when given an index
   void percolateDown(int index);	//called after a delete
   void percolateUp(int index);    //called after an insert
   void huffmanEncode(node* currPtr);           //given a string, return an encoded binary string
   void huffmanWork(node*p, string str); //huffmanEncode helper funtion
   char* getFile() {return fileName;}
   void deleteAll(node* currPtr);        //deletes all the memory we've allocated
};

Heap::~Heap()
{
    deleteAll(heap[0]); //calls recursive delete because destructor can't be recursive
}

void Heap::deleteAll(node* currPtr)
{
   node* rPtr = currPtr->getRight();
   node* lPtr = currPtr->getLeft();
   
   if(rPtr != NULL)
   {
       deleteAll(rPtr);
   }
   if(lPtr != NULL )
   {
       deleteAll(lPtr);
   }
   delete currPtr;
}

Heap::Heap(char* file)
{
    for(int i = 0; i < 27; i++)
    {
        encodeMap[i] = "~";
    }
    fileName = file;
    charsUsed = 0; //setting the # of unique chars used
    unsigned i;
    int newChar;
    int iBuf;
    int *alph = new int[27]; //alphabet array - last index is for spaces
    ifstream read;
    string fileLine;
    read.open(fileName, ifstream::in); 
    getline(read, fileLine, '\n'); //fileLine becomes the string line that was read
    transform(fileLine.begin(), fileLine.end(), fileLine.begin(), ::tolower);
    
    
    // Reads one char at a time from string until '\n' and places them in *it
    for(string::iterator it = fileLine.begin(); it != fileLine.end(); ++it)
    {
        //cout << *it; //to read out the entire line for debugging purposes
        iBuf = *it - 96; //*it holds ASCII value already even as char, compiler knows this
        if(iBuf == -64) //is a space (32 - 97). 32 is ASCII for 'space', 97 is ASCII for 'a'
        {
            alph[0]++;
            if(alph[0] == 1) //check for unique space char used
            {
                charsUsed++;
            }
        }
        else if (iBuf >= 1 && iBuf < 27) //to stay in the range of lowercase letters only
        {
            alph[iBuf]++;
            if(alph[iBuf] == 1)
            {
                charsUsed++; //check for unique letters used
            }
        }
    }
    /*
    for (i = 0; i < 27; i++) //cout the character frequency array for easy checking
    {
        if (alph[i] > 0)
        {
            cout << "Letter '" << char(i + 96) << "' has freq: " << alph[i] << endl;
        }
    }
    cout << "chars used: " << charsUsed << endl;*/
    
    int charsAdded = 0;
    int oldMin = 0;
    int nextMin;
    while(charsAdded != charsUsed) //Builds the vector sorted by alph and then by ASCII
    {
        nextMin = INT_MAX;
        for(i = 0; i < 27; i++)
        {
            if(alph[i] > oldMin && alph[i] < nextMin)
            {
                nextMin = alph[i];
            }
        }
        for(i = 0; i < 27; i++)
        {
            if(alph[i] == nextMin)
            {
                if(i == 0)
                {
                    node* temp = new node(char(32),alph[i]);
                    insert(temp); //adds space char
                    charsAdded++;
                }
                else
                {
                    node* temp = new node(char(i + 96),alph[i]);
                    insert(temp); //adds all nodes
                    charsAdded++;
                }
            }
        }
       oldMin = nextMin;
    } 
    read.close();
}

void Heap::insert(node* element)
{
   heap.push_back(element);
   percolateUp(heap.size()-1);
}

node* Heap::deletemin()
{
   node* min = heap.front();  
   heap[0] = heap.at(heap.size()-1);    
   heap.pop_back();
   percolateDown(0);
   return min;
}

void Heap::print()
{
   cout << endl << endl;
   int i;
    for (i = 0; i < heap.size(); i++) //cout the character frequency array for easy checking
    {
        cout << heap[i]->getChar() << ":" << heap[i]->getFreq() << " ";
    }
    cout << endl;
    for(i = 0; i < heap.size(); i++) //prints out full list of children and parent
    {
       char lChar = (left(i) < 0) ? 45 : heap[left(i)]->getChar();
       char rChar = (right(i) < 0) ? 45 : heap[right(i)]->getChar();
       char pChar = (parent(i) < 0) ? 45 : heap[parent(i)]->getChar();
       
        cout << "i:" << setw(2) << i << " | " << "char:" << heap[i]->getChar() << " | " 
        << "parent:" << pChar << " | " << "left:" << lChar
        << " | " << "right:" << rChar << endl;
    }
}

void Heap::percolateUp(int index)
{
   int pIndex = parent(index);
   int pFreq = (pIndex < 0) ? -1 : heap[pIndex]->getFreq();
   int currFreq = heap[index]->getFreq();
   
   if(currFreq < pFreq)
   {
       node* temp = heap[index];
       heap[index] = heap[pIndex];
       heap[pIndex] = temp;
       percolateUp(pIndex);
   }
}

void Heap::percolateDown(int index)
{     
   int lIndex = left(index);
   int lFreq = (lIndex < 0) ? INT_MAX : heap[lIndex]->getFreq();
   int rIndex = right(index);
   int rFreq = (rIndex < 0) ? INT_MAX : heap[rIndex]->getFreq();
   int currFreq = heap[index]->getFreq();
   int nextIndex;
   int minFreqIndex;
   
   //continue if both children indexes exist and currFreq isn't smaller than both children
   if( (lIndex > 0 && currFreq > lFreq) || (rIndex > 0 && currFreq > rFreq) )
   {
       if(rIndex < 0)
           minFreqIndex = lIndex; //if only left child exists, set freq
       else if(lIndex < 0)
           minFreqIndex = rIndex; //if only right child exists, set freq(shouldn't happen)
       else
           minFreqIndex = (lFreq < rFreq) ? lIndex : rIndex; //if both are positive, set the lesser to minFreq
       
       node* temp = heap[index];
       heap[index] = heap[minFreqIndex];
       heap[minFreqIndex] = temp;
       percolateDown(minFreqIndex);       
   }
}

int Heap::left(int parent)
{
   int i = (parent*2) + 1; 
   return ( i < heap.size() ) ? i : -1;
}

int Heap::right(int parent)
{
   int i = (parent*2) + 2; 
   return ( i < heap.size() ) ? i : -1;
}

int Heap::parent(int child)
{
   if (child != 0)
   {
       int i = (child - 1) >> 1;
       return i;
   }
   return -1; 
}

void Heap::buildTrie()
{
   while(heap.size()>1)
   {
       node* catch1 = deletemin();
       //cout << endl << "Char Delete(right) - " << catch1->getChar() << ":" << catch1->getFreq();
       node* catch2 = deletemin();
       //cout << endl << "Char Delete(left) - " << catch2->getChar() << ":" << catch2->getFreq();

       node* temp = new node(catch1, catch2);
       //cout << endl << "New Node - " << temp->getChar() << ":" << temp->getFreq() << " - " << "rChild: " 
            //<< temp->getRight()->getChar() << ", lChild: " <<temp->getLeft()->getChar() << endl << endl;
       insert(temp);
   }
   huffmanEncode(heap[0]); //encodes the built trie
   huffmanDecode(heap[0]); //decodes lines past the string
}

void Heap::huffmanEncode(node* pin)
{
    node* p = pin;
	string str = "";
	huffmanWork(p,str);
    /*
    for(int i = 0; i < 27; i++)
    {
        if(encodeMap[i] != "~")
        {
            cout << char(i+96) << ":" << encodeMap[i] << endl;
        }
    }*/

	ifstream read;
	string fileLine;
	read.open(getFile(), ifstream::in); 
	getline(read, fileLine, '\n');
	string out = "";
  
	for(string::iterator it = fileLine.begin(); it < fileLine.end();++it)
	{
		if(*it == 32)
		{
			out = out + (encodeMap[0]);
		}
		else
		{
			out = out + (encodeMap[*it - 96]);
		}
	}
	cout << out << endl;
    read.close();
}

void Heap::huffmanWork(node*p, string str)
{
    if(p->getLeft() != NULL)
	{
		huffmanWork(p->getLeft(), str + "1");
	}
    
	if(p->getRight() != NULL)
	{
		huffmanWork(p->getRight(), str + "0");
	}

	if(p->getChar() == ' ')
	{
		encodeMap[0] = str;
	}
	else if(p->getChar() >= 97 && p->getChar() <= 122)
	{
        int i = p->getChar();
		encodeMap[i - 96] = str;
	}
}

void Heap::huffmanDecode(node* currPtr)
{
    node* p = currPtr;
	ifstream read;
	string fileLine;
	string out = "";
	read.open(getFile(), ifstream::in);
	getline(read, fileLine); //fileLine becomes the string line that was read	while(!read.eof())

	while(!read.eof())
	{
		getline(read,fileLine);
		node* curNode = currPtr;
		for(string::iterator it = fileLine.begin(); it != fileLine.end(); ++it)
		{
            char temp = *it;
			if(temp == '1')//may need .equals
			{
				if(curNode->getLeft() != NULL)
				{
					curNode = curNode->getLeft();
				}
				else
				{
					curNode = currPtr->getLeft();
				}
			}
			else
			{
				if(curNode->getRight() != NULL)
				{
					curNode = curNode->getRight();
				}
				else
				{
					curNode = currPtr->getRight();
				}
			}
            char c = curNode->getChar();
            if( ((c >= 97) && (c <= 122)) || (c == 32) )
            {
                out = out + c;
            }
		}
		cout << out << endl;
        out = "";
	}
}


int main(int argc, char *argv[])
{
   // Create the heap
   Heap* myheap = new Heap(argv[1]);
   myheap->buildTrie();
   delete myheap;
   return 0;
}
