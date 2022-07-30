#include <stdio.h>
#include <ctype.h>
/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char token_list[100][100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int getNextToken(void);
void ifstmt(void);
void forLoop(void);
void forEach(void);
void whileLoop(void);
void doWhile(void);
void block(void);
void switchStmt(void);
void assignmentstmt(void);
void returnStmt(void);

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define FOREACH_TOKEN 27
#define FOR_TOKEN 28
#define SWITCH_TOKEN 29
#define DO_TOKEN 30
#define WHILE_TOKEN 31
#define IF_TOKEN 32
#define RETURN 33
#define LEFT_BLOCK 34
#define RIGHT_BLOCK 35
#define IF_CODE 36
#define ELSE_CODE 37
#define SEMICOLON 38
#define CASE 39
/******************************************************/

/* main driver */
int main(void)
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("sample.in", "r")) == NULL)
        printf("ERROR - cannot open sample.in \n");
    else
    {
        getChar();
        do
        {
            getNextToken();
            if (nextToken == IF_CODE)
                ifstmt();
            else if (nextToken == FOR_TOKEN)
                forLoop();
            else if (nextToken == FOREACH_TOKEN)
                forLoop();
            else if (nextToken == DO_TOKEN)
                doWhile();
            else if (nextToken == WHILE_TOKEN)
                whileLoop();
        } while (nextToken != EOF);
    }
    return 0;
}

// ifstmt --> function code for if statement
// Java if --> if '(' <boolexpr> ')' <statement> [else <statement>]
void ifstmt(void)
{
    if (nextToken != IF_CODE)
        error();
    else
    {
        getNextToken();
        if (nextToken != LEFT_PAREN)
            error();
        else
        {
            getNextToken();
            boolexpr();
            if (nextToken != RIGHT_PAREN)
                error();
            else
            {
                getNextToken();
                statement();
                if (nextToken == ELSE_CODE)
                {
                    getNextToken();
                    statement();
                }
            }
        }
    }
}

// for - function code for for loop
// Java for --> for '(' <options> ')' { <statement> }

void forLoop(void)
{
    if (nextToken != FOR_TOKEN)
    {
        error();
    }
    else
    {
        getNextToken();
        if (nextToken != LEFT_PAREN)
        {
            error();
        }
        else
        {
            getNextToken();
            options();
            if (nextToken != RIGHT_PAREN)
            {
                error();
            }
            else
            {
                getNextToken();
                if (nextToken != LEFT_BLOCK)
                    error();
                else
                {
                    statement();
                    if (nextToken != RIGHT_BLOCK)
                        error();
                    else
                        getNextToken();
                }
            }
        }
    }
}

// forEach - function code for foreach loop
// Java foreach --> foreach '(' <options> ')' <statement>

void forEach(void)
{
    if (nextToken != FOREACH_TOKEN)
    {
        error();
    }
    else
    {
        getNextToken();
        if (nextToken != LEFT_PAREN)
        {
            error();
        }
        else
        {
            getNextToken();
            options();
            if (nextToken != RIGHT_PAREN)
            {
                error();
            }
            else
            {
                getNextToken();
                statement();
            }
        }
    }
}

// options --> id | id()

void options(void)
{
    if (nextToken != id)
    {
        error();
    }
    else
    {
        getNextToken();
        if (nextToken == LEFT_PAREN)
        {
            getNextToken();
            if (nextToken != RIGHT_PAREN)
            {
                error();
            }
            else
            {
                getNextToken();
            }
        }
    }
}

// whileLoop - function code for do-while loop
// Java while --> while '(' <condition> ')' '{' <statement> '}'

void whileLoop(void)
{
    if (nextToken != WHILE_TOKEN)
        error();
    else
    {
        getNextToken();
        if (nextToken != LEFT_PAREN)
            error();
        else
        {
            condition();
            if (nextToken != RIGHT_PAREN)
                error();
            else
            {
                getnextToken();
                if (nextToken != LEFT_BLOCK)
                    error();
                else
                {
                    statement();
                    if (nextToken != RIGHT_BLOCK)
                        error();
                    else
                        getNextToken();
                }
            }
        }
    }
}

// do-while - function code for do-while loop
// Java do-while --> do '{' <statement> '}' while '(' <condition> ')'

