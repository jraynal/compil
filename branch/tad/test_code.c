#include <assert.h>
#include "code.h"

#ifndef DEBUG
#define DB(say)
#else
#define DB(say) fprintf(stderr,#say"\n");
#endif

#define TEST1 "my name is %s\n","bob"
#define TEST1_1 " and i am %d\n",18

#define TEST2 "my name is %s and i am %d or %d \n","ernest",70, 72


int main() {
	DB(##################################################);
	DB(Test de Code:);
	struct _code *my_code=initCode();
	struct _code *my_code2=initCode();
	struct _code *my_code3;

	DB(Chaine sans arguments...)
	DB(ajout...)
	addCode(my_code,TEST1);
	DB(impression...)
	printCode(STDOUT_FILENO,my_code);
	DB(ajout...)
	addCode(my_code,TEST1_1);
	DB(impression...)
	printCode(STDOUT_FILENO,my_code);

	// printf(TEST2 );
	DB(ajout2...)
	addCode(my_code2,TEST2);
	DB(impression...)
	printCode(STDOUT_FILENO,my_code);
	printCode(STDOUT_FILENO,my_code2);
	
	DB(fusion...)
	my_code3 = fusionCode(my_code,my_code2);
	DB(impression...)
	printCode(STDOUT_FILENO,my_code3);

	DB(destruction);
	deleteCode(my_code3);
	//ne pas delete les autres deja fusionnés

	DB(impression...)
	printCode(STDOUT_FILENO,my_code3);


	DB(##################################################);
	return EXIT_SUCCESS;
}
