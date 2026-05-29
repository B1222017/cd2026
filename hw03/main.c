#include <stdio.h>
#include <string.h>

/*
    HW#3 Recursive-Descent Parser

    Grammar:
        S  -> E S'
        S' -> epsilon
        S' -> + S
        E  -> num
        E  -> ( S )

    Scanner is adapted from HW#2:
        number -> LITERAL_TOKEN
        +      -> PLUS_TOKEN
        (      -> LEFTPAREN_TOKEN
        )      -> REFTPAREN_TOKEN
*/

#define LITERAL_TOKEN     1
#define PLUS_TOKEN        2
#define LEFTPAREN_TOKEN   3
#define REFTPAREN_TOKEN   4
#define END_TOKEN         5
#define ERROR_TOKEN       6

char input[1000];
int pos = 0;
int current_token = 0;
int valid = 1;

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void printIndent(int depth) {
    int i;
    for (i = 0; i < depth; i++) {
        printf("  ");
    }
}

/*
    Scanner:
    Based on HW#2 scanner logic.
    It reads characters and converts them into token types.
*/
void nextToken() {
    char c;

    while (is_space(input[pos])) {
        pos++;
    }

    c = input[pos];

    if (c == '\0') {
        current_token = END_TOKEN;
    }
    else if (is_digit(c)) {
        while (is_digit(input[pos])) {
            pos++;
        }
        current_token = LITERAL_TOKEN;
    }
    else {
        switch (c) {
            case '+':
                pos++;
                current_token = PLUS_TOKEN;
                break;

            case '(':
                pos++;
                current_token = LEFTPAREN_TOKEN;
                break;

            case ')':
                pos++;
                current_token = REFTPAREN_TOKEN;
                break;

            default:
                pos++;
                current_token = ERROR_TOKEN;
                break;
        }
    }
}

void error() {
    valid = 0;
}

/* S -> E S' */
void parse_S(int depth);

/* S' -> + S | epsilon */
void parse_Sp(int depth);

/* E -> num | ( S ) */
void parse_E(int depth);

void parse_S(int depth) {
    if (!valid) return;

    printIndent(depth);
    printf("S -> E S'\n");

    if (current_token == LITERAL_TOKEN || current_token == LEFTPAREN_TOKEN) {
        parse_E(depth + 1);
        parse_Sp(depth + 1);
    }
    else {
        error();
    }
}

void parse_Sp(int depth) {
    if (!valid) return;

    if (current_token == PLUS_TOKEN) {
        printIndent(depth);
        printf("S' -> + S\n");

        nextToken();
        parse_S(depth + 1);
    }
    else if (current_token == REFTPAREN_TOKEN || current_token == END_TOKEN) {
        printIndent(depth);
        printf("S' -> epsilon\n");
    }
    else {
        error();
    }
}

void parse_E(int depth) {
    if (!valid) return;

    if (current_token == LITERAL_TOKEN) {
        printIndent(depth);
        printf("E -> num\n");

        nextToken();
    }
    else if (current_token == LEFTPAREN_TOKEN) {
        printIndent(depth);
        printf("E -> ( S )\n");

        nextToken();
        parse_S(depth + 1);

        if (current_token == REFTPAREN_TOKEN) {
            nextToken();
        }
        else {
            error();
        }
    }
    else {
        error();
    }
}

int main() {
    printf("Please enter expression: ");

    if (scanf("%999s", input) != 1) {
        printf("Rejected\n");
        return 0;
    }

    pos = 0;
    valid = 1;

    nextToken();
    parse_S(0);

    if (valid && current_token == END_TOKEN) {
        printf("Accepted\n");
    }
    else {
        printf("Rejected\n");
    }

    return 0;
}
