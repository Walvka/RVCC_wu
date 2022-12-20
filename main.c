#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum{
    TK_PUNCT,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token{
    TokenKind Kind;
    Token *Next;
    int Val;
    char *Loc;
    int Len;
};

static void error(char* Fmt, ...){
    va_list VA;
    va_start(VA, Fmt);
    vfprintf(stderr, Fmt, VA);
    fprintf(stderr, "\n");

    va_end(VA);
    exit(1);
}

static int equal(Token *Tok, char *Str){
    return memcmp(Tok->Loc, Str, Tok->Len) == 0 && Str[Tok->Len] == '\0';
}

static Token *skip(Token *Tok, char *Str){
    if(!equal(Tok,Str)){
        error("expect '%s'", Str);
    }
    return Tok->Next;
}

static int getNumber(Token *Tok){
    if(Tok->Kind != TK_NUM){
        error("expect a number");
    }
    return Tok->Val;
}

static Token *newToken(TokenKind Kind, char *Start, char *End){
    Token *Tok = calloc(1, sizeof(Token));
    Tok->Kind = Kind;
    Tok->Loc = Start;
    Tok->Len = End - Start;
    return Tok;
}

static Token *tokenize(char *P){
    Token Head = {};
    Token *Cur = &Head;

    while(*P){
        if(isspace(*P)){
            ++P;
            continue;
        }

        if(isdigit(*P)){
            Cur->Next = newToken(TK_NUM, P, P);
            Cur = Cur->Next;
            const char *OldPtr = P;
            Cur->Val = strtoul(P, &P, 10);
            Cur->Len = P - OldPtr;
            continue;
        }

        if(*P == '+' || *P == '-'){
            Cur->Next = newToken(TK_PUNCT, P, P+1);
            Cur = Cur->Next;
            ++P;
            continue;
        }

        error("invalid token: %c", *P);
    }

    Cur->Next = newToken(TK_EOF, P, P);

    return Head.Next;
}

int main(int Argc, char **Argv) {

    if(Argc != 2){
        fprintf(stderr, "%s: invalid number of arguments\n", Argv[0]);
        return 1;
    }

    Token *Tok = tokenize(Argv[1]);
    printf("  .global main\n");
    printf("main:\n");
    printf("  li a0, %d\n", getNumber(Tok));
    Tok = Tok->Next;

    while(Tok->Kind != TK_EOF){
        if(equal(Tok, "+")){
            Tok = Tok->Next;
            printf("  addi a0, a0, %d\n", getNumber(Tok));
            Tok = Tok->Next;
            continue;
        }

        Tok = skip(Tok, "-");
        printf("  addi a0, a0, -%d\n", getNumber(Tok));
        Tok = Tok->Next;
    }
    printf("  ret\n");

    return 0;
}