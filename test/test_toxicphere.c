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

TEST_CASE(sidechains_bigger,{
	  // arrange
	  char smile[] = "CCN(CCOCC)NCCN"; 
	char toxicphore[] ="NCCN"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})


TEST_CASE(no_match,{
	  // arrange
	  char smile[] = "CCOCC"; 
	char toxicphore[] ="CN"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(0, test);  
})


TEST_CASE(atom_are_present_but_no_connection,{
	  // arrange
	  char smile[] = "NCCO"; 
	char toxicphore[] ="NCO"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(0, test);  
})


TEST_CASE(mixed_atomtypes,{
	  // arrange
	  char smile[] = "CCONCC"; 
	char toxicphore[] ="CON"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})


TEST_CASE(sidechain_but_no_connection_to_neighbour,{
	  // arrange
	  char smile[] = "CC(N)CC"; 
	char toxicphore[] ="CNC"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(0, test);  
})


TEST_CASE(overlapping_matches,{
	  // arrange
	  char smile[] = "COCOC"; 
	char toxicphore[] ="COC"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})

TEST_CASE(ring_structure_match,{
	  // arrange
	  char smile[] = "C1CCCCC1"; 
	char toxicphore[] ="CCC"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})


TEST_CASE(more_atom_in_tox,{
	  // arrange
	  char smile[] = "CC"; 
	char toxicphore[] ="CCC"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(0, test);  
})


TEST_CASE(toxic_with_sidechain,{
	  // arrange
	printf("Toxic with sidechain \n"); 
	  char smile[] = "CC(C)CC"; 
	char toxicphore[] ="C(C)C"; 

	  // assert 
	int test = toxicphore_function(smile, toxicphore); 



	  //test 
	CHECK_EQ_INT(1, test);  
})

MAIN_RUN_TESTS(Propanal_dobbeltbond, 
	       NC, 
	       CO, 
	       multiple_matches, 
	       sidechains, 
	       sidechains_bigger,
	       no_match,
	       atom_are_present_but_no_connection,
	       mixed_atomtypes,
	       sidechain_but_no_connection_to_neighbour,
	       overlapping_matches,
	       ring_structure_match,
	       more_atom_in_tox,
	       toxic_with_sidechain)
