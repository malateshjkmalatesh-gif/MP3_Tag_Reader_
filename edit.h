#ifndef EDIT
#define EDIT
#include <stdio.h>
#include <string.h>
#include "type.h"
                            


Status edit_file_info(char* tag, char* info, char* src_fname);

//Status open_files(FILE* src, FILE* dest, char* fname);

Status copy_header(FILE* src, FILE* dest);

Status tag_copy_search(char* user_tag, FILE* src, FILE* dest);

void copy_user_size(FILE* dest, int len);

Status find_size(FILE* fptr, int* size);

void copy_flag(FILE* src, FILE* dest);

void copy_user_data(FILE* dest, char* info);

void copy_size(FILE* src, FILE* dest);

void copy_data(int size, FILE* src, FILE* dest);

Status read_edit_validate(char* , char*);

#endif