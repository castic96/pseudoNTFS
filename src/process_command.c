/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: process_command.c
    Tento modul slouží ke zpracování jednotlivých pøíkazù 
    a volání pøíslušných funkcí pro vykonání požadované
    funkcionality zadaného pøíkazu.
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "command.h"
#include "file_manager.h"
#include "process_command.h"
#include "fs_structures.h"
#include "global_vars.h"
#include "functions_helper.h"
#include "functions.h"

/* ____________________________________________________________________________

	int process_requirement_before(command *current_command, char *file_name)
    
    Zpracování pøíkazu pøed prvním naformátováním souboru. Vrací 1, pokud
    je pøíkaz validní, v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_requirement_before(command *current_command, char *file_name) {
	
	if (current_command->function == NULL) {
		printf("Unknown command!\n");
		return 0;
	}
	
	if (strcmp("format", current_command->function) == 0) {
		if (process_command_format(current_command, file_name) != 1) {
			printf("Error in function process_command_format()\n");
			return 0;
		}
	}
	else {
		printf("Unknown command!\n");
		return 0;
	}
	
	return 1;
}


/* ____________________________________________________________________________

	int process_requirement_after(command *current_command, char *file_name)
    
    Zpracování pøíkazu po prvním naformátováním souboru. Vrací 1, pokud
    je pøíkaz validní, v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_requirement_after(command *current_command, char *file_name) {
	
	if (current_command->function == NULL) {
		printf("Unknown command!\n");
		return 0;
	}
	
	if (strcmp("format", current_command->function) == 0) {
		if (process_command_format(current_command, file_name) != 1) {
			printf("Error in function process_command_format()\n");
			return 0;
		}
	}
	else if (strcmp("ls", current_command->function) == 0) {
		if (process_command_ls(current_command, file_name) != 1) {
			printf("Error in function process_command_ls()\n");
			return 0;
		}
	}
	else if (strcmp("pwd", current_command->function) == 0) {
		if (process_command_pwd(current_command, file_name) != 1) {
			printf("Error in function process_command_pwd()\n");
			return 0;
		}
	}
	else if (strcmp("cd", current_command->function) == 0) {
		if (process_command_cd(current_command, file_name) != 1) {
			printf("Error in function process_command_cd()\n");
			return 0;
		}
	}
	else if (strcmp("mkdir", current_command->function) == 0) {
		if (process_command_mkdir(current_command, file_name) != 1) {
			printf("Error in function process_command_mkdir()\n");
			return 0;
		}
	}
	else if (strcmp("incp", current_command->function) == 0) {
		if (process_command_incp(current_command, file_name) != 1) {
			printf("Error in function process_command_incp()\n");
			return 0;
		}
	}
	else if (strcmp("outcp", current_command->function) == 0) {
		if (process_command_outcp(current_command, file_name) != 1) {
			printf("Error in function process_command_outcp()\n");
			return 0;
		}
	}
	else if (strcmp("cat", current_command->function) == 0) {
		if (process_command_cat(current_command, file_name) != 1) {
			printf("Error in function process_command_cat()\n");
			return 0;
		}
	}
	else if (strcmp("cp", current_command->function) == 0) {
		if (process_command_cp(current_command, file_name) != 1) {
			printf("Error in function process_command_cp()\n");
			return 0;
		}
	}
	else if (strcmp("mv", current_command->function) == 0) {
		if (process_command_mv(current_command, file_name) != 1) {
			printf("Error in function process_command_mv()\n");
			return 0;
		}
	}
	else if (strcmp("rm", current_command->function) == 0) {
		if (process_command_rm(current_command, file_name) != 1) {
			printf("Error in function process_command_rm()\n");
			return 0;
		}
	}
	else if (strcmp("rmdir", current_command->function) == 0) {
		if (process_command_rmdir(current_command, file_name) != 1) {
			printf("Error in function process_command_rmdir()\n");
			return 0;
		}
	}
	else if (strcmp("info", current_command->function) == 0) {
		if (process_command_info(current_command, file_name) != 1) {
			printf("Error in function process_command_info()\n");
			return 0;
		}
	}
	else if (strcmp("load", current_command->function) == 0) {
		if (process_command_load(current_command, file_name) != 1) {
			printf("Error in function process_command_load()\n");
			return 0;
		}
	}
	else if (strcmp("slink", current_command->function) == 0) {
		if (process_command_slink(current_command, file_name) != 1) {
			printf("Error in function process_command_slink()\n");
			return 0;
		}
	}
	else if (strcmp("exit", current_command->function) == 0) {
		if (process_command_exit(current_command, file_name) != 1) {
			printf("Error in function process_command_exit()\n");
			return 0;
		}
	}
	else {
		printf("Unknown command!\n");
		return 0;
	}
	
	return 1;
}


/* ____________________________________________________________________________

	int process_command_format(command *current_command, char *file_name)
    
    Zpracuje pøíkaz format. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_format(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int param_length = strlen(current_command->first_param);
		char *size_str = NULL;
		int size_int = 0;
		
		size_str = calloc(1, (strlen(current_command->first_param) + 1) * sizeof(char));
		strcpy(size_str, current_command->first_param);
		
		if ((size_str[param_length - 1] == 'B')  
			&& (size_str[param_length - 2] == 'M')) {
			size_str[strlen(size_str) - 1] = '\0';
			size_str[strlen(size_str) - 1] = '\0';
		
			size_int = atoi(size_str);
		
			if (size_str != NULL) {
				free(size_str);
				size_str = NULL;
			}
			
			if (format_file(file_name, size_int) != 1) {
				return 0;
			}
			
			if (load_file(file_name) != 1) {
				return 0;
			}
			
			printf("OK\n");
			return 1;
		}
		else {
			return 0;
		}	
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_ls(command *current_command, char *file_name)
    
    Zpracuje pøíkaz ls. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_ls(command *current_command, char *file_name) {
	
	if ((current_command->first_param == NULL) && (current_command->second_param == NULL)) {
		func_ls(pwd, file_name);
		return 1;
	}
	else if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int32_t uid_found = get_uid_from_path_dir(current_command->first_param, file_name);
		
		if (uid_found != -1) {
			func_ls(uid_found, file_name);
		} 
		else {
			printf("PATH NOT FOUND\n");
		}
		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_pwd(command *current_command, char *file_name)
    
    Zpracuje pøíkaz pwd. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_pwd(command *current_command, char *file_name) {
	
	if ((current_command->first_param == NULL) && (current_command->second_param == NULL)) {
		func_pwd(pwd, file_name);		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_cd(command *current_command, char *file_name)
    
    Zpracuje pøíkaz cd. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_cd(command *current_command, char *file_name) {
	
	if ((current_command->first_param == NULL) && (current_command->second_param == NULL)) {
		return 1;
	}
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int32_t uid_found = get_uid_from_path_dir(current_command->first_param, file_name);
		
		if (uid_found != -1) {
			func_cd(uid_found);
		} 
		else {
			printf("PATH NOT FOUND\n");
		}
				
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_mkdir(command *current_command, char *file_name)
    
    Zpracuje pøíkaz mkdir. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_mkdir(command *current_command, char *file_name) {	
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		char **path_and_name = NULL;
		
		path_and_name = parse_path(current_command->first_param);
		
		if (path_and_name[0] == NULL) {
			if (is_name_unique(path_and_name[1], pwd, file_name)) {
				func_mkdir(pwd, path_and_name[1], file_name);
			}
			else {
				printf("EXIST\n");
			}
		}
		else {
			int32_t uid_found = get_uid_from_path_dir(path_and_name[0], file_name);
		
			if (uid_found != -1) {
				if (is_name_unique(path_and_name[1], uid_found, file_name)) {
					func_mkdir(uid_found, path_and_name[1], file_name);
				}
				else {
					printf("EXIST\n");
				}
			}
			else {
				printf("PATH NOT FOUND\n");
			}
		}
		
		/* uvolnìní pamìti po `path_and_name' */
		if (path_and_name != NULL) {
			free_path_and_name(path_and_name);
			path_and_name = NULL;
		}
		
		return 1;
	}
	else {
		return 0;
	}
}


