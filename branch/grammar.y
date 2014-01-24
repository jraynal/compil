%{
	#include "tad/structs.h"
	#include "tad/tree.h"
	#include "tad/list.h"
	
	#include <stdio.h>
	#include "sem_actions.h"
	char *footer();
	char *header();
	extern int yylineno;
	int yylex ();
	int yyerror ();
	struct _layer *my_ctxt;
	int is_first_declarator=1;

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
	char * text;
	struct _attribute *attr;
	struct _list * list;
	struct _attribute*(*unaryOp)(struct _attribute*);
	enum _affectation affect;
	enum _type type;

	// struct _variable * var;
	// void * obj;
}

%type<text> IDENTIFIER 
%type<Int> CONSTANTI
%type<Float> CONSTANTF
%type<attr> primary_expression postfix_expression unary_expression multiplicative_expression additive_expression 
%type<attr> comparison_expression expression parameter_declaration declarator declaration function_definition external_declaration
%type<attr> compound_statement expression_statement selection_statement iteration_statement jump_statement declaration_list statement_list statement 
%type<list> declarator_list argument_expression_list parameter_list
%type<unaryOp> unary_operator
%type<affect> SUB_ASSIGN MUL_ASSIGN ADD_ASSIGN assignment_operator


%type<type> type_name

%%

primary_expression
: IDENTIFIER 										{$$=getVar($1,my_ctxt);}
| CONSTANTI											{$$=newInt($1);}
| CONSTANTF 										{$$=newFloat($1);}
| '(' expression ')'    							{$$=$2;}
| IDENTIFIER '(' ')'								{$$=simpleFuncall(my_ctxt,$1);}
| IDENTIFIER '(' argument_expression_list ')' 		{$$=multipleFuncall(my_ctxt,$1,$3);}
| IDENTIFIER INC_OP 								{$$=varIncr($1,my_ctxt);}
| IDENTIFIER DEC_OP									{$$=varDecr($1,my_ctxt);}
;

postfix_expression
: primary_expression								{$$=$1;}
| postfix_expression '[' expression ']'				{$$=getValArray($1,$3);}
;

argument_expression_list
: expression 										{$$=expression_list($1);}
| argument_expression_list ',' expression			{$$=insert_expr_list($3,$1);}
;

unary_expression
: postfix_expression								{$$=$1;}
| INC_OP unary_expression							{$$=prefixedVarIncr($2);}
| DEC_OP unary_expression							{$$=prefixedVarDecr($2);}
| unary_operator unary_expression   				{$$=$1($2);}
;

unary_operator
: '-' 												{$$=neg;}	
;

multiplicative_expression
: unary_expression                               	{$$=$1;}
| multiplicative_expression '*' unary_expression 	{$$=multiply($1,$3);}
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
: unary_expression assignment_operator comparison_expression {assignment($1,$2,$3);}
| comparison_expression 		{$$=$1;}
;

assignment_operator
: '='        									{$$=AFF;}   
| MUL_ASSIGN 									{$$=MUL;}               
| ADD_ASSIGN 									{$$=ADD;}
| SUB_ASSIGN            			{$$=SUB;}
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
| INT   		{$$=INT_TYPE;}
| FLOAT			{$$=FLOAT_TYPE;}
;

declarator
: IDENTIFIER  									{
													if(is_first_declarator) {
														fprintf(stderr,"at %s line %d\n",__func__,__LINE__);
														my_ctxt=add_layer(my_ctxt);
														is_first_declarator=0;
													}
													$$=declareVar($1,my_ctxt);
												}
| '(' declarator ')'                      		{$$=$2;}
| declarator '[' CONSTANTI ']'             		{$$=declare_array($1,$3);}
| declarator '[' ']'                        	{$$=declare_array($1,0);}
| declarator '(' parameter_list ')'				{$$=multiple_declare_function($1,$3);}
| declarator '(' ')'							{$$=simple_declare_function($1);}
;

parameter_list
: parameter_declaration							{$$=init_list();insertElmnt($1,$$);}
| parameter_list ',' parameter_declaration		{insertElmnt($3,$1);$$=$1;}
;

parameter_declaration
: type_name declarator							{ $$=allocate_id($2,$1); }
;

statement
: compound_statement   {$$=$1;}
| expression_statement {$$=$1;}
| selection_statement  {$$=$1;}
| iteration_statement  {$$=$1;}
| jump_statement       {$$=$1;}
;

