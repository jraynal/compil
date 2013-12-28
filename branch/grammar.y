%{
	#include "includes/structs.h"
	#include "includes/tree.h"
	#include "includes/list.h"
	
	#include <stdio.h>
	#include "sem_actions.h"
	char *footer();
	char *header();
	extern int yylineno;
	int yylex ();
	int yyerror ();



%}

%token IDENTIFIER CONSTANTF CONSTANTI
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP
%token SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN
%token TYPE_NAME
%token INT FLOAT VOID
%token IF ELSE WHILE RETURN FOR
%start program

%union{
	int Int;
	float Float;
	int affect;
	char * text;
	struct _variable *(*unaryOp)(struct _variable *);
	struct _list * list;
	struct _variable * var;
	enum _type type;
	void * obj;
}

%type<text> IDENTIFIER
%type<Int> CONSTANTI
%type<Float> CONSTANTF
%type<list> declarator_list
%type<var> declarator
%type<obj> expression
%type<obj> multiplicative_expression additive_expression comparison_expression unary_expression primary_expression postfix_expression
%type<affect> SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN assignment_operator
%type<type> type_name
%type<unaryOp> unary_operator

%%

primary_expression
: IDENTIFIER 										{$$=getVar($1,htable);if($$==NULL) fprintf(stderr, "undefined value %s\n",$1 );free($1);}
| CONSTANTI											{$$=varCreateInt($1);}
| CONSTANTF 										{$$=varCreateFloat($1);}
| '(' expression ')'    							{$$=$2;}
| IDENTIFIER '(' ')'								{ $$=NULL;}
| IDENTIFIER '(' argument_expression_list ')' 		{ $$=NULL;}
| IDENTIFIER INC_OP 								{ $$=NULL;}
| IDENTIFIER DEC_OP									{ $$=NULL;}
;

postfix_expression
: primary_expression								{$$=$1;}
| postfix_expression '[' expression ']'				{}
;

argument_expression_list
: expression 										{}
| argument_expression_list ',' expression			{}
;

unary_expression
: postfix_expression								{$$=$1;}
| INC_OP unary_expression							{$$=incr($2);}
| DEC_OP unary_expression							{$$=decr($2);}
| unary_operator unary_expression   				{$$=$1($2);}
;

unary_operator
: '-' 											{$$=neg;}	
;

multiplicative_expression
: unary_expression                               	{$$=$1;}
| multiplicative_expression '*' unary_expression 	{$$=mul($1,$3);}
| multiplicative_expression '/' unary_expression 	{$$=divide($1,$3);}//div is already in lib std
;

additive_expression
: multiplicative_expression                        	{$$=$1;}
| additive_expression '+' multiplicative_expression	{$$=add($1,$3);}
| additive_expression '-' multiplicative_expression	{$$=sub($1,$3);}
;

comparison_expression
: additive_expression                           	{$$=$1;}
| additive_expression '<' additive_expression   	{$$=l_op($1,$3);}
| additive_expression '>' additive_expression   	{$$=g_op($1,$3);}
| additive_expression LE_OP additive_expression 	{$$=le_op($1,$3);}
| additive_expression GE_OP additive_expression 	{$$=ge_op($1,$3);}
| additive_expression EQ_OP additive_expression 	{$$=eq_op($1,$3);}
| additive_expression NE_OP additive_expression 	{$$=ne_op($1,$3);}
;

expression
// on suppose que $1 est uniquement un identifiant
: unary_expression assignment_operator comparison_expression {affectValue($1,$2,$3);}
| comparison_expression
;

assignment_operator
: '='        									{$$=0;}   
| MUL_ASSIGN 									{$$=1;}               
| ADD_ASSIGN 									{$$=2;}
| SUB_ASSIGN            						{$$=3;}
;

declaration
: type_name declarator_list ';' 				{setTypeList($2,$1);}
;

declarator_list
: declarator 									{$$=init_list();insertElmnt($1,$$);}
| declarator_list ',' declarator 				{insertElmnt($3,$1);$$=$1;}
;

type_name
: VOID 			{$$=VOID_TYPE;}
| INT   		{$$=INT_TYPE;}//{union _value val; $$=varCreate(INT_TYPE,val);}
| FLOAT			{$$=FLOAT_TYPE;}//{union _value val; $$=varCreate(FLOAT_TYPE,val);}
;

