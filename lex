%{
#include <stdio.h>
#include <string.h>
#include "syndax_tree.h"
#include "y.tab.h"
#include "log.h"


#define N 8  // number of the reserved words of the language
//#define TRUE 1
//#define FALSE 0
extern int line_count;
int is_reserved(char* lexeme);

///parse the tokens found by the lexer to parser in syd.y by the @yylval
///which type is declared in y.tab.h
%}

%%

[a-zA-Z][a-zA-Z0-9"_"]*         {
                                   int i;
                                   fprintf(file, "%s\n",yytext);
                                   i=is_reserved(yytext);
                                   if(i==ID) {
                                       yylval.string = strdup(yytext);
                                       fprintf(file, "ID\n");
                                   }
                                   else {
                                       yylval.integer = i;
                                       fprintf(file,"RESERVED\n");
                                   }
                                   return (i);
                                 }
[1-9][0-9]*                     {
                                    fprintf(file, "%s\n",yytext);
                                    yylval.string = strdup(yytext);
                                    return (NUM);
                                }
"0"                             {
                                    fprintf(file, "%s\n",yytext);
                                    yylval.string = strdup(yytext);
                                    return (NUM);
                                }
"*"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('*');
                            }
"/"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('/');
                            }
"+"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('+');
                            }
"-"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('-');
                            }
"="                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('=');
                            }
"<"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('<');
                            }
">"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('>');
                            }
"<="                        {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return (LE);
                            }
">="                        {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return (GE);
                            }
"=="                        {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return (EQ);
                            }
"!="                        {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return (NE);
                            }
"("                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('(');
                            }
")"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return (')');
                            }
";"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return (';');
                            }
"{"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('{');
                            }
"}"                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return ('}');
                            }
","                         {
                                fprintf(file, "%s\n",yytext);
                                yylval.string = strdup(yytext);
                                return (',');
                            }
"//"                        {
                                register int c;       // register flag for the compiler so that variable "c"
                                                      // will be stored in a register for regural use in the code below
                                int done = 0;

                                do {
                                    while((c=input()) != '\n' && c!=EOF);   //repeat for every input that isn't the character '/n'
                                    if (c=='\n') {
                                        done = 1;
                                        line_count++;
                                    }
                                    if (c==EOF) {
                                        printf("End of File");
                                        done = 1;
                                    }
                                } while(!done);

                            }
[\n]                        {
                                line_count++;
                            }
[ \t]+            {}   //ignore whitespaces
.                   { fprintf(file, "Unknown Character %s\n", yytext); }

%%

static char* reserved[N] = {"false", "true", "if", "else", "while", "int", "return", "break"};
static int reserved_values[N] = {FALSE, TRUE, IF, ELSE, WHILE, INT, RETURN, BREAK};

int is_reserved(char* lexeme) {
	int i;
  	for (i=0; i<N && strcmp(lexeme, reserved[i]) !=0; i++);

   	if(i<N)
        return reserved_values[i];
    else
        return ID;
}
