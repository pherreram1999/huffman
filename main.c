#include <stdio.h>
#include <stdlib.h>

typedef struct symbol {
    // el caracgter en cuestion
    char character;
    // cuantas veces a parce este caraccter en el archivo
    unsigned int frequency;
    // levar contando cuantos nodos vamos econtrando
    unsigned int * len;
    struct symbol * next; // node next list element
} Symbol;

/**
 * Crea un nuevo nodo de la lista
 * @param c
 * @return
 */
Symbol * new(char c){
    Symbol * s = (Symbol *) malloc(sizeof(Symbol));
    s->next = NULL;
    s->len = NULL;
    s->frequency = 1;
    s->character = c;
    return s;
}


/**
 * Retorna el ultimo nodo de una lista
 * @param nav
 * @return
 */
Symbol * last(Symbol * nav){
    if(nav == NULL)
        return NULL;
    while(nav->next != NULL){
        nav = nav->next;
    }
    return nav;
}

/**
 * Agregar un nuevo simbolo a la lista
 * @param list = lista raiz
 * @param c = nuevo simbolo (char)
 * @return = retorna 1 si se inserto correctamente, 0 si no.
 */
short int add(Symbol ** list,char c){
    // si el padre es null, entonces, es nuestro primer elementro
    if(* list == NULL){
        * list = new(c);
        // como es nuestro primer nodo, inciado la longitud de arreglo en 1
        (*list)->len = (unsigned int *) malloc(sizeof(unsigned int));
        *(*list)->len = 0;
        return 1;
    }
    // nos vamos al utlimo nodo
    Symbol * l = last(* list);
    l->next = new(c);
    // sumamos uno a la longitud y asignamos la direccion de memoria al siguiente nodo (para que sea el mismo contador en memoria)
    (*(l->len))++;
    l->next->len = l->len;
    return 1;
}

/**
 * Busca el nodo de un simbolo (char) en la lista y lo regresa
 * @param list = la lista
 * @param c = el char que se busca
 * @return = regresa el nodo, si no lo encuentra retorna NULL;
 */
Symbol * search(Symbol ** list,char c){
    if(*list == NULL)
        return NULL;
    Symbol * nav = *list;

    while(nav->next != NULL){
        if(nav->character == c){
            return nav;
        }
        nav = nav->next;
    }
    return NULL;
}

void printList(Symbol * nav){
    if(nav == NULL){
        printf("\nLista vacia :O\n");
        return;
    }
    while(nav->next != NULL){
        printf("-- Caracter: '%c' => Frecuencia: %u\n",nav->character,nav->frequency);
        nav = nav->next;
    }
}

int main(int argc,char ** args) {
    if(argc != 2){
        printf("\nPor favor propociona el path del archivo :|\n");
        exit(1);
    }

    char * filename = args[1];
    printf("\nfilename: %s\n",filename);

    FILE * file = fopen(filename,"r");
    if(file == NULL){
        printf("No fue posible abrir el archivo");
        exit(1);
    }

    int assci; // ascci number position
    char character; // char value to save
    // creamos nuestra list symbols
    Symbol * list = NULL;
    Symbol * symbol = NULL;

    printf("leyendo caracteres");
    while((assci = fgetc(file)) != EOF){
        character = (char) assci;
        symbol = search(&list,character);
        if(symbol != NULL){
            symbol->frequency++;
        } else {
            add(&list,character);
        }
    }
    printf("longitud del arreglo: %d\n",*(list)->len);
    printList(list);
    return 0;
}