declarator
: IDENTIFIER  									{$$=declareVar($1,htable);if(!$$)printf("No return\n");}
| '(' declarator ')'                      		{$$=NULL;}
| declarator '[' CONSTANTI ']'             		{$$=NULL;}
| declarator '[' ']'                        	{$$=NULL;}
| declarator '(' parameter_list ')'				{$$=$1;}
| declarator '(' ')'							{$$=$1;fprintf(stderr, "function detected : %s \n",((struct _variable*)$1)->name );}
;

parameter_list
: parameter_declaration							{}
| parameter_list ',' parameter_declaration		{}
;

parameter_declaration
: type_name declarator							{}
;

statement
: compound_statement
| expression_statement 
| selection_statement
| iteration_statement
| jump_statement
;

compound_statement
: '{' '}'   								//{htable=init_tree();}
| '{' statement_list '}'					//{htable=init_tree();}
| '{' declaration_list statement_list '}'	//{htable=init_tree();}
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
: type_name declarator compound_statement {setType($2,$1);fprintf(stderr,"end of %s (type=%d)\n",((struct _variable*)$2)->name,(int)$1);}
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
	garbageCollector = init_list();
	fprintf(stdout, "%s\n",header() );
	yyparse ();
	fprintf(stdout, "%s\n",footer() );
	
	del_tree(htable);
	del_list_and_content(garbageCollector);
	free (file_name);
	fclose(input);
	return 0;
}


