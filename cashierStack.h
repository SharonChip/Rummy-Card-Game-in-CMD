#pragma once
#include "stackItem.h"
#include <iostream>
class CashierStack {
private:
	stackItem* head;
	int numInDeck;
public:
	CashierStack();
	~CashierStack();
	Card* pop();
	void push(Card* c);
	void shuffle();
	void shuffleNitem(int n);
	void printStackForDeBugOnly()const;
};
