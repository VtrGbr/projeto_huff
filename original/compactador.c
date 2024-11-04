#include "compactador.h"


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
        printf("byte: %c freq: %llu --> ", *(uchar*)atual->byte, *(ulli*)atual->frequencia);
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
    if (raiz == NULL) return 0;  // Caso base: árvore vazia

    // Caso folha
    if (raiz->esq == NULL && raiz->dir == NULL) {
        if (*(uchar*)raiz->byte == '*' || *(uchar*)raiz->byte == '\\') {
            return 2;  // Conta o caractere de escape '\\' + caractere especial
        }
        return 1;  // Conta apenas o caractere da folha
    }

    // Conta o nó interno '*' + tamanho dos filhos
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

//Funcao para escrever a arvore em pre-ordem e colocar no arquivo, posteriormente
//Vale salientar que devemos considerar que cada noh interno eh "*"
//A folha possui o caracter ou byte que queremos para formar o arquivo posteriormente na descompactacao
void pre_ordem(No* raiz,FILE * arquivo_compactado){
    //Verificacao de seguranca
    if( raiz != NULL){
        //Vamos verificar se encontramos um noh folha
        if( raiz->esq == NULL && raiz->dir == NULL){
            //Vamos para o caso "especial", que eh o noh folha ter o "*" ou o "\\"
            if( *(uchar*)raiz->byte == '*' || *(uchar*)raiz->byte == '\\'){
              //Lembrando que quando fazemos "*(uchar*)raiz->byte", estamos querendo saber qual o conteudo presente no raiz->byte, ou seja, qual o byte presente naquele noh folha
              fputc('\\',arquivo_compactado);

            }else{
                //Caso ele nao seja "*" nem "\\", entao coloque no vetor o byte presente no noh byte
                fputc(*(uchar*)raiz->byte,arquivo_compactado);
            }
        }else{
            //Caso nao estivermos em um noh folha, estaremos em um noh intermediario, que eh representado por "*"
            fputc('*',arquivo_compactado);
        }

        //Observacao: incrementamos o valor de "*i" para que possamos saber o valor dele ao final da funcao e podermos usarmos este valor ao escrever a arvore no arquivo compactado e tambem para podermos navegar no vetor

        pre_ordem(raiz->esq,arquivo_compactado);
        pre_ordem(raiz->dir,arquivo_compactado );
    }
}   


//Vamos gerar o arquivo compactado

//Funcao para escrever o cabecalho
//Construcao do cabecalho:
        /*
        A realizacao do cabecalho serah feita da seguinte forma:
            1º: declararemos que o cabecalho vai ser um "ushort" (unsigned short),pois garanto que nao terah numeros negativos. E um ushort possui espaco para 16 bits, justamente o que queremos.
            2º: O cabeçalho receberah: bits_lixo << 13 | tamanho_arvore. Ele "juntarah" os bits_lixo com o tamanho da arvore no cabecalho. Explicaremos mais detalhado dentro da funcao

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
void escrever_cabecalho(FILE* arquivo_compactado,int bits_lixo,int tamanho_arvore){
    
    // Cria os 2 bytes de cabeçalho para armazenar bitsLixo e tamanhoArvore
    unsigned short cabecalho = (bits_lixo << 13) | tamanho_arvore;
    
    //fwrite(&cabecalho, sizeof(unsigned short), 1, arquivo_compactado); // Escreve o cabeçalho no arquivo

    //primeiroByte: Extrai os 8 bits mais significativos (à esquerda) de cabecalho para armazenar no primeiro byte.
    //cabecalho >> 8 desloca o valor 8 bits para a direita, eliminando os 8 bits menos significativos.
    /*
    Como isso funciona: Deslocaremos o cabecalho 8 bits para a direita, garantindo que so tenhamos os bits mais significativos.
    Após isso aplicamos uma máscara que serve para para preservar apenas os 8 bits menos significativos do valor resultante do deslocamento. Assim garantimos que apenas os ultimos 8 bits sejam considerados e que qualquer bit extra, caso o valor inicial fosse maior que 16 bits, seja removido.
    */
    unsigned char primeiroByte = (cabecalho >> 8) & 0xFF; // Byte mais significativo
    
    //& 0xFF assegura que apenas os 8 bits mais significativos sejam armazenados em primeiroByte.
    //segundoByte: Extrai os 8 bits menos significativos (à direita) de cabecalho.
    // cabecalho & 0xFF aplica uma máscara de 8 bits, garantindo que apenas os bits menos significativos permaneçam.
    
    //0xFF em hexadecimal representa 11111111 em binário, ou seja, 8 bits com valor 1.

    /*
    Vamos mostrar um exemplo para ficar mais intuitivo:
        Imagine que o cabecalho seja  0110000010000011

         cabecalho: 0110000010000011
              0xFF: 0000000011111111
         resultado(and): 0000000010000011

         Perceba que os 8 bits menos significativos foram preservados

    */
    unsigned char segundoByte = cabecalho & 0xFF;          // Byte menos significativo

    
    fputc(primeiroByte, arquivo_compactado);
    fputc(segundoByte, arquivo_compactado);

    /*
    Fizemos esta abordagem pois, quando comparamos os bytes do arquivo pelo leitor Hexadecimal, notamos que sempre o cabeçalho estava trocado, o que era pra ser "07 80", tava saindo "80 07". Os bytes de lixo de tamanho da arvore, por algum motivo, estavam sendo trocados, entao usamos a abordagem de separar os dois bytes e escrever o primeiro byte que possui o lixo, primeiro no arquivo, e depois escrever o byte correspondente ao tamanho da arvore
    */

}


