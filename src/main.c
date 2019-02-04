/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: main.c
    Tento modul sjednocuje všechny komponenty programu a obsahuje spustitelný
    bod programu.
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include "fs_structures.h"
#include "file_manager.h"
#include "shell.h"
#include "global_vars.h"

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define NUM_OF_ARGS		2		/* poèet nutných argumentù pøíkazové øádky */

/* ____________________________________________________________________________
 
    Globální promìnné modulu
   ____________________________________________________________________________
*/
char *file_name = NULL; 			/* název souboru se strukturou FS */


/* ____________________________________________________________________________

    void showhelp()
    
    Vytiskne na konzoli krátkou nápovìdu, jak používat program.
   ____________________________________________________________________________
*/
void showhelp() {
	
	printf("\nHELP:\n");
	printf("\nUsage:\n   pseudoNTFS.exe <file_system>\n\n");
	printf("   <file_system>");
	printf(" - name of the file system to");
	printf(" save inforamtion of files.\n");
	printf("Example:\n   pseudoNTFS.exe myFS \n\n");
}


/* ____________________________________________________________________________

    void init(int argc, char *argv[])
    
    Zpracuje parametry pøíkazové øádky `argv'. Parametr `argc' je poèet 
	parametrù na pøíkazové øádce.
   ____________________________________________________________________________
*/
void init(int argc, char *argv[]) {
	
	/* vypsání úvodní hlášky */
	printf("PseudoNTFS File System 1.0 Rel. 04-Feb-2019\n");
	printf("Semestral work - subject KIV/ZOS\n\n");
		
	if (argc != NUM_OF_ARGS) {
		printf("Incorrect count of arguments.\n");
		showhelp();
		exit(EXIT_FAILURE);
	}
	
	/* alokování pamìti pro název souboru -
	o jedna vìtší kvùli ukonèovacímu znaku '\000' */ 
	file_name = calloc(1, (strlen(argv[1]) + 1) * sizeof(char));
	strcpy(file_name, argv[1]);
}


/* ____________________________________________________________________________

    void run()
    
    Výkonná funkce programu. Vykonává veškerý výpoèet programu a dává
    dohromady všechny jeho èásti.
   ____________________________________________________________________________
*/
void run() {
	
	if (!file_exists(file_name)) {
		shell_before_format(file_name);
	}
	else {
		if (load_file(file_name) != 1) {
			exit(EXIT_FAILURE);
		}
	}
	
	shell_after_format(file_name);
}


/* ____________________________________________________________________________

    void shutdown() 
    
    Dealokuje pamì, referenèní promìnné nastaví na NULL, pøipraví
    na ukonèení programu.
   ____________________________________________________________________________
*/
void shutdown() {
	
	int mft_item_count = (global_boot_record->bitmap_start_address - 
			global_boot_record->mft_start_address) / sizeof(mft_item);
	
	/* uvolnìní pamìti po `file_name' */
	if (file_name != NULL) {
		free(file_name);
		file_name = NULL;
	}
	
	/* uvolnìní pamìti po `global_boot_record' */
	if (global_boot_record != NULL) {
		free(global_boot_record);
		global_boot_record = NULL;
	}
	
	/* uvolnìní pamìti po `global_mft_item_array' */
	if (global_mft_item_array != NULL) {
		delete_mft_item_array(global_mft_item_array, mft_item_count);
		global_mft_item_array = NULL;
	}
	
	/* uvolnìní pamìti po `global_bitmap' */
	if (global_bitmap != NULL) {
		free(global_bitmap);
		global_bitmap = NULL;
	}
	
	/* uzavøení proudù */
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}


/* ____________________________________________________________________________
 
    HLAVNÍ FUNKCE PROGRAMU
   ____________________________________________________________________________
*/
int main(int argc, char *argv[]) {
	
	init(argc, argv);
	run();
	shutdown();
	
	return EXIT_SUCCESS;
}