#define FALSE 257
#define TRUE 258
#define IF 259
#define ELSE 260
#define WHILE 261
#define INT 262
#define RETURN 263
#define BREAK 264
#define NUM 265
#define LE 266
#define GE 267
#define EQ 268
#define NE 269
#define ID 270
typedef union
{
    int   integer;
    char*  string;
    char  character;
    AST_Node* node;
} YYSTYPE;
extern YYSTYPE yylval;
