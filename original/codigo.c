#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

//Criacao da lista
Lista* criar_lista(){
    Lista* list = (Lista*)malloc(sizeof(Lista));
    if(list!= NULL){
        list->inicio = NULL;
        list->tam = 0;
        return list;
    }else{
        printf("Falha na criacao da lista\n");
        return NULL;
    }

}

//Criacao do novo noh
/*
No* criar_noh(void* byte, void* frequencia){
    No* new_noh = (No*)malloc(sizeof(No));

    if( new_noh != NULL){
        *((uchar*)new_noh->byte) = *((uchar*)byte);
        *((ulli*)new_noh->frequencia) = *((ulli*)frequencia);
        new_noh->dir = NULL;
        new_noh->esq = NULL;
        new_noh->prox = NULL;
        
        return new_noh;
    }else{
        printf("Falha na criacao do noh\n");
        return NULL;
    }
}
*/
No* criar_noh(void* byte, void* frequencia){
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

//Inserir na lista de maneira ordenada( o inicio da fila sempre vai ficar com o valor de menor frequencia)
/*
Caso 1: A lista estah vazia. Caso isso ocorra o noh a ser inserido ficarah no inicio
Caso 2: A lista nao estah mais vazia,vamos ter que verificar se o elemento que queremos inserir tem frequencia menor que o 1º noh da lista
        Neste caso o noh vai ficar na frente do noh do inicio
Caso 3: O noh a ser inserido vai ficar no meio ou no fim


*/

/*
void lista_ordenado(Lista *list, No* no){
    
    if( list == NULL || no == NULL){
        return; //Houve algum erro na alocacao da lista ou do noh
    }
    
    //Caso 1
    if( list->inicio == NULL){
        list->inicio = no;
    
    }
    //Caso 2
    else if( no->frequencia < list->inicio->frequencia){
        //O novo noh precisa apontar para o inicio da lista
        no->prox = list->inicio;

        //Agora temos que atualizar o inicio da lista
        list->inicio = no;
       
    }
    //Caso 3
    else{
        /*Temos que percorrer a lista, para isso faremos um noh auxiliar,
            pois nao podemos alterar o "list->inicio"(so alteramos se queremos mexer no inicio da lista)

        
        No* aux = list->inicio; //Criamos um ponteiro auxiliar para o inicio da lista

        //Verificar se a lista tem um proximo noh
        
        while( aux->prox != NULL && aux->prox->frequencia < no->frequencia){
            //aux->prox->frequencia: estamos pegando a frequencia do proximo noh
            aux = aux->prox; // caminhando na lista
        }
        /*Caso o prox seja nulo teremos uma insercao no final ou 
        a frequencia do proximo nao eh igual a frequencia do noh que queremos inserir
        O novo nog vai ficar apos o auxiliar e antes do proximo, ficando no meio deles
        
        //Temos que colocar o novo noh na lista
        no->prox = aux->prox;

        //Temos que atualizar o auxiliar
        aux->prox = no;
        
    }
    list->tam++;
}
*/


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
void tabela_frequencia( const char* nome_arquivo,ulli frequencia[TAM]){
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
        

        while( (byte = fgetc(arquivo)) != EOF){
            frequencia[(uchar)byte]++; // cada byte que ele achar na frequencia serah adicionado mais 1
        }

        //Passo 4:
        fclose(arquivo);

    }

}

//Funcao para imprimir a tabela de frequencia
void imprimir_frequencia( ulli frequencia[TAM]){
    for( int i = 0; i < TAM; i++){
        if( frequencia[i] > 0){
            printf("%c: %u\n", (uchar)i,frequencia[i]);
        }
    }
}

//imprimir a arvore em pre-ordem
void imprimir_arvore(No* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Se o nó for uma folha, imprime o byte e um caractere especial para identificar folha
    if (raiz->esq == NULL && raiz->dir == NULL) {
        printf("(%c)", *(uchar*)raiz->byte);
    } else {
        // Se for um nó interno, imprimimos um caractere especial
        printf("*");
    }

    // Recursivamente imprime a subárvore esquerda e direita (pré-ordem)
    imprimir_arvore(raiz->esq);
    imprimir_arvore(raiz->dir);
}


void imprimir_lista(Lista* list) {
    No* atual = list->inicio;  // Começa pelo primeiro nó da lista
    while (atual != NULL) {
        // Converte os ponteiros void* para os tipos corretos antes de imprimir
        printf("byte: %c freq: %llu --> ", *(unsigned char*)atual->byte, *(unsigned long long int*)atual->frequencia);
        atual = atual->prox;  // Avança para o próximo nó
    }
    printf("NULL\n");  // Indica o fim da lista
}


