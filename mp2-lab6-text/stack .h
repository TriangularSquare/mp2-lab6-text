#pragma once

template <class T>
class TStack {
	T* mas;
	int size;
	int pos;
public:
	TStack(int _size = 10);
	~TStack();
	TStack(const TStack& ts);

	bool IsEmpty();
	bool IsFull();
	void Clear();

	void Push(T elem);
	T Pop();
	T Top();

	bool operator==(const TStack& ts) const;
	bool operator!=(const TStack& ts) const;
	TStack& operator=(const TStack& ts);

	/*
	friend ostream& operator<<(ostream& out, const TStack& ts) {
		for (int i = 0; i <= ts.pos; i++) {
			out << ts.arr[i] << ' ';
		}
		return out;
	}*/
};

template <class T>
TStack<T>::TStack(int _size) {
	if (_size <= 0) {
		throw "ERROR";
	}
	size = _size;
	mas = new T[size];
	pos = -1;
}

template <class T>
TStack<T>::~TStack() {
	delete[] mas;
}

template <class T>
TStack<T>::TStack(const TStack& ts) {
	size = ts.size;
	pos = ts.pos;
	mas = new T[size];
	for (int i = 0; i <= pos; i++) {
		mas[i] = ts.mas[i];
	}
}

template <class T>
bool TStack<T>::IsEmpty() {
	return (pos == -1);
}

template<class T>
inline bool TStack<T>::IsFull()
{
	return (pos == size - 1);
}

template <class T>
void TStack<T>::Push(T elem) {
	if (IsFull()) {
		throw "ERROR";
	}

	pos++;
	mas[pos] = elem;
}

template<class T>
inline void TStack<T>::Clear()
{
	pos = -1;
}

template <class T>
inline T TStack<T>::Pop() {
	if (IsEmpty()) {
		throw "ERROR";
	}

	T elem = mas[pos];
	pos--;
	return elem;
}

template<class T>
inline T TStack<T>::Top()
{
	if (IsEmpty()) {
		throw "ERROR";
	}

	return mas[pos];
}

template<class T>
inline bool TStack<T>::operator==(const TStack& ts) const
{
	if (size != ts.size) {
		return false;
	}

	for (int i = 0; i < size; i++) {
		if (mas[i] != ts.mas[i]) {
			return false;
		}
	}

	return true;
}

template<class T>
inline bool TStack<T>::operator!=(const TStack& ts) const
{
	return !(this == &ts);
}

template<class T>
TStack<T>& TStack<T>::operator=(const TStack& ts)
{
	if (size != ts.size) {
		delete[] mas;
		size = ts.size;
		mas = new T[size];
	}
	pos = ts.pos;

	for (int i = 0; i <= pos; i++) {
		mas[i] = ts.mas[i];
	}
	return *this;
}