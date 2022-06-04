#include <stdio.h>
#include <stdlib.h>

typedef struct symbol {
    // el caracgter en cuestion
    char character;
    // cuantas veces a parce este caraccter en el archivo
    int frequency;
    // levar contando cuantos nodos vamos econtrando
    int * len;
    struct symbol * next; // node next list element
    struct symbol * prev; // node next list element
    int index;
} Symbol;

/**
 * Crea un nuevo nodo de la lista
 * @param c
 * @return
 */
Symbol * new(char c,int index){
    Symbol * s = (Symbol *) malloc(sizeof(Symbol));
    s->next = NULL;
    s->prev = NULL;
    s->len = NULL;
    s->frequency = 1;
    s->character = c;
    s->index = index;
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
    while(nav != NULL){
        printf("%d - Caracter: '%c' => Frecuencia: %u\n",nav->index,nav->character,nav->frequency);
        nav = nav->next;
    }
}

Symbol * subarray(Symbol * arr, int start, int max){
    int len = max - start;
    Symbol * newarr = (Symbol *) malloc(len * sizeof(Symbol));
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
		if(nav->next!=NULL)
			nav = nav->next;
		else
			nav=nav;
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

}

Symbol * mergeSort(Symbol * arr,int n) {
    // el elemento mas sencillo
    if (n == 1)
        return arr;
    int middle = n / 2;
    int lenright = (n % 2) == 0 ? middle : middle + 1;

    Symbol * left = subarray(arr, 0, middle);
    Symbol * right = subarray(arr, middle, n);

    Symbol * sortedLeft = mergeSort(left,middle);
    Symbol * sortedRight = mergeSort(right,lenright);

    return merge(sortedLeft,sortedRight,middle,lenright);
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

    while((assci = fgetc(file)) != EOF){
        character = (char) assci;
        symbol = search(&list,character);
        if(symbol != NULL){
            symbol->frequency++;
        } else {
            symbol = last(list);
            add(&list, new(character,symbol != NULL ? symbol->index + 1 : 0));
            symbol = NULL;
        }
    }

    int len = *(list->len);
    printf("\nlongitud del arreglo: %d\n",len);
    Symbol * simbolos = toArray(list);
    for(int i = 0; i < len; i++){
        Symbol s = simbolos[i];
        printf("%d - Caracter: '%c' => Frecuencia: %u\n",s.index,s.character,s.frequency);
    }

    //mergeSort(simbolos,len);

    return 0;
}