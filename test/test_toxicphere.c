#include "mtest.h"
#include "toxicphore.h"

/* In the following test we test to see if it can find a dobbelt bond and if it searches through 3 C atoms before finding a correct match */
TEST_CASE(Propanal_dobbeltbond,{
	  // arrange
	  char smile[] = "CCC=O"; 
	char toxicphore[] ="C=O"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})

/* simpel chain with molekyle at the end */ 
TEST_CASE(NC,{
	  // arrange
	  char smile[] = "CCCCNC"; 
	char toxicphore[] ="NC"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})

/* CCOCC and we are search for CO with is in the middle */ 
TEST_CASE(CO,{
	  // arrange
	  char smile[] = "CCOCC"; 
	char toxicphore[] ="CO"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})

/* here the string contains multiple matches. we just need to find one. We exptect 1 as a result */ 
TEST_CASE(multiple_matches,{
	  // arrange
	  char smile[] = "CCNCCN"; 
	char toxicphore[] ="CN"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})

/*has sidechains */ 
TEST_CASE(sidechains,{
	  // arrange
	  char smile[] = "CC(C)CC"; 
	char toxicphore[] ="CCC"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})

MAIN_RUN_TESTS(Propanal_dobbeltbond, NC, CO, multiple_matches, sidechains)
