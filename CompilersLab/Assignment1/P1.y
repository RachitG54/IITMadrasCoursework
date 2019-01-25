/* BISON FILE adder.y */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex();
extern void yyerror(char *);
void print(char*);
char* increate(int);
void strcat2(char*,char*);
int strlen2(char*);
char* arr1[1000];
char* arr2[1000];
char* arr3[1000];
int indice = 0;
%}

%union {
	int	int_val;
	char   string_val[256];
	char *dstring_val;
}

%token <string_val>  INTEGER STRING 
%token <dstring_val> PLUS SEMICOLON MINUS MULTIPLY DIVIDE EQUALS LEFT RIGHT CLASS CLEFT CRIGHT SLEFT SRIGHT PUBLIC FUNCDEF STRINGD PRINTS EXTENDS QMARK RETURNS INTD BOOLEAND IFS ELSES WHILES ANDS ORS LESSTHAN NOTS DOTS LENGTH COMMA TRUES FALSES THIS NEW DEFINE
%type <dstring_val> Goal MacroDefinitionS TypeDeclarationS MainClass TypeIdentifierS MethodDeclarationS TypeDeclaration CommaTypeIdS CommaTypeIdQ StatementS MethodDeclaration Type CommaExpS CommaExpQ Statement Expression PrimaryExpression MacroDefinition CommaIdS CommaIdQ MacroDefStatement MacroDefExpression Identifier

%start Goal

%nonassoc "then"
%nonassoc ELSES
%%
Goal: MacroDefinitionS MainClass TypeDeclarationS {$$ = increate(strlen2($1)+strlen2($2)+strlen2($3));strcat2($$,$1);strcat2($$,$2);strcat($$,$3);print($$);};

MacroDefinitionS : %empty{$$ = increate(0);$$[0] = '\0';}
			| MacroDefinition MacroDefinitionS{char* temp = increate(strlen2($1)+strlen2($2));strcat2(temp,$1);strcat(temp,$2);$$ = temp;}

TypeDeclarationS : %empty{$$ = increate(0);$$[0] = '\0';}
			| TypeDeclaration TypeDeclarationS{char* temp = increate(strlen2($1)+strlen2($2));strcat2(temp,$1);strcat(temp,$2);$$ = temp;}

MainClass : CLASS Identifier CLEFT PUBLIC FUNCDEF LEFT STRINGD SLEFT SRIGHT Identifier RIGHT CLEFT PRINTS LEFT Expression RIGHT SEMICOLON CRIGHT CRIGHT{$$ = increate(strlen2("class")+strlen2($2)+strlen2("()")+strlen2("public")+strlen2("static void main")+strlen2("()")+strlen2("string () { System.out.println ( ) ; }{}{}")+strlen2("[]")+strlen2($10)+strlen2($15));
strcat2($$,"class");strcat2($$,$2);strcat2($$,"{ public static void main( String[]");strcat2($$,$10);strcat2($$,"){ System.out.println (");strcat2($$,$15);strcat2($$,");}}");
}

TypeIdentifierS : %empty{$$ = increate(0);$$[0] = '\0';}
			| TypeIdentifierS  Type Identifier SEMICOLON{char* temp = increate(strlen2($1)+strlen2($2)+strlen2($3)+2);/*strcat2(temp,$1);strcat2(temp,$2);strcat2($$,$3);strcat2($$,";");*/sprintf($$,"%s%s %s;",$1,$2,$3); $$ = $1;}

MethodDeclarationS : %empty{$$ = increate(0);$$[0] = '\0';}
			| MethodDeclaration MethodDeclarationS{char* temp = increate(strlen2($1)+strlen2($2));strcat2(temp,$1);strcat(temp,$2);$$ = temp;}

TypeDeclaration : CLASS Identifier CLEFT TypeIdentifierS MethodDeclarationS CRIGHT{
	$$ = increate(strlen2("class")+strlen2($2)+strlen2("{")+strlen2($4)+strlen2($5)+strlen2("}"));
	strcat2($$,"class");strcat2($$,$2);strcat2($$,"{");strcat2($$,$4);strcat2($$,$5);strcat2($$,"}");
}
			| CLASS Identifier EXTENDS Identifier CLEFT TypeIdentifierS MethodDeclarationS CRIGHT{
	$$ = increate(strlen2("class")+strlen2($2)+strlen2("{")+strlen2($6)+strlen2($7)+strlen2("}")+strlen2("extends")+strlen2($4));
	strcat2($$,"class");strcat2($$,$2);strcat2($$,"extends");strcat2($$,$4);strcat2($$,"{");strcat2($$,$6);strcat2($$,$7);strcat2($$,"}");
}

