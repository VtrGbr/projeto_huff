#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca/unity.h"

#define TAM 256
typedef unsigned long long int ulli; 
typedef unsigned char uchar; 

typedef struct stack {
    int valor;
    struct stack* prox;
} Stack;

typedef struct fila {
    int valor;
    struct fila* prox;
} Fila;

typedef struct list {
    int valor;
    struct list* prox;
} List;

typedef struct tree {
    int valor;
    struct tree* dir, *esq;
} Tree;

typedef struct no {
    uchar byte;
    ulli frequencia;
    struct no* dir, *esq, *prox;
} No;

typedef struct lista {
    No* inicio;
    int tam;
} Lista;

void setUp() {}
void tearDown() {}

No* criar_noh(uchar byte, ulli frequencia) {
    No* new_noh = (No*)malloc(sizeof(No));
    if (new_noh != NULL) {
        new_noh->byte = byte;
        new_noh->frequencia = frequencia;
        new_noh->dir = NULL;
        new_noh->esq = NULL;
        new_noh->prox = NULL;
    } else {
        printf("Falha na criação do nó\n");
    }
    return new_noh;
}

// Função para inserir na pilha
Stack* push(Stack** pilha, int valor) {
    Stack* stac = (Stack*)malloc(sizeof(Stack));
    if (stac != NULL) {
        stac->valor = valor;
        stac->prox = *pilha;
        *pilha = stac;
    }
    return *pilha;
}

// Função para inserir na fila
void inserir_fila(Fila** queue, int dado) {
    Fila* novo = (Fila*)malloc(sizeof(Fila));
    if (novo != NULL) {
        novo->valor = dado;
        novo->prox = NULL;

        if (*queue == NULL) {
            *queue = novo;
        } else {
            Fila* aux = *queue;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = novo;
        }
    } else {
        printf("Falha na alocação!\n");
    }
}

// Função para inserir na lista "normal"
List* add(List* cabeca, int item) {
    List* new_no = (List*)malloc(sizeof(List));
    if (new_no != NULL) {
        new_no->valor = item;
        new_no->prox = cabeca;
    }
    return new_no;
}

// Função para remover da pilha
Stack* pop(Stack** pilha) {
    if (*pilha != NULL) {
        Stack* aux = *pilha;
        *pilha = aux->prox;
        free(aux);
    } else {
        printf("Pilha vazia\n");
    }
    return *pilha;
}

// Função para remover da fila
Fila* desinfileirar(Fila** queue) {
    Fila* rem = NULL;
    if (*queue != NULL) {
        rem = *queue;
        *queue = rem->prox;
    } else {
        printf("Fila vazia!\n");
    }
    return rem;
}

// Função para remover da lista "normal"
List* remover(List* cabeca, int item) {
    List* previa = NULL;
    List* corrente = cabeca;

    while (corrente != NULL && corrente->valor != item) {
        previa = corrente;
        corrente = corrente->prox;
    }

    if (corrente == NULL) {
        return cabeca;
    }

    if (previa == NULL) {
        cabeca = corrente->prox;
    } else {
        previa->prox = corrente->prox;
    }

    free(corrente);
    return cabeca;
}

// Função para testes da pilha
void teste_pilha() {
    Stack* pilha = NULL;

    pilha = push(&pilha, 10);
    pilha = push(&pilha, 5);
    pilha = push(&pilha, 7);

    TEST_ASSERT_EQUAL(5, pop(&pilha)->valor);
    TEST_ASSERT_EQUAL(10, pop(&pilha)->valor);
    TEST_ASSERT_EQUAL(7, pop(&pilha)->valor);
}

// Função para testes da fila
void teste_fila() {
    Fila* queue = NULL;

    inserir_fila(&queue, 3);
    inserir_fila(&queue, 5);
    inserir_fila(&queue, 4);

    TEST_ASSERT_EQUAL(3, desinfileirar(&queue)->valor);
    TEST_ASSERT_EQUAL(5, desinfileirar(&queue)->valor);
    TEST_ASSERT_EQUAL(4, desinfileirar(&queue)->valor);
}

// Função para testes da lista "normal"
void teste_lista_normal() {
    List* lista = NULL;

    lista = add(lista, 3);
    lista = add(lista, 10);
    lista = add(lista, 50);

    lista = remover(lista, 10);
    TEST_ASSERT_EQUAL(50, lista->valor);

    lista = remover(lista, 3);
    TEST_ASSERT_EQUAL(50, lista->valor);

    lista = remover(lista, 50);
    TEST_ASSERT_NULL(lista);  // A lista deve estar vazia no final
}

// Função principal para rodar os testes
int main() {
    UNITY_BEGIN();

    RUN_TEST(teste_pilha);
    RUN_TEST(teste_fila);
    RUN_TEST(teste_lista_normal);

    return UNITY_END();
}
