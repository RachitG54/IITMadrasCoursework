	//FLEX FILE adder.l 

%{
#include <stdio.h>
#include<string.h>
#include "P1.tab.h" // the output of bison on adder.y
void yyerror(char*); 
int yyparse(void); 
%}

%%
				

"//"[^\n]*[\n] 	;
"("				return LEFT;
")"				return RIGHT;
"+"				return PLUS; 
"-"				return MINUS;
"*"				return MULTIPLY;
"/"				return DIVIDE;
"="				return EQUALS;
";"				return SEMICOLON;
"class"			return CLASS;
"{"				return CLEFT;
"["				return SLEFT;
"}"				return CRIGHT;
"]"				return SRIGHT;
"public"		return PUBLIC;
"static void main" return FUNCDEF;
"String"		return STRINGD;
"System.out.println"	return PRINTS;
"extends"		return EXTENDS;
"?"				return QMARK;
"return"		return RETURNS;
"int"			return INTD;
"boolean"		return BOOLEAND;
"if"			return IFS;
"else"			return ELSES;
"while"			return WHILES;
"&&"			return ANDS;
"||"			return ORS;
"<"				return LESSTHAN;
"!"				return NOTS;
"."				return DOTS;
"length"		return LENGTH;
","				return COMMA;
"true"			return TRUES;
"false"			return FALSES;
"this"			return THIS;
"new"			return NEW;
"#define"		return DEFINE;

0|[1-9][0-9]*			{ 
					//printf("Saw an integer %s\n",yytext); 
					strcpy(yylval.string_val,yytext);
					return INTEGER; 
				}
[a-z|A-Z|_][a-z|A-Z|0-9|_]*         {
					strcpy(yylval.string_val,yytext);
					return STRING; 
				}
[ \t\n]+		;		//do nothing on whitespace
<<EOF>>			{ return 0; }
.               {  return 1;}

%%

void yyerror(char* str) { printf("// Failed to parse macrojava code."); }

int yywrap(void) { return 1; }

int main(void) {
	yyparse();	
}
