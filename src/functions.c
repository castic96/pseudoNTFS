/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: functions.c
    Tento modul obsahuje funkce pro vykon�v�n� jednotliv�ch
    p��kaz� shellu.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "command.h"
#include "functions.h"
#include "fs_structures.h"
#include "functions_helper.h"
#include "global_vars.h"
#include "process_command.h"

/* ____________________________________________________________________________

	void func_ls(int32_t dir_uid, char *file_name)
    
    Zajist� vyps�n� obsahu adres��e zadan�ho parametrem `dir_uid'. Parametr 
	`file_name' obsahuje n�zev souboru, kter� reprezentuje disk.
   ____________________________________________________________________________
*/
void func_ls(int32_t dir_uid, char *file_name) {
	
	int i, j;
	
	/* po�et MFT z�znam� pro jednu polo�ku */
	int mft_count;
	
	/* pole UID obsahu dan�ho adres��e */
	int *dir_content;
	
	/* pomocn� prom�nn� pro za��te�n� adresu, na kterou odkazuje fragment */
	int32_t temp_fragment_start_address;
	
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_dir = NULL;
	
	/* pomocn� prom�nn� pro jednu MFT polo�ku */
	mft_item *mft_temp = NULL;
	
	/* pomocn� prom�nn� pro pole MFT polo�ek */
	mft_item **mft_temp2 = NULL;
	
	/* aktu�ln� hledan� po�ad� MFT polo�ky */
	int search_item_order = 1;
	
	curr_dir = get_mft_item_by_uid(dir_uid);
	
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
					mft_temp2 = get_mft_item_by_uid(dir_content[j]);
				
					/* pokud se nejedn� o prvn� adresu prvn�ho fragmentu prvn� MFT polo�ky */
					/* jednalo by se o tisknut� odkazu na nad�azen� adres�� */
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
}


