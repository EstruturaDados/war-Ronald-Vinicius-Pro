#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* -------------------- ESTRUTURAS -------------------- */

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* -------------------- FUNÇÕES DE MISSÃO -------------------- */

// Sorteia uma missão e copia para destino
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Exibe uma missão passada por valor (somente texto)
void exibirMissao(const char* missao) {
    printf("Sua missão: %s\n", missao);
}

// Verifica se a missão foi cumprida (lógica simples)
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {

    // Exemplo de condição simples: missão contém a palavra "Conquistar"
    // e o jogador possui ao menos 3 territórios
    if (strstr(missao, "Conquistar") != NULL) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) {
                count++;
            }
        }
        if (count >= 3) return 1;
    }

    // Exemplo de eliminar tropas vermelhas
    if (strstr(missao, "vermelha") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelha") == 0) {
                return 0; // ainda existe tropa vermelha
            }
        }
        return 1; // todas eliminadas
    }

    return 0;
}

/* -------------------- FUNÇÃO DE ATAQUE -------------------- */

void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Não é possível atacar territórios da mesma cor.\n");
        return;
    }

    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("%s (A) rolou %d | %s (D) rolou %d\n",
           atacante->nome, dadoA, defensor->nome, dadoD);

    if (dadoA > dadoD) {
        printf("Atacante venceu! Conquistou o território.\n");

        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= atacante->tropas / 2;
    } else {
        printf("Defensor resistiu! Atacante perde 1 tropa.\n");
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/* -------------------- EXIBIÇÃO -------------------- */

void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n------ MAPA ------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%s | Cor: %s | Tropas: %d\n",
               mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------\n");
}

/* -------------------- LIBERAÇÃO DE MEMÓRIA -------------------- */

void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
}

/* --------------------------- MAIN --------------------------- */

int main() {
    srand(time(NULL));

    /* --- Criando o vetor de missões --- */
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Conquistar ao menos 2 territorios centrais",
        "Acumular 10 tropas",
        "Controlar qualquer continente completamente"
    };
    int totalMissoes = 5;

    /* --- Alocando a missão do jogador --- */
    char* missaoJogador = (char*) malloc(100 * sizeof(char));
    atribuirMissao(missaoJogador, missoes, totalMissoes);

    printf("=== MISSÃO SORTEADA ===\n");
    exibirMissao(missaoJogador);

    /* --- Criando mapa --- */
    int tamanho = 5;
    Territorio* mapa = (Territorio*) calloc(tamanho, sizeof(Territorio));

    // Exemplos de territórios
    strcpy(mapa[0].nome, "Territorio A"); strcpy(mapa[0].cor, "Azul"); mapa[0].tropas = 5;
    strcpy(mapa[1].nome, "Territorio B"); strcpy(mapa[1].cor, "Vermelha"); mapa[1].tropas = 3;
    strcpy(mapa[2].nome, "Territorio C"); strcpy(mapa[2].cor, "Azul"); mapa[2].tropas = 4;
    strcpy(mapa[3].nome, "Territorio D"); strcpy(mapa[3].cor, "Verde"); mapa[3].tropas = 2;
    strcpy(mapa[4].nome, "Territorio E"); strcpy(mapa[4].cor, "Vermelha"); mapa[4].tropas = 1;

    /* --- Loop do jogo (simplificado) --- */

    int turno = 1, venceu = 0, opcao, a, d;

    while (!venceu) {

        printf("\n===== TURNO %d =====\n", turno);
        exibirMapa(mapa, tamanho);

        printf("\n1 - Atacar\n2 - Passar turno\nOpcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            printf("Escolha atacante (0-%d): ", tamanho - 1);
            scanf("%d", &a);
            printf("Escolha defensor (0-%d): ", tamanho - 1);
            scanf("%d", &d);

            atacar(&mapa[a], &mapa[d]);
        }

        // Verificação silenciosa da missão
        if (verificarMissao(missaoJogador, mapa, tamanho)) {
            printf("\n=== MISSÃO CUMPRIDA! VOCÊ VENCEU! ===\n");
            venceu = 1;
        }

        turno++;
        if (turno > 30) { // Limite de segurança
            printf("Fim do jogo (limite de turnos).\n");
            break;
        }
    }

    liberarMemoria(mapa, missaoJogador);

    return 0;
}

