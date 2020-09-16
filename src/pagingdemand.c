#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <sys/queue.h>

//constantes globales
#define FULL 16
#define OFFSET 8
#define FRAMES 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
//vars globales
unsigned int indice_de_memoria = 0;
FILE * backing_store;
//pages
typedef struct _Page {
    LIST_ENTRY(_Page) pptrs;
    unsigned int vpn;
    unsigned int pfn;
} Page;

//list of pages
LIST_HEAD(list_of_pages, _Page) tabla_de_paginas;
//list of frames
int memoria_principal[FRAMES][FRAME_SIZE];
//Imprime la tabla de páginas
void printPT();
//Libera la memoria de la tabla de páginas
void freePT();
//encuentra la primera página que coincide o la crea y la inserta en la lista
bool insertOrCreatePage(unsigned int vpn, unsigned int offset);
void backingStoreData(unsigned int vpn, unsigned int pfn, unsigned int offset);
Page *create_page(unsigned int vpn,unsigned int pfn);
unsigned int translate(unsigned int address);
void inicializar_memoria();

int main(int argc, char *argv[])
{
    //Inicialización de tabla de páginas
    LIST_INIT(&tabla_de_paginas);
    //inicialización de memoria principal
    inicializar_memoria();
    //Lectura de archivos
    FILE *fp;
    fp  = fopen ("./assets/addresses.txt", "r");
    backing_store = fopen ("./assets/BACKING_STORE.bin", "rb+");
    unsigned int addr;
    while(fscanf(fp, "%u", &addr)!= EOF)
    {
        if(addr>65535)
            printf("The address %u is not valid\n",addr);
        else
            translate(addr);
    }
    fclose (fp);
    fclose (backing_store);
    freePT();
    return 0;
}

void inicializar_memoria(){
    for (int i = 0; i < FRAMES; i++)
    {
        for (int j = 0; j < FRAME_SIZE; j++)
        {
            //Inicializo la memoria principal con ceros
            memoria_principal[i][j]=0;
            //el cero representará un espacio vacío de memoria
        }
    }
}
bool insertOrCreatePage(unsigned int vpn, unsigned int offset){

    Page * p;
    int requested;
    unsigned int vaddress = vpn*256+offset;
    unsigned int paddress;
    LIST_FOREACH(p, &tabla_de_paginas, pptrs) {
        //CASO: HIT
        if(p->vpn==vpn){
            //leer desde frame
            paddress = p->pfn*256+offset;
            requested = memoria_principal[p->pfn][offset];
            printf("\nVirtual index: %u Physical address: %u Value: %d\n",vaddress,paddress,requested);
            return true;
        }  
    }
    //CASO: MISS
    //Cargar proceso desde BACKING_STORE a un frame
    backingStoreData(vpn,indice_de_memoria,offset);
    //Guardar página en tabla de páginas
    Page * pg = create_page(vpn,indice_de_memoria);
    requested = memoria_principal[indice_de_memoria][offset];
    paddress = indice_de_memoria*256+offset;
    printf("\nVirtual index: %u Physical address: %u Value: %d\n",vaddress,paddress,requested);
    indice_de_memoria++;
    LIST_INSERT_HEAD(&tabla_de_paginas, pg, pptrs);
    
    
    return false;
}
void backingStoreData(unsigned int vpn, unsigned int pfn, unsigned int offset){
    //OFFSET DEL BACKING_STORE
    int file_offset = vpn * 256;
    //PROCESO DEL BACKING_STORE
    char num[FRAME_SIZE];
    //UBICAR EL OFFSET
    fseek(backing_store, file_offset, SEEK_SET);
    //LEER 256 CARACTERES DESDE EL OFFSET
    fread(&num, 1, 256, backing_store);
    //reseteo el puntero del archivo
    rewind(backing_store);
    //Lleno la posición de memoria con la data del frame
    for (size_t i = 0; i < FRAME_SIZE; i++)
    {
        memoria_principal[pfn][i]=(int)num[i];
    }
        
}

unsigned int translate(unsigned int address){

    unsigned int p_num=address>>OFFSET;
    unsigned int ofs=address%PAGE_SIZE;

    bool hom =insertOrCreatePage(p_num,ofs);
    /*
    //Insertar o crear página
    if(hom)
        printf("    \nHIT   page number %u\n",p_num);
    else
        printf("    \nMISS   page number %u\n",p_num);
    */
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

