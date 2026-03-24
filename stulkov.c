#include <stdio.h>
#define MAX 100

typedef struct 
{
    int data[MAX];
    int top;
}stack;


void init(stack *s)
{
    s->top = -1;
}


int empty(stack *s)
{
    if(s->top == -1)
    return 1;
    else
    return 0;
}


int full(stack *s)
{
    if(s->top == MAX - 1)
        return 1;
    else
        return 0;
}


void push(stack *s, int value)
{
    if(full(s))
    {
        printf("stack overflow");
        return;
    }
    s->top += 1;
    s->data[s->top] = value;
}

int pop(stack *s)
{
    int value;
    if(empty(s))
    {
        printf("stack empty");
        return 0;
    }

    value = s->data[s->top];
    s->top -= 1;
    return value;
}


int peek(stack *s)
{
    if(empty(s))
    {
        printf("stack empty");
        return -1;
    }

    return s->data[s->top];
}

int main()
{
    stack s;
    init(&s);
    
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);
    
    printf("Top: %d\n", peek(&s));
    printf("Pop: %d\n", pop(&s));
    printf("Pop: %d\n", pop(&s));
    printf("Pop: %d\n", pop(&s));
    printf("Pop: %d\n", pop(&s));
    
    return 0;
}