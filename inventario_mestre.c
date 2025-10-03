#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
   Código da Ilha – Edição Free Fire
   No nível: Mestre
   Agora tem: prioridade (1..5), ordenação (nome/tipo/prioridade) e busca binária por nome.

   
   Menu p/ ordenar por: nome, tipo ou prioridade
   Busca binária por nome (exigee estar ordenado por nome)
   Contador de comparacoes na ordenação (análise de desempenho)

   O jogador pode:
   Inserir item
   Remover item (por nome)
   Listar itens
   Ordenar (Insertion Sort)
   Buscar binária por nome (se estiver ordenado por nome)
*/

#define TAM_NOME 30
#define TAM_TIPO 20
#define CAPACIDADE 10

// enum do critério de ordenação
typedef enum {
    CRI_NOME = 0,
    CRI_TIPO = 1,
    CRI_PRIORIDADE = 2
} CriterioOrdenacao;

// struct do item (agora com prioridade 1..5)
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int  quantidade;
    int  prioridade; 
} Item;

// protótipos
void limparEntrada(void);
void lerString(char *dest, int tam, const char *rotulo);
int  lerInteiroPositivo(const char *rotulo);
int  lerInteiroEntre(const char *rotulo, int min, int max);

bool inserirItem(Item mochila[], int *usados);
bool removerItem(Item mochila[], int *usados);
void listarItens(const Item mochila[], int usados);

// ordenação por Insertion Sort + contador de comparações
long insertionSort(Item a[], int n, CriterioOrdenacao crit);

// busca binária por nome, conta comparações 
int  buscaBinariaPorNome(const Item a[], int n, const char *alvo, long *comparacoes);

// estado da ordenação: último critério e se está ordenado
typedef struct {
    bool ordenado;
    CriterioOrdenacao criterio;
} EstadoOrdenacao;

 
  // Menu. inserir, remover, listar, ordenar (nome/tipo/prioridade), buscar binária e sair.

