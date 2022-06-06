#include "gameEngine.h"
using namespace std;

//constructor and its aiding methods
gameEngine::gameEngine(){
	//initialize 104 cards
	packetInit();
	//build the game deck and shuffle
	deckInit();
	//rest of properties set to 0
	players = NULL;
	howManyPlayers = 0;
	boardSets = NULL;
	howManySets = 0;
}
void gameEngine::packetInit(){
	//for 4 different signs, we make 4*2 skips (2 decks) over both decks
	int iterations = CARDS + 8;
	int times = 0, k = 0;
	char signs[4] = { 'H', 'D', 'S', 'C' };
	for (int j = 1; j < iterations; j++) {
		if (j % 14 == 0) {
			//every 26+2(=iterations/4) iterations, move the signs array index one step forward
			if (j % 28 == 0)
				k++;
			//every 13+1 iterations, since 14%14=0 and we need 1, move j once forward
			j++;
			times++;
		}
		cards[j - 1 - times].setCard(signs[k], j % 14);
	}
}
void gameEngine::deckInit(){
	//move all the cards to the cashier deck
	for (int i = 0; i < CARDS; i++)
		deck.push(&cards[i]);
	//shuffle the cashier deck
	deck.shuffle();
}
gameEngine::~gameEngine(){
	//when the compiler frees any allocated memory, it automatically calls
	//for the object's destructor
	int i;
	//freeing the players array
	for (i = 0; i < howManyPlayers; i++)
		delete players[i];
	delete[] players;
	//freeing the sets array
	for (i = 0; i < howManySets; i++)
		delete boardSets[i];
	delete[] boardSets;
	//note: included classes are automatically destructed, such as
	//CashierStack deck, trash;
}

