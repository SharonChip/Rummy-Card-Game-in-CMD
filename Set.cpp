#include "Set.h"
using namespace std;

Set::Set(){
	cards = NULL;
	howManyCards = 0;
}
Set::Set(const Set& s){
	cards = new Card * [s.howManyCards];
	howManyCards = s.howManyCards;
	for (int i = 0; i < s.howManyCards; i++)
		cards[i] = s.cards[i];
}
void Set::addCard(Card* c){
	if (!c)
		return;
	//new temp array
	Card** temp = new Card * [howManyCards + 1];
	for (int i = 0; i < howManyCards; i++)
		temp[i] = cards[i];
	//set the class' array to the temp array
	temp[howManyCards] = c;
	delete[] cards;
	cards = temp;
	howManyCards++;
}
Card* Set::extractCard(char char_c, int int_val){
	int  current_val, k = 0, i;
	char current_sign;
	Card* my_card = NULL;
	Card** temp;
	//allocate memory with a smaller size
	Card** new_arr = new Card * [howManyCards - 1];
	//in one loop, search the requested extraction and simultaneously copy
	for (i = 0; i < howManyCards; i++) {
		current_val = cards[i]->getValue();
		current_sign = cards[i]->getSign();
		//if found the card, break the loop and continue without
		//checking the rest of the cards in the array
		if (current_val == int_val && current_sign == char_c) {
			my_card = cards[i];
			i++;
			break;
		}
		if (k < howManyCards - 1)
			new_arr[k++] = cards[i];
	}
	//card isn't in the array
	if (!my_card) {
		//free un-used memory
		delete[] new_arr;
		//temporary and local Card object to elegantly print the card
		//will get destructed at the end of this method
		Card temp;
		temp.setCard(char_c, int_val);
		cout << "The card ";
		temp.printCard();
		cout << " is not in the set, try again: ";
		return NULL;
	}
	//if the card has been found, continue copying the rest into the
	//new array, but this time without further calls for getValue and getSign
	if (i < howManyCards)
		for (; i < howManyCards; i++)
			new_arr[k++] = cards[i];
	//free the previous array and update the fields
	delete[] cards;
	cards = new_arr;
	howManyCards--;
	return my_card;
}
bool Set::isSameValueSerial() const{
	//in this method we assume at least 3 cards in the serial
	//since isValidSet checks for this case before calling this method
	//-------------
	//in case there are more than 4 cards, there's at least 1 repetitive sign
	if (howManyCards > 4 || howManyCards < 1)
		return false;
	//define h,d,c,s as switches (0 or 1) - if found more than 1, return false
	char h, d, c, s;
	h = d = c = s = 0;
	char temp_c = 0;
	int temp_n = 0;
	int num = cards[0]->getValue();
	for (int i = 0; i < howManyCards; i++) {
		temp_c = cards[i]->getSign();
		temp_n = cards[i]->getValue();
		//in case of different number, return false
		if (temp_n != num)
			return false;
		//check signs
		switch (temp_c) {
		case 'H':
			h++;
			break;
		case 'S':
			s++;
			break;
		case 'D':
			d++;
			break;
		case 'C':
			c++;
			break;
		}
		//if any sign appears more than once, return false
		if (s > 1 || c > 1 || d > 1 || h > 1)
			return false;
	}
	return true;
}
bool Set::isSequential() const {
	//in this method we assume at least 3 cards in the serial
	//since isValidSet checks for this case before calling this method
	//-------------
	//check if all signs are the same
	char sign = cards[0]->getSign(), temp_c = 0;
	int temp_i;
	int min, max, sum;
	sum = min = max = cards[0]->getValue();
	for (int i = 1; i < howManyCards; i++) {
		//check char
		temp_c = cards[i]->getSign();
		if (temp_c != sign)
			return false;
		//check for an ascending serial with a difference of 1
		//for this purpose, we'll use the max and min values
		temp_i = cards[i]->getValue();
		sum += temp_i;
		if (temp_i > max)
			max = temp_i;
		if (temp_i < min)
			min = temp_i;
	}
	//use the sum of an arithmetic progression to determine if the serial is valid
	//the amount of cards that should be in the serial is (max-min)+1
	//the formula for an arithmetic progression's sum is S_n = (n*(a_1 + a_n)) / 2
	//in this case, 'a_1' = min, 'a_n' = max and 'n' is the difference between the two
	int required_sum = ((max - min + 1) * (min + max))/ 2;
	if (sum == required_sum)
		return true;
	return false;
}
bool Set::isValidSet() const{
	//checking for at least 3 cards
	if (howManyCards < 3)
		return false;
	if (isSequential() || isSameValueSerial())
		return true;
	return false;
}
void Set::printSet() const{
	int i = 0;
	//if set's empty
	if (isEmpty()) {
		cout << "empty set.\n";
		return;
	}
	//print set
	for (; i < howManyCards - 1; i++) {
		Set::cards[i]->printCard();
		cout << ", ";
	}
	Set::cards[i]->printCard();
	cout << endl;
}
bool Set::isEmpty() const {
	if (howManyCards == 0 && cards == NULL)
		return true;
	return false;
}
Set::~Set() {
	delete[] cards;
}