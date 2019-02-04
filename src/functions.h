/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: functions.h
    Detailní informace v souboru functions.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define MFT_FRAGMENTS_COUNT		32		/* poèet fragmentù v jednom MFT */
#define BUFF_SIZE				1024	/* velikost pomocného bufferu */

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
void func_ls(int32_t dir_uid, char *file_name);
void func_pwd(int32_t local_pwd, char *file_name);
void func_mkdir(int32_t dir_uid, char *dir_name, char *file_name);
void func_cd(int32_t new_pwd_uid);
void func_incp(char *path_pc, int32_t dir_uid_ntfs, char *name_ntfs, char *file_name_ntfs);
void func_outcp(int32_t file_uid_ntfs, char *path_pc, char *file_name_ntfs);
void func_cat(int32_t file_uid, char *file_name);
void func_cp(int32_t uid_src_file, int32_t uid_dest_dir, char *name_dest, char *file_name);
void func_mv(int32_t uid_src_dir, int32_t uid_src_file, int32_t uid_dst_dir, char *name_dst, char *file_name);
void func_rm(int32_t uid_parent_dir, int32_t uid_file, char *file_name);
void func_rmdir(int32_t uid_parent, int32_t uid_dir, char *file_name);
void func_info(int32_t uid, char *file_name);
void func_load(char *path_pc, char *file_name);
void func_slink(int32_t uid_src_file, int32_t uid_dest_dir, char *name_dest, char *file_name);

#endif
