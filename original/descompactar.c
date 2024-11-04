#include "descompactar.h"


// Função para abrir um arquivo, solicitando o nome ao usuário
FILE* AbrirArquivo(char *nome) {
    FILE *arquivo;
    while (1) {
        printf("Digite o nome do arquivo: ");
        scanf("%s", nome);
        arquivo = fopen(nome, "rb"); // Abre o arquivo em modo leitura binária
        if (arquivo != NULL) {
            return arquivo; // Retorna o ponteiro do arquivo se aberto com sucesso
        } else {
            printf("ERRO: Não foi possível abrir o arquivo '%s'. Tente novamente.\n", nome);
        }
    }
}

no_t *criar_no(unsigned int caractere, no_t *esquerda, no_t *direita) 
{
    no_t *novo_no = (no_t *)malloc(sizeof(no_t));
    novo_no->caractere = caractere;
    novo_no->esquerda = esquerda;
    novo_no->direita = direita;
    return (novo_no);
}

/* apaga árvore*/
void liberar_arvore(no_t *arvore_huff) 
{
    if (arvore_huff != NULL) 
    {
        liberar_arvore(arvore_huff->esquerda);
        liberar_arvore(arvore_huff->direita);
        free(arvore_huff);
    }
}

/* verifica se o nó é uma folha ou não*/
int eh_folha_descompactar(no_t *arvore_binaria) 
{
    return ((arvore_binaria != NULL) && (arvore_binaria->esquerda == NULL) && (arvore_binaria->direita == NULL));
}

/* verifica se o bit está definido na posição ou não*/
int bit_esta_definido_descompactar(unsigned int caractere, int posicao) 
{
    unsigned int mascara = 1 << posicao;
    return (mascara & caractere);
}

void descompactar(FILE *arquivo_entrada, unsigned int tamanho_lixo, int tamanho_arvore, no_t *arvore_huff, FILE *arquivo_saida) 
{
    if (tamanho_arvore == 0) 
    {
        return;
    }

    no_t *no_atual = arvore_huff;
    unsigned int byte_atual;
    unsigned int ultimo_byte; 
    long long int total_bytes; 
    long long int bytes; 
    int i; 

    fseek(arquivo_entrada, -1, SEEK_END);
    ultimo_byte = getc(arquivo_entrada);
    total_bytes = ftell(arquivo_entrada);
    fseek(arquivo_entrada, (2 + tamanho_arvore), 0);

    byte_atual = getc(arquivo_entrada);
    for (bytes = (2 + tamanho_arvore); bytes < (total_bytes - 1); bytes++) 
    {
        for (i = 7; i >= 0; i--) 
        {
            /* se o bit estiver definido, anda para a direita na árvore */
            if (bit_esta_definido_descompactar(byte_atual, i) != 0) 
            {
                if (no_atual->direita != NULL)
                {
                    no_atual = no_atual->direita;
                }
            }
            else 
            {
                if (no_atual->esquerda != NULL)
                {
                    no_atual = no_atual->esquerda;
                }
            }
            if (eh_folha_descompactar(no_atual) != 0) 
            {
                fprintf(arquivo_saida, "%c", no_atual->caractere);
                no_atual = arvore_huff;
            }
        }
        byte_atual = getc(arquivo_entrada);
    }

    for (i = 7; i >= (signed int)tamanho_lixo; i--) 
    {
        if (bit_esta_definido_descompactar(byte_atual, i) != 0) 
        {
            if (no_atual->direita != NULL)
            {
                no_atual = no_atual->direita;
            }
        }
        else 
        {
            if (no_atual->esquerda != NULL)
            {
                no_atual = no_atual->esquerda;
            }
        }
        if (eh_folha_descompactar(no_atual) != 0) 
        {
            fprintf(arquivo_saida, "%c", no_atual->caractere);
            no_atual = arvore_huff;
        }
    }
    printf("Descompactacao concluida!\n");
    return;
}

unsigned int obter_tamanho_lixo(FILE *arquivo_entrada) 
{
    unsigned int tamanho_lixo = 0;
    unsigned char primeiro_byte;
    
    fseek(arquivo_entrada, 0, SEEK_SET);
    
    primeiro_byte = getc(arquivo_entrada);
    tamanho_lixo = (unsigned int)(primeiro_byte >> 5); // Obtém o tamanho do lixo
    return (tamanho_lixo);
}

