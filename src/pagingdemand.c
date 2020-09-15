#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
unsigned int insertOrCreatePage(unsigned int vpn);

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

unsigned int insertOrCreatePage(unsigned int vpn){

    Page * p;
    printf("--------Looking for %u-------\n",vpn);
    LIST_FOREACH(p, &tabla_de_paginas, pptrs) {
        if(p->vpn==vpn){
            printf("HIT %u %u \n",p->vpn,p->pfn);
            printf("-----------------------------\n",vpn);
            return p->pfn;
        }  
    }
    printf("MISS %u-------\n",vpn);
    Page * pg = create_page(vpn,indice_de_memoria);
    indice_de_memoria++;
    LIST_INSERT_HEAD(&tabla_de_paginas, pg, pptrs);
    printf("-----------------------------\n",vpn);
    return pg->pfn;
}
unsigned int translate(unsigned int address){

    printf("The address %u is getting translated...\n",address);

    unsigned int p_num=address>>OFFSET;
    unsigned int ofs=address%PAGE_SIZE;
    
    printf("Page number %u...\n",p_num);
    printf("Offset %u...\n",ofs);
    //
    insertOrCreatePage(p_num);
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