//Preencher a lista


void preencher( Lista* list, ulli *table){
 
    for( int i = 0; i < TAM;i++){
        if( table[i] > 0){ // Para pegar os valores maiores que 1
            // Alocar dinamicamente o byte e a frequência para usar void*, para evitar casting de maneira errada
            uchar* ponteiro_para_byte = (uchar*)malloc(sizeof(uchar));
            ulli* ponteiro_para_frequencia = (ulli*)malloc(sizeof(ulli));
 
            //Verificacao de seguranca
            if( ponteiro_para_byte != NULL && ponteiro_para_frequencia != NULL){
                
                *ponteiro_para_byte = (uchar)i;
                *ponteiro_para_frequencia = table[i];
            
 
                No* noh = criar_noh(ponteiro_para_byte,ponteiro_para_frequencia);
 
                //Verificacao de seguranca
                if( noh != NULL){
                    //Chamo a funcao para inserir ordenado na lista
                    lista_ordenado(list,noh);
 
                }else{
                    printf("Falha na alocacao de preenchimento da lista\n");
                    return; // parar o procedimento
                }
            }else{
                printf("Falha na alocacao dos ponteiros para byte e frenquencia, na funcao preencher\n");
                return;
            }
 
        }
    }
}

//Funcao para remover o noh que esta no inicio da lista
/*
Lembrando que queremos retornar o noh com a menor frequencia da lista para podermos gerar a arvore
*/
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
Na geracao da arvore, teremos um retorno para a raiz da arvore, pois assim podemos percorrer toda a arvore a partir deste noh raiz
*/

No* gerar_arvore(Lista* list){
    /*
    Criaremos 3 ponteiros:
        - O primeiro ponteiro "primeiro" irah receber o noh com a frequencia da lista(sendo a lista organizada para ser retirado o noh de menor frequencia)
        - O segundo ponteiro "segundo" irah receber o segundo noh de menor frequencia da lista
        - O terceiro ponteiro "novo" serah o noh que juntaremos a frequencia do "primeiro" e "segundo" ponteiro 
    */
    No* primeiro;
    No* segundo;
    No* novo;
    
    //Enquanto o tamanho da lista for maior que 1 vamos continuar executando o codigo, pois quando o tamanho da lista for igual a 1, significa que temos apenas um noh e esse noh serah o noh raiz

    while( list->tam > 1){
        primeiro = remover_no_inicio(list);
        segundo = remover_no_inicio(list);

        ulli freq_primeiro = *(ulli*)primeiro->frequencia;
        ulli freq_segundo = *(ulli*)segundo->frequencia;

        // Alocar memória para a nova frequência
        ulli* nova_frequencia = (ulli*)malloc(sizeof(ulli));

        if( nova_frequencia != NULL){
            // Soma das frequências dos dois nós
            *nova_frequencia = freq_primeiro + freq_segundo;
            //Agora precisamos criar o noh "novo" e alocar memoria e concetar o "primeiro" e o "segundo"
            novo = criar_noh(NULL,nova_frequencia);
            //Verificacao de seguranca
            if( novo != NULL){
                novo->esq = primeiro;
                novo->dir = segundo;
                novo->prox = NULL; // Neste momento nao aponta para ninguem

                //Agora vamos inserir o noh "novo" novamente na lista e repetir o processo ate o tamanho da lista ser 1
                lista_ordenado(list,novo);
                
                
            }else{
                printf("Falha na alocacao do noh novo, na funcao gerar_arvore\n");
                return NULL;
            }

        }else{
            printf("Falha na alocacao de nova frequencia\n");
            return NULL;

        }
        
    }

    //Quando o loop acabar, ou seja, "list->tam" == 1, retornaremos o noh raiz da arvore que sera o inicio da lista
    return remover_no_inicio(list);
}



