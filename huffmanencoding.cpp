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
	int freq[];
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
		int i;
		ifstream read;
		read.open((char*)fileName, ifstream::in);
		getline(read,fileName, '\n');
		transform(fileName.begin(), fileName.end(), fileName.begin(), ::tolower);
		int charsUsed = 0;
		int newChar;
		int iBuf;
		for(i = 0; i < fileName.length();i++)
		{
			iBuf = (int)fileName.substr()(i,1) - 97;
			if(iBuf == 32)//is a space
			{
				if(!(++freq[26] - 1))//if freq[index] == 1
				{
					charsUsed++;
				}
			}
			else
			{
				if(!(++freq[iBuf] - 1))//if freq[index] == 1
				{
					charsUsed++;
				}
			}
		}

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
		///assign the correct parents and left/right.
		for(i = 0; i < nodes.size();i++)
		{
			if((i + 1) / 2 >= 0)//has parent,allows truncation
			{
				node* temp = &nodes[ceil(i / 2 - 1)]
				((node)nodes[i]).setParent(temp);
			}
			if((i+1) * 2 - 1 < nodes.size()) // has left
			{
				node* temp = &nodes[(i+1) * 2 - 1]
				((node)nodes[i]).setLeft(temp);
			}

			if((i+1) * 2 < nodes.size())
			{
				node* temp = &nodes[(i+1) * 2]
				((node)nodes[i]).setRight(temp);
			}
		}

		//now reorder 2 min, and repeat until list cleared


	}
};




int main (string fileName)
{

	return 0;
}



