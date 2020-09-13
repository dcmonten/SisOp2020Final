#include <stdio.h>
#include <stdlib.h>

unsigned int translate(unsigned int address);

int main(int argc, char *argv[])
{
    FILE *fp;
    fp  = fopen ("./assets/addresses.txt", "r");
    unsigned int addr;

    while(fscanf(fp, "%u", &addr)!= EOF)
    {
        if(addr>65535)
            printf("This address %u is not valid\n",addr);
        
        else 
            translate(addr);
    }

    fclose (fp);
    return 0;
}

unsigned int translate(unsigned int address){

    printf("This address %u is getting translated. Wait.\n",address);

    return 0;
}