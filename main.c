#include <stdio.h>
#include <stdlib.h>

int main(int Argc, char **Argv) {

    if(Argc != 2){
        fprintf(stderr, "%s: invalid number of arguments\n", Argv[0]);
        return 1;
    }

    char *P = Argv[1];
    printf("  .global main\n");
    printf("main:\n");
    printf("  li a0, %ld\n", strtol(P, &P, 10));

    while(*P)
    {
        if(*P == '+')
        {
            ++P;
            printf(" addi a0, a0, %ld\n", strtol(P, &P, 10));
            continue;
        }

        if(*P == '-')
        {
            ++P;
            printf(" addi a0, a0, -%ld\n", strtol(P, &P, 10));
            continue;
        }
    }
    printf("  ret\n");

    return 0;
}