#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
   Código da Ilha – Edição Free Fire
   No nível: Aventureiro
   agora tem BUSCA por nome (sequencial, strcmp).

   Inserir item
   Remover item (por nome)
    Listar itens
   Buscar item por nome (sequencial com strcmp)
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
int  buscarItemPorNome(const Item mochila[], int usados, const char *alvo);

/* Menu: inserir remover listar buscar (strcmp) e sair. */
int main(void) {
    Item mochila[CAPACIDADE];
    int usados = 0;
    int opcao;

    puts("=== Inventario (Nivel Aventureiro) ===");
    puts("Vetor (10 itens) + BUSCA por nome (strcmp).");

    do {
        puts("\nMenu:");
        puts("1) Inserir novo item");
        puts("2) Remover item (por nome)");
        puts("3) Listar itens");
        puts("4) Buscar item por nome");
        puts("0) Sair");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            puts("Entrada invalida.");
            limparEntrada();
            continue;
        }
        limparEntrada();

        switch (opcao) {
            case 1:
                if (inserirItem(mochila, &usados)) puts("[OK] Item inserido.");
                listarItens(mochila, usados);
                break;

            case 2:
                if (removerItem(mochila, &usados)) puts("[OK] Item removido.");
                listarItens(mochila, usados);
                break;

            case 3:
                listarItens(mochila, usados);
                break;

            case 4: {
                char alvo[TAM_NOME];
                lerString(alvo, TAM_NOME, "Nome do item a buscar");
                int idx = buscarItemPorNome(mochila, usados, alvo);
                if (idx >= 0) {
                    puts("\n[ENCONTRADO]");
                    printf("Nome: %s\n", mochila[idx].nome);
                    printf("Tipo: %s\n", mochila[idx].tipo);
                    printf("Quantidade: %d\n", mochila[idx].quantidade);
                } else {
                    puts("\n[NAO ENCONTRADO]");
                }
                listarItens(mochila, usados);
            } break;

            case 0:
                puts("Saindo...");
                break;

            default:
                puts("Opcao invalida.");
        }
    } while (opcao != 0);

    return 0;
}

/* limpa stdin após scanf */
void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* lê string com rótulo (fgets) e remove '\n' */
void lerString(char *dest, int tam, const char *rotulo) {
    printf("%s: ", rotulo);
    if (fgets(dest, tam, stdin) == NULL) { dest[0] = '\0'; return; }
    size_t n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') dest[n - 1] = '\0';
}

/* lê inteiro > 0 */
int lerInteiroPositivo(const char *rotulo) {
    int v;
    while (true) {
        printf("%s: ", rotulo);
        if (scanf("%d", &v) != 1) { puts("Inteiro invalido."); limparEntrada(); continue; }
        limparEntrada();
        if (v <= 0) { puts("Deve ser > 0."); continue; }
        return v;
    }
}

/* insere item se houver espaço */
bool inserirItem(Item mochila[], int *usados) {
    if (*usados >= CAPACIDADE) { puts("[ERRO] Mochila cheia."); return false; }

    Item novo;
    lerString(novo.nome, TAM_NOME, "Nome");
    if (strlen(novo.nome) == 0) { puts("[ERRO] Nome vazio."); return false; }

    lerString(novo.tipo, TAM_TIPO, "Tipo (arma/municao/cura/ferramenta)");
    if (strlen(novo.tipo) == 0) { puts("[ERRO] Tipo vazio."); return false; }

    novo.quantidade = lerInteiroPositivo("Quantidade");
    mochila[*usados] = novo;
    (*usados)++;
    return true;
}

/* remove item por nome (strcmp). move o ultimo p/ pos removida */
bool removerItem(Item mochila[], int *usados) {
    if (*usados == 0) { puts("[ERRO] Mochila vazia."); return false; }

    char alvo[TAM_NOME];
    lerString(alvo, TAM_NOME, "Nome do item a remover");

    int idx = -1;
    for (int i = 0; i < *usados; i++) {
        if (strcmp(mochila[i].nome, alvo) == 0) { idx = i; break; }
    }
    if (idx < 0) { puts("[INFO] Nao encontrado."); return false; }

    mochila[idx] = mochila[*usados - 1];
    (*usados)--;
    return true;
}

/* lista o vetor */
void listarItens(const Item mochila[], int usados) {
    puts("\n--- Itens ---");
    if (usados == 0) { puts("(vazia)"); return; }
    for (int i = 0; i < usados; i++) {
        printf("%02d) %-20s | tipo: %-10s | qtd: %d\n",
               i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

/* busca sequencial por nome (strcmp). retorna indice ou -1 */
int buscarItemPorNome(const Item mochila[], int usados, const char *alvo) {
    bool found = false;
    int indice = -1;
    for (int i = 0; i < usados; i++) {
        if (strcmp(mochila[i].nome, alvo) == 0) { found = true; indice = i; break; }
    }
    return found ? indice : -1;
}
