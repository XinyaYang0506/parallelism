/* bank.c : A file containing data and routines for managing bank accounts */

#include "bank.h"

#include "../mythreads.h"
#include <unistd.h>

#define MAX_ACCOUNTS 100

/*
 * This structure holds the information for one account.
 * You may need to add fields to this struct to properly synchronize transactions.
 */
typedef struct account {
  int balance;
  size_t transaction_count;
  pthread_mutex_t mutex;
} account_t;

/*
 * To keep things simple, we'll just use an array of account structs and keep
 * track of the index of the next unused account.
 */
static account_t accounts[MAX_ACCOUNTS];
static size_t next_account = 0;

/*
 * Initialize a new account. This function will only be called at startup,
 * so there's no need to worry about synchronization here. If you add any fields
 * to the account struct make sure you initialize them here!
 *
 * This function returns an account ID that bank users will have to use for to
 * add transactions or transfers to their accounts.
 */
size_t create_account (int starting_balance) {
  // Get the ID of the account we're currently adding
  size_t current_account = next_account;
  next_account++;
	
  // Initialize the account information
  accounts[current_account].balance = starting_balance;
  accounts[current_account].transaction_count = 0;
  mutex_init(&accounts[current_account].mutex);
	
  // Return the account ID
  return current_account;
}

/*
 * Get the current balance for the specified account
 */
int get_balance (size_t account) {
  return accounts[account].balance;
}

/*
 * Get the current transaction count for the specified account
 */
size_t get_transaction_count (size_t account) {
  return accounts[account].transaction_count;
}

/*
 * Add a transaction to the specified account
 */
void do_transaction (size_t account, int amount) {
  mutex_lock(&accounts[account].mutex);
  accounts[account].balance += amount;
  accounts[account].transaction_count++;
  mutex_unlock(&accounts[account].mutex);
}

/*
 * Transfer money between two accounts
 */
void do_transfer (size_t from_account, size_t to_account, int amount) {
  mutex_lock(&accounts[from_account].mutex);
  accounts[from_account].balance -= amount;
  accounts[from_account].transaction_count++;
  mutex_unlock(&accounts[from_account].mutex);
	mutex_lock(&accounts[to_account].mutex);
  accounts[to_account].balance += amount;
  accounts[to_account].transaction_count++;
  mutex_unlock(&accounts[to_account].mutex);
}
