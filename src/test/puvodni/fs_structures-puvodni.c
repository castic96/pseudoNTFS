/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: fs_structures.c
    Tento modul slou�� k ulo�en� struktur pot�ebn�ch pro 
    souborov� syst�m NTFS a z�kladn� operace s nimi.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "fs_structures.h"

/* ____________________________________________________________________________

	boot_record *init_boot_record(int size)
    
    Inicializuje boot record s po��te�n�mi hodnotami podle zadan� velikosti
    `size'. Takov� boot record vr�t�.
   ____________________________________________________________________________
*/
boot_record *init_boot_record(int size_mb) {
	
	boot_record *new_boot_rec = (boot_record *) calloc(1, sizeof(boot_record));    
    
    /* celkov� velikost v B */
    int size_b = size_mb * MB_TO_B_CONST;     
    
    /* pomocn� prom�nn� pro v�po�et velikosti mft oblasti */
    double temp_mft_size = size_b * 0.1 / sizeof(mft_item);
    
	/* velikost mft oblasti - cca 10% z celkov� velikosti */
    int mft_size = (int) (ceil(temp_mft_size) * sizeof(mft_item));
    
    /* celkov� po�et cluster� - datov� ��st je cca 90% zbytku */
    int cluster_count = (int) ceil((size_b - mft_size - sizeof(boot_record)) 
							* (0.9 / CLUSTER_SIZE));     
    int bitmap_size = cluster_count * sizeof(int8_t);
	
	strcpy(new_boot_rec->signature, "zcastora");
	strcpy(new_boot_rec->volume_descriptor, "myNTFS 2018-2019");
	new_boot_rec->mft_start_address = sizeof(boot_record);
	new_boot_rec->bitmap_start_address = sizeof(boot_record) + mft_size;
	new_boot_rec->disk_size = CLUSTER_SIZE * cluster_count; // + mft_size + sizeof(boot_record) + bitmap_size ???
	new_boot_rec->cluster_size = CLUSTER_SIZE;
	new_boot_rec->cluster_count = cluster_count;
	new_boot_rec->data_start_address = new_boot_rec->bitmap_start_address 
										+ bitmap_size;
	new_boot_rec->mft_max_fragment_count = MFT_FRAGMENTS_COUNT;    

	return new_boot_rec;
}


/* ____________________________________________________________________________

	mft_item **init_mft_item_array(int mft_item_count, int32_t data_start_address)
    
    Inicializuje pole MFT polo�ek. Po�et MFT polo�ek (velikost pole) je d�n
	parametrem `mft_item_count'. Parametr `data_start_address' obsahuje prvn� 
	adresu datov� oblasti disku. Prvn� z�znam MFT pole obsahuje adres�� root, 
	dal�� z�znamy jsou voln�. Fiunkce vrac� inicializovan� pole MFT polo�ek.
   ____________________________________________________________________________
*/
mft_item **init_mft_item_array(int mft_item_count, int32_t data_start_address) {
	
	int i;	
	
	mft_item **new_mft_item_arr = (mft_item **) calloc(mft_item_count, sizeof(mft_item *));
	
	new_mft_item_arr[0] = init_mft_item_root(data_start_address);
	
	for (i = 1; i < mft_item_count; i++) {
		new_mft_item_arr[i] = init_mft_item_free();
	}
	
	return new_mft_item_arr;
}


/* ____________________________________________________________________________

	mft_item *init_mft_item_root(int32_t data_start_address)
    
    Inicializuje MFT polo�ku pro adres�� root a do prvn�ho fragmentu ulo��
	adresu po��tku datov� oblasti disku `data_start_address', kde je ulo�en
	odkaz na sebe sam�ho. Inicializovanou MFT polo�ku vr�t�.
   ____________________________________________________________________________
*/
mft_item *init_mft_item_root(int32_t data_start_address) {
	
	mft_item *new_mft_item_root = (mft_item *) calloc(1, sizeof(mft_item));
	
	new_mft_item_root->uid = UID_ITEM_ROOT;
	new_mft_item_root->type = 1;
	new_mft_item_root->item_order = 1;
	new_mft_item_root->item_order_total = 1;
	new_mft_item_root->item_size = CLUSTER_SIZE;
	strcpy(new_mft_item_root->item_name, "root");
	
	new_mft_item_root->fragments[0].fragment_start_address = data_start_address;
	new_mft_item_root->fragments[0].fragment_count = 1;
	
	return new_mft_item_root;
}


