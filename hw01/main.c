#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 自定義字元判斷函式 (不使用 ctype.h) */
int is_alpha(char c) { 
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'); 
}

int is_digit(char c) { 
    return (c >= '0' && c <= '9'); 
}

int is_space(char c) { 
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r'); 
}

/* 處理標示符 (ID) 與 關鍵字 (Keywords) */
void handle_id_or_keyword(char first_char, FILE *fp) {
    char buffer[256];
    int idx = 0;
    buffer[idx++] = first_char;
    
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (is_alpha(c) || is_digit(c)) {
            buffer[idx++] = (char)c;
        } else {
            ungetc(c, fp); // 多讀的退回串流
            break;
        }
    }
    buffer[idx] = '\0';

    // 辨識 Keywords
    if (strcmp(buffer, "int") == 0) printf("%s: TYPE_TOKEN\n", buffer);
    else if (strcmp(buffer, "main") == 0) printf("%s: MAIN_TOKEN\n", buffer);
    else if (strcmp(buffer, "if") == 0) printf("%s: IF_TOKEN\n", buffer);
    else if (strcmp(buffer, "else") == 0) printf("%s: ELSE_TOKEN\n", buffer);
    else if (strcmp(buffer, "while") == 0) printf("%s: WHILE_TOKEN\n", buffer);
    else printf("%s: ID_TOKEN\n", buffer); // 其餘皆為 ID
}

/* 處理數字 (only int) */
void handle_number(char first_char, FILE *fp) {
    char buffer[256];
    int idx = 0;
    buffer[idx++] = first_char;

    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (is_digit(c)) {
            buffer[idx++] = (char)c;
        } else {
            ungetc(c, fp);
            break;
        }
    }
    buffer[idx] = '\0';
    printf("%s: LITERAL_TOKEN\n", buffer); //
}

int main() {
    // 開啟測試用的 Crap-C 檔案
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Error opening input.txt");
        return 1;
    }

    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        // 忽略空白字元
        if (is_space(ch)) continue;

        // DFA 狀態分支
        if (is_alpha(ch)) {
            handle_id_or_keyword((char)ch, fp);
        } else if (is_digit(ch)) {
            handle_number((char)ch, fp);
        } else {
            int next;
            switch (ch) {
                case '=':
                    next = fgetc(fp);
                    if (next == '=') printf("==: EQUAL_TOKEN\n");
                    else { ungetc(next, fp); printf("=: ASSIGN_TOKEN\n"); }
                    break;
                case '>':
                    next = fgetc(fp);
                    if (next == '=') printf(">=: GREATEREQUAL_TOKEN\n");
                    else { ungetc(next, fp); printf(">: GREATER_TOKEN\n"); }
                    break;
                case '<':
                    next = fgetc(fp);
                    if (next == '=') printf("<=: LESSEQUAL_TOKEN\n");
                    else { ungetc(next, fp); printf("<: LESS_TOKEN\n"); }
                    break;
                case '+': printf("+: PLUS_TOKEN\n"); break;
                case '-': printf("-: MINUS_TOKEN\n"); break;
                case '(': printf("(: LEFTPAREN_TOKEN\n"); break;
                case ')': printf("): REFTPAREN_TOKEN\n"); break; // 遵照範例拼寫 REFT
                case '{': printf("{: LEFTBRACE_TOKEN\n"); break;
                case '}': printf("}: REFTBRACE_TOKEN\n"); break; // 遵照範例拼寫 REFT
                case ';': printf(";: SEMICOLON_TOKEN\n"); break;
                case ',': printf(",: COMMA_TOKEN\n"); break;
                default: break; 
            }
        }
    }

    fclose(fp);
    return 0;
}