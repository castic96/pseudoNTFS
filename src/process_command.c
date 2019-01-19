/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: process_command.c
    Tento modul slouží ke zpracování jednotlivých pøíkazù 
    a volání pøíslušných funkcí pro vykonání požadované
    funkcionality zadaného pøíkazu.
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "command.h"
#include "file_manager.h"
#include "process_command.h"
#include "fs_structures.h"
#include "global_vars.h"
#include "functions_helper.h"
#include "functions.h"

/* ____________________________________________________________________________

	int process_requirement_before(command *current_command, char *file_name)
    
    Zpracování pøíkazu pøed prvním naformátováním souboru. Vrací 1, pokud
    je pøíkaz validní, v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_requirement_before(command *current_command, char *file_name) {
	
	if (current_command->function == NULL) {
		printf("Neznamy prikaz!\n");
		return 0;
	}
	
	if (strcmp("format", current_command->function) == 0) {
		if (process_command_format(current_command, file_name) != 1) {
			printf("Chyba pri vykonavani funkce process_command_format()\n");
			return 0;
		}
	} 
	else {
		printf("Neznamy prikaz!\n");
		return 0;
	}
	
	return 1;
}


/* ____________________________________________________________________________

	int process_requirement_after(command *current_command, char *file_name)
    
    Zpracování pøíkazu po prvním naformátováním souboru. Vrací 1, pokud
    je pøíkaz validní, v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_requirement_after(command *current_command, char *file_name) {
	
	if (current_command->function == NULL) {
		printf("Neznamy prikaz!\n");
		return 0;
	}
	
	if (strcmp("format", current_command->function) == 0) {
		if (process_command_format(current_command, file_name) != 1) {
			printf("Chyba pri vykonavani funkce process_command_format()\n");
			return 0;
		}
	}
	else if (strcmp("ls", current_command->function) == 0) {
		if (process_command_ls(current_command, file_name) != 1) {
			printf("Chyba pri vykonavani funkce process_command_ls()\n");
			return 0;
		}
	}
	else if (strcmp("pwd", current_command->function) == 0) {
		if (process_command_pwd(current_command, file_name) != 1) {
			printf("Chyba pri vykonavani funkce process_command_pwd()\n");
			return 0;
		}
	}
	else {
		printf("Neznamy prikaz!\n");
		return 0;
	}
	
	return 1;
}


/* ____________________________________________________________________________

	int process_command_format(command *current_command, char *file_name)
    
    Zpracuje pøíkaz format. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_format(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int param_length = strlen(current_command->first_param);
		char *size_str = NULL;
		int size_int = 0;
		
		size_str = calloc(1, (strlen(current_command->first_param) + 1) * sizeof(char));
		strcpy(size_str, current_command->first_param);
		
		if ((size_str[param_length - 1] == 'B')  
			&& (size_str[param_length - 2] == 'M')) {
			size_str[strlen(size_str) - 1] = '\0';
			size_str[strlen(size_str) - 1] = '\0';
		
			size_int = atoi(size_str);
		
			if (size_str != NULL) {
				free(size_str);
				size_str = NULL;
			}
			
			if (format_file(file_name, size_int) != 1) {
				return 0;
			}
			
			if (load_file(file_name) != 1) {
				return 0;
			}
			printf("Formatovani dokonecno.\n");
			return 1;
		}
		else {
			return 0;
		}	
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_ls(command *current_command, char *file_name)
    
    Zpracuje pøíkaz ls. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_ls(command *current_command, char *file_name) {
	
	if ((current_command->first_param == NULL) && (current_command->second_param == NULL)) {
		func_ls(pwd, file_name);
		return 1;
	}
	else if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int32_t uid_found = get_uid_from_path(current_command->first_param, file_name);
		
		if (uid_found != -1) {
			func_ls(uid_found, file_name);
		} 
		else {
			printf("PATH NOT FOUND\n");
		}
		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_pwd(command *current_command, char *file_name)
    
    Zpracuje pøíkaz pwd. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_pwd(command *current_command, char *file_name) {
	
	if ((current_command->first_param == NULL) && (current_command->second_param == NULL)) {
		func_pwd(pwd, file_name);		
		return 1;
	}
	else {
		return 0;	
	}
}
