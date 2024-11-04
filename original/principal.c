#include "compactador.h"
#include "descompactar.h"
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
int main() {
    char nome_arquivo_original[100], nome_arquivo_compactado[100];
    int opcao;
    char felicidade;

    
    do
    {
        printf("\nDigite a opcao que voce quer:\n 1 - compactar;\n 2 - descompactar;\n 3 - sair\n");
        scanf("%d",&opcao);
        getchar();

        if( opcao == 1){
            // Lendo os nomes dos arquivos
            printf("Digite o nome do arquivo original: ");
            fgets(nome_arquivo_original, sizeof(nome_arquivo_original), stdin);
            nome_arquivo_original[strcspn(nome_arquivo_original, "\n")] = 0; // Remover nova linha

            printf("Digite o nome do arquivo compactado: ");
            fgets(nome_arquivo_compactado, sizeof(nome_arquivo_compactado), stdin);
            nome_arquivo_compactado[strcspn(nome_arquivo_compactado, "\n")] = 0; // Remover nova linha

            compactar_arquivo(nome_arquivo_original, nome_arquivo_compactado);

        }else if( opcao == 2){
            printf("Marcio esta feliz?");
            scanf(" %c",&felicidade);
            getchar();

            if( felicidade == 's'){
                //macioFeliz();
                descompactarPrincipal();
            }else{
                descompactarPrincipal();
            }

        }else{
            printf("opcao invalida\n");
        }

    } while (opcao != 3);
    

    

    return 0;
}