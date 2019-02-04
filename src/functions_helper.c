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
    
    Dle zadan�ho `uid' najde MFT z�znam nebo z�znamy a vr�t� je. Pokud
    neexistuje z�znam s dan�m UID, vrac� NULL.
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
	
	if (count == 0) {
		return NULL;
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
    
    Na�te ze souboru, kter� je ulo�en� v `path_name', seznam UID v�ech polo�ek,
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
		printf("Error in function load_dir_data()\n");
		return NULL;
	}
}


/* ____________________________________________________________________________

    int save_dir_data(int32_t address, int value, char *path_name)
    
    Do adres��e, kter� je ulo�en� ve FS v cest� `path_name' ulo�� na adresu 
	`address' hodnotu souboru z parametru `value'. Vrac� 1, pokud v�e prob�hlo 
	v po��dku, v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int save_dir_data(int32_t address, int value, char *path_name) {
	
	int32_t end_int_value = END_VALUE;
	FILE *file = NULL;
	
	file = fopen(path_name, "r+b");
	
	if (file != NULL) {
		fseek(file, address, SEEK_SET);
		fwrite(&value, sizeof(int32_t), 1, file);
		fwrite(&end_int_value, sizeof(int32_t), 1, file);
		
		fclose(file);
		
		return 1;
	}
	else {
		printf("Error in function save_dir_data()\n");
		return 0;
	}
}


/* ____________________________________________________________________________

    int save_file_data(mft_item *found_mft_item, char *buffer, int32_t buff_size, int32_t num_of_clusters, char *file_name)
    
    Ulo�� data z parametru `buffer' do souboru, kter� je reprezentov�n MFT
	z�znamem ulo�en�m v parametru `found_mft_item'. Parametr `buff_size'
	obsahuje velikost bufferu, parametr `num_of_clusters' obsahuje celkov�
	po�et cluster� pro dan� MFT z�znam. Parametr `file_name' obsahuje n�zev 
	souboru, kter� reprezentuje disk. Vrac� 1, pokud v�e prob�hlo 
	v po��dku, v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int save_file_data(mft_item *found_mft_item, char *buffer, int32_t buff_size, int32_t num_of_clusters, char *file_name) {
	
	int i, j;
	
	FILE *file = NULL;
	
	int32_t frag_count_temp = 0;

	/* po�et ji� pou�it�ch cluster� */
	int32_t clusters_used = 0;
	
	/* po�et zapsan�ch B */
	int written_bytes = 0;
	
	file = fopen(file_name, "r+b");
	
	if (file != NULL) {
		j = 0;
		
		while (clusters_used < num_of_clusters) {
			
			frag_count_temp = found_mft_item->fragments[j].fragment_count;
			
			fseek(file, found_mft_item->fragments[j].fragment_start_address, SEEK_SET);
			
			if (buff_size < (CLUSTER_SIZE * frag_count_temp)) {
				written_bytes = fwrite(buffer, sizeof(char), buff_size, file);
			}
			else {
				written_bytes = fwrite(buffer, sizeof(char), CLUSTER_SIZE * frag_count_temp, file);				
			}
			
			buffer += written_bytes;
			
			buff_size -= written_bytes;
					
			clusters_used += frag_count_temp;
			
			j++;
		}
		
		fclose(file);
		
		return 1;
	}
	else {
		printf("Error in function save_file_data()\n");
		return 0;
	}
}

/* ____________________________________________________________________________

    int32_t get_uid_from_path_dir(char *path, char *file_name)
    
    Dle zadan� cesty `path' najde UID hledan�ho adres��e v diskov�m souboru
	`file_name'. P�i �sp�chu vrac� UID hledan�ho adres��e, v opa�n�m p��pad�
	vrac� -1.
   ____________________________________________________________________________
*/
int32_t get_uid_from_path_dir(char *path, char *file_name) {
	
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
		if (strcmp(dir_name, "..") == 0) {
			actual_dir = get_parent_from_uid(actual_dir, file_name);
		} 
		else if (strcmp(dir_name, ".") == 0) {	
		}
		else {
			actual_dir = get_uid_by_name_dir(dir_name, actual_dir, file_name);
			
			if (actual_dir == -1) {
				return -1;
			}
		}
		
		dir_name = strtok(NULL, "/");
	}
	
	return actual_dir;
}


