/*
 * huffmanencoding.cpp

 *
 *  Created on: Feb 14, 2014
 *      Author: Giorulesall
 */
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>


using namespace std;
class node;

class trie
{
private:
	string input;//The string which is used to set the architecture of the trie
	int freq[];
	int nodes;
	node* root;
public:
	trie()
	{
		freq = new int[27];//last index is for spaces
		nodes = 0;
	}
	void addNode(node add)
	{

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

		int charsAdded;
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
					addNode(node());//FIX ARGUMENTS
					charsAdded++;
				}
			}
			oldMin = nextMin;

		}
	}
};

class node
{
private:
	char elem;
	int freq;
	node* parent; //itself if root
	node* left;
	node* right;


public:
node()
{

}
node(node* p, node* l, node* r, int f)
{
	parent = p;
	left = l;
	right = r;
	freq = f;
}


};


int main (string fileName)
{

	return 0;
}



