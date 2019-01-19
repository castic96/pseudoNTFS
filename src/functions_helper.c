/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: functions_helper.c
    Tento modul obsahuje pomocné funkce pro realizaci funkcí 
	pro vykonávání jednotlivých pøíkazù shellu.
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

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
    
    Dle zadaného `uid' najde MFT záznam nebo záznamy a vrátí je.
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
    
    Naète ze souboru, který je uložený v `path_name' seznam UID všech položek,
    které jsou uloženy v dané složce. Poèáteèní adresa pro ètení je uložena
    v parametru `start_address'. Pøi úspìšném naètení vrací funkce vrací pole 
	int hodnot, které odpovídají UID všech položek dané složky. Pøi neúspìchu
	vrací NULL.
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
    
    Dle zadané cesty `path' najde UID hledaného adresáøe v diskovém souboru
	`file_name'. Pøi úspìchu vrací UID hledaného adresáøe, v opaèném pøípadì
	vrací -1.
   ____________________________________________________________________________
*/
int32_t get_uid_from_path(char *path, char *file_name) {
	
	char *dir_name = NULL;
	int32_t actual_dir = -1;
	
	/* jedná se o absolutní cestu - zaèínám v adresáøi root */
	if (*path == '/') {
		actual_dir = 1;
		path++;
		
		/* bylo zadáno jen "/" - jedná se o vypsání obsahu adresáøe root */
		if (*path == '\0') {
			return actual_dir; 
		}
	}
	
	/* jedná se o relativní cestu */
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
    
    Dle UID aktuálního adresáøe `local_pwd' zjistí, zda se v tomto adresáøi
	nachází	adresáø s názvem `dir_name'. Parametr `file_name' obsahuje název 
	souboru, který reprezentuje disk. Pøi úspìchu vrací UID hledaného 
	adresáøe, v opaèném pøípadì vrací -1.
   ____________________________________________________________________________
*/
int32_t get_uid_by_name_dir(char *dir_name, int32_t local_pwd, char *file_name) {
	
	int i, j;
	
	/* poèet MFT záznamù pro jednu položku */
	int mft_count;

	/* pole UID obsahu dané složky */
	int *dir_content;
	
	/* získané UID složky k vrácení */	
	int32_t return_uid;
	
	/* pomocná promìnná pro zaèáteèní adresu, na kterou odkazuje fragment */
	int32_t temp_fragment_start_address;
	
	/* odkaz na složku - pole MFT záznamù */
	mft_item **curr_dir = NULL;
	
	/* pomocná promìnná pro jednu MFT položku */
	mft_item *mft_temp = NULL;
	
	/* pomocná promìnná pro pole MFT položek */
	mft_item **mft_temp2 = NULL;

	/* aktuálnì hledané poøadí MFT položky */
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
					
					/* pokud se nejedná o první adresu prvního fragmentu první MFT položky */
					/* jednalo by se o tisknutí odkazu na nadøazený adresáø */
					if (!((search_item_order == 1) && (i == 0) && (j == 0))) {
						if (strcmp(mft_temp2[0]->item_name, dir_name) == 0) {
							
							/* uvolnìní pamìti po `dir_content' */
							if (dir_content != NULL) {
								free(dir_content);
								dir_content = NULL;
							}
							
							/* pokud byl nalezen odpovídající název, ale nejde o adresáø */
							if (mft_temp2[0]->type != 1) {
								
								/* uvolnìní pamìti po `mft_temp2' */
								if (mft_temp2 != NULL) {
									free(mft_temp2);
									mft_temp2 = NULL;
								}
								
								return -1;
							}
							else {
								return_uid = mft_temp2[0]->uid;
								
								/* uvolnìní pamìti po `mft_temp2' */
								if (mft_temp2 != NULL) {
									free(mft_temp2);
									mft_temp2 = NULL;
								}
								
								return return_uid;
							}
						}						
					}
					
					j++;
					
					/* uvolnìní pamìti po `mft_temp2' */
					if (mft_temp2 != NULL) {
						free(mft_temp2);
						mft_temp2 = NULL;
					}
				}
				
				/* uvolnìní pamìti po `dir_content' */
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
	
	/* uvolnìní pamìti po `curr_dir' */
	if (curr_dir != NULL) {
		free(curr_dir);
		curr_dir = NULL;
	}
	
	return -1;
}