//Imprimir a tabela de codigos
void imprimir_tabela_codigos(No* raiz, int codigo[], int profundidade) {
    // Caso base: Se o nó for nulo, retornamos
    if (raiz == NULL) {
        return;
    }

    // Se for uma folha (nó sem filhos), imprimimos o byte e o código correspondente
    if (raiz->esq == NULL && raiz->dir == NULL) {
        printf("Byte: %c | Codigo: ", *(uchar*)raiz->byte);
        for (int i = 0; i < profundidade; i++) {
            printf("%d", codigo[i]);  // Imprimindo o código acumulado até o momento
        }
        printf("\n");
        return;
    }

    // Se não for folha, seguimos para a subárvore esquerda e adicionamos '0' ao código
    codigo[profundidade] = 0;
    imprimir_tabela_codigos(raiz->esq, codigo, profundidade + 1);

    // Seguimos para a subárvore direita e adicionamos '1' ao código
    codigo[profundidade] = 1;
    imprimir_tabela_codigos(raiz->dir, codigo, profundidade + 1);
}


//funcao auxiliar
void tab_cod_aux(No* raiz) {
    int codigo[100];  // Array temporário para armazenar os bits do código (100 é um tamanho seguro)
    imprimir_tabela_codigos(raiz, codigo, 0);
}


//Gerar a tabela de codigos ou dicionario
/*
Nesta funcao vamos percorrer a arvore ja gerada e ir analisando quais sao os noh folha. Caso o noh nao seja folha faremos o seguinte:
    - Recursivamente, toda vez que a arvore for para a esquerda vamos adicionar o caracter "0" no "codigo_atual";
      Caso a arvore va para a direita vamos adicionar o caracter "1" no "codigo_atual";

Caso encontremos um noh folha vamos pegar esse "codigo_atual" e colocar na "tabela_codigos" e o indice dessa tabela serah o noh raiz->byte.

Temos que lembrar que os noh folha nos interessam, pois eh nele que estah de fato o nosso bit, os noh que nao sao folha sao apenas coneccoes que usamos para criar a arvore


Exemplo: 
           *  
    *            *
1      3      4     5

Quando percorremos a arvore o codigo do numero 1 serah:
    - 00(Pois o numero 1 eh um noh folha e para acessarmos ele precisamos ir duas vezes para a esquerda); 
O codigo do numero 5 serah:
    -11(Pois o numero 5 eh um noh folha e para acessarmos ele precisamos ir duas vezes para a direita)

Logo a tabela de codigos para esta arvore de exemplo seria:
1  - 00  
3  - 01
4  - 10
5  - 11



*/

void gerar_tabela_codigos(No* raiz, char* codigo_atual,char** tabela_codigos){
    //Achamos um noh folha
    if( raiz->dir == NULL && raiz->esq == NULL){
        //Vamos copiar o codigo que fizemos indo para a esquerda ou para a direita e colocar na "tabela_codigos"
        tabela_codigos[*(uchar*)(raiz->byte)] = strdup(codigo_atual); 
        //strcpy(tabela_codigos[*(uchar*)raiz->byte],codigo_atual);

        /*
            A funcao "strdup" tem a funcao de duplicar a string que seria o codigo da arvore pecorrida
        Aloca dinamicamente a memoria suficiente para armazenar uma copia de uma string fornecida como argumento.Dentro da funcao ela vai copiar o conteudo da string passada no parametro para o espaco que ela alocou. No final ela retorna um ponteiro para a nova string duplicada
        */
       return;
    }else{
        //O noh agora nao eh uma folha entao vamos percorrer tanto para a esquerda quanto para a direita 
        //Vamos criar um vetor char auxiliar para a esquerda para nao perdermos nenhuma possivel referencia. Faremos o mesmo para percorrer o lado direito da arvore
        char aux_esqueda[TAM];
        //Vamos copiar o que tem no "codigo_atual" para este vetor "aux_esquerda" que criamos para nele irmos colocando "0" quando a arvore for para a esquerda
        strcpy(aux_esqueda,codigo_atual);
        
        //Agora vamos usar a funcao "strcat" para ir concatenando "0" ao vetor auxiliar que criamos, pois lembre-se que quando percorremos a esquerda da arvore colocarmos "0"
        strcat(aux_esqueda,"0");
        //Agora vamos chamar a funcao recursivamente para irmos percorrendo a arvore para a esquerda

        
        //Lembrete: no lugar de "codigo_atual" colocamos o aux_esquerda pois eh ele que queremos ir colocando o "0" para quando acharmos o noh folha colocarmos o codigo do noh folha achado

        //O que fizemos para percorrer o lado esquerdo da arvore faremos o mesmo para percorrer o lado direito da arvore

        char aux_direita[TAM];
        strcpy(aux_direita,codigo_atual);
        strcat(aux_direita,"1");
        
        gerar_tabela_codigos(raiz->esq,aux_esqueda,tabela_codigos);
        gerar_tabela_codigos(raiz->dir,aux_direita,tabela_codigos);
    }
 

}