/* ____________________________________________________________________________

    int32_t get_uid_from_path_all(char *path, char *file_name)
    
    Dle zadan� cesty `path' najde UID hledan�ho souboru v diskov�m souboru
	`file_name'. P�i �sp�chu vrac� UID hledan�ho souboru, v opa�n�m p��pad�
	vrac� -1.
   ____________________________________________________________________________
*/
int32_t get_uid_from_path_all(char *path, char *file_name) {
	
	char *file = NULL;
	
	int32_t actual_file = -1;
	
	/* jedn� se o absolutn� cestu - za��n�m v adres��i root */
	if (*path == '/') {
		actual_file = 1;
		path++;
		
		/* bylo zad�no jen "/" - jedn� se o adres�� root */
		if (*path == '\0') {
			return actual_file; 
		}
	}
	
	/* jedn� se o relativn� cestu */
	else {
		actual_file = pwd;
	}
	
	file = strtok(path, "/");
	
	while (file != NULL) {
		if (strcmp(file, "..") == 0) {
			actual_file = get_parent_from_uid(actual_file, file_name);
		} 
		else if (strcmp(file, ".") == 0) {	
		}
		else {
			actual_file = get_uid_by_name_all(file, actual_file, file_name);
			
			if (actual_file == -1) {
				return -1;
			}
		}
		
		file = strtok(NULL, "/");
	}
	
	return actual_file;
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
							
							/* uvoln�n� pam�ti po `curr_dir' */
							if (curr_dir != NULL) {
								free(curr_dir);
								curr_dir = NULL;
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


/* ____________________________________________________________________________

    int32_t get_uid_by_name_all(char *name, int32_t local_pwd, char *file_name)
    
    Dle UID aktu�ln�ho adres��e `local_pwd' zjist�, zda se v tomto adres��i
	nach�z�	soubor s n�zvem `name'. Parametr `file_name' obsahuje n�zev 
	souboru, kter� reprezentuje disk. P�i �sp�chu vrac� UID hledan�ho 
	souboru, v opa�n�m p��pad� vrac� -1.
   ____________________________________________________________________________
*/
int32_t get_uid_by_name_all(char *name, int32_t local_pwd, char *file_name) {
	
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
						if (strcmp(mft_temp2[0]->item_name, name) == 0) {
							
							/* uvoln�n� pam�ti po `dir_content' */
							if (dir_content != NULL) {
								free(dir_content);
								dir_content = NULL;
							}
							
							/* uvoln�n� pam�ti po `curr_dir' */
							if (curr_dir != NULL) {
								free(curr_dir);
								curr_dir = NULL;
							}
							
							return_uid = mft_temp2[0]->uid;
							
							/* uvoln�n� pam�ti po `mft_temp2' */
							if (mft_temp2 != NULL) {
								free(mft_temp2);
								mft_temp2 = NULL;
							}
								
							return return_uid;
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


/* ____________________________________________________________________________

    int32_t add_new_dir(char *dir_name, int32_t parent_dir_uid, char *file_name)
    
    Zkontroluje, zda existuje voln� MFT z�znam a zda je voln� m�sto v datov�
	oblasti a pokud ano, vytvo�� a p�id� nov� adres��. Jm�no adres��e je
	obsa�eno v parametru `dir_name', UID nad�azen�ho adres��e v `parent_dir_uid'.
	Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje disk. 
	P�i �sp�chu vrac� UID nov�ho adres��e, v opa�n�m p��pad� vrac� -1.
   ____________________________________________________________________________
*/
int32_t add_new_dir(char *dir_name, int32_t parent_dir_uid, char *file_name) {
	
	int i;
	
	/* po��te�n� adresa datov�ho bloku pro nov� adres�� */
	int32_t dir_start_address = -1;
	
	/* celkov� po�et MFT z�znam� */
	int mft_item_count;
	
	/* nalezen� voln� MFT z�znam */
	mft_item *found_mft_item = NULL;
	
	mft_item_count = (global_boot_record->bitmap_start_address - 
					global_boot_record->mft_start_address) / sizeof(mft_item);
	
	
	/* nalezen� voln�ho MFT z�znamu */
	for (i = 0; i < mft_item_count; i++) {
		if (global_mft_item_array[i]->uid == 0) {
			found_mft_item = global_mft_item_array[i];
			break;
		}
	}
	
	/* pokud ��dn� MFT z�znam nen� voln� */
	if (found_mft_item == NULL) {
		printf("MFT items are full.\n");
		return -1;
	}
	
	/* nalezen� voln�ho clusteru */
	for (i = 0; i < global_boot_record->cluster_count; i++) {
		if (global_bitmap[i] == 0) {
			global_bitmap[i] = 1;
			dir_start_address = global_boot_record->data_start_address + (i * CLUSTER_SIZE);
			break;
		}
	}
	
	/* pokud ��dn� cluster nen� voln� */
	if (dir_start_address == -1) {
		printf("Data block is full.\n");
		return -1;
	}
	
	/* zaps�n� hodnot nov�ho MFT z�znamu pro nov� adres�� */
	found_mft_item->uid = get_free_uid();	
	found_mft_item->type = 1;
	found_mft_item->item_order = 1;
	found_mft_item->item_order_total = 1;
	found_mft_item->item_size = CLUSTER_SIZE;
	strcpy(found_mft_item->item_name, dir_name);
	
	found_mft_item->fragments[0].fragment_start_address = dir_start_address;
	found_mft_item->fragments[0].fragment_count = 1;
	
	/* ulo�en� dat nov�ho adres��e do datov� oblasti (pouze odkaz na nad�azen� adres��) */
	if (save_dir_data(dir_start_address, parent_dir_uid, file_name) == 0) {
		printf("Error during saving directory data into data block.\n");
		return -1;
	}
	
	return found_mft_item->uid;	
}


/* ____________________________________________________________________________

    int write_global_vars_to_file(char *path_name) 
    
    Do souboru reprezentuj�c� disk, kter� je ulo�en� v `path_name', 
	ulo�� hodnoty z glob�ln�ch prom�nn�ch pro boot record, pole MFT polo�ek a 
	bitmapu. Vrac� 1, pokud v�e prob�hlo v po��dku, v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int write_global_vars_to_file(char *path_name) {
	
	int i;
	
	/* celkov� po�et MFT z�znam� */
	int mft_item_count;
	
	FILE *file = NULL;
	
	file = fopen(path_name, "r+b");
	
	if (file != NULL) {
		mft_item_count = (global_boot_record->bitmap_start_address - 
				global_boot_record->mft_start_address) / sizeof(mft_item);
		
		/* z�pis global boot record */
		fseek(file, 0, SEEK_SET);
		fwrite(global_boot_record, sizeof(boot_record), 1, file);
		
		/* z�pis global_mft_item_array */
		for (i = 0; i < mft_item_count; i++) {
			fwrite(global_mft_item_array[i], sizeof(mft_item), 1, file);
		}
		
		/* z�pis global_bitmap */
		fwrite(global_bitmap, sizeof(int8_t), (size_t) global_boot_record->cluster_count, file);
		
		fclose(file);
		
		return 1;
	}
	else {
		printf("Error in function write_global_vars_to_file()\n");
		return 0;
	}
}


/* ____________________________________________________________________________

    int32_t get_free_uid()
    
    Vr�t� unik�tn� voln� id pro MFT polo�ku.
   ____________________________________________________________________________
*/
int32_t get_free_uid() {
	
	int i;
	
	/* celkov� po�et MFT z�znam� */
	int mft_item_count;
	
	int32_t temp = 0;
	
	mft_item_count = (global_boot_record->bitmap_start_address - 
			global_boot_record->mft_start_address) / sizeof(mft_item);
	
	for (i = 0; i < mft_item_count; i++) {
		if (global_mft_item_array[i]->uid >= temp) {
			temp = global_mft_item_array[i]->uid;
		}
	}
	
	temp++;
	
	return temp;
}


/* ____________________________________________________________________________

    int is_name_unique(char *name, int32_t dir_uid, char *file_name)
    
    Zkontroluje, jestli je jm�no souboru p�edan� parametrem `name' unik�tn�
	v dan�m adres��i, jeho� UID se obsa�eno v parametru `dir_uid'. Parametr 
	`file_name' obsahuje n�zev souboru, kter� reprezentuje disk. Vrac� 1, 
	pokud je jm�no souboru v dan�m adres��i unik�tn�, v opa�n�m p��pad� 
	vrac� 0.
   ____________________________________________________________________________
*/
int is_name_unique(char *name, int32_t dir_uid, char *file_name) {
	if (get_uid_by_name_all(name, dir_uid, file_name) == -1) {
		return 1;
	}
	
	return 0;
}


/* ____________________________________________________________________________

    int32_t get_parent_from_uid(int32_t dir_uid, char *file_name)
    
    Vr�t� UID rodi�e adres��e, jeho� UID je zadan� v parametru `dir_uid'.
	Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje disk. 
	Vrac� UID rodi�e.
   ____________________________________________________________________________
*/
int32_t get_parent_from_uid(int32_t dir_uid, char *file_name) {
	
	int i;
	
	/* po�et MFT z�znam� pro jednu polo�ku */
	int mft_count;
	
	/* pole UID obsahu dan� slo�ky */
	int *dir_content;
	
	/* odkaz na slo�ku - pole MFT z�znam� */
	mft_item **curr_dir = NULL;
	
	/* pomocn� prom�nn� pro jednu MFT polo�ku */
	mft_item *mft_temp = NULL;
	
	/* pomocn� prom�nn� pro pole MFT polo�ek */
	mft_item **mft_temp2 = NULL;
	
	/* pomocn� prom�nn� pro n�vratovou hodnotu */
	int return_value = -1;
	
	curr_dir = get_mft_item_by_uid(dir_uid);
	
	mft_count = curr_dir[0]->item_order_total;
	
	for (i = 0; i < mft_count; i++) {
		if (curr_dir[i]->item_order == 1) {
			mft_temp = curr_dir[i];
			break;			
		}
	}
	
	dir_content = load_dir_data(mft_temp->fragments[0].fragment_start_address, file_name);
	
	return_value = dir_content[0];
	
	/* uvoln�n� pam�ti po `dir_content' */
	if (dir_content != NULL) {
		free(dir_content);
		dir_content = NULL;
	}
	
	/* uvoln�n� pam�ti po `curr_dir' */
	if (curr_dir != NULL) {
		free(curr_dir);
		curr_dir = NULL;
	}
	
	return return_value;
}


/* ____________________________________________________________________________

    int32_t add_new_file(char *buffer, int32_t buff_size, int32_t dir_uid_ntfs, char *name_ntfs, char *file_name)
    
    Zkontroluje, zda existuje voln� MFT z�znam a zda je voln� m�sto v datov�
	oblasti a pokud ano, vytvo�� a p�id� nov� soubor. Obsah souboru je
	v parametru `buffer', velikost tohoto bufferu v parametru `buffer_size',
	UID adres��e, kde m� b�t soubor ulo�en je v `parent_dir_uid' a jm�no nov�ho
	souboru je v parametru `name_ntfs'.	Parametr `file_name' obsahuje n�zev 
	souboru, kter� reprezentuje disk. P�i �sp�chu vrac� UID nov�ho souboru, 
	v opa�n�m p��pad� vrac� -1.
   ____________________________________________________________________________
*/
int32_t add_new_file(char *buffer, int32_t buff_size, int32_t dir_uid_ntfs, char *name_ntfs, char *file_name) {
	
	int i, j;
	
	/* po��te�n� adresa datov�ho bloku pro nov� soubor */
	int32_t file_start_address = -1;
	
	/* po�et cluster� pot�ebn�ch pro zaps�n� bufferu */
	int32_t num_of_clusters = -1;
	
	/* celkov� po�et MFT z�znam� */
	int mft_item_count;
	
	/* nalezen� voln� MFT z�znam */
	mft_item *found_mft_item = NULL;
	
	/* po�et ji� pou�it�ch cluster� */
	int32_t clusters_used = 0;
	
	/* voln� clustery */
	int *free_clusters = NULL;
	
	mft_item_count = (global_boot_record->bitmap_start_address - 
					global_boot_record->mft_start_address) / sizeof(mft_item);
	
	/* nalezen� voln�ho MFT z�znamu */
	for (i = 0; i < mft_item_count; i++) {
		if (global_mft_item_array[i]->uid == 0) {
			found_mft_item = global_mft_item_array[i];
			break;
		}
	}
	
	/* pokud ��dn� MFT z�znam nen� voln� */
	if (found_mft_item == NULL) {
		printf("MFT items are full.\n");
		return -1;
	}

	/* zji�t�n� pot�ebn�ho po�tu cluster� */
	if (buff_size % CLUSTER_SIZE == 0) {
		num_of_clusters = (int32_t) buff_size / CLUSTER_SIZE;
    }
	else {
		num_of_clusters = (int32_t) (buff_size / CLUSTER_SIZE) + 1;
	}
	
	/* pokud je volno m�n� cluster�, ne� je pot�eba */
	if (get_free_clust_num() < num_of_clusters) {
		printf("Data block is full.\n");
		return -1;
	}
	
	j = 0;
	
	while (clusters_used < num_of_clusters) {
		if (j >= MFT_FRAGMENTS_COUNT) {
			printf("Data block is full - data broken.\n");
			break;
		}
		
		free_clusters = get_largest_continuous_clust(num_of_clusters - clusters_used);
		
		clusters_used += free_clusters[1];
	
		found_mft_item->fragments[j].fragment_start_address = global_boot_record->data_start_address + (free_clusters[0] * CLUSTER_SIZE);
		found_mft_item->fragments[j].fragment_count = free_clusters[1];
		
		for (i = free_clusters[0]; i < (free_clusters[0] + free_clusters[1]); i++) {
			global_bitmap[i] = 1;
		}
		
		j++;
		
		/* uvoln�n� pam�ti po `free_clusters' */
		if (free_clusters != NULL) {
			free(free_clusters);
			free_clusters = NULL;
		}	
	}
	
	/* zaps�n� hodnot nov�ho MFT z�znamu pro nov� soubor */
	found_mft_item->uid = get_free_uid();	
	found_mft_item->type = 0;
	found_mft_item->item_order = 1;
	found_mft_item->item_order_total = 1;
	found_mft_item->item_size = buff_size;
	strcpy(found_mft_item->item_name, name_ntfs);
	
	/* ulo�en� dat nov�ho souboru do datov� oblasti */
	if (save_file_data(found_mft_item, buffer, buff_size, num_of_clusters, file_name) == 0) {
		printf("Error during saving directory data into data block.\n");
		return -1;
	}
	
	return found_mft_item->uid;	
}


/* ____________________________________________________________________________

    int get_free_clust_num()
    
    Zjist� celkov� po�et voln�ch cluster� ve FS a hodnotu vr�t�.
   ____________________________________________________________________________
*/
int get_free_clust_num() {
	
	int i;
	
	/* po�et voln�ch cluster� */
	int free = 0;	
	
	for (i = 0; i < global_boot_record->cluster_count; i++) {
		if (global_bitmap[i] == 0) {
			free++;
		}
	}
	
	return free;
}


/* ____________________________________________________________________________

    int *get_largest_continuous_clust(int32_t num_of_clusters)
    
    Zjist� nejdel�� sekvenci voln�ch cluster�, do maxim�ln� v��e zadan� 
	parametrem `num_of_clusters'. Tuto hodnotu vr�t�.
   ____________________________________________________________________________
*/
int *get_largest_continuous_clust(int32_t num_of_clusters) {
	
	int i;
	
	int free = 0;
	int first = -1;
	
	/* n�vratov� hodnota - pole o 2 prvc�ch
		1. pozice: prvn� cluster nalezen� oblasti
		2. pozice: po�et cluster� nalezen� oblasti 
	*/
	int *return_value = NULL;
	
	return_value = (int *) calloc(2, sizeof(int));
	
	return_value[0] = -1;
	return_value[1] = 0;
		
	for (i = 0; i < global_boot_record->cluster_count; i++) {
		if (global_bitmap[i] == 0) {
			if (free == 0) {
				first = i;
			}
			
			free++;
			
			if (free == num_of_clusters) {
				return_value[0] = first;
				return_value[1] = free;
				
				return return_value;
			}
		}
		else {
			if (free > return_value[1]) {
				return_value[0] = first;
				return_value[1] = free;
			}
			
			free = 0;
			first = -1;
		}
	}
	
	if (free > return_value[1]) {
		return_value[0] = first;
		return_value[1] = free;
	}
	
	return return_value;	
}


/* ____________________________________________________________________________

    int add_to_parent(int32_t parent_dir_uid, int32_t file_uid, char *file_name)
    
    Do adres��e, kde m� b�t ulo�en soubor (resp. podadres��) p�id� UID tohoto
	souboru (resp. podadres��e). V parametru `parent_dir_uid' je obsa�eno
	UID adres��e, v parametru `file_uid' je obsa�eno UID 
	souboru (resp. podadres��e). Parametr `file_name' obsahuje n�zev 
	souboru, kter� reprezentuje disk. Vrac� 1, pokud v�e prob�hlo v po��dku, 
	v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int add_to_parent(int32_t parent_dir_uid, int32_t file_uid, char *file_name) {
	
	int i;
	
	int32_t address;
	
	int return_value = -1;
		
	/* po�et MFT z�znam� pro jednu polo�ku */
	int mft_count; 
	
	/* pole UID obsahu dan� slo�ky */
	int *dir_content;
	
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_dir = NULL;
	
	/* pomocn� prom�nn� pro jednu MFT polo�ku */
	mft_item *mft_temp = NULL;
		
	/* pomocn� prom�nn� pro za��te�n� adresu, na kterou odkazuje fragment */
	int32_t temp_fragment_start_address;
	
	curr_dir = get_mft_item_by_uid(parent_dir_uid);	
	
	mft_count = curr_dir[0]->item_order_total;
	
	/* nalezen� posledn� polo�ky */
	for (i = 0; i < mft_count; i++) {
		if (curr_dir[i]->item_order == mft_count) {
			mft_temp = curr_dir[i];
			break;			
		}
	}
	
	/* nalezen� posledn�ho fragmentu */
	for (i = 0; i < MFT_FRAGMENTS_COUNT; i++) {
		if (mft_temp->fragments[i].fragment_start_address == 0) {
			break;
		}
		
		temp_fragment_start_address = mft_temp->fragments[i].fragment_start_address;
	}
	
	dir_content = load_dir_data(temp_fragment_start_address, file_name);
	
	i = 0;
	
	while (dir_content[i] != -1) {
		i++;
	}
	
	address = temp_fragment_start_address + i * sizeof(int32_t);
	
	/* ulo�en� UID souboru do datov� oblasti nad�azen�ho adres��e */
	return_value = save_dir_data(address, file_uid, file_name);
	
	/* uvoln�n� pam�ti po `dir_content' */
	if (dir_content != NULL) {
		free(dir_content);
		dir_content = NULL;
	}
	
	/* uvoln�n� pam�ti po `curr_dir' */
	if (curr_dir != NULL) {
		free(curr_dir);
		curr_dir = NULL;
	}
	
	if (return_value == 0) {
		return 0;
	}
		
	return 1;
}


/* ____________________________________________________________________________

    int remove_from_parent(int32_t uid_parent_dir, int32_t uid_file, char *file_name)
    
    Odstran� UID souboru, kter� je p�edan� parametrem `uid_file' z datov�
	oblasti adres��e s UID `uid_parent_dir'. Parametr `file_name' obsahuje 
	n�zev souboru, kter� reprezentuje disk. Vrac� 1, pokud v�e prob�hlo 
	v po��dku, v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int remove_from_parent(int32_t uid_parent_dir, int32_t uid_file, char *file_name) {
	
	int i, j;
	
	int32_t address_uid_file;
	int32_t address_uid_last;
	
	int return_value1 = -1;
	int return_value2 = -1;
		
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_dir = NULL;
	
	/* pole UID obsahu dan� slo�ky */
	int *dir_content;
	
	/* pomocn� prom�nn� pro za��te�n� adresu, na kterou odkazuje fragment */
	int32_t temp_fragment_start_address;
	
	
	curr_dir = get_mft_item_by_uid(uid_parent_dir);
	
	temp_fragment_start_address = curr_dir[0]->fragments[0].fragment_start_address;
	
	dir_content = load_dir_data(temp_fragment_start_address, file_name);
	
	i = 0;
	
	while (dir_content[i] != -1) {
		if (dir_content[i] == ((int) uid_file)) {
			break;
		}
		
		i++;
	}

	address_uid_file = temp_fragment_start_address + i * sizeof(int32_t);
	
	i++;
	j = 0;
	
	if (dir_content[i] == -1) {
		return_value1 = save_dir_data(address_uid_file, dir_content[i], file_name);
	}
	
	while (dir_content[i] != -1) {
		return_value2 = save_dir_data(address_uid_file + j * sizeof(int32_t), dir_content[i], file_name);
		
		if (return_value2 == 0) {
			break;
		}
		
		j++;
		i++;
	}
	
	/* uvoln�n� pam�ti po `dir_content' */
	if (dir_content != NULL) {
		free(dir_content);
		dir_content = NULL;
	}
	
	/* uvoln�n� pam�ti po `curr_dir' */
	if (curr_dir != NULL) {
		free(curr_dir);
		curr_dir = NULL;
	}
	
	if ((return_value1 == 0) || (return_value2 == 0)) {
		return 0;
	}
		
	return 1;
}


/* ____________________________________________________________________________

    char *get_data_from_file(int32_t file_uid, char *file_name)
    
    Ze souboru s UID, kter� je v parametru `file_uid' na�te data, kter� ulo��
	do bufferu. Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje 
	disk. Vrac� buffer s daty, pokud v�e prob�hlo v po��dku, v opa�n�m p��pad� 
	vrac� NULL.
   ____________________________________________________________________________
*/
char *get_data_from_file(int32_t file_uid, char *file_name) {
	
	int i, j;
	
	/* buffer pro data */
	char *buffer = NULL;
	
	/* pomocn� ukazatel na buffer s daty */
	char *buff_pointer = NULL;
	
	/* velikost bufferu v B */
	int32_t buff_size = 0;
	
	/* handler na soubor */
	FILE *file = NULL;
	
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_file = NULL;
	
	/* pomocn� prom�nn� pro za��te�n� adresu, na kterou odkazuje fragment */
	int32_t frag_start_addr_temp = 0;
	
	/* po�et cluster�, kter� obsahuje dan� fragment */
	int32_t frag_count_temp = 0;
	
	/* po�et p�e�ten�ch B */
	int read_bytes = 0;
	
	file = fopen(file_name, "r+b");
	
	if (file != NULL) {
		j = 0;
		
		curr_file = get_mft_item_by_uid(file_uid);		
		
		buff_size = curr_file[0]->item_size;
		
		buffer = (char *) calloc(buff_size, sizeof(char));
		
		buff_pointer = buffer;
		
		frag_start_addr_temp = curr_file[0]->fragments[j].fragment_start_address;
		frag_count_temp = curr_file[0]->fragments[j].fragment_count;
		
		while (frag_start_addr_temp != 0) {
			fseek(file, frag_start_addr_temp, SEEK_SET);
			
			if (buff_size < (CLUSTER_SIZE * frag_count_temp)){
				read_bytes = fread(buff_pointer, sizeof(char), buff_size, file);	
			}
			else {
				read_bytes = fread(buff_pointer, sizeof(char), CLUSTER_SIZE * frag_count_temp, file);
			}
			
			buff_pointer += read_bytes;
			
			buff_size -= read_bytes;
			
			j++;
			
			frag_start_addr_temp = curr_file[0]->fragments[j].fragment_start_address;
			frag_count_temp = curr_file[0]->fragments[j].fragment_count;
		}
		
		/* uvoln�n� pam�ti po `curr_file' */
		if (curr_file != NULL) {
			free(curr_file);
			curr_file = NULL;
		}
	
		fclose(file);
		
		return buffer;
	}
	else {
		printf("Error in function get_data_from_file()\n");
		return NULL;
	}
}


/* ____________________________________________________________________________

    char **parse_path(char *input_param)
    
    Vstup, kter� je ulo�en v parametru `input_param', rozd�l� na cestu a jm�no
	souboru. Vrac� pole o dvou prvc�ch, kde na prvn� pozici je cesta a na 
	druh� pozici jm�no souboru.
   ____________________________________________________________________________
*/
char **parse_path(char *input_param) {
	
	/* odd�len� n�zvu od cesty */
	char *path = NULL;
	char *name = NULL;
	char **path_and_name = NULL;
	
	path_and_name = (char **) calloc(2, sizeof(char *));
	
	name = (char *) calloc(NAME_LEN, sizeof(char));
		
	char *first_char = input_param;
	char *input = input_param;
	
	char *last_occ = strrchr(input, '/');
	
	if (last_occ == NULL) {
		strcpy(name, first_char);
	}
	else {
		if (*(last_occ + 1) != '\0') {
			path = (char *) calloc(PATH_LEN, sizeof(char));
			
			last_occ++;
			strcpy(name, last_occ);
			*last_occ = '\0';
			strcpy(path, input);
		}
		else {
			*last_occ = '\0';
			last_occ = strrchr(input, '/');
			
			if (last_occ == NULL) {
				strcpy(name, first_char);
			}
			else {
				path = (char *) calloc(PATH_LEN, sizeof(char));
				
				last_occ++;
				strcpy(name, last_occ);
				*last_occ = '\0';
				strcpy(path, input);			
			}						
		}
	}
	
	path_and_name[0] = path;
	path_and_name[1] = name;
	
	return path_and_name;	
}


/* ____________________________________________________________________________

    void free_path_and_name(char **path_and_name)
    
    Uvoln� pam� po poli cesty a jm�na, kter� je p�ed�no v parametru 
	`path_and_name'.
   ____________________________________________________________________________
*/
void free_path_and_name(char **path_and_name) {
	
	if (path_and_name == NULL) {
		return;
	}
	
	/* uvoln�n� pam�ti po `path_and_name[0]' */
	if (path_and_name[0] != NULL) {
		free(path_and_name[0]);
		path_and_name[0] = NULL;
	}
	
	/* uvoln�n� pam�ti po `path_and_name[1]' */
	if (path_and_name[1] != NULL) {
		free(path_and_name[1]);
		path_and_name[1] = NULL;
	}
	
	/* uvoln�n� pam�ti po `path_and_name' */
	if (path_and_name != NULL) {
		free(path_and_name);
		path_and_name = NULL;
	}
}


/* ____________________________________________________________________________

    void set_default_mft_item(mft_item *curr_item)
    
    Nastav� MFT z�znam, kter� je p�edan� v parametru `curr_item' na hodnoty
    voln�ho z�znamu.
   ____________________________________________________________________________
*/
void set_default_mft_item(mft_item *curr_item) {
	
	int i;
	
	curr_item->uid = UID_ITEM_FREE;
	curr_item->type = -1;
	curr_item->item_order = 0;
	curr_item->item_order_total = 0;
	curr_item->item_size = 0;
	strcpy(curr_item->item_name, "");
	
	for (i = 0; i < MFT_FRAGMENTS_COUNT; i++) {
		curr_item->fragments[i].fragment_start_address = 0;
		curr_item->fragments[i].fragment_count = 0;
	}
}


/* ____________________________________________________________________________

    int32_t add_new_slink(int32_t uid_src_file, char *name_dest, char *file_name)
    
    Zkontroluje, zda existuje voln� MFT z�znam a zda je voln� m�sto v datov�
	oblasti a pokud ano, vytvo�� nov� symbolick� link. Jm�no linku je
	obsa�eno v parametru `name_dest', UID souboru, na kter� link odkazuje 
	v `uid_src_file'. Parametr `file_name' obsahuje n�zev souboru, kter� 
	reprezentuje disk. P�i �sp�chu vrac� UID nov�ho symbolick�ho linku,
	v opa�n�m p��pad� vrac� -1.
   ____________________________________________________________________________
*/
int32_t add_new_slink(int32_t uid_src_file, char *name_dest, char *file_name) {
	
	int i;
	
	/* po��te�n� adresa datov�ho bloku pro nov� symbolick� link */
	int32_t slink_start_address = -1;
	
	/* celkov� po�et MFT z�znam� */
	int mft_item_count;
	
	/* nalezen� voln� MFT z�znam */
	mft_item *found_mft_item = NULL;
	
	mft_item_count = (global_boot_record->bitmap_start_address - 
					global_boot_record->mft_start_address) / sizeof(mft_item);
	
	
	/* nalezen� voln�ho MFT z�znamu */
	for (i = 0; i < mft_item_count; i++) {
		if (global_mft_item_array[i]->uid == 0) {
			found_mft_item = global_mft_item_array[i];
			break;
		}
	}
	
	/* pokud ��dn� MFT z�znam nen� voln� */
	if (found_mft_item == NULL) {
		printf("MFT items are full.\n");
		return -1;
	}
	
	/* nalezen� voln�ho clusteru */
	for (i = 0; i < global_boot_record->cluster_count; i++) {
		if (global_bitmap[i] == 0) {
			global_bitmap[i] = 1;
			slink_start_address = global_boot_record->data_start_address + (i * CLUSTER_SIZE);
			break;
		}
	}
	
	/* pokud ��dn� cluster nen� voln� */
	if (slink_start_address == -1) {
		printf("Data block is full.\n");
		return -1;
	}
	
	/* zaps�n� hodnot nov�ho MFT z�znamu pro nov� symbolick� link */
	found_mft_item->uid = get_free_uid();	
	found_mft_item->type = 2;
	found_mft_item->item_order = 1;
	found_mft_item->item_order_total = 1;
	found_mft_item->item_size = SLINK_SIZE;
	strcpy(found_mft_item->item_name, name_dest);
	
	found_mft_item->fragments[0].fragment_start_address = slink_start_address;
	found_mft_item->fragments[0].fragment_count = 1;
	
	/* ulo�en� dat nov�ho symbolick�ho linku do datov� oblasti (pouze odkaz na odkazovan� soubor) */
	if (save_dir_data(slink_start_address, uid_src_file, file_name) == 0) {
		printf("Error during saving symbolic link data into data block.\n");
		return -1;
	}
	
	return found_mft_item->uid;	
}


/* ____________________________________________________________________________

    int32_t get_file_uid_from_slink(int32_t uid_slink, char *file_name)
    
    Z�sk� UID souboru, na kter� odkazuje symbolick� link s UID `uid_slink'.
	Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje disk. 
	P�i �sp�chu vrac� UID souboru, na kter� odkazuje symbolick� link, 
	v opa�n�m p��pad� vrac� -1.
   ____________________________________________________________________________
*/
int32_t get_file_uid_from_slink(int32_t uid_slink, char *file_name) {
	
	/* odkaz na symbolick� link - pole MFT z�znam� */
	mft_item **curr_slink = NULL;
	
	/* UID souboru, na kter� odkazuje symbolick� link */
	int32_t file_uid = -1;
	
	/* pole UID obsahu dan�ho symbolick�ho linku */
	int *slink_content;
	
	/* odkaz na soubor - pole MFT z�znam� */
	mft_item **curr_file = NULL;
	
	/* pomocn� prom�nn� pro n�vratovou hodnotu */
	int32_t return_value = -1;
	
	curr_slink = get_mft_item_by_uid(uid_slink);
	
	slink_content = load_dir_data(curr_slink[0]->fragments[0].fragment_start_address, file_name);
	
	file_uid = slink_content[0];
	
	curr_file = get_mft_item_by_uid(file_uid);
	
	if (curr_file != NULL) {
		return_value = file_uid;
	}
	else {
		return_value = -1;
	}
	
	/* uvoln�n� pam�ti po `slink_content' */
	if (slink_content != NULL) {
		free(slink_content);
		slink_content = NULL;
	}
	
	/* uvoln�n� pam�ti po `curr_slink' */
	if (curr_slink != NULL) {
		free(curr_slink);
		curr_slink = NULL;
	}
	
	/* uvoln�n� pam�ti po `curr_file' */
	if (curr_file != NULL) {
		free(curr_file);
		curr_file = NULL;
	}
	
	return return_value;
}