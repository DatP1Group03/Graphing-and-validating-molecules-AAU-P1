// validation.c
#include <stdio.h>
#include <ctype.h>
#include "validation.h"
#include <string.h>
/**
Funktion 1: Is_permitted() tjekker om karakterene i input arrayet stemmer med vores array af tiladte karaktere
Funktion 2: closed_brackets() tjekker om alle brackets er lukket, og om der er ekstra brackets /paranteser nogle steder
Funktion 3: ring_closed jekker om ringet lukkes ved at loop igennem input array og toggle int arrayet balance[d] hvis vi finder et atom og der efterkommer et nr
Funktion 4: misc_check tjekker en række små ting som tomme branches og forkerte start symboler
Funktion 5: add_error er en helper funktion til at tilføje errors til vores errors array.
Funktion 6: print_error kører en for lykke og printer alle fejlbeskeder hvis de findes
Funktion 7: validate_smiles kører alle tjeks.(F1-F4)

*/
#define MAX_ERRORS 50
#define MAX_MSG_LEN 128
#define MAX_INPUT 100

typedef struct {
    char message[MAX_MSG_LEN];
    int position;
} Error; // struct til at holde på to datatyper, både fejlbesked og position i stringen

Error errors[MAX_ERRORS]; // arrray af error struct
int error_count = 0;
int smiles_size = 0;
int atom_count = 0;

char smiles_symbols[] = { // GYLDIGE SMILES KARAKTER
    'C', 'N', 'O', 'S','H',
    'c', 'n', 'o', 's', 'p',
    '-', '=', '#', ':', '/', '\\',
    '(', ')','[',']',
    '1','2','3','4','5','6','7','8','9',
    '@', '.', '*'
};

int smiles_count = sizeof(smiles_symbols) / sizeof(smiles_symbols[0]);
    int smiles_input_size = 0;

void add_error(const char *msg, int pos) {
    if (error_count < MAX_ERRORS) { // hvis mindre end max tilladte errors
        strncpy(errors[error_count].message, msg, MAX_MSG_LEN - 1); // kopiere fejlbesked til error[error_count].message, som er initialiseret til 0.
        errors[error_count].position = pos; // samme princip, error[error_count].position = pos (som vi får fra vores funktioner i parameter)
        error_count++; //increment til næste fejl
    }
}
void is_permitted(const char *input) {
    smiles_input_size = 0;
    atom_count = 0;


    for (int i = 0; input[i] != '\0'; i++) { // loop igennem vores input array
        int match_found = 0; // nulstilles ved hver itteration
        for (int m = 0; m < smiles_count; m++) {
            if (input[i] == smiles_symbols[m]) {
                match_found = 1; // sæt til 0, og så starter vi næste itteration
                break;
            }
        }

        if (!match_found) { // loopet igennem alle karakter, men ikke fundet en gyldig karakter
            add_error("Illegal character\n", i+1);

        }
    }


}

int count_smiles(const char *input) {
    int smile_size = 0;
    for (int i = 0; input[i] != '\0';i++) {
        smile_size++;
    }
    return smile_size;
}
int count_atoms(const char *input) {
    int count_of_atoms = 0;
    for (int i = 0; input[i] != '\0';i++) {
        if (isalpha(input[i])) {
            count_of_atoms++;
        }
    }
    return count_of_atoms;
}
void closed_brackets(const char *input) {
    char stack_parantheses[MAX_INPUT]; //tallet kommer fra maxInput men har ik sat det ind endnu.
    char stack_brackets[MAX_INPUT];
    int top = -1;
    int top_brackets =-1;

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '(') {
            stack_parantheses[++top] = '('; // tilføj ( til toppen at stacken
        }
        if (input[i] == ')' && stack_parantheses[top] != '(') {
            add_error("Closing parantheses has no matching opening parantheses \n", i+1);

        }
        if (input[i] == ')' && stack_parantheses[top] == '(' ) {
            top--; // pop top af stacken, og gør plads til en ny char
        }

        if (input[i] == '[') {

            stack_brackets[++top_brackets] = '[';
        }

        if (input[i] == ']' && stack_brackets[top_brackets] != '[') {

            add_error("Closing bracket has no matching opening bracket \n", i+1);

        }
        if (input[i] == ']' && stack_brackets[top_brackets] == '[' ) {
            top_brackets--;
        }

        if (input[i] == ']' && input[i+1] == ']' ) {
            add_error("Two consecutive brackets \n", i+1);

        }

    }

    if (top != -1) {
        add_error("Unmatched '(' remain", -1);
    }
    if (top_brackets != -1) {
        add_error("Unmatched '[' remain", -1);

    }

}

void ring_closed(const char *input) {

    int balance[10] = {0}; // til at holde styr på 9 mulige ringe(maksimum i SMILES)

    for (int i = 0; input[i] != '\0'; i++) {
        smiles_size++;
        if (isalpha(input[i]) && input[i+1] != '\0' && isdigit(input[i+1])) {
            int d = input[i+1] - '0'; // d er sat til den digit vi har fundet efter en atom,
            if (d < 0 || d > 9) { // til at håndtere eg. 10, så vil digit = 0, derfor laver vi et hurtig tjek
                add_error("Invalid ring digit",i+1);

            }
                balance[d] ^= 1; // XOR toggle
                i++;
            }
        }
        for (int d = 0; d < 10; d++) {
            if (balance[d] != 0) {
                add_error("Ring not closed properly", -1);

            }
        }


    }

void misc_check(const char *input) {

    if (input[0] == '=' || input[0] == '-' || input[0] == '#') { // check hvis smiles tring enten begynder eller slutter med bindingsymbol

        add_error("Smiles string cannot contain a bond symbol as the first character\n", 1);

    }
    if (input[smiles_input_size-1] == '=' || input[smiles_input_size-1] == '-' || input[smiles_input_size-1] == '#') {
        add_error("Smiles string cannot contain a bond symbol as the last character\n", smiles_input_size);

    }
    for (int i = 0; i <smiles_input_size;i++) {
        if (input[i] == '(' && input[i+1] == ')') {
           add_error("Empty branch in SMILES string",-1);

        }
        if (input[i] == '=' || input[i] == '-'|| input[i] == '#') {
            if (input[i+1] == '=' || input[i+1] == '-'|| input[i+1] == '#') {
                add_error("Two consecutive bond symbols beginning at position", i+1);
            }
        }
    }
}

void print_errors(void) {
    for (int i = 0; i < error_count; i++) {
        printf("Error %d: %s (position %d)\n",i+1, errors[i].message, errors[i].position);
    }
}


int validate_smiles(const char *input) {

    // kør alle vores tjeks, og sæt error_count til 0 igen, fordi main() bare kører den igen.
    error_count = 0;
    is_permitted(input);
    smiles_input_size = count_smiles(input);
    atom_count = count_atoms(input);
    closed_brackets(input);
    ring_closed(input);
    misc_check(input);
    if (error_count > 0) {
        print_errors();
        return 0;
    }

    return 1;
}
int get_error_count(void) {
    return error_count;
}

const char* get_error_message(int index) {
    if (index >= 0 && index < error_count)
        return errors[index].message;
    return "";
}

int get_error_position(int index) {
    if (index >= 0 && index < error_count)
        return errors[index].position;
    return -1;
}


