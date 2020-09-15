#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

//globales
#define FULL 16
#define OFFSET 8
#define FRAMES 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
unsigned int indice_de_memoria = 0;
//frames
typedef struct _Frame {
    LIST_ENTRY(_Frame) ptrs;
    unsigned int pfn;
    char frame[FRAME_SIZE];
} Frame;
//pages
typedef struct _Page {
    LIST_ENTRY(_Page) ptrs;
    unsigned int vpn;
    unsigned int pfn;
} Page;

//list of pages
LIST_HEAD(list_of_pages, _Page) tabla_de_paginas;
//list of frames
LIST_HEAD(list_of_frames, _Frame) memoria_principal;

Page *create_page(int vpn,int pfn);
Frame *create_frame(int pfn, char * frame);

unsigned int translate(unsigned int address);

int main(int argc, char *argv[])
{
    FILE *fp;
    fp  = fopen ("./assets/addresses.txt", "r");

    unsigned int addr;

    while(fscanf(fp, "%u", &addr)!= EOF)
    {
        if(addr>65535)
            printf("The address %u is not valid\n",addr);
        
        else 
            translate(addr);
    }

    fclose (fp);
    return 0;
}

unsigned int translate(unsigned int address){

    printf("The address %u is getting translated...\n",address);

    unsigned int p_num=address>>OFFSET;
    unsigned int ofs=address%PAGE_SIZE;
    
    printf("Page number %u...\n",p_num);
    printf("Offset %u...\n",ofs);

    return 0;
}

Page *create_page(int vpn,int pfn){
    Page *page = (Page *)malloc(sizeof(Page));
    page->vpn = vpn;
    page->pfn = pfn;
}
Frame *create_frame(int pfn, char * frame){
    Frame *frm = (Frame *)malloc(sizeof(Frame));
    frm->pfn = pfn;
    strcpy(frm->frame, frame, FRAME_SIZE);
}