char *footer(){
	return "ret void\n		}\n		\n		declare float @norm_pi_pi(float %a)\n		declare float @get_track_angle(%struct.tTrkLocPos*)\n		declare float @get_pos_to_right(%struct.tTrkLocPos*)\n		declare float @get_pos_to_middle(%struct.tTrkLocPos*)\n		declare float @get_pos_to_left(%struct.tTrkLocPos*)\n		declare float @get_pos_to_start(%struct.tTrkLocPos*)\n		declare float @get_track_seg_length(%struct.trackSeg*)\n		declare float @get_track_seg_width(%struct.trackSeg*)\n		declare float @get_track_seg_start_width(%struct.trackSeg*)\n		declare float @get_track_seg_end_width(%struct.trackSeg*)\n		declare float @get_track_seg_radius(%struct.trackSeg*)\n		declare float @get_track_seg_right_radius(%struct.trackSeg*)\n		declare float @get_track_seg_left_radius(%struct.trackSeg*)\n		declare float @get_track_seg_arc(%struct.trackSeg*)\n		declare %struct.trackSeg* @get_track_seg_next(%struct.trackSeg*)\n		declare float @get_car_yaw(%struct.CarElt*)\n	";
}
char *header(){
	return
	"\n			target triple = \"x86_64-unknown-linux-gnu\"\n		\n		%struct.CarElt = type {\n			i32,\n			%struct.tInitCar,\n			%struct.tPublicCar,\n			%struct.tCarRaceInfo,\n			%struct.tPrivCar,\n			%struct.tCarCtrl,\n			%struct.tCarPitCmd,\n			%struct.RobotItf*,\n			%struct.CarElt*\n		}\n		\n		%struct.tInitCar = type {\n			[32 x i8],\n			[32 x i8],\n			[32 x i8],\n			[32 x i8],\n			i32,\n			i32,\n			i32,\n			i32,\n			[3 x float],\n			%struct.t3Dd,\n			%struct.t3Dd,\n			%struct.t3Dd,\n			float,\n			float,\n			%struct.t3Dd,\n			[4 x %struct.tWheelSpec],\n			%struct.tVisualAttributes\n		}\n		\n		%struct.t3Dd = type {\n			float,\n			float,\n			float\n		}\n		\n		%struct.tWheelSpec = type {\n			float,\n			float,\n			float,\n			float,\n			float\n		}\n		\n		%struct.tVisualAttributes = type {\n			i32,\n			[2 x %struct.t3Dd],\n			float\n		}\n		\n		%struct.tPublicCar = type {\n			%struct.tDynPt,\n			%struct.tDynPt,\n			[4 x [4 x float]],\n			%struct.tTrkLocPos,\n			i32,\n			[4 x %struct.tPosd]\n		}\n		\n		%struct.tDynPt = type {\n			%struct.tPosd,\n			%struct.tPosd,\n			%struct.tPosd\n		}\n		\n		%struct.tPosd = type {\n			float,\n			float,\n			float,\n			float,\n			float,\n			float\n		}\n		\n		%struct.tTrkLocPos = type {\n			%struct.trackSeg*,\n			i32,\n			float,\n			float,\n			float,\n			float\n		}\n		\n		%struct.trackSeg = type {\n			i8*,\n			i32,\n			i32,\n			i32,\n			i32,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			%struct.t3Dd,\n			[4 x %struct.t3Dd],\n			[7 x float],\n			float,\n			float,\n			float,\n			%struct.t3Dd,\n			i32,\n			float,\n			i32,\n			float,\n			%struct.SegExt*,\n			%struct.trackSurface*,\n			[2 x %struct.trackBarrier*],\n			%struct.RoadCam*,\n			%struct.trackSeg*,\n			%struct.trackSeg*,\n			%union.anon.0\n		}\n		\n		%struct.SegExt = type {\n			i32,\n			i32*\n		}\n		\n		%struct.trackSurface = type {\n			%struct.trackSurface*,\n			i8*,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float\n		}\n		\n		%struct.trackBarrier = type {\n			i32,\n			float,\n			float,\n			%struct.trackSurface*,\n			%class.v2t\n		}\n		\n		%class.v2t = type {\n			%union.anon\n		}\n		\n		%union.anon = type {\n			%struct.anon\n		}\n		\n		%struct.anon = type {\n			float,\n			float\n		}\n		\n		%struct.RoadCam = type {\n			i8*,\n			%struct.t3Dd,\n			%struct.RoadCam*\n		}\n		\n		%union.anon.0 = type {\n			%struct.anon.1\n		}\n		\n		%struct.anon.1 = type {\n			%struct.trackSeg*,\n			%struct.trackSeg*\n		}\n		\n		%struct.tCarRaceInfo = type {\n			double,\n			i8,\n			double,\n			double,\n			double,\n			double,\n			float,\n			float,\n			i32,\n			i32,\n			i32,\n			i32,\n			double,\n			i32,\n			double,\n			double,\n			float,\n			float,\n			double,\n			%struct.TrackOwnPit*,\n			i32,\n			%struct.CarPenaltyHead\n		}\n		\n		%struct.TrackOwnPit = type {\n			%struct.tTrkLocPos,\n			i32,\n			float,\n			float,\n			i32,\n			[4 x %struct.CarElt*]\n		}\n		\n		%struct.CarPenaltyHead = type {\n			%struct.CarPenalty*,\n			%struct.CarPenalty**\n		}\n		\n		%struct.CarPenalty = type {\n			i32,\n			i32,\n			%struct.anon.2\n		}\n		\n		%struct.anon.2 = type {\n			%struct.CarPenalty*,\n			%struct.CarPenalty**\n		}\n		\n		%struct.tPrivCar = type {\n			i8*,\n			i8*,\n			i32,\n			[32 x i8],\n			[4 x %struct.tWheelState],\n			[4 x %struct.tPosd],\n			i32,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			[10 x float],\n			i32,\n			i32,\n			[4 x float],\n			[4 x float],\n			i32,\n			i32,\n			float,\n			%struct.t3Dd,\n			%struct.t3Dd,\n			i32,\n			i32,\n			%struct.tCollisionState_\n		}\n		\n		%struct.tWheelState = type {\n			%struct.tPosd,\n			float,\n			float,\n			i32,\n			%struct.trackSeg*,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float,\n			float\n		}\n		\n		%struct.tCollisionState_ = type {\n			i32,\n			[3 x float],\n			[3 x float]\n		}\n		\n		%struct.tCarCtrl = type {\n			float,\n			float,\n			float,\n			float,\n			i32,\n			i32,\n			[4 x [32 x i8]],\n			[4 x float],\n			i32\n		}\n		\n		%struct.tCarPitCmd = type {\n			float,\n			i32,\n			i32,\n			%struct.tCarPitSetup\n		}\n		\n		%struct.tCarPitSetup = type {\n			%struct.tCarPitSetupValue,\n			[4 x %struct.tCarPitSetupValue],\n			[4 x %struct.tCarPitSetupValue],\n			[4 x %struct.tCarPitSetupValue],\n			%struct.tCarPitSetupValue,\n			%struct.tCarPitSetupValue,\n			[4 x %struct.tCarPitSetupValue],\n			[4 x %struct.tCarPitSetupValue],\n			[4 x %struct.tCarPitSetupValue],\n			[4 x %struct.tCarPitSetupValue],\n			[4 x %struct.tCarPitSetupValue],\n			[4 x %struct.tCarPitSetupValue],\n			[2 x %struct.tCarPitSetupValue],\n			[2 x %struct.tCarPitSetupValue],\n			[2 x %struct.tCarPitSetupValue],\n			[2 x %struct.tCarPitSetupValue],\n			[2 x %struct.tCarPitSetupValue],\n			[8 x %struct.tCarPitSetupValue],\n			[2 x %struct.tCarPitSetupValue],\n			[3 x %struct.tCarPitSetupValue],\n			[3 x %struct.tCarPitSetupValue],\n			[3 x %struct.tCarPitSetupValue],\n			[3 x %struct.tCarPitSetupValue],\n			[3 x %struct.tCarPitSetupValue],\n			[3 x %struct.tCarPitSetupValue],\n			[3 x i32]\n		}\n		\n		%struct.tCarPitSetupValue = type {\n			float,\n			float,\n			float\n		}\n		\n		%struct.RobotItf = type {\n			void (i32, %struct.tTrack*, i8*, i8**, %struct.Situation*)*,\n			 {}*,\n			 {}*,\n			 {}*,\n			 i32 (i32, %struct.CarElt*, %struct.Situation*)*,\n			 void (i32)*,\n			 i32\n		}\n		\n		%struct.tTrack = type {\n			i8*,\n			i8*,\n			i8*,\n			i8*,\n			i8*,\n			i8*,\n			i32,\n			i32,\n			float,\n			float,\n			%struct.tTrackPitInfo,\n			%struct.trackSeg*,\n			%struct.trackSurface*,\n			%struct.t3Dd,\n			%struct.t3Dd,\n			%struct.tTrackGraphicInfo\n		}\n		\n		%struct.tTrackPitInfo = type {\n			i32,\n			i32,\n			i32,\n			i32,\n			float,\n			float,\n			float,\n			%struct.trackSeg*,\n			%struct.trackSeg*,\n			%struct.trackSeg*,\n			%struct.trackSeg*,\n			%struct.TrackOwnPit*,\n			i32,\n			i32\n		}\n		\n		%struct.tTrackGraphicInfo = type {\n			i8*,\n			i8*,\n			i32,\n			[3 x float],\n			i32,\n			i8**,\n			%struct.tTurnMarksInfo\n		}\n		\n		%struct.tTurnMarksInfo = type {\n			float,\n			float,\n			float,\n			float\n		}\n		\n		%struct.Situation = type {\n			%struct.tRaceAdmInfo,\n			double,\n			double,\n			i32,\n			%struct.CarElt**\n		}\n		\n		%struct.tRaceAdmInfo = type {\n			i32,\n			i32,\n			i32,\n			i32,\n			i32,\n			i64\n		}\n		\n		define void @drive(i32 %index, %struct.CarElt* %car, %struct.Situation* %s) {\n	\n	%ctrl		= getelementptr %struct.CarElt* %car, i32 0, i32 5\n	%public_car	= getelementptr %struct.CarElt* %car, i32 0, i32 2\n	%pos		= getelementptr %struct.tPublicCar* %public_car, i32 0, i32 3\n	%seg.addr	= getelementptr %struct.tTrkLocPos* %pos, i32 0, i32 0\n	%seg		= load %struct.trackSeg** %seg.addr\n\n	%steer		= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 0\n	%accelCmd	= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 1\n	%brakeCmd	= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 2\n	%clutchCmd	= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 3\n	%gear		= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 4\n\n	%road_angle = call float @get_track_angle(%struct.tTrkLocPos* %pos)\n	%car_angle	= call float @get_car_yaw(%struct.CarElt* %car)\n	%angle		= fsub float %road_angle, %car_angle\n	%nangle		= call float @norm_pi_pi(float %angle)\n\n	%posmid		= call float @get_pos_to_middle(%struct.tTrkLocPos* %pos)\n	%width		= call float @get_track_seg_width(%struct.trackSeg* %seg)\n	%corr		= fdiv float %posmid, %width\n	%cangle		= fsub float %nangle, %corr\n\n";
}

