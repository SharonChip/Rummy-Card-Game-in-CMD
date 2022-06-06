#pragma once
#include "card.h"
class stackItem{
	Card* card;
	stackItem* next;
public:
	stackItem(Card* c, stackItem* n=nullptr);
	Card* getCard()const;
	stackItem* getNextItem()const;
};