//Funcao para contar o total de bits do arquivo
//Adendo: utilizamos ull ( unsigned long long), pois ele suporta numeros bem altos, esse tipo de variavel ocupa 8 bytes. Caso sejam inseridos arquivos grandes, de 1 gb por exemplo, o codigo nao corre o risco de ter algum "estouro"
ull contar_bits_totais( const char* nome_arquivo, char** tabela_codigos){
    //Abrindo o arquivo original
    FILE* arquivo_original = fopen(nome_arquivo,"rb");

    //Verificacao de seguranca
    if( arquivo_original == NULL){
        perror("Falha ao abrir o arquivo");
        return 0; // Sinalizando que houve falha na abertura do arquivo
    }else{

        ull total_bits = 0;
        int byte; // Armazenarah temporariamente os bytes do arquivo, e serah ele que usaremos como indice para acessar a tabela_codigos correspondente aquele byte

        //Loop para ler o byte de cada arquivo ate chegar ao final do arquivo
        while((byte = fgetc(arquivo_original)) != EOF){
            //Auxiliar para nao perdemos nenhuma referencia
            char* auxiliar = tabela_codigos[(uchar)byte];

            //Calculando o total de bits
            /*
            O comprimento da string determina o numero de bits usados para representar esse byte. Quando somamos os comprimentos dos codigos de todos os bytes obtemos o numero total de bits
            */
            total_bits += strlen(auxiliar);
        }

        //Fechar o arquivo
        fclose(arquivo_original);

        return total_bits;
    }


}


//Vamos gerar o arquivo compactado
/*
Nesta funcao leremos o arquivo original byte por byte, depois substituir cada byte pelo seu codigo respectivo de Huffman, que foi gerado anteriormente(gerar_tabela_codigos); armazenando essa sequencia no arquivo compactado.

    -Teremos o arquivo original que queremos compactar(lido em modo binario);
    -Teremos a tabela_codigos: Matriz onde cada indice corresponde a um byte, e o valor eh a sequencia de bits que representa aquele byte no codigo de Huffman
    -Teremos a leitura e codificacao: Para cada byte lido do arquivo original, a funcao substitui o byte por sua sequencia de bits correspondente da "tabela_codigos";
    -Escrita em blocos de 1 byte(8 bits): Armazenaremso esses bits em um "buffer" de 8 bits. Quando o buffer estiver cheio(ja completamos 1 byte), ele é escrito no arquivo compactado. Caso restem bits incompletos no final, eles sao preenchidos com 0 para completar o byte final

    Adendo: Antes de colocarmos os bits no arquivo, temos que gerar o cabecalho e colocarmos ele no arquivo compactado para ajudar na descompactacao.
        A realizacao do cabecalho serah feita da seguinte forma:
            1º: declararemos que o cabecalho vai ser um "ushort" (unsigned short),pois garanto que nao terah numeros negativos. E um ushort possui espaco para 16 bits, justamente o que queremos.
            2º: O cabeçalho receberah: bits_lixo << 13 | tamanho_arvore. Ele "juntarah" os bits_lixo com o tamanho da arvore no cabecalho. Explicaremos mais detalhado dentro da funcao
*/

