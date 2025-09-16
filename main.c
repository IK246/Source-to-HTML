#include "main.h"

int main(int argc, char *argv[])
{
    // checks the arguments
    if(argc != 2)
    {
        printf(R "Info:" Re "Invalid Arguments\n");
        return 0;
    }
    char *ptr;

    // validating the file extension
    if((ptr = strstr(argv[1], ".c")) != NULL)
    {
        if(!strcmp(ptr, ".c"))
        {
            file_info info; // structre declaration

            // stores fname in the structre
            info.src_fname = argv[1];

            sprintf(info.dest_fname, "%s.html", info.src_fname);

            if(s2html(&info) == e_success);
            puts(G "Info:" Re "Operation Done!");
        }
        else
            puts(R "Info:" Re "Misplaced Extension!");
    }
    else
        puts(R "Info:" Re ".c Extension Missing!");
    return 0;
}
