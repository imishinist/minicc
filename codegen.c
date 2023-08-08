#include "minicc.h"

static char *argreg[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

static int count(void) {
    static int i = 1;
    return i++;
}

void codegen() {
    // アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");
    for (int i = 0; code[i]; i++) {
        gen(code[i]);
        printf("  pop rax\n");
    }

    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");

    printf("  ret\n");
}

void gen_lval(Node *node) {
    if (node->kind != ND_LVAR)
        error("not local variable");

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
}

//
// Code generator
//
void gen(Node *node) {
    int c;
    switch (node->kind) {
    case ND_IF:
        c = count();
        if (node->els) {
            gen(node->cond);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .L.else.%d\n", c);
            gen(node->then);
            printf("  jmp .L.end.%d\n", c);
            printf(".L.else.%d:\n", c);
            gen(node->els);
            printf(".L.end.%d:\n", c);
        } else {
            gen(node->cond);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .L.end.%d\n", c);
            gen(node->then);
            printf(".L.end.%d:\n", c);
        }
        return;
    case ND_WHILE:
        c = count();
        printf(".L.begin.%d:\n", c);
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .L.end.%d\n", c);
        gen(node->then);
        printf("  jmp .L.begin.%d\n", c);
        printf(".L.end.%d:\n", c);
        return;
    case ND_FOR:
        c = count();
        if (node->init)
            gen(node->init);
        printf(".L.begin.%d:\n", c);
        if (node->cond) {
            gen(node->cond);
            printf("  cmp rax, 0\n");
            printf("  je .L.end.%d\n", c);
        }
        gen(node->then);
        if (node->inc)
            gen(node->inc);
        printf("  jmp .L.begin.%d\n", c);
        printf(".L.end.%d:\n", c);
        return;
    case ND_FUNCALL:
        int nargs = 0;
        for (Node *arg = node->args; arg; arg = arg->next) {
            gen(arg);
            printf("  push rax\n");
            nargs++;
        }
        for (int i = nargs - 1; i >= 0; i--)
            printf("  pop %s\n", argreg[i]);

        printf("  mov rax, 0\n");
        printf("  call %s\n", node->funcname);
        return;
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ASSIGN:
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    case ND_RETURN:
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        printf("  add rax, rdi\n");
        break;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        break;
    case ND_NUM:
        // already handled
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");       // cmpで比較したレジスタの値が同じだったらalに1がセットされる
        printf("  movzb rax, al\n");
        break;
    case ND_NE:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LT:
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    }
    printf("  push rax\n");
}
