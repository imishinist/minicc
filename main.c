#include "minicc.h"

Token *token;
char *user_input;

extern Node *code[100];

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }
    user_input = argv[1];
    token = tokenize();

    // for (Token *t = token; t; t = t->next) fprintf(stderr, "'%.*s' (%d) = %d\n", t->len, t->str, t->val, t->kind);
    program();

    codegen();
    return 0;
}
