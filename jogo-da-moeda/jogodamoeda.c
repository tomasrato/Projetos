#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {                                                /*estrutura para definir as moedas por fila*/
    int moedas;                                                 /*poderia ter sido usado um ponteiro*/
} Fila;

void EliminarJogoGuardado(int totalmoedas, int carregarguardado) {              /*função que elimina o jogoguardado caso alguém vença e o jogo*/
    if ((totalmoedas == 0 || totalmoedas == 1) && carregarguardado == 1) {      /*a ser jogado for um jogo guardado*/
        remove("jogoguardado.txt");
    }
}

int TotalMoedas (Fila nFila[], int numerofilas) {               /*função que calcula e retorna o total de moedas*/
    int totalmoedas = 0;                                        /*adiciona totas as moedas a variavel "totalmoedas"*/

    for (int i = 0; i < numerofilas; i++) {
        totalmoedas += nFila[i].moedas;
    }

    return totalmoedas;
}

void EstadoJogo (Fila nFila[], int numerofilas) {               /*função que mostra o estado do jogo*/
    printf("Estado do jogo:\n");                                /*imprime que a fila i+1 tem nFila[i] moedas*/
    for (int i = 0; i < numerofilas; i++) {                     /*e repete para todas as filas existentes no tabuleiro*/
        printf("Fila %i: %i moedas\n", i+1, nFila[i].moedas);
    }
}

int DefinirFilas () {                                           /*função que define e retorna para o main o número de filas no caso de criar novo jogo*/
    double numerofilas;                                         /*apenas aceita numero inteiros positivos e impede bugs usando o while da linha 35*/

    do {
        printf("Quantas filas deseja no seu jogo?\n");
        if (scanf("%le", &numerofilas) != 1 || numerofilas <=0 || numerofilas - (int)numerofilas != 0) {
            printf("Digite um número inteiro positivo!\n");
            while (getchar() != '\n');
        }
        else {
            break;
        }
    } while (1);

    return numerofilas;
}

Fila* DefinirMoedas (int numerofilas) {                         /*função que define e retorna para o main as moedas por cada fila definida na estrutura Fila*/
    Fila *nFila;                                                /*começa por alocar memória suficiente para cada fila e por fim utiliza o mesmo*/
    double numeromoedas;                                        /*método da função anterior, apenas lê números inteiros positivos*/
                                                                /*usando o while igual ao anteriormente mencionado*/
    nFila = (Fila *)malloc(sizeof(Fila)*numerofilas);

    for (int i = 0; i < numerofilas; i++) {        
        do {
            printf("Quantas moedas tem a fila %i?\n", i+1);
            if (scanf("%le", &numeromoedas) != 1 || numeromoedas <= 0 || numeromoedas - (int)numeromoedas != 0) {
                printf("Digite um número inteiro positivo!\n");
                while (getchar() != '\n');
            }
            else {
                nFila[i].moedas = numeromoedas;
                break;
            }
        } while (1);
    }

    return nFila;
}

