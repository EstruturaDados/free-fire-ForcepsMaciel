#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
   Código da Ilha – Edição Free Fire
   No nível: Novato
   O programa está simulando a mochila inicial do jogador conforme prescrito.

   - Inserir um item
   - Remover um item (por nome)
   - Listar todos os itens
   - Buscar um item por nome (busca sequencial)
*/   



#define TAM_NOME 30
#define TAM_TIPO 20
#define CAPACIDADE 10

typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int  quantidade;
} Item;


void limparEntrada(void);
void lerString(char *dest, int tam, const char *rotulo);
int  lerInteiroPositivo(const char *rotulo);

bool inserirItem(Item mochila[], int *usados);
bool removerItem(Item mochila[], int *usados);
void listarItens(const Item mochila[], int usados);
int  buscarItemSequencial(const Item mochila[], int usados,
                          const char *alvo, bool explicativo);

 
  // Toda a função principa tem: inserir novo item, remover item (por nome), listar itens, buscar item (por nome), com opção de modo explicativo (humano) e sair.

int main(void) {
    Item mochila[CAPACIDADE];
    int usados = 0;
    int opcao;

    puts("=== Inventario do Jogador (Nivel Novato) ===");
    puts("Mochila: lista ESTATICA (vetor) com capacidade para 10 itens.");
    puts("Opcoes: inserir, remover, listar e buscar (sequencial).");
    puts("-------------------------------------------------------------");

    do {
        puts("\nMenu:");
        puts("1) Inserir novo item");
        puts("2) Remover item (por nome)");
        puts("3) Listar itens");
        puts("4) Buscar item (por nome)");
        puts("0) Sair");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            puts("Entrada invalida. Tente novamente.");
            limparEntrada();
            continue;
        }
        limparEntrada();

        switch (opcao) {
            case 1:
                if (inserirItem(mochila, &usados)) {
                    puts("[OK] Item inserido.");
                }
                listarItens(mochila, usados);
                break;

            case 2:
                if (removerItem(mochila, &usados)) {
                    puts("[OK] Item removido.");
                }
                listarItens(mochila, usados);
                break;

            case 3:
                listarItens(mochila, usados);
                break;

            case 4: {
                char alvo[TAM_NOME];
                lerString(alvo, TAM_NOME, "Nome do item a buscar");

                int modo;
                puts("Buscar de forma:");
                puts("1) Explicativa (mostrando cada comparacao)");
                puts("2) Direta (apenas resultado)");
                printf("Escolha: ");
                if (scanf("%d", &modo) != 1) {
                    puts("Entrada invalida. Usando modo direto.");
                    limparEntrada();
                    modo = 2;
                }
                limparEntrada();

                int idx = buscarItemSequencial(mochila, usados, alvo, (modo == 1));
                if (idx >= 0) {
                    printf("[ENCONTRADO] %s | tipo: %s | qtd: %d (pos %d)\n",
                           mochila[idx].nome, mochila[idx].tipo,
                           mochila[idx].quantidade, idx);
                } else {
                    puts("[NAO ENCONTRADO] Esse item nao esta na mochila.");
                }
                listarItens(mochila, usados);
            } break;

            case 0:
                puts("Saindo... Boa sorte na ilha!");
                break;

            default:
                puts("Opcao invalida. Tente novamente.");
        }

    } while (opcao != 0);

    return 0;
}


  // limparEntrada(), simula o "flush" do stdin lendo e descartando caracteres até '\n' ou EOF, Útil após scanf pra evitar quebras nas próximas leituras.

void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descarta */ }
}


   // lerString(dest, tam, rotulo), lê uma string com rótulo usando fgets, remove o '\n' final (se existir) e garante terminação NUL. Em caso de erro, grava string vazia.

