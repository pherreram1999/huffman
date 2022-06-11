// Huffman Coding in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 50

typedef struct symbol {
    // el caracgter en cuestion
    unsigned char character;
    int ascii;
    // cuantas veces a parce este caraccter en el archivo
    int frequency;
    // levar contando cuantos nodos vamos econtrando
    int * len;
    // llevar el codigo coficado
    //char codficacion[8];
    // para saber si este nodo ya sido contabilizadp
    struct symbol * next; // node next list element
    struct symbol * prev; // node prev list element
    // tambien nos funcionara como arbol
    struct symbol * izqUno, * derCero;
    int index;
} Symbol;

typedef struct codigo {
    char codigo;

} Codigo;

/**
 * Almacena los caracteres y fungue como arbol
 */
struct Caracter {
    char simbolo;
    unsigned freq;
    char * codigo;
    // hijos del nodo
    struct Caracter * izq, * der;
};

/**
 * Nos ayuda a generar los codigos
 */
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct Caracter **array;
};


struct Caracter * crearCaracter(char item, unsigned freq) {
    struct Caracter *temp = (struct Caracter *)malloc(sizeof(struct Caracter));

    temp->codigo = NULL;
    temp->izq = temp->der = NULL;
    temp->simbolo = item;
    temp->freq = freq;

    return temp;
}

// Create min heap
struct MinHeap * createMinH(unsigned capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array = (struct Caracter **) malloc(minHeap->capacity * sizeof(struct Caracter *));
    return minHeap;
}

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

Symbol * last(Symbol * nav){
    if(nav == NULL)
        return NULL;
    while(nav->next != NULL){
        nav = nav->next;
    }
    return nav;
}

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

char * convertirToSimbolos(Symbol * nav){
    char * arr = (char * ) malloc( (*(nav->len)) * sizeof (char));
    for(int i = 0; i < (*(nav->len)); i++){
        arr[i] = nav->character;
        if(nav->next!=NULL){
            nav = nav->next;
        }else {
            nav = nav;
        }
    }
    return arr;
}

void printArrSimb(Symbol * nav,char arr[]){
    for(int i = 0; i < (*(nav->len)); i++)
        printf("%c",arr[i]);
}

int * convertirToFrencuencias(Symbol * nav){
    int * arr = (int * ) malloc( (*(nav->len)) * sizeof (int));
    for(int i = 0; i < (*(nav->len)); i++){
        arr[i] = nav->frequency;
        if(nav->next!=NULL){
            nav = nav->next;
        }else {
            nav = nav;
        }
    }
    return arr;
}

void printArrfrec(Symbol * nav,int arr[]){
    for(int i = 0; i < (*(nav->len)); i++)
        printf("%d,",arr[i]);
}
// Function to swap
void intercarbiarNodos(struct Caracter **a, struct Caracter **b) {
    struct Caracter *t = *a;
    *a = *b;
    *b = t;
}

// Heapify
void minHeapify(struct MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        intercarbiarNodos(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Check if size if 1
int longitudEnUno(struct MinHeap *minHeap) {
    return (minHeap->size == 1);
}

// Extract min
struct Caracter *extractMin(struct MinHeap *minHeap) {
    struct Caracter *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

// Insertion function
void insertMinHeap(struct MinHeap *minHeap, struct Caracter *minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

int esHoja(struct Caracter *root) {
    return !(root->izq) && !(root->der);
}

struct MinHeap *createAndBuildMinHeap(char item[], int freq[], int size) {
    struct MinHeap *minHeap = createMinH(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = crearCaracter(item[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

struct Caracter * construirArbol(char item[], int freq[], int size) {
    struct Caracter *left, *right, *top;
    struct MinHeap *minHeap = createAndBuildMinHeap(item, freq, size);

    while (!longitudEnUno(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = crearCaracter('$', left->freq + right->freq);

        top->izq = left;
        top->der = right;

        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}



void printArray(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);

    printf("\n");
}

char * arrayToChar(int arr[], int n){
    char * tmp = (char *) malloc(MAX_TREE_HT * sizeof(char));
    tmp = "";
    for (int i = 0; i < n; i++){
        char simbolo =  (arr[i] + '0');
        strcat(tmp,&simbolo);
    }
    return tmp;
}


void imprimirCodigos(struct Caracter * simbolo, int arr[], int top) {
    if (simbolo->izq) {
        arr[top] = 0;
        imprimirCodigos(simbolo->izq, arr, top + 1);
    }
    if (simbolo->der) {
        arr[top] = 1;
        imprimirCodigos(simbolo->der, arr, top + 1);
    }
    if (esHoja(simbolo)) {

        printf("  %c  | ",simbolo->simbolo);
        printArray(arr,top);
    }
}

FILE * crearArchivoDiccionario(){
    FILE * fptr = fopen("diccionario.txt","w");
    fprintf(fptr,"\0"); // imprimimos NADA para vaciar el archivo si ya existia
    fclose(fptr);
    // ahora lo abrimos en append mode
    return fopen("diccionario.txt","a");
}

void guardarCodigos(struct Caracter * simbolo, int arr[], int top,FILE * archivo){
    if (simbolo->izq) {
        arr[top] = 0;
        guardarCodigos(simbolo->izq, arr, top + 1,archivo);
    }
    if (simbolo->der) {
        arr[top] = 1;
        guardarCodigos(simbolo->der, arr, top + 1,archivo);
    }
    if (esHoja(simbolo)) {
        fprintf(archivo,"%d=", simbolo->simbolo);
        for(int i = 0; i < top; i++){
            fprintf(archivo,"%d",arr[i]);
        }
        fprintf(archivo,"\n");
        //printArray(arr, top);

    }
}



// Print the array

int main(int argc,char ** args) {

    if(argc != 2){
        printf("\nPor favor propociona el path del archivo :|\n");
        exit(1);
    }

    char * filename = args[1];
    printf("\nfilename: %s\n",filename);
    Symbol * list = generarListaSimbolos(filename);
    if(list == NULL){
        printf("\nNO fue posible generar la lista, verique el path\n");
        exit(1);
    }

    char * simbolos = convertirToSimbolos(list);
    int * frecuencias = convertirToFrencuencias(list);
    int longitud = *(list->len);

    struct Caracter * arbol = construirArbol(simbolos, frecuencias, longitud);
    int arr[MAX_TREE_HT], top = 0;


    //guardarCodigos(arbol, arr, top,crearArchivoDiccionario());


    /*printf(" Char | Huffman code ");
    printf("\n--------------------\n");
    imprimirCodigos(arbol, arr, top);*/
    // guaradamos el diccionar
    guardarCodigos(arbol,arr,top,crearArchivoDiccionario());

    return 0;
}