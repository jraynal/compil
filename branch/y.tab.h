#define IDENTIFIER 257
#define CONSTANTF 258
#define CONSTANTI 259
#define INC_OP 260
#define DEC_OP 261
#define LE_OP 262
#define GE_OP 263
#define EQ_OP 264
#define NE_OP 265
#define SUB_ASSIGN 266
#define MUL_ASSIGN 267
#define ADD_ASSIGN 268
#define TYPE_NAME 269
#define INT 270
#define FLOAT 271
#define VOID 272
#define IF 273
#define ELSE 274
#define WHILE 275
#define RETURN 276
#define FOR 277
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union{
	int Int;
	float Float;
	char * text;
	struct _attribute *attr;
	struct _list * list;
	struct _attribute*(*unaryOp)(struct _attribute*);
	enum _affectation affect;

	struct _variable * var;
	enum _type type;
	void * obj;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