compound_statement
: '{' '}'   								{}
| '{' statement_list '}'					{$$=$2;}
| '{' declaration_list statement_list '}'	{$$=concat($2,$3); close_layer(my_ctxt);}
;

declaration_list
: declaration {$$=$1;}
| declaration_list declaration {
								$$=$2;
								is_first_declarator=1; //tout a été init dans le contexte
								}
;

statement_list
: statement 									{$$=new_statement_list($1);}
| statement_list statement 						{$$=insert_statement_list($2,$1);}
;

expression_statement
: ';' 										{}
| expression ';'  							{$$=$1;}
;

selection_statement
: IF '(' expression ')' statement {$$=selection($3,$5,NULL);}
| IF '(' expression ')' statement ELSE statement {$$=selection($3,$5,$7);}
| FOR '(' expression_statement expression_statement expression ')' statement {$$=loop($3,$4,$5,$7);}
;

iteration_statement
: WHILE '(' expression ')' statement {$$=loop(NULL,$3,NULL,$5);}
;

jump_statement
: RETURN ';' {}
| RETURN expression ';' {$$=$2;}
;

program
: external_declaration															{print($1);}
| program external_declaration													{print($2);}
;

external_declaration
: function_definition															{$$=$1;}
| declaration 																	{$$=$1;}
;

function_definition
: type_name declarator compound_statement 										{$$ = make_function($1,$2,$3);}
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

	fprintf(stderr,"at %s line %d\n",__func__,__LINE__);
	my_ctxt = init_layer();
	fprintf(stdout, "%s\n",header() );
	// my_ctxt= add_layer(my_ctxt);
	//fprintf(stderr,"empty: %d\n", is_empty(garbageCollector));
	yyparse ();
	fprintf(stdout, "%s\n",footer() );
	delete_layer(my_ctxt);
	// fprintf(stderr,"true: %d\n", 1==1);
	// fprintf(stderr, "size:%d\n",garbageCollector->size);
	// fprintf(stderr,"empty: %d\n", is_empty(garbageCollector));
	// del_list_and_content(garbageCollector);

	free (file_name);
	fclose(input);
	return 0;
}


