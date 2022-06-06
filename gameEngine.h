#include "Player.h"
#include <io.h>
#include <fcntl.h>
#define CARDS 104
#define SIZE 101

class gameEngine{
	Card cards[104];
	CashierStack deck;
	CashierStack trash;
	Player** players;
	int howManyPlayers;
	Set** boardSets;
	int howManySets;

	//constructor aiding methods
	void packetInit();
	void deckInit();

	//turn and its aiding methods
	bool turn(Player& p);
	int startingChoice(Card** card_pointer);
	int operationChoice();
	void deletePlayerFromArr(char* p_name);
	void boardSetsPlusOne();
	void cardFromHandToSet(Player& p, Card* c);
	void cardFromSetToSet();
	Set** saveSetsBeforeTurn();
	Card** saveCardsBeforeTurn(Player& p);
	bool isRollback();
	void rollback(Set** s, int s_size, Card** c, int c_size, Player& p);
	bool endTurn(Set** s, int s_size, Card** c, Player& p);

	//start game methods
	void welcomeToRummy();
	void playersArrInit();

	//miscellaneous
	void getCard(int& val, char& ch);
	int getSetIdx();
	void printPlayerHand(Player& p);
	void printBoard();
	void printFirstTrashCard(Card* c);
	Card** thrownCardsToArr(Card** old_arr, int& size, Card* new_card);
	Card* extractCardFromPlayer(Player& p);

public:
	gameEngine();
	void startGame();
	~gameEngine();
};