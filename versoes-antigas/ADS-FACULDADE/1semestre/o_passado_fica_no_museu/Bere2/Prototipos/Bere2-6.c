#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>

/* Funcoes e variaveis globais */
void menuPrincipal(void);
void menuLimpeza(void);
void menuAlimento(void); // Declaracoes das Funcoes
void menuPadaria(void);
void menuPagamento(void);
void menuCaixa(void);
void menuCancelar(void);
void abrePadaria(void);
bool caixaAberto = false;

float vLimpeza = 0, vPadaria = 0, vAlimento = 0, totalDia =  0; // Variaveis globais
float fPagar = 0;
float vCar = 0;
float trocoInicial = 0;

/* <<<<<<< VALORES DOS PRODUTOS >>>>>>> */
struct produto {
    int codigoProduto;
    char nomeProduto[50];
    float valorProduto;
    int estoqueProduto;
};
struct produto limpeza[7] = {
    {11,"Detergente",1.99,50},{12,"Sabao em Po 1KG",9.99,30},{13,"Esponja",1.50,0},{14,"Amaciante 1LT",15.00,20}, // limpeza.codigoProduto // limpeza.nomeProduto // limpeza.valorProduto // limpeza.estoqueProduto
    {15,"Bucha de Pia (KIT C/3)",4.99,100},{16,"Desinfetante 1LT",7.99,15},{17,"Sabao em Barra (UND)",1.00,60}};

struct produto alimentos[7] = {
    {21,"Cafe",19.99,10},{22,"Leite Caixa",5.90,15},{23,"Arroz 1KG",4.50,10},{24,"Feijao Preto 1KG",8.00,5}, // limpeza.codigoProduto // limpeza.nomeProduto // limpeza.valorProduto // limpeza.estoqueProduto
    {25,"Acucar 1KG",5.00,20},{26,"Sal 1KG",2.00,6},{27,"Farinha de Trigo 1KG",5.00,15}};

struct produto padaria[7] = {
    {31,"Pao de Forma PCT",9.50},{32,"Pao Integral PCT",12.50},{33,"Pao Frances UND",1.90},{34,"Sonho UND",8.50},
    {35,"Biscoito KG",12.50},{36,"Pao Doce UND",2.50},{37,"Salgado",17.50}};

