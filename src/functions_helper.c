/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: functions_helper.c
    Tento modul obsahuje pomocn� funkce pro realizaci funkc� 
	pro vykon�v�n� jednotliv�ch p��kaz� shellu.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "fs_structures.h"
#include "global_vars.h"
#include "functions_helper.h"

/* ____________________________________________________________________________

	mft_item **get_mft_item_by_uid(int32_t uid)
    
    Dle zadan�ho `uid' najde MFT z�znam nebo z�znamy a vr�t� je.
   ____________________________________________________________________________
*/
mft_item **get_mft_item_by_uid(int32_t uid) {
	
	int i = 0;
	int j = 0;
	int count = 0;
	int mft_item_count;	
	mft_item **curr_mft_item_arr = NULL;
	
	mft_item_count = (global_boot_record->bitmap_start_address - 
					global_boot_record->mft_start_address) / sizeof(mft_item);
	
	for (i = 0; i < mft_item_count; i++) {
		if (global_mft_item_array[i]->uid == uid) {
			count++;
		}
	}
	
	curr_mft_item_arr = (mft_item **) calloc(count, sizeof(mft_item *));
	
	for (i = 0; i < mft_item_count; i++) {
		if (global_mft_item_array[i]->uid == uid) {
			curr_mft_item_arr[j] = global_mft_item_array[i];
			j++;
		}
	}
	
	return curr_mft_item_arr;
}


/* ____________________________________________________________________________

    int *load_dir_data(int32_t start_address, char *path_name)
    
    Na�te ze souboru, kter� je ulo�en� v `path_name' seznam UID v�ech polo�ek,
    kter� jsou ulo�eny v dan� slo�ce. Po��te�n� adresa pro �ten� je ulo�ena
    v parametru `start_address'. P�i �sp�n�m na�ten� vrac� funkce vrac� pole 
	int hodnot, kter� odpov�daj� UID v�ech polo�ek dan� slo�ky. P�i ne�sp�chu
	vrac� NULL.
   ____________________________________________________________________________
*/
int *load_dir_data(int32_t start_address, char *path_name) {
	
	int i;
	int count = 0;
	int temp_value;
	
	int *output = NULL; //FREE!!!
	FILE *file = NULL;
	
	file = fopen(path_name, "rb");
	
	if (file != NULL) {
		fseek(file, start_address, SEEK_SET);
		
		do {
			fread(&temp_value, sizeof(int), 1, file);
			count++;
		} while (temp_value != -1);
		
		output = (int *) calloc(count, sizeof(int));
		
		fseek(file, start_address, SEEK_SET);
		
		for (i = 0; i < count; i++) {
			fread(&temp_value, sizeof(int), 1, file);
			output[i] = temp_value;
		}
		
		fclose(file);
		
		return output;
	}
	else {
		printf("Chyba pri vykonavani funkce load_dir_data()\n");
		return NULL;
	}
}


/* ____________________________________________________________________________

    int32_t get_uid_from_path(char *path, char *file_name)
    
    Dle zadan� cesty `path' najde UID hledan�ho adres��e v diskov�m souboru
	`file_name'. P�i �sp�chu vrac� UID hledan�ho adres��e, v opa�n�m p��pad�
	vrac� -1.
   ____________________________________________________________________________
*/
int32_t get_uid_from_path(char *path, char *file_name) {
	
	char *dir_name = NULL;
	int32_t actual_dir = -1;
	
	/* jedn� se o absolutn� cestu - za��n�m v adres��i root */
	if (*path == '/') {
		actual_dir = 1;
		path++;
		
		/* bylo zad�no jen "/" - jedn� se o vyps�n� obsahu adres��e root */
		if (*path == '\0') {
			return actual_dir; 
		}
	}
	
	/* jedn� se o relativn� cestu */
	else {
		actual_dir = pwd;		
	}
	
	dir_name = strtok(path, "/");
	
	while (dir_name != NULL) {
		actual_dir = get_uid_by_name_dir(dir_name, actual_dir, file_name);
		
		if (actual_dir == -1) {
			return -1;
		}
		
		dir_name = strtok(NULL, "/");
	}
	
	return actual_dir;
}


