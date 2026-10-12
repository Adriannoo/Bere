#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>

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

float vLimpeza = 0, vPadaria = 0, vAlimento = 0, totalDia =  0;
float tLimpeza = 0, tPadaria = 0, tAlimento = 0;                 // Variaveis globais
float fPagar = 0;
float vCar = 0;
float vAbre = 0;
int opcao = 0, opcao2 = 0, linha = 0; // Variavel para opcao do usuario

/* <<<<<<< VALORES DOS PRODUTOS >>>>>>> */
struct produto {
    int codigoProduto;
    char nomeProduto[50];
    float valorProduto;
    int estoqueProduto;
};

struct produto limpeza[][1] = { // struct matriz para cada item da limpeza.
    {{11, "Detergente", 1.99, 50}},
    {{12, "Sabao em Po 1KG", 9.99, 30}},
    {{13, "Esponja", 1.50, 0}},
    {{14, "Amaciante 1LT", 15.00, 20}},
    {{15, "Bucha de Pia (KIT C/3)", 4.99, 100}},
    {{16, "Desinfetante 1LT", 7.99, 15}},
    {{17, "Sabao em Barra (UND)", 1.00, 60}}
};

struct produto alimentos[][1] = { // struct matriz para cada item de alimentos.
    {{21, "Cafe", 19.99, 10}},
    {{22, "Leite Caixa", 5.90, 15}},
    {{23, "Arroz 1KG", 4.50, 10}},
    {{24, "Feijao Preto 1KG", 8.00, 5}},
    {{25, "Acucar 1KG", 5.00, 20}},
    {{26, "Sal 1KG", 2.00, 6}},
    {{27, "Farinha de Trigo 1KG", 5.00, 15}}
};

struct produto padaria[][1] = { // struct matriz para cada item da padaria.
    {{31, "Pao de Forma PCT", 9.50}},
    {{32, "Pao Integral PCT", 12.50}},
    {{33, "Pao Frances UND", 1.90}},
    {{34, "Sonho UND", 8.50}},
    {{35, "Biscoito KG", 12.50}},
    {{36, "Pao Doce UND", 2.50}},
    {{37, "Salgado", 17.50}}
};

float truncar(float valor, int casasDecimais) {
    float potencia = pow(10.0f, casasDecimais);
    return truncf(valor * potencia) / potencia;
}

void delay(float delayEmSegundos){    // Funcao utilizando a biblioteca "<time.h>" para delay.
    float mili = 1000 * delayEmSegundos;
    clock_t comecoT = clock();
    while (clock() < comecoT + mili);
}

int pegaTamanho(char* array[]) {
    int tamanho = 0;
    while (array[tamanho] != NULL) {
        tamanho++;
    }
    return tamanho;
}

