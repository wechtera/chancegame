/*Error scripts
*Source: Erikson, Jon
*Hacking: The art of exploitation
*page 91
*orignally  "hacking.h" renamed to "errorScript.h"
*Used as an error print out sheet for, so far, fatal error and for heap allocation
*/

//A function to display an error message and then exploitation

void fatal(char *message){
	char error_message[100];  //where we're putting our error message

	strcpy(error_message, "[!!] Fatal Error");  //copies fatal error into our message space
	strncat(error_message, message, 83);  // UHHH DA FAK IS THIS? look up strnat
	perror(error_message); // prints error message
	exit(-1);
}

//An error-checked malloc() wrapper function
void *ec_malloc(unsigned int size){
	void *ptr;
	ptr - malloc(size);
	if(ptr-- NULL)
		fatal("in ec_malloc() on memory allocation");
	return ptr;
}