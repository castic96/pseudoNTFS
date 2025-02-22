/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: file_manager.c
    Tento modul se star� o pr�ci s datov�m souborem pro ulo�en�
    souborov�ho syst�mu pseudoNTFS. Star� se o jeho vytvo�en�, ulo�en�
    struktur, form�tov�n� atd.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "file_manager.h"
#include "fs_structures.h"
#include "global_vars.h"

/* ____________________________________________________________________________

    int file_exists(char *path_name)
    
    Zjis�uje, zda soubor se zadanou cestou `path_name' existuje.
    Pokud existuje, vrac� 1, v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int file_exists(char *path_name) {
	
	if (access(path_name, F_OK) != -1) {
		return 1;
	} 
	else {
		return 0;
	}
}


/* ____________________________________________________________________________

    int format_file(char *path_name, int size)
    
    Zform�tuje soubor ulo�en� v `path_name' na po��te�n� hodnoty. 
	Pokud soubor neexistuje, vytvo�� jej. P�i �sp�chu vrac� 1, 
	v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int format_file(char *path_name, int size) {
	
	int i;
	int mft_item_count;
	int32_t data_start_address;
	int32_t root_link = UID_ITEM_ROOT;
	int32_t end_int_value = END_VALUE;
	
	FILE *file = NULL;
	boot_record *curr_boot_rec = NULL;
	mft_item **curr_mft_item_arr = NULL;
	int8_t *curr_bitmap = NULL;
	
	file = fopen(path_name, "wb");
	
	if (file != NULL) {
		
		/* inicializace struktur FS */
		curr_boot_rec = init_boot_record(size);
	
		mft_item_count = (curr_boot_rec->bitmap_start_address - 
						curr_boot_rec->mft_start_address) / sizeof(mft_item);
		data_start_address = curr_boot_rec->data_start_address;
		
		curr_mft_item_arr = init_mft_item_array(mft_item_count, data_start_address);
	
		curr_bitmap = init_bitmap(curr_boot_rec->cluster_count);
		
		
		/* zaps�n� struktur FS do souboru */
		fwrite(curr_boot_rec, sizeof(boot_record), 1, file);
		
		for (i = 0; i < mft_item_count; i++) {
			fwrite(curr_mft_item_arr[i], sizeof(mft_item), 1, file);
		}
		
		fwrite(curr_bitmap, sizeof(int8_t), (size_t) curr_boot_rec->cluster_count, file);
	
		/* zaps�n� odkazu na root adres�� na za��tek datov� oblasti + ukon�ovac� hodnota */
		fwrite(&root_link, sizeof(int32_t), 1, file);
		fwrite(&end_int_value, sizeof(int32_t), 1, file);
			
		/* zaps�n� EOF na konec cel�ho souboru */
        fseek(file, curr_boot_rec->data_start_address + curr_boot_rec->disk_size, SEEK_SET);
        putc(EOF, file);
		
		fclose(file);
		
	    /* uvoln�n� pam�ti po `curr_boot_rec' */
		if (curr_boot_rec != NULL) {
			free(curr_boot_rec);
			curr_boot_rec = NULL;
		}
	
		/* uvoln�n� pam�ti po `curr_mft_item_arr' */
		if (curr_mft_item_arr != NULL) {
			delete_mft_item_array(curr_mft_item_arr, mft_item_count);
			curr_mft_item_arr = NULL;
		}
	
		/* uvoln�n� pam�ti po `curr_bitmap' */
		if (curr_bitmap != NULL) {
			free(curr_bitmap);
			curr_bitmap = NULL;
		}
		
		return 1;
	}
	else {
		printf("Error in function format_file()\n");
		return 0;
	}
}


/* ____________________________________________________________________________

    int load_file(char *path_name)
    
    Na�te datov� struktury ze souboru, kter� je ulo�en� v `path_name'.
	P�i �sp�chu vrac� 1, v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int load_file(char *path_name) {
	
	int i;
	int mft_item_count;
	int global_mft_item_count;

	FILE *file = NULL;
	boot_record *curr_boot_rec = NULL;
	mft_item **curr_mft_item_arr = NULL;
	int8_t *curr_bitmap = NULL;
	
	file = fopen(path_name, "rb");
	
	if (file != NULL) {		
		curr_boot_rec = calloc_boot_record();
		fread(curr_boot_rec, sizeof(boot_record), 1, file);
		
		mft_item_count = (curr_boot_rec->bitmap_start_address - 
							curr_boot_rec->mft_start_address) / sizeof(mft_item);
							
		curr_mft_item_arr = calloc_mft_item_array(mft_item_count);
		
		for (i = 0; i < mft_item_count; i++) {
			fread(curr_mft_item_arr[i], sizeof(mft_item), 1, file);
		}
		
		curr_bitmap = calloc_bitmap(curr_boot_rec->cluster_count);
		fread(curr_bitmap, sizeof(int8_t), (size_t) curr_boot_rec->cluster_count, file);
		
		/* uvoln�n� pam�ti po `global_boot_record' + v�po�et global_mft_item_count */
		if (global_boot_record != NULL) {
			global_mft_item_count = (global_boot_record->bitmap_start_address - 
							global_boot_record->mft_start_address) / sizeof(mft_item);
			free(global_boot_record);
			global_boot_record = NULL;
		}
		
		/* uvoln�n� pam�ti po `global_mft_item_array' */
		if (global_mft_item_array != NULL) {
			delete_mft_item_array(global_mft_item_array, global_mft_item_count);
			global_mft_item_array = NULL;
		}
		
		/* uvoln�n� pam�ti po `global_bitmap' */
		if (global_bitmap != NULL) {
			free(global_bitmap);
			global_bitmap = NULL;
		}
		
		global_boot_record = curr_boot_rec;
		global_mft_item_array = curr_mft_item_arr;
		global_bitmap = curr_bitmap;
		
		pwd = 1;
		
		fclose(file);
				
		return 1;	
	}
	else {
		printf("Error in function load_file()\n");
		return 0;
	}
}