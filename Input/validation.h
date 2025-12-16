// validation.h
#ifndef VALIDATION_H
#define VALIDATION_H

#define MAX_ERRORS 50
#define MAX_MSG_LEN 128
#define MAX_INPUT 100

typedef struct {
    char message[MAX_MSG_LEN];
    int position;
} Error; // struct til at holde på to datatyper, både fejlbesked og position i stringen

// Funktioner
int is_permitted(const char *input, int *error_count, Error errors[]);
int closed_brackets(const char *input, int *error_count, Error errors[]);
int misc_check(const char *input, int *error_count, Error errors[]);
int ring_closed(const char *input, int *error_count, Error errors[]);
int validate_smiles(const char *input, int *error_count, Error errors[MAX_ERRORS]);
void add_error(const char *msg, int pos, int *error_count, Error errors[]);
void print_errors(int error_count, Error errors[]);
int count_atoms(const char *input);

int get_error_count(int *error_count);
const char* get_error_message(int index, int *error_count, Error errors[]);
int get_error_position(int index, int *error_count, Error errors[]);
int count_smiles(const char *input);
#endif