//turn and its aiding methods
bool gameEngine::turn(Player& p) {
	//starting turn
	int choice = -1;
	Card* temp_card = NULL;
	//printing board, greeting message, player's hand and trash card
	cout << "\n~~~~~~~~ " << p.getName() << "'s turn" << " ~~~~~~~~\n\n";
	printBoard();
	printPlayerHand(p);
	choice = startingChoice(&temp_card);
	switch (choice) {
		//drawing a card from deck
		case 1:
			p.setCard(temp_card);
			break;
		//drawing a card from trash
		case 2:
			p.setCard(temp_card);
			break;
		//quitting game
		case 3:
			deletePlayerFromArr(p.getName());
			return true;
	}
	//before main turn sequence, save all current sets and the player's hand
	Set** sets_before_turn = saveSetsBeforeTurn();
	int how_many_sets_before_turn = howManySets;
	Card** thrown_cards = NULL;
	int thrown_cards_size = 0;
	//main turn sequence
	while (choice) {
		printPlayerHand(p);
		choice = operationChoice();
		switch (choice) {
			//create a new set
			case 1:
				boardSetsPlusOne();
				break;
			//add a card from your hand to an existing set
			case 2:
				//save card in this scope to use between methods
				cout << "Pick a card to add to a set (example: C 3): ";
				temp_card = extractCardFromPlayer(p);
				thrown_cards = thrownCardsToArr(thrown_cards, thrown_cards_size, temp_card);
				cardFromHandToSet(p, temp_card);
				break;
			//allocate between existing sets
			case 3:
				cardFromSetToSet();
				break;
			//show game board
			case 4:
				printBoard();
				break;
			//end turn
			case 0:
				if (isRollback()) {
					rollback(sets_before_turn, how_many_sets_before_turn, thrown_cards, thrown_cards_size, p);
					//update properties in case another rollback will be required
					choice = -1;
					sets_before_turn = saveSetsBeforeTurn();
					how_many_sets_before_turn = howManySets;
					thrown_cards = NULL;
					thrown_cards_size = 0;
				}
				else {
					if (endTurn(sets_before_turn, how_many_sets_before_turn, thrown_cards, p))
						return true;
					return false;
				}
				break;
		}
	}
}
int gameEngine::startingChoice(Card** card_pointer) {
	int choice = -1;
	Card* temp_card = trash.pop();
	//print first trash card
	if (temp_card) {
		cout << "First card from the trash deck: ";
		temp_card->printCard();
		cout << endl << endl;
	}
	else
		cout << "Trash deck is currently empty.\n\n";
	//starting turn menu
	while (choice < 1 || 3 < choice) {
		cout << "What would you like to do?\n";
		cout << "1. Draw a card from the cashier deck.\n";
		cout << "2. Draw a card from the trash deck.\n";
		cout << "3. Quit game.\n";
		cout << "Your choice: ";
		cin.ignore();
		cin >> choice;
		cout << endl;
		//check for valid choice values, if not then get input again
		if (choice < 1 || 3 < choice) {
			cout << "Invalid choice, try again.\n\n";
			continue;
		}
		switch (choice) {
		//place the trash card back and get the next card from the cashier deck
		case 1:
			if (temp_card)
				trash.push(temp_card);
			temp_card = deck.pop();
			if (!temp_card) {
				cout << "Cashier deck is empty, try again.\n\n";
				choice = -1;
			}
			else
				*card_pointer = temp_card;
			break;
		//trash card is already out	
		case 2:
			if (!temp_card) {
				cout << "Trash deck is empty, try again.\n\n";
				choice = -1;
			}
			else
				*card_pointer = temp_card;
			break;
		//place the trash card back
		case 3:
			trash.push(temp_card);
			*card_pointer = NULL;
			break;
		}
	}
	return choice;
}
int gameEngine::operationChoice() {
	int choice = -1;
	//operation menu
	while (choice < 0 || 4 < choice) {
		cout << "What would you like to do?\n";
		cout << "1. Create a new set.\n";
		cout << "2. Add a card from your hand to an existing set.\n";
		cout << "3. Allocate between existing sets.\n";
		cout << "4. Show game board.\n";
		cout << "0. End your turn.\n";
		cout << "Your choice: ";
		cin.clear();
		cin >> choice;
		cout << endl;
		//check for valid choice values, if not then get input again
		if (choice < 0 || 4 < choice)
			cout << "Invalid choice, try again.\n\n";
		//if a player wishes to move cards to or from sets and there are none yet,
		//print an error and input again
		if (choice == 2 || choice == 3) {
			if (!howManySets) {
				cout << "Error, no sets created yet.\n\n";
				choice = -1;
				continue;
			}
		}
	}
	return choice;
}
void gameEngine::deletePlayerFromArr(char* p_name){
	Player** temp = new Player * [howManyPlayers - 1];
	int i, k = 0, idx;
	//find player p_name's index in the array and copy the old array
	for (i = 0; i < howManyPlayers; i++) {
		//if the player is found, break the loop and continue from the same
		//place in another loop
		if (!strcmp(players[i]->getName(), p_name)) {
			idx = i++;
			break;
		}
		//in case player has not been found in the array
		//delete the un-used memory
		if (k == howManyPlayers - 1) {
			delete[] temp;
			return;
		}
		temp[k++] = players[i];
	}
	//finish the loop if the player has been found
	for (; i < howManyPlayers; i++)
		temp[k++] = players[i];
	//return the player's cards to the deck, shuffle and desturct
	int numOfCards = players[idx]->getNumberOfCards();
	for (int i = 0; i < numOfCards; i++) {
		deck.push(players[idx]->extractCard());
		deck.shuffle();
	}
	cout  << p_name << " has left the game.\n\n";
	delete players[idx];
	//update the rest of the properties
	howManyPlayers--;
	delete[] players;
	players = temp;
}
void gameEngine::boardSetsPlusOne(){
	//create new array and import the elements
	Set** temp = new Set * [howManySets + 1];
	for (int i = 0; i < howManySets; i++)
		temp[i] = boardSets[i];
	temp[howManySets] = new Set;
	//update the rest of the properties
	howManySets++;
	delete[] boardSets;
	boardSets = temp;
}
void gameEngine::cardFromHandToSet(Player& p, Card* c){
	if (!c)
		return;
	int set_num = -1;
	//check if there are any sets yet
	if (howManySets == 0)
		return;
	cout << "Pick a set to add the card to: ";
	set_num = getSetIdx();
	cout << endl;
	boardSets[set_num]->addCard(c);
}
void gameEngine::cardFromSetToSet(){
	//check if there are at least 2 sets before input from player
	if (howManySets == 0)
		return;
	if (howManySets == 1) {
		cout << "Cannot move cards from a set to itself.\n\n";
		return;
	}
	int number, from_set, to_set;
	char sign;
	number = sign = from_set = to_set = -1;
	Card* temp_card = NULL;
	//get sets and card from the user
	cout << "Pick a set to move a card from: ";
	from_set = getSetIdx();
	cout << "Pick a set to move a card to: ";
	to_set = getSetIdx();
	if (to_set == from_set) {
		cout << "Cannot move cards from a set to itself.\n\n";
		return;
	}
	if (boardSets[from_set]->isEmpty()) {
		cout << "The requested set is empty.\n\n";
		return;
	}
	cout << "Pick a card to remove from the set: ";
	getCard(number, sign);
	//move from 'from_set' to 'to_set'
	temp_card = boardSets[from_set]->extractCard(sign, number);
	while (!temp_card) {
		getCard(number, sign);
		temp_card = boardSets[from_set]->extractCard(sign, number);
	}
	boardSets[to_set]->addCard(temp_card);
}
Set** gameEngine::saveSetsBeforeTurn(){
	if (boardSets == NULL || howManySets == 0)
		return NULL;
	//create a new array of sets, and for each set in the array
	//use the copy constructor to initialize it
	Set** copy = new Set * [howManySets];
	for (int i = 0; i < howManySets; i++)
		copy[i] = new Set(*boardSets[i]);
	return copy;
}
Card** gameEngine::saveCardsBeforeTurn(Player& p){
	//create an array of pointers to cards
	int size = p.getNumberOfCards();
	Card** copy = new Card * [size];
	//get each card from the player's hand into the array (as a pointer!)
	for (int i = 0; i < size; i++)
		copy[i] = p.extractCard();
	//return the cards back in order
	for (int i = 0; i < size; i++)
		p.setCard(copy[i]);
	return copy;
}
Card** gameEngine::thrownCardsToArr(Card** old_arr, int& size, Card* new_card) {
	if (size < 0 || !new_card)
		return NULL;
	Card** new_arr = new Card * [size + 1];
	//empty
	if (!old_arr)
		new_arr[0] = new_card;
	//not empty
	else {
		for (int i = 0; i < size; i++) {
			new_arr[i] = old_arr[i];
		}
		new_arr[size] = new_card;
	}
	delete[] old_arr;
	size++;
	return new_arr;
}
Card* gameEngine::extractCardFromPlayer(Player& p){
	if (p.getNumberOfCards() == 0)
		return NULL;
	int number = -1;
	char sign = -1;
	//get card info from player
	Card* temp_card = NULL;
	getCard(number, sign);
	temp_card = p.extractCard(sign, number);
	//check if card in hand
	while (!temp_card) {
		getCard(number, sign);
		temp_card = p.extractCard(sign, number);
	}
	return temp_card;
}
bool gameEngine::isRollback(){
	for (int i = 0; i < howManySets; i++) {
		if (!boardSets[i]->isValidSet()) {
			cout << "One or more of the sets are not valid. Initiating rollback.\n\n";
			return true;
		}
	}
	return false;
}
void gameEngine::rollback(Set** s, int s_size, Card** c, int c_size, Player& p){
	//board
	int i;
	for (i = 0; i < howManySets; i++)
		delete boardSets[i];
	delete[] boardSets;
	boardSets = s;
	howManySets = s_size;
	//player hand
	for (i = 0; i < c_size; i++)
		p.setCard(c[i]);
	delete[] c;
}
bool gameEngine::endTurn(Set** s, int s_size, Card** c, Player& p){
	int number = -1, i;
	char sign = -1;
	Card* temp_card = NULL;
	//free the sets in the sets pointers array
	for (i = 0; i < s_size; i++)
		delete s[i];
	//free the sets pointers array and cards pointers array
	delete[] s;
	delete[] c;
	//check if at least 1 card remains
	if (1 <= p.getNumberOfCards()) {
		cout << "Pick a card to discard to the trash deck (example: C 3): ";
		temp_card = extractCardFromPlayer(p);
		trash.push(temp_card);
	}
	//if no cards are left in the player's hand after discarding one, the player wins
	if (p.getNumberOfCards() == 0)
		return false;
	return true;
}

