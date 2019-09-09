#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//You may also add more include directives as well.

// THIS VALUE CANNOT BE CHANGED.
// You should use this value when creating your buffer.
// And also to limit the amount of bytes each read CAN do.
#define BUFF_MAX 13
// ABOVE VALUE CANNOT BE CHANGED //

//////////////////////////////////////////////
///////////////// CONSTANTS //////////////////
//////////////////////////////////////////////

#define WELCOME "Welcome to the File Copy Program by Chris Eckhardt!\n"
#define PROMPT_1 "Enter the name of the file to copy from:\n"
#define PROMPT_2 "Enter the name of the file to copy to:\n"
#define PERMS 0666 /* read/write for owner, group, others */
#define SUCCESS "\nFile Copy Successful, %d bytes copied.\n\n"
#define P1_FAIL "@ Prompt 1 : open() "
#define P2_FAIL "@ Prompt 2 : open() "
#define COPY_ERROR "@ COPY : Byte count mis-match "
#define CLOSE_1_ERROR "@ close : read file did not close "
#define CLOSE_2_ERROR "@ close : copy file did not close "


int
main(int argc, char const *argv[])
{
    //////////////////////////////////////////////
    //////////// VARIABLES & BUFFER //////////////
    //////////////////////////////////////////////
    
    /*
        In this section, variables that will be used to hold input and return 
        values are defined and the read_buffer which will be used in the actual
        copy-file process
     */

    char *source_name = malloc(sizeof(char) * 100);
    char *copy_name = malloc(sizeof(char) * 100);
    size_t *read_buffer = malloc(sizeof(size_t) * BUFF_MAX);

    int n_read, n_write, fd_source, fd_copy;
    int bytes_coppied = 0;

    //////////////////////////////////////////////
    //////////////// FIRST PROMPT ////////////////
    //////////////////////////////////////////////

    /*
        Get source file to be copied. If it does not exist, 
        open() will return -1 and the program will exit with code 1.
    */

    printf(WELCOME);
    printf(PROMPT_1);
    scanf("%s",source_name);
    if((fd_source = open(source_name, O_RDONLY)) < 0)
    {
        perror(P1_FAIL);
        exit(1);
    }

    //////////////////////////////////////////////
    /////////////// SECOND PROMPT ////////////////
    //////////////////////////////////////////////

    /*
        If the copy-to file exists the target will be overwritten. 
        If it does not exist, a new file will be created. This is 
        established with the flag O_CREAT and O_TRUNC as a second param. PERMS 
        is a constant that dictates what permissions the target file 
        will have.
    */

    printf(PROMPT_2);
    scanf("%s",copy_name);
    if((fd_copy = open(copy_name, O_WRONLY | O_CREAT | O_TRUNC, PERMS)) < 0)
    {
        perror(P2_FAIL);
        exit(1);
    }
    
    //////////////////////////////////////////////
    ///////////////// COPY FILE //////////////////
    //////////////////////////////////////////////

    /*
        This section is where the actual copying is done. read() 
        returns 0 when EOF is reached, this will be the terminating 
        condition for the while loop. Next, the if statement will 
        check if the number of bytes coppied (return value of write()) 
        matches the number of bytes read (return value of read()). 
        If they do not match, an error message will appear and the 
        program will exit with code 1.

        NOTE: the two commented-out lines at the bottom of the if-
        statement are used for debugging purposes. The first prints 
        the contents of the read_buffer cast to an unsigned-long. 
        The second prints the number of bytes written into the new file.
     */

    while((n_read = read(fd_source, read_buffer, BUFF_MAX)) > 0)
    {
        if((n_write = write(fd_copy, read_buffer, n_read)) != n_read)
        {
            perror(COPY_ERROR);
            exit(1);
        }
        bytes_coppied += n_write;
        //printf("buffer reads as : %lu\n", (unsigned long) read_buffer); // for debugging
        //printf("bytes written : %d\n", n_write); // for debugging
    }

    //////////////////////////////////////////////
    //////////////// CLOSE FILES /////////////////
    //////////////////////////////////////////////

    /*
        Both opened files must be closed. close() returns -1 if
        error occurs.
    */

    if(close(fd_source) < 0)
    {
        perror(CLOSE_1_ERROR);
        exit(1);
    }

    if(close(fd_copy) < 0)
    {
        perror(CLOSE_2_ERROR);
        exit(1);
    }

    //////////////////////////////////////////////
    ////////////// COPY WAS SUCCESSFUL ///////////
    //////////////////////////////////////////////

    /*
        Hopefully we get here.
    */

    printf(SUCCESS, bytes_coppied);
    
    //////////////////////////////////////////////
    //////////////// EXIT PROGRAM ////////////////
    //////////////////////////////////////////////

    return 0;
}