/* ____________________________________________________________________________

	mft_item *init_mft_item_free()
    
    Inicializuje volnou MFT polo�ku, tu n�sledn� vr�t�.
   ____________________________________________________________________________
*/
mft_item *init_mft_item_free() {
	
	mft_item *new_mft_item_free = (mft_item *) calloc(1, sizeof(mft_item));
	
	new_mft_item_free->uid = UID_ITEM_FREE;
	new_mft_item_free->type = -1;
	new_mft_item_free->item_order = 0;
	new_mft_item_free->item_order_total = 0;
	new_mft_item_free->item_size = 0;
	strcpy(new_mft_item_free->item_name, "");
	
	return new_mft_item_free;
}


/* ____________________________________________________________________________

	void delete_mft_item_array(mft_item **curr_mft_item_arr, int mft_item_count)
    
    Uvoln� pam� po poli MFT polo�ek, kter� je p�ed�no v parametru
	`curr_mft_item_arr'. Po�et polo�ek je v parametru `mft_item_count'.
   ____________________________________________________________________________
*/
void delete_mft_item_array(mft_item **curr_mft_item_arr, int mft_item_count) {
	
	int i;
	
	for (i = 0; i < mft_item_count; i++) {
		free(curr_mft_item_arr[i]);
		curr_mft_item_arr[i] = NULL;
	}
	
	free(curr_mft_item_arr);
	curr_mft_item_arr = NULL;
}


/* ____________________________________________________________________________

	int8_t *init_bitmap(int cluster_count)
    
    Inicializuje bitmapu na po��te�n� hodnoty podle po�tu cluster� disku 
    `cluster_count'. Inicializovanou bitmapu vr�t�.
   ____________________________________________________________________________
*/
int8_t *init_bitmap(int cluster_count) {
	
	int i;
	
	int8_t *new_bitmap = (int8_t *) calloc(cluster_count, sizeof(int8_t));
	
	new_bitmap[0] = 1;
	
	for (i = 1; i < cluster_count; i++) {
		new_bitmap[i] = BITMAP_FREE;
	}
	
	return new_bitmap;
}


/* ____________________________________________________________________________

	boot_record *calloc_boot_record()
    
    Alokuje m�sto v pam�ti pro strukturu boot_record. Vr�t� ukazatel na 
	po��tek alokovan� oblasti.
   ____________________________________________________________________________
*/
boot_record *calloc_boot_record() {
	
	boot_record *new_boot_rec = (boot_record *) calloc(1, sizeof(boot_record));

	return new_boot_rec;
}


/* ____________________________________________________________________________

	mft_item **calloc_mft_item_array(int mft_item_count)
    
    Alokuje m�sto v pam�ti pro pole obsahuj�c� ukazatele na strukturu mft_item. 
	Vr�t� ukazatel na po��tek alokovan� oblasti.
   ____________________________________________________________________________
*/
mft_item **calloc_mft_item_array(int mft_item_count) {
	
	int i;	
	
	mft_item **new_mft_item_arr = (mft_item **) calloc(mft_item_count, sizeof(mft_item *));
	
	for (i = 0; i < mft_item_count; i++) {
		new_mft_item_arr[i] = (mft_item *) calloc(1, sizeof(mft_item));
	}
	
	return new_mft_item_arr;
}


/* ____________________________________________________________________________

	int8_t *calloc_bitmap(int cluster_count) 
    
    Alokuje m�sto v pam�ti pro bitmapu. Vr�t� ukazatel na po��tek alokovan� 
	oblasti.
   ____________________________________________________________________________
*/
int8_t *calloc_bitmap(int cluster_count) {
	
	int8_t *new_bitmap = (int8_t *) calloc(cluster_count, sizeof(int8_t));
	
	return new_bitmap;
}
