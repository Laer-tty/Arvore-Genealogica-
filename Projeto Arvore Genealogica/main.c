#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    char nome[50];
    char nome_conjuge[50];
    struct no *filho_mais_velho; //Ponteiro para esquerda
    struct no *proximo_irmao;     //Ponteiro para direita
} NoArv;

//-------------------------------------
// FUNÇÕES PARA MANIPULAÇÃO DA ÁRVORE
//-------------------------------------

//Cria um novo nó na árvore
NoArv* criar_no(char* nome, char* nome_conjuge) {
    NoArv* novo_no = (NoArv*) malloc(sizeof(NoArv));
    if (novo_no != NULL) {
        strcpy(novo_no->nome, nome);
        strcpy(novo_no->nome_conjuge, nome_conjuge);
        novo_no->filho_mais_velho = NULL;
        novo_no->proximo_irmao = NULL;
    }
    return novo_no;
}

//Busca um nó na árvore pelo nome
NoArv* buscar_no(NoArv* raiz, char* nome) {
    if (raiz == NULL) {
        return NULL;
    }
    if (strcmp(raiz->nome, nome) == 0) {
        return raiz;
    }
    NoArv* no_encontrado = buscar_no(raiz->filho_mais_velho, nome);
    if (no_encontrado != NULL) {
        return no_encontrado;
    }
    return buscar_no(raiz->proximo_irmao, nome);
}

NoArv* inserir_no(NoArv* pai, char* nome, char* nome_conjuge) {
    NoArv* novo_no = criar_no(nome, nome_conjuge);
    if (pai->filho_mais_velho == NULL) {
        pai->filho_mais_velho = novo_no;
    } else {
        NoArv* irmao = pai->filho_mais_velho;
        while (irmao->proximo_irmao != NULL) {
            irmao = irmao->proximo_irmao;
        }
        irmao->proximo_irmao = novo_no;
    }
    return pai;
}

//Insere uma pessoa na árvore
void inserir_pessoa(NoArv** raiz) {
    char nome[50], nome_conjuge[50], nome_pai[50];
    printf("\n\tDigite um nome: ");
    scanf("%s", nome);
    printf("\n\tDigite o nome do conjuge: ");
    scanf("%s", nome_conjuge);
    printf("\n\tDigite o nome do pai (ou NULL se for raiz): ");
    scanf("%s", nome_pai);

    if (strcmp(nome_pai, "NULL") == 0) {
        *raiz = criar_no(nome, nome_conjuge);
    } else {
        NoArv* pai = buscar_no(*raiz, nome_pai);
        if (pai != NULL) {
            if (strcmp(pai->nome_conjuge, nome_pai) == 0) {
                printf("\tErro: o nome do pai nao pode ser o mesmo que o nome do conjuge.\n");
            } else {
                inserir_no(pai, nome, nome_conjuge);
                printf("\tValor inserido com sucesso.\n");
            }
        } else {
            printf("\tPai nao encontrado.\n");
        }
    }
}

//Imprime a árvore indentada
void imprimir_arvore(NoArv* raiz, int nivel) {
    if (raiz) {
        for (int i = 0; i < nivel; i++) {
            printf("  ");
        }
        printf("%s - %s\n", raiz->nome, raiz->nome_conjuge);
        imprimir_arvore(raiz->filho_mais_velho, nivel + 1);
        imprimir_arvore(raiz->proximo_irmao, nivel);
    }
}

//Função para buscar o pai de um nó
NoArv* buscar_pai(NoArv* raiz, char* nome) {
    if (raiz == NULL) {
        return NULL;
    }
    if (raiz->filho_mais_velho && strcmp(raiz->filho_mais_velho->nome, nome) == 0) {
        return raiz;
    }
    NoArv* filho = raiz->filho_mais_velho;
    while (filho != NULL) {
        if (filho->proximo_irmao && strcmp(filho->proximo_irmao->nome, nome) == 0) {
            return raiz;
        }
        filho = filho->proximo_irmao;
    }
    NoArv* pai = buscar_pai(raiz->filho_mais_velho, nome);
    if (pai != NULL) {
        return pai;
    }
    return buscar_pai(raiz->proximo_irmao, nome);
}

// Verifica se a é pai de b
int e_pai(NoArv* a, NoArv* b) {
    NoArv* pai = buscar_pai(a, b->nome);
    return pai != NULL && strcmp(pai->nome, a->nome) == 0;
}

