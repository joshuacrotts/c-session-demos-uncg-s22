#include <stdio.h>
#include <stdlib.h>

#include "student.h"
#include "stack.h"

int main(int argc, char *argv[]) {
    stack stk;
    stack_init(&stk);

    struct student *s1 = student_init("Joshua", 4.0);
    struct student *s2 = student_init("Bob", 3.25);
    struct student *s3 = student_init("Alan", 4);
    struct student *s4 = student_init("Robert", 2.25);

    printf("Pushing Joshua, Robert, and Bob...\n");
    stack_push(&stk, s1); // Joshua
    stack_push(&stk, s4); // Robert
    stack_push(&stk, s2); // Bob

    stack_free(&stk);
    return 0;
}