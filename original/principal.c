#include "compactador.h"

int main() {
    char nome_arquivo_original[100], nome_arquivo_compactado[100],opcao[20];

    
    do
    {
        printf("Digite a opcao que voce quer: compactar, descompactar ou sair\n");
        fgets(opcao,20,stdin);
        opcao[strcspn(opcao, "\n")] = 0; // Remover nova linha

        if( strcmp(opcao, "compactar") == 0){
            // Lendo os nomes dos arquivos
            printf("Digite o nome do arquivo original: ");
            fgets(nome_arquivo_original, sizeof(nome_arquivo_original), stdin);
            nome_arquivo_original[strcspn(nome_arquivo_original, "\n")] = 0; // Remover nova linha

            printf("Digite o nome do arquivo compactado: ");
            fgets(nome_arquivo_compactado, sizeof(nome_arquivo_compactado), stdin);
            nome_arquivo_compactado[strcspn(nome_arquivo_compactado, "\n")] = 0; // Remover nova linha

            compactar_arquivo(nome_arquivo_original, nome_arquivo_compactado);

        }else if( strcmp(opcao,"descompactar") == 0){
            printf("EM processo\n");
        }else{
            printf("opcao invalida\n");
        }

    } while (strcmp(opcao,"sair") != 0);
    

    

    return 0;
}