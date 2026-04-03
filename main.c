
#include "type.h"
#include "view.h"
#include "edit.h"

type Type_of_operation(const char* );

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Invalid input in command lines\n");
        return 1;
    }
    if(argc == 3)
    {
        if(Type_of_operation(argv[1]) != view)
        {
            printf("Invalid arguments\n");
            return 1;
        }

        printf("Operation checked successfully\n");
        viewInfo view_info;

        if(read_validate(argv[2], &view_info) == failure)
        {
            printf("Invalid arguments name\n");
            return 1;
        }
        
        if(view_file_info(&view_info) == failure)
        {
            printf("Unable to read data properly\n");
            return 1;
        }
    }
    else if(argc == 5)
    {
        if(Type_of_operation(argv[1]) != edit)
        {
            printf("Invalid arguments\n");
            return 1;
        }

        printf("Operation checked successfully\n");

        if(read_edit_validate(argv[2], argv[4]) == failure)
        {
            printf("Invalid arguments name\n");
            return 1;
        }
        printf("Validated successfully\n");
        
        if(edit_file_info(argv[2], argv[3], argv[4]) == failure)
        {
            printf("Unable to read data properly\n");
            return 1;
        }
    }
    else if(argc == 2)
    {
        if(Type_of_operation(argv[1]) == help)
        {
            printf("-a Modifies Artist name\n-A Modifies Album\n-t Modifies Title\n-y Modifies year\n-c Modifies content type\n-C Modifies comment\n-i Modifies People involved list\n-p Modifies copyrights\n-s Modifies composer\n");
            return 0;
        }
    }

}

type Type_of_operation(const char* operation)
{
    if(!strcmp(operation , "-v"))
    {
        return view;
    }
    else if(!strcmp(operation, "-e"))
    {
        return edit;
    }
    else if(!strcmp(operation, "--help"))
    {
        return help;
    }
    return unsupported;
}