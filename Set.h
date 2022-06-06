#include "cashierStack.h"

class Set {
	Card** cards;
	int howManyCards;
public:
	Set();
	Set(const Set &s);
	void addCard(Card* c);
	Card* extractCard(char char_c, int int_val);
	bool isSameValueSerial() const;
	bool isSequential() const;
	bool isValidSet() const;
	void printSet() const;
	bool isEmpty() const;
	~Set();
};