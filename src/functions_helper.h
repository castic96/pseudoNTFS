/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: functions_helper.h
    Detailní informace v souboru functions_helper.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _FUNCTIONS_HELPER_H
#define _FUNCTIONS_HELPER_H

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define CLUSTER_SIZE			1024	/* velikost clusteru v B */
#define MFT_FRAGMENTS_COUNT		32		/* poèet fragmentù v jednom MFT */
#define	PATH_LEN				512		/* maximální délka cesty ve znacích */
#define	NAME_LEN				12		/* délka jména ve znacích */
#define UID_ITEM_FREE			0		/* hodnota UID volného mft záznamu */
#define SLINK_SIZE				8		/* velikost symbolického linku */

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
mft_item **get_mft_item_by_uid(int32_t uid);
int *load_dir_data(int32_t start_address, char *path_name);
int32_t get_uid_from_path_dir(char *path, char *file_name);
int32_t get_uid_from_path_all(char *path, char *file_name);
int32_t get_uid_by_name_dir(char *dir_name, int32_t local_pwd, char *file_name);
int32_t get_uid_by_name_all(char *name, int32_t local_pwd, char *file_name);
int save_dir_data(int32_t address, int value, char *path_name);
int save_file_data(mft_item *found_mft_item, char *buffer, int32_t buff_size, int32_t num_of_clusters, char *file_name);
int32_t add_new_dir(char *dir_name, int32_t parent_dir_uid, char *file_name);
int write_global_vars_to_file(char *path_name);
int32_t get_free_uid();
int is_name_unique(char *name, int32_t dir_uid, char *file_name);
int32_t get_parent_from_uid(int32_t dir_uid, char *file_name);
int32_t add_new_file(char *buffer, int32_t buff_size, int32_t dir_uid_ntfs, char *name_ntfs, char *file_name);
int get_free_clust_num();
int *get_largest_continuous_clust(int32_t num_of_clusters);
int add_to_parent(int32_t parent_dir_uid, int32_t file_uid, char *file_name);
int remove_from_parent(int32_t uid_parent_dir, int32_t uid_file, char *file_name);
char *get_data_from_file(int32_t file_uid, char *file_name);
char **parse_path(char *input_param);
void free_path_and_name(char **path_and_name);
void set_default_mft_item(mft_item *curr_item);
int32_t add_new_slink(int32_t uid_src_file, char *name_dest, char *file_name);
int32_t get_file_uid_from_slink(int32_t uid_slink, char *file_name);

#endif