void humanoxbasico(Fila nFila[], int carregarguardado) {                    /*função do modo de jogo humano vs maquina nivel basico*/
    FILE *guardarficheiro;                                                  /*utiliza duas entradas que são a estrutura Fila e a variavel "carregarguardado"*/
    double fila, quantidademoedas;                                          /*esta última indicando se deve ou não carregar o jogo guardado*/
    int jogada = 1, totalmoedas, maquinafila, maquinamoeda, numerofilas;
    srand(time(NULL));

    if (carregarguardado == 0) {                                                            
        numerofilas = DefinirFilas();                                                       /*utiliza as funções anteriormente mencionadas para*/
        nFila = DefinirMoedas(numerofilas);                                                 /*definir o tabuleiro e começar o jogo*/
        printf("Jogo Iniciou!\nPara guardar o jogo, no estado atual, inserir 0 0.\n");
    } else {
        guardarficheiro = fopen("jogoguardado.txt", "r");                                   /*carrega o jogo carregado no ficheiro "jogoguardado.txt"*/
        fseek(guardarficheiro, 2, SEEK_SET);                                                /*este comando serve para o cursor ir para a segunda linha*/
        fscanf(guardarficheiro, "\n%i,%i,", &jogada, &numerofilas);                         /*pois apenas nessa linha as informações interessam*/
        nFila = (Fila *)malloc(sizeof(Fila)*numerofilas);                                   /*como o jogo é novo é necessário alocar a memória*/
        for (int i = 0; i < numerofilas; i++) {
            fscanf(guardarficheiro, "%i,", &nFila[i].moedas);
        }
        fclose(guardarficheiro);
        printf("Continuar Jogo!\n");
    }                                          

    while (jogada == 1 || jogada == 2) {
        
        EstadoJogo(nFila, numerofilas);                                                     /*mostra o tabuleiro do jogo*/

        if (jogada == 1) {

            printf("Jogador %i - Escolha a fila de moedas e o número de moedas a retirar.\n", jogada);  

            if (scanf("%le %le", &fila, &quantidademoedas) == 2 && fila - (int)fila == 0 && quantidademoedas - (int)quantidademoedas == 0) {
                if (quantidademoedas <= nFila[(int)fila - 1].moedas && quantidademoedas > 0 && fila > 0 && fila <= numerofilas) {
                    nFila[(int)fila - 1].moedas -= quantidademoedas;
                    
                    jogada = 2;                                                             /*muda a jogada para a máquina*/
                                                
                }
                else if (quantidademoedas == 0 && fila == 0) {                              /*caso a jogada seja "0 0" guarda o ficheiro*/
                    if ((guardarficheiro = fopen("jogoguardado.txt", "r")) != NULL) {       /*se ja existir o ficheiro "jogoguardado.txt" elimina-o*/
                        remove("jogoguardado.txt");
                    }

                    guardarficheiro = fopen("jogoguardado.txt", "w");                       
                    fprintf(guardarficheiro, "2\n");                                        /*começa por guardar o modo de jogo*/
                    fprintf(guardarficheiro, "%i,%i,", jogada, numerofilas);                /*de seguida guarda as informações necessárias pela ordem:*/
                    for (int i = 0; i < numerofilas; i++) {                                 /*jogada, numero de filas, moedas da fila 1, moedas da fia 2...*/
                        fprintf(guardarficheiro, "%i,", nFila[i].moedas);
                    }
                    fclose(guardarficheiro);

                    printf("O jogo foi guardado.\n");
                
                    jogada = 0;                                                             /*encerra o loop terminando o jogo e voltando para o menu*/
                }
                else {
                    printf("Digite valores possíveis para este jogo!\n");                
                }
            }
            else {
                printf("Digite valores possíveis para este jogo!\n");                
                while (getchar() != '\n');
            } 

        }
        else if (jogada == 2) {

            do {
                maquinafila = (rand() %numerofilas + 1);                                        /*calcula uma jogada aleatória válida, de 1 até o numerodefilas*/
                if (nFila[maquinafila - 1].moedas != 0) {            
                    maquinamoeda = (rand() %nFila[maquinafila - 1].moedas + 1); 

                    printf("A máquina escolheu a fila %i e retirou %i moeda(s).\n", maquinafila, maquinamoeda);

                    nFila[maquinafila - 1].moedas -= maquinamoeda; 
 

                    jogada = 1;

                    break;
                }
            } while (1);
            
        }

        totalmoedas = TotalMoedas(nFila, numerofilas);

        if (totalmoedas == 0) {                             /*verifica se houve algum vencedor na última jogada*/
            EstadoJogo(nFila, numerofilas);                                                         
            switch (jogada) {
                case 1:
                    printf("Venceu!\n");
                    break;
                case 2:
                    printf("Perdeu!\n");
                    break;
            }
            jogada = 0;
                 
        }                                                   
        else if (totalmoedas == 1) {
            EstadoJogo(nFila, numerofilas);
            switch (jogada) {
                case 1:
                    printf("Perdeu!\n");
                    break;
                case 2:
                    printf("Venceu!\n");
                    break;
            }
            jogada = 0;         
        }
    }

    EliminarJogoGuardado(totalmoedas, carregarguardado);

    free(nFila);            /*liberta a memória alocada para evitar memoryleaks*/
}