CommaTypeIdS : %empty{$$ = increate(0);$$[0] = '\0';}
			| COMMA Type Identifier CommaTypeIdS{
			char* temp = increate(strlen2($2)+strlen2($3)+strlen2($4)+2);
			strcat2(temp,",");strcat2(temp,$2);strcat2(temp,$3);strcat2(temp,$4); $$ = temp;
			}

CommaTypeIdQ : %empty{$$ = increate(0);$$[0] = '\0';}
			| Type Identifier CommaTypeIdS{

			$$ = increate(strlen2($1)+strlen2($2)+strlen2($3)+2);
			strcat2($$,$1);strcat2($$,$2);strcat($$,$3); 
			}
StatementS : %empty{$$ = increate(0);$$[0] = '\0';}
			| Statement StatementS{char* temp = increate(strlen2($1)+strlen2($2));strcat2(temp,$1);strcat(temp,$2);$$ = temp;}

MethodDeclaration : PUBLIC Type Identifier LEFT CommaTypeIdQ RIGHT CLEFT TypeIdentifierS StatementS RETURNS Expression SEMICOLON CRIGHT{
	
	$$ = increate(strlen("public ( ) { return ; }")+strlen($2)+strlen($3)+strlen($5)+strlen($8)+strlen($9)+strlen($11));
	strcat2($$,"public");strcat2($$,$2);strcat2($$,$3);strcat2($$,"(");strcat2($$,$5);strcat2($$,"){");strcat2($$,$8);
	strcat2($$,$9);strcat2($$,"return");strcat2($$,$11);strcat($$,";}");
}

Type : INTD SLEFT SRIGHT{$$ = increate(strlen2("int [] "));strcpy($$,"int [] ");}
			| BOOLEAND{$$ = increate(strlen2("boolean"));strcpy($$,"boolean ");}
			| INTD{$$ = increate(strlen2("int "));strcpy($$,"int ");}
			| Identifier{$$ = increate(strlen2($1));strcat2($$,$1);}

CommaExpS : %empty{$$ = increate(0);$$[0] = '\0';}
			| COMMA Expression CommaExpS{char* temp = increate(strlen2(",")+strlen2($2)+strlen2($3));strcat2(temp,",");strcat2(temp,$2);strcat2(temp,$3);$$ = temp;}


CommaExpQ : %empty{$$ = increate(0);$$[0] = '\0';}
			| Expression CommaExpS{$$ = increate(strlen2($1)+strlen2($2));strcat2($$,$1);strcat2($$,$2);}

Statement : CLEFT StatementS CRIGHT{$$ = increate(strlen($2)+2);strcat2($$,"{");strcat2($$,$2);strcat2($$,"}");}
			| PRINTS LEFT Expression RIGHT SEMICOLON{$$ = increate(strlen2($3)+strlen2("System.println")+4);strcat2($$,"System.out.println");strcat2($$,"(");strcat2($$,$3);strcat2($$,");");}
			| Identifier EQUALS Expression SEMICOLON{$$ = increate(strlen2($1)+strlen2($3)+3);
			  strcat2($$,$1);strcat2($$,"=");strcat2($$,$3);strcat($$,";");
			}
			| Identifier SLEFT Expression SRIGHT EQUALS Expression SEMICOLON{

			$$ = increate(strlen2($1)+strlen2($3)+strlen2($6)+11);strcat2($$,$1);strcat2($$,"[");strcat2($$,$3);strcat2($$,"]=");strcat2($$,$6);strcat2($$,";");
			}
			| IFS LEFT Expression RIGHT Statement       			%prec "then"   {$$ = increate(strlen2($3)+strlen2($5)+6);strcat2($$,"if(");strcat2($$,$3);strcat2($$,")");strcat2($$,$5);}
		
			| IFS LEFT Expression RIGHT Statement ELSES Statement {$$ = increate(strlen2($3)+strlen2($5)+12+ strlen($7));strcat2($$,"if(");strcat2($$,$3);strcat2($$,")");strcat2($$,$5);strcat2($$,"else");strcat($$,$7);}
			| WHILES LEFT Expression RIGHT Statement{$$ = increate(strlen2($3)+strlen2($5)+12);strcat2($$,"while(");strcat2($$,$3);strcat2($$,")");strcat2($$,$5);}
			| Identifier LEFT CommaExpQ RIGHT SEMICOLON{$$ = increate(strlen2($1)+strlen2($3)+4);strcat2($$,$1);strcat2($$,"(");strcat2($$,$3);strcat2($$,");");
			int i;
				for(i=indice-1;i>=0;i--)
				{
					if(strcmp(arr1[i],$1)==0)
					{
						char* token1 = strtok(arr3[i], ",");
						char* token2 = strtok($3, ",");
						char* anscpy = increate(1100);
						char* arr2cpy = increate(1100);
						strcpy(arr2cpy,arr2[i]);
						anscpy[0]='\0';
						int ansindex = 0;
					    while(( token1 != NULL )&&(token2!=NULL)) 
					    {
						  int lenarr2 = strlen(arr2cpy);
						  int lentok = strlen(token1);
					      int j,k;
					      for(j=0;j<lenarr2;j++)
					      {
					      	for(k=0;k<lentok;k++)
					      	{
					      		if(arr2cpy[j+k]!=token1[k])
					      		{
					      			break;
					      		}
					      	}
					      	if(k==lentok)
					      	{
					      		int x=0;
					      		for(x=0;token2[x]!='\0';x++)
					      		{
					      		  anscpy[ansindex++] = token2[x];
					      		}
					      		j = j+k-1;
					      	}
					      	else
					      	{
					      		anscpy[ansindex++] = arr2cpy[j];
					      	}
					      }
					      anscpy[ansindex++] = '\0';
					      strcpy(arr2cpy,anscpy);
					      token1 = strtok(NULL, ",");
					      token2 = strtok(NULL, ",");
					    }
					    $$ = increate(500);
						strcpy($$,arr2cpy);
						break;
					}
				}

			}

