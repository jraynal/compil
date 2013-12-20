%{
	#include "includes/structs.h"
	#include "includes/tree.h"
	
	#include <stdio.h>
	#include "sem_actions.h"

	extern int yylineno;
	int yylex ();
	int yyerror ();

	typedef void*(*fPointer)(void*);

%}

%token IDENTIFIER CONSTANTF CONSTANTI
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN
%token TYPE_NAME
%token INT FLOAT VOID
%token IF ELSE WHILE RETURN FOR
%start program

%union{
	int integer;
	void * obj;
	int affect;
	char * text;
	struct _variable *(*unaryOp)(struct _variable *);
	struct _list * list;
	enum _type type;
}

%type<text> IDENTIFIER
%type<list> declarator_list
%type<integer> CONSTANTI
%type<obj>  CONSTANTF  expression 
%type<obj> multiplicative_expression additive_expression comparison_expression unary_expression primary_expression postfix_expression 
%type<affect> SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN assignment_operator
%type<type> type_name 
%type<unaryOp> unary_operator

%%

primary_expression
: IDENTIFIER 									{printf("id lu : %s\n", $1); insertNode(htable,$1);$$=get_node(htable,$1);printf("<fin>\n");}
| CONSTANTI										{$$=varCreateInt($1);}
| CONSTANTF
| '(' expression ')'    						{$$=$2;}
| IDENTIFIER '(' ')'							{ insertNode(htable,$1);$$=get_node(htable,$1);}
| IDENTIFIER '(' argument_expression_list ')' 	{ insertNode(htable,$1);$$=get_node(htable,$1);}
| IDENTIFIER INC_OP 							{ insertNode(htable,$1);$$=get_node(htable,$1);}
| IDENTIFIER DEC_OP								{ insertNode(htable,$1);$$=get_node(htable,$1);}
;

postfix_expression
: primary_expression
| postfix_expression '[' expression ']'
;

argument_expression_list
: expression 				
| argument_expression_list ',' expression
;

unary_expression
: postfix_expression				{$$=$1;}
| INC_OP unary_expression			{$$=incr($2);}
| DEC_OP unary_expression			{$$=decr($2);}
| unary_operator unary_expression   {$$=$1($2);}
;

unary_operator
: '-' 								{$$=neg;}	
;

multiplicative_expression
: unary_expression                                          {$$=$1;}
| multiplicative_expression '*' unary_expression            {$$=mul($1,$3);}
| multiplicative_expression '/' unary_expression            {$$=divide($1,$3);}//div is already in lib std
;

additive_expression
: multiplicative_expression                                 {$$=$1;}
| additive_expression '+' multiplicative_expression         {$$=add($1,$3);}
| additive_expression '-' multiplicative_expression         {$$=sub($1,$3);}
;

comparison_expression
: additive_expression                                       {$$=$1;}
| additive_expression '<' additive_expression               {$$=l_op($1,$3);}
| additive_expression '>' additive_expression               {$$=g_op($1,$3);}
| additive_expression LE_OP additive_expression             {$$=le_op($1,$3);}
| additive_expression GE_OP additive_expression             {$$=ge_op($1,$3);}
| additive_expression EQ_OP additive_expression             {$$=eq_op($1,$3);}
| additive_expression NE_OP additive_expression             {$$=ne_op($1,$3);}
;

expression
// on suppose que $1 est uniquement un identifiant
: unary_expression assignment_operator comparison_expression {affectValue($1,$2,$3);}
| comparison_expression
;

assignment_operator
: '='        			{$$=0;}   
| MUL_ASSIGN 			{$$=1;}               
| ADD_ASSIGN 			{$$=2;}
| SUB_ASSIGN            {$$=3;}
;

declaration
: type_name declarator_list ';' {setType($2,$1);}
;

declarator_list
: declarator 					{$$=createList();/*addElmtList()*/}
| declarator_list ',' declarator 	{/*addElmtList()*/}
;

type_name
: VOID 			{$$=VOID_TYPE;}
| INT   		{$$=1;}//{union _value val; $$=varCreate(INT_TYPE,val);}
| FLOAT			{$$=2;}//{union _value val; $$=varCreate(FLOAT_TYPE,val);}
;

declarator
: IDENTIFIER  
| '(' declarator ')'                      
| declarator '[' CONSTANTI ']'             
| declarator '[' ']'                        
| declarator '(' parameter_list ')'
| declarator '(' ')'
;

parameter_list
: parameter_declaration
| parameter_list ',' parameter_declaration
;

parameter_declaration
: type_name declarator
;

statement
: compound_statement
| expression_statement 
| selection_statement
| iteration_statement
| jump_statement
;

compound_statement
: '{' '}'   								{htable=init_tree();}
| '{' statement_list '}'					{htable=init_tree();}
| '{' declaration_list statement_list '}'	{htable=init_tree();}
;

declaration_list
: declaration
| declaration_list declaration
;

statement_list
: statement
| statement_list statement
;

expression_statement
: ';'
| expression ';' 
;

selection_statement
: IF '(' expression ')' statement
| IF '(' expression ')' statement ELSE statement
| FOR '(' expression_statement expression_statement expression ')' statement
;

iteration_statement
: WHILE '(' expression ')' statement
;

jump_statement
: RETURN ';'
| RETURN expression ';'
;

program
: external_declaration
| program external_declaration
;

external_declaration
: function_definition
| declaration
;

function_definition
: type_name declarator compound_statement
;

%%
#include <stdio.h>
#include <string.h>

extern int column;
extern int yylineno;
extern FILE *yyin;

char *file_name = NULL;

int yyerror (char *s) {
	fflush (stdout);
	fprintf (stderr, "%s:%d:%d: %s\n", file_name, yylineno, column, s);
	return 0;
}


int main (int argc, char *argv[]) {
	FILE *input = NULL;
	if (argc==2) {
	input = fopen (argv[1], "r");
	file_name = strdup (argv[1]);
	if (input) {
		yyin = input;
	}
	else {
	  fprintf (stderr, "%s: Could not open %s\n", *argv, argv[1]);
		return 1;
	}
	}
	else {
	fprintf (stderr, "%s: error: no input file\n", *argv);
	return 1;
	}
	yyparse ();
	free (file_name);
	return 0;
}