unsigned int obter_tamanho_arvore(FILE *arquivo_entrada) 
{
    unsigned int tamanho_arvore;
    unsigned char primeiro_byte;
    unsigned char segundo_byte;

    //Esse comando move o "cursor" do arquivo para o início (0) do arquivo (SEEK_SET), para garantir que a leitura comece do início.
    fseek(arquivo_entrada, 0, SEEK_SET);

    primeiro_byte = getc(arquivo_entrada); 
    segundo_byte = getc(arquivo_entrada);

    primeiro_byte = primeiro_byte << 3;
    primeiro_byte = primeiro_byte >> 3;
    tamanho_arvore = ((primeiro_byte << 8) | segundo_byte);
    
    return (tamanho_arvore);
}

unsigned int *obter_array_arvore(FILE *arquivo_entrada, unsigned int tamanho_arvore) 
{
    unsigned int i;
    unsigned int *array_arvore = (unsigned int *)malloc(sizeof(unsigned int) * tamanho_arvore);
    
    fseek(arquivo_entrada, 2, 0);

    for (i = 0; i < tamanho_arvore; i++) 
    {
        array_arvore[i] = getc(arquivo_entrada);
    }
    return (array_arvore); 
}

no_t *construir_arvore(unsigned int **array_arvore) 
{
    if (**array_arvore == '*') 
    {
        no_t *esquerda;
        (*array_arvore)++; 
        esquerda = construir_arvore(array_arvore);
        (*array_arvore)++; 
        return (criar_no('*', esquerda, construir_arvore(array_arvore)));
    }
    else if (**array_arvore == '\\') 
    {
        *array_arvore = (*array_arvore + 1);
        return (criar_no(**array_arvore, NULL, NULL));
    }
    return (criar_no(**array_arvore, NULL, NULL)); 
}

no_t *obter_arvore(FILE *arquivo_entrada, unsigned int tamanho_arvore) 
{
    if (tamanho_arvore == 0) 
    {
        return NULL;
    }

    unsigned int *array_arvore = obter_array_arvore(arquivo_entrada, tamanho_arvore);
    unsigned int *array_arvore_auxiliar = array_arvore;

    no_t *raiz_arvore = construir_arvore(&array_arvore);

    free(array_arvore_auxiliar);

    return (raiz_arvore);
}