void doWhile(void)
{
    if (nextToken != DO_TOKEN)
        error();
    else
    {
        getNextToken();
        if (nextToken != LEFT_BLOCK)
            error();
        else
        {
            getNextToken();
            statement();
            if (nextToken != RIGHT_BLOCK)
                error();
            else
            {
                getnextToken();
                if (nextToken != WHILE_TOKEN)
                    error();
                else
                {
                    getNextToken();
                    if (nextToken != LEFT_PAREN)
                        error();
                    else
                    {
                        getNextToken();
                        condition();
                        if (nextToken != RIGHT_PAREN)
                            error();
                        else
                        {
                            getNextToken();
                            if (nextToken != SEMICOLON)
                                error();
                            else
                                getNextToken();
                        }
                    }
                }
            }
        }
    }
}

// block - function code for block
// block --> '{' <statement> '}'
void block(void)
{
    if (nextToken != LEFT_BLOCK)
        error();
    else
    {
        statement();
        if (nextToken != RIGHT_BLOCK)
            error();
        else
            getNextToken();
    }
}

// <assignmentstmt> identifier = <term>;
// <term> → identifier | int_literal

void assignmentstmt(void)
{
    if (nextToken == IDENT)
        getNextToken(); /* Get the next token */
                        /* It was not an identifier*/
    else
        error();
    if (nextToken == ASSIGN_OP)
        getNextToken();
    else
        error();
    term(); /* Parse the term */
    if (nextToken == SEMICOLON)
        getNextToken();
    else
        error();
}

void term(void)
{
    /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT)
        getNextToken();
    else
        error();
}

// switchStmt - function code for Switch Statement
// Java switch ( <expression> ) '{' case value: <statement> '}'
// value - 0 | 1

void switchStmt(void)
{
    if (nextToken != SWITCH_TOKEN)
        error();
    else
    {
        getNextToken();
        if (nextToken != LEFT_PAREN)
            error();
        else
        {
            expression();
            getNextToken();
            if (nextToken != RIGHT_PAREN)
                error();
            else
            {
                getNextToken();
                if (nextToken != LEFT_BLOCK)
                    error();
                else
                {
                    getNextToken();
                    if (nextToken != CASE)
                        error();
                    else
                    {
                        statement();
                        if (nextToken != RIGHT_BLOCK)
                            error();
                        else
                            getNextToken();
                    }
                }
            }
        }
    }
}

// returnStmt - function code for return statement
// Java return - return returnvalue;
// returnvalue - 0 | 1

void returnStmt(void)
{
    if (nextToken != RETURN)
        error();
    else
    {
        getNextToken();
        if (nextToken != 0 || nextToken != 1)
            error();
        else
            getNextToken();
    }
}

/******************************************************/
/* lookup - a function to look up operators and
parentheses and return the token */
int lookup(char ch)
{
    switch (ch)
    {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    case '{':
        addChar();
        nextToken = LEFT_BLOCK;
        break;
    case '}':
        addChar();
        nextToken = RIGHT_BLOCK;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void)
{
    if (lexLen <= 98)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    }
    else
        printf("Error - lexeme is too long \n");
}

/******************************************************/
/* getChar - a function to get the next character of
input and determine its character class */

void getChar(void)
{
    if ((nextChar = getc(in_fp)) != EOF)
    {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}

/******************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank(void)
{
    while (isspace(nextChar))
        getChar();
}

/******************************************************/
/* getNextToken - Lexical Analyzer which returns tokens */
int getNextToken(void)
{
    lexLen = 0;
    getNonBlank();
    switch (charClass)
    {

    /* Identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        if (strcmp(lexeme, "switch") == 1)
        {
            nextToken = SWITCH_TOKEN;
            break;
        }
        else if (strcmp(lexeme, "foreach") == 1)
        {
            nextToken = FOREACH_TOKEN;
            break;
        }
        else if (strcmp(lexeme, "for") == 1)
        {
            nextToken = FOR_TOKEN;
            break;
        }
        else if (strcmp(lexeme, "while") == 1)
        {
            nextToken = FOR_TOKEN;
            break;
        }
        else if (strcmp(lexeme, "if") == 1)
        {
            nextToken = FOR_TOKEN;
            break;
        }
        else if (strcmp(lexeme, "return") == 1)
        {
            nextToken = FOR_TOKEN;
            break;
        }
        nextToken = IDENT;
        break;

    /* Integer literals */
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

    /* Parentheses and operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

    /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = '\0';
        break;
    } /* End of switch */
    printf("Next token is: %d, Next lexeme is %s\n",
           nextToken, lexeme);
    return nextToken;
} /* End of function getNextToken */
