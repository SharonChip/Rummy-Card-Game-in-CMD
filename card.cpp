#include "Card.h"
#include <iostream>
using namespace std;

#include <io.h>
#include <fcntl.h>


void Card::setCard(char c, int v)  {
	sign = c;
	value = v;
}

void Card::printCard() const{

	_setmode(_fileno(stdout), _O_U16TEXT);
	switch (sign) {
	case 'H':
		wcout << HEART;
		break;
	case 'S':
		wcout << SPADE;
		break;
	case 'C':
		wcout << CLUB;
		break;
	case 'D':
		wcout << DIAMOND;
		break;
	}
	_setmode(_fileno(stdout), _O_TEXT);
	switch (value) { 
	case 11:
		cout << " J";
		break;
	case 12:
		cout << " Q";
		break;
	case 13:
		cout << " K";
		break;
	case 1:
		cout << " A";
		break;
	default: cout << " " << (int)value;
	
	}
		
		
}

char Card::getSign() const
{
	return sign;
}

int Card::getValue() const
{
	return value;
}
#ifdef DEBUG
Card::~Card()
{
	cout << "\n Kill ";
	printCard();
}
#endif // DEBUG