// Verifica se a é filho de b
int e_filho(NoArv* a, NoArv* b) {
    return e_pai(b, a);
}

// Verifica se a e b são irmãos
int sao_irmaos(NoArv* raiz, NoArv* a, NoArv* b) {
    NoArv* pai_de_a = buscar_pai(raiz, a->nome);
    NoArv* pai_de_b = buscar_pai(raiz, b->nome);
    return pai_de_a == pai_de_b && pai_de_a != NULL;
}

// Verifica se a é tio de b
int e_tio(NoArv* raiz, NoArv* a, NoArv* b) {
    NoArv* pai_de_b = buscar_pai(raiz, b->nome);
    return pai_de_b != NULL && sao_irmaos(raiz, a, pai_de_b);
}

// Verifica se a é sobrinho de b
int e_sobrinho(NoArv* raiz, NoArv* a, NoArv* b) {
    return e_tio(raiz, b, a);
}

// Verifica se a e b são primos
int sao_primos(NoArv* raiz, NoArv* a, NoArv* b) {
    NoArv* pai_de_a = buscar_pai(raiz, a->nome);
    NoArv* pai_de_b = buscar_pai(raiz, b->nome);
    return sao_irmaos(raiz, pai_de_a, pai_de_b);
}

//Função para determinar o parentesco de duas pessoas
void determinar_parentesco(NoArv* raiz, char* nome1, char* nome2) {
    NoArv* pessoa1 = buscar_no(raiz, nome1);
    NoArv* pessoa2 = buscar_no(raiz, nome2);

    if (pessoa1 == NULL || pessoa2 == NULL) {
        printf("\tUma ou ambas as pessoas nao foram encontradas na arvore.\n");
        return;
    } else {
        if (e_pai(pessoa1, pessoa2)) {
            printf("\n\t%s eh pai/mae de %s.\n", pessoa1->nome, pessoa2->nome);
        } else if (sao_irmaos(raiz,pessoa1, pessoa2)) {
            printf("\n\t%s e %s sao irmaos.\n", pessoa1->nome, pessoa2->nome);
            printf("\n\t%s eh cunhado/cunhada de %s.\n",pessoa1->nome, pessoa2->nome_conjuge);
        } else if (e_filho(pessoa1, pessoa2)) {
            printf("\n\t%s eh filho/filha de %s.\n", pessoa1->nome, pessoa2->nome);
        } else if (e_tio(raiz,pessoa1, pessoa2)) {
            printf("\n\t%s eh tio/tia de %s.\n", pessoa1->nome, pessoa2->nome);
        } else if (e_sobrinho(raiz, pessoa1, pessoa2)) {
            printf("\n\t%s eh sobrinho/sobrinha de %s.\n", pessoa1->nome, pessoa2->nome);
        } else if (sao_primos(raiz,pessoa1, pessoa2)) {
            printf("\n\t%s e %s sao primos.\n", pessoa1->nome, pessoa2->nome);
        } else {
            printf("\n\tNao foi possivel determinar o parentesco entre %s e %s.\n", pessoa1->nome, pessoa2->nome);
        }
    }
}

//-------------------------
// CAMADA DE PERSISTENCIA
//-------------------------

//Carrega a árvore a partir de um arquivo de texto
int carrega_arquivo(NoArv** raiz) {
    FILE* arquivo;
    arquivo = fopen("arvore_genealogica.txt", "r");
    if (arquivo == NULL) {
        printf("\tErro ao abrir o arquivo para leitura.\n");
        return 0;
    }

    char nome[50], nome_conjuge[50];
    int nivel, nivel_atual = -1;
    NoArv* nos[100]; // Array para manter os nós por nível

    while (fscanf(arquivo, "%s %s %d", nome, nome_conjuge, &nivel) != EOF) {
        NoArv* novo_no = criar_no(nome, nome_conjuge);
        if (nivel == 0) {
            *raiz = novo_no;
        } else {
            if (nivel > nivel_atual) {
                nos[nivel-1]->filho_mais_velho = novo_no;
            } else {
                NoArv* irmao = nos[nivel-1]->filho_mais_velho;
                while (irmao->proximo_irmao != NULL) {
                    irmao = irmao->proximo_irmao;
                }
                irmao->proximo_irmao = novo_no;
            }
        }
        nos[nivel] = novo_no;
        nivel_atual = nivel;
    }

    fclose(arquivo);
    return 1;
}

