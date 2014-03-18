#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LEFT_ASSOC  0
#define RIGHT_ASSOC 1

/**
 * Push a char onto the stack
 * @param stack    -> The stack being modified
 * @param top      -> The index for the top element
 * @param element  -> The char being added
 */
void push_char(char *stack, int *top, const char element) {
    stack[++(*top)] = element;
}


/**
 * Pop the top item off of the stack.
 * @param  stack -> The stack being modified
 * @param  top   -> The index for the top element
 * @return the character removed from the stack
 */
char pop_char(const char *stack, int *top) {
    return stack[(*top)--];
}


/**
 * Push an int onto the stack
 * @param stack    -> The stack being modified
 * @param top      -> The index for the top element
 * @param element  -> The int being added
 */
void push_int(int *stack, int *top, const int element) {
    stack[++(*top)] = element;
}


/**
 * Pop the top item off of the stack.
 * @param  stack -> The stack being modified
 * @param  top   -> The index for the top element
 * @return the int removed from the stack
 */
int pop_int(const int *stack, int *top) {
    return stack[(*top)--];
}


/**
 * Get the precedence for the given operator.
 * @param  element -> The operator being checked
 * @return the integer precedence value
 */
int precedence(const char element) {
    switch (element) {
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
    }
    return 0;
}


/**
 * Get operator associativity
 * @param  operator -> The operator being checked
 * @return the associativity constant
 */
int associativity(const char operator) {
    return LEFT_ASSOC;
}


/**
 * Check whether the stack is empty.
 * @param  stack -> The stack being checked
 * @param  top   -> The index of the top value
 * @return whether the stack is empty
 */
bool is_empty(const char *stack, const int top) {
    return stack[top] == ' ';
}


/**
 * Convert an infix expression to a postfix one.
 * @param infix -> The expression to be converted
 * @return the postfix expression
 */
char *infix_to_postfix(const char *infix) {
    size_t length = strlen(infix);
    char *postfix = (char *) malloc(((2 * length) + 1) * sizeof(char));
    int top       = 0;
    int p_index   = 0;
    char stack[length];
    char current;
    char temp;
    for (size_t i = 0; i < length; ++i) {
        current = infix[i];
        if (current == ' ') continue;
        if (isdigit(current)) {
            postfix[p_index] = current;
            ++p_index;
            char next = infix[i + 1];
            while (isdigit(next)) {
                postfix[p_index] = next;
                ++p_index;
                ++i;
                next = infix[i + 1];
            }
            postfix[p_index] = ' ';
            ++p_index;
        } else if (is_empty(stack, top) || stack[top] == '(') {
            push_char(stack, &top, current);
        } else if (current == '(') {
            push_char(stack, &top, current);
        } else if (current == ')') {
            while (true) {
                temp = pop_char(stack, &top);
                if (temp == '(') break;
                postfix[p_index] = temp;
                ++p_index;
                postfix[p_index] = ' ';
                ++p_index;
            }
        } else if (precedence(current) > precedence(stack[top])) {
            push_char(stack, &top, current);
        } else if (precedence(current) == precedence(stack[top])) {
            push_char(stack, &top, current);
        } else if (precedence(current) < precedence(stack[top])) {
            temp = pop_char(stack, &top);
            postfix[p_index] = temp;
            ++p_index;
            postfix[p_index] = ' ';
            ++p_index;
        }
    }
    while (top != -1) {
        temp = pop_char(stack, &top);
        if (temp == '(' || temp == ')') {
            printf("\nMismatched parentheses. Exiting...\n");
            exit(EXIT_FAILURE);
        }
        postfix[p_index] = temp;
        ++p_index;
        postfix[p_index] = ' ';
        ++p_index;
    }
    postfix[strlen(postfix) - 2] = '\0';
    return postfix;
}


/**
 * Evaluate a postfix expression
 * @param  expression -> The expression to be evaluated
 * @return the result of the expression
 */
int eval(const char *expression) {
    size_t length = strlen(expression);
    int top = 0;
    int stack[length];
    char current;
    for (size_t i = 0; i < length; ++i) {
        current = expression[i];
        if (current == '\0' || isblank(current)) {
            continue;
        }
        else if (isdigit(current)) {
            int count = 1;
            char next = expression[i + 1];
            while (isdigit(next)) {
                ++count;
                next = expression[i + count];
            }
            if (count == 1) {
                push_int(stack, &top, current - '0');
            } else {
                char *num = (char *) malloc(count * sizeof(char));
                for (int j = 0; j < count; ++j) {
                    num[j] = expression[i];
                    ++i;
                }
                push_int(stack, &top, atoi(num));
                free(num);
            }
        } else {
            int operand2 = pop_int(stack, &top);
            int operand1 = pop_int(stack, &top);
            int result;
            switch (current) {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    result = operand1 / operand2;
                    break;
            }
            push_int(stack, &top, result);
        }
    }
    return stack[top];
}


/*
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s \"<string>\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *postfix = infix_to_postfix(argv[1]);
    int result = eval(postfix);

    printf("Infix:   %s\n", argv[1]);
    printf("Postfix: %s\n", postfix);
    printf("Result:  %d\n", result);

    free(postfix);
    return 0;
}
*/
