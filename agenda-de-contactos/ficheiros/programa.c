#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "programa.h"

void PRESSENTER() {
    printf(MAGENTA "\nPressione \"ENTER\" para continuar..." RESET);
    while (getchar() != '\n');
}

void MenuInicial(HashTable H, Historico I) {

    int RESPOSTA;
    Contacto NOVO;
    bool SHUTDOWN = false;
    
    while (!SHUTDOWN) {
        printf(GREEN "\n\n\n==========MENU DE ESCOLHAS==========\n" RESET);
        printf(BLUE "|1| -> " CYAN "Listar Contactos\n" RESET);
        printf(BLUE "|2| -> " CYAN "Criar Contacto\n" RESET);
        printf(BLUE "|3| -> " CYAN "Editar Contacto Existente\n" RESET);
        printf(BLUE "|4| -> " CYAN "Apagar Contacto\n" RESET);
        printf(BLUE "|5| -> " CYAN "Pesquisar Contacto\n" RESET);
        printf(BLUE "|6| -> " CYAN "Histórico de Chamadas\n" RESET);
        printf(BLUE "|7| -> " CYAN "Sair\n" RESET);
        printf(GREEN "\n==========MENU DE ESCOLHAS (ADMIN)==========\n" RESET);
        printf(BLUE "|8| -> " CYAN "Simular Chamada\n\n" RESET);

        printf(BLUE "Digite a sua opção\n" YELLOW "-> ");
        scanf("%i", &RESPOSTA);
        while ((getchar()) != '\n');

        switch(RESPOSTA) {
            case 1:
                ListarContactos(H);
                break;

            case 2:
                CriarContacto(H);
                break;

            case 3:
                EditarContacto(H);
                break;

            case 4:
                ApagarContacto(H);
                break;

            case 5:
                ABPTree T = CreateTree(H);
                PesquisarContacto(H, T);
                T = MakeABPEmpty(T);
                break;

            case 6:
                HistoricoChamadas(I);
                break;

            case 7:
            printf("\n");
                SHUTDOWN = true;
                break;

            case 8:
                AVLTree V = CreateAVL(H);
                SimularChamada(I, V);
                V = MakeAVLEmpty(V);
                break;

            default:
                printf(RED "\nOpção inválida, tente novamente.\n" RESET);
                PRESSENTER();
        }
    }

}

void ListarContactos(HashTable H) {

    printf(GREEN "\n\n\n==========LISTA DE CONTACTOS==========\n" RESET);
    PrintTable(H);

    PRESSENTER();
}

void CriarContacto(HashTable H) {

    printf(GREEN "\n\n\n==========CRIAR CONTACTO==========\n" RESET);

    Contacto NOVO;

    printf(BLUE "\nDigite o nome do contacto\n" YELLOW "-> ");
    fgets(NOVO.nome, MAX_NOME, stdin);
    NOVO.nome[strcspn(NOVO.nome, "\n")] = '\0';

    printf(BLUE "\nDigite o número do contacto\n" YELLOW "-> ");
    fgets(NOVO.numero, MAX_NUM, stdin);
    NOVO.numero[strcspn(NOVO.numero, "\n")] = '\0';

    if ((strlen(NOVO.nome) <= 0) || strlen(NOVO.numero) <= 0) {
        printf(RED "\nImpossível criar contacto.\n" RESET);

        PRESSENTER();

        return;
    }

    Insert(NOVO, H);

    printf(MAGENTA "\nContacto criado!\n" RESET); //-> " YELLOW "%s | %s\n" RESET, NOVO.nome, NOVO.numero);

    PRESSENTER();
}

