#include "stackItem.h"


stackItem::stackItem(Card * c, stackItem* n)
{
	card = c;
	next = n;
}

Card * stackItem::getCard() const
{
	return card;
}

stackItem * stackItem::getNextItem() const
{
	return next;
}
