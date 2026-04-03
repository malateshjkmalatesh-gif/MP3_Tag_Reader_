#include "type.h"
//#include <stdlib.h>
#include "view.h"

char* tag_matching(char* input)
{
    if(strcmp(input, "TIT2") == 0)      
    {                               
        return "TITLE";                 
    }
    if(strcmp(input, "TALB") == 0)      
    {                               
        return "ALBUM";                 
    }
    if(strcmp(input, "TPE1") == 0)      
    {                               
        return "Artist name";                 
    }
    if(strcmp(input, "TYER") == 0)      
    {                               
        return "Year";                 
    }
    if(strcmp(input, "TCON") == 0)      
    {                               
        return "Content type";                 
    }
    if(strcmp(input, "COMM") == 0)      
    {                               
        return "Comment";                 
    }
    if(strcmp(input, "IPLS") == 0)      
    {                               
        return "Involved People List";                 
    }
    if(strcmp(input, "TCOP") == 0)      
    {                               
        return "Copyrights";                 
    }
    if(strcmp(input, "TCOM") == 0)      
    {                               
        return "Composer";                 
    }
    return NULL;
}

Status read_validate(char* input, viewInfo *view_info)
{
    if(strstr(input, ".mp3") == NULL || strcmp(strstr(input, ".mp3"), ".mp3"))
    {
        return failure;
    }
    printf("File name : %s\n", input);
    view_info->file_name = input;
    return success;
}

Status tag_reading(FILE* fptr, char* name)
{
    char buff[5];
    fread(buff, 1, 4, fptr);
    buff[4] = '\0';
    //printf("%s\n", buff);
    if(tag_matching(buff) == NULL)
    {
        printf("Tag name is not valid\n");
        return failure;
    }
    strcpy(name, tag_matching(buff));
    //printf("%c\n", name[0]);
    
    return success;
}

Status find_size(FILE* fptr, int* size)
{
    char buff[4];
    fread(buff, 1, 4, fptr);
    for(int j=0; j<4; j++)
    {
        for(int i=7; i>=0; i--)
        {
            if(buff[3-j]&(1<<i))
            {
                *size = (*size) | (1<<(j*7 + i));
            }
        }
    }
    if(*size == 0)
    {
        return failure;
    }
    return success;
}

Status info_reading(FILE* fptr, char* info, int size)
{
    fread(info, 1, size-1, fptr);
    info[size-1] = '\0';
    if(info == NULL)
    {
        return failure;
    }
    return success;
}

Status view_file_info(viewInfo *view_info)
{
    //open mp3 file
    printf("----------------------------------------------------\n");
    printf("\t\tmp3 file information\n");
    printf("----------------------------------------------------\n");
    view_info->fptr = fopen(view_info->file_name, "r");

    //Error handling
    if(view_info->fptr == NULL)
    {
        printf("Error : Unable to open file\n");
        return failure;
    }

    //skip header
    fseek(view_info->fptr, 10, SEEK_SET);
    if(ftell(view_info->fptr) != 10)
    {
        printf("Header not skipped properly\n");
        return failure;
    }

    //tag reading
    Content content[6];
    for(int i=0; i<6; i++)
    {

        if(tag_reading(view_info->fptr, content[i].tag_name) == failure)
        {
            printf("Unable to read tag properly in %d info\n", i);
            return failure;
        }
        printf("%-22s", content[i].tag_name);

        int size = 0;
        //printf("%ld\n", ftell(view_info->fptr));
        if(find_size(view_info->fptr, &size) == failure)
        {
            printf("Unable to find size in %d information\n", i);
            return failure;
        }
        
        //skip flag
        fseek(view_info->fptr, 3, SEEK_CUR);
        
        if(info_reading(view_info->fptr, content[i].info, size) == failure)
        {
            printf("Unable to read information properly in %d info\n", i);
            return failure;
        }
        printf(":\t%s\n", content[i].info);
    }
    return success;
}

