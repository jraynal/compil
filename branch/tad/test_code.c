#include <assert.h>
#include "code.h"

#ifndef DEBUG
#define DB(say)
#else
#define DB(say) fprintf(stderr,#say"\n");
#endif

#define TEST1 "my name is bob \n"
#define TEST2 "my name is michel and i am %d  or %d\n",12,7 


int main() {
	DB(##################################################);
	DB(Test de Code:);
	struct _code *my_code=initCode();

	DB(Chaine sans arguments...)
	DB(ajout...)
	addCode(my_code,TEST1);
	DB(impression...)
	printCode(STDOUT_FILENO,my_code);

	// printf(TEST2 );
	DB(ajout2...)
	addCode(my_code,TEST2);
	DB(impression...)
	printCode(STDOUT_FILENO,my_code);

	DB(destruction);
	deleteCode(my_code);

	DB(impression...)
	printCode(STDOUT_FILENO,my_code);


	DB(##################################################);
	return EXIT_SUCCESS;
}
