#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef enum {
	DEPOSIT,
	WITHDRAW
}TransactionType;

typedef struct {
	TransactionType type;
	double amount;
	time_t time;
}Transaction;

typedef struct {
	char name[50];
	double balance;
	Transaction *transactions;
	int transactionCount;
	int capacity;
}BankAccount;

void createAccount(BankAccount *account, const char* name);
void logTransaction( BankAccount *account, const TransactionType type, const double amount);
void showAccount(const BankAccount *account);
void deposit(BankAccount *account, const double amount);
int withdraw(BankAccount *account, const double amount);
void wipeTransactionLog(BankAccount *account);
void showTransactions(const BankAccount *account);
void flushInputBuffer();

void createAccount(BankAccount *account, const char* name){
	strncpy(account->name, name, sizeof(account->name)-1);
	account->name[sizeof(account->name)-1] = '\0';
	account->balance = 0.0;
	account->transactionCount = 0;
	account-> capacity = 2;
	account->transactions = (Transaction *) malloc(account->capacity * sizeof(Transaction));
	if(account->transactions == NULL){
		fprintf(stderr, "Memory allocation failed \n");
		exit(1);
	}
}

void logTransaction( BankAccount *account, const TransactionType type, const double amount){
	if(type == DEPOSIT){
		deposit(account, amount);
	}else if(type == WITHDRAW){
		int check = withdraw(account, amount);
		if(check == 1){
			printf("Error cannot withdraw more than you have \n");
			return;
		}
	}

	if(account->transactionCount >= account->capacity){
		account->capacity *= 2;
		account->transactions = (Transaction*) realloc(account->transactions, account->capacity * sizeof(Transaction));
		if(account->transactions == NULL){
			fprintf(stderr, "Memory allocation failed \n");
			exit(1);
		}
	}

	Transaction newTransaction;
	newTransaction.type = type;
	newTransaction.amount = amount;
	newTransaction.time = time(NULL);


	account->transactions[account->transactionCount] = newTransaction; 
	account->transactionCount++;

}

void showAccount(const BankAccount *account) {
	printf("Name: %s\n", account->name);
	printf("Balance: %.2f\n", account->balance);
}

void showTransactions(const BankAccount *account){
	for(int i = 0; i < account->transactionCount; i++){
		Transaction t = account->transactions[i];
        	char *typeStr = (t.type == DEPOSIT) ? "Deposit" : "Withdrawal";
        	char timeString[26];
        	struct tm *tm_info = localtime(&t.time);
        	strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", tm_info);
        	printf("%s: %.2f on %s\n", typeStr, t.amount, timeString);
	}

}
void deposit(BankAccount *account, const double amount){
	account->balance += amount;
}

int withdraw(BankAccount *account, const double amount){
	double balance = account->balance;
	if((balance -= amount) >= 0){
		account->balance -= amount;
	}
	else{
		return 1;
	}
	return 0;
}

void wipeTransactionLog(BankAccount *account){
	free(account->transactions);
	account->transactions = NULL;
}

void display(){
	 printf("Select one of the options\n");
         printf("1 Create an Account\n");
         printf("2 Deposit\n");
         printf("3 Withdraw\n");
         printf("4 ShowBalance\n");
         printf("5 Show Transaction History\n");
	 printf("9 To quit\n");
}

void flushInputBuffer(){
	int c;
	while((c = getchar()) != '\n' && c != EOF);
}

int main(){
	
	BankAccount users[10];
	char name[50] = "Chad Takemall";
	double balance = 500;
	int userSelection;
	int valid;
	printf("Welcom to the bank\n");
	do{
		display();
		valid = scanf("%d", &userSelection);
		if(valid != 1){
			do{
				flushInputBuffer();
				printf("Invalid Input\n");
				display();
				valid = scanf("%d", &userSelection);
			}while(valid != 1);
		}
		
		switch(userSelection){
			case 1:
				printf("Selected 1 \n");
				break;

			case 2:
				printf("Selected 2 \n");
				break;

			case 3:
				printf("Selected 3 \n");
				break;

			case 4:
				printf("Selected 4 \n");
				break;

			case 5:
				printf("Selected 5 \n");
				break;
			default:
				printf("Please select options 1-5 or 9 to end/n");
				break;
		}
	}while(userSelection != 9);
//	createAccount(&user1, name);
	
//	logTransaction(&user1, DEPOSIT, 500);
//	logTransaction(&user1, WITHDRAW, 422.21);

//	showAccount(&user1);

//	wipeTransactionLog(&user1);		
	
	return 0;
}

