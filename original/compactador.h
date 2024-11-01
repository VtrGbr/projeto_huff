#ifndef compactador_h
#define compactador_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 256 // Existem 256 valores possíveis para um byte (0 a 255)

typedef unsigned char uchar;
typedef unsigned short ushort; // Serve para o cabecalho (linha 371)
typedef unsigned long long int ulli;
typedef unsigned long long ull; //Serah usado para saber o total de bits do arquivo compactado

typedef struct no{
    void* byte;
    void* frequencia;
    struct no* dir,*esq,*prox;
}No;

typedef struct lista{
    No* inicio;
    int tam;
}Lista;


//Criacao do noh lista
Lista* criar_lista();


//Cria o noh da arvore
//void* byte: aqui serah colocar o byte do noh
//void* frequencia: aqui terah a frequencia do byte 
No* criar_noh(void* byte, void* frequencia);

//Lista* list: lista com o byte e a sua frequencia
//No no: noh que serah inserido na lista

//Inserir na lista de maneira ordenada( o inicio da fila sempre vai ficar com o valor de menor frequencia)
/*
Caso 1: A lista estah vazia. Caso isso ocorra o noh a ser inserido ficarah no inicio
Caso 2: A lista nao estah mais vazia,vamos ter que verificar se o elemento que queremos inserir tem frequencia menor que o 1º noh da lista
        Neste caso o noh vai ficar na frente do noh do inicio
Caso 3: O noh a ser inserido vai ficar no meio ou no fim


*/

void lista_ordenado(Lista *list, No* no) ;




//Tabela de frequencia
/*
Para sabermos a frequencia de cada byte de um arquivo devemos seguir os seguintes passos:
1. Abrir o arquivo:
Abra o arquivo no modo binário para garantir que todos os bytes sejam lidos corretamente, independentemente do conteúdo.
2. Inicializar a tabela de frequência:
Use um vetor de tamanho 256 (porque há 256 possíveis valores de byte, de 0 a 255) para armazenar a frequência de cada byte.
3. Ler os bytes do arquivo:
Leia o arquivo byte por byte, e para cada byte lido, incremente o valor correspondente na tabela de frequência.
4. Fechar o arquivo:
Após processar o arquivo, feche-o para liberar os recursos.
*/

//Usamos o "const char" para que receber o nome do arquivo, e o const serve para garantir que o vetor char serah somente para leitura
void tabela_frequencia( const char* nome_arquivo,ulli frequencia[TAM]);

//Funcao para imprimir a tabela de frequencia
//Para fins de debug criamos esta funcao que mostra a tabela de frequencia de cada byte
void imprimir_frequencia( ulli frequencia[TAM]);


//imprimir a arvore em pre-ordem
//Para fins de debug criamos a funcao que imprime a arvore em pre-ordem
void imprimir_arvore(No* raiz);


//Para fins de debug criamos esta funcao para imprimir a lista. Para podermos analisar se a frenquencia estah em ordem crescente
void imprimir_lista(Lista* list);


//Recebemos uma lista e a tabela de frequencia
//Analisamos os valores da tabela que sao maiores que 0 frequencias
//Alocamos memoria para o ponteiro dos bytes e da frenquencia para podermos inserir na funcao "lista_ordenado"
void preencher( Lista* list, ulli *table);


//Funcao para remover o noh que esta no inicio da lista
/*
Lembrando que queremos retornar o noh com a menor frequencia da lista para podermos gerar a arvore
*/
No* remover_no_inicio(Lista* list);


/*
Na geracao da arvore, teremos um retorno para a raiz da arvore, pois assim podemos percorrer toda a arvore a partir deste noh raiz

    Criaremos 3 ponteiros:
        - O primeiro ponteiro "primeiro" irah receber o noh com a frequencia da lista(sendo a lista organizada para ser retirado o noh de menor frequencia)
        - O segundo ponteiro "segundo" irah receber o segundo noh de menor frequencia da lista
        - O terceiro ponteiro "novo" serah o noh que juntaremos a frequencia do "primeiro" e "segundo" ponteiro 
    

*/
No* gerar_arvore(Lista* list);

//Funcao de debug para podermos analisar se a tabela de codigo estah funcionando corretamente
//Nela temos o noh raiz da arvore, o codigo huffman e a profundidade
void imprimir_tabela_codigos(No* raiz, int codigo[], int profundidade);

//funcao auxiliar para imprimir a tabela de codigos
void tab_cod_aux(No* raiz);

//Gerar a tabela de codigos ou dicionario
/*
Nesta funcao vamos percorrer a arvore ja gerada e ir analisando quais sao os noh folha. Caso o noh nao seja folha faremos o seguinte:
    - Recursivamente, toda vez que a arvore for para a esquerda vamos adicionar o caracter "0" no "codigo_atual";
      Caso a arvore va para a direita vamos adicionar o caracter "1" no "codigo_atual";

Caso encontremos um noh folha vamos pegar esse "codigo_atual" e colocar na "tabela_codigos" e o indice dessa tabela serah o noh raiz->byte.
*/
void gerar_tabela_codigos(No* raiz, char* codigo_atual,char** tabela_codigos);

//Funcao para contar o total de bits do arquivo
//Adendo: utilizamos ull ( unsigned long long), pois ele suporta numeros bem altos, esse tipo de variavel ocupa 8 bytes. Caso sejam inseridos arquivos grandes, de 1 gb por exemplo, o codigo nao corre o risco de ter algum "estouro"
//Ela recebe o nome do arquivo original e o dicionario de codigos
ull contar_bits_totais( const char* nome_arquivo, char** tabela_codigos);

//Vamos gerar o arquivo compactado
/*
Nesta funcao leremos o arquivo original byte por byte, depois substituir cada byte pelo seu codigo respectivo de Huffman, que foi gerado anteriormente(gerar_tabela_codigos); armazenando essa sequencia no arquivo compactado.
*/
void gerar_arquivo_compactado(const char* nome_arquivo_original, const char* nome_arquivo_compactado, char** tabela_codigos,int bits_lixo,int tamanho_arvore,char* arvore_huffman);

//Funcao para calcular o total de bits_lixo do arquivo
/*
Para sabermos o total de bits_lixo devemos saber o total de bits do arquivo compactado com os codigos de huffman. 
Depois disso dividimos esse total de bits por 8, para sabermos quantos bytes completos foram escritos.
O resto dessa divisao subtraimos 8 - resto, e achamos a quantidade de bits_lixo;
*/
int total_bits_lixo( ull tot_bit);



int calcular_tamanho_arvore(No* raiz);

//Funcao para escrever a arvore em pre-ordem e colocar no arquivo, posteriormente
//Vale salientar que devemos considerar que cada noh interno eh "*"
//A folha possui o caracter ou byte que queremos para formar o arquivo posteriormente na descompactacao
void pre_ordem(No* raiz, uchar* tree, int *i);

//Funcao para gerar o codigo da arvore (printar em pre-ordem)
char* codificar(char** tabela_codigos,const char* nome_arquivo);

//Funcao geral para compactar o arquivo
//Receberemos o nome do arquivo original e nome do arquivo para onde irah a compactacao
void compactar_arquivo(const char* nome_arquivo_original, const char* nome_arquivo_compactado);

#endif