char *footer(){
	return "ret void\n}\n\ndeclare float @norm_pi_pi(float %a)\ndeclare float @get_track_angle(%struct.tTrkLocPos*)\ndeclare float @get_pos_to_right(%struct.tTrkLocPos*)\ndeclare float @get_pos_to_middle(%struct.tTrkLocPos*)\ndeclare float @get_pos_to_left(%struct.tTrkLocPos*)\n	declare float @get_pos_to_start(%struct.tTrkLocPos*)\ndeclare float @get_track_seg_length(%struct.trackSeg*)\ndeclare float @get_track_seg_width(%struct.trackSeg*)\ndeclare float @get_track_seg_start_width(%struct.trackSeg*)\ndeclare float @get_track_seg_end_width(%struct.trackSeg*)\ndeclare float @get_track_seg_radius(%struct.trackSeg*)\ndeclare float @get_track_seg_right_radius(%struct.trackSeg*)\n	declare float @get_track_seg_left_radius(%struct.trackSeg*)\ndeclare float @get_track_seg_arc(%struct.trackSeg*)\ndeclare %struct.trackSeg* @get_track_seg_next(%struct.trackSeg*)\ndeclare float @get_car_yaw(%struct.CarElt*)\n";
}
char *header(){
	return
"\ntarget triple = \"x86_64-unknown-linux-gnu\"\n\n%struct.CarElt = type {\ni32,\n%struct.tInitCar,\n%struct.tPublicCar,\n%struct.tCarRaceInfo,\n%struct.tPrivCar,\n%struct.tCarCtrl,\n%struct.tCarPitCmd,\n%struct.RobotItf*,\n%struct.CarElt*\n}\n\n%struct.tInitCar = type {\n[32 x i8],\n[32 x i8],\n[32 x i8],\n[32 x i8],\ni32,\ni32,\ni32,\ni32,\n[3 x float],\n%struct.t3Dd,\n%struct.t3Dd,\n%struct.t3Dd,\nfloat,\nfloat,\n%struct.t3Dd,\n[4 x %struct.tWheelSpec],\n%struct.tVisualAttributes\n}\n\n%struct.t3Dd = type {\nfloat,\nfloat,\nfloat\n}\n\n%struct.tWheelSpec = type {\nfloat,\nfloat,\nfloat,\nfloat,\nfloat\n}\n\n%struct.tVisualAttributes = type {\ni32,\n[2 x %struct.t3Dd],\nfloat\n}\n\n%struct.tPublicCar = type {\n%struct.tDynPt,\n%struct.tDynPt,\n[4 x [4 x float]],\n%struct.tTrkLocPos,\ni32,\n[4 x %struct.tPosd]\n}\n\n%struct.tDynPt = type {\n%struct.tPosd,\n%struct.tPosd,\n%struct.tPosd\n}\n\n%struct.tPosd = type {\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat\n}\n\n%struct.tTrkLocPos = type {\n%struct.trackSeg*,\ni32,\nfloat,\nfloat,\nfloat,\nfloat\n}\n\n%struct.trackSeg = type {\ni8*,\ni32,\ni32,\ni32,\ni32,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\n%struct.t3Dd,\n[4 x %struct.t3Dd],\n[7 x float],\nfloat,\nfloat,\nfloat,\n%struct.t3Dd,\ni32,\nfloat,\ni32,\nfloat,\n%struct.SegExt*,\n%struct.trackSurface*,\n[2 x %struct.trackBarrier*],\n%struct.RoadCam*,\n%struct.trackSeg*,\n%struct.trackSeg*,\n%union.anon.0\n}\n\n%struct.SegExt = type {\ni32,\ni32*\n}\n\n%struct.trackSurface = type {\n%struct.trackSurface*,\ni8*,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat\n}\n\n%struct.trackBarrier = type {\ni32,\nfloat,\nfloat,\n%struct.trackSurface*,\n%class.v2t\n}\n\n%class.v2t = type {\n%union.anon\n}\n\n%union.anon = type {\n%struct.anon\n}\n\n%struct.anon = type {\nfloat,\nfloat\n}\n\n%struct.RoadCam = type {\ni8*,\n%struct.t3Dd,\n%struct.RoadCam*\n}\n\n%union.anon.0 = type {\n%struct.anon.1\n}\n\n%struct.anon.1 = type {\n%struct.trackSeg*,\n%struct.trackSeg*\n}\n\n%struct.tCarRaceInfo = type {\ndouble,\ni8,\ndouble,\ndouble,\ndouble,\ndouble,\nfloat,\nfloat,\ni32,\ni32,\ni32,\ni32,\ndouble,\ni32,\ndouble,\ndouble,\nfloat,\nfloat,\ndouble,\n%struct.TrackOwnPit*,\ni32,\n%struct.CarPenaltyHead\n}\n\n%struct.TrackOwnPit = type {\n%struct.tTrkLocPos,\ni32,\nfloat,\nfloat,\ni32,\n[4 x %struct.CarElt*]\n}\n\n%struct.CarPenaltyHead = type {\n%struct.CarPenalty*,\n%struct.CarPenalty**\n}\n\n%struct.CarPenalty = type {\ni32,\ni32,\n%struct.anon.2\n}\n\n%struct.anon.2 = type {\n%struct.CarPenalty*,\n%struct.CarPenalty**\n}\n\n%struct.tPrivCar = type {\ni8*,\ni8*,\ni32,\n[32 x i8],\n[4 x %struct.tWheelState],\n[4 x %struct.tPosd],\ni32,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\n[10 x float],\ni32,\ni32,\n[4 x float],\n[4 x float],\ni32,\ni32,\nfloat,\n%struct.t3Dd,\n%struct.t3Dd,\ni32,\ni32,\n%struct.tCollisionState_\n}\n\n%struct.tWheelState = type {\n%struct.tPosd,\nfloat,\nfloat,\ni32,\n%struct.trackSeg*,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat,\nfloat\n}\n\n%struct.tCollisionState_ = type {\ni32,\n[3 x float],\n[3 x float]\n}\n\n%struct.tCarCtrl = type {\nfloat,\nfloat,\nfloat,\nfloat,\ni32,\ni32,\n[4 x [32 x i8]],\n[4 x float],\ni32\n}\n\n%struct.tCarPitCmd = type {\nfloat,\ni32,\ni32,\n%struct.tCarPitSetup\n}\n\n%struct.tCarPitSetup = type {\n%struct.tCarPitSetupValue,\n[4 x %struct.tCarPitSetupValue],\n[4 x %struct.tCarPitSetupValue],\n[4 x %struct.tCarPitSetupValue],\n%struct.tCarPitSetupValue,\n%struct.tCarPitSetupValue,\n[4 x %struct.tCarPitSetupValue],\n[4 x %struct.tCarPitSetupValue],\n[4 x %struct.tCarPitSetupValue],\n[4 x %struct.tCarPitSetupValue],\n[4 x %struct.tCarPitSetupValue],\n[4 x %struct.tCarPitSetupValue],\n[2 x %struct.tCarPitSetupValue],\n[2 x %struct.tCarPitSetupValue],\n[2 x %struct.tCarPitSetupValue],\n[2 x %struct.tCarPitSetupValue],\n[2 x %struct.tCarPitSetupValue],\n[8 x %struct.tCarPitSetupValue],\n[2 x %struct.tCarPitSetupValue],\n[3 x %struct.tCarPitSetupValue],\n[3 x %struct.tCarPitSetupValue],\n[3 x %struct.tCarPitSetupValue],\n[3 x %struct.tCarPitSetupValue],\n[3 x %struct.tCarPitSetupValue],\n[3 x %struct.tCarPitSetupValue],\n[3 x i32]\n}\n\n%struct.tCarPitSetupValue = type {\nfloat,\nfloat,\nfloat\n}\n\n%struct.RobotItf = type {\nvoid (i32, %struct.tTrack*, i8*, i8**, %struct.Situation*)*,\n {}*,\n {}*,\n {}*,\n i32 (i32, %struct.CarElt*, %struct.Situation*)*,\n void (i32)*,\n i32\n}\n\n%struct.tTrack = type {\ni8*,\ni8*,\ni8*,\ni8*,\ni8*,\ni8*,\ni32,\ni32,\nfloat,\nfloat,\n%struct.tTrackPitInfo,\n%struct.trackSeg*,\n%struct.trackSurface*,\n%struct.t3Dd,\n%struct.t3Dd,\n%struct.tTrackGraphicInfo\n}\n\n%struct.tTrackPitInfo = type {\ni32,\ni32,\ni32,\ni32,\nfloat,\nfloat,\nfloat,\n%struct.trackSeg*,\n%struct.trackSeg*,\n%struct.trackSeg*,\n%struct.trackSeg*,\n%struct.TrackOwnPit*,\ni32,\ni32\n}\n\n%struct.tTrackGraphicInfo = type {\ni8*,\ni8*,\ni32,\n[3 x float],\ni32,\ni8**,\n%struct.tTurnMarksInfo\n}\n\n%struct.tTurnMarksInfo = type {\nfloat,\nfloat,\nfloat,\nfloat\n}\n\n%struct.Situation = type {\n%struct.tRaceAdmInfo,\ndouble,\ndouble,\ni32,\n%struct.CarElt**\n}\n\n%struct.tRaceAdmInfo = type {\ni32,\ni32,\ni32,\ni32,\ni32,\ni64\n}\n\ndefine void @drive(i32 %index, %struct.CarElt* %car, %struct.Situation* %s) {\n\n%ctrl= getelementptr %struct.CarElt* %car, i32 0, i32 5\n%public_car= getelementptr %struct.CarElt* %car, i32 0, i32 2\n%pos= getelementptr %struct.tPublicCar* %public_car, i32 0, i32 3\n%seg.addr= getelementptr %struct.tTrkLocPos* %pos, i32 0, i32 0\n%seg= load %struct.trackSeg** %seg.addr\n\n%steer= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 0\n%accelCmd= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 1\n%brakeCmd= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 2\n%clutchCmd= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 3\n%gear= getelementptr %struct.tCarCtrl* %ctrl, i32 0, i32 4\n\n%road_angle = call float @get_track_angle(%struct.tTrkLocPos* %pos)\n%car_angle= call float @get_car_yaw(%struct.CarElt* %car)\n%angle= fsub float %road_angle, %car_angle\n%nangle= call float @norm_pi_pi(float %angle)\n\n%posmid= call float @get_pos_to_middle(%struct.tTrkLocPos* %pos)\n%width= call float @get_track_seg_width(%struct.trackSeg* %seg)\n%corr= fdiv float %posmid, %width\n%cangle= fsub float %nangle, %corr\n\n";
}