/* ____________________________________________________________________________

	int process_command_incp(command *current_command, char *file_name)
    
    Zpracuje pøíkaz incp. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_incp(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param != NULL)) {
		char **path_and_name = NULL;
		
		path_and_name = parse_path(current_command->second_param);
		
		if (path_and_name[0] == NULL) {
			if (is_name_unique(path_and_name[1], pwd, file_name)) {
				func_incp(current_command->first_param, pwd, path_and_name[1], file_name); 
			}
			else {
				printf("EXIST\n");
			}
		}
		else {
			int32_t uid_found = get_uid_from_path_dir(path_and_name[0], file_name);
		
			if (uid_found != -1) {
				if (is_name_unique(path_and_name[1], uid_found, file_name)) {
					func_incp(current_command->first_param, uid_found, path_and_name[1], file_name);
				}
				else {
					printf("EXIST\n");
				}
			}
			else {
				printf("PATH NOT FOUND\n");
			}
		}
		
		/* uvolnìní pamìti po `path_and_name' */
		if (path_and_name != NULL) {
			free_path_and_name(path_and_name);
			path_and_name = NULL;
		}
		
		return 1;
	}
	else {
		return 0;
	}
}


/* ____________________________________________________________________________

	int process_command_outcp(command *current_command, char *file_name)
    
    Zpracuje pøíkaz outcp. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_outcp(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param != NULL)) {
		int32_t uid_found = get_uid_from_path_all(current_command->first_param, file_name);
		mft_item **curr_item = NULL;
		int32_t uid_temp = -1;
		
		if (uid_found != -1) {
			
			curr_item = get_mft_item_by_uid(uid_found);
			
			if (curr_item[0]->type == 0) {
				func_outcp(uid_found, current_command->second_param, file_name);
			}
			else if (curr_item[0]->type == 2) {
				uid_temp = get_file_uid_from_slink(uid_found, file_name);
				
				if (uid_temp != -1) {
					func_outcp(uid_temp, current_command->second_param, file_name);
				}
				else {
					printf("SYMBOLIC LINK IS BROKEN\n");
				}
			}
			else {
				printf("FILE NOT FOUND\n");
			}
			
			/* uvolnìní pamìti po `curr_item' */
			if (curr_item != NULL) {
				free(curr_item);
				curr_item = NULL;
			}
		}
		else {
			printf("FILE NOT FOUND\n");
		}
		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_cat(command *current_command, char *file_name)
    
    Zpracuje pøíkaz cat. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_cat(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int32_t uid_found = get_uid_from_path_all(current_command->first_param, file_name);
		mft_item **curr_item = NULL;
		int32_t uid_temp = -1;
		
		if (uid_found != -1) {
			
			curr_item = get_mft_item_by_uid(uid_found);
			
			if (curr_item[0]->type == 0) {
				func_cat(uid_found, file_name);
			}
			else if (curr_item[0]->type == 2) {
				uid_temp = get_file_uid_from_slink(uid_found, file_name);
				
				if (uid_temp != -1) {
					func_cat(uid_temp, file_name);
				}
				else {
					printf("SYMBOLIC LINK IS BROKEN\n");
				}
			}
			else {
				printf("FILE NOT FOUND\n");
			}
			
			/* uvolnìní pamìti po `curr_item' */
			if (curr_item != NULL) {
				free(curr_item);
				curr_item = NULL;
			}
		}
		else {
			printf("FILE NOT FOUND\n");
		}
		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_cp(command *current_command, char *file_name)
    
    Zpracuje pøíkaz cp. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_cp(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param != NULL)) {
		int32_t uid_found_source = 0;
		int32_t uid_found_dest = 0;
		mft_item **curr_item = NULL;
		char **path_and_name = NULL;
		int32_t uid_temp = -1;
				
		/* nalezení zdroje */
		uid_found_source = get_uid_from_path_all(current_command->first_param, file_name);
		
		if (uid_found_source != -1) {
			
			//vyøešit slink!
			curr_item = get_mft_item_by_uid(uid_found_source);
			
			if (curr_item[0]->type == 0) {
				path_and_name = parse_path(current_command->second_param);
	
				/* nalezení cíle */
				if (path_and_name[0] == NULL) {
					if (is_name_unique(path_and_name[1], pwd, file_name)) {
						func_cp(uid_found_source, pwd, path_and_name[1], file_name);
					}
					else {
						printf("EXIST\n");
					}
				}
				else {
					uid_found_dest = get_uid_from_path_dir(path_and_name[0], file_name);
		
					if (uid_found_dest != -1) {
						if (is_name_unique(path_and_name[1], uid_found_dest, file_name)) {
							func_cp(uid_found_source, uid_found_dest, path_and_name[1], file_name);
						}
						else {
							printf("EXIST\n");
						}
					}
					else {
						printf("PATH NOT FOUND\n");
					}
				}
				
				/* uvolnìní pamìti po `path_and_name' */
				if (path_and_name != NULL) {
					free_path_and_name(path_and_name);
					path_and_name = NULL;
				}
			}
			else if (curr_item[0]->type == 2) {
				path_and_name = parse_path(current_command->second_param);
				
				/* nalezení cíle */
				if (path_and_name[0] == NULL) {
					if (is_name_unique(path_and_name[1], pwd, file_name)) {
						uid_temp = get_file_uid_from_slink(uid_found_source, file_name);
						
						if (uid_temp != -1) {
							func_slink(uid_temp, pwd, path_and_name[1], file_name);
						}
						else {
							printf("SYMBOLIC LINK IS BROKEN\n");
						}
					}
					else {
						printf("EXIST\n");
					}
				}
				else {
					uid_found_dest = get_uid_from_path_dir(path_and_name[0], file_name);
		
					if (uid_found_dest != -1) {
						if (is_name_unique(path_and_name[1], uid_found_dest, file_name)) {
							uid_temp = get_file_uid_from_slink(uid_found_source, file_name);
						
							if (uid_temp != -1) {
								func_slink(uid_temp, uid_found_dest, path_and_name[1], file_name);
							}
							else {
								printf("SYMBOLIC LINK IS BROKEN\n");
							}
						}
						else {
							printf("EXIST\n");
						}
					}
					else {
						printf("PATH NOT FOUND\n");
					}
				}
				
				/* uvolnìní pamìti po `path_and_name' */
				if (path_and_name != NULL) {
					free_path_and_name(path_and_name);
					path_and_name = NULL;
				}	
			}
			else {
				printf("FILE NOT FOUND\n");
			}
			
			/* uvolnìní pamìti po `curr_item' */
			if (curr_item != NULL) {
				free(curr_item);
				curr_item = NULL;
			}
		}
		else {
			printf("FILE NOT FOUND\n");
		}
		
		return 1;
	}
	else {
		return 0;
	}
}


