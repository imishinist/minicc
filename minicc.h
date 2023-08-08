#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// tokenize.c
//

// トークンの種類
typedef enum {
    TK_RESERVED, // 記号
    TK_IDENT,    // 識別子
    TK_NUM,      // 整数トークン
    TK_EOF,      // 入力の終わりを表すトークン
    TK_RETURN,   // return
    TK_IF,
    TK_ELSE,
    TK_FOR,
    TK_WHILE,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind; // トークンの型
    Token *next;    // 次の入力トークン
    int val;        // kindがTK_NUMの場合、その数値
    char *str;      // トークン文字列
    int len;        // トークンの長さ
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
Token *consume_kind(TokenKind tk);
void expect(char *op);
int expect_number();
bool at_eof();
bool is_ident1(char c);
bool is_ident2(char c);
Token *tokenize();

//
// parse.c
//

// AST node type
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // number

  ND_EQ, // ==
  ND_NE, // !=
  ND_LT, // <
  ND_LE, // <=

  ND_ASSIGN, // =
  ND_LVAR,   // local variable
  ND_RETURN, // return

  ND_IF,
  ND_WHILE,
  ND_FOR,

  ND_FUNCALL, // function call
} NodeKind;

typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

// AST node type
typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *next;
    Node *lhs;
    Node *rhs;

    // "if" or "for" statement
    Node *cond;
    Node *then;
    Node *els;
    Node *init;
    Node *inc;

    // "function calling"
    char *funcname;
    Node *args;

    int val;
    int offset;
};

extern Node *code[100];

Node *program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

LVar *find_lvar(Token *tok);

//
// codegen.c
//
void codegen();
void gen(Node *node);


extern Token *token;
extern char *user_input;
