#include "Set.h"

class Player{
	char* name;
	Card** hand;
	int numInHand;
public:
	void setName(char* name);
	void setCard(Card* card);
	Card* extractCard(char sign = -1, int value = -1);
	char* getName() const;
	int getNumberOfCards() const;
	void printHand() const;
	Player(char* name);
	~Player();
};