/* ____________________________________________________________________________

	int process_command_mv(command *current_command, char *file_name)
    
    Zpracuje pøíkaz mv. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_mv(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param != NULL)) {
		int32_t uid_found_source_file = 0;
		int32_t uid_found_source_dir = 0;
		int32_t uid_found_dest = 0;
		mft_item **curr_item = NULL;
		char **path_and_name_src = NULL;
		char **path_and_name_dst = NULL;
		char *temp_first_param = NULL;

		temp_first_param = (char *) calloc(strlen(current_command->first_param) + 1, sizeof(char));
		
		strncpy(temp_first_param, current_command->first_param, strlen(current_command->first_param));
		
		/* øešení src */
		uid_found_source_file = get_uid_from_path_all(current_command->first_param, file_name);
		
		if (uid_found_source_file != -1) {
			path_and_name_src = parse_path(temp_first_param);
			
			if (path_and_name_src[0] == NULL) {
				uid_found_source_dir = pwd;
			}
			else {
				uid_found_source_dir = get_uid_from_path_dir(path_and_name_src[0], file_name);
			}
			
			curr_item = get_mft_item_by_uid(uid_found_source_file);
			
			if ((curr_item[0]->type == 0) || (curr_item[0]->type == 2)) {
				path_and_name_dst = parse_path(current_command->second_param);
	
				/* øešení dst */
				if (path_and_name_dst[0] == NULL) {
					if (is_name_unique(path_and_name_dst[1], pwd, file_name)) {
						func_mv(uid_found_source_dir, uid_found_source_file, pwd, path_and_name_dst[1], file_name);
					}
					else {
						printf("EXIST\n");
					}
				}
				else {
					uid_found_dest = get_uid_from_path_dir(path_and_name_dst[0], file_name);
		
					if (uid_found_dest != -1) {
						if (is_name_unique(path_and_name_dst[1], uid_found_dest, file_name)) {
							func_mv(uid_found_source_dir, uid_found_source_file, uid_found_dest, path_and_name_dst[1], file_name);
						}
						else {
							printf("EXIST\n");
						}
					}
					else {
						printf("PATH NOT FOUND\n");
					}
				}
				
				/* uvolnìní pamìti po `path_and_name_dst' */
				if (path_and_name_dst != NULL) {
					free_path_and_name(path_and_name_dst);
					path_and_name_dst = NULL;
				}
			}
			else {
				printf("FILE NOT FOUND\n");
			}
			
			/* uvolnìní pamìti po `curr_item' */
			if (curr_item != NULL) {
				free(curr_item);
				curr_item = NULL;
			}
			
			/* uvolnìní pamìti po `path_and_name_src' */
			if (path_and_name_src != NULL) {
				free_path_and_name(path_and_name_src);
				path_and_name_src = NULL;
			}
		}
		else {
			printf("FILE NOT FOUND\n");
		}
		
		/* uvolnìní pamìti po `temp_first_param' */
		if (temp_first_param != NULL) {
			free(temp_first_param);
			temp_first_param = NULL;
		}
		
		return 1;
	}
	else {
		return 0;
	}
}


