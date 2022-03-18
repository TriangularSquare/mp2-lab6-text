#pragma once
#include <fstream>
#include "stack .h"
using namespace std;

struct TNode {
	char str[81];
	TNode* pNext, * pDown;
	TNode(char* _str = nullptr, TNode* _pNext = nullptr, TNode* _pDown = nullptr);
};

inline TNode::TNode(char* _str, TNode* _pNext, TNode* _pDown)
{
	pNext = _pNext;
	pDown = _pDown;
	if (_str == nullptr)
		str[0] = '\0';
	else
		strcpy(str, _str);
}


class TText {
	TNode* pFirst, * pCurr;
	TStack<TNode> st;
	TNode* ReadRec(ifstream& fin);
	int textLevel = 0;
	void PrintRec(TNode* p);
public:

};
