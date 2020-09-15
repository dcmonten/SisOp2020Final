#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
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
    LIST_ENTRY(_Frame) fptrs;
    unsigned int pfn;
    char frame[FRAME_SIZE];
} Frame;
//pages
typedef struct _Page {
    LIST_ENTRY(_Page) pptrs;
    unsigned int vpn;
    unsigned int pfn;
} Page;

//list of pages
LIST_HEAD(list_of_pages, _Page) tabla_de_paginas;
//list of frames
LIST_HEAD(list_of_frames, _Frame) memoria_principal;
//Imprime la tabla de páginas
void printPT();
//Libera la memoria de la tabla de páginas
void freePT();
//encuentra la primera página que coincide o la crea y la inserta en la lista
bool insertOrCreatePage(unsigned int vpn);

Page *create_page(unsigned int vpn,unsigned int pfn);
Frame *create_frame(int pfn, char * frame);

unsigned int translate(unsigned int address);

int main(int argc, char *argv[])
{
    //Inicialización de listas
    LIST_INIT(&tabla_de_paginas);
    LIST_INIT(&memoria_principal);
    //Lectura de archivo
    FILE *fp;
    fp  = fopen ("./assets/addresses.txt", "r");

    unsigned int addr;

    while(fscanf(fp, "%u", &addr)!= EOF)
    {
        if(addr>65535)
            printf("The address %u is not valid\n",addr);
        
        else{
            translate(addr);
        }
            
    }
    fclose (fp);
    freePT();
    return 0;
}

bool insertOrCreatePage(unsigned int vpn){

    Page * p;

    LIST_FOREACH(p, &tabla_de_paginas, pptrs) {
        //CASO: HIT
        if(p->vpn==vpn){
            return true;
        }  
    }
    

    //CASO: MISS
    Page * pg = create_page(vpn,indice_de_memoria);
    indice_de_memoria++;
    LIST_INSERT_HEAD(&tabla_de_paginas, pg, pptrs);
    return false;
}
unsigned int translate(unsigned int address){


    unsigned int p_num=address>>OFFSET;
    unsigned int ofs=address%PAGE_SIZE;

    //printf("The address is %u, page number %u, offset %u.\n",address,p_num,ofs);
    
    //Insertar o crear página
    if(insertOrCreatePage(p_num))
        printf("    \nHIT   page number %u\n",p_num);
    else
        printf("    \nMISS   page number %u\n",p_num);

    //Frame * f = create_frame(indice_de_memoria,);
    
    //LIST_INSERT_HEAD(&memoria_principal, f, fptrs);
    
    return 0;
}

void printPT(){
    Page * p;
    printf("Pages table---------------------\n");
    LIST_FOREACH(p, &tabla_de_paginas, pptrs) {
        printf("\n%u %u \n",p->vpn,p->pfn);
    }
    printf("End pages table---------------------\n");
    freePT();
}

void freePT(){
    Page *node;
    while (!LIST_EMPTY(&tabla_de_paginas)) {
        node = LIST_FIRST(&tabla_de_paginas);
        LIST_REMOVE(node, pptrs);
        free(node);
    }
}

Page *create_page(unsigned int vpn,unsigned int pfn){
    Page *page = (Page *)malloc(sizeof(Page));
    page->vpn = vpn;
    page->pfn = pfn;
}
Frame *create_frame(int pfn, char * frame){
    Frame *frm = (Frame *)malloc(sizeof(Frame));
    frm->pfn = pfn;
    strcpy(frm->frame, frame);
}
