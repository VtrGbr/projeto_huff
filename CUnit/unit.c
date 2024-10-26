#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h> 

#define TAM 256
typedef unsigned long long int ulli; 
typedef unsigned char uchar; 

typedef struct stack{
    int valor;
    struct stack* prox;
}Stack;
typedef struct fila{
    int valor;
    struct fila* prox;
}Fila;

typedef struct list{
    int valor;
    struct list* prox;
}List;

typedef struct tree{
    int valor;
    struct tree* dir,*esq;
}Tree;

typedef struct no{
    uchar byte;
    ulli frequencia;
    struct no* dir,*esq,*prox;
}No;

typedef struct lista{
    No* inicio;
    int tam;
}Lista;

No* criar_noh(uchar byte, ulli frequencia){
    No* new_noh = (No*)malloc(sizeof(No));

    if( new_noh != NULL){
        new_noh->byte = byte;
        new_noh->frequencia = frequencia;
        new_noh->dir = NULL;
        new_noh->esq = NULL;
        new_noh->prox = NULL;
        
        return new_noh;
    }else{
        printf("Falha na criacao do noh\n");
        return NULL;
    }
}



//Funcao para inserir na pilha
Stack* push(Stack** pilha, int valor){
    Stack* stac = (Stack*)malloc(sizeof(Stack));
    if (stac != NULL) {
        stac->valor = valor;
        stac->prox = *pilha;
        *pilha = stac;
    }
    return *pilha;
}

//Funcao para inserir fila
void inserir( Fila **queue, int dado){
    Fila* new = (Fila*)malloc(sizeof(Fila));
    Fila* aux;
    if( new != NULL){
        new->valor = dado;
        new->prox = NULL;
        if(*queue == NULL){// Primeiro dado a entrar na fila
           *queue = new; 
        }else{
            aux = *queue;
            //Percorrendo na fila
            while(aux->prox != NULL){
                aux = aux->prox;
            }
            aux->prox = new;
        }
    }else{
        printf("Falha na alocacao!\n");
    }
}
//Funcao para inserir na tabela de frequencia
/*void tabela_frequencia( const char* nome_arquivo,ulli frequencia[TAM]){
    //Passo 1:
    FILE* arquivo = fopen(nome_arquivo,"rb"); // rb = read binary (leitura binaria, em traducao livre)

    //Verificacao de seguranca
    if( arquivo == NULL){
        perror("Falha ao abrir o arquivo");
        return;
    }else{
        //Passo 3:
        int byte;

        //Inicializar
        for(int i = 0; i < TAM; i++){
            frequencia[i] = 0;
        }

        while( (byte = fgetc(arquivo)) != EOF){
            frequencia[(uchar)byte]++; // cada byte que ele achar na frequencia serah adicionado mais 1
        }

        //Passo 4:
        fclose(arquivo);

    }

}
*/
//Funcao para inserir lista "normal"
No *add(List *cabeca,int item)
{
    List *new_no = (No*)malloc(sizeof(No));
    
    if(new_no != NULL){
        
        new_no->valor = item;
        new_no->prox = cabeca;

        return new_no;
    }

    return NULL;
    
}

//Funcao para inserir na arvore "normal"



//Funcao para inserir lista_ordenado
void lista_ordenado(Lista *list, No* no) {
    if (list == NULL || no == NULL) {
        return; // Houve algum erro na alocação da lista ou do nó
    }

    // Caso 1: A lista está vazia, insere o nó como o primeiro elemento
    if (list->inicio == NULL) {
        list->inicio = no;
        no->prox = NULL; // Garante que o próximo do nó inserido aponte para NULL
    }
    // Caso 2: O nó deve ser inserido antes do primeiro elemento (frequência menor)
    else if (*(ulli*)no->frequencia <= *(ulli*)list->inicio->frequencia) {
        no->prox = list->inicio;  // O novo nó aponta para o atual início
        list->inicio = no;        // Atualiza o início da lista
    }
    // Caso 3: Percorre a lista para encontrar a posição correta
    else {
        No* aux = list->inicio;

        // Procura o local correto para inserir o nó
        while (aux->prox != NULL && *(ulli*)aux->prox->frequencia < *(ulli*)no->frequencia) {
            aux = aux->prox; // Avança para o próximo nó
        }

        // Insere o nó no local correto
        no->prox = aux->prox; // O próximo do nó inserido será o próximo do auxiliar
        aux->prox = no;       // O auxiliar aponta para o nó recém-inserido
    }

    list->tam++; // Incrementa o tamanho da lista
}




//Funcao para remover da pilha
Stack* pop(Stack** pilha){
    if( *pilha != NULL){
        Stack* aux = *pilha;
        *pilha = aux->prox;

        free(aux);

        return *pilha;
    }else{
        printf("Pilha vazia\n");
        return NULL;
    }
}