void bits_compactados(const char* nome_arquivo_original, FILE* arquivo_compactado, char** tabela_codigos){
    //Vamos criar dois arquivos e abrir eles
    
    FILE* arquivo_original = fopen(nome_arquivo_original,"rb"); // Arquivo que serah lido e compactado
   
    //Verificacao de seguranca
    if( arquivo_original == NULL){
        perror("Falha ao abrir os arquivos");
        return; // Paro o processo
    }else{


        int byte; // Armazena temporariamente cada byte lido do arquivo original
        uchar buffer = 0; // Armazena ate 8 bits que serao escritos no arquivo compactado
        int bits_preenchidos = 0; // conta quantos bits foram preenchidos no buffer

        //Leitura do arquivo original
        //Le cada byte do arquivo original usando fgetc(), vamos ler o arquivo ate encontrarmos o final do arquivo
        while((byte = fgetc(arquivo_original)) != EOF){

            //Para cada byte lido, a funcao obtem o codigo de Huffman correspondente da "tabela_codigos"

            char *codigos = tabela_codigos[(uchar)byte];

            //Agora vamos percorrer "codigos" e para cada caracter no codigo de Huffman,ele irah verificar se esse caracter eh '1',caso seja '1', ele adiciona o bit correspondente no "buffer"

            for( int i = 0; codigos[i] != '\0'; i++){
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

        //Fechamos o arquivo
        fclose(arquivo_original);
    }


}

//Funcao geral para compactar o arquivo
void compactar_arquivo(const char* nome_arquivo_original, const char* nome_arquivo_compactado) {
    ulli frequencia[TAM] = {0};
    
    // Passo 1: Gerar tabela de frequências
    tabela_frequencia(nome_arquivo_original, frequencia);

    
    // Passo 2: Criar lista de nós e gerar a árvore de Huffman
    Lista* lista = criar_lista();
    preencher(lista, frequencia);

    No* raiz = gerar_arvore(lista);


    // Passo 3: Gerar tabela de códigos
    char* tabela_codigos[TAM] = {0};
    char codigo_inicial[TAM] = "";
    
    gerar_tabela_codigos(raiz, codigo_inicial, tabela_codigos);

    ull tam_arq = contar_bits_totais(nome_arquivo_original,tabela_codigos); 

    int bit_lix = total_bits_lixo(tam_arq);  //bit_lixo 
    int tam_arvore = calcular_tamanho_arvore(raiz); // tamanho da arvore

    //Gerar o arquivo compactado
    FILE* arquivo_compactado = fopen(nome_arquivo_compactado,"wb");
    if( arquivo_compactado == NULL){
        perror("Falha ao abrir o arquivo");
        return;
    }
    escrever_cabecalho(arquivo_compactado,bit_lix,tam_arvore);
    pre_ordem(raiz,arquivo_compactado);
    //char* huffman = codificar(tabela_codigos,nome_arquivo_original); //codigo de huffman

    // Passo 4: Escrever o arquivo compactado
    //escrever_arquivo_compactado(nome_arquivo_original, nome_arquivo_compactado, tabela_codigos);
    bits_compactados(nome_arquivo_original,arquivo_compactado,tabela_codigos);

    //Debugg
    /*
    imprimir_frequencia(frequencia);
    printf("\n");
    imprimir_lista(lista);
    printf("\n");
    imprimir_arvore(raiz);
    printf("\n");
    tab_cod_aux(raiz);
    */

    // Libere a memória da tabela de códigos
    for (int i = 0; i < TAM; i++) {
        if (tabela_codigos[i] != NULL) {
            free(tabela_codigos[i]);
        }
    }
    //Liberar memoria dos codigos do huffman
    //free(huffman);

    fclose(arquivo_compactado);
}