void printMenu(int const escolhaMenu) { // Print dos Menus referente a escolha.
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

        printf("|      $ - Valor total de vendas hoje: R$ %.2f - $\t|\n", totalDia); // Atualizacao do valor total das vendas
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
            "| Cod  | Nome do produto             | Valor  | Estoque |\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL // Finaliza o Array de Strings
        };

        for (i = 0; i < pegaTamanho(printMenuLimpeza); i++) {
            printf("%s", printMenuLimpeza[i]);
        }

        for (i = 0; i < 7; i++) { //apresentaçao dos itens de limpeza
           printf("| %4d | %-27s | %6.2f | %7d |\n",
           limpeza[i][0].codigoProduto,
           limpeza[i][0].nomeProduto,
           limpeza[i][0].valorProduto,
           limpeza[i][0].estoqueProduto);
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
            "|Cod   | Nome do produto             | Valor  | Estoque |\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL // Finaliza o Array de Strings
        };

        for(i = 0; i < pegaTamanho(printMenuAlimentos); i++) {
            printf("%s", printMenuAlimentos[i]);
        }

        for (i = 0; i < 7; i++) { //apresentaçao dos itens de alimentos
            printf("| %4d | %-27s | %6.2f | %7d |\n",
            alimentos[i][0].codigoProduto,
            alimentos[i][0].nomeProduto,
            alimentos[i][0].valorProduto,
            alimentos[i][0].estoqueProduto);
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
            "|Cod   | Nome do produto             | Valor  | Estoque |\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL    // Finaliza o Array de Strings
        };

        for(i = 0; i < pegaTamanho(printMenuPadaria); i++) {
            printf("%s", printMenuPadaria[i]);
        }

        for (i = 0; i < 7; i++) { //apresentaçao dos itens de padaria
            printf("| %4d | %-27s | %6.2f | %7d |\n",
            padaria[i][0].codigoProduto,
            padaria[i][0].nomeProduto,
            padaria[i][0].valorProduto,
            padaria[i][0].estoqueProduto);
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
        printf("|\t $ - Valor Total Carrinho:  R$ %.2f - $\t\t|\n", vCar);
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
        printf("|\t $ - Valor Total Carrinho:  R$ %.2f - $\t|\n", totalDia);
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

void menuPrincipal() { // Funcao do Menu Principal
    while (opcao!=7)
    {
        printMenu(0); // 0 == Menu Principal

        scanf("%d", &opcao);

        if (caixaAberto == false && opcao != 7 && opcao != 5)  { //  === VERIFICAÇÃO CAIXA ABERTO CASO ESTIVER FECHADO SOLICITA ABERTURA
            system("cls");
            printf("O caixa esta fechado, realize a abertura");
            delay(0.5);
            for ( int j = 0; j <= 2; j++) {
                printf(".");
                delay(0.5);
            }
            menuPrincipal();

        } else {
            switch(opcao) { // Switch Case para as sessoes do mercadinho
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


void menuLimpeza() { // Funcao Menu Limpeza
    float qnt;

    printMenu(1); // 1 == Menu de Limpeza
    scanf("%d", &opcao);

    if (opcao >= 11 && opcao <= 17) {
        linha = opcao - 11;             // -11 por conta da matriz iniciando em 0 para identificar o cod de item

        printf("\n%s\n", limpeza[linha][0].nomeProduto);            //acessa a linha digitada em opcao e a coluna [0].

        if (limpeza[linha][0].estoqueProduto == 0) {
            printf("Infelizmente o item nao registra estoque, digite um novo item\n");
            delay(3);
            menuLimpeza();
            return;
        }

        printf("Inserir quantidade: ");
        scanf("%f", &qnt);

        while (qnt > limpeza[linha][0].estoqueProduto) {            //caso o solicitado seja maior do que o estoque
            printf("Quantidade insuficiente, digite um novo valor: \n");
            delay(2);
            scanf("%f", &qnt);
        }

        printf("%.2f x %s adicionado ao carrinho.\n", qnt, limpeza[linha][0].nomeProduto);
        delay(3);

        limpeza[linha][0].estoqueProduto -= qnt;                //subtrai no estoque
        vLimpeza += limpeza[linha][0].valorProduto * qnt;       //vLimpeza apresenta o total na Limpeza
        vCar += limpeza[linha][0].valorProduto * qnt;           // vCar e a variavel para apresentar o total no carrinho

        menuLimpeza();
    }
    else if (opcao == 18) {
        printf("\nVoltar ao menu principal\n");
        delay(2);
    }
    else {
        printf("\nOpcao invalida... Tente Novamente\n");
        delay(2);
        menuLimpeza();
    }
}

void menuAlimento() { // Funcao Menu Alimentos
    float qnt;
    printMenu(2); // 2 == Menu de Alimentos

    scanf("%d", &opcao);

    if (opcao >= 21 && opcao <= 27) {
        linha = opcao - 21;             //-21 por conta da matriz iniciando em 0 para identificar o cod de item
        printf("\n%s:\n", alimentos[linha][0].nomeProduto);       //acessa a linha digitada em opcao e a coluna [0].

        if (alimentos[linha][0].estoqueProduto == 0) {            //caso o solicitado seja maior do que o estoque
            printf("Infelizmente o item nao registra estoque, digite um novo item\n");
            delay(3);
            menuAlimento();
            return;
        }

        printf("Inserir quantidade: ");
        scanf("%f", &qnt);

        while (qnt > alimentos[linha][0].estoqueProduto) {
            printf("Quantidade insuficiente, digite um novo valor: \n");
            delay(3);
            scanf("%f", &qnt);
        }

        printf("%.2f x %s adicionado ao carrinho.\n", qnt, alimentos[linha][0].nomeProduto);
        delay(2);

        alimentos[linha][0].estoqueProduto -= qnt;                  //subtrai no estoque
        vAlimento += alimentos[linha][0].valorProduto * qnt;        //vAlimento apresenta o total no Alimento
        vCar += alimentos[linha][0].valorProduto * qnt;             // vCar e a variavel para apresentar o total no carrinho
        menuAlimento();
    }
    else if (opcao == 28) {
        printf("\nVoltar ao menu principal\n");
        delay(2);
    }
    else {
        printf("\nOpcao invalida... Tente Novamente\n");
        delay(2);
        menuAlimento();
    }
}

void menuPadaria() { // Funcao Menu Padaria
    float qnt;
    printMenu(3); // 3 == Menu de Padaria

    scanf("%d", &opcao);

    if (opcao >= 31 && opcao <= 37) {
        linha = opcao - 31;                 //-31 por conta da matriz iniciando em 0 para identificar o cod de item
        printf("\n%s:\n", padaria[linha][0].nomeProduto);       //acessa a linha digitada em opcao e a coluna [0].

        if (padaria[linha][0].estoqueProduto == 0) {            //caso o solicitado seja maior do que o estoque
            printf("Infelizmente o item nao registra estoque, digite um novo item\n");
            delay(3);
            menuPadaria();
            return;
        }

        printf("Inserir quantidade: ");
        scanf("%f", &qnt);

        while (qnt > padaria[linha][0].estoqueProduto) {
            printf("Quantidade insuficiente, digite um novo valor: \n");
            delay(2);
            scanf("%f", &qnt);
        }

        printf("%.2f x %s adicionado ao carrinho.\n", qnt, padaria[linha][0].nomeProduto);
        delay(2);
        padaria[linha][0].estoqueProduto -= qnt;                    //subtrai no estoque
        vPadaria += padaria[linha][0].valorProduto * qnt;           //vPadaria apresenta o total na Padaria
        vCar += padaria[linha][0].valorProduto * qnt;               // vCar e a variavel para apresentar o total no carrinho
        menuPadaria();
    }
    else if (opcao == 38) {
        printf("\nVoltar ao menu principal\n");
        delay(2);
    }
    else {
        printf("\nOpcao invalida... Tente Novamente\n");
        delay(2);
        menuPadaria();
    }
}

void menuPagamento() // Funcao Menu Pagamento
{
    /* <<< VARIAVEIS DE ARMAZENAMENTO TEMPORARIO */
    float vTotal = 0, vTcdesconto = 0, troco = 0, valorCobrado = 0, descontoReal = 0;

    /* <<< VARIAVEIS DE DESCONTO */
    float descontoCinco = 0.05, descontoDez = 0.10, descontoInformado = 0;

    vTotal = vLimpeza + vPadaria + vAlimento; // Calculo valor total sem desconto

    printMenu(4); // 4 == Menu Pagamento

    if(fPagar != 0){
        scanf("%d", &opcao);   /* <--------------- PAGAMENTO EM DINHEIRO */
        switch(opcao){
            case 41:
                printf("\n");
                printf("Valor total: %.2f R$\n", fPagar);
                printf("Inserir valor recebido em dinheiro\n");
                scanf("%.2f", &valorCobrado);

                if (valorCobrado > fPagar)
                {
                    totalDia = totalDia + fPagar;  /* <----------- Soma do total diario */
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
                    tLimpeza = tLimpeza + vLimpeza;
                    tAlimento = tAlimento + vAlimento;
                    tPadaria = tPadaria + vPadaria;
                    vLimpeza= 0; vAlimento= 0; vPadaria= 0; fPagar= 0; vCar = 0;      /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                    menuPrincipal();
                    break;
                }
                if (valorCobrado == fPagar) {
                    totalDia = totalDia + fPagar;  /* <----------- Soma do total diario */
                    system("cls");
                    printf("Processando pagamento");
                    delay(1);
                    for ( int j = 0; j <= 2; j++){
                        printf(".");
                        delay(1);
                    }
                    system("cls");
                    printf("Pagamento realizado com sucesso!\n\n");
                    delay(3);
                    tLimpeza = tLimpeza + vLimpeza;
                    tAlimento = tAlimento + vAlimento;
                    tPadaria = tPadaria + vPadaria;
                    vLimpeza= 0; vAlimento= 0; vPadaria= 0; fPagar= 0; vCar = 0;      /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                    menuPrincipal();
                    break;
                }
                if (valorCobrado < fPagar) {
                    fPagar = fPagar - valorCobrado;
                    totalDia = totalDia + valorCobrado;
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
                scanf("%d", &opcao);

                if (opcao == 51) {
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
                    tLimpeza = tLimpeza + vLimpeza;
                    tAlimento = tAlimento + vAlimento;
                    tPadaria = tPadaria + vPadaria;
                    vLimpeza= 0; vAlimento= 0; vPadaria= 0; fPagar= 0; vCar = 0;      /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                    menuPrincipal();
                }

                else if (opcao == 52)
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

                else if (opcao == 53)
                {
                    printf("Voltar ao menu anterior\n\n");
                    menuPagamento();
                }

                else {
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

    scanf("%d", &opcao);   /* <--------------- PAGAMENTO EM DINHEIRO */
    float opcaoDesconto = 0.00;
    float recebido;

    switch(opcao){ // Switch case para verificar valor com desconto
        case 41:
            printf("\nValor total: %.2f R$\n", vTotal);

            if (vTotal <= 50) {
                printf("O desconto de 5 porcento sera aplicado\n");
                descontoReal = vTotal * descontoCinco;
            }

            else if (vTotal > 50 && vTotal < 200) {
                printf("O desconto de 10 porcento sera aplicado\n");
                descontoReal = vTotal * descontoDez;
            }

            else if (vTotal > 200) {
                printf("Informe a porcentagem de desconto (MAX 50):");
                scanf("%f",&opcaoDesconto);
                if (opcaoDesconto < 50) {
                    printf("O desconto de %.2f porcento sera aplicado\n", opcaoDesconto);
                    descontoInformado = opcaoDesconto / 100;
                    descontoReal = vTotal * descontoInformado;
                } else {
                    printf("Erro, digite um valor de desconto valido!");
                    menuPagamento();
                }
            }

            vTcdesconto = vTotal - descontoReal; // Calculo valor total com desconto
            vTcdesconto = truncar(vTcdesconto, 2);
            printf("\nValor com desconto: %f R$\n", vTcdesconto);
            printf("Inserir valor recebido em dinheiro\n");
            scanf("%f", &recebido);

            if (recebido >= vTcdesconto) {
                troco = recebido - vTcdesconto;
                totalDia = totalDia + vTcdesconto;  /* <----------- Soma do total diario */
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
                tLimpeza = tLimpeza + vLimpeza;
                tAlimento = tAlimento + vAlimento;
                tPadaria = tPadaria + vPadaria;
                vLimpeza= 0; vAlimento= 0; vPadaria= 0; fPagar= 0; vCar = 0;      /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                break;
            }

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
            fPagar = vTotal - opcao;
            totalDia= totalDia + opcao;
            printf("Falta pagar %.2f R$", fPagar);
            delay(2);
            menuPagamento();
            break;

        case 42:       /* <--------- PAGAMENTO EM CART�O */
            printf("Valor total: %.2f R$\n\n",vTotal);
            printf("51. Pagamento realizado\n");
            printf("52. Pagamento nao realizado\n");
            printf("53. Voltar ao  menu anterior\n");
            scanf("%d", &opcao2);

            if (opcao2 == 51)
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
                tLimpeza = tLimpeza + vLimpeza;
                tAlimento = tAlimento + vAlimento;
                tPadaria = tPadaria + vPadaria;
                vLimpeza= 0; vAlimento= 0; vPadaria= 0; fPagar= 0; vCar = 0;      /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
            }

            else if (opcao2 == 52)
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

            else if (opcao2 == 53)
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

    printMenu(5); // 5 == Menu Caixa
    scanf("%d", &opcao);

    switch (opcao) {
        case 1: { // 1 == Abertura do Caixa

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
                scanf("%d",&opcao2);
                printf("|=======================================================|\n");

                switch (opcao2){
                    case 1:
                        printf("|\t@ ---- Valor de Abertura do Caixa ---- @\t|\n");
                        printf("| -> R$ ");
                        scanf("%.2f", &vAbre);

                        while (vAbre <=0){
                            printf ("\nErro.. digite um novo valor:\n");
                            scanf ("%f", &vAbre);}

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
                        printf("Valor de abertura de caixa %.2f R$", vAbre);
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

            system("cls");
            printf("1 - Fechar caixa\n");
            printf("2 - Cancelar\n");
            scanf("%d",&opcao2);

            switch (opcao2){
                case 1:
                    printf("Valor de abertura: R$ %.2f\n\n", vAbre);
                    totalDia = totalDia - vAbre;
                    if (totalDia <= 0){
                        totalDia = 0;
                    }
                    printf("Faturamento total: %.2f\n", totalDia);
                    printf("\nTotal Limpeza: %.2f\n", tLimpeza);
                    printf("Total Alimento: %.2f\n", tAlimento);
                    printf("Total Padaria: %.2f\n", tPadaria);
                    caixaAberto = false;
                    delay(5);
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


void abrePadaria() { // Funcao da Abertura da Padaria
    printf("Abertura padaria");
    delay(0.5);
    for (int j = 0; j <= 2; j++) {
        printf(".");
        delay(0.5);
    }
    system("cls");

    for (int i = 0; i < 7; i++) {
        printf("Digite o estoque de %s: ", padaria[i][0].nomeProduto);
        scanf("%d", &padaria[i][0].estoqueProduto);
        system("cls");
    }
}

void menuCancelar(){

    system("cls");
    printf("Deseja cancelar a venda?\n");
    printf("1 - SIM?\n");
    printf("2 - NAO\n");

    scanf("%d",&opcao);
    system("cls");

    switch (opcao) {
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