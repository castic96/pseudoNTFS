/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: command.c
    Tento modul slouží pro rozdìlení vstupu na funkci
    a její parametry. Slouží jako pøepravka tìchto hodnot.
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "command.h"

/* ____________________________________________________________________________

	command *new_command(char *buffer)
    
    Naète data z parametru `buffer' a rozdìlí je podle mezery na funkci
	a její parametry. Vrací strukturu `command', která slouží jako pøepravka
	výše zmínìných hodnot.
   ____________________________________________________________________________
*/
command *new_command(char *buffer) {
	
	command *new_comm = (command *) calloc(1, sizeof(command));
	
	new_comm->function = strtok(buffer, " ");
	new_comm->first_param = strtok(NULL, " ");
	new_comm->second_param = strtok(NULL, " ");
	
	remove_spaces(new_comm->function);
	remove_spaces(new_comm->first_param);
	remove_spaces(new_comm->second_param);

	return new_comm;
}


/* ____________________________________________________________________________

	void remove_spaces(char *buffer)
    
    Naète data z parametru `buffer' a odstraní bílé znaky.
   ____________________________________________________________________________
*/
void remove_spaces(char *buffer) {
	
	if (buffer == NULL) {
		return;
	}
	
	char *i = buffer;
	char *j = buffer;
	
	while (*j != '\0') {
		
		*i = *j++;
		
		if (!isspace(*i)) {
			i++;
		}
	}
	*i = '\0';
}
