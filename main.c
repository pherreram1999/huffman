#include <stdio.h>
#include <stdlib.h>

/**
 * Structs para el los caracteres
 */
typedef struct symbol {
    // el caracgter en cuestion
    char character;
    int ascii;
    // cuantas veces a parce este caraccter en el archivo
    int frequency;
    // levar contando cuantos nodos vamos econtrando
    int * len;
    // para saber si este nodo ya sido contabilizadp
    struct symbol * next; // node next list element
    struct symbol * prev; // node next list element
    int index;
} Symbol;

/**
 * Struct para el arbol
 */

typedef struct node {
    Symbol * simbolo; // del padre que proviene
    int peso; // la suam de sus pesos,
    int bit;
    int busy;
    struct node * izq; // hijo izquierdo
    struct node * der; // hijo derecho
} Node;

/**
 * Crea un nuevo nodo de la lista
 * @param c
 * @return
 */
Symbol * new(char c,int index,int  ascii){
    Symbol * s = (Symbol *) malloc(sizeof(Symbol));
    s->index = index;
    s->next = NULL;
    s->prev = NULL;
    s->len = NULL;
    s->frequency = 1;
    s->character = c;
    s->ascii = ascii;

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
short int add(Symbol ** list,Symbol * s){
    // si el padre es null, entonces, es nuestro primer elementro
    if(* list == NULL){
        * list = s;
        // como es nuestro primer nodo, inciado la longitud de arreglo en 1
        (*list)->len = (int *) malloc(sizeof(int));
        *(*list)->len = 1;
        return 1;
    }
    // nos vamos al utlimo nodo
    Symbol * l = last(* list);
    l->next = s;
    // sumamos uno a la longitud y asignamos la direccion de memoria al siguiente nodo (para que sea el mismo contador en memoria)
    (*l->len)++;
    l->next->len = l->len;
    s->prev = l;
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

    while(nav != NULL){
        if(nav->character == c){
            return nav;
        }
        nav = nav->next;
    }
    return NULL;
}

/**
 * Imprime un array de simbolos
 * @param simbolos
 * @param n
 */
void printArray(Symbol * simbolos,int n){
    for(int i = 0; i < n; i++){
        printf("assci: %d \t|\tFrencuencia: %d \t|\t caracter: '%c'\n",simbolos[i].ascii,simbolos[i].frequency,simbolos[i].character);
    }
}

/**
 * Convierte una lista en array
 * @param nav
 * @return
 */
Symbol * toArray(Symbol * nav){
    Symbol * arr = (Symbol * ) malloc( (*(nav->len)) * sizeof (Symbol));
    for(int i = 0; i < (*(nav->len)); i++){
        arr[i] = *nav;
        if(nav->next!=NULL){
            nav = nav->next;
        }else {
            nav = nav;
        }
    }
    return arr;
}


void swap(Symbol * a, Symbol * b){
    Symbol aux = *a;
    *a = *b;
    *b = aux;
}

int partition(Symbol * simbolos,int start, int final){
    Symbol  pivot = simbolos[final];
    int i = (start - 1);
    for(int j = start; j <= final - 1; j++){
        if(simbolos[j].frequency < pivot.frequency){
            i++;
            swap(&simbolos[i],&simbolos[j]);
        }
    }
    swap(&simbolos[i + 1],&simbolos[final]);
    return i + 1;
}

void quickSort(Symbol * simbolos,int start,int final){
    if(start < final){
        int pi = partition(simbolos,start,final);
        quickSort(simbolos,start, pi - 1);
        quickSort(simbolos,pi + 1,final);
    }
}

/**
 * Lee un archivo y genera una lista con la frecuencia decada una de los caracteres
 * @param path = directorio del archivo a leer
 * @return
 */
Symbol * generarListaSimbolos(char * path){
    FILE * file = fopen(path,"r");
    if(file == NULL){
        printf("No fue posible abrir el archivo");
        return NULL;
    }
    Symbol * list = NULL;
    Symbol * symbol = NULL;
    int assci; // ascci number position
    char character; // char value to save
    int index;
    while((assci = fgetc(file)) != EOF){
        character = (char) assci;
        symbol = search(&list,character);
        if(symbol != NULL){
            symbol->frequency++;
        } else {
            symbol = last(list);
            index = symbol != NULL ? symbol->index + 1 : 0;
            add(&list, new(character,index,assci));
            symbol = NULL;
        }
    }
    return list;
}



int main(int argc,char ** args) {
    if(argc != 2){
        printf("\nPor favor propociona el path del archivo :|\n");
        exit(1);
    }

    char * filename = args[1];

    printf("\nfilename: %s\n",filename);
    // creamos nuestra list symbols
    Symbol * list = generarListaSimbolos(filename);
    if(list == NULL){
        printf("\nNO fue posible generar la lista, verique el path\n");
        exit(1);
    }
    Symbol * simbolos = toArray(list);
    int n =  *simbolos[0].len;
    quickSort(simbolos,0,n - 1);
    printArray(simbolos,n);
    // imprimimos nuestra lista


    return 0;
}


