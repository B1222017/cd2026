#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char character;
    int count;
    struct Node *next;
} Node;

Node* createNode(char c) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->character = c;
        newNode->count = 1;
        newNode->next = NULL;
    }
    return newNode;
}

void updateList(Node **head, char c) {
    Node *current = *head;
    Node *prev = NULL;

    while (current != NULL) {
        if (current->character == c) {
            current->count++;
            return;
        }
        prev = current;
        current = current->next;
    }

    // If not found, create new node and append to the end
    Node *newNode = createNode(c);
    if (*head == NULL) {
        *head = newNode;
    } else {
        prev->next = newNode;
    }
}

// Function to free the memory
void freeList(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    // Open the source file itself
    FILE *fp = fopen(__FILE__, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    Node *head = NULL;
    int ch; 

    while ((ch = fgetc(fp)) != EOF) {
        if (ch >= 0 && ch <= 127) {
            updateList(&head, (char)ch);
        }
    }

    fclose(fp);

    Node *current = head;
    printf("Character Count Results:\n");
    printf("------------------------\n");
    while (current != NULL) {
        if (current->character == '\n') {
            printf("\\n  : %d\n", current->count);
        } else if (current->character == ' ') {
            printf("' ' : %d\n", current->count);
        } else if (current->character == '\t') {
            printf("\\t  : %d\n", current->count);
        } else {
            printf("%-3c : %d\n", current->character, current->count);
        }
        current = current->next;
    }

    freeList(head);

    return 0;
}
