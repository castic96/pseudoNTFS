/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: main.c
    Tento modul sjednocuje v�echny komponenty programu a obsahuje spustiteln�
    bod programu.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

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

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
#define NUM_OF_ARGS		2		/* po�et nutn�ch argument� p��kazov� ��dky */

/* ____________________________________________________________________________
 
    Glob�ln� prom�nn� modulu
   ____________________________________________________________________________
*/
char *file_name = NULL; 			/* n�zev souboru se strukturou FS */


/* ____________________________________________________________________________

    void showhelp()
    
    Vytiskne na konzoli kr�tkou n�pov�du, jak pou��vat program.
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
    
    Zpracuje parametry p��kazov� ��dky `argv'. Parametr `argc' je po�et 
	parametr� na p��kazov� ��dce.
   ____________________________________________________________________________
*/
void init(int argc, char *argv[]) {
	
	/* vyps�n� �vodn� hl�ky */
	printf("PseudoNTFS File System 1.0 Rel. 04-Feb-2019\n");
	printf("Semestral work - subject KIV/ZOS\n\n");
		
	if (argc != NUM_OF_ARGS) {
		printf("Incorrect count of arguments.\n");
		showhelp();
		exit(EXIT_FAILURE);
	}
	
	/* alokov�n� pam�ti pro n�zev souboru -
	o jedna v�t�� kv�li ukon�ovac�mu znaku '\000' */ 
	file_name = calloc(1, (strlen(argv[1]) + 1) * sizeof(char));
	strcpy(file_name, argv[1]);
}


/* ____________________________________________________________________________

    void run()
    
    V�konn� funkce programu. Vykon�v� ve�ker� v�po�et programu a d�v�
    dohromady v�echny jeho ��sti.
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
    
    Dealokuje pam�, referen�n� prom�nn� nastav� na NULL, p�iprav�
    na ukon�en� programu.
   ____________________________________________________________________________
*/
void shutdown() {
	
	int mft_item_count = (global_boot_record->bitmap_start_address - 
			global_boot_record->mft_start_address) / sizeof(mft_item);
	
	/* uvoln�n� pam�ti po `file_name' */
	if (file_name != NULL) {
		free(file_name);
		file_name = NULL;
	}
	
	/* uvoln�n� pam�ti po `global_boot_record' */
	if (global_boot_record != NULL) {
		free(global_boot_record);
		global_boot_record = NULL;
	}
	
	/* uvoln�n� pam�ti po `global_mft_item_array' */
	if (global_mft_item_array != NULL) {
		delete_mft_item_array(global_mft_item_array, mft_item_count);
		global_mft_item_array = NULL;
	}
	
	/* uvoln�n� pam�ti po `global_bitmap' */
	if (global_bitmap != NULL) {
		free(global_bitmap);
		global_bitmap = NULL;
	}
	
	/* uzav�en� proud� */
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}


/* ____________________________________________________________________________
 
    HLAVN� FUNKCE PROGRAMU
   ____________________________________________________________________________
*/
int main(int argc, char *argv[]) {
	
	init(argc, argv);
	run();
	shutdown();
	
	return EXIT_SUCCESS;
}