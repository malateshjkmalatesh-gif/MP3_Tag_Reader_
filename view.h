#ifndef VIEW
#define VIEW
#include <stdio.h>
#include <string.h>
#include "type.h"


typedef struct 
{
    
    char* file_name;
    FILE *fptr;
} viewInfo;

typedef struct
{
    char tag_name[50];
    char info[100];

}Content;




Status read_validate(char* , viewInfo*);

Status view_file_info(viewInfo*);

Status tag_reading(FILE* fptr, char* tag_name);

Status info_reading(FILE* , char*, int);

Status find_size(FILE*, int*);

#endif