void gerar_arquivo_compactado(const char* nome_arquivo_original, const char* nome_arquivo_compactado, char** tabela_codigos,int bits_lixo,int tamanho_arvore,char* arvore_huffman){
    //Vamos criar dois arquivos e abrir eles
    FILE* arquivo_original = fopen(nome_arquivo_original,"rb"); // Arquivo que serah lido e compactado
    FILE* arquivo_compactado = fopen(nome_arquivo_compactado,"wb"); // Arquivo onde os dados compactados serao escritos

    //Verificacao de seguranca
    if( arquivo_compactado == NULL || arquivo_original == NULL){
        perror("Falha ao abrir os arquivos");
        return; // Paro o processo
    }else{

        //Construcao do cabecalho:
        /*
        O cabecalho tem que ter 3 bits (mais significativos) representando os bits_lixo e 13 bits( menos significativos) representando o tamanho da arvore.

        Queremos explicar como vai funcionar o calculo do cabecalho: 
        ushort cabecalho = (bits_lixo << 13) | tamanho_arvore

        Vamos deslocar os bits de lixo 13 vezes para a esquerda, pois assim garantimos que o bits de lixo serao os 3 bits mais significativos do cabecalho. Logo depois faremos a operacao OR bit a bit, para colocar o tamanho da arvore; pois esta operacao vai comparar os bits e quando um desses bits forem 1, ela coloca 1

        Exemplo
        bits_lixo = 7 ( 0000000000000111)
        tamanho da arvore = 1234 ( 10011010010)

        Quando fazemos bits_lixo << 13, obteremos: 
        1110000000000000

        Logo apos vamos fazer a comparacao bit a bit: ((bits_lixo << 13) | tamanho_arvore)
        1110000000000000
             10011010010
        1110010011010010 
        */
        ushort cabecalho = (bits_lixo << 13) | tamanho_arvore;

        //Depois de feito o cabecalho escreveremos ele no arquivo compactado
        fwrite(&cabecalho, sizeof(ushort),1,arquivo_compactado);

        //Escrevendo a arvore de Huffman no arquivo
        fwrite(arvore_huffman,sizeof(char),tamanho_arvore,arquivo_compactado);

        int byte; // Armazena temporariamente cada byte lido do arquivo original
        uchar buffer = 0; // Armazena ate 8 bits que serao escritos no arquivo compactado
        int bits_preenchidos = 0; // conta quantos bits foram preenchidos no buffer

        //Leitura do arquivo original
        //Le cada byte do arquivo original usando fgetc(), vamos ler o arquivo ate encontrarmos o final do arquivo
        while((byte = fgetc(arquivo_original)) != EOF){

            //Para cada byte lido, a funcao obtem o codigo de Huffman correspondente da "tabela_codigos"

            char *codigos = tabela_codigos[(uchar)byte];

            //Agora vamos percorrer "codigos" e para cada caracter no codigo de Huffman,ele irah verificar se esse caracter eh '1',caso seja '1', ele adiciona o bit correspondente no "buffer"

            for( int i = 0; i < strlen(codigos); i++){
                /*
                Importante explicar o porquê desse '1':
                Caso o caracter seja '1' isso quer dizer que, no codigo de Huffman, esse bit precisa ser ativado (ou seja, colocado como 1) no "buffer". Por isso usamos a operacao bit a bit para adcionar o bit 1 na posicao correta do "buffer".

                Caso o caracter seja '0' nao fazemos nada porque o bit ja estah desativado (preenchido com 0) e o "buffer" permanece inalterado, pois o "buffer" comeca com todos os bits desligados

                Isso garante que a sequencia correta de bits seja escrita no arquivo compactado
                */
                if( codigos[i] == '1'){
                    /*
                    A operacao "buffer |= ( 1 << ( 7  - bits_preenchidos))" define o bit na posicao correta do "buffer".

                    "( 7 - bits_preenchidos)": calcula a posicao exata do bit no byte, comecando da posicao mais a esquerda (mais significativo) ate o bit mais a direita (menos significativo).

                    "buffer |=" : o operador "|=" adiciona o bit 1 aquela posicao especifica do "buffer", sem alterar os outros bits que ja estao armazenados
                    */

                    buffer |= ( 1 << ( 7  - bits_preenchidos)); // adiciona 1 no bit selecionado 

                }

                bits_preenchidos++;
                
                //Completamos 1 byte
                if( bits_preenchidos == 8){
                    //Apos completar 1 byte, ele eh colocado no arquivo compactado
                    fputc(buffer,arquivo_compactado); // Escreve o byte completo
                    buffer = 0; // Zeramos o "buffer" para armazenar o proximo conjuto de bits
                    bits_preenchidos = 0; 

                }
            }

        }

        //Caso restem bits no "buffer", vamos preencher esses bits restante com 0, para formar 1 byte completo
        if( bits_preenchidos > 0){

            fputc(buffer,arquivo_compactado);
        }

        //Fechamos os arquivos
        fclose(arquivo_compactado);
        fclose(arquivo_original);
    }


}

//Funcao para calcular o total de bits_lixo do arquivo
/*
Para sabermos o total de bits_lixo devemos saber o total de bits do arquivo compactado com os codigos de huffman. 
Depois disso dividimos esse total de bits por 8, para sabermos quantos bytes completos foram escritos.
O resto dessa divisao subtraimos 8 - resto, e achamos a quantidade de bits_lixo;

Exemplo: 
    Total de bits = 50

    bytes completado = 6(48 bits);
    resto da divisao  = 2;
    bits_lixo = 8 - 2 = 6 bits
    

*/

