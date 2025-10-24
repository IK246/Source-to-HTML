#include "main.h"

status main(int count, char *vect[])
{
    // checks the arguments
    if(count != 2)
    {
        puts(R "Info: " Re "Invalid Arguments");
        return e_failure;
    }

    char *ptr;

    // validating the file extension
    if((ptr = strstr(vect[1], ".c")) != NULL)
    {
        if(!strcmp(ptr, ".c"))
        {
            // structure declaration
            file_info info;

            // strores file name in the structure
            info.src_fname = vect[1];

            // creating the destination file
            sprintf(info.dest_fname, "%s.html", info.src_fname);

            // operation begins
            if(s2html(&info) == e_success)
            {
                printf(G "Info: " Re "Operation Done!\n\t" C "File Name:" Re " %s\n", info.dest_fname);
                return e_success;
            }
        }
        else
            puts(R "Info: " Re "Misplaced Extension!");
    }
    else
        puts(R "Info: " Re ".c Extension Missing!");

    return e_failure;
}