void humanoxavancado(Fila nFila[], int carregarguardado) {
    FILE *guardarficheiro; 
    double fila, quantidademoedas;
    int jogada = 1, totalmoedas, numerofilas, filasrestantes, avaliarfila, maiorfila, jogadafeita = 0, menorfila;

    if (carregarguardado == 0) {
        numerofilas = DefinirFilas();
        nFila = DefinirMoedas(numerofilas);
        printf("Jogo Iniciou!\nPara guardar o jogo, no estado atual, inserir 0 0.\n");
    } else {
        guardarficheiro = fopen("jogoguardado.txt", "r");
        fseek(guardarficheiro, 2, SEEK_SET);
        fscanf(guardarficheiro, "\n%i,%i,", &jogada, &numerofilas);
        nFila = (Fila *)malloc(sizeof(Fila)*numerofilas);
        for (int i = 0; i < numerofilas; i++) {
            fscanf(guardarficheiro, "%i,", &nFila[i].moedas);
        }
        fclose(guardarficheiro);
        printf("Continuar Jogo!\n");
    }

    while (jogada == 1 || jogada == 2) {
        
        EstadoJogo(nFila, numerofilas);

        if (jogada == 1) {

            printf("Jogador %i - Escolha a fila de moedas e o número de moedas a retirar.\n", jogada);

            if (scanf("%le %le", &fila, &quantidademoedas) == 2 && fila - (int)fila == 0 && quantidademoedas - (int)quantidademoedas == 0) {
                if (quantidademoedas <= nFila[(int)fila - 1].moedas && quantidademoedas > 0 && fila > 0 && fila <= numerofilas) {
                    nFila[(int)fila - 1].moedas -=quantidademoedas;
                    
                    jogada = 2;
                    
                }
                else if (quantidademoedas == 0 && fila == 0) {
                    if ((guardarficheiro = fopen("jogoguardado.txt", "r")) != NULL) {
                        remove("jogoguardado.txt");
                    }

                    guardarficheiro = fopen("jogoguardado.txt", "w");
                    fprintf(guardarficheiro, "3\n");
                    fprintf(guardarficheiro, "%i,%i,", jogada, numerofilas);
                    for (int i = 0; i < numerofilas; i++) {
                        fprintf(guardarficheiro, "%i,", nFila[i].moedas);
                    }
                    fclose(guardarficheiro);

                    printf("O jogo foi guardado.\n");
                
                    jogada = 0;
                }
                else {
                    printf("Digite valores possíveis para este jogo!\n");                
                }
            }
            else {
                printf("Digite valores possíveis para este jogo!\n");                
                while (getchar() != '\n');
            } 

        }
        else if (jogada == 2) {                         /*código da máquina avançada explicada no relatório*/

            filasrestantes = 0;
            jogadafeita = 0;
           
            for (int i = 0; i < numerofilas; i++) {
                if (nFila[i].moedas != 0) {
                    filasrestantes++;
                }
            }

            if (filasrestantes % 2 == 0 && filasrestantes != 2) {           /*se o numero restante de filas restantes for par e*/
                maiorfila = 0;                                              /*diferente de 2, a maquina retira a maior fila*/
                for (int i = 0; i < numerofilas; i++) {
                    if (nFila[i].moedas > nFila[maiorfila].moedas) {
                        maiorfila = i;
                    }
                }

                printf("A máquina escolheu a fila %i e retirou %i moeda(s).\n", maiorfila + 1, nFila[maiorfila].moedas);

                nFila[maiorfila].moedas = 0;

            }
            else if (filasrestantes == 2) {
                avaliarfila = 0;
                for (int i = 0; i < numerofilas; i++) {                 /*se for o numero de filas restantes for igual a 2*/
                    if (nFila[i].moedas > 2) {                          /*a maquina vê quantas filas têm um númeor de moedas*/
                        avaliarfila++;                                  /*maior que 2 e:*/
                    }
                }
                if (avaliarfila == 2) {
                    for (int i = 0; i < numerofilas && jogadafeita == 0; i++) {         /*se forem duas filas, retira, da maior fila o número*/
                        if (nFila[i].moedas > 0) {                                      /*necessário de moedas até sobrarem duas moedas*/
                            jogadafeita = 1;

                            printf("A máquina escolheu a fila %i e retirou %i moeda(s).\n", i+1, nFila[i].moedas - 2);

                            nFila[i].moedas = 2;
                        }
                    }
                }
                else if (avaliarfila == 1) {                                            /*se for uma fila e a outra tiver menos que duas moedas*/
                    maiorfila = 0;                                                      /*coloca a maior fila com zero modas, caso a outra fila seja igual a dois*/
                    for (int i = 0; i < numerofilas; i++) {                             /*retira uma moeda da maior fila*/
                        if (nFila[i].moedas > nFila[maiorfila].moedas) {
                            maiorfila = i;
                        }
                    }

                    menorfila = maiorfila;

                    for (int i = 0; i < numerofilas; i++) {
                        if (nFila[i].moedas < nFila[menorfila].moedas && nFila[i].moedas != 0) {
                            menorfila = i;
                        }
                    }

                    if (nFila[menorfila].moedas < 2) {
                        printf("A máquina escolheu a fila %i e retirou %i moeda(s).\n", maiorfila + 1, nFila[maiorfila].moedas);       
                        nFila[maiorfila].moedas = 0;
                    }
                    else {
                        nFila[maiorfila].moedas--;
                        printf("A máquina escolheu a fila %i e retirou 1 moeda(s).\n", maiorfila + 1);
                    }

                    
                }
                else {
                    maiorfila = 0;
                    for (int i = 0; i < numerofilas; i++) {                             /*se nao houverem filas com mais de duas moedas*/
                        if (nFila[i].moedas > nFila[maiorfila].moedas) {                /*coloca a maior fila com zero moedas*/
                            maiorfila = i;
                        }
                    }

                    printf("A máquina escolheu a fila %i e retirou %i moeda(s).\n", maiorfila+1, nFila[maiorfila].moedas);
                    nFila[maiorfila].moedas = 0;
          
                }
            }
            else if (filasrestantes % 2 != 0) {                                                             /*se o numero de filas restantes for ímpar:*/
                if (filasrestantes != 1) {                                                                  /*se esse número for diferente de 1, retira 1 moeda*/
                    for (int i = 0; i < numerofilas && jogadafeita == 0; i++) {                             /*à primeira fila com moedas*/
                        if (nFila[i].moedas > 0) {
                            nFila[i].moedas--;
                            jogadafeita = 1;
                            printf("A máquina escolheu a fila %i e retirou 1 moeda(s).\n", i+1);
                        }
                    }
                }
                else {                                                                                      
                    for (int i = 0; i < numerofilas && jogadafeita == 0; i++) {                             /*se esse número for 1, coloca essa fila com apenas 1 moeda*/
                        if (nFila[i].moedas > 0) {
                            jogadafeita = 1;
                            printf("A máquina escolheu a fila %i e retirou %i moeda(s).\n", i+1, nFila[i].moedas - 1);
                            nFila[i].moedas = 1;
                        }
                    }
                }
            }
            
            jogada = 1;
            
        }

        totalmoedas = TotalMoedas(nFila, numerofilas);

        if (totalmoedas == 0) {   
            EstadoJogo(nFila, numerofilas);                                                          
            switch (jogada) {
                case 1:
                    printf("Venceu!\n");
                    break;
                case 2:
                    printf("Perdeu!\n");
                    break;
            }
            jogada = 0;                     
        }                                                   
        else if (totalmoedas == 1) {
            EstadoJogo(nFila, numerofilas);
            switch (jogada) {
                case 1:
                    printf("Perdeu!\n");
                    break;
                case 2:
                    printf("Venceu!\n");
                    break;
            }
            jogada = 0;
        }
    }

    EliminarJogoGuardado(totalmoedas, carregarguardado);

    free(nFila);
}

