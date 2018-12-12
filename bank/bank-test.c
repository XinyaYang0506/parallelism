#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "bank.h"

#define NUM_ACCOUNTS 10
#define TRANSACTIONS 10000

// We will return this struct from each thread to track transaction amounts and
// counts
typedef struct result {
  int expected_final_amount;
  size_t expected_transaction_count;
} result_t;

// We'll use a global array of test account IDs so threads can access it
static size_t accounts[NUM_ACCOUNTS];

void* test_thread (void* arg);

int main (int argc, char** argv) {
  // Create test accounts
  for (size_t i=0; i<NUM_ACCOUNTS; i++) {
    accounts[i] = create_account(0);
  }
	
  // Create a thread for each account
  pthread_t threads[NUM_ACCOUNTS];
	
  for (size_t i=0; i<NUM_ACCOUNTS; i++) {
    if (pthread_create(&threads[i], NULL, test_thread, (void*)accounts[i])) {
      perror("Failed to create thread");
      exit(2);
    }
  }
	
  // Wait for each thread to finish
  result_t* results[NUM_ACCOUNTS];
	
  for (size_t i=0; i<NUM_ACCOUNTS; i++) {
    if (pthread_join(threads[i], (void**)&results[i])) {
      perror("Failed to join with thread");
      exit(2);
    }
  }
	
  // Compute the expected global balance and total transaction count
  int expected_total = 0;
  size_t expected_count = 0;
  for (size_t i=0; i<NUM_ACCOUNTS; i++) {
    expected_total += results[i]->expected_final_amount;
    expected_count += results[i]->expected_transaction_count;
  }
	
  // Compute the actual global balance and total transaction count
  int actual_total = 0;
  size_t actual_count = 0;
  for (size_t i=0; i<NUM_ACCOUNTS; i++) {
    actual_total += get_balance(accounts[i]);
    actual_count += get_transaction_count(accounts[i]);
  }
	
  // Validate results
  if (actual_total == expected_total) {
    printf("The total amount of money is correct!\n");
  } else if (actual_total > expected_total) {
    printf("Uh oh! Your bank system created $%d out of thin air.\n",
            actual_total - expected_total);
  } else {
    printf("Uh oh! Your bank system lost $%d somewhere.\n",
            expected_total - actual_total);
  }
	
  if (actual_count == expected_count) {
    printf("The actual transaction count is correct!\n");
  } else if (actual_count > expected_count) {
    printf("Uh oh! Your bank system created %lu transactions out of thin air.\n",
            actual_count - expected_count);
  } else {
    printf("Uh oh! Your bank system lost %lu transactions.\n",
           expected_count - actual_count);
  }
	
  return 0;
}

/*
 * This is the thread test function. It receives an account ID cast to the void* arg.
 * This function should return a pointer to a result_t struct to validate results.
 */
void* test_thread (void* arg) {
  // Unpack the input parameter
  size_t account_id = (size_t)arg;
	
  // Set up the result struct
  result_t* result = (result_t*)malloc(sizeof(result_t));
  result->expected_final_amount = 0;
  result->expected_transaction_count = 0;
	
  // Set up the random number generator seed
  unsigned seed = time(NULL);
	
  // Run a bunch of random transactions
  for (int t=0; t<TRANSACTIONS; t++) {
    // Choose an action at random
    int action = rand_r(&seed) % 10;
		
    // Generate a random amount
    int amount = rand_r(&seed) % 1000 - 400;
		
    if(action < 9) {
      // 90% of the time, do a normal transaction
			
      // Do the transaction
      do_transaction(account_id, amount);
			
      // Update our expected amount and transaction count
      result->expected_final_amount += amount;
      result->expected_transaction_count++;
			
    } else {
      // 10% of the time, do a transfer to a random account
			
      // Generate a random account id
      size_t to_account = rand_r(&seed) % NUM_ACCOUNTS;
			
      // If the other account is *not* the same as the current account, do the
      // transfer
      if(to_account != account_id) {
        do_transfer(account_id, to_account, amount);
        result->expected_transaction_count += 2;
        // We're adding two transactions because a transfer logs a transaction
        // on *both* accounts.  We'll just check the total number of
        // transactions, so it's okay that this is counted in the wrong place.
      }
    }
  }

  return result;
}