int total_bits_lixo( ull tot_bit){
    int bit_lx = 8 - ( tot_bit % 8);

    //Adendo: Caso o resutado de "bit_lx" seja == 8, quer dizer que nao ha bits_lixo, pois ficaria 8 - 0 = 8;
    if( bit_lx == 8){
        bit_lx = 0; // nao ha bits_lixo
    }     

    return bit_lx;
}


//Funcao para calcular o tamanho da arvore

//Deixar como teste, modificar esta função
int calcular_tamanho_arvore(No* raiz) {
    // Supondo que você tenha uma função que percorre a árvore e retorna o tamanho.
    // Exemplo de uma função de pré-ordem que calcula o número de bytes necessários.
    if (raiz == NULL) {
        return 0;
    }
    // Se é folha, 1 byte para o valor, mais o bit de controle (conta como 1 byte no total)
    if (raiz->esq == NULL && raiz->dir == NULL) {
        return 1 + sizeof(char); // 1 byte para o valor + bit de controle
    }
    // Para nós internos, conta o bit de controle + tamanho dos filhos
    return 1 + calcular_tamanho_arvore(raiz->esq) + calcular_tamanho_arvore(raiz->dir);
}

//Funcao para gerar o codigo da arvore (printar em pre-ordem)
char* codificar(char** tabela_codigos,const char* nome_arquivo){
    FILE* arquivo = fopen(nome_arquivo,"rb");

    //Verificacao de seguranca
    if( arquivo == NULL){
        perror("Falha ao abrir o arquivo");
        return NULL;
    }
    ull tam = contar_bits_totais(nome_arquivo,tabela_codigos);
    char* codigo = (char*)calloc(tam + 1,sizeof(char));

    int byte;

    while((byte = fgetc(arquivo)) != EOF){
        strcat(codigo,tabela_codigos[(uchar)byte]);
    }

    fclose(arquivo);
    return codigo;
}

//Funcao geral para compactar o arquivo
void compactar_arquivo(const char* nome_arquivo_original, const char* nome_arquivo_compactado) {
    ulli frequencia[TAM] = {0};
    
    // Passo 1: Gerar tabela de frequências
    tabela_frequencia(nome_arquivo_original, frequencia);

   imprimir_frequencia(frequencia);
    printf("\n");

    // Passo 2: Criar lista de nós e gerar a árvore de Huffman
    Lista* lista = criar_lista();
    preencher(lista, frequencia);

    imprimir_lista(lista);
    printf("\n");

    No* raiz = gerar_arvore(lista);

    imprimir_arvore(raiz);
    printf("\n");

    // Passo 3: Gerar tabela de códigos
    char* tabela_codigos[TAM] = {0};
    char codigo_inicial[TAM] = "";
    
    gerar_tabela_codigos(raiz, codigo_inicial, tabela_codigos);

    tab_cod_aux(raiz);

    ull tam_arq = contar_bits_totais(nome_arquivo_original,tabela_codigos); 

    int bit_lix = total_bits_lixo(tam_arq);
    int tam_arvore = calcular_tamanho_arvore(raiz);
    char* huffman = codificar(tabela_codigos,nome_arquivo_original);

    // Passo 4: Escrever o arquivo compactado
    //escrever_arquivo_compactado(nome_arquivo_original, nome_arquivo_compactado, tabela_codigos);
    gerar_arquivo_compactado(nome_arquivo_original,nome_arquivo_compactado,tabela_codigos,bit_lix,tam_arvore,huffman);

    // Libere a memória da tabela de códigos
    for (int i = 0; i < TAM; i++) {
        if (tabela_codigos[i] != NULL) {
            free(tabela_codigos[i]);
        }
    }
}


int main() {
    char nome_arquivo_original[100], nome_arquivo_compactado[100];

    // Lendo os nomes dos arquivos
    printf("Digite o nome do arquivo original: ");
    fgets(nome_arquivo_original, sizeof(nome_arquivo_original), stdin);
    nome_arquivo_original[strcspn(nome_arquivo_original, "\n")] = 0; // Remover nova linha

    printf("Digite o nome do arquivo compactado: ");
    fgets(nome_arquivo_compactado, sizeof(nome_arquivo_compactado), stdin);
    nome_arquivo_compactado[strcspn(nome_arquivo_compactado, "\n")] = 0; // Remover nova linha

    compactar_arquivo(nome_arquivo_original, nome_arquivo_compactado);

    return 0;
}