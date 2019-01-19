/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: shell.c
    Tento modul reprezentuje shell, kter� zpracov�v� vstup 
    od u�ivatele a ��d� o zpracov�n� po�adavku.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "command.h"
#include "process_command.h"

/* ____________________________________________________________________________

	void shell_before_format(char *file_name)
    
    Shell, kter� je spu�t�n� p�ed prvn�m naform�tov�n�m souboru.
   ____________________________________________________________________________
*/
void shell_before_format(char *file_name) {
	
	command *current_command = NULL;
	char buffer[BUFF_SIZE];
	int isCorrect = 0;
		
	do {
		memset(buffer, 0, BUFF_SIZE * sizeof(char));
		fgets(buffer, BUFF_SIZE, stdin);
		
		current_command = new_command(buffer);
		
		isCorrect = process_requirement_before(current_command, file_name);
	
		/* uvoln�n� pam�ti po `current_command' */
		if (current_command != NULL) {
			free(current_command);
			current_command = NULL;
		}
		
	} while (!isCorrect);
}


/* ____________________________________________________________________________

	void shell_after_format(char *file_name)
    
    Shell, kter� je spu�t�n� po prvn�m naform�tov�n�m souboru.
   ____________________________________________________________________________
*/
void shell_after_format(char *file_name) {
	
	command *current_command = NULL;
	char buffer[BUFF_SIZE];
		
	do {
		memset(buffer, 0, BUFF_SIZE * sizeof(char));
		fgets(buffer, BUFF_SIZE, stdin);
		
		current_command = new_command(buffer);
		
		process_requirement_after(current_command, file_name);
	
		/* uvoln�n� pam�ti po `current_command' */
		if (current_command != NULL) {
			free(current_command);
			current_command = NULL;
		}
		
	} while (1);
}