/* ____________________________________________________________________________

	void func_pwd(int32_t local_pwd, char *file_name)
    
    Vyp�e absolutn� cestu z adres��e, jej� UID je v parametru `local_pwd'. 
	Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje disk.
   ____________________________________________________________________________
*/
void func_pwd(int32_t local_pwd, char *file_name) {
	
	int i, j;
	
	/* pole ukazatel� na n�zvy adres��u na cest� */
	char *name_pointer_handler[BUFF_SIZE];
	
	/* pomocn� ukazatel na jm�no adres��e */
	char *name_temp;
	
	/* buffer pro slo�en� n�zvu hledan� cesty */
	char buffer[BUFF_SIZE];
	
	/* pomocn� ukazatel na buffer hledan� cesty */
	char *buffer_temp;
	
	/* pomocn� prom�nn� pro local_pwd */
	int32_t temp_pwd = local_pwd;
	
	/* po�et MFT z�znam� pro jednu polo�ku */
	int mft_count;
	
	/* pole UID obsahu dan� slo�ky */
	int *dir_content;
	
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_dir = NULL;
	
	/* pomocn� prom�nn� pro jednu MFT polo�ku */
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


/* ____________________________________________________________________________

	void func_mkdir(int32_t dir_uid, char *dir_name, char *file_name)
    
    Zajist� vytvo�en� adres��e v adres��i, kter� je zadan� parametrem 
	`dir_uid'. Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje 
	disk.
   ____________________________________________________________________________
*/
void func_mkdir(int32_t parent_dir_uid, char *dir_name, char *file_name) {
	
	/* UID nov� vytvo�en�ho adres��e */
	int32_t dir_uid = -1;
	
	dir_uid = add_new_dir(dir_name, parent_dir_uid, file_name);
	
	if (dir_uid == -1) {
		printf("ERROR.\n");
		return;
	}
	
	if (add_to_parent(parent_dir_uid, dir_uid, file_name) == 0) {
		printf("Error during saving directory data into data block.\n");
		return;
	}
	
	write_global_vars_to_file(file_name);
	
	printf("OK\n");
}


/* ____________________________________________________________________________

	void func_cd(int32_t new_pwd_uid)
    
    Zm�n� glob�ln� cestu (pwd) do adres��e, jeho� UID je zadan� v parametru 
	`new_pwd_uid'.
   ____________________________________________________________________________
*/
void func_cd(int32_t new_pwd_uid) {
	pwd = new_pwd_uid;
	
	printf("OK\n");
}


/* ____________________________________________________________________________

	void func_incp(char *path_pc, int32_t dir_uid_ntfs, char *name_ntfs, char *file_name_ntfs)
    
    Nahraje soubor dan� parametrem `path_pc' z pevn�ho disku do adres��e s UID
    `dir_uid_ntfs' souborov�ho syst�mu pseudoNTFS a nov�mu souboru p�i�ad�
    n�zev `name_ntfs'. Parametr `file_name_ntfs' obsahuje n�zev souboru, kter� 
	reprezentuje disk pseudoNTFS.
   ____________________________________________________________________________
*/
void func_incp(char *path_pc, int32_t dir_uid_ntfs, char *name_ntfs, char *file_name_ntfs) {
	
	/* handler na soubor */
	FILE *file = NULL;
	
	/* velikost souboru na pevn�m disku */
	int32_t file_size = -1;
	
	/* buffer pro na�ten� data */
	char *buffer = NULL;
	
	/* UID nov� vytvo�en�ho souboru */
	int32_t file_uid = -1;
	
	file = fopen(path_pc, "r+b");
	
	if (file != NULL) {
		
		/* zji�t�n� velikosti souboru */
		fseek(file, 0, SEEK_END);
		file_size = (int32_t) ftell(file);
		fseek(file, 0, SEEK_SET);
		
		/* na�ten� dat ze souboru pevn�ho disku do bufferu */
		buffer = (char *) calloc(file_size, sizeof(char));
		fread(buffer, sizeof(char), file_size, file);
		
		file_uid = add_new_file(buffer, file_size, dir_uid_ntfs, name_ntfs, file_name_ntfs);
		
		if (file_uid == -1) {
			printf("ERROR\n");
			return;
		}
		
		/* uvoln�n� pam�ti po `buffer' */
		if (buffer != NULL) {
			free(buffer);
			buffer = NULL;
		}
		
		fclose(file);
		
		if (add_to_parent(dir_uid_ntfs, file_uid, file_name_ntfs) == 0) {
			printf("Error during saving link into data block of master directory.\n");
			return;
		}
		
		write_global_vars_to_file(file_name_ntfs);
		
		printf("OK\n");
	}
	else {
		printf("FILE NOT FOUND\n");
	}
}


/* ____________________________________________________________________________

	void func_outcp(int32_t file_uid_ntfs, char *path_pc, char *file_name_ntfs)
    
    Nahraje soubor s UID `file_uid_ntfs' ze souborov�ho syst�mu pseudoNTFS do
	um�st�n� na pevn�m disku, kter� je d�no parametrem `path_pc'. Parametr 
	`file_name_ntfs' obsahuje n�zev souboru, kter� reprezentuje 
	disk pseudoNTFS.
   ____________________________________________________________________________
*/
void func_outcp(int32_t file_uid_ntfs, char *path_pc, char *file_name_ntfs) {
	
	/* handler na soubor */
	FILE *file = NULL;
	
	/* buffer pro na�ten� data */
	char *buffer = NULL;
	
	/* velikost bufferu pro na�ten� data */
	int32_t buff_size;
	
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_item = NULL;
	
	file = fopen(path_pc, "wb");
	
	if (file != NULL) {
		curr_item = get_mft_item_by_uid(file_uid_ntfs);
		
		buff_size = curr_item[0]->item_size;
		
		buffer = get_data_from_file(file_uid_ntfs, file_name_ntfs);
		
		if (buffer == NULL) {
			return;
		}
		
		fwrite(buffer, sizeof(char), buff_size, file);
		
		/* uvoln�n� pam�ti po `buffer' */
		if (buffer != NULL) {
			free(buffer);
			buffer = NULL;
		}
		
		/* uvoln�n� pam�ti po `curr_item' */
		if (curr_item != NULL) {
			free(curr_item);
			curr_item = NULL;
		}
		
		fclose(file);
		
		printf("OK\n");
	}
	else {
		printf("PATH NOT FOUND\n");
		return;
	}
}


/* ____________________________________________________________________________

	void func_cat(int32_t file_uid, char *file_name)
    
    Vyp�e obsah souboru s UID `file_uid' ze souborov�ho syst�mu pseudoNTFS.
	Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje disk 
	pseudoNTFS.
   ____________________________________________________________________________
*/
void func_cat(int32_t file_uid, char *file_name) {
	
	/* buffer pro na�ten� data */
	char *buff_from_data = NULL;
	
	/* buffer pro data pro tisk na konzoli */
	char *buff_to_print = NULL;
	
	/* velikost bufferu pro na�ten� data */
	int32_t buff_from_data_size;
	
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_item = NULL;
	
	curr_item = get_mft_item_by_uid(file_uid);
	
	buff_from_data_size = curr_item[0]->item_size;
	
	buff_from_data = get_data_from_file(file_uid, file_name);
	
	if (buff_from_data == NULL) {
		return;
	}
	
	buff_to_print = (char *) calloc(buff_from_data_size + 1, sizeof(char));
	
	strncpy(buff_to_print, buff_from_data, buff_from_data_size);
	
	printf("%s\n", buff_to_print);
	
	/* uvoln�n� pam�ti po `buff_from_data' */
	if (buff_from_data != NULL) {
		free(buff_from_data);
		buff_from_data = NULL;
	}
	
	/* uvoln�n� pam�ti po `buff_to_print' */
	if (buff_to_print != NULL) {
		free(buff_to_print);
		buff_to_print = NULL;
	}
	
	/* uvoln�n� pam�ti po `curr_item' */
	if (curr_item != NULL) {
		free(curr_item);
		curr_item = NULL;
	}
}


/* ____________________________________________________________________________

	void func_cp(int32_t uid_src_file, int32_t uid_dest_dir, char *name_dest, char *file_name)
    
    Zkop�ruje soubor s UID `uid_source_file' do um�st�n� dan�ho adres��em
	`uid_dest_dir' a souboru p�i�ad� n�zev `name_dest'. Parametr `file_name' 
	obsahuje n�zev souboru, kter� reprezentuje disk pseudoNTFS.
   ____________________________________________________________________________
*/
void func_cp(int32_t uid_src_file, int32_t uid_dest_dir, char *name_dest, char *file_name) {
	
	/* buffer pro na�ten� data */
	char *buffer = NULL;
	
	/* velikost bufferu pro na�ten� data */
	int32_t buff_size;
	
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_item = NULL;
	
	/* UID nov� vytvo�en�ho souboru */
	int32_t uid_new_file = -1;
	
	
	curr_item = get_mft_item_by_uid(uid_src_file);
		
	buff_size = curr_item[0]->item_size;
	buffer = get_data_from_file(uid_src_file, file_name);
		
	if (buffer == NULL) {
		return;
	}
	
	uid_new_file = add_new_file(buffer, buff_size, uid_dest_dir, name_dest, file_name);
	
	if (uid_new_file == -1) {
		return;
	}
	
	/* uvoln�n� pam�ti po `buffer' */
	if (buffer != NULL) {
		free(buffer);
		buffer = NULL;
	}
	
	/* uvoln�n� pam�ti po `curr_item' */
	if (curr_item != NULL) {
		free(curr_item);
		curr_item = NULL;
	}
	
	if (add_to_parent(uid_dest_dir, uid_new_file, file_name) == 0) {
		printf("Error during saving link into data block of master directory.\n");
		return;
	}
		
	write_global_vars_to_file(file_name);
	
	printf("OK\n");
}


/* ____________________________________________________________________________

	void func_mv(int32_t uid_src_dir, int32_t uid_src_file, int32_t uid_dst_dir, char *name_dst, char *file_name)
    
    P�esune soubor s UID `uid_src_file' ze slo�ky s UID `uid_src_dir' 
	do um�st�n� dan�ho adres��em `uid_dst_dir' a souboru p�i�ad� n�zev 
	`name_dst'. Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje 
	disk pseudoNTFS.
   ____________________________________________________________________________
*/
void func_mv(int32_t uid_src_dir, int32_t uid_src_file, int32_t uid_dst_dir, char *name_dst, char *file_name) {
	
	/* odkaz na zdrojov� soubor - pole MFT z�znam� */
	mft_item **src_file = NULL;
	
	if (remove_from_parent(uid_src_dir, uid_src_file, file_name) == 0) {
		printf("Error during removing link from data block of master directory.\n");
		return;
	}
	
	src_file = get_mft_item_by_uid(uid_src_file);
	
	strcpy(src_file[0]->item_name, name_dst);
	
	/* uvoln�n� pam�ti po `src_file' */
	if (src_file != NULL) {
		free(src_file);
		src_file = NULL;
	}
	
	if (add_to_parent(uid_dst_dir, uid_src_file, file_name) == 0) {
		printf("Error during saving link into data block of master directory.\n");
		return;
	}
		
	write_global_vars_to_file(file_name);
	
	printf("OK\n");
}


/* ____________________________________________________________________________

	void func_rm(int32_t uid_parent_dir, int32_t uid_file, char *file_name)
    
    Odtran� soubor s UID `uid_file' ze slo�ky s UID `uid_parent_dir'.
	Parametr `file_name' obsahuje n�zev souboru, kter� reprezentuje disk 
	pseudoNTFS.
   ____________________________________________________________________________
*/
void func_rm(int32_t uid_parent_dir, int32_t uid_file, char *file_name) {
	
	int i, j;
	
	int bm_index = -1;
	
	/* odkaz na soubor - pole MFT z�znam� */
	mft_item **curr_file = NULL;
	
	/* pomocn� prom�nn� pro za��te�n� adresu, na kterou odkazuje fragment */
	int32_t temp_frag_start_addr = -1;
	
	/* pomocn� prom�nn� pro po�et cluster� dan�ho fragmentu */
	int32_t temp_frag_count = 0;
	
	curr_file = get_mft_item_by_uid(uid_file);
	
	if (remove_from_parent(uid_parent_dir, uid_file, file_name) == 0) {
		printf("Error during removing link from data block of master directory.\n");
		return;
	}
	
	/* nastaven� p��slu�n�ch pozic bitmapy na 0 - volno */
	for (i = 0; i < MFT_FRAGMENTS_COUNT; i++) {
		temp_frag_start_addr = curr_file[0]->fragments[i].fragment_start_address;
		temp_frag_count = curr_file[0]->fragments[i].fragment_count;
				
		if (temp_frag_start_addr != 0) {
			bm_index = (temp_frag_start_addr - global_boot_record->data_start_address) / CLUSTER_SIZE;
			
			for (j = 0; j < temp_frag_count; j++) {
				global_bitmap[bm_index] = 0;
				bm_index++;
			}
		}
		else {
			break;
		}
	}
	
	set_default_mft_item(curr_file[0]);
	
	/* uvoln�n� pam�ti po `curr_file' */
	if (curr_file != NULL) {
		free(curr_file);
		curr_file = NULL;
	}
	
	write_global_vars_to_file(file_name);
	
	printf("OK\n");
}


/* ____________________________________________________________________________

	void func_rmdir(int32_t uid_parent, int32_t uid_dir, char *file_name)
    
    Odstran� adres�� s UID `uid_dir' za p�edpokladu, �e je pr�zdn�. Nad�azen�
	adres�� m� UID `uid_parent'. Parametr `file_name' obsahuje n�zev souboru, 
	kter� reprezentuje disk pseudoNTFS.
   ____________________________________________________________________________
*/
void func_rmdir(int32_t uid_parent, int32_t uid_dir, char *file_name) {
	
	int i, j;
	
	int bm_index = -1;
	
	/* odkaz na adres�� - pole MFT z�znam� */
	mft_item **curr_dir = NULL;

	/* pole UID obsahu dan� slo�ky */
	int *dir_content;

	/* pomocn� prom�nn� pro za��te�n� adresu, na kterou odkazuje fragment */
	int32_t temp_frag_start_addr = -1;
	
	/* pomocn� prom�nn� pro po�et cluster� dan�ho fragmentu */
	int32_t temp_frag_count = 0;
	
	curr_dir = get_mft_item_by_uid(uid_dir);
	
	dir_content = load_dir_data(curr_dir[0]->fragments[0].fragment_start_address, file_name);
		
	if (dir_content == NULL) {
		return;
	}
	
	if ((dir_content[0] != -1) && (dir_content[1] == -1)) {
		if (remove_from_parent(uid_parent, uid_dir, file_name) == 0) {
			printf("Error during removing link from data block of master directory.\n");
			return;
		}
		
		/* nastaven� p��slu�n�ch pozic bitmapy na 0 - volno */
		for (i = 0; i < MFT_FRAGMENTS_COUNT; i++) {
			temp_frag_start_addr = curr_dir[0]->fragments[i].fragment_start_address;
			temp_frag_count = curr_dir[0]->fragments[i].fragment_count;
				
			if (temp_frag_start_addr != 0) {
				bm_index = (temp_frag_start_addr - global_boot_record->data_start_address) / CLUSTER_SIZE;
			
				for (j = 0; j < temp_frag_count; j++) {
					global_bitmap[bm_index] = 0;
					bm_index++;
				}
			}
			else {
				break;
			}
		}
		
		set_default_mft_item(curr_dir[0]);
		
		write_global_vars_to_file(file_name);
		
		printf("OK\n");
	}
	else {
		printf("NOT EMPTY\n");
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
}


/* ____________________________________________________________________________

	void func_info(int32_t uid, char *file_name)
    
    Vyp�e informace o souboru/adres��i s UID `uid'. Parametr `file_name' 
	obsahuje n�zev souboru, kter� reprezentuje disk pseudoNTFS.
   ____________________________________________________________________________
*/
void func_info(int32_t uid, char *file_name) {
	
	int i;
	
	/* odkaz na polo�ku - pole MFT z�znam� */
	mft_item **curr_item = NULL;
	
	/* pomocn� prom�nn� pro jednu MFT polo�ku */
	mft_item *mft_temp = NULL;
	
	/* po�et cluster� */
	int num_of_clusters = -1;
	
	/* po�et fragment� */
	int num_of_fragments = 0;
	
	curr_item = get_mft_item_by_uid(uid);
	
	mft_temp = curr_item[0];
	
	/* zji�t�n� po�tu fragment� */
    for (i = 0; i < MFT_FRAGMENTS_COUNT; i++) {
        if (mft_temp->fragments[i].fragment_start_address != 0){
            num_of_fragments++;
        }
    }
    
	/* zji�t�n� po�tu cluster� */
	if (mft_temp->item_size % CLUSTER_SIZE == 0) {
		num_of_clusters = (int) mft_temp->item_size / CLUSTER_SIZE;
    }
	else {
		num_of_clusters = (int) (mft_temp->item_size / CLUSTER_SIZE) + 1;
	}
	
	if (mft_temp->type == 0) {
		printf("NAME: %s; TYPE: file; UID: %d; SIZE: %d; FRAGMENTS COUNT: %d; CLUSTERS COUNT: %d\n", 
			mft_temp->item_name, mft_temp->uid, mft_temp->item_size, num_of_fragments, num_of_clusters);
	}
	else if (mft_temp->type == 1) {
		printf("NAME: %s; TYPE: directory; UID: %d; SIZE: %d; FRAGMENTS COUNT: %d; CLUSTERS COUNT: %d\n", 
			mft_temp->item_name, mft_temp->uid, mft_temp->item_size, num_of_fragments, num_of_clusters);
	}
	else {
		printf("NAME: %s; TYPE: symbolic link; UID: %d; SIZE: %d; FRAGMENTS COUNT: %d; CLUSTERS COUNT: %d\n", 
			mft_temp->item_name, mft_temp->uid, mft_temp->item_size, num_of_fragments, num_of_clusters);
	}
	
	/* uvoln�n� pam�ti po `curr_dir' */
	if (curr_item != NULL) {
		free(curr_item);
		curr_item = NULL;
	}
}


/* ____________________________________________________________________________

	void func_load(char *path_pc, char *file_name)
    
    Na�te soubor, kter� le�� v cest� `path_pc' z pevn�ho disku. V tomto souboru
	jsou ulo�eny jednotliv� p��kazy, funkce je sekven�n� vykon�. Parametr 
	`file_name' obsahuje n�zev souboru, kter� reprezentuje disk pseudoNTFS.
   ____________________________________________________________________________
*/
void func_load(char *path_pc, char *file_name) {
	
	/* handler na soubor */
	FILE *file = NULL;
	
	/* buffer jedn� ��dky */
	char line_buff[BUFF_SIZE];
	
	/* aktu�ln� p��kaz */	
	command *current_command = NULL;
	
	/* index posledn�ho znaku v bufferu ��dky */
	int last_char_num = - 1;
	
	file = fopen(path_pc, "r");
	
	if (file != NULL) {
		memset(line_buff, 0, BUFF_SIZE * sizeof(char));
		
		while (fgets(line_buff, BUFF_SIZE, file) != NULL) {
			
			/* p�esko�en� pr�zdn� ��dky nebo koment��e */
			if ((*line_buff == '\n') || (*line_buff == '#')) {
				memset(line_buff, 0, BUFF_SIZE * sizeof(char));
				continue;
			}
			
			/* odstran�n� konce ��dky */
			last_char_num = strlen(line_buff) - 1;
			if (line_buff[last_char_num] == '\n') {
				line_buff[last_char_num] = '\0';
			}
			
			printf("%s\n", line_buff);
			
			current_command = new_command(line_buff);
			
			process_requirement_after(current_command, file_name);
			
			/* uvoln�n� pam�ti po `current_command' */
			if (current_command != NULL) {
				free(current_command);
				current_command = NULL;
			}
			
			memset(line_buff, 0, BUFF_SIZE * sizeof(char));
		}
		
		fclose(file);
	}
	else {
		printf("FILE NOT FOUND\n");
	}
}


/* ____________________________________________________________________________

	void func_slink(int32_t uid_src_file, int32_t uid_dest_dir, char *name_dest, char *file_name)
    
    Vytvo�� symbolick� link na soubor, kter� m� UID p�edan� v parametru 
	`uid_src_file'. Parametr `uid_dest_dir' obsahuje UID slo�ky, ve kter� 
	se bude dan� symbolick� link nach�zet. Parametr `file_name' obsahuje n�zev 
	souboru, kter� reprezentuje disk pseudoNTFS.
   ____________________________________________________________________________
*/
void func_slink(int32_t uid_src_file, int32_t uid_dest_dir, char *name_dest, char *file_name) {
	
	/* UID nov� vytvo�en�ho symbolick�ho linku */
	int32_t uid_new_slink = -1;
	
	uid_new_slink = add_new_slink(uid_src_file, name_dest, file_name);
	
	if (uid_new_slink == -1) {
		return;
	}
	
	if (add_to_parent(uid_dest_dir, uid_new_slink, file_name) == 0) {
		printf("Error during saving link into data block of master directory.\n");
		return;
	}
		
	write_global_vars_to_file(file_name);
	
	printf("OK\n");
}
