#pragma once
#define SPADE L"\u2660"
#define CLUB L"\u2663"
#define HEART L"\u2665"
#define DIAMOND L"\u2666"

class Card {
private:
	char sign;
	int value;
public:
	void setCard(char c, int v);
	void printCard() const;
	char getSign()const;
	int getValue()const;
#ifdef DEBUG
	~Card();
#endif
};