/* ____________________________________________________________________________

    int32_t get_uid_by_name_dir(char *dir_name, int32_t local_pwd, char *file_name)
    
    Dle UID aktu�ln�ho adres��e `local_pwd' zjist�, zda se v tomto adres��i
	nach�z�	adres�� s n�zvem `dir_name'. Parametr `file_name' obsahuje n�zev 
	souboru, kter� reprezentuje disk. P�i �sp�chu vrac� UID hledan�ho 
	adres��e, v opa�n�m p��pad� vrac� -1.
   ____________________________________________________________________________
*/
int32_t get_uid_by_name_dir(char *dir_name, int32_t local_pwd, char *file_name) {
	
	int i, j;
	
	/* po�et MFT z�znam� pro jednu polo�ku */
	int mft_count;

	/* pole UID obsahu dan� slo�ky */
	int *dir_content;
	
	/* z�skan� UID slo�ky k vr�cen� */	
	int32_t return_uid;
	
	/* pomocn� prom�nn� pro za��te�n� adresu, na kterou odkazuje fragment */
	int32_t temp_fragment_start_address;
	
	/* odkaz na slo�ku - pole MFT z�znam� */
	mft_item **curr_dir = NULL;
	
	/* pomocn� prom�nn� pro jednu MFT polo�ku */
	mft_item *mft_temp = NULL;
	
	/* pomocn� prom�nn� pro pole MFT polo�ek */
	mft_item **mft_temp2 = NULL;

	/* aktu�ln� hledan� po�ad� MFT polo�ky */
	int search_item_order = 1;
	
	curr_dir = get_mft_item_by_uid(local_pwd); //FREE
	
	mft_count = curr_dir[0]->item_order_total;
	
	while(search_item_order <= mft_count) {
		for (i = 0; i < mft_count; i++) {
			if (curr_dir[i]->item_order == search_item_order) {
				mft_temp = curr_dir[i];
				break;			
			}
		}
		
		for (i = 0; i < MFT_FRAGMENTS_COUNT; i++) {
			temp_fragment_start_address = mft_temp->fragments[i].fragment_start_address;
			
			if (temp_fragment_start_address != 0) {
				dir_content = load_dir_data(temp_fragment_start_address, file_name);
				
				if (dir_content == NULL) {
					continue;
				}
				
				j = 0;
				
				while (dir_content[j] != -1) {
					mft_temp2 = get_mft_item_by_uid(dir_content[j]); //FREE
					
					/* pokud se nejedn� o prvn� adresu prvn�ho fragmentu prvn� MFT polo�ky */
					/* jednalo by se o tisknut� odkazu na nad�azen� adres�� */
					if (!((search_item_order == 1) && (i == 0) && (j == 0))) {
						if (strcmp(mft_temp2[0]->item_name, dir_name) == 0) {
							
							/* uvoln�n� pam�ti po `dir_content' */
							if (dir_content != NULL) {
								free(dir_content);
								dir_content = NULL;
							}
							
							/* pokud byl nalezen odpov�daj�c� n�zev, ale nejde o adres�� */
							if (mft_temp2[0]->type != 1) {
								
								/* uvoln�n� pam�ti po `mft_temp2' */
								if (mft_temp2 != NULL) {
									free(mft_temp2);
									mft_temp2 = NULL;
								}
								
								return -1;
							}
							else {
								return_uid = mft_temp2[0]->uid;
								
								/* uvoln�n� pam�ti po `mft_temp2' */
								if (mft_temp2 != NULL) {
									free(mft_temp2);
									mft_temp2 = NULL;
								}
								
								return return_uid;
							}
						}						
					}
					
					j++;
					
					/* uvoln�n� pam�ti po `mft_temp2' */
					if (mft_temp2 != NULL) {
						free(mft_temp2);
						mft_temp2 = NULL;
					}
				}
				
				/* uvoln�n� pam�ti po `dir_content' */
				if (dir_content != NULL) {
					free(dir_content);
					dir_content = NULL;
				}
			}
			else {
				break;
			}
		}
		search_item_order++;
	}
	
	/* uvoln�n� pam�ti po `curr_dir' */
	if (curr_dir != NULL) {
		free(curr_dir);
		curr_dir = NULL;
	}
	
	return -1;
}