int main(void) {
    Item mochila[CAPACIDADE];
    int usados = 0;

    EstadoOrdenacao estado = { .ordenado = false, .criterio = CRI_NOME };

    int opcao;

    puts(" Inventario (Nivel Mestre) ");
    puts("Vetor + prioridade + Insertion Sort + Busca Binaria por nome.");

    do {
        puts("\nMenu:");
        puts("1) Inserir novo item");
        puts("2) Remover item (por nome)");
        puts("3) Listar itens");
        puts("4) Ordenar itens");
        puts("5) Buscar BINARIA por nome (requer ordenado por nome)");
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
                if (inserirItem(mochila, &usados)) {
                    puts("[OK] Item inserido.");
                    estado.ordenado = false; // inseriu perde ordenação
                }
                listarItens(mochila, usados);
                break;

            case 2:
                if (removerItem(mochila, &usados)) {
                    puts("[OK] Item removido.");
                    estado.ordenado = false; // removeu perde ordenação
                }
                listarItens(mochila, usados);
                break;

            case 3:
                listarItens(mochila, usados);
                break;

            case 4: {
                puts("\nOrdenar por:");
                puts("1) Nome (A..Z)");
                puts("2) Tipo (A..Z)");
                puts("3) Prioridade (5..1)");
                printf("Escolha: ");
                int esc;
                if (scanf("%d", &esc) != 1) {
                    puts("Entrada invalida.");
                    limparEntrada();
                    break;
                }
                limparEntrada();

                CriterioOrdenacao crit;
                if (esc == 1) crit = CRI_NOME;
                else if (esc == 2) crit = CRI_TIPO;
                else if (esc == 3) crit = CRI_PRIORIDADE;
                else { puts("Opcao invalida."); break; }

                long comps = insertionSort(mochila, usados, crit);
                estado.ordenado = true;
                estado.criterio = crit;

                printf("[OK] Ordenado por %s. Comparacoes: %ld\n",
                    crit == CRI_NOME ? "NOME" :
                    crit == CRI_TIPO ? "TIPO" : "PRIORIDADE", comps);

                listarItens(mochila, usados);
            } break;

            case 5: {
                if (!(estado.ordenado && estado.criterio == CRI_NOME)) {
                    puts("[ERRO] Para busca binaria por nome, a mochila deve estar ORDENADA por NOME.");
                    puts("Use a opcao 4 e escolha 'Nome (A..Z)'.");
                    break;
                }
                char alvo[TAM_NOME];
                lerString(alvo, TAM_NOME, "Nome do item a buscar (binaria por nome)");
                long comps = 0;
                int idx = buscaBinariaPorNome(mochila, usados, alvo, &comps);
                if (idx >= 0) {
                    puts("\n[ENCONTRADO]");
                    printf("Nome: %s\n", mochila[idx].nome);
                    printf("Tipo: %s\n", mochila[idx].tipo);
                    printf("Quantidade: %d\n", mochila[idx].quantidade);
                    printf("Prioridade: %d\n", mochila[idx].prioridade);
                } else {
                    puts("\n[NAO ENCONTRADO]");
                }
                printf("Comparacoes (busca binaria): %ld\n", comps);
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


  // limpa stdin após scanf 

void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}


   // lê string e remove '\n' 

void lerString(char *dest, int tam, const char *rotulo) {
    printf("%s: ", rotulo);
    if (fgets(dest, tam, stdin) == NULL) { dest[0] = '\0'; return; }
    size_t n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') dest[n - 1] = '\0';
}


   // lê inteiro > 0 

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


   // lê inteiro no intervalo [min..max]

int lerInteiroEntre(const char *rotulo, int min, int max) {
    int v;
    while (true) {
        printf("%s (%d..%d): ", rotulo, min, max);
        if (scanf("%d", &v) != 1) { puts("Inteiro invalido."); limparEntrada(); continue; }
        limparEntrada();
        if (v < min || v > max) { puts("Fora do intervalo."); continue; }
        return v;
    }
}


  

bool inserirItem(Item mochila[], int *usados) {
    if (*usados >= CAPACIDADE) { puts("[ERRO] Mochila cheia."); return false; }

    Item novo;
    lerString(novo.nome, TAM_NOME, "Nome");
    if (strlen(novo.nome) == 0) { puts("[ERRO] Nome vazio."); return false; }

    lerString(novo.tipo, TAM_TIPO, "Tipo (arma/municao/cura/ferramenta)");
    if (strlen(novo.tipo) == 0) { puts("[ERRO] Tipo vazio."); return false; }

    novo.quantidade = lerInteiroPositivo("Quantidade");
    novo.prioridade = lerInteiroEntre("Prioridade", 1, 5); // 1..5

    mochila[*usados] = novo;
    (*usados)++;
    return true;
}


  // remove item por nome (strcmp). move o ultimo p/ pos removida

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


   // lista itens: indice, nome, tipo, quantidade, prioridade

void listarItens(const Item mochila[], int usados) {
    puts("\n--- Itens ---");
    if (usados == 0) { puts("(vazia)"); return; }
    for (int i = 0; i < usados; i++) {
        printf("%02d) %-20s | tipo: %-10s | qtd: %2d | prio: %d\n",
               i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
}


static int compara(const Item *a, const Item *b, CriterioOrdenacao crit) {
    if (crit == CRI_NOME) {
        return strcmp(a->nome, b->nome);
    } else if (crit == CRI_TIPO) {
        return strcmp(a->tipo, b->tipo);
    } else {
        // maior prioridade primeiro 
        return (b->prioridade - a->prioridade);
    }
}



long insertionSort(Item a[], int n, CriterioOrdenacao crit) {
    long comparacoes = 0;

    for (int i = 1; i < n; i++) {
        Item chave = a[i];
        int j = i - 1;

        // compara até achar posicao
        while (j >= 0) {
            comparacoes++;
            if (compara(&a[j], &chave, crit) > 0) {
                a[j + 1] = a[j];
                j--;
            } else {
                break;
            }
        }
        a[j + 1] = chave;
    }
    return comparacoes;
}


  // case-sensitive

int buscaBinariaPorNome(const Item a[], int n, const char *alvo, long *comparacoes) {
    int ini = 0, fim = n - 1;
    long comps = 0;

    while (ini <= fim) {
        int mid = (ini + fim) / 2;
        comps++;
        int c = strcmp(a[mid].nome, alvo);

        if (c == 0) { if (comparacoes) *comparacoes = comps; return mid; }
        else if (c < 0) ini = mid + 1;
        else            fim = mid - 1;
    }

    if (comparacoes) *comparacoes = comps;
    return -1;
}
