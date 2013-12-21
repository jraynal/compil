%{
	#include "includes/structs.h"
	#include "includes/tree.h"
	
	#include <stdio.h>
	#include "sem_actions.h"
	char *footer();
	char *header();
	extern int yylineno;
	int yylex ();
	int yyerror ();

	int new_reg(){
	static i =0;
	i++;
	return i;
}

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
	float Float;
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
%type<Float> CONSTANTF
%type<obj>    expression 
%type<obj> multiplicative_expression additive_expression comparison_expression unary_expression primary_expression postfix_expression 
%type<affect> SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN assignment_operator
%type<type> type_name 
%type<unaryOp> unary_operator

%%

primary_expression
: IDENTIFIER 									{ insertNode(htable,$1);$$=get_node(htable,$1);}
| CONSTANTI										{$$=varCreateInt($1);}
| CONSTANTF 									{$$=varCreateFloat($1);}
| '(' expression ')'    						{$$=$2;}
| IDENTIFIER '(' ')'							{printf("fct_id lu %s\n",$1 ); insertNode(htable,$1);$$=get_node(htable,$1);}
| IDENTIFIER '(' argument_expression_list ')' 	{ insertNode(htable,$1);$$=get_node(htable,$1);}
| IDENTIFIER INC_OP 							{ insertNode(htable,$1);$$=get_node(htable,$1);}
| IDENTIFIER DEC_OP								{ insertNode(htable,$1);$$=get_node(htable,$1);}
;

postfix_expression
: primary_expression							{$$=$1}
| postfix_expression '[' expression ']'			{}
;

argument_expression_list
: expression 									{}
| argument_expression_list ',' expression		{}
;

unary_expression
: postfix_expression				{$$=$1;}
| INC_OP unary_expression			{$$=incr($2);}
| DEC_OP unary_expression			{$$=decr($2);}
| unary_operator unary_expression   {$$=$1($2);}
;

unary_operator
: '-' 											{$$=neg;}	
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
: '='        												{$$=0;}   
| MUL_ASSIGN 												{$$=1;}               
| ADD_ASSIGN 												{$$=2;}
| SUB_ASSIGN            									{$$=3;}
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
| INT   		{$$=INT_TYPE;}//{union _value val; $$=varCreate(INT_TYPE,val);}
| FLOAT			{$$=FLOAT_TYPE;}//{union _value val; $$=varCreate(FLOAT_TYPE,val);}
;

declarator
: IDENTIFIER  												{add_identifier();}
| '(' declarator ')'                      					{}
| declarator '[' CONSTANTI ']'             					{}
| declarator '[' ']'                        				{}
| declarator '(' parameter_list ')'							{}
| declarator '(' ')'										{}
;

parameter_list
: parameter_declaration										{}
| parameter_list ',' parameter_declaration					{}
;

parameter_declaration
: type_name declarator										{}
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
	htable=init_tree();
	yyparse ();
	free (file_name);
	return 0;
}


