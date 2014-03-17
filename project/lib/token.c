#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct token {
    char value;
    struct token *next;
} token;


/**
 * Reverse a linked list of tokens
 * @param  root -> Pointer to the first token in the list
 * @return pointer to the first token in the reversed list
 */
token *reverse_tokens(token *root) {
  token *new_root = NULL;
  while (root) {
    token *next = root->next;
    root->next  = new_root;
    new_root    = root;
    root        = next;
  }
  return new_root;
}


/**
 * Deallocate a linked list of tokens
 * @param root -> Pointer to the first element in the linked list.
 */
void delete_tokens(token *root) {
    token *node = root;
    while (node != NULL) {
        token *temp = node;
        node = node->next;
        free(temp);
    }
    root = NULL;
}

/**
 * Convert a string of delimited characters into a linked-list of tokens
 * @param  str   -> The string of characters
 * @param  delim -> The string of delimiters
 * @return a pointer to the first element in the linked list
 */
token *tokenize(char *str, char *delim) {
    token *token_list = (token *) malloc(sizeof(token));
    char buffer[strlen(str) + 1];
    strcpy(buffer, str);
    char *token_string = strtok(buffer, delim);
    while (token_string != NULL) {
        token *new_token = (token *) malloc(sizeof(token));
        new_token->value = token_string[0];
        new_token->next  = token_list;
        token_list       = new_token;
        token_string     = strtok(NULL, delim);
    }
    return reverse_tokens(token_list);
}


/**
 * Print the list of tokens starting with root
 * @param root -> Pointer to the first token in the list.
 */
void print_tokens(token *root) {
    while (root) {
        if (isalpha(root->value)) {
            printf("%c\n", root->value);
        }
        root = root->next;
    }
}
