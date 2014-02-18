/*
 * huffmanencoding.cpp
*/
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>


using namespace std;
class node;

class node
{
private:
	char elem;
	int freq;//freq = 0 if a hypothetical joint node(inner node), used for sorting
	node* parent; //itself if root
	node* left;
	node* right;


public:
node()
{

}
node(char c, int f)
{
	elem = c;
	freq = f;//if = 0 then is an inner node
}
node(node* p, node* l, node* r,char c, int f)
{
	parent = p;
	left = l;
	right = r;
	elem = c;
	freq = f;
}
void setLeft(node* l)
{
	left = l;
}
void setRight(node* r)
{
	right = r;
}
void setParent(node* p)
{
	parent = p;
}
node* getLeft()
{
	return left;
}
node* getRight()
{
	return right;
}
node* getParent()
{
	return parent;
}
int getFreq()
{
	return freq;
}
char getChar()
{
	return elem;
}
char getElem()
{
	return getChar();
}





};

class trie
{
private:
	string input;//The string which is used to set the architecture of the trie
	int* freq;
	int nodeCount;
	vector<node> nodes;
public:
	trie()
	{
		freq = new int[27];//last index is for spaces
		nodeCount = 0;
	}
	void addInitialNode(node add)
	{

		nodes.push_back(add);//do only if didnt add an inner node
	}


	void setInput(string fileName)
	{
		unsigned i;
		ifstream read;
		read.open(fileName.c_str(), ifstream::in); //converts string to cstring to match parameter of open()
		getline(read, fileName, '\n');
		transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
		int charsUsed = 0;
		int newChar;
		int iBuf;

		// C++11 range-based loop (makes things simple and easy)
		// Reads one char at a time from string until '\n' and places them in c
		for(char& c : fileName)
		{
			cout << c; //to read out the entire line for debugging purposes
			iBuf = c - 97; //c holds ASCII value already even as char, compiler knows this
			if(iBuf == -65) //is a space (32 - 97). 32 is ASCII for 'space', 97 is ASCII for 'a'
			{
				freq[26]++;
				if(freq[26] == 1) //check for unique space char used
				{
					charsUsed++;
				}
			}
			else
			{
				freq[iBuf]++;
				if(freq[iBuf] == 1)
				{
					charsUsed++; //check for unique letters used
				}
			}
		}
		cout << endl;

		for (i = 0; i < 27; i++) //cout the character frequency array for easy checking
		{
			if (freq[i] > 0)
			{
				cout << "Letter '" << char(i + 97) << "' has freq: " << freq[i] << endl;
			}
		}
		cout << endl; // end of cout for easy checking

		int charsAdded = 0;
		int oldMin = 0;
		int nextMin;
		while(charsAdded != charsUsed)
		{
			nextMin = INT_MAX;
			for(i = 0; i < 27; i++)
			{
				if(freq[i] > oldMin && freq[i] < nextMin)
				{
					nextMin = freq[i];
				}
			}
			for(i = 0; i < 27; i++)
			{
				if(freq[i] == nextMin)
				{
					//node temp = ;
					addInitialNode(node(char(i + 97),freq[i]));
					charsAdded++;
				}
			}
			oldMin = nextMin;
		}

		if(nodes.size() > 1)
		{
			i = 0;
			auto start = nodes.begin();
			int temp,offset;

			while(i < nodes.size() - 1)
			{
				temp = ((node)node[i]).getFreq() + ((node)node[i]).getFreq();
				offset = 0;
				while(temp > ((node)node[i+offset]).getFreq())
				{
					offset++;

				}
				node tempNode = node('\n',((node)nodes[i]).getFreq() + ((node)nodes[i+1]).getFreq());
				nodes.insert(start+i+offset,tempNode);
				i+=2;
			}
		}
		//if i is last element, then is already sorted

		///assign the correct parents and left/right.
		for(i = 0; i < nodes.size();i++)
		{
			if((i + 1) / 2 >= 0)//has parent,allows truncation
			{
				node* tempN = &nodes[ceil(i / 2 - 1)]
				((node)nodes[i]).setParent(tempN);
			}
			if((i+1) * 2 - 1 < nodes.size()) // has left
			{
				node* tempN = nodes[(i+1) * 2 - 1]
			   ((node)nodes[i]).setLeft(tempN);
			}

			if((i+1) * 2 < nodes.size())
			{
				node* temp = &nodes[(i+1) * 2]
				((node)nodes[i]).setRight(temp);
			}
		}



};




int main (string fileName)
{

	return 0;
}