void humanoxhumano(Fila nFila[], int carregarguardado) {
    FILE *guardarficheiro;
    double fila, quantidademoedas;
    int totalmoedas, jogada = 1, numerofilas;

    
    if (carregarguardado == 0) {
        numerofilas = DefinirFilas();
        nFila = DefinirMoedas(numerofilas);
        printf("Jogo Iniciou!\nPara guardar o jogo, no estado atual, inserir 0 0.\n");
    } else {
        guardarficheiro = fopen("jogoguardado.txt", "r");
        fseek(guardarficheiro, 2, SEEK_SET);
        fscanf(guardarficheiro, "\n%i,%i,", &jogada, &numerofilas);
        nFila = (Fila *)malloc(sizeof(Fila)*numerofilas);
        for (int i = 0; i < numerofilas; i++) {
            fscanf(guardarficheiro, "%i,", &nFila[i].moedas);
        }
        fclose(guardarficheiro);
        printf("Continuar Jogo!\n");
    }

    while (jogada == 1 || jogada == 2) {
            
        EstadoJogo(nFila, numerofilas);
        
        printf("Jogador %i - Escolha a fila de moedas e o número de moedas a retirar.\n", jogada);

        if (scanf("%le %le", &fila, &quantidademoedas) == 2 && fila - (int)fila == 0 && quantidademoedas - (int)quantidademoedas == 0) {
            if (quantidademoedas <= nFila[(int)fila - 1].moedas && quantidademoedas > 0 && fila > 0 && fila <= numerofilas) {
                nFila[(int)fila - 1].moedas -= quantidademoedas;
                if (jogada == 1) {                                      /*alterna a jogada entre o jogador 1 e 2*/
                    jogada = 2;
                }
                else if (jogada == 2) {
                    jogada = 1;
                }

            }
            else if (quantidademoedas == 0 && fila == 0) {
                if ((guardarficheiro = fopen("jogoguardado.txt", "r")) != NULL) {
                    remove("jogoguardado.txt");
                }

                guardarficheiro = fopen("jogoguardado.txt", "w");
                fprintf(guardarficheiro, "1\n");
                fprintf(guardarficheiro, "%i,%i,", jogada, numerofilas);
                for (int i = 0; i < numerofilas; i++) {
                    fprintf(guardarficheiro, "%i,", nFila[i].moedas);
                }
                fclose(guardarficheiro);

                printf("O jogo foi guardado.\n");
                
                jogada = 0;
            }
            else {
                printf("Digite valores possíveis para este jogo!\n");                
            }
        }
        else {
            printf("Digite valores possíveis para este jogo!\n");                
            while (getchar() != '\n');
        }

        totalmoedas = TotalMoedas(nFila, numerofilas); 

        if (totalmoedas == 0) { 
            EstadoJogo(nFila, numerofilas); 
            printf("Jogador %i Venceu!\n", jogada);
            jogada = 0;
        }
        else if (totalmoedas == 1) {
            EstadoJogo(nFila, numerofilas); 
            switch (jogada) {
                case 1:
                    printf("Jogador 2 Venceu!\n");
                    break;
                case 2:  
                    printf("Jogador 1 Venceu!\n");
                    break;
            }
            jogada = 0;
        }

    }

    EliminarJogoGuardado(totalmoedas, carregarguardado);

    free(nFila);

}


