#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>

/* Funcoes e variaveis globais *///
void menuPrincipal(void);
void menuLimpeza(void);
void menuAlimento(void); // Declaracoes das Funcoes
void menuPadaria(void);
void menuPagamento(void);
void menuAbertura(void);
void menuFechamento(void);
void menuCancelar(void);
void menufPagamento(void);
void abrePadaria(void);
void login(void);
bool caixaAberto = false;

float vLimpeza = 0, vPadaria = 0, vAlimento = 0, totalDia =  0; // Variaveis globais
float fPagar = 0;
float vCar = 0;
float trocoInicial = 0;
/* <<<<<<< VALORES DOS PRODUTOS >>>>>>> */
float precosLimpeza[7] = {1.99, 8.99, 1.50, 15.00, 4.99, 7.99, 1.00};
float precosPadaria[7] = {9.50, 12.50, 1.90, 8.50, 12.50, 2.50, 17.50};
float precosAlimentos[7] = {19.99, 5.90, 4.50, 8.00, 5.00, 2.00, 5.00};
/* <<<<<<< ESTOQUE DOS PRODUTOS >>>>>>> */
int estoqueLimpeza[7] = {50, 30, 0, 20, 100, 15, 60};
int estoqueAlimentos[7] = {10, 15, 10, 5, 20, 6, 15};
int estoquePadaria[7] = {};

void delay(float delayemSegundos){    // Funcao utilizando a biblioteca "<time.h>" para delay.
    float mili = 1000 * delayemSegundos;
    clock_t comecoT = clock();
    while (clock() < comecoT + mili);
}

void login(){
    char usuario[50];
    char senha[50];
    system("cls");
    printf("Login: ");
    fgets(usuario, sizeof(usuario), stdin);
    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);

    usuario[strcspn(usuario, "\n")] = '\0';
    senha[strcspn(senha, "\n")] = '\0';

    if (strcmp(usuario, "vendedor1") == 0 && strcmp(senha, "101010") == 0) {
        printf("\nLogin realizado com sucesso!!!");
        delay(2);
        menuPrincipal();
    } else {
        printf("\nLogin e/ou senha invalidos!!!");
        delay(2);
        login();
    }
}
void esperarEnter() {
    printf("\nPressione Enter para voltar ao menu...");
    while (getchar() != '\n') {
        getchar(); // Espera o Enter
    }
}

int pegaTamanho(char* array[]) {
    int tamanho = 0;
    while (array[tamanho] != NULL) {
        tamanho++;
    }
    return tamanho;
}