char *footer(){
	return "
			ret void
		}
		
		declare float @norm_pi_pi(float %a)
		declare float @get_track_angle(%struct.tTrkLocPos*)
		declare float @get_pos_to_right(%struct.tTrkLocPos*)
		declare float @get_pos_to_middle(%struct.tTrkLocPos*)
		declare float @get_pos_to_left(%struct.tTrkLocPos*)
		declare float @get_pos_to_start(%struct.tTrkLocPos*)
		declare float @get_track_seg_length(%struct.trackSeg*)
		declare float @get_track_seg_width(%struct.trackSeg*)
		declare float @get_track_seg_start_width(%struct.trackSeg*)
		declare float @get_track_seg_end_width(%struct.trackSeg*)
		declare float @get_track_seg_radius(%struct.trackSeg*)
		declare float @get_track_seg_right_radius(%struct.trackSeg*)
		declare float @get_track_seg_left_radius(%struct.trackSeg*)
		declare float @get_track_seg_arc(%struct.trackSeg*)
		declare %struct.trackSeg* @get_track_seg_next(%struct.trackSeg*)
		declare float @get_car_yaw(%struct.CarElt*)
	";
}
char *header(){
	return
	"
			target triple = "x86_64-unknown-linux-gnu"
		
		%struct.CarElt = type {
			i32,
			%struct.tInitCar,
			%struct.tPublicCar,
			%struct.tCarRaceInfo,
			%struct.tPrivCar,
			%struct.tCarCtrl,
			%struct.tCarPitCmd,
			%struct.RobotItf*,
			%struct.CarElt*
		}
		
		%struct.tInitCar = type {
			[32 x i8],
			[32 x i8],
			[32 x i8],
			[32 x i8],
			i32,
			i32,
			i32,
			i32,
			[3 x float],
			%struct.t3Dd,
			%struct.t3Dd,
			%struct.t3Dd,
			float,
			float,
			%struct.t3Dd,
			[4 x %struct.tWheelSpec],
			%struct.tVisualAttributes
		}
		
		%struct.t3Dd = type {
			float,
			float,
			float
		}
		
		%struct.tWheelSpec = type {
			float,
			float,
			float,
			float,
			float
		}
		
		%struct.tVisualAttributes = type {
			i32,
			[2 x %struct.t3Dd],
			float
		}
		
		%struct.tPublicCar = type {
			%struct.tDynPt,
			%struct.tDynPt,
			[4 x [4 x float]],
			%struct.tTrkLocPos,
			i32,
			[4 x %struct.tPosd]
		}
		
		%struct.tDynPt = type {
			%struct.tPosd,
			%struct.tPosd,
			%struct.tPosd
		}
		
		%struct.tPosd = type {
			float,
			float,
			float,
			float,
			float,
			float
		}
		
		%struct.tTrkLocPos = type {
			%struct.trackSeg*,
			i32,
			float,
			float,
			float,
			float
		}
		
		%struct.trackSeg = type {
			i8*,
			i32,
			i32,
			i32,
			i32,
			float,
			float,
			float,
			float,
			float,
			float,
			float,
			float,
			float,
			%struct.t3Dd,
			[4 x %struct.t3Dd],
			[7 x float],
			float,
			float,
			float,
			%struct.t3Dd,
			i32,
			float,
			i32,
			float,
			%struct.SegExt*,
			%struct.trackSurface*,
			[2 x %struct.trackBarrier*],
			%struct.RoadCam*,
			%struct.trackSeg*,
			%struct.trackSeg*,
			%union.anon.0
		}
		
		%struct.SegExt = type {
			i32,
			i32*
		}
		
		%struct.trackSurface = type {
			%struct.trackSurface*,
			i8*,
			float,
			float,
			float,
			float,
			float,
			float
		}
		
		%struct.trackBarrier = type {
			i32,
			float,
			float,
			%struct.trackSurface*,
			%class.v2t
		}
		
		%class.v2t = type {
			%union.anon
		}
		
		%union.anon = type {
			%struct.anon
		}
		
		%struct.anon = type {
			float,
			float
		}
		
		%struct.RoadCam = type {
			i8*,
			%struct.t3Dd,
			%struct.RoadCam*
		}
		
		%union.anon.0 = type {
			%struct.anon.1
		}
		
		%struct.anon.1 = type {
			%struct.trackSeg*,
			%struct.trackSeg*
		}
		
		%struct.tCarRaceInfo = type {
			double,
			i8,
			double,
			double,
			double,
			double,
			float,
			float,
			i32,
			i32,
			i32,
			i32,
			double,
			i32,
			double,
			double,
			float,
			float,
			double,
			%struct.TrackOwnPit*,
			i32,
			%struct.CarPenaltyHead
		}
		
		%struct.TrackOwnPit = type {
			%struct.tTrkLocPos,
			i32,
			float,
			float,
			i32,
			[4 x %struct.CarElt*]
		}
		
		%struct.CarPenaltyHead = type {
			%struct.CarPenalty*,
			%struct.CarPenalty**
		}
		
		%struct.CarPenalty = type {
			i32,
			i32,
			%struct.anon.2
		}
		
		%struct.anon.2 = type {
			%struct.CarPenalty*,
			%struct.CarPenalty**
		}
		
		%struct.tPrivCar = type {
			i8*,
			i8*,
			i32,
			[32 x i8],
			[4 x %struct.tWheelState],
			[4 x %struct.tPosd],
			i32,
			float,
			float,
			float,
			float,
			float,
			float,
			float,
			float,
			[10 x float],
			i32,
			i32,
			[4 x float],
			[4 x float],
			i32,
			i32,
			float,
			%struct.t3Dd,
			%struct.t3Dd,
			i32,
			i32,
			%struct.tCollisionState_
		}
		
		%struct.tWheelState = type {
			%struct.tPosd,
			float,
			float,
			i32,
			%struct.trackSeg*,
			float,
			float,
			float,
			float,
			float,
			float,
			float,
			float,
			float,
			float
		}
		
		%struct.tCollisionState_ = type {
			i32,
			[3 x float],
			[3 x float]
		}
		
		%struct.tCarCtrl = type {
			float,
			float,
			float,
			float,
			i32,
			i32,
			[4 x [32 x i8]],
			[4 x float],
			i32
		}
		
		%struct.tCarPitCmd = type {
			float,
			i32,
			i32,
			%struct.tCarPitSetup
		}
		
		%struct.tCarPitSetup = type {
			%struct.tCarPitSetupValue,
			[4 x %struct.tCarPitSetupValue],
			[4 x %struct.tCarPitSetupValue],
			[4 x %struct.tCarPitSetupValue],
			%struct.tCarPitSetupValue,
			%struct.tCarPitSetupValue,
			[4 x %struct.tCarPitSetupValue],
			[4 x %struct.tCarPitSetupValue],
			[4 x %struct.tCarPitSetupValue],
			[4 x %struct.tCarPitSetupValue],
			[4 x %struct.tCarPitSetupValue],
			[4 x %struct.tCarPitSetupValue],
			[2 x %struct.tCarPitSetupValue],
			[2 x %struct.tCarPitSetupValue],
			[2 x %struct.tCarPitSetupValue],
			[2 x %struct.tCarPitSetupValue],
			[2 x %struct.tCarPitSetupValue],
			[8 x %struct.tCarPitSetupValue],
			[2 x %struct.tCarPitSetupValue],
			[3 x %struct.tCarPitSetupValue],
			[3 x %struct.tCarPitSetupValue],
			[3 x %struct.tCarPitSetupValue],
			[3 x %struct.tCarPitSetupValue],
			[3 x %struct.tCarPitSetupValue],
			[3 x %struct.tCarPitSetupValue],
			[3 x i32]
		}
		
		%struct.tCarPitSetupValue = type {
			float,
			float,
			float
		}
		
		%struct.RobotItf = type {
			void (i32, %struct.tTrack*, i8*, i8**, %struct.Situation*)*,
			 {}*,
			 {}*,
			 {}*,
			 i32 (i32, %struct.CarElt*, %struct.Situation*)*,
			 void (i32)*,
			 i32
		}
		
		%struct.tTrack = type {
			i8*,
			i8*,
			i8*,
			i8*,
			i8*,
			i8*,
			i32,
			i32,
			float,
			float,
			%struct.tTrackPitInfo,
			%struct.trackSeg*,
			%struct.trackSurface*,
			%struct.t3Dd,
			%struct.t3Dd,
			%struct.tTrackGraphicInfo
		}
		
		%struct.tTrackPitInfo = type {
			i32,
			i32,
			i32,
			i32,
			float,
			float,
			float,
			%struct.trackSeg*,
			%struct.trackSeg*,
			%struct.trackSeg*,
			%struct.trackSeg*,
			%struct.TrackOwnPit*,
			i32,
			i32
		}
		
		%struct.tTrackGraphicInfo = type {
			i8*,
			i8*,
			i32,
			[3 x float],
			i32,
			i8**,
			%struct.tTurnMarksInfo
		}
		
		%struct.tTurnMarksInfo = type {
			float,
			float,
			float,
			float
		}
		
		%struct.Situation = type {
			%struct.tRaceAdmInfo,
			double,
			double,
			i32,
			%struct.CarElt**
		}
		
		%struct.tRaceAdmInfo = type {
			i32,
			i32,
			i32,
			i32,
			i32,
			i64
		}
		
		define void @drive(i32 %index, %struct.CarElt* %car, %struct.Situation* %s) {
	";
}

