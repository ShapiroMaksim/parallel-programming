#pragma once

#include "TLink.h"

template<class T>
class THeadList {
protected:
	TLink<T>* pHead, * pFirst, * pPrev, * pCurr, * pLast;
	int pos, size;
public:
	THeadList();
	void InsFirst(T elem);
	void InsCurr(T elem);
	void InsLast(T elem);
	void DelCurr();
	void Reset();
	void GoNext();
	bool IsEnd();
	bool IsEmpty();
	int GetCurrVal();
	~THeadList();
};

template<class T>
THeadList<T>::THeadList() {
	TLink<T>* tmp = new TLink<T>;
	pHead = tmp;
	pHead->pNext = tmp;
	pFirst = pCurr = pPrev = pLast = nullptr;
	pos = -1;
	size = 0;
}

template<class T>
void THeadList<T>::InsFirst(T elem) {
	TLink<T>* tmp = new TLink<T>;
	tmp->val = elem;
	if (size > 0) {
		pHead->pNext = tmp;
		tmp->pNext = pFirst;
		pFirst = tmp;
		pLast = tmp;
		size++;
		pos++;
	}
	else {
		pHead->pNext = tmp;
		tmp->pNext = pHead;
		pFirst = tmp;
		pLast = tmp;
		size++;
		pos = 0;
		pCurr = pFirst;
	}
}

template<class T>
void THeadList<T>::InsCurr(T elem) {
	if (this->size == 0) {
		InsFirst(elem);
	}
	else {

		TLink<T>* tmp = new TLink<T>;
		tmp->val = elem;
		pPrev->pNext = tmp;
		tmp->pNext = pCurr;
		pCurr = tmp;
		pFirst = pHead->pNext;
		size++;
	}
}

template<class T>
void THeadList<T>::InsLast(T elem) {
	TLink<T>* tmp = new TLink<T>;
	tmp->val = elem;
	pCurr = pLast;
	pCurr->pNext = tmp;
	pLast = tmp;
	pLast->pNext = pHead;
	size++;
}

template<class T>
void THeadList<T>::DelCurr() {
	if (pCurr != pHead && size > 0) {
		TLink<T>* tmp = pCurr;
		pPrev->pNext = pCurr->pNext;
		delete tmp;
		pCurr = pPrev->pNext;
		pFirst = pHead->pNext;
		if (pCurr == pHead)
			pLast = pPrev;
		size--;
	}
	else
		throw - 1;
}

template<class T>
void THeadList<T>::Reset() {
	pCurr = pFirst;
	pPrev = pHead;
	pos = 0;
}

template<class T>
void THeadList<T>::GoNext() {
	pPrev = pCurr;
	pCurr = pCurr->pNext;
	pos++;
}

template<class T>
bool THeadList<T>::IsEnd() {
	return pCurr == pHead;
}

template<class T>
bool THeadList<T>::IsEmpty() {
	return this->size == 0;
}

template<class T>
int THeadList<T>::GetCurrVal() {
	return pCurr->val;
}

template<class T>
THeadList<T>::~THeadList() {
	pCurr = pFirst;
	while (pCurr != pHead) {
		pPrev = pCurr;
		pCurr = pCurr->pNext;
		delete pPrev;
	}
	delete pHead;
}