int main() {
    Fila *nFila;
    FILE *guardarficheiro; 
    double menuescolha;
    int sistemaligado = 1, escolhaligada, mododejogo;

    do {
        printf("Bem-vindo ao Jogo da Moeda!\n");                        /*Menu inicial interativo*/
        printf("Por favor digite a opção que pretende:\n");
        printf("    1 - Iniciar Novo Jogo\n");
        printf("    2 - Continuar Jogo Guardado\n");
        printf("    3 - Sair\n");

        if (scanf("%le", &menuescolha) != 1 || menuescolha <=0 || menuescolha > 3 || menuescolha - (int)menuescolha != 0) {
            printf("Número não encontrado no menu!\n");
            while (getchar() != '\n');
        }
        else {
            switch ((int)menuescolha) {
                case 1:
                    do {
                        escolhaligada = 1;

                        printf("Por favor digite o modo que pretende:\n");          /*Menu interativo dos modos de jogo*/
                        printf("    1 - Humano X Humano\n");
                        printf("    2 - Humano X Máquina Básica\n");
                        printf("    3 - Humano X Máquina Avançada\n");
                        printf("    4 - Voltar ao Menu anterior\n");

                        if (scanf("%le", &menuescolha) != 1 || menuescolha <=0 || menuescolha > 4 || menuescolha - (int)menuescolha != 0) {
                            printf("Número não encontrado no menu!\n");
                            while (getchar() != '\n');
                        }
                        else {
                            switch ((int)menuescolha) {                             /*escolhe um modo de jogo*/
                                case 1:
                                    humanoxhumano(nFila, 0);
                                    break;
                                case 2:
                                    humanoxbasico(nFila, 0);
                                    break;
                                case 3:
                                    humanoxavancado(nFila, 0);
                                    break;
                                case 4:
                                    break;
                            }
                            escolhaligada = 0;
                        }
                    } while (escolhaligada == 1);
                    
                    break;

                case 2:  
                    if ((guardarficheiro = fopen("jogoguardado.txt", "r")) == NULL) {
                        printf("Não existe nenhum jogo guardado!\n");       /*indica que não existe nenhum jogo guardado*/
                    } else {
                        fscanf(guardarficheiro, "%i\n", &mododejogo);
                        fclose(guardarficheiro);
                        switch (mododejogo) {
                            case 1:                                         /*escolhe um modo de jogo indicando que é para carregar o jogo guardado*/
                                humanoxhumano(nFila, 1);
                                break;
                            case 2:
                                humanoxbasico(nFila, 1);
                                break;
                            case 3:
                                humanoxavancado(nFila, 1);
                                break;
                        }
                    }

                    break;

                case 3:                                                     /*desliga/fecha o jogo*/
                    sistemaligado = 0;
                    break;

            }
        }

    } while (sistemaligado == 1);

}