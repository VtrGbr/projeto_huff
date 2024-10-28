#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct heap{
    int *valores;
    int tamanho;
    int capacidade;
    int comparacoes;
}Heap;

typedef struct no{
    int valor;
    struct no* prox;
}No;

typedef struct filaPri{
    No* cabeca;
    int comparacoes;
}FilaPri;

/*
Criacoes da fila de prioridade com heap e da fila de prioridade sem heap
*/

Heap* criar_heap(int capacidade){
    //Alocacao de memoria
    Heap* heap = (Heap*)malloc(sizeof(Heap));

    //Verificacao de seguranca
    if( heap != NULL){
        heap->valores = (int*)calloc(capacidade,sizeof(int)); // Alocando memoria para o vetor de valores
        //Verificacao de seguranca
        if( heap->valores != NULL){
            heap->tamanho = 0;
            heap->capacidade = capacidade;
            heap->comparacoes = 0;

            return heap;
        }else{
            printf("Falha no vetor de valores\n");
            return NULL;
        }
    }else{
        printf("Falha na alocacao da heap\n");
        return NULL;
    }
}


FilaPri* criar_fila(){
    FilaPri* fila = ( FilaPri*)malloc(sizeof(FilaPri));

    //Verificacao de seguranca
    if( fila != NULL){
        fila->cabeca = NULL;
        fila->comparacoes = 0;

        return fila;
    }else{
        printf("Falha na alocacao de memoria da fila de prioridade sem heap\n");
        return NULL;
    }
}

/*
Funcoes para insercao e remocao dos valores numa heap e numa fila sem heap

*/

//Funcao para pegar o pai
int pegar_pai(Heap *heap,int i){
    return (i - 1)/2;
}

void swap(int* valor1, int* valor2) {
    int aux = *valor1;
    *valor1 = *valor2;
    *valor2 = aux;
}

void inserir_heap(Heap* heap, int valor){
    //Chegou na capacidade total
    if( heap->tamanho == heap->capacidade){
        return; // Pare o processo
    }else{

        int i = heap->tamanho++;
        heap->valores[i] = valor;

        while (i > 0)
        {
            int pai = pegar_pai(heap,heap->tamanho);
            heap->comparacoes++;

            if( heap->valores[i] >= heap->valores[pai]){
                break;
            }

            swap(&heap->valores[i],&heap->valores[pai]);
            i = pai;
           
        }
    }

}

void remover_heap(Heap* heap){
    //Fila vazia
    if( heap->tamanho == 0){
        return;
    }
    int remover = heap->valores[0];
    heap->valores[0] == heap->valores[--heap->tamanho];

    int i = 0;

    while( 2 * i + 1 < heap->tamanho){
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        int menor = esq;

        heap->comparacoes++;

        if( dir < heap->tamanho && heap->valores[dir] < heap->valores[esq]){
            menor = dir;
        }

        heap->comparacoes++;
        if( heap->valores[i] <= heap->valores[menor]){
            break;
        }

        swap(&heap->valores[i],&heap->valores[menor]);

        i = menor;
    }
    

}

void inserir_fila(FilaPri* fila, int valor){
    No* queue = (No*)malloc(sizeof(No));

    //Verificacao de seguranca
    if( queue != NULL){
        queue->valor = valor;
        queue->prox = NULL;
    }else{
        printf("Falha na alocacao da fila, na insercao");
        return;
    }

    if( fila->cabeca == NULL || fila->cabeca->valor >= valor){
        queue->prox = fila->cabeca;
        fila->cabeca  = queue;
        return;
    }

    No* aux = fila->cabeca;
    while( aux->prox != NULL && aux->prox->valor < valor){
        fila->comparacoes++;
        aux = aux->prox;
    }

    fila->comparacoes++;
    queue->prox = aux->prox;
    aux->prox = queue;
}


void remover_fila(FilaPri* fila){
    //Fila vazia
    if( fila->cabeca == NULL){
        return;
    }

    int valor = fila->cabeca->valor;

    No* aux = fila->cabeca;
    fila->cabeca = fila->cabeca->prox;
    free(aux);


}




int main() {
    Heap* heap = criar_heap(100);
    FilaPri* fila = criar_fila();
    int n = 1000;
    FILE* resultados;

    resultados = fopen("valores.txt", "w");

    if (resultados == NULL) {
        perror("Falha ao abrir o arquivo");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int valor = rand() % 10000;

        inserir_heap(heap, valor);
        inserir_fila(fila, valor);

        fprintf(resultados, "%d %d %d\n", i + 1, heap->comparacoes, fila->comparacoes);
    }

    for (int i = 0; i < n; i++) {
        remover_heap(heap);
        remover_fila(fila);
    }

   // fprintf(resultados, "Total de comparacoes na fila de prioridade com heap: %d\n", heap->comparacoes);
   // fprintf(resultados, "Total de comparacoes na fila de prioridade sem heap: %d\n", fila->comparacoes);

    fclose(resultados);

 //   printf("Comparacoes na fila de prioridade com heap: %d\n", heap->comparacoes);
  //  printf("Comparacoes na fila de prioridade sem heap: %d\n", fila->comparacoes);

    free(heap->valores);
    free(heap);
    free(fila);

    return 0;
}

