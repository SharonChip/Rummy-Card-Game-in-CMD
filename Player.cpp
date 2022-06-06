#include "Player.h"
using namespace std;

void Player::setName(char* name) {
	this->name = new char[strlen(name) + 1];
	strcpy_s(this->name, strlen(name) + 1, name);
}
void Player::setCard(Card* card){
	if (!card)
		return;
	//new larger array
	Card** temp = new Card * [numInHand + 1];
	int i;
	//copying
	for (i = 0; i < numInHand; i++)
		temp[i] = hand[i];
	//adding new card and updating fields
	temp[i] = card;
	delete[] hand;
	hand = temp;
	numInHand++;
}
Card* Player::extractCard(char char_c, int int_val){
	int current_val, k = 0, i = 0;
	char current_sign;
	Card* my_card = NULL;
	Card** new_arr = new Card * [numInHand - 1];
	//check for default values
	//if default values, save the pointer to the first card
	//and move i forward once, thus copying all the pointers after it in the upcoming loop
	if (char_c == -1 && int_val == -1 || (char_c == hand[0]->getSign() && int_val == hand[0]->getValue())){
		i++;
		my_card = hand[0];
	}
	if (!my_card) {
		for (; i < numInHand; i++) {
			current_val = hand[i]->getValue();
			current_sign = hand[i]->getSign();
			//if found the card, break the loop and continue without
			//checking the rest of the cards in the array
			if (current_val == int_val && current_sign == char_c) {
				my_card = hand[i];
				i++;
				break;
			}
			//make sure there aren't too many cards in new_arr
			if (k < numInHand - 1)
				new_arr[k++] = hand[i];
		}
	}
	//if the card hasn't been found, new_card is still NULL
	if (!my_card) {
		delete[] new_arr;
		//temporary and local Card object to elegantly print the card
		//will get destructed at the end of this method
		Card temp;
		temp.setCard(char_c, int_val);
		cout << "The card ";
		temp.printCard();
		cout << " is not in your hand, try again: ";
		return NULL;
	}
	//if the card has been found, continue copying the rest into the
	//new array, but this time without further calls for getValue and getSign
	if (i < numInHand)
		for (; i < numInHand; i++)
			new_arr[k++] = hand[i];
	//free the previous array and update the fields
	delete[] hand;
	hand = new_arr;
	numInHand--;
	return my_card;
}
char* Player::getName() const{
	return name;
}
int Player::getNumberOfCards() const{
	return numInHand;
}
void Player::printHand() const{
	int i = 0;
	//if hand's empty
	if (numInHand == 0) {
		cout << "No cards.\n";
		return;
	}
	//print hand
	for (; i < numInHand - 1; i++) {
		hand[i]->printCard();
		cout << ", ";
	}
	hand[i]->printCard();
}
Player::Player(char* name){
	if (name)
		setName(name);
	else
		this->name = NULL;
	hand = NULL;
	numInHand = 0;
}
Player::~Player(){
	delete[] name;
	delete[] hand;
}