//start game method
void gameEngine::startGame(){
	//greeting message
	welcomeToRummy();
	//get players from user and initialize array
	playersArrInit();
	//add one card to the trash deck
	trash.push(deck.pop());
	//starting game
	int i, game = 1, num_of_players_before_player_left = howManyPlayers;
	while (game) {
		for (i = 0; i < howManyPlayers; i++){
			//check if only 1 player remains
			if (howManyPlayers == 1) {
				i = game = 0;
				break;
			}
			//continuing game
			//initiate next player's turn if they still have cards
			//note: operator || evaluates left operand first and if the value
			//is logically true then it avoids evaluating the right operand.
			if (players[i]->getNumberOfCards() == 0 || !turn(*players[i])) {
				game = 0;
				break;
			}
			//if the player left, move the turn accordingly by changing i
			if (num_of_players_before_player_left != howManyPlayers) {
				//if the last player in the array left, no changes are required
				if (i == num_of_players_before_player_left - 1) {
					num_of_players_before_player_left = howManyPlayers;
					continue;
				}
				//if a player who isn't last in the array left the game
				//we need to move the index back one time
				else {
					num_of_players_before_player_left = howManyPlayers;
					i--;
				}
			}
		}
	}
	//winner declaration
	cout << endl << endl << players[i]->getName();
	cout << " is the winner!\n";
}
void gameEngine::welcomeToRummy(){
	//greeting message
	cout << "\t";
	_setmode(_fileno(stdout), _O_U16TEXT);
	wcout << HEART << CLUB << DIAMOND << SPADE;
	_setmode(_fileno(stdout), _O_TEXT);
	cout << " Welcome to Rummy ";
	_setmode(_fileno(stdout), _O_U16TEXT);
	wcout << SPADE << DIAMOND << CLUB << HEART;
	_setmode(_fileno(stdout), _O_TEXT);
	cout << endl << endl;
}
void gameEngine::playersArrInit(){
	//get number of players (at least 2)
	while (howManyPlayers < 2) {
		cout << "Enter number of players: ";
		cin.clear();
		cin >> howManyPlayers;
		if (howManyPlayers < 2)
			cout << "Invalid value, try again.\n";
	}
	//initialize player array
	char temp_name[SIZE];
	Card* temp_card = NULL;
	players = new Player * [howManyPlayers];
	for (int i = 0; i < howManyPlayers; i++) {
		//get player name from user
		cout << "Enter name of player " << i + 1;
		cout << " (up to " << SIZE - 1 << " characters): ";
		cin.clear();
		cin >> temp_name;
		players[i] = new Player(temp_name);
		//each player gets 14 cards
		for (int j = 0; j < 14; j++) {
			temp_card = deck.pop();
			players[i]->setCard(temp_card);
		}
	}
}

