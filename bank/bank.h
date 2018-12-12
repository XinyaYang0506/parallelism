#ifndef BANK_H
#define BANK_H

#include <unistd.h>

size_t create_account (int starting_balance);

int get_balance (size_t account);

size_t get_transaction_count (size_t account);

void do_transaction (size_t account, int amount);

void do_transfer (size_t from_account, size_t to_account, int amount);

#endif
