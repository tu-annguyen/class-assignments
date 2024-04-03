#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    int64_t *items;
};

Stack *stack_create(void) {
    Stack *s = (Stack *) calloc(1, sizeof(Stack));

    if (!s) { // Check to see if s was allocated unsuccessfully.
        return NULL;
    }
    // Instantiate top, capacity and items in s.
    s->top = 0;
    s->capacity = MIN_CAPACITY;

    // Allocate memory for each element in items.
    s->items = (int64_t *) calloc(s->capacity, sizeof(int64_t));

    if (!s->items) { // Check to see if s->items was allocated unsuccessfully.
        free(s->items);
        free(s);
        return NULL;
    }

    return s;
}

uint32_t stack_capacity(Stack *s) {
    return s->capacity;
}

uint32_t stack_top(Stack *s) {
    return s->top;
}

void stack_delete(Stack **s) {
    free((*s)->items);
    (*s)->items = NULL;

    free(*s);
    *s = NULL;

    return;
}

bool stack_empty(Stack *s) {
    // printf("top: %d\n", stack_top(s));
    return stack_top(s) == 0;
}

bool stack_push(Stack *s, int64_t x) {
    if (stack_top(s) == stack_capacity(s)) {
        s->capacity = 2 * stack_capacity(s);
        s->items = (int64_t *) realloc(s->items, stack_capacity(s) * sizeof(int64_t));

        // Checks for unsuccessful reallocation.
        if (!s) {
            return false;
        }
    }

    s->items[stack_top(s)] = x;
    s->top += 1;

    return true;
}

bool stack_pop(Stack *s, int64_t *x) {
    if (stack_empty(s)) {
        return false;
    }

    s->top -= 1;
    *x = s->items[s->top];

    return true;
}

void stack_print(Stack *s) {
    for (int i = 0; i < stack_capacity(s); i += 1) {
        printf("%ld ", s->items[i]);
    }
    printf("\n");
}