FILE* remover_huff(char nome[])
{
    int n = strlen(nome);
    nome[n - 5] = '\0'; // Remove a extensão do arquivo
    return fopen(nome, "wb"); // Abre o arquivo de saída em modo escrita binária
}
/*
void macioFeliz (){

    printf(
        "           :YYJ     ~@&.                             .&&^                                            \n"
        "           &@&@P    7@@.   .::....    .:.     ..  .:  ?J.  ^@&..   .  .:.   .:.      .::.           \n"
        "          G@P &@7   7@@. ~&@55&@&? :B@&#&@G.  &@P&@& .@@~ ?@@@##! J@&B#&@@G##&@&^  ~#@&#@&5         \n"
        "         ?@@^ ?@@:  7@@. B@#  B@B  &@B   #@#  &@&:   .@@!  G@&    J@@~  #@@.  @@G .@@Y  .@@P        \n"
        "        :@@&&&&@@&  7@@. .&@GGBJ   @@P   B@&  &@P    .@@!  G@&    J@@   G@&   &@G :@@7   &@B        \n"
        "       .&@B    .@@G 7@@. :@@GGPY7. ~&@BP#@&^  &@G    .@@!  !@@GG7 Y@@   B@&   &@G  ?@@GP&@#.        \n"
        "       .~~      .~~ .~^ :&&7~~!&@B   :!7!:    :^. .^!J&&5?7~777~. .~^   :~:   ^~:    ^!7~.          \n"
        "                        ^&@PYYP&#^             :Y#@@@@&&&&#&&B5^                                    \n"
        "                          .^^^:             .J#@@@&&&&&&&&#&&@@&B!                                  \n"
        "                                            P&&&&&BG55B&@@@@@@@@@&G.                                \n"
        "                                           Y&##BY7!^..:~JPB###B#&&@#:                                \n"
        "                                          :&@&#Y^:.......:::^~~!7?G&7                                \n"
        "                                          .&@@&?~~77?J?!^^^~7JJJ?!P@J                                \n"
        "                                           7@@B~!?JY555?~^!5GBGPY?P@!                                \n"
        "                                            G&J^::::^^~^^^!J7!!??!JB.                                \n"
        "                                            .?~:::..::^!7!??!^^~~~!:                                 \n"
        "                                             :^^::::^^^~~!JYJ7~~~!.                                 \n"
        "                                             ^~^:::::^~~~!!7??~~!~                                  \n"
        "                                             YY~^::::^~~~!7?J7~!!:                                  \n"
        "                                            :PJ77~^^::::^^!7!!!7^                                    \n"
        "                                           ~5G?^!7?7!^^^~!7777?7.                                    \n"
        "                                      .:~7J55PY^:^!77?JYYYYYJ?7JG:                                  \n"
        "                               .^!7?J55PPPPP55PY^:^~7Y5P555J?7!5&G!.                                \n"
        "                          .!JPB#####BBGPPPPP555PY^:^!J5YYYY?77?G#BBGPY!:                             \n"
        "                        ~P###############BB#BBBBBBYJ?!7??????YG#BBGGB###B5?^.                        \n"
        "                      .5#######B##############&####&#J~~~^~?5GBB##GBBBB#####BY:                      \n"
        "                     ^B####BB##B###########B#####BBBBBP?^^?GB###############BBGJ                    \n"
        "                    7########BB###########BB#B########BBPG######BBBBBBBBBBBGPGGG5                    \n"
        "                  .Y##########B############B#####B######B##BGGPPPPPPPPPPPPGGGGBG#J                   \n"
        "                 ^G#####&##############B#BB###BB#BB####BBBBBBBGPPPPPPPPPPPPGGGBG#B!                  \n"
        "                .#######&############################&#B#########BBGGGGGGGGGGGG#&BP                  \n"
        "              ^~~!!!~~~~~~~~~~~~~!7777!!!!!!777!!!!!!!!~!!!!!!!!!!!!~~~~~~~~~5GY!~~                  \n"
        "              &@#                 B&&#     5@&&.                             P&!                     \n"
        "         ~PBG7#@B   ^5GGP?.       &@&@P   ~@&@@:  .5GGGP!   ~G~^PB~  ~5BBGJ  YG~   ~5BBG7.          \n"
        "        5@@J~J@@B  G@&~^Y@@.      &@?#@^ .@&~@@:   ?!~!@@G  5@@#YJ. #@&!~?!  @@P  #@&!~P@@^         \n"
        "        @@P   G@B .@@&PGG&&!      &@7.@& B@^.@@:  YBBP5&@#  Y@@.   ~@@^      @@5 ~@@^   &@B         \n"
        "        B@@^.^@@#  #@&^..:^       &@? Y@&@5 :@@: ?@@7.^@@&  5@@    .@@B:.:~  @@5 .&@B..7@@7         \n"
        "         J#&#YJ&5   7B####Y       P&~  G&G  .##.  5&#B57#5  7&B     .5#&##Y  G&?   J#&&&P^          \n"
        "                                         ^~.            :~:             ~~^                          \n"
        "                          ^7.           ~@@5           .&@#            .@@@                          \n"
        "                         !@@P^~. ^^  ^!^.!7::^^     .^^.~7^   :~!7!^   .@@&                          \n"
        "                        P@@@&#&!~@@P&@@G~@@G!@@B   .@@&^@@&  !&&BG&@@J .@@&                          \n"
        "                         P@@7   ~@@@!.  ~@@G Y@@?  #@@:.@@&   .:^^7@@@ .@@&                          \n"
        "                         P@@!   ~@@G    ~@@G  B@@.J@@! .@@& ~&@&G55@@@ .@@&                          \n"
        "                         5@@P...~@@G    !@@G   &@&@@Y  .@@& &@@?  7@@@ .@@&                          \n"
        "                         .B@@@@J^@@P    ~@@P   .&@@G   .&@# ^#@@&&5Y@& .&@#                          \n"
        "                            ..                                 ..                                    \n"
    );
}
*/
void descompactarPrincipal()
{
    char nome[100];
    FILE *arquivo_entrada;
    FILE *arquivo_saida;
    
    while (1) { // Loop até que o arquivo seja encontrado
        arquivo_entrada = AbrirArquivo(nome); // Chama a função AbrirArquivo
        
        if (arquivo_entrada == NULL) {
            printf("ERRO: Não existe arquivo com o nome digitado. Por favor, tente novamente.\n");
        } else {
            printf("Arquivo encontrado.\n");
            break; // Sai do loop se o arquivo for encontrado
        }
    }

    // Continua com a execução da função se o arquivo foi encontrado
    arquivo_saida = remover_huff(nome);
    unsigned int tamanho_lixo = obter_tamanho_lixo(arquivo_entrada);
    unsigned int tamanho_arvore = obter_tamanho_arvore(arquivo_entrada);
    no_t *arvore_huff = obter_arvore(arquivo_entrada, tamanho_arvore);
    descompactar(arquivo_entrada, tamanho_lixo, tamanho_arvore, arvore_huff, arquivo_saida);
    liberar_arvore(arvore_huff);
    fclose(arquivo_entrada);
    fclose(arquivo_saida);
}