void printMenu(int const escolhaMenu) {
    int i = 0;
    system("cls");
    if(escolhaMenu == 0) { // 0 == Menu principal.
        const char* printMenuPrincipal[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t ------------ Menu Principal ------------\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            "| -> Cod: 1  - Menu de Materiais de Limpeza \t\t|\n",
            "| -> Cod: 2  - Menu de Alimentos \t\t\t|\n",
            "| -> Cod: 3  - Menu da Padaria \t\t\t\t|\n",
            "|.......................................................|\n",
            "| -> Cod: 4  - Menu de Pagamento \t\t\t|\n",
            "| -> Cod: 5  - Abertura de Caixa \t\t\t|\n",
            "| -> Cod: 6  - Fechamento de Caixa \t\t\t|\n",
            "| -> Cod: 7  - Cancelar Venda \t\t\t\t|\n",
            "| -> Cod: 8  - Sair do Programa \t\t\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL // Finaliza o Array de Strings
            };

        for(i = 0 ; i < pegaTamanho(printMenuPrincipal) ; i++) {
            printf("%s", printMenuPrincipal[i]);
        }

        printf("|      $ - Valor total de vendas hoje: R$ %.2f - $\t|\n",totalDia); // Atualizacao do valor total das vendas
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    if (escolhaMenu == 1) { // 1 == Menu Limpeza.
        const char* nomesProdutosLimpeza[] = {
            "Detergente",
            "Sabao em Po 1KG",
            "Esponja",
            "Amaciante 1LT",
            "Bucha de Pia (KIT C/3)",
            "Desinfetante 1LT",
            "Sabao em Barra (UND)",
            NULL
        };

        const char* printMenuLimpeza[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t------------ Menu de Limpeza ------------\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL // Finaliza o Array de Strings
        };

        for (i = 0; i < pegaTamanho(printMenuLimpeza); i++) {
            printf("%s", printMenuLimpeza[i]);
        }

        for (i = 0; i < pegaTamanho(nomesProdutosLimpeza); i++) {
            printf("| -> Cod: %d - %-23s R$ %.2f  (%d)\t|\n",
                11 + i, nomesProdutosLimpeza[i], precosLimpeza[i], estoqueLimpeza[i]);
        }

        printf("|.......................................................|\n");
        printf("| -> Cod: 18 - Voltar ao Menu Principal \t\t|\n");
        printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
        printf("|    $ - Carrinho materiais de limpeza:  R$ %.2f - $\t|\n", vLimpeza);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    if (escolhaMenu == 2) { // 2 == Menu Alimentos.
        const char* nomesProdutosAlimento[] = {
            "Cafe",
            "Leite Caixa",
            "Arroz 1KG",
            "Feijao Preto 1KG",
            "Acucar 1KG",
            "Sal 1KG",
            "Farinha de Trigo 1KG",
            NULL
        };

        const char* printMenuAlimentos[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|      ------------ Menu de Alimentos ------------\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL // Finaliza o Array de Strings
        };

        for(i = 0; i < pegaTamanho(printMenuAlimentos); i++) {
            printf("%s", printMenuAlimentos[i]);
        }

        for (i = 0; i < pegaTamanho(nomesProdutosAlimento); i++) {
            printf("| -> Cod: %d - %-23s R$ %.2f  (%d)\t|\n",
                21 + i, nomesProdutosAlimento[i], precosAlimentos[i], estoqueAlimentos[i]);
        }

        printf("|.......................................................|\n");
        printf("| -> Cod: 28 - Voltar ao Menu Principal \t\t|\n");
        printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
        printf("|\t $ - Carrinho de Alimentos:  R$ %.2f - $\t|\n", vAlimento);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");

    }

    if (escolhaMenu == 3) { // 3 == Menu Padaria.
        const char* nomesProdutosPadaria[]= {
            "Pao de Forma PCT",
            "Pao Integral PCT",
            "Pao Frances UND",
            "Sonho UND",
            "Biscoito KG",
            "Pao Doce UND",
            "Salgado",
            NULL
        };

        const char* printMenuPadaria[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|      ------------ Menu da Padaria ------------\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL    // Finaliza o Array de Strings
        };

        for(i = 0; i < pegaTamanho(printMenuPadaria); i++) {
            printf("%s", printMenuPadaria[i]);
        }

        for(i = 0; i < pegaTamanho(nomesProdutosPadaria); i++) {
            printf("| -> Cod: %d - %-23s R$ %.2f  (%d)\t|\n",
                31 + i, nomesProdutosPadaria[i], precosPadaria[i], estoquePadaria[i]);
        }

        printf("|.......................................................|\n");
        printf("| -> Cod: 38 - Voltar ao Menu Principal \t\t|\n");
        printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
        printf("|\t  $ - Carrinho da Padaria:  R$ %.2f - $\t\t|\n", vPadaria);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }
}

void menuPrincipal(){ // Funcao do Menu Principal

    int opcaoUm = 0; // Variavel opcao do usuario

    while (opcaoUm!=8)
    {
        printMenu(0); // 0 == Menu Principal

        scanf("%d", &opcaoUm);

        if (opcaoUm == 5) {
          menuAbertura();
        }
        else if (caixaAberto == false && opcaoUm != 8)  { //  === VERIFICAÇÃO CAIXA ABERTO CASO ESTIVER FECHADO SOLICITA ABERTURA
            system("cls");
                printf("O caixa esta fechado, realize a abertura");
                delay(0.5);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(0.5);
                }
            menuPrincipal();

        } else {
            switch(opcaoUm) { // Switch Case para as sessoes do mercadinho
                case 1:
                    menuLimpeza();
                    break;
                case 2:
                    menuAlimento();
                    break;
                case 3:
                    menuPadaria();
                    break;
                case 4:
                    menuPagamento();
                    break;
                case 5:
                    menuAbertura();
                    break;
                case 6:
                    menuFechamento();
                    break;
                case 7:
                    menuCancelar();
                    break;
                case 8:
                    if (caixaAberto == true) {
                        printf("Ha um caixa aberto, por favor, realize o fechamento antes de sair do sistema...");
                        delay(3);
                        menuPrincipal();
                    } else {
                        printf("Encerrando o sistema...");
                        delay(3);
                        exit (0);
                    }
                default:
                    printf("Opcao invalida... Tente novamente\n");
                    menuPrincipal();
            }
        }
    }
}

void menuLimpeza(){ // Funcao Menu Limpeza

    int cLimpeza; // Opcao do usuario
    float qnt;

    printMenu(1);

    scanf("%d", &cLimpeza);

    switch(cLimpeza){ // Switch case para selecao dos produtos
        case 11:    /*<----- DETERGENTE */
            printf("\nDetergente \n");
            if (estoqueLimpeza[0] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueLimpeza[0]) {
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Detergente adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueLimpeza[0] = estoqueLimpeza[0] - qnt;
            vLimpeza += precosLimpeza[0] * qnt;
            vCar += precosLimpeza[0] * qnt;
            menuLimpeza();
            break;

        case 12:    /*<----- SABAO EM PO */
            printf("\nSabao em po \n");
            if (estoqueLimpeza[1] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueLimpeza[1]) {
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Sabao em po adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueLimpeza[1] = estoqueLimpeza[1] - qnt;
            vLimpeza += precosLimpeza[1] * qnt;
            vCar += precosLimpeza[1] * qnt;
            menuLimpeza();
            break;

        case 13:    /*<----- ESPONJA*/
            printf("\nEsponja \n");
            printf("Infelizmente o item nao registra estoque, digite um novo item\n");
            delay(3);
            menuLimpeza();
            break;

        case 14:    /*<----- AMACIANTE*/
            printf("\nAmaciante \n");
            if (estoqueLimpeza[3] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueLimpeza[3]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Amaciante adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueLimpeza[3] = estoqueLimpeza[3] - qnt;
            vLimpeza += precosLimpeza[3] * qnt;
            vCar += precosLimpeza[3] * qnt;
            menuLimpeza();
            break;

        case 15:    /*<----- BUCHA DE PIA*/
            printf("Bucha de Pia (Kit c/3) \n");
            if (estoqueLimpeza[4] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueLimpeza[4]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Bucha de Pia adicionada ao carrinho.\n",qnt);
            delay(3);
            estoqueLimpeza[4] = estoqueLimpeza[4] - qnt;
            vLimpeza += precosLimpeza[4] * qnt;
            vCar += precosLimpeza[4] * qnt;
            menuLimpeza();
            break;

        case 16:    /*<----- DESINFETANTE*/
            printf("\n");
            printf("Desinfetante (1LT) \n");
            if (estoqueLimpeza[5]== 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueLimpeza[5]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Desinfetante adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueLimpeza[5] = estoqueLimpeza[5] - qnt;
            vLimpeza += precosLimpeza[5] * qnt;
            vCar += precosLimpeza[5] * qnt;
            menuLimpeza();
            break;

        case 17:    /*<----- SABAO EM BARRA*/
            printf("\n");
            printf("Sabao em Barra (UN) \n");
            if (estoqueLimpeza[6] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueLimpeza[6]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sabao em Barra adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueLimpeza[6] = estoqueLimpeza[6] - qnt;
            vLimpeza += precosLimpeza[6] * qnt;
            vCar += precosLimpeza[6] * qnt;
            menuLimpeza();
            break;

        case 18:    /*<----- VOLTAR*/
            printf("\n");
            printf("Voltar ao menu principal\n");
            delay(2);
            break;

        default:
            printf("\n");
            printf("Opcao invalida... Tente Novamente\n");
            delay(2);
            menuLimpeza();
            break;
        }
}

void menuAlimento(){ // Funcao Menu Alimentos

    int cAlimento;
    float qnt;

    printMenu(2); // 2 == Menu de Alimentos

    printf("Selecione uma opcao\n");
    scanf("%d", &cAlimento);

    switch(cAlimento){ // Switch case para selecao dos produtos
        case 21:    /*<----- CAFE */
            printf("\nCafe:\n");
            if (estoqueAlimentos[0] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueAlimentos[0]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Cafe adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueAlimentos[0] = estoqueAlimentos[0] - qnt;
            vAlimento += precosAlimentos[0] * qnt;
            vCar += precosAlimentos[0] * qnt;
            menuAlimento();
            break;

        case 22:    /*<----- LEITE */
            printf("\n");
            printf("\nLeite:\n");
            if (estoqueAlimentos[1] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueAlimentos[1]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Leite adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueAlimentos[1] = estoqueAlimentos[1] - qnt;
            vAlimento += precosAlimentos[1] * qnt;
            vCar += precosAlimentos[1] * qnt;
            menuAlimento();
            break;

        case 23:    /*<----- ARROZ*/
            printf("Arroz 1kg:\n");
            if (estoqueAlimentos[2] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueAlimentos[2]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Arroz 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueAlimentos[2] = estoqueAlimentos[2] - qnt;
            vAlimento += precosAlimentos[2] * qnt;
            vCar += precosAlimentos[2] * qnt;
            menuAlimento();
            break;

        case 24:    /*<----- FEIJAO PRETO*/
            printf("\nFeijao Preto 1kg:\n");
            if (estoqueAlimentos[3] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueAlimentos[3]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Feijao preto 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueAlimentos[3] = estoqueAlimentos[3] - qnt;
            vAlimento += precosAlimentos[3] * qnt;
            vCar += precosAlimentos[3] * qnt;
            menuAlimento();
            break;

        case 25:    /*<----- ACUCAR*/
            printf("\nAcucar 1kg:\n");
            if (estoqueAlimentos[4] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueAlimentos[4]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Acucar 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueAlimentos[4] = estoqueAlimentos[4] - qnt;
            vAlimento += precosAlimentos[4] * qnt;
            vCar += precosAlimentos[4] * qnt;
            menuAlimento();
            break;

        case 26:    /*<----- ACUCAR*/
            printf("\nSal 1kg:\n");
            if (estoqueAlimentos[5] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueAlimentos[5]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sal 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            estoqueAlimentos[5] = estoqueAlimentos[5] - qnt;
            vAlimento += precosAlimentos[5] * qnt;
            vCar += precosAlimentos[5] * qnt;
            menuAlimento();
            break;

        case 27:    /*<----- ACUCAR*/
            printf("\nFarinha de Trigo 1kg:\n");
            if (estoqueAlimentos[6] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoqueAlimentos[6]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Farinha de Trigo 1kg adicionada ao carrinho.\n",qnt);
            delay(3);
            estoqueAlimentos[6] = estoqueAlimentos[6] - qnt;
            vAlimento += precosAlimentos[6] * qnt;
            vCar += precosAlimentos[6] * qnt;
            menuAlimento();
            break;

        case 28:    /*<----- VOLTAR*/
            printf("\nVoltar ao menu principal\n");
            delay(2);
            break;

        default:
            printf("\nOpcao invalida... Tente Novamente\n");
            delay(2);
            menuAlimento();
            break;
        }
}

void menuPadaria(){ // Funcao Menu Padaria

    int cPadaria;
    float qnt;

    printMenu(3); // 3 == Menu Padaria

    scanf("%d", &cPadaria);

    switch(cPadaria){ // Switch case para selecao dos protudos
        case 31:   /*<----- PAO DE FORMA */
            printf("\nPao de Forma(PCT):\n");
            if (estoquePadaria[0] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoquePadaria[0]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao de Forma adicionado ao carrinho.\n",qnt);
            delay(2);
            estoquePadaria[0] = estoquePadaria[0] - qnt;
            vPadaria += precosPadaria[0] * qnt;
            vCar += precosPadaria[0] * qnt;
            menuPadaria();
            break;

        case 32:    /*<----- PAO INTEGRAL */
            printf("\nPao Integral(PCT):\n");
            if (estoquePadaria[1] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoquePadaria[1]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao integral adicionado ao carrinho.\n",qnt);
            delay(2);
            estoquePadaria[1] = estoquePadaria[1] - qnt;
            vPadaria += precosPadaria[1] * qnt;
            vCar += precosPadaria[1] * qnt;
            menuPadaria();
            break;

        case 33:    /*<----- PAO FRANCES*/
            printf("\nPao Frances(UND):\n");
            if (estoquePadaria[2] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoquePadaria[2]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao frances adicionado ao carrinho.\n",qnt);
            delay(2);
            estoquePadaria[2] = estoquePadaria[2] - qnt;
            vPadaria += precosPadaria[2] * qnt;
            vCar += precosPadaria[2] * qnt;
            menuPadaria();
            break;

        case 34:    /*<----- SONHO*/
            printf("\nSonho(UND):\n");
            if (estoquePadaria[3] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoquePadaria[3]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sonho adicionado ao carrinho.\n",qnt);
            delay(2);
            estoquePadaria[3] = estoquePadaria[3] - qnt;
            vPadaria += precosPadaria[3] * qnt;
            vCar += precosPadaria[3] * qnt;
            menuPadaria();
            break;

        case 35:    /*<----- SONHO*/
            printf("\nBiscoito(KG): \n");
            if (estoquePadaria[4] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoquePadaria[4]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Biscoito adicionado ao carrinho.\n",qnt);
            delay(2);
            estoquePadaria[4] = estoquePadaria[4] - qnt;
            vPadaria += precosPadaria[4] * qnt;
            vCar += precosPadaria[4] * qnt;
            menuPadaria();
            break;

        case 36:    /*<----- SONHO*/
            printf("\nPao Doce(UND):\n");
            if (estoquePadaria[5] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoquePadaria[5]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao Doce adicionado ao carrinho.\n",qnt);
            delay(2);
            estoquePadaria[5] = estoquePadaria[5] - qnt;
            vPadaria += precosPadaria[5] * qnt;
            vCar += precosPadaria[5] * qnt;
            menuPadaria();
            break;

        case 37:    /*<----- SONHO*/
            printf("\nSalgado(UND):\n");
            if (estoquePadaria[6] == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > estoquePadaria[6]){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Salgado adicionado ao carrinho.\n",qnt);
            delay(2);
            estoquePadaria[6] = estoquePadaria[6] - qnt;
            vPadaria += precosPadaria[6] * qnt;
            vCar += precosPadaria[6] * qnt;
            menuPadaria();
            break;

        case 38:    /*<----- VOLTAR*/
            printf("\nVoltar ao menu principal\n");
            delay(2);
            break;

        default:
            printf("\nOpcao invalida... Tente Novamente\n");
            delay(2);
            menuPadaria();
            break;
    }
}

void menuPagamento() // Funcao Menu Pagamento
{
    int cPagamento, cCartao;

    /* <<< VARIAVEIS DE ARMAZENAMENTO TEMPORARIO */
    float vTotal = 0, vTcdesconto = 0, troco = 0, valorCobrado = 0, descontoReal =0;

    /* <<< VARIAVEIS DE ARMAZENAMENTO FIXO */
    float descontoCinco, descontoDez, descontoInformado, descontoBruto;

        /* DESCONTOS */
    descontoCinco = 0.05;   /* <<<- 5% desconto */
    descontoDez = 0.10;  /* <<<- 10% desconto */

    vTotal = vLimpeza + vPadaria + vAlimento; // Calculo valor total sem desconto
    system("cls");
    printf("Forma de Pagamento:\n");
    printf("41. Dinheiro\n");
    printf("42. Cartao\n");
    printf("43. Voltar ao menu principal\n");

    scanf("%d", &cPagamento);   /* <--------------- PAGAMENTO EM DINHEIRO */

    switch(cPagamento){ // Switch case para verificar valor com desconto
        case 41:
            printf("\nValor total: %.2f R$\n",vTotal);

            if (vTotal <= 50)
            {
                printf("O desconto de 5 porcento sera aplicado\n");
                descontoReal = vTotal * descontoCinco;
            }

            else if (vTotal > 50 && vTotal < 200)
            {
                printf("O desconto de 10 porcento sera aplicado\n");
                descontoReal = vTotal * descontoDez;
            }

            else if (vTotal >200)
            {
                printf("Informe a porcentagem de desconto:");
                    scanf("%f",&descontoBruto);
                        printf("O desconto de %.0f porcento sera aplicado\n", descontoBruto);
                    descontoInformado = descontoBruto / 100;
                    descontoReal = vTotal * descontoInformado;
            }

            vTcdesconto = vTotal - descontoReal; // Calculo valor total com desconto
            printf("\nValor com desconto: %.2f R$\n", vTcdesconto);
            printf("Inserir valor recebido em dinheiro\n");
            scanf("%f", &valorCobrado);

            if (valorCobrado > vTcdesconto)
            {
                troco= valorCobrado- vTcdesconto;
                totalDia= totalDia + vTcdesconto;  /* <----------- Soma do total diario */
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("\nTroco: %.2f R$\n", troco);
                printf("Pagamento realizado com sucesso!\n");
                delay(2);
                vLimpeza= 0;
                vAlimento= 0;
                vPadaria= 0;
                fPagar= 0;  /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                vCar= 0;
                break;
            }

            else if (valorCobrado == vTcdesconto)
            {
                totalDia = totalDia + vTcdesconto;
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("Pagamento realizado com sucesso!\n");
                delay(2);
                vLimpeza = 0;
                vAlimento = 0;
                vPadaria = 0;
                fPagar = 0;
                vCar = 0;/*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                break;
            } else {
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("O valor integral sera cobrado!\n");
                delay(2);
                system("cls");
                fPagar = vTotal - valorCobrado;
                totalDia= totalDia + valorCobrado;
                printf("Falta pagar %.2f R$", fPagar);
                delay(2);
                menufPagamento();
                break;
            }

        case 42:       /* <--------- PAGAMENTO EM CART�O */
            printf("Valor total: %.2f R$\n\n",vTotal);
            printf("51. Pagamento realizado\n");
            printf("52. Pagamento nao realizado\n");
            printf("53. Voltar ao  menu anterior\n");
            scanf("%d", &cCartao);

            if (cCartao == 51)
            {
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("Pagamento realizado com sucesso!\n");
                delay(2);
                totalDia = totalDia + vTotal; /* <----------- Soma do total di�rio */
                vPadaria = 0; vAlimento= 0; vLimpeza =0; vCar = 0;  /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
            }

            else if (cCartao == 52)
            {
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("\nPagamento nao realizado!\n");
                delay(3);
                menuPagamento();
            }

            else if (cCartao == 53)
            {
                printf("\nVoltar ao menu anterior\n");
                delay(2);
                menuPagamento();
            }

            else
            {
                printf("\nOpcao invalida... Tente novamente\n");
                delay(2);
            }

            break;

        case 43:
            printf("\nVoltar ao menu principal\n");
            menuPrincipal();
            break;

        default:
            printf("\nOpcao invalida... Tente Novamente\n");
            delay(2);
            menuPagamento();
            break;
    }
}

void menufPagamento() //FUNCAO CRIADA PARA CASO A PESSOA NAO DE DINHEIRO SUFICIENTE NO PAGAMENTO NO MENUPRINCIPAL
{
    int cPagamento= 0, cCartao= 0;
    float troco = 0, valorCobrado = 0; /* <<< VARIAVEIS DE ARMAZENAMENTO TEMPOR�RIO */

    system("cls");
    printf("\nForma de Pagamento:\n\n");
    printf("41. Dinheiro\n");
    printf("42. Cartao\n");
    printf("43. Voltar ao menu principal\n");

    scanf("%d", &cPagamento);   /* <--------------- PAGAMENTO EM DINHEIRO */
    switch(cPagamento){
        case 41:
            printf("\n");
            printf("Valor total: %.2f R$\n",fPagar);
            printf("Inserir valor recebido em dinheiro\n");
            scanf("%f", &valorCobrado);

            if (valorCobrado >= fPagar)
            {
                totalDia = totalDia + fPagar;  /* <----------- Soma do total di�rio */
                troco = valorCobrado - fPagar;
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("\nTroco: %.2f R$\n", troco);
                printf("Pagamento realizado com sucesso!\n\n");
                delay(3);
                vLimpeza= 0;
                vAlimento= 0;
                vPadaria= 0;
                fPagar= 0;
                vCar = 0;  /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                menuPrincipal();
                break;
            }

            else if (valorCobrado < fPagar)
            {
                fPagar = fPagar - valorCobrado;
                totalDia=  totalDia + valorCobrado;
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("\nFalta pagar %.2f R$ \n\n", fPagar);
                delay(3);
                menufPagamento();
                break;
            }

        case 42:       /* <--------- PAGAMENTO EM CARTAO */
            printf("Valor total: %.2f R$\n",fPagar);
            printf("51. Pagamento realizado\n");
            printf("52. Pagamento nao realizado\n");
            printf("53. Voltar ao  menu anterior\n");
            scanf("%d", &cCartao);

            if (cCartao == 51)
            {
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("\nPagamento realizado com sucesso!\n\n");
                delay(3);
                totalDia= totalDia + fPagar; /* <----------- Soma do total diario */
                vPadaria = 0; vAlimento= 0; vLimpeza =0; vCar = 0;  /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                menuPrincipal();
            }

            else if (cCartao == 52)
            {
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("\nPagamento nao realizado!\n");
                delay(3);
                menufPagamento();
            }

            else if (cCartao == 53)
            {
                printf("Voltar ao menu anterior\n\n");
                menufPagamento();
            }

            else
            {
                printf("Opcao invalida... Tente novamente\n\n");
            }

            break;

        case 43:
            printf("Voltar ao menu principal\n\n");
            menuPrincipal();
            break;

        default:
            printf("Opcao invalida... Tente Novamente\n\n");
            delay(2);
            menufPagamento();
            break;
    }
}

void menuAbertura(){ // Funcao do Menu de abertura do caixa
    int cAbertura = 0;

    if (caixaAberto == true) // VERIFICA SE HÁ CAIXA ABERTO
    {
        system("cls");
        printf("Existe um caixa em aberto, finalize para realizar uma nova abertura\n");
        for ( int i = 3; i != 0; i--) {
            printf("Retornando em %d segundos \n",i);
            delay(1);
        }
        menuPrincipal();

    } else {

        system("cls");
        printf("    <Abertura de caixa>\n");
        printf("1 - Abrir caixa\n");
        printf("2 - Cancelar\n");
        scanf("%d",&cAbertura);

        switch (cAbertura){
            case 1:
                printf("Informe o valor: ");
                scanf("%f",&trocoInicial);

               while (trocoInicial <=0){
                   printf ("\nErro.. digite um novo valor:\n");
                   scanf ("%f", &trocoInicial);}

                    system("cls");
                    printf("Validando abertura de caixa");
                    delay(0.5);
                    for ( int j = 0; j <= 2; j++) {
                        printf(".");
                        delay(0.5);
                    }
                    system("cls");
                    printf("Caixa aberto com sucesso!!!\n");
                    delay(1);
                    printf("Valor de abertura de caixa %.2f R$", trocoInicial);
                    delay(1);
                    caixaAberto = true;
                    system("cls");
                    abrePadaria();
                    menuPrincipal();
                    break;

            case 2:
                menuPrincipal();
                break;

            default:
                printf("Opcao invalida, tente novamente...");
                delay(1);
                menuAbertura();
                break;
        }
    }
}

void abrePadaria(){ // Funcao da Abertura da Padaria
    printf("Abertura padaria");
    delay(0.5);
    for ( int j = 0; j <= 2; j++){
        printf(".");
        delay(0.5);
    }
    system("cls");

    printf("Digite o estoque de Pao de Forma (pacote): "); // SOLICITA AO USUÁRIO QUE INFORME A QUANTIDADE DE ITENS EM ESTOQUE
    scanf("%d", &estoquePadaria[0]);
    system("cls");
    printf("Digite o estoque de Pao Integral (pacote): ");
    scanf("%d", &estoquePadaria[1]);
    system("cls");
    printf("Digite o estoque de pao Frances (Unidade): ");
    scanf("%d", &estoquePadaria[2]);
    system("cls");
    printf("Digite o estoque de Sonho (Unidade): ");
    scanf("%d", &estoquePadaria[3]);
    system("cls");
    printf("Digite o estoque de Biscoito (KG): ");
    scanf("%d", &estoquePadaria[4]);
    system("cls");
    printf("Digite o estoque de Pao Doce (Unidade): ");
    scanf("%d", &estoquePadaria[5]);
    system("cls");
    printf("Digite o estoque de Salgado (Unidade): ");
    scanf("%d", &estoquePadaria[6]);
    system("cls");
}

void menuFechamento() { // Funcao do menu de Fechamento do caixa
    int cFechamento = 0;
    float vFechamentoD = 0, vFechamentoC = 0, vFechageral = 0, trocoFechamento = 0;

    system("cls");
    printf("1 - Fechar caixa\n");
    printf("2 - Cancelar\n");
    scanf("%d",&cFechamento);

    switch (cFechamento){
        case 1:
            printf("Informe o valor de troco: ");
            scanf("%f", &trocoFechamento);
            if(trocoFechamento != trocoInicial) { //VERIFICA SE O VALOR INFORMADO É IGUAL AO INFORMADO AO ABRIR O CAIXA
                printf("O valor de troco esta incorreto, tente novamente\n");
                delay(1);
                menuFechamento();
            }
            else if(trocoFechamento == trocoInicial) {
                printf("Total de vendas: %.2f\n", totalDia);
                printf("Informe valor em dinheiro: ");
                scanf("%f",&vFechamentoD);
                printf("\nInforme valor em cartao: ");
                scanf("%f",&vFechamentoC);
                vFechageral = vFechamentoD + vFechamentoC;
            if (fabs(vFechageral - totalDia) < 0.01) { //fabs retorna o valor da operação dentro do ( ) .
                printf("Caixa fechado  com sucesso\n"); // se a diferença for menor que 0.01, consideramos os números iguais.
                printf("Valor total em dinheiro: %.2f R$\n", vFechamentoD);
                printf("Valor total em cartao: %.2f R$\n", vFechamentoC);
                printf("Valor de troco inicial: %.2f R$\n", trocoInicial);
                trocoInicial = 0;
                caixaAberto = false;
                delay(3);
                //esperarEnter();
                } else if ( vFechageral > totalDia) // SE OS VALORES INFORMADOS FOREM MAIORES
                    {
                    printf("Esta sobrando dinheiro, tente novamente...\n");
                    vFechageral = 0;
                    delay(2);
                    menuFechamento();
                }

                else if ( vFechageral < totalDia ) // SE OS VALORES INFORMADOS FOREM MENORES
                    {
                    printf("Valores insuficientes\n");
                    printf("Tente novamente...");
                    delay(2);
                    menuFechamento();
                }
            }
            break;

        case 2:
            menuPrincipal();
            break;

        default:
            printf("Opcao Invalida!");
            delay(2);
            break;
    }
}

void menuCancelar(){

    int cCancelar = 0;

    system("cls");
    printf("Deseja cancelar a venda?\n");
    printf("1 - SIM?\n");
    printf("2 - NAO\n");

    scanf("%d",&cCancelar);
    system("cls");

    switch (cCancelar) {
        case 1:
            vPadaria = 0; vAlimento= 0; vLimpeza =0; vCar=0; // CANCELA A VENDA E ZERA TODAS AS VARIÁVEIS DE ACUMULAÇÃO DE VALORES
            printf("Venda cancelada com sucesso...\n");
            delay(3);
            menuPrincipal();
            break;

        case 2:
            menuPrincipal();
            break;

        default:
            printf("Opcao Invalida!");
            delay(2);
            break;
    }
}

int main(){
    system("color 0C"); // Vermelho
    // system("color 7D"); // Roxo
    // system("color 1F"); // Azul
    login(); // Funcao Principal
    return 0;
}
