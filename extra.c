//
// Created by sistemas on 4/06/22.
//

#include <stdio.h>
#include <stdlib.h>

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

void dispose(Symbol * list){
    Symbol * l = last(list);
    while(l != NULL){
        Symbol  * prev = l->prev;
        prev->next = NULL;
        free(l);
        l = prev;
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