Expression : PrimaryExpression ANDS PrimaryExpression{$$ = increate(strlen2($1)+strlen2("and")+strlen2($3));strcat2($$,$1);strcat2($$,"&&");strcat($$,$3);}
			| PrimaryExpression ORS PrimaryExpression{$$ = increate(strlen2($1)+strlen2("and")+strlen2($3));strcat2($$,$1);strcat2($$,"||");strcat($$,$3);}
			| PrimaryExpression NOTS EQUALS PrimaryExpression{$$ = increate(strlen2($1)+strlen2("and")+strlen2($4));strcat2($$,$1);strcat2($$,"!=");strcat($$,$4);}
			| PrimaryExpression LESSTHAN EQUALS PrimaryExpression{$$ = increate(strlen2($1)+strlen2("and")+strlen2($4));strcat2($$,$1);strcat2($$,"<=");strcat2($$,$4);}
			| PrimaryExpression PLUS PrimaryExpression{$$ = increate(strlen2($1)+strlen2("+")+strlen2($3));strcat2($$,$1);strcat2($$,"+");strcat2($$,$3);}
			| PrimaryExpression MINUS PrimaryExpression{$$ = increate(strlen2($1)+strlen2("and")+strlen2($3));strcat2($$,$1);strcat2($$,"-");strcat2($$,$3);}
			| PrimaryExpression MULTIPLY PrimaryExpression{$$ = increate(strlen2($1)+strlen2("and")+strlen2($3));strcat2($$,$1);strcat2($$,"*");strcat2($$,$3);}
			| PrimaryExpression DIVIDE PrimaryExpression{$$ = increate(strlen2($1)+strlen2("and")+strlen2($3));strcat2($$,$1);strcat2($$,"/");strcat2($$,$3);}
			| PrimaryExpression SLEFT PrimaryExpression SRIGHT{$$ = increate(strlen2($1)+strlen2("and")+strlen2($3));strcat2($$,$1);strcat2($$,"[");strcat2($$,$3);strcat($$,"]");}
			| PrimaryExpression DOTS LENGTH{$$ = increate(strlen2($1)+strlen(".length")); strcpy($$,$1);strcat($$,".length");}
			| PrimaryExpression{$$ = increate(strlen2($1)); strcat2($$,$1);}
			| PrimaryExpression DOTS Identifier LEFT CommaExpQ RIGHT{$$ = increate(strlen2($1)+2+strlen2($3)+2+strlen2($5)+1);
			strcat2($$,$1);strcat2($$,".");strcat2($$,$3);strcat2($$,"(");
			strcat2($$,$5);strcat($$,")");
			}
			| Identifier LEFT CommaExpQ RIGHT{
			$$ = increate(strlen2($1)+2+strlen2($3)+1);strcat2($$,$1);strcat2($$,"(");strcat2($$,$3);strcat($$,")");
			int i;
				for(i=indice-1;i>=0;i--)
				{
					if(strcmp(arr1[i],$1)==0)
					{
						char* token1 = strtok(arr3[i], ",");
						char* token2 = strtok($3, ",");
						char* anscpy = increate(1100);
						char* arr2cpy = increate(1100);
						strcpy(arr2cpy,arr2[i]);
						anscpy[0]='\0';
						int ansindex = 0;
					    while(( token1 != NULL )&&(token2!=NULL)) 
					    {
						  int lenarr2 = strlen(arr2cpy);
						  int lentok = strlen(token1);
					      int j,k;
					      for(j=0;j<lenarr2;j++)
					      {
					      	for(k=0;k<lentok;k++)
					      	{
					      		if(arr2cpy[j+k]!=token1[k])
					      		{
					      			break;
					      		}
					      	}
					      	if(k==lentok)
					      	{
					      		int x=0;
					      		for(x=0;token2[x]!='\0';x++)
					      		{
					      		  anscpy[ansindex++] = token2[x];
					      		}
					      		j = j+k-1;
					      	}
					      	else
					      	{
					      		anscpy[ansindex++] = arr2cpy[j];
					      	}
					      }
					      anscpy[ansindex++] = '\0';
					      strcpy(arr2cpy,anscpy);
					      token1 = strtok(NULL, ",");
					      token2 = strtok(NULL, ",");
					    }
					    $$ = increate(500);
						strcpy($$,arr2cpy);
						break;
					}
				}
			}

PrimaryExpression : INTEGER{$$ = increate(strlen2($1));strcpy($$,$1);}
			| TRUES{$$ = increate(strlen2("true"));strcpy($$,"true");}
			| FALSES{$$ = increate(strlen2("false"));strcpy($$,"false");}
			| Identifier{$$ = increate(strlen2($1));strcpy($$,$1);}
			| THIS{$$ = increate(strlen2("this"));strcpy($$,"this");}
			| NEW INTD SLEFT Expression SRIGHT{$$ = increate(strlen2($4)+12);strcpy($$,"new int [");strcat2($$,$4);strcat($$,"]");}
			| NEW Identifier LEFT RIGHT{$$ = increate(strlen2($2)+8);strcat2($$,"new");strcat2($$,$2);strcat($$,"()");}
			| NOTS Expression {$$ = increate(strlen2($2)+1);strcat2($$,"!");strcat($$,$2);}
			| LEFT Expression RIGHT{$$ = increate(strlen2($2)+4);strcpy($$,"(");strcat2($$,$2);strcat($$,")");}

MacroDefinition : MacroDefExpression{$$ = increate(strlen2($1));strcpy($$,$1);}
			| MacroDefStatement{$$ = increate(strlen2($1));strcpy($$,$1);}

CommaIdS : %empty{$$ = increate(0);$$[0] = '\0';}
			| COMMA Identifier CommaIdS{int totallength = strlen2($2)+strlen2($3)+4; char* temp = increate(totallength);
			strcat2(temp,",");strcat2(temp,$2);strcat2(temp,$3);$$ = temp;
			}

CommaIdQ : %empty{$$ = increate(0);$$[0] = '\0';}
			| Identifier CommaIdS{$$ = increate(strlen2($1)+strlen2($2));strcat2($$,$1);strcat($$,$2);}

MacroDefStatement : DEFINE Identifier LEFT CommaIdQ RIGHT CLEFT StatementS CRIGHT{
	arr1[indice] = increate(strlen($2)); strcpy(arr1[indice],$2);
    arr2[indice] = increate(strlen($7)); strcpy(arr2[indice],$7);
    arr3[indice] = increate(strlen($4)); strcpy(arr3[indice],$4);
    indice++;
    $$ = increate(0);$$[0] = '\0';
	/*int totallength = strlen2("#define")+strlen2($2)+1+strlen2($4)+2+strlen2($7)+1;$$ = increate(totallength);
	strcpy($$,"#define ");strcat2($$,$2);strcat2($$,"(");strcat2($$,$4);strcat2($$,"){");strcat2($$,$7);strcat($$,"}");*/
}

MacroDefExpression : DEFINE Identifier LEFT CommaIdQ RIGHT LEFT Expression RIGHT{
    arr1[indice] = increate(strlen($2)); strcpy(arr1[indice],$2);
    arr2[indice] = increate(strlen($7)); strcpy(arr2[indice],$7);
    arr3[indice] = increate(strlen($4)); strcpy(arr3[indice],$4);
    indice++;
    $$ = increate(0);$$[0] = '\0'; 
	/*int totallength = strlen2("#define")+strlen2($2)+1+strlen2($4)+2+strlen2($7)+1;$$ = increate(totallength);
	strcpy($$,"#define ");strcat2($$,$2);strcat2($$,"(");strcat2($$,$4);strcat2($$,")(");strcat2($$,$7);strcat($$,")");*/}

Identifier : STRING{$$ = increate(strlen2($1));strcpy($$,$1);}


%%

void print(char* s) {
	printf("%s\n",s);
}
void strcat2(char* a,char* b)
{
	strcat(a," ");
	strcat(a,b);
}
int strlen2(char* a)
{
	return (7 +strlen(a));
}
char* increate(int length)
{
	char* s = (char*)malloc(sizeof(char)*(length+7));
	s[0] = '\0';
	return s;
}
