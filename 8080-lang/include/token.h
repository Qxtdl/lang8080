#pragma once

enum Token {
    TOKEN_FN,
    TOKEN_AT,
    TOKEN_IF,
    TOKEN_RETURN,
    TOKEN_GOTO,
    TOKEN_ASM,    

    TOKEN_VOID,
    TOKEN_BYTE,
    TOKEN_WORD,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_EQUAL,
    TOKEN_EQUALS,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_COMMA,
    TOKEN_STAR,
    TOKEN_SEMICOLON,
    TOKEN_COLON,

    // fakes
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,

    TOKEN_LAST
};

static const char *keywords[] = {
    [TOKEN_FN] = "fn",
    [TOKEN_AT] = "at",
    [TOKEN_IF] = "if",
    [TOKEN_RETURN] = "return",
    [TOKEN_GOTO] = "goto",
    [TOKEN_ASM] = "asm", 

    [TOKEN_VOID] = "void",
    [TOKEN_BYTE] = "byte",
    [TOKEN_WORD] = "word",

    [TOKEN_PLUS] = "+",
    [TOKEN_MINUS] = "-",
    [TOKEN_EQUAL] = "=",
    [TOKEN_EQUALS] = "==",

    [TOKEN_LPAREN] = "(",
    [TOKEN_RPAREN] = ")",
    [TOKEN_LBRACE] = "{",
    [TOKEN_RBRACE] = "}",
    [TOKEN_COMMA] = ",",
    [TOKEN_STAR] = "*",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_COLON] = ":",
};
