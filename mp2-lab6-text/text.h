#pragma once
#include <fstream>
#include "stack.h"
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
	TStack<TNode*> st;
	TNode* ReadRec(ifstream& fin);
	int textLevel = 0;
	void PrintRec(TNode* p);
	void WriteRec(TNode* p, ostream& out);
public:
	void GoNextNode();
	void GoDownNode();
	void GoUp();
	void GoFirstNode();

	void InsNextLine(char* _str);
	void InsNextSection(char* _str);
	void InsDownLine(char* _str);
	void InsDownSection(char* _str);

	void DelNext();
	void DelDown();

	void Reset();
	void GoNext();
	bool IsEnd();

	char* GetCurrentLine();

	void Load(string fn);
	void Print();
	void Save(string fn);
};

inline void TText::WriteRec(TNode* p, ostream& out)
{
	if (p != nullptr) {
		out << p->str << '\n';
		if (p->pDown != nullptr) {
			out << "{\n";
			WriteRec(p->pDown, out);
			out << "\n}";
		}
		WriteRec(p->pNext, out);
	}
}

inline void TText::GoNextNode()
{
	if (pCurr != nullptr && pCurr->pNext != nullptr) {
		st.Push(pCurr);
		pCurr = pCurr->pNext;
	}
}

inline void TText::GoDownNode()
{
	if (pCurr != nullptr && pCurr->pDown != nullptr) {
		st.Push(pCurr);
		pCurr = pCurr->pDown;
	}
}

inline void TText::GoUp()
{
	if (!st.IsEmpty()) {
		TNode* prevNode = st.Pop();
		pCurr = prevNode;
	}
}

inline void TText::GoFirstNode()
{
	st.Clear();
	pCurr = pFirst;
}

inline void TText::InsNextLine(char* _str)
{
	if (pCurr != nullptr) {
		TNode* newNode = new TNode(_str);
		newNode->pNext = pCurr->pNext;
		pCurr->pNext = newNode;
	}
}

inline void TText::InsNextSection(char* _str)
{
	if (pCurr != nullptr) {
		TNode* newNode = new TNode(_str);
		newNode->pDown = pCurr->pNext;
		pCurr->pNext = newNode;
	}
}

inline void TText::InsDownLine(char* _str)
{
	if (pCurr != nullptr) {
		TNode* newNode = new TNode(_str);
		newNode->pNext = pCurr->pDown;
		pCurr->pDown = newNode;
	}
}

inline void TText::InsDownSection(char* _str)
{
	if (pCurr != nullptr) {
		TNode* newNode = new TNode(_str);
		newNode->pDown = pCurr->pDown;
		pCurr->pDown = newNode;
	}
}

inline void TText::DelNext()
{
	if (pCurr != nullptr) {
		TNode* pDel = pCurr->pNext;
		if (pDel != nullptr) {
			pCurr->pNext = pDel->pNext;
			delete pDel;
		}
	}
}

inline void TText::DelDown()
{
	if (pCurr != nullptr) {
		TNode* pDel = pCurr->pDown;
		if (pDel != nullptr) {
			pCurr->pNext = pDel->pNext;
			delete pDel;
		}
	}
}