//miscellaneous
void gameEngine::getCard(int& val, char& ch){
	cin.clear();
	cin >> ch >> val;
	while ((ch != 'H' && ch != 'D' && ch != 'S' && ch != 'C') || (val < 1 || 13 < val)){
		cout << "Invalid value, try again: ";
		cin.clear();
		cin >> ch >> val;
	}
}
int gameEngine::getSetIdx(){
	int set_num = -1;
	while (set_num < 1 || set_num > howManySets) {
		cin.clear();
		cin >> set_num;
		if (set_num < 1 || set_num > howManySets)
			cout << "Invalid value, choose again: ";
	}
	set_num--;
	return set_num;
}
void gameEngine::printFirstTrashCard(Card* c) {
	if (c) {
		cout << "First card from the trash deck: ";
		c->printCard();
	}
	else
		cout << "No cards in the trash deck.";
	cout << endl << endl;
}
void gameEngine::printBoard(){
	cout << "~~~~ Board status ~~~~\n";
	if (howManySets == 0)
		cout << "Board's currently empty.\n";
	for (int i = 0; i < howManySets; i++) {
		cout << "Set number " << i + 1 << ": ";
		boardSets[i]->printSet();
	}
	cout << endl;
}
void gameEngine::printPlayerHand(Player& p){
	if (p.getNumberOfCards() == 0) {
		cout << p.getName() << ", you do not have any cards remaining.\n";
		return;
	}
	cout  << p.getName() << ", your cards are:\n";
	p.printHand();
	cout << endl << endl;
}