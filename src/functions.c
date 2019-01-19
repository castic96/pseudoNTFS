/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: functions.c
    Tento modul obsahuje funkce pro vykonávání jednotlivých
    pøíkazù shellu.
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "functions.h"
#include "fs_structures.h"
#include "functions_helper.h"

/* ____________________________________________________________________________

	void func_ls(int32_t dir_uid, char *file_name)
    
    Zajistí vypsání obsahu složky zadané parametrem `dir_uid'. Parametr 
	`file_name' obsahuje název souboru, který reprezentuje disk.
   ____________________________________________________________________________
*/
void func_ls(int32_t dir_uid, char *file_name) {
	
	int i, j;
	
	/* poèet MFT záznamù pro jednu položku */
	int mft_count;
	
	/* pole UID obsahu dané složky */
	int *dir_content;
	
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
	
	curr_dir = get_mft_item_by_uid(dir_uid); //FREE
	
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
						if (mft_temp2[0]->type == 0) {
							printf("- %s\n", mft_temp2[0]->item_name);
						}
						else if (mft_temp2[0]->type == 1) {
							printf("+ %s\n", mft_temp2[0]->item_name);
						}
						else {
							printf("* %s\n", mft_temp2[0]->item_name);					
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
}


/* ____________________________________________________________________________

	void func_pwd(int32_t local_pwd, char *file_name)
    
    Vypíše absolutní cestu ze složky, jejíž UID je v parametru `local_pwd'. 
	Parametr `file_name' obsahuje název souboru, který reprezentuje disk.
   ____________________________________________________________________________
*/
void func_pwd(int32_t local_pwd, char *file_name) {
	
	int i, j;
	
	/* pole ukazatelù na názvy adresáøu na cestì */
	char *name_pointer_handler[BUFF_SIZE];
	
	/* pomocný ukazatel na jméno adresáøe */
	char *name_temp;
	
	/* buffer pro složení názvu hledané cesty */
	char buffer[BUFF_SIZE];
	
	/* pomocný ukazatel na buffer hledané cesty */
	char *buffer_temp;
	
	/* pomocná promìnná pro local_pwd */
	int32_t temp_pwd = local_pwd;
	
	/* poèet MFT záznamù pro jednu položku */
	int mft_count;
	
	/* pole UID obsahu dané složky */
	int *dir_content;
	
	/* odkaz na složku - pole MFT záznamù */
	mft_item **curr_dir = NULL;
	
	/* pomocná promìnná pro jednu MFT položku */
	mft_item *mft_temp = NULL;
	
	memset(buffer, 0, BUFF_SIZE * sizeof(char));
	
	if (temp_pwd == 1) {
		printf("/\n");
		return;
	}
	
	j = 0;
	
	while (temp_pwd != 1) {		
		curr_dir = get_mft_item_by_uid(temp_pwd); //FREE
		mft_count = curr_dir[0]->item_order_total;
		
		for (i = 0; i < mft_count; i++) {
			if (curr_dir[i]->item_order == 1) {
				mft_temp = curr_dir[i];
				break;			
			}
		}
		
		name_pointer_handler[j] = mft_temp->item_name;
		
		dir_content = load_dir_data(mft_temp->fragments[0].fragment_start_address, file_name);
		
		if (dir_content == NULL) {
			return;
		}
		
		temp_pwd = dir_content[0];
		
		/* uvolnìní pamìti po `dir_content' */
		if (dir_content != NULL) {
			free(dir_content);
			dir_content = NULL;
		}
		
		j++;
	}
	
	buffer_temp = buffer;
	
	for (i = 0; i < j; i++) {
		*buffer_temp = '/';
		buffer_temp++;
		name_temp = name_pointer_handler[(j - i - 1)];
		
		while (*name_temp != 0) {
			*buffer_temp = *name_temp;
			buffer_temp++;
			name_temp++;
		}
		
		*buffer_temp = 0;
	}	
	
	printf("%s\n", buffer);
}
