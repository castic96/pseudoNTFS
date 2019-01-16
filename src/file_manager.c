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
    
    Zform�tuje soubor na po��te�n� hodnoty. Pokud soubor neexistuje, vytvo�� 
	jej. P�i �sp�chu vrac� 1, v opa�n�m p��pad� vrac� 0.
   ____________________________________________________________________________
*/
int format_file(char *path_name, int size) {
	
	int i;
	int mft_item_count;
	int32_t data_start_address;
	
	boot_record *curr_boot_rec = NULL;
	mft_item **curr_mft_item_arr = NULL;
	int8_t *curr_bitmap = NULL;
	
	curr_boot_rec = init_boot_record(size);
	
	mft_item_count = (curr_boot_rec->bitmap_start_address - 
						curr_boot_rec->mft_start_address) / sizeof(mft_item);
	data_start_address = curr_boot_rec->data_start_address;
		
	curr_mft_item_arr = init_mft_item_array(mft_item_count, data_start_address);
	
	curr_bitmap = init_bitmap(curr_boot_rec->cluster_count);
	
    /* test boot recordu
    printf("----------- BOOT RECORD -----------\n");
    printf("Signature: \t\t%s\n", curr_boot_rec->signature);
    printf("Description: \t\t%s\n", curr_boot_rec->volume_descriptor);
    printf("Disk size: \t\t%dB\n", curr_boot_rec->disk_size);
    printf("Cluster count: \t\t%d\n", curr_boot_rec->cluster_count);
    printf("Cluster size: \t\t%dB\n", curr_boot_rec->cluster_size);
    printf("MFT start address: \t%d\n", curr_boot_rec->mft_start_address);
    printf("Bit map start address: \t%d\n", curr_boot_rec->bitmap_start_address);
    printf("Data start address: \t%d\n", curr_boot_rec->data_start_address);
    printf("MFT fragments count: \t%d\n", curr_boot_rec->mft_max_fragment_count);
    printf("-----------------------------------\n");
    */
    
    /* test napln�n� pole MFT z�znam�
    for (i = 0; i < mft_item_count; i++) {
    	printf("UID: %d\n", curr_mft_item_arr[i]->uid);
    	printf("Type: %d\n", curr_mft_item_arr[i]->type);
    	printf("Name: %s\n", curr_mft_item_arr[i]->item_name);
    	printf("\n\n");
	}*/
	
	//uvolnit pam� po curr_mft_item_arr!!!
    
	/* test napln�n� bitmapy
	for (i = 0; i < curr_boot_rec->cluster_count; i++) {
		printf("%d ", curr_bitmap[i]);
	}*/
    
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
}


/* ____________________________________________________________________________

  
   ____________________________________________________________________________
*/
int load_file() {
	//zde na��st struktury do glob�ln�m prom. - je jistota, �e zde bude soubor reprezentuj�c� disk existovat
}