//salva a árvore em ordem em um arquivo
void salvar_em_ordem(NoArv* raiz, FILE* arquivo, int nivel) {
    if (raiz) {
        fprintf(arquivo, "%s %s %d\n", raiz->nome, raiz->nome_conjuge, nivel);
        salvar_em_ordem(raiz->filho_mais_velho, arquivo, nivel + 1);
        salvar_em_ordem(raiz->proximo_irmao, arquivo, nivel);
    }
}

//Salva a árvore em um arquivo de texto
void salvar_arvore(NoArv* raiz) {
    FILE* arquivo;
    arquivo = fopen("arvore_genealogica.txt", "w");
    if (arquivo == NULL) {
        printf("\tErro ao abrir o arquivo para escrita.\n");
        return;
    }
    salvar_em_ordem(raiz, arquivo,0);
    fclose(arquivo);
}

//-------------------------
//          TELAS
//-------------------------

//Monta o menu de opções
void monta_tela() {
    printf("    -----------------------");
    printf("\n\t1 - Ler dados do arquivo\n\t2 - Listar arvore genealogica indentada\n\t3 - Inserir pessoas para arvore genealogica\n\t4 - Indicar parentesco entre duas pessoas\n\t5 - Sair e Gravar dados do arquivo\n");
    printf("    -----------------------\n\t");
}

//Finaliza a opção escolhida
void termina_opcao() {
    printf("\n\t<tecle ENTER para continuar>"); getchar(); getchar();
}

//Limpa a tela
void limpa_tela() {
    system("cls");
}

//Escolhe uma opção do menu
char escolhe_opcao() {
    return getchar();
}

//-------------------------
//   PROGRAMA PRINCIPAL
//-------------------------

int main() {
    NoArv *raiz = NULL;
    char opcao;
    int arquivo_carregado = 0;

    while (1) {
        limpa_tela();
        monta_tela();
        printf("Escolha uma opcao: ");
        opcao = escolhe_opcao();
        switch (opcao) {
            case '1': //ler o arquivo de texto
                if (arquivo_carregado || raiz != NULL) {
                    printf("\n\tOs dados ja foram carregados.\n");
                } else {
                    arquivo_carregado = carrega_arquivo(&raiz);
                    if (arquivo_carregado) {
                        printf("\n\tDados carregados com sucesso.\n");
                    }
                }
                termina_opcao();
                break;
            case '2': //impressão da arvore genealogica indentada
                if (raiz == NULL) {
                    printf("\n\tArvore genealogica nao carregada ou arquivo vazio. Por favor, carregue os dados do arquivo primeiro.\n");
                } else {
                    printf("\n\tImpressao da Arvore Genealogica Indentada:\n\n");
                    imprimir_arvore(raiz, 0);
                }
                termina_opcao();
                break;
            case '3': //inserir pessoa na arvore genealogia
                if (raiz == NULL && !arquivo_carregado) {
                    int resposta;
                    printf("\n\tArvore genealogica nao carregada ou arquivo vazio. Deseja inserir dados mesmo assim? \n\t1 - Sim \n\t2 - Nao\n ");
                    printf("\tEscolha uma opcao: ");
                    scanf("%d",&resposta);
                    switch(resposta) {
                        case 1:
                            inserir_pessoa(&raiz);
                            break;
                        case 2:
                            printf("\n\tInsercao cancelada.\n");
                            break;
                        default:
                            printf("\n\tOpcao invalida!\n");
                    }
                } else {
                    inserir_pessoa(&raiz);
                }
                termina_opcao();
                break;
            case '4': //determina o parentesco entre duas pessoas
                if (raiz == NULL) {
                    printf("\n\tArvore genealogica nao carregada ou arquivo vazio. Por favor, carregue os dados do arquivo primeiro.\n");
                } else {
                    char nome1[50], nome2[50];
                    printf("\n\tDigite o nome da primeira pessoa: ");
                    scanf("%s", nome1);
                    printf("\n\tDigite o nome da segunda pessoa: ");
                    scanf("%s", nome2);
                    determinar_parentesco(raiz, nome1, nome2);
                }
                termina_opcao();
                break;
            case '5': //salva o arquivo e sai
                salvar_arvore(raiz);
                exit(1);
            default:
                printf("\n\tOpcao invalida!\n");
                termina_opcao();
                break;
        }
    }
}