void EditarContacto(HashTable H) {

    printf(GREEN "\n\n\n==========EDITAR CONTACTO==========\n");

    char INPUT[MAX_NOME];

    printf(BLUE "\nDigite o nome do contacto que quer editar\n" YELLOW "-> ");
    fgets(INPUT, MAX_NOME, stdin);
    INPUT[strcspn(INPUT, "\n")] = 0;

    Contacto* c = Find(INPUT, H);

    if (c == NULL) {
        printf(RED "\nContacto não encontrado.\n" RESET);

        PRESSENTER();

        return;
    }

    printf(MAGENTA "\nContacto encontrado -> " YELLOW "%s | %s\n" RESET, c->nome, c->numero);

    char NOVO_NOME[MAX_NOME];
    char NOVO_NUM[MAX_NUM];

    printf(BLUE "\nNovo nome (ou pressione \"ENTER\" para manter)\n" YELLOW "-> ");
    fgets(NOVO_NOME, MAX_NOME, stdin);
    NOVO_NOME[strcspn(NOVO_NOME, "\n")] = 0;

    printf(BLUE "\nNovo número (ou pressione \"ENTER\" para manter)\n" YELLOW "-> ");
    fgets(NOVO_NUM, MAX_NUM, stdin);
    NOVO_NUM[strcspn(NOVO_NUM, "\n")] = 0;

    if (strlen(NOVO_NOME) > 0)
        strcpy(c->nome, NOVO_NOME);

    if (strlen(NOVO_NUM) > 0)
        strcpy(c->numero, NOVO_NUM);

    printf(MAGENTA "\nContacto atualizado -> " YELLOW "%s | %s\n", c->nome, c->numero);

    PRESSENTER();
}

void ApagarContacto(HashTable H) {

    printf(GREEN "\n\n\n==========APAGAR CONTACTO==========\n" RESET);

    char INPUT[MAX_NOME];

    printf(BLUE "\nDigite o nome do contacto que quer apagar\n" YELLOW "-> ");
    fgets(INPUT, MAX_NOME, stdin);
    INPUT[strcspn(INPUT, "\n")] = 0;

    Contacto* c = Find(INPUT, H);

    if (c == NULL) {
        printf(RED "\nContacto não encontrado.\n" RESET);

        PRESSENTER();
        return;
    }

    printf(MAGENTA "\nContacto encontrado -> " YELLOW "%s | %s\n" RESET, c->nome, c->numero);

    Delete(c->nome, H);

    printf(MAGENTA"\nContacto apagado -> " YELLOW "%s | %s\n", INPUT, c->numero);

    PRESSENTER();
}

void PesquisarContacto(HashTable H, ABPTree T) {

    printf(GREEN "\n\n\n==========PROCURAR CONTACTO==========\n" RESET);
    
    char INPUT[MAX_NOME];

    printf(BLUE "\nDigite o nome do contacto que quer procurar\n" YELLOW "-> ");
    fgets(INPUT, MAX_NOME, stdin);
    INPUT[strcspn(INPUT, "\n")] = 0;

    printf(GREEN "\n\n\n==========CONTACTOS ENCONTRADOS==========\n\n");

    FindABP(INPUT, T);

    PRESSENTER();
}

void HistoricoChamadas(Historico I) {
    printf(GREEN "\n\n\n==========HISTÓRICO DE CHAMADAS==========\n\n" RESET);
    NavegarHistorico(I);

    PRESSENTER();
}

void SimularChamada(Historico I, AVLTree V) {

    ElementType2 X;
    char NUMERO[MAX_NUM];
    const char* ID;

    printf(GREEN "\n\n\n==========SIMULAR CHAMADA==========\n\n" RESET);

    printf(BLUE "Digite o número do chamador " CYAN "(ou escreva \"OCULTO\" para chamada anónima)\n" YELLOW "-> ");
    fgets(NUMERO, MAX_NUM, stdin);
    NUMERO[strcspn(NUMERO, "\n")] = 0;

    printf(BLUE "\nDigite o nome do destinatário\n" YELLOW "-> ");
    fgets(X.destinatario, MAX_NOME, stdin);
    X.destinatario[strcspn(X.destinatario, "\n")] = 0;

    ID = IdentifyCaller(NUMERO, V);
    strncpy(X.chamador, ID, MAX_NOME - 1);
    X.chamador[MAX_NOME - 1] = '\0';

    printf(BLUE"\nDigite o número do destinatário\n" YELLOW "-> ");
    fgets(X.numero, MAX_NUM, stdin);
    X.numero[strcspn(X.numero, "\n")] = 0;

    printf(BLUE "\nDigite a duração da chamada " CYAN "(em segundos)\n" YELLOW "-> ");
    scanf("%d", &X.duracao);
    while (getchar() != '\n');

    InsertList(I, X);

    printf(MAGENTA"\nChamada registrada com sucesso!\n" RESET);

    PRESSENTER();

}