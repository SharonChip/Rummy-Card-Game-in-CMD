#include "CashierStack.h"
#include <ctime>
#include <iostream>
using namespace std;

CashierStack::CashierStack(){
	head = nullptr;
	numInDeck = 0;
}

CashierStack::~CashierStack()
{
#ifdef DEBUG
	cout << "\n Kill Stack";
#endif // DEBUG


	while (head) {
		stackItem* tmp = head;
		head = head->getNextItem();
		delete tmp;
	}
}

Card* CashierStack::pop() {
	stackItem* tmp = head;
	if (head) { 
		Card* res = head->getCard();
		head = head->getNextItem(); 
		delete tmp;
		numInDeck--;
		return res;

	}
	return nullptr;
}

void CashierStack::push(Card* c) {
	stackItem* newItem = new stackItem(c,head);
	numInDeck++;
	head = newItem;
}

void CashierStack::shuffle() {
srand(std::time(nullptr)); // use current time as seed for random generator

	for (int i = 0; i < 200; i++) {
		int random_variable = std::rand();
		shuffleNitem(rand()%(numInDeck));
	}
	//printStackForDeBugOnly();
}

void CashierStack::shuffleNitem(int n)
{
	CashierStack tmp;//will be destruct at the end of this block - will call destructor
	while (n>1 && head) { 
		tmp.push(pop()); 
		n--;
	}

	Card* c = pop();
	

	while (tmp.head) {
		push(tmp.pop());
	}
	push(c);
}

void CashierStack::printStackForDeBugOnly() const
{
	stackItem* tmp = head;
	cout << "---------------";
	for(int i =0; tmp;i++,tmp=tmp->getNextItem()) {
		cout << "\n no " << i<<" - ";
		tmp->getCard()->printCard();
	
	}

}