//Funcao para remover da fila
Fila* desinfileirar( Fila** queue){
    Fila* rem = NULL; //Noh que serah removido
    if(*queue != NULL){
        rem = *queue;
        *queue = rem->prox;
    }else{
        printf("Fila vazia!\n");
    }

    return rem;
}

//Funcao para remover da lista "normal"
List* remover( List* cabeca, int item){
    List *previa = NULL;
    List *corrente = cabeca;

    //Percorre a lista ate achar o numero ou chegar no fim da lista
    while(corrente != NULL && corrente->valor != item){

        previa = corrente;
        corrente = corrente->prox;
    }

    if( corrente == NULL){
        return cabeca;
    }

    if( previa == NULL){
        cabeca = corrente->prox;
    }else{
        previa->prox = corrente->prox;
    }

    free(corrente);
    return cabeca;
}

//Funcao para remover da arvore normal

//Funcao para remover da lista_ordenado
No* remover_no_inicio(Lista* list){

    /*
    Temos quer ter um ponteiro auxiliar e inicializa-lo com NULL, pois vamos retornar o auxiliar, pois se nao tiver ninguem na lista removeremos o null
    */
    No* aux = NULL;

    if( list->inicio != NULL){
        aux = list->inicio; // o auxiliar aponta para o inicio
        //O inicio da lista eh o proximo ponteiro
        list->inicio = aux->prox;  // O inicio vai para o proximo para nao perdermos o encadeamento da lista

        //Para nao termos problemas de tentar acessar, de maneira indevida, o campo proximo fora da manipulacao da lista
        aux->prox = NULL;

        //Retirarmos 1 do tamanho da lista
        list->tam--;
    }

    return aux;
}



/*
TESTES
*/

//Testar a pilha
void teste_pilha(){
    Stack* pilha = NULL;

    pilha = push(&pilha,10);
    pilha = push(&pilha,5);
    pilha = push(&pilha,7);

    CU_ASSERT_EQUAL(pop(&pilha),7);
    CU_ASSERT_EQUAL(pop(&pilha),5);
    CU_ASSERT_EQUAL(pop(&pilha),10);
}


//Testar a fila
void teste_fila(){
    Fila* queue = NULL;

    inserir(queue,3);
    inserir(queue,5);
    inserir(queue,4);

    CU_ASSERT_EQUAL(desinfileirar(queue),3);
    CU_ASSERT_EQUAL(desinfileirar(queue),5);
    CU_ASSERT_EQUAL(desinfileirar(queue),4);
} 

//Testar a lista "normal"
void teste_lista_normal(){
    List* lista = NULL;

    lista = add(lista,3);
    lista = add(lista,10);
    lista = add(lista,50);

    CU_ASSERT_EQUAL(remover(lista,10),10);
    CU_ASSERT_EQUAL(remover(lista,3),3);
    CU_ASSERT_EQUAL(remover(lista,50),50);
}


//Testar a lista_ordenado
void teste_lista_ordenado(){
    Lista lista = {NULL, 0};
    No* primeiro = criar_noh(10, 5);
    No* segundo = criar_noh(80, 3);
    No* terceiro = criar_noh(40, 1);

    lista_ordenado(&lista, primeiro);
    lista_ordenado(&lista, segundo);
    lista_ordenado(&lista, terceiro);
    CU_ASSERT_EQUAL(remover_no_inicio(&lista)->frequencia, 1);
    CU_ASSERT_EQUAL(remover_no_inicio(&lista)->frequencia, 3);
    CU_ASSERT_EQUAL(remover_no_inicio(&lista)->frequencia, 5);
}

//main
int main(){
    CU_initialize_registry();

    //Criacao de suites para testes

    //Teste da pilha
    CU_pSuite suite_pilha = CU_add_suite("Suite de teste da pilha",0,0);
    //Verificacao de seguranca
    if( suite_pilha == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_add_test(suite_pilha,"teste_pilha",teste_pilha);

    //Teste da fila
    CU_pSuite suite_fila = CU_add_suite("Suite de teste da fila",0,0);
    //Verificacao de seguranca
    if( suite_fila == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_add_test(suite_fila,"teste_fila",teste_fila);

    //Teste da lista "normal"
    CU_pSuite suite_lista_normal = CU_add_suite("Suite de teste da lista normal",0,0);
    //Verificacao de seguranca
    if( suite_lista_normal == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_add_test(suite_lista_normal,"teste_lista_normal",teste_lista_normal);

    //Teste da lista_ordenado
    CU_pSuite suite_lista_ordenado = CU_add_suite("Suite de teste da lista_ordenada",0,0);
    //Verificacao de seguranca
    if( suite_lista_ordenado == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_add_test(suite_lista_ordenado,"teste_lista_ordenado",teste_lista_ordenado);


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}