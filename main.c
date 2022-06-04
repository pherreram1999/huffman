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

void printList(Symbol * nav){
    if(nav == NULL){
        printf("\nLista vacia :O\n");
        return;
    }
    printf("Lista de longitud: %d => \n",*(nav->len));
    while(nav != NULL){
        printf("%d - Caracter: '%c' => Frecuencia: %u => ascii: %d\n",nav->index,nav->character,nav->frequency,nav->ascii);
        nav = nav->next;
    }
}

void printArray(Symbol * simbolos,int n){
    for(int i = 0; i < n; i++){
        printf("assci: %d \t|\tFrencuencia: %d\n",simbolos[i].ascii,simbolos[i].frequency);
    }
}

Symbol * subarray(Symbol * arr, int start, int max){
    int len = max - start;
    Symbol * newarr = (Symbol *) malloc(len * (sizeof(Symbol)) );
    for(int i = 0; i < len; i++){
        newarr[i] = arr[start];
        start++;
    }
    return newarr;
}

Symbol * shift(Symbol * nav){
    if(nav->next != NULL){
        Symbol * next = nav->next;
        Symbol * prev = nav->prev;
        next->prev = NULL;
        (*(next->len))--;
        return prev;
    }
    return nav = NULL;
}

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

Symbol * merge(Symbol * left,Symbol * right, int nl,int nr) {
    int k = 0, j = 0, len = nr - nl,index = 0;
    Symbol * arr = (Symbol *) malloc(len * sizeof(Symbol));
    while(k < nl && j < nr){
        if(left[k].frequency <= right[j].frequency){
            arr[index] = left[k];
            k++;
        } else {
            arr[index] = right[j];
            j++;
        }
        index++;
    }

    while(k < nl){
        arr[index] = left[k];
        k++;
        index++;
    }

    while(j < nr){
        arr[index] = right[j];
        j++;
        index++;
    }
    return arr;
}

void dispose(Symbol * list){
    Symbol * l = last(list);
    while(l != NULL){
        Symbol  * prev = l->prev;
        prev->next = NULL;
        free(l);
        l = prev;
    }
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

Symbol smallSymbol(Symbol * arr, int n){
    Symbol small = arr[0];
    for (int i = 1; i < n; i++) {
        // si la frencia del chico es mayor al elemento
        // actual, entonces el elemento actual es mas chico
        if(small.frequency >  arr[i].frequency){
            small = arr[i];
        }
    }

    // ahora vamos por el segun mas chico, descartando el que ya econtramos
    return small;
}

Symbol * substract(Symbol * arr,int index,int n){
    Symbol * newarr = (Symbol *) malloc((n - 1) * sizeof(Symbol));
    Symbol s;
    int i = 0;
    int count = 0;
    while(i < n){
        if(i != index){
            s = arr[count];
            s.index = count;
            newarr[count] = s;
            count++;
        }
        i++;
    }
    s = newarr[0];
    (*s.len)--;
    free(arr);
    return newarr;
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
    int index;
    // creamos nuestra list symbols
    Symbol * list = NULL;
    Symbol * symbol = NULL;

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
    Symbol * simbolos = toArray(list);
    int n =  *simbolos[0].len;
    quickSort(simbolos,0,n - 1);
    printArray(simbolos,n);
    // imprimimos nuestra lista


    return 0;
}