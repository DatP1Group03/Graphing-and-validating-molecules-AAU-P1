// validation.h
#ifndef VALIDATION_H
#define VALIDATION_H

// Funktioner
void is_permitted(const char *input);
void closed_brackets(const char *input);
void misc_check(const char *input);
void ring_closed(const char *input);
int validate_smiles(const char *input);
void add_error(const char *msg, int pos);
void print_errors(void);
//Variabler

extern int smiles_size;
extern int smiles_count;
extern char smiles_symbols[];

#endif