void delay(float delayEmSegundos){    // Funcao utilizando a biblioteca "<time.h>" para delay.
    float mili = 1000 * delayEmSegundos;
    clock_t comecoT = clock();
    while (clock() < comecoT + mili);
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
            "| -> Cod: 5  - Caixa \t\t\t\t\t|\n",
            "| -> Cod: 6  - Cancelar venda \t\t\t\t|\n",
            "| -> Cod: 7  - Sair do Programa \t\t\t|\n",
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

        for (i = 0; i < 7; i++) {
            printf("| -> Cod: %d - %-23s R$ %.2f  (%d)\t|\n",
                limpeza[i].codigoProduto, limpeza[i].nomeProduto, limpeza[i].valorProduto, limpeza[i].estoqueProduto);
        }

        printf("|.......................................................|\n");
        printf("| -> Cod: 18 - Voltar ao Menu Principal \t\t|\n");
        printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
        printf("|    $ - Carrinho materiais de limpeza:  R$ %.2f - $\t|\n", vLimpeza);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    if (escolhaMenu == 2) { // 2 == Menu Alimentos.

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

        for (i = 0; i < 7; i++) {
            printf("| -> Cod: %d - %-23s R$ %.2f  (%d)\t|\n",
                alimentos[i].codigoProduto, alimentos[i].nomeProduto,alimentos[i].valorProduto, alimentos[i].estoqueProduto);
        }

        printf("|.......................................................|\n");
        printf("| -> Cod: 28 - Voltar ao Menu Principal \t\t|\n");
        printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
        printf("|\t $ - Carrinho de Alimentos:  R$ %.2f - $\t|\n", vAlimento);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");

    }

    if (escolhaMenu == 3) { // 3 == Menu Padaria.


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

        for(i = 0; i < 7; i++) {
            printf("| -> Cod: %d - %-23s R$ %.2f  (%d)\t|\n",
                padaria[i].codigoProduto, padaria[i].nomeProduto, padaria[i].valorProduto, padaria[i].estoqueProduto);
        }

        printf("|.......................................................|\n");
        printf("| -> Cod: 38 - Voltar ao Menu Principal \t\t|\n");
        printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
        printf("|\t  $ - Carrinho da Padaria:  R$ %.2f - $\t\t|\n", vPadaria);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    if (escolhaMenu == 4) { // 4 == Menu Pagamento
        const char* printMenuPagamento[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t ------------ Menu Pagamento ------------\t|\n",
            "|\t ---* Escolha uma forma de pagamento *---\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            "| -> Cod: 41 - Dinheiro \t\t\t\t|\n",
            "| -> Cod: 42 - Cartao   \t\t\t\t|\n",
            "|.......................................................|\n",
            "| -> Cod: 43 - Voltar ao Menu Principal \t\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL
        };
        for (i = 0; i < pegaTamanho(printMenuPagamento); i++) {
            printf("%s", printMenuPagamento[i]);
        }
        printf("|\t $ - Valor Total Carrinho:  R$ %.2f - $\t\t|\n", totalDia);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    if (escolhaMenu == 5) { // 5 == Menu Caixa
        const char* printMenuCaixa[] = { // 5 == Menu Caixa
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t  ------------ Menu Caixa ------------\t\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            "| -> Cod: 1 - Abertura do Caixa  \t\t\t|\n",
            "| -> Cod: 2 - Fechamento do Caixa\t\t\t|\n",
            "|.......................................................|\n",
            "| -> Cod: 3 - Voltar ao Menu Principal \t\t\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL
        };
        for (i = 0; i < pegaTamanho(printMenuCaixa); i++) {
            printf("%s", printMenuCaixa[i]);
        }
        printf("|\t $ - Valor Total Carrinho:  R$ %.2f - $\t\t|\n", totalDia);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    if (escolhaMenu == 6) { // 6 == Menu Abertura Caixa
        const char* printMenuAberturaCaixa[] = { // 5 == Menu Caixa
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t  ------------ Menu Caixa ------------\t\t|\n",
            "|\t  ---------* Abertura Caixa *---------\t\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            "| -> Cod: 1 - Informar o Valor de Abertura\t\t|\n",
            "| -> Cod: 2 - Voltar ao Caixa\t\t\t\t|\n",
            "|=======================================================|\n",
            "Opcao..:  ",
            NULL
        };
        for (i = 0; i < pegaTamanho(printMenuAberturaCaixa); i++) {
            printf("%s", printMenuAberturaCaixa[i]);
        }
    }
}

void menuPrincipal(){ // Funcao do Menu Principal
    int opcaoUm = 0; // Variavel opcao do usuario

    while (opcaoUm!=7)
    {
        printMenu(0); // 0 == Menu Principal

        scanf("%d", &opcaoUm);

        if (caixaAberto == false && opcaoUm != 7 && opcaoUm != 5)  { //  === VERIFICAÇÃO CAIXA ABERTO CASO ESTIVER FECHADO SOLICITA ABERTURA
            system("cls");
            printf("O caixa esta fechado, realize a abertura");
            delay(0.5);
            for ( int j = 0; j <= 2; j++) {
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
                    menuCaixa();
                    break;
                case 6:
                    menuCancelar();
                    break;
                case 7:
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
            if (limpeza[0].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > limpeza[0].estoqueProduto) {
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Detergente adicionado ao carrinho.\n",qnt);
            delay(3);
            limpeza[0].estoqueProduto = limpeza[0].estoqueProduto - qnt;
            vLimpeza += limpeza[0].valorProduto * qnt;
            vCar += limpeza[0].valorProduto * qnt;
            menuLimpeza();
            break;

        case 12:    /*<----- SABAO EM PO */
            printf("\nSabao em po \n");
            if (limpeza[1].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > limpeza[1].estoqueProduto) {
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Sabao em po adicionado ao carrinho.\n",qnt);
            delay(3);
            limpeza[1].estoqueProduto = limpeza[1].estoqueProduto - qnt;
            vLimpeza += limpeza[1].valorProduto * qnt;
            vCar += limpeza[1].valorProduto * qnt;
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
            if (limpeza[3].estoqueProduto== 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > limpeza[3].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Amaciante adicionado ao carrinho.\n",qnt);
            delay(3);
            limpeza[3].estoqueProduto = limpeza[3].estoqueProduto - qnt;
            vLimpeza += limpeza[3].valorProduto * qnt;
            vCar += limpeza[3].valorProduto * qnt;
            menuLimpeza();
            break;

        case 15:    /*<----- BUCHA DE PIA*/
            printf("Bucha de Pia (Kit c/3) \n");
            if (limpeza[4].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > limpeza[4].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Bucha de Pia adicionada ao carrinho.\n",qnt);
            delay(3);
            limpeza[4].estoqueProduto = limpeza[4].estoqueProduto - qnt;
            vLimpeza += limpeza[4].valorProduto * qnt;
            vCar += limpeza[4].valorProduto * qnt;
            menuLimpeza();
            break;

        case 16:    /*<----- DESINFETANTE*/
            printf("\n");
            printf("Desinfetante (1LT) \n");
            if (limpeza[5].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > limpeza[5].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Desinfetante adicionado ao carrinho.\n",qnt);
            delay(3);
            limpeza[5].estoqueProduto = limpeza[5].estoqueProduto - qnt;
            vLimpeza += limpeza[5].valorProduto * qnt;
            vCar += limpeza[5].valorProduto * qnt;
            menuLimpeza();
            break;

        case 17:    /*<----- SABAO EM BARRA*/
            printf("\n");
            printf("Sabao em Barra (UN) \n");
            if (limpeza[6].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > limpeza[6].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sabao em Barra adicionado ao carrinho.\n",qnt);
            delay(3);
            limpeza[6].estoqueProduto = limpeza[6].estoqueProduto - qnt;
            vLimpeza += limpeza[6].valorProduto * qnt;
            vCar += limpeza[6].valorProduto * qnt;
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
            if (alimentos[0].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > alimentos[0].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Cafe adicionado ao carrinho.\n",qnt);
            delay(3);
            alimentos[0].estoqueProduto = alimentos[0].estoqueProduto - qnt;
            vAlimento += alimentos[0].valorProduto * qnt;
            vCar += alimentos[0].valorProduto * qnt;
            menuAlimento();
            break;

        case 22:    /*<----- LEITE */
            printf("\n");
            printf("\nLeite:\n");
            if (alimentos[1].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > alimentos[1].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Leite adicionado ao carrinho.\n",qnt);
            delay(3);
            alimentos[1].estoqueProduto = alimentos[1].estoqueProduto - qnt;
            vAlimento += alimentos[1].valorProduto * qnt;
            vCar += alimentos[1].valorProduto * qnt;
            menuAlimento();
            break;

        case 23:    /*<----- ARROZ*/
            printf("Arroz 1kg:\n");
            if (alimentos[2].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > alimentos[2].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Arroz 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            alimentos[2].estoqueProduto = alimentos[2].estoqueProduto - qnt;
            vAlimento += alimentos[2].valorProduto * qnt;
            vCar += alimentos[2].valorProduto * qnt;
            menuAlimento();
            break;

        case 24:    /*<----- FEIJAO PRETO*/
            printf("\nFeijao Preto 1kg:\n");
            if (alimentos[3].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > alimentos[3].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Feijao preto 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            alimentos[3].estoqueProduto = alimentos[3].estoqueProduto - qnt;
            vAlimento += alimentos[3].valorProduto * qnt;
            vCar += alimentos[3].valorProduto * qnt;
            menuAlimento();
            break;

        case 25:    /*<----- ACUCAR*/
            printf("\nAcucar 1kg:\n");
            if (alimentos[4].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > alimentos[4].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Acucar 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            alimentos[4].estoqueProduto = alimentos[4].estoqueProduto - qnt;
            vAlimento += alimentos[4].valorProduto * qnt;
            vCar += alimentos[4].valorProduto * qnt;
            menuAlimento();
            break;

        case 26:    /*<----- ACUCAR*/
            printf("\nSal 1kg:\n");
            if (alimentos[5].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > alimentos[5].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sal 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            alimentos[5].estoqueProduto = alimentos[5].estoqueProduto - qnt;
            vAlimento += alimentos[5].valorProduto * qnt;
            vCar += alimentos[5].valorProduto * qnt;
            menuAlimento();
            break;

        case 27:    /*<----- ACUCAR*/
            printf("\nFarinha de Trigo 1kg:\n");
            if (alimentos[6].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > alimentos[6].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Farinha de Trigo 1kg adicionada ao carrinho.\n",qnt);
            delay(3);
            alimentos[6].estoqueProduto = alimentos[6].estoqueProduto - qnt;
            vAlimento += alimentos[6].valorProduto * qnt;
            vCar += alimentos[6].valorProduto * qnt;
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
            if (padaria[0].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > padaria[0].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao de Forma adicionado ao carrinho.\n",qnt);
            delay(2);
            padaria[0].estoqueProduto = padaria[0].estoqueProduto - qnt;
            vPadaria += padaria[0].valorProduto* qnt;
            vCar += padaria[0].valorProduto * qnt;
            menuPadaria();
            break;

        case 32:    /*<----- PAO INTEGRAL */
            printf("\nPao Integral(PCT):\n");
            if (padaria[1].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > padaria[1].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao integral adicionado ao carrinho.\n",qnt);
            delay(2);
            padaria[1].estoqueProduto = padaria[1].estoqueProduto - qnt;
            vPadaria += padaria[1].valorProduto * qnt;
            vCar += padaria[1].valorProduto * qnt;
            menuPadaria();
            break;

        case 33:    /*<----- PAO FRANCES*/
            printf("\nPao Frances(UND):\n");
            if (padaria[2].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > padaria[2].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao frances adicionado ao carrinho.\n",qnt);
            delay(2);
            padaria[2].estoqueProduto = padaria[2].estoqueProduto - qnt;
            vPadaria += padaria[2].valorProduto * qnt;
            vCar += padaria[2].valorProduto * qnt;
            menuPadaria();
            break;

        case 34:    /*<----- SONHO*/
            printf("\nSonho(UND):\n");
            if (padaria[3].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > padaria[3].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sonho adicionado ao carrinho.\n",qnt);
            delay(2);
            padaria[3].estoqueProduto = padaria[3].estoqueProduto - qnt;
            vPadaria += padaria[3].valorProduto * qnt;
            vCar += padaria[3].valorProduto * qnt;
            menuPadaria();
            break;

        case 35:    /*<----- SONHO*/
            printf("\nBiscoito(KG): \n");
            if (padaria[4].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > padaria[4].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Biscoito adicionado ao carrinho.\n",qnt);
            delay(2);
            padaria[4].estoqueProduto = padaria[4].estoqueProduto - qnt;
            vPadaria += padaria[4].valorProduto * qnt;
            vCar += padaria[4].valorProduto * qnt;
            menuPadaria();
            break;

        case 36:    /*<----- SONHO*/
            printf("\nPao Doce(UND):\n");
            if (padaria[5].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > padaria[5].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao Doce adicionado ao carrinho.\n",qnt);
            delay(2);
            padaria[5].estoqueProduto = padaria[5].estoqueProduto - qnt;
            vPadaria += padaria[5].valorProduto* qnt;
            vCar += padaria[5].valorProduto * qnt;
            menuPadaria();
            break;

        case 37:    /*<----- SONHO*/
            printf("\nSalgado(UND):\n");
            if (padaria[6].estoqueProduto == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > padaria[6].estoqueProduto){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Salgado adicionado ao carrinho.\n",qnt);
            delay(2);
            padaria[6].estoqueProduto = padaria[6].estoqueProduto - qnt;
            vPadaria += padaria[6].valorProduto * qnt;
            vCar += padaria[6].valorProduto * qnt;
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
    float vTotal = 0, vTcdesconto = 0, troco = 0, valorCobrado = 0, descontoReal = 0;

    /* <<< VARIAVEIS DE ARMAZENAMENTO FIXO */
    float descontoCinco, descontoDez, descontoInformado, descontoBruto;

        /* DESCONTOS */
    descontoCinco = 0.05;   /* <<<- 5% desconto */
    descontoDez = 0.10;  /* <<<- 10% desconto */

    vTotal = vLimpeza + vPadaria + vAlimento; // Calculo valor total sem desconto

    printMenu(4); // 4 == Menu Pagamento
    if(fPagar != 0){
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
                menuPagamento();
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
                menuPagamento();
            }

            else if (cCartao == 53)
            {
                printf("Voltar ao menu anterior\n\n");
                menuPagamento();
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
            menuPagamento();
            break;
    }
    } else {
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
                menuPagamento();
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
}

void menuCaixa() {
    int entradaUsuario = 0;
    printMenu(5); // 5 == Menu Caixa
    scanf("%d", &entradaUsuario);

    switch (entradaUsuario) {
        case 1: { // 1 == Abertura do Caixa
            int cAbertura = 0;

            if (caixaAberto == true) // VERIFICA SE HÁ CAIXA ABERTO
            {
                system("cls");
                printf("! - Erro, existe um caixa em aberto - !\n");
                for ( int i = 3; i != 0; i--) {
                    printf("Retornando em %d segundos \n",i);
                    delay(0.5);
                }
                menuCaixa();

            } else {
                printMenu(6); // 6 == Abertura do Caixa
                fflush(stdout);
                scanf("%d",&cAbertura);
                printf("|=======================================================|\n");

                switch (cAbertura){
                    case 1:
                        printf("|\t@ ---- Valor de Abertura do Caixa ---- @\t|\n");
                        printf("| -> R$ ");
                        scanf("%.2f", &trocoInicial);

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
                        menuCaixa();
                    break;

                    default:
                        printf("Opcao invalida, tente novamente...");
                    delay(1);
                    menuCaixa();
                    break;
                }
            }
        }
        case 2: {
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
                        menuCaixa();
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
                            delay(2);
                            menuCaixa();
                        }

                        else if ( vFechageral < totalDia ) // SE OS VALORES INFORMADOS FOREM MENORES
                            {
                            printf("Valores insuficientes\n");
                            printf("Tente novamente...");
                            delay(2);
                            menuCaixa();
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

    for(int i = 0; i < 7; i++){
    system("cls");
    printf("Digite o estoque de %s: ", padaria[i].nomeProduto); // SOLICITA AO USUÁRIO QUE INFORME A QUANTIDADE DE ITENS EM ESTOQUE
    scanf("%d", &padaria[i].estoqueProduto);}

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
    menuPrincipal(); // Funcao Principal
    return 0;
}