void lerString(char *dest, int tam, const char *rotulo) {
    printf("%s: ", rotulo);
    if (fgets(dest, tam, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }
    size_t n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') dest[n - 1] = '\0';
}


   // lerInteiroPositivo(rotulo), lê um inteiro > 0. Em caso de entrada inválida ou <= 0, solicita novamente.

int lerInteiroPositivo(const char *rotulo) {
    int v;
    while (true) {
        printf("%s: ", rotulo);
        if (scanf("%d", &v) != 1) {
            puts("Valor invalido. Digite um inteiro.");
            limparEntrada();
            continue;
        }
        limparEntrada();
        if (v <= 0) {
            puts("O valor deve ser > 0. Tente novamente.");
            continue;
        }
        return v;
    }
}


  // inserirItem(mochila, usados), adiciona um novo item à mochila se tiver espaço. Evita nome duplicado, solicita nome, tipo e quantidade (> 0), e retorna true em sucesso; false caso contrário.

static int stricmp_ascii(const char *a, const char *b) {
    
    while (*a && *b) {
        unsigned char ca = (unsigned char)*a;
        unsigned char cb = (unsigned char)*b;
        if (ca >= 'A' && ca <= 'Z') ca = (unsigned char)(ca - 'A' + 'a');
        if (cb >= 'A' && cb <= 'Z') cb = (unsigned char)(cb - 'A' + 'a');
        if (ca != cb) return (int)ca - (int)cb;
        ++a; ++b;
    }
    unsigned char ca = (unsigned char)*a;
    unsigned char cb = (unsigned char)*b;
    if (ca >= 'A' && ca <= 'Z') ca = (unsigned char)(ca - 'A' + 'a');
    if (cb >= 'A' && cb <= 'Z') cb = (unsigned char)(cb - 'A' + 'a');
    return (int)ca - (int)cb;
}

bool inserirItem(Item mochila[], int *usados) {
    if (*usados >= CAPACIDADE) {
        puts("[ERRO] Mochila cheia. Remova algo antes de inserir.");
        return false;
    }

    Item novo;
    lerString(novo.nome, TAM_NOME, "Nome");
    if (strlen(novo.nome) == 0) {
        puts("[ERRO] Nome nao pode ser vazio.");
        return false;
    }

    // verifica duplicidade por nome (case-insensitive)
    for (int i = 0; i < *usados; i++) {
        if (stricmp_ascii(mochila[i].nome, novo.nome) == 0) {
            puts("[ERRO] Ja existe um item com esse nome. Remova ou edite primeiro.");
            return false;
        }
    }

    lerString(novo.tipo, TAM_TIPO, "Tipo (arma/municao/cura/ferramenta)");
    if (strlen(novo.tipo) == 0) {
        puts("[ERRO] Tipo nao pode ser vazio.");
        return false;
    }

    novo.quantidade = lerInteiroPositivo("Quantidade");
    mochila[*usados] = novo;
    (*usados)++;
    return true;
}


  // removerItem(mochila, usados), remove item pelo nome, se encontrado. Também move o último elemento e retorna true em sucesso; false caso contrário.

bool removerItem(Item mochila[], int *usados) {
    if (*usados == 0) {
        puts("[ERRO] Mochila vazia.");
        return false;
    }

    char alvo[TAM_NOME];
    lerString(alvo, TAM_NOME, "Nome do item a remover");

    int idx = -1;
    for (int i = 0; i < *usados; i++) {
        if (stricmp_ascii(mochila[i].nome, alvo) == 0) {
            idx = i;
            break;
        }
    }

    if (idx < 0) {
        puts("[INFO] Item nao encontrado. Nada removido.");
        return false;
    }

    mochila[idx] = mochila[*usados - 1];
    (*usados)--;
    return true;
}


   // exibe uma tabela com índice, nome, tipo e quantidade dos itens presentes e se estver vazia, vai mostra "(vazia)".

void listarItens(const Item mochila[], int usados) {
    puts("\n--- Itens na mochila ---");
    if (usados == 0) {
        puts("(vazia)");
        return;
    }
    for (int i = 0; i < usados; i++) {
        printf("%02d) %-20s | tipo: %-10s | qtd: %d\n",
               i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
    puts("------------------------");
}


  // realizando busca sequencial por nome (case-insensitive).

int buscarItemSequencial(const Item mochila[], int usados,
                         const char *alvo, bool explicativo) {
    int comparacoes = 0;

    for (int i = 0; i < usados; i++) {
        comparacoes++;
        if (explicativo) {
            printf("Comparando \"%s\" com \"%s\" (pos %d)... ",
                   alvo, mochila[i].nome, i);
        }

        if (stricmp_ascii(mochila[i].nome, alvo) == 0) {
            if (explicativo) {
                printf("IGUAL -> encontrado!\n");
                printf("[Resumo] Comparacoes: %d\n", comparacoes);
            }
            return i;
        } else if (explicativo) {
            printf("diferente.\n");
        }
    }

    if (explicativo) {
        printf("[Resumo] Nao encontrado apos %d comparacoes.\n", comparacoes);
    }
    return -1;
}
