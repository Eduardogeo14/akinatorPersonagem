#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct No{

    char texto[50];
    bool ehPergunta;
    struct No *esq;
    struct No *dir;

}No;


No *criarNo(char *texto, bool ehPergunta){

    No *novo = (No*) malloc (sizeof(No));

    strcpy(novo->texto, texto);
    novo->ehPergunta = ehPergunta;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;

}

No *criarArvore(){

    No *raiz = criarNo("O seu personagem tem poderes magicos? \n", true);

    raiz->esq = criarNo("Ele e da Marvel?", true);
    raiz->esq->esq = criarNo("Dr. Estranho", false);
    raiz->esq->dir = criarNo("Harry Potter", false);

    raiz->dir = criarNo("Ele e um detetive?", true);
    raiz->dir->esq = criarNo("Sherlock Holmes", false);
    raiz->dir->dir = criarNo("Batman", false);

    return raiz;
}

No *jogarEinserir(No *atual){

    if(atual == NULL){
        return NULL;
    }

    if(atual->ehPergunta == false){
        printf("O personagem que vc pensou foi o: %s ? \n", atual->texto);

        char resposta;
        printf("S - Sim | N - Nao \n");
        scanf(" %c", &resposta);

        if(resposta == 'S' || resposta == 's'){
            printf("Acertei !!! \n");
            return atual;
        } else {

            char novoPersonagem[50];
            char novaPerguntaTexto[50];

            printf("Qual personagem vc pensou ? ");
            scanf(" %49[^\n]", novoPersonagem);
            getchar();

            printf("Qual pergunta difere o personagem %s do personagem %s ?", atual->texto, novoPersonagem);
            scanf(" %49[^\n]", novaPerguntaTexto);
            getchar();

            No *novoNo = criarNo(novoPersonagem, false);

            No *novaPergunta = criarNo(novaPerguntaTexto, true);

            novaPergunta->esq = novoNo;
            novaPergunta->dir = atual;

            return novaPergunta;

        }
    }

    char resposta;
    printf("\n %s (S/N) ? ", atual->texto);
    scanf(" %c", &resposta);

    if(resposta == 'S' || resposta == 's'){
        atual->esq = jogarEinserir(atual->esq);
    } else {
        atual->dir = jogarEinserir(atual->dir);
    }

    return atual;
}

void salvarArvore(No *atual, FILE *arquivo){

    if(atual == NULL){
        int tamanhoZero = 0;
        fwrite(&tamanhoZero, sizeof(int), 1, arquivo);
        return;
    }

    int tamanho = strlen(atual->texto) + 1;
    fwrite(&tamanho, sizeof(int), 1, arquivo);
    fwrite(atual->texto, sizeof(char), tamanho, arquivo);
    fwrite(&atual->ehPergunta, sizeof(bool), 1, arquivo);

    salvarArvore(atual->esq, arquivo);
    salvarArvore(atual->dir, arquivo);

}

No *carregarArvore(FILE *arquivo) {
    int tamanho;

    if (fread(&tamanho, sizeof(int), 1, arquivo) != 1 || tamanho == 0) {
        return NULL;
    }

    char texto[50];
    fread(texto, sizeof(char), tamanho, arquivo);

    bool ehPergunta;
    fread(&ehPergunta, sizeof(bool), 1, arquivo);

    No *novo = criarNo(texto, ehPergunta);

    novo->esq = carregarArvore(arquivo);
    novo->dir = carregarArvore(arquivo);

    return novo;
}

int main() {

    system("color 0B");
    No *raiz = NULL;
    FILE *arquivo = fopen("akinator.dat", "rb");

    if (arquivo != NULL) {
        raiz = carregarArvore(arquivo);
        fclose(arquivo);
        printf("Arvore carregada com sucesso do arquivo!\n");
    } else {
        raiz = criarArvore();
        printf("Novo jogo iniciado!\n");
    }

    int continuar = 1;
    while (continuar) {
        printf("--- Iniciando o jogo ---\n");
        raiz = jogarEinserir(raiz);

        printf("Jogar novamente? (1-Sim | 0-Nao): ");
        scanf("%d", &continuar);
    }

    arquivo = fopen("akinator.dat", "wb");
    if (arquivo != NULL) {
        salvarArvore(raiz, arquivo);
        fclose(arquivo);
        printf("Arvore salva com sucesso no arquivo!\n");
    } else {
        printf("Erro ao salvar o arquivo!\n");
    }

    return 0;
}