/* ____________________________________________________________________________

	int process_command_rm(command *current_command, char *file_name)
    
    Zpracuje pøíkaz rm. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_rm(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int32_t uid_found_file = 0;
		int32_t uid_found_dir = 0;
		mft_item **curr_item = NULL;
		char **path_and_name = NULL;
		char *temp_first_param = NULL;
		
		temp_first_param = (char *) calloc(strlen(current_command->first_param) + 1, sizeof(char));
		
		strncpy(temp_first_param, current_command->first_param, strlen(current_command->first_param));
		
		uid_found_file = get_uid_from_path_all(current_command->first_param, file_name);
		
		if (uid_found_file != -1) {
			path_and_name = parse_path(temp_first_param);
			
			if (path_and_name[0] == NULL) {
				uid_found_dir = pwd;
			}
			else {
				uid_found_dir = get_uid_from_path_dir(path_and_name[0], file_name);
			}
			
			curr_item = get_mft_item_by_uid(uid_found_file);
			
			if ((curr_item[0]->type == 0) || (curr_item[0]->type == 2)) {
				func_rm(uid_found_dir, uid_found_file, file_name);
			}
			else {
				printf("FILE NOT FOUND\n");
			}
			
			/* uvolnìní pamìti po `curr_item' */
			if (curr_item != NULL) {
				free(curr_item);
				curr_item = NULL;
			}
			
			/* uvolnìní pamìti po `path_and_name' */
			if (path_and_name != NULL) {
				free_path_and_name(path_and_name);
				path_and_name = NULL;
			}
		}
		else {
			printf("FILE NOT FOUND\n");
		}
		
		/* uvolnìní pamìti po `temp_first_param' */
		if (temp_first_param != NULL) {
			free(temp_first_param);
			temp_first_param = NULL;
		}
		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_rmdir(command *current_command, char *file_name)
    
    Zpracuje pøíkaz rmdir. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_rmdir(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int32_t uid_found_dir = 0;
		int32_t uid_found_parent = 0;
		char **path_and_name = NULL;
		char *temp_first_param = NULL;
		
		temp_first_param = (char *) calloc(strlen(current_command->first_param) + 1, sizeof(char));
		
		strncpy(temp_first_param, current_command->first_param, strlen(current_command->first_param));
		
		uid_found_dir = get_uid_from_path_dir(current_command->first_param, file_name);
		
		if (uid_found_dir == 1) {
			printf("CANNOT REMOVE ROOT DIR\n");
			return 1;
		}
		
		if (uid_found_dir != -1) {
			path_and_name = parse_path(temp_first_param);
			
			if (path_and_name[0] == NULL) {
				uid_found_parent = pwd;
			}
			else {
				uid_found_parent = get_uid_from_path_dir(path_and_name[0], file_name);
			}
			
			func_rmdir(uid_found_parent, uid_found_dir, file_name);
			
			/* uvolnìní pamìti po `path_and_name' */
			if (path_and_name != NULL) {
				free_path_and_name(path_and_name);
				path_and_name = NULL;
			}
		} 
		else {
			printf("FILE NOT FOUND\n");
		}
		
		/* uvolnìní pamìti po `temp_first_param' */
		if (temp_first_param != NULL) {
			free(temp_first_param);
			temp_first_param = NULL;
		}
		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_info(command *current_command, char *file_name)
    
    Zpracuje pøíkaz info. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_info(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int32_t uid_found = get_uid_from_path_all(current_command->first_param, file_name);
		
		if (uid_found != -1) {
			func_info(uid_found, file_name);
		}
		else {
			printf("FILE NOT FOUND\n");
		}
		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_load(command *current_command, char *file_name)
    
    Zpracuje pøíkaz load. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_load(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		func_load(current_command->first_param, file_name);
		
		return 1;
	}
	else {
		return 0;	
	}
}


/* ____________________________________________________________________________

	int process_command_slink(command *current_command, char *file_name)
    
    Zpracuje pøíkaz slink. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_slink(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param != NULL)) {
		int32_t uid_found_source = 0;
		int32_t uid_found_dest = 0;
		mft_item **curr_item = NULL;
		char **path_and_name = NULL;
				
		/* nalezení zdroje */
		uid_found_source = get_uid_from_path_all(current_command->first_param, file_name);
		
		if (uid_found_source != -1) {
			
			curr_item = get_mft_item_by_uid(uid_found_source);
			
			if (curr_item[0]->type == 0) {
				path_and_name = parse_path(current_command->second_param);
	
				/* nalezení cíle */
				if (path_and_name[0] == NULL) {
					if (is_name_unique(path_and_name[1], pwd, file_name)) {
						func_slink(uid_found_source, pwd, path_and_name[1], file_name);
					}
					else {
						printf("EXIST\n");
					}
				}
				else {
					uid_found_dest = get_uid_from_path_dir(path_and_name[0], file_name);
		
					if (uid_found_dest != -1) {
						if (is_name_unique(path_and_name[1], uid_found_dest, file_name)) {
							func_slink(uid_found_source, uid_found_dest, path_and_name[1], file_name);
						}
						else {
							printf("EXIST\n");
						}
					}
					else {
						printf("PATH NOT FOUND\n");
					}
				}
				
				/* uvolnìní pamìti po `path_and_name' */
				if (path_and_name != NULL) {
					free_path_and_name(path_and_name);
					path_and_name = NULL;
				}				
			}
			else {
				printf("FILE NOT FOUND\n");
			}
			
			/* uvolnìní pamìti po `curr_item' */
			if (curr_item != NULL) {
				free(curr_item);
				curr_item = NULL;
			}
		}
		else {
			printf("FILE NOT FOUND\n");
		}
		
		return 1;
	}
	else {
		return 0;
	}
}


/* ____________________________________________________________________________

	int process_command_exit(command *current_command, char *file_name)
    
    Zpracuje pøíkaz exit. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_exit(command *current_command, char *file_name) {
	
	if ((current_command->first_param == NULL) && (current_command->second_param == NULL)) {
		run_shell = 0;
		
		return 1;
	}
	else {
		return 0;
	}
}