#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Structs para el los caracteres
 */
typedef struct symbol {
    // el caracgter en cuestion
    unsigned char character;
    int ascii;
    // cuantas veces a parce este caraccter en el archivo
    int frequency;
    // llevar el codigo coficado
    char codficacion[15];
    // para saber si este nodo ya sido contabilizadp
    struct symbol * next, * prev; // node next list element
    // tambien nos funcionara como arbol
    struct symbol * izqUno, * derCero;
} Symbol;

typedef struct tabla {
    unsigned char caracter;
    char codigo[15];
    unsigned long int bits; // aqui guardamos la codificacion
    unsigned char nbits; // numero de bists de la codificacion,
    struct tabla * next;
} Tabla;


/**
 * Crea un nuevo nodo de la lista
 * @param c
 * @return
 */
Symbol * nuevoNodo(int ascii){
    Symbol * s = (Symbol *) malloc(sizeof(Symbol));
    s->next = NULL;
    s->izqUno = NULL;
    s->derCero = NULL;
    s->frequency = 1;
    s->character =  (unsigned char) ascii;
    s->ascii = ascii;
    return s;
}


/**
 * Retorna el ultimo nodo de una lista
 * @param nav
 * @return
 */
Symbol * getUltimoElemento(Symbol * nav){
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
short int insertarEnLista(Symbol ** list,Symbol * s){
    // si el padre es null, entonces, es nuestro primer elementro
    if(*list == NULL){
        * list = s;
        // como es nuestro primer nodo, inciado la longitud de arreglo en 1
        return 1;
    }
    // nos vamos al utlimo nodo
    Symbol * l = getUltimoElemento(*list);
    s->prev = l;
    l->next = s;

    // sumamos uno a la longitud y asignamos la direccion de memoria al siguiente nodo (para que sea el mismo contador en memoria)
    return 1;
}

/**
 * Busca el nodo de un simbolo (char) en la lista y lo regresa
 * @param list = la lista
 * @param c = el char que se busca
 * @return = regresa el nodo, si no lo encuentra retorna NULL;
 */
Symbol * buscarCaracter(Symbol ** list, unsigned char c){
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

void printList(Symbol * nav){
    if(nav == NULL){
        printf("\nLista vacia :O\n");
        return;
    }
    printf("<---------->\n");
    while(nav != NULL){
        printf("assci: %d \t|\tFrencuencia: %d \t|\t caracter: '%c'\n",nav->ascii,nav->frequency,nav->character);
        nav = nav->next;
    }
}




/**
 * Lee un archivo y genera una lista con la frecuencia decada una de los caracteres
 * @param path = directorio del archivo a leer
 * @return
 */
Symbol * generarListaSimbolos(char * path,int * contador){
    FILE * file = fopen(path,"r");
    if(file == NULL){
        printf("No fue posible abrir el archivo");
        return NULL;
    }
    Symbol * list = NULL;
    Symbol * symbol = NULL;
    int ascii; // ascci number position
    while((ascii = fgetc(file)) != EOF){
        symbol = buscarCaracter(&list, ascii);
        if(symbol != NULL){
            symbol->frequency++;
        } else {
            insertarEnLista(&list, nuevoNodo(ascii));
            (*contador)++;
        }
    }
    fclose(file);
    return list;
}

int insertarOrden(Symbol ** head,Symbol * nodoInsertar){
    Symbol * nav = *head;
    Symbol * nodoActual = NULL;
    if(*head == NULL){
        *head = nodoInsertar;
        return 0;
    }

    while(nav->next != NULL && nav->frequency < nodoInsertar->frequency){
        nav = nav->next;
    }

    if(nav->frequency <= nodoInsertar->frequency){
        Symbol * prev = nav->prev;
        nodoInsertar->prev = prev;
        nodoInsertar->next = nav;
    } else {
        nav->next = nodoInsertar;
    }

}

Symbol * pop(Symbol * nav){
    while (nav->next->next != NULL){
        nav = nav->next;
    }
    Symbol * prev = nav;
    nav = nav->next;
    prev->next = NULL;
    return nav;
}



int esHoja(Symbol * nodo){
    if(nodo->izqUno == NULL && nodo->derCero == NULL)
        return 1;
    return 0;
}

void recorrerPreorden(Symbol * s){
    if(s != NULL){
        if(esHoja(s)){
            printf("Frecuencia: %d  \t|\t Caracter: %c\n",s->frequency,s->character);
        }
        recorrerPreorden(s->izqUno);
        recorrerPreorden(s->derCero);
    }
}


/**
 * Convierte una lista en array
 * @param nav
 * @return
 */
Symbol * toArray(Symbol * nav,int n){
    Symbol * arr = (Symbol * ) malloc( n * sizeof (Symbol));
    for(int i = 0; i < n; i++){
        arr[i] = *nav;
        if(nav->next!=NULL){
            nav = nav->next;
        }else {
            nav = nav;
        }
    }
    return arr;
}



void sacarNodo(Symbol * nodo){
    Symbol * next = nodo->next;
    nodo->next = NULL;
}

Symbol * generalArbol(Symbol ** lista){
    while((*lista)->next != NULL){
        Symbol * nodoIzq = (*lista);
        Symbol * nodoDer = (*lista)->next;
        *lista = (*lista)->next->next;
        nodoIzq->next = NULL;
        nodoDer->next = NULL;

        // sacamos los nodos de la lista
        // creamos un nuevo nodo como padre de los primeros
        Symbol * padre = (Symbol *) malloc(sizeof(Symbol));
        padre->izqUno = nodoIzq;
        padre->derCero = nodoDer;
        padre->ascii = 0;
        padre->next = NULL;
        padre->frequency = padre->izqUno->frequency + padre->derCero->frequency;
        padre->character = 0; // no es un caracter real
        //printList(*lista);
        insertarOrden(&(*lista),padre);
    }
    return *lista;
}


void printTable(Tabla * t){
    Tabla * nav = t;
    while(nav){
        printf("Caracter: %c \t|\t codigo: %s\n",nav->caracter,nav->codigo);
        nav = nav->next;
    }
}

void ordenar(Symbol ** lista){
    Symbol * nav, *listaTmp;
    if(*lista == NULL)
        return;
    listaTmp = *lista;
    *lista = NULL;
    while(listaTmp != NULL){
        nav = listaTmp;
        listaTmp = nav->next;
        nav->next = NULL;
        insertarOrden(lista,nav);
    }

}


/**
 * de aun arreglo general la lista de nodos
 * @param simbolos
 * @param n
 * @return
 */
Symbol * toList(Symbol * simbolos,int n){
    for(int i = 0; i < n; i++){
        Symbol * s = &simbolos[i];
        if(i == 0){
            s->prev = NULL;
        } else {
            s->prev = &(simbolos[i - 1]);
        }
        s->next = &(simbolos[i + 1]);
    }
    Symbol * penultimo = &simbolos[n - 1];
    penultimo->next = NULL;
    return &simbolos[0];
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

int main(int argc,char ** args) {
    if(argc != 2){
        printf("\nPor favor propociona el path del archivo :|\n");
        exit(1);
    }
    char * filename = args[1];

    printf("\nfilename: %s\n",filename);
    // creamos nuestra list symbols
    int noElementos = 0;
    Symbol * list = generarListaSimbolos(filename,&noElementos);
    Symbol * arr = toArray(list,noElementos);
    quickSort(arr,0,noElementos - 1);
    list = toList(arr,noElementos);
    return 0;
}


