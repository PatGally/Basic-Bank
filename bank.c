#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

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
void create(BankAccount *user);
void submitDeposit(BankAccount *user, const TransactionType type);
void submitWithdraw(BankAccount *user, const TransactionType type);

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

const int withdraw(BankAccount *account, const double amount){
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
         printf("4 Show Balance\n");
         printf("5 Show Transaction History\n");
	 printf("9 To quit\n");
}


void create(BankAccount *user){
	bool val;
	char name[50];

	do {
	    printf("Please enter your name: ");
	    fflush(stdout);

	    if (!fgets(name, sizeof(name), stdin)) {

	        printf("Input error. Please try again.\n");
	        val = false;
	        continue;
	    }


	    name[strcspn(name, "\n")] = '\0';


	    if (name[0] == '\0') {
	        printf("Invalid name. Please enter a valid name.\n");
	        val = false;
	    } else {
	        val = true; 
	    }

	} while (!val);


	createAccount(user, name);

}

void submitDeposit(BankAccount *user, const TransactionType type){
	
	char buf[64];
	double amount;
    	int valid;

	do {
        	printf("Enter amount: ");

        	if (!fgets(buf, sizeof(buf), stdin)) {
            		printf("Input error. Try again.\n");
            		valid = 0;
            		continue;
        	}

        	char *endptr;
        	amount = strtod(buf, &endptr);

        	if (endptr == buf || *endptr != '\n') {
        	    printf("Invalid input. Please enter a number.\n");
        	    valid = 0;
        	} else if (amount <= 0) {
        	    printf("Amount must be more than 0.\n");
        	    valid = 0;
        	} else {
        	    valid = 1;  
        	}

    	} while (!valid);

   	logTransaction(user, type, amount); 
	printf("Deposit Successful");

}

void submitWithdraw( BankAccount *user, const TransactionType type){
	char buf[64];
	double amount;
	int valid;
	do {
                printf("Enter amount: ");

                if (!fgets(buf, sizeof(buf), stdin)) {
                        printf("Input error. Try again.\n");
                        valid = 0;
                        continue;
                }
		char *endptr;
                amount = strtod(buf, &endptr);

                if (endptr == buf || *endptr != '\n') {
                    printf("Invalid input. Please enter a number.\n");
                    valid = 0;
                } else if (amount <= 0) {
                    printf("Amount must be more than 0.\n");
                    valid = 0;
                } else {
                    valid = 1;
                }

        } while (!valid);
	
	logTransaction(user, type, amount);
        
	printf("Withdraw successful\n");

}

int main(){
	char buf[64];
	BankAccount user;
	double balance = 500;
	int userSelection;
	int valid;
	int userCreated = 0;
	printf("Welcom to the bank\n");

	
	do {
    	    display();

    	    if (!fgets(buf, sizeof(buf), stdin)) {
        	continue;
    	    }
	
	    userSelection = (int)strtol(buf, NULL, 10);
		
	    if (userCreated == 0 && userSelection != 1) {
                 printf("Error: You must create an account first.\n");
                 continue;
            }

	    switch (userSelection) {
	        case 1:
	            create(&user);
		    userCreated = 1;
	            break;

        	case 2:
        	    submitDeposit(&user, DEPOSIT);
        	    break;

        	case 3:
        	    submitWithdraw(&user, WITHDRAW);
        	    break;

        	case 4:
        	    showAccount(&user);
        	    break;
	
        	case 5:
        	    showTransactions(&user);
        	    break;
	
        	default:
        	    printf("Please select options 1-5 or 9 to end\n");
        	    break;
   	   }

	} while (userSelection != 9);
			
	
	return 0;
}

