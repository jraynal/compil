#include "code.h"
#include <stdio.h>
#define CHK(message,truc) do{if(truc == NULL){fprintf(stderr,#message" in "#truc" at %s in %s line %d\n",__FILE__,__func__,__LINE__);exit(EXIT_FAILURE);}}while(0)

#ifndef DEBUG
#define DB(say)
#else
#define DB(say) fprintf(stderr,#say"\n");
#endif


/* Private */
static int isEnd(struct _code* c, struct _string *str);
static int addTail(struct _code *c, struct _string *str );
static struct _string *getNext(struct _string *str);
static struct _string *initString(char *txt);
static void deleteString(struct _string *str);
static int isEmpty(struct _code* c);
/* Public */
struct _code *fusionCode(struct _code* c1, struct _code* c2) {
	CHK(invalid arg,c1);
	CHK(invalid arg,c2);
	struct _code *cmaster=initCode();
	CHK(initCode,cmaster);
	
	if(!isEmpty(c1) && ! isEmpty(c2)){
		cmaster->begin=c1->begin;
		CHK(laststring,c1->end);
		c1->end->next = c2->begin;
		cmaster->end = c2->end;
	}
	else if (!isEmpty(c1)){
		cmaster->begin=c1->begin;
		cmaster->end = c1->end;
	}
	else{
		cmaster->begin=c2->begin;
		cmaster->end = c2->end;
	}
	free(c1);
	free(c2);
	return cmaster;
}

struct _code *addCode(struct _code* code, char* str,...) {
	char *tmp=NULL;
	int len=0;
	/* formatage de la chaine */
	va_list argp;
	va_start(argp, str);
	len = snprintf(NULL, 0, str, argp);
	// fprintf(stderr,"(%d octets)\n",len);
	CHK(bad alloc,(tmp = malloc((len+2) * sizeof(char))));
	
	len = vsnprintf(tmp, len+2, str, argp);
	printf("%s\n",tmp );
	va_end(argp);

	/* Ajout dans le code */
	addTail(code,initString(tmp));
	return code;
}

int printCode(int fd, struct _code *code) {
	CHK(is NULL,code);
	struct _string * tmp = code->begin;
	int written=0,r=0;
	while(tmp!=NULL) {
		written=0; r=0;
		CHK(is NULL, tmp);
		while(written < tmp->length) {
			if((r=write(fd,tmp->text,tmp->length-written))==-1)
				return EXIT_FAILURE;
			written+=r;
		}
		tmp=getNext(tmp);
	}
	return EXIT_SUCCESS;
}

struct _code *initCode() {
	struct _code *code=malloc(sizeof(struct _code));
	code->begin=NULL;
	code->end=NULL;
	return code;
}

void deleteCode(struct _code* code) {
	struct _string *tmp = code->begin;
	struct _string *tmp2;
	while(tmp){
		tmp2=getNext(tmp);
		deleteString(tmp);
		tmp=tmp2;
	}
	free(code);
}

/* Private */
static int addTail(struct _code *c, struct _string *str ) {
	if(!c||!str)
		return EXIT_FAILURE;
	else if (isEmpty(c)) {
		c->begin = str;
		c->end = str;
	}
	else {
		c->end->next=str;
		c->end=str;
	}
	return EXIT_SUCCESS;
}

static struct _string *getNext(struct _string *str) {
	if(str)
		return str->next;
	return NULL;
}

static struct _string *initString(char *txt) {
	struct _string *str=malloc(sizeof(struct _string));
	str->text=txt;
	str->length=strlen(txt)+1;
	str->next=NULL;
	return str; 
}

static void deleteString(struct _string* str) {
	if(str)
		free(str);
}

static int isEnd(struct _code* c, struct _string *str) {
	CHK(is NULL,c);
	CHK(is NULL,str);
	return (c->end == str && str->next == NULL);
}

static int isEmpty(struct _code* c){
	CHK(is NULL,c);
	if((c->begin == NULL && c->end != NULL) || (c->begin != NULL && c->end == NULL)){
		fprintf(stderr, "invalid situation in code value at %s in %s line %d\n",__FILE__,__func__,__LINE__);exit(1);
	}

	return (c->begin == NULL && c->end == NULL);
}
