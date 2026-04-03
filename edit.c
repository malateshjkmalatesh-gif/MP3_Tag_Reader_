#include <stdio.h>
#include <string.h>
#include "type.h"
#include "edit.h"

char* search_tag(char* input)
{
    if(strcmp(input, "-a") == 0)      
    {                               
        return "TPE1";                 
    }  
    if(strcmp(input, "-A") == 0)      
    {                               
        return "TALB";                 
    } 
    if(strcmp(input, "-t") == 0)      
    {                               
        return "TIT2";                 
    }
    if(strcmp(input, "-y") == 0)      
    {                               
        return "TYER";                 
    }
    if(strcmp(input, "-c") == 0)      
    {                               
        return "TCON";                 
    }
    if(strcmp(input, "-C") == 0)      
    {                               
        return "COMM";                 
    }
    if(strcmp(input, "-i") == 0)      
    {                               
        return "IPLS";                 
    }
    if(strcmp(input, "-p") == 0)      
    {                               
        return "TCOP";                 
    }
    if(strcmp(input, "-s") == 0)      
    {                               
        return "TCOM";                 
    }
    return NULL;
}

Status read_edit_validate(char* tag, char* fname)
{
    if(strcmp(tag, "-a")==0 || strcmp(tag, "-A")==0 || strcmp(tag, "-y")==0 || strcmp(tag, "-t")==0 || strcmp(tag, "-c")==0 || strcmp(tag, "-C")==0 
    || strcmp(tag, "-i") == 0 || strcmp(tag, "-p") == 0 || strcmp(tag, "-s") == 0)
    {
        
        if(strstr(fname, ".mp3") == NULL || strcmp(strstr(fname, ".mp3"), ".mp3"))
        {
            return failure;
        }  
    }
    else
    {
        
        return failure;
    }
    
    return success;
}

// Status open_files(FILE* src, FILE* dest, char* fname)
// {
//     src = fopen(fname, "r");
//     if(src == NULL)
//     {
//         printf("Error : unable to open %s file\n", fname);
//         return failure;
//     }

//     dest = fopen("temp.mp3", "w");
//     if(dest == NULL)
//     {
//         printf("Error : unable to open temp.mp3 file\n");
//         return failure;
//     }

//     return success;
// }

Status copy_header(FILE* src, FILE* dest)
{
    char buff[10];
    fread(buff, 1, 10, src);
    fwrite(buff, 1, 10, dest);
    if(ftell(src)!=10 || ftell(dest)!=10)
    {
        return failure;
    }
    return success;
}

Status tag_copy_search(char* user_tag, FILE* src, FILE* dest)
{
    char buff[5];
    fread(buff, 1, 4, src);
    fwrite(buff, 1, 4, dest);

    buff[4] = '\0';
    if(strcmp(buff, user_tag) == 0)
    {
        return success;
    }
    return failure;
}

void copy_user_size(FILE* dest, int len)
{
    char ch;
    for(int i=3; i>=0; i--)
    {
        ch = 0;
        for(int j=7; j>=0; j++)
        {
            if(len & (1<<(i*8 + j)))
            {
                ch = ch | (1<<j);
            }
        }
        fputc(ch, dest);
    }
}

Status find_src_size(FILE* fptr, int* size)
{
    char buff[4];
    fread(buff, 1, 4, fptr);
    *size = *size>>24 | buff[0];
    *size = *size>>16 | buff[1];
    *size = *size>>8 | buff[2];
    *size = *size>>0 | buff[3];
    // for(int j=0; j<4; j++)
    // {
    //     for(int i=7; i>=0; i--)
    //     {
    //         if(buff[3-j]&(1<<i))
    //         {
    //             *size = (*size) | (1<<(j*7 + i));
    //         }
    //     }
    // }
    printf("Size found : %d\n", *size);
    if(*size == 0)
    {
        return failure;
    }
    printf("Successfully edited\n");
    return success;
}

void copy_flag(FILE* src, FILE* dest)
{
    char buff[3];
    fread(buff, 1, 3, src);
    fwrite(buff, 1, 3, dest);
}

void copy_user_data(FILE* dest, char* info)
{
    fwrite(info, 1, strlen(info), dest);
}

void copy_size(FILE* src, FILE* dest)
{
    char buff[4];
    fread(buff, 1, 4, src);
    fwrite(buff, 1, 4, dest);
}

void copy_data(int size, FILE* src, FILE* dest)
{
    char buff[size];
    fread(buff, 1, size, src);
    fwrite(buff, 1, size, dest);
}

Status edit_file_info(char* tag, char* info, char* src_fname)
{
    //file declaration and opening
    FILE* src = fopen(src_fname, "r+");
    FILE* dest = fopen("temp.mp3", "w+");
    if(src == NULL || dest == NULL)
    {
        printf("Error : unable to open %s temp.mp3 file\n", src_fname);
        return failure;
    }
    
    //copy header
    if(copy_header(src, dest) == failure)
    {
        printf("Unable to copy whole header\n");
        return failure;
    }
    printf("Header copied successfully\n");

    //find matching tag given by user
    char* tag_name = search_tag(tag);
    printf("%s\n", tag_name);
    //copy other content until tan_name matched with file data
    for(int i=0; i<6; i++)
    {
        int size = 0;
        if(tag_copy_search(tag_name, src, dest) ==  success)
        {
            copy_user_size(dest, strlen(info)+1);

            
            if(find_src_size(src, &size) == failure)
            {
                printf("Unable to find size in original file information\n");
                return failure;
            }

            //copy flag
            copy_flag(src, dest);

            copy_user_data(dest, info);

            fseek(src, size-1, SEEK_CUR);

            break;
        }

        copy_size(src, dest);
        
        fseek(src, -4, SEEK_CUR);

        if(find_src_size(src, &size) == failure)
        {
            printf("Unable to find size in original file information\n");
            return failure;
        }

        //copy flag
        copy_flag(src, dest);

        copy_data(size-1, src, dest);

    }

    //copy remaining data
    int ch;
    while((ch = fgetc(src)) != EOF)
    {
        fputc(ch, dest);
    }

    fseek(src,0,SEEK_SET);
    fseek(dest,0,SEEK_SET);
     while((ch = fgetc(dest)) != EOF)
    {
        fputc(ch, src);
    }
    fclose(src);
    fclose(dest);
    return success;
}