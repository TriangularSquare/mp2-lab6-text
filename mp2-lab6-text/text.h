#pragma once
#include <fstream>
#include "stack.h"
using namespace std;

struct TNode;
class TText;

struct TMem {
	TNode* pFirst;
	TNode* pFree;
	TNode* pLast;
};

struct TNode {
	char str[81];
	TNode* pNext, * pDown;

	bool Garbage;

	static TMem mem;

	TNode(char* _str = nullptr, TNode* _pNext = nullptr, TNode* _pDown = nullptr);

	void* operator new(size_t size);
	void operator delete(void* ptr);

	static void InitMem(size_t size);
	static void CleanMem(TText& t);

	static void PrintFreeNodes();
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

inline void* TNode::operator new(size_t size)
{
	TNode* tmp = mem.pFree;
	if (tmp == nullptr)
		throw "Out of memory";
	mem.pFree = mem.pFree->pNext;
	return tmp;
}

inline void TNode::operator delete(void* ptr)
{
	TNode* tmp = mem.pFree;
	TNode* _ptr = (TNode*)ptr;
	_ptr->pNext = tmp;
	mem.pFree = _ptr;
}

inline void TNode::InitMem(size_t size)
{
	mem.pFirst = (TNode*) new char[size * sizeof(TNode)];

	mem.pFree = mem.pFirst;
	mem.pLast = mem.pFirst + (size - 1);

	TNode* _ptr = mem.pFirst;
	for (int i = 0; i < size - 1; i++)
	{
		_ptr->pNext = _ptr + 1;
		_ptr->str[0] = 0;
		_ptr->Garbage = true;

		_ptr += 1;
	}

	mem.pLast->pNext = nullptr;
	mem.pLast->str[0] = 0;
}

inline void TNode::PrintFreeNodes()
{
	TNode* _ptr = mem.pFree;
	while (_ptr != nullptr)
	{
		if (_ptr->str[0] != '\0')
			cout << _ptr->str << '\n';
		_ptr = _ptr->pNext;
	}
}


class TText {
	TNode* pFirst, * pCurr;
	TStack<TNode*> st;

	TNode* ReadRec(ifstream& fin);

	int textLevel = 0;

	void PrintRec(TNode* p);
	void WriteRec(TNode* p, ostream& out);

	TNode* CopyNode(TNode* p);
public:
	TText();
	TText(TNode* tn);

	TText* GetCopy();

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

	void NotGarbage();
};

inline TNode* TText::ReadRec(ifstream& is)
{
	TNode* pTemp = nullptr, * pHead = nullptr;
	char str[81];

	while (!is.eof())
	{
		is.getline(str, 81, '\n');
		if (str[0] == '{')
			pTemp->pDown = ReadRec(is);
		else if (str[0] == '}')
			break;
		else if (strcmp(str, "") == 0)
			continue;
		else
		{
			TNode* newNode = new TNode(str);
			if (pHead == nullptr)
				pTemp = pHead = newNode;
			else
				pTemp->pNext = newNode;
			pTemp = newNode;
		}
	}
	return pHead;
}

inline void TText::PrintRec(TNode* p)
{
	if (p != nullptr)
	{
		for (int i = 0; i < textLevel; i++)
			cout << "   ";

		if (p == pCurr) 
			cout << "*";
		else 
			cout << " ";

		cout << p->str << '\n';

		textLevel++;
		PrintRec(p->pDown);
		textLevel--;
		PrintRec(p->pNext);
	}
}

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

inline TNode* TText::CopyNode(TNode* p)
{
	TNode* pd, * pn, * pCopy;

	if (p->pDown != nullptr)
		pd = CopyNode(p->pDown);
	else pd = nullptr;

	if (p->pNext != nullptr)
		pn = CopyNode(p->pNext);
	else pn = nullptr;

	pCopy = new TNode(p->str, pn, pd);
	return pCopy;
}


inline TText::TText()
{

}

inline TText::TText(TNode* tn)
{
	pFirst = tn;
	pCurr = nullptr;
}

inline TText* TText::GetCopy()
{
	TText* res;
	res = new TText(CopyNode(pFirst));
	return res;
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

inline void TText::Reset()
{
	st.Clear();

	if (pFirst != nullptr)
	{
		pCurr = pFirst;
		st.Push(pCurr);
		if (pCurr->pNext != nullptr)
			st.Push(pCurr->pNext);
		if (pCurr->pDown != nullptr)
			st.Push(pCurr->pDown);
	}
}

inline void TText::GoNext()
{
	pCurr = st.Pop();

	if (pCurr != pFirst)
	{
		if (pCurr->pNext != nullptr)
			st.Push(pCurr->pNext);
		if (pCurr->pDown != nullptr)
			st.Push(pCurr->pDown);
	}
}

inline bool TText::IsEnd()
{
	return st.IsEmpty();
}

inline char* TText::GetCurrentLine()
{
	return pCurr->str;
}

inline void TText::Load(string fn)
{
	ifstream ifs(fn);
	pFirst = ReadRec(ifs);
}

inline void TText::Print()
{
	PrintRec(pFirst);
}

inline void TText::Save(string fn)
{
	std::ofstream out;
	out.open(fn);

	if (!out.is_open()) throw "Export exception!";

	WriteRec(pFirst, out);
}

inline void TText::NotGarbage()
{
	pCurr->Garbage = false;
}

inline void TNode::CleanMem(TText& t)
{
	for (t.Reset(); !t.IsEnd(); t.GoNext())
	{
		t.NotGarbage();
	}

	TNode* p = TNode::mem.pFree;
	while (p != nullptr)
	{
		p->Garbage = false;
		p = p->pNext;
	}

	p = TNode::mem.pFirst;
	for (p = TNode::mem.pFirst; p <= TNode::mem.pLast; p++)
	{
		if (p->Garbage)
		{
			delete p;
			p->Garbage = false;
		}
	}
}
