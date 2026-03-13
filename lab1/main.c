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

void scannerUpdate(Node **head, char c) {
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

    Node *newNode = createNode(c);
    if (*head == NULL) *head = newNode;
    else prev->next = newNode;
}

void freeList(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    FILE *fp = fopen(__FILE__, "r");
    if (fp == NULL) return 1;

    Node *head = NULL;
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch >= 0 && ch <= 127) {
            scannerUpdate(&head, (char)ch);
        }
    }
    fclose(fp);

    Node *current = head;
    printf("Scanner generated tokens:\n");
    int lineCount = 0;
    while (current != NULL) {
        if (current->character == '\n') printf("'\\n' ");
        else if (current->character == '\t') printf("'\\t' ");
        else if (current->character == ' ') printf("' ' ");
        else printf("'%c' ", current->character);
        
        if (++lineCount % 10 == 0) printf("\n");
        
        current = current->next;
    }
    printf("\n");

    freeList(head);
    return 0;
}
