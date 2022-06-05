#include <stdio.h>
#include <stdlib.h>



/**
 * Structs para el los caracteres
 */
typedef struct symbol {
    // el caracgter en cuestion
    unsigned char character;
    int ascii;
    // cuantas veces a parce este caraccter en el archivo
    int frequency;
    // levar contando cuantos nodos vamos econtrando
    int * len;
    // llevar el codigo coficado
    char codficacion[8];
    // para saber si este nodo ya sido contabilizadp
    struct symbol * next; // node next list element
    struct symbol * prev; // node prev list element
    // tambien nos funcionara como arbol
    struct symbol * izqUno, * derCero;
    int index;
} Symbol;

typedef struct tabla {
    unsigned char caracter;
    unsigned long int bits; // aqui guardamos la codificacion
    unsigned char nbits; // numero de bists de la codificacion,
    struct tabla * next;
} Tabla;


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
    s->izqUno = NULL;
    s->derCero = NULL;
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
    s->izqUno = NULL;
    s->derCero = NULL;
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

void printList(Symbol * nav){
    if(nav == NULL){
        printf("\nLista vacia :O\n");
        return;
    }
    printf("Lista de longitud: %d => \n",*(nav->len));
    while(nav != NULL){
        printf("%d - assci: %d \t|\tFrencuencia: %d \t|\t caracter: '%c'\n",nav->index,nav->ascii,nav->frequency,nav->character);
        nav = nav->next;
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
        s->index = i;
        s->next = &(simbolos[i + 1]);
    }
    Symbol * penultimo = &simbolos[n - 1];
    penultimo->next = NULL;
    return &simbolos[0];
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
    fclose(file);
    return list;
}

int insertarOrden(Symbol ** head,Symbol * nodoInsertar){
    if(*head == NULL){
        *head = nodoInsertar;
        (*head)->next = NULL;
        return 1;
    }
    Symbol * nav,* element;
    nav = *head;
    // buscamos el elemento dentro de las frecuencias
    while(nav && nav->frequency < nodoInsertar->frequency){
        element = nav; // guardamos el elemento que estemos recorriendo
        nav = nav->next; // pasamos al siguiente nodo
    }
    // insertamos el elemento
    nodoInsertar->next = nav;
    if(element){
        // insertar nodo
        nodoInsertar->index = element->index + 1;
        element->next = nodoInsertar;
        element->index--;
    } else {
        // nuevo nodo
        *head = nodoInsertar;
    }
}

void recorrerPreorden(Symbol * s){
    if(s == NULL)
        return;
    printf("Frecuencia: %d\n",s->frequency);
    recorrerPreorden(s->izqUno);
    recorrerPreorden(s->derCero);
}


Symbol * maketree(Symbol ** raiz){
    Symbol * arbol = *raiz;
    Symbol * nuevoNodo = NULL;
    // mientras aun tenganos minimo dos nodos
    while (arbol && arbol->next){
        //printf("index: %d | frecuencia: %d | assci: %d\n",arbol->index,arbol->frequency,arbol->character);
        nuevoNodo = (Symbol *) malloc(sizeof(Symbol));
        nuevoNodo->character = 0; // no tiene una letra en especfico
        nuevoNodo->izqUno = arbol; // el nodo actual es el hijo izquierdo con carga de uno
        arbol = arbol->next; // recorremos el arbol
        nuevoNodo->derCero = arbol; // el siguiente es el hijo derecho
        arbol = arbol->next;
        // sumamos las frecuencias los nodos en el nuevo nodo
        nuevoNodo->frequency = nuevoNodo->izqUno->frequency + nuevoNodo->derCero->frequency;
        //recorrerPreorden(nuevoNodo);
        //recorrerPreorden(nuevoNodo);
        // por ultimo lo insertamos en el arbol en orden
        insertarOrden(raiz,nuevoNodo);
    }
}

void insertarTabla(unsigned char c,int l,int v,Tabla ** t){
    Tabla * nuevaTabla,* nav,* element;
    nuevaTabla = (Tabla *) malloc(sizeof(Tabla));
    nuevaTabla->caracter = c;
    nuevaTabla->bits = v;
    nuevaTabla->nbits = 1;
    if(*t == NULL){
        *t = nuevaTabla;
        (*t)->next = NULL;
    } else {
        nav = *t;
        element = NULL;
        // nos localizamos en medio
        while(nav && nav->caracter < nuevaTabla->caracter){
            element = nav;
            nav = nav->next;
        }
        // insertamos el elemento
        nuevaTabla->next = nav;
        if(element != NULL){
            element->next = nuevaTabla;
        } else {
            *t = nuevaTabla;
        }
    }
}

Tabla * buscarCaracter(Tabla * t,unsigned char c){
    Tabla * nav = t;
    while(nav && nav->caracter != c){
        nav = nav->next;
    }
    return nav;
}

void crearTabla(Symbol ** arbol,int l,int v,Tabla ** t) {
    Symbol * s = *arbol;
    if (s->izqUno) crearTabla(&s->izqUno, l + 1, (v << 1) | 1,t);
    if (s->derCero) crearTabla(&s->derCero, l + 1, v << 1,t);
    if(!s->izqUno && !s->derCero) insertarTabla(s->character,l,v,t);
}

void codificar(const char * path,const char * destiny,Tabla ** tabla){
    FILE * file = fopen(path,"r");
    FILE * compress = fopen(destiny,"wb");
    unsigned char c;
    unsigned int dword = 0;
    int nbits = 0;
    while((c = fgetc(file)) && !feof(file)){
        Tabla * t = buscarCaracter(*tabla,c);
        while(nbits + t->nbits > 32){
            c = dword >> (nbits - 8);
            fwrite(&c,sizeof(char),1,compress);
            nbits -= 8;
        }
        dword <<= t->nbits; // se hace espacio para el nuevo caracter
        dword |= t->nbits; // insertar nuevo caracgter
        nbits += 8;
    }

    while (nbits > 0){
        if(nbits >= 0){
            c = dword >> (nbits - 8);
        } else {
            c = dword << (8 - nbits);
        }
        fwrite(&c,sizeof(char),1,compress);
        nbits -= 8;
    }
    fclose(compress);
    fclose(file);
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
    Symbol * arbol = toList(simbolos,n);
    printList(arbol);
    maketree(&arbol);
    Tabla * codigos = NULL;
    crearTabla(&arbol,0,0,&codigos);
    return 0;
}


