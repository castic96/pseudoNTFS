/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: file_manager.c
    Tento modul se stará o práci s datovım souborem pro uloení
    souborového systému pseudoNTFS. Stará se o jeho vytvoøení, uloení
    struktur, formátování atd.
    
    Dialekt: C11
    Pøekladaè: kadı kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "file_manager.h"
#include "fs_structures.h"

/* ____________________________________________________________________________

    int file_exists(char *path_name)
    
    Zjisuje, zda soubor se zadanou cestou `path_name' existuje.
    Pokud existuje, vrací 1, v opaèném pøípadì vrací 0.
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
    
    Zformátuje soubor na poèáteèní hodnoty. Pokud soubor neexistuje, vytvoøí 
	jej. Pøi úspìchu vrací 1, v opaèném pøípadì vrací 0.
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
    
    /* test naplnìní pole MFT záznamù
    for (i = 0; i < mft_item_count; i++) {
    	printf("UID: %d\n", curr_mft_item_arr[i]->uid);
    	printf("Type: %d\n", curr_mft_item_arr[i]->type);
    	printf("Name: %s\n", curr_mft_item_arr[i]->item_name);
    	printf("\n\n");
	}*/
	
	//uvolnit pamì po curr_mft_item_arr!!!
    
	/* test naplnìní bitmapy
	for (i = 0; i < curr_boot_rec->cluster_count; i++) {
		printf("%d ", curr_bitmap[i]);
	}*/
    
    /* uvolnìní pamìti po `curr_boot_rec' */
	if (curr_boot_rec != NULL) {
		free(curr_boot_rec);
		curr_boot_rec = NULL;
	}
	
	/* uvolnìní pamìti po `curr_mft_item_arr' */
	if (curr_mft_item_arr != NULL) {
		delete_mft_item_array(curr_mft_item_arr, mft_item_count);
		curr_mft_item_arr = NULL;
	}
	
	/* uvolnìní pamìti po `curr_bitmap' */
	if (curr_bitmap != NULL) {
		free(curr_bitmap);
		curr_bitmap = NULL;
	}
}


/* ____________________________________________________________________________

  
   ____________________________________________________________________________
*/
int load_file() {
	//zde naèíst struktury do globálním prom. - je jistota, e zde bude soubor reprezentující disk existovat
}


