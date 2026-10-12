#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>

// ===== FUNÇÕES/PROTOTIPAGENS ===== //
void menuPrincipal(void);
void menuLimpeza(void);
void menuAlimento(void);
void menuPadaria(void);
void menuPagamento(void);
void menuCaixa(void);
void menuCancelar(void);
void abrePadaria(void);

//===== VARIÁVEIS BOOL =====//
bool caixaAberto = false;
bool deve = false;

//==== VARIÁVEIS FLOAT =====//
float vTotal = 0, vTcdesconto = 0, troco = 0, recebido = 0, vPagar = 0;
float vLimpeza = 0, vPadaria = 0, vAlimento = 0, totalDia =  0;
float tLimpeza = 0, tPadaria = 0, tAlimento = 0;
float fPagar = 0;
float vCar = 0;
float vAbre = 0;

//==== VARIÁVEIS INTEIROS ====//
int opcao, opcao2, linha = 0;

//=== STRUCTS ===//
struct produto {
    int codigoProduto;
    char nomeProduto[50];
    float valorProduto;
    int estoqueProduto;
};

//==== MATRIZ DE STRUCT LIMPEZA ====//
//=== Array Bidimensional, cada elemento um produto ===//
struct produto limpeza[7][1] = { // matriz de struct para cada item da limpeza.
    {{11, "Detergente", 1.99, 50}},
    {{12, "Sabao em Po 1KG", 9.99, 30}},
    {{13, "Esponja", 1.50, 0}},
    {{14, "Amaciante 1LT", 15.00, 20}},
    {{15, "Bucha de Pia (KIT C/3)", 4.99, 100}},
    {{16, "Desinfetante 1LT", 7.99, 15}},
    {{17, "Sabao em Barra (UND)", 1.00, 60}}
};

//==== MATRIZ DE STRUCT ALIMENTOS ====//
struct produto alimentos[7][1] = { // matriz de struct para cada item de alimentos.
    {{21, "Cafe", 19.99, 10}},
    {{22, "Leite Caixa", 5.90, 15}},
    {{23, "Arroz 1KG", 4.50, 10}},
    {{24, "Feijao Preto 1KG", 8.00, 5}},
    {{25, "Acucar 1KG", 5.00, 20}},
    {{26, "Sal 1KG", 2.00, 6}},
    {{27, "Farinha de Trigo 1KG", 5.00, 15}}
};

//==== MATRIZ DE STRUCT PADARIA ====//
struct produto padaria[7][1] = { // matriz de struct para cada item da padaria.
    {{31, "Pao de Forma PCT", 9.50,0}},
    {{32, "Pao Integral PCT", 12.50,0}},
    {{33, "Pao Frances UND", 1.90,0}},
    {{34, "Sonho UND", 8.50,0}},
    {{35, "Biscoito KG", 12.50,0}},
    {{36, "Pao Doce UND", 2.50,0}},
    {{37, "Salgado", 17.50,0}}
};

//==== MATRIZ RESET ESTOQUE LIMPEZA ====//
struct produto limpezaPadrao[7][1] = {
    {{11, "Detergente", 1.99, 50}},
    {{12, "Sabao em Po 1KG", 9.99, 30}},
    {{13, "Esponja", 1.50, 0}},
    {{14, "Amaciante 1LT", 15.00, 20}},
    {{15, "Bucha de Pia (KIT C/3)", 4.99, 100}},
    {{16, "Desinfetante 1LT", 7.99, 15}},
    {{17, "Sabao em Barra (UND)", 1.00, 60}}
};

//==== MATRIZ RESET ESTOQUE ALIMENTO ====//
struct produto alimentoPadrao[7][1] = {
    {{21, "Cafe", 19.99, 10}},
    {{22, "Leite Caixa", 5.90, 15}},
    {{23, "Arroz 1KG", 4.50, 10}},
    {{24, "Feijao Preto 1KG", 8.00, 5}},
    {{25, "Acucar 1KG", 5.00, 20}},
    {{26, "Sal 1KG", 2.00, 6}},
    {{27, "Farinha de Trigo 1KG", 5.00, 15}}
};


//==== FUNCAO CRIADA PARA RESET DE ESTOQUE DE PRODUTOS ====//
void reset() {
    for (int i = 0; i < 7; i++) {
        limpeza[i][0] = limpezaPadrao[i][0];
        alimentos[i][0] = alimentoPadrao[i][0];
    }
}


//==== FUNÇÃO DELAY ====//
//== Função utilizando a biblioteca "<time.h>" para delay. ==//
void delay(float delayEmSegundos) {
    float mili = 1000 * delayEmSegundos;
    clock_t comecoT = clock();
    while (clock() < comecoT + mili);
}

//==== FUNÇÃO PEGA TAMANHO ====//
//== Retorna o tamanho de um array de strings ==//
int pegaTamanho(char* array[]) {
    int tamanho = 0;
    while (array[tamanho] != NULL) {
        tamanho++;
    }
    return tamanho;
}

//==== FUNÇÃO TRUNCAR ====//
//== Limita o tamanho de um float, faz a potencia, trunca e volta a virgula ==//
float truncar(float valor, int casasDecimais) {
    float potencia = pow(10.0f, casasDecimais);
    return truncf(valor * potencia) / potencia;
}

//==== FUNÇÃO PRINTMENU ====//
//== Exibe os menus conforme a escolha ==//
void printMenu(int const escolhaMenu) {
    int i = 0;
    system("cls");

    //==== MENU PRINCIPAL ====//
    if(escolhaMenu == 0) {
        const char* printMenuPrincipal[] = { // Array de ponteiro, guarda endereço de cada string.
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
            NULL
        };

        for(i = 0 ; i < pegaTamanho(printMenuPrincipal) ; i++) {
            printf("%s", printMenuPrincipal[i]); // "i" Aponta para a string do Array.
        }

        printf("|      $ - Valor total de vendas hoje: R$ %.2f - $\t|\n", totalDia);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    //==== MENU LIMPEZA ====//
    if (escolhaMenu == 1) {
        const char* printMenuLimpeza[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t------------ Menu de Limpeza ------------\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            "| Cod  | Nome do produto             | Valor  | Estoque |\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL
        };

        for (i = 0; i < pegaTamanho(printMenuLimpeza); i++) {
            printf("%s", printMenuLimpeza[i]);
        }

        for (i = 0; i < 7; i++) {
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

    //==== MENU ALIMENTOS ====//
    if (escolhaMenu == 2) {
        const char* printMenuAlimentos[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|      ------------ Menu de Alimentos ------------\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            "| Cod  | Nome do produto             | Valor  | Estoque |\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL
        };

        for(i = 0; i < pegaTamanho(printMenuAlimentos); i++) {
            printf("%s", printMenuAlimentos[i]);
        }

        for (i = 0; i < 7; i++) {
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

    //==== MENU PADARIA ====//
    if (escolhaMenu == 3) {
        const char* printMenuPadaria[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|      ------------ Menu da Padaria ------------\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            "| Cod  | Nome do produto             | Valor  | Estoque |\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL
        };

        for(i = 0; i < pegaTamanho(printMenuPadaria); i++) {
            printf("%s", printMenuPadaria[i]);
        }

        for (i = 0; i < 7; i++) {
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

    //==== MENU PAGAMENTO ====//
    if (escolhaMenu == 4) {
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

        printf("|\t$ - Valor Total Carrinho: R$ %.2f - $\t\t|\n", vCar);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    //==== MENU CAIXA ====//
    if (escolhaMenu == 5) {
        const char* printMenuCaixa[] = {
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

        printf("|\t $ - Valor Total de Vendas:  R$ %.2f - $\t|\n", totalDia);
        printf("|=======================================================|\n");
        printf("Opcao..:  ");
    }

    //==== MENU ABERTURA DE CAIXA ====//
    if (escolhaMenu == 6) {
        const char* printMenuAberturaCaixa[] = {
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

    //==== MENU FECHAMENTO DE CAIXA ====//
    if (escolhaMenu == 7) {
        const char* printMenuFechamentoCaixa[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t  ------------ Menu Caixa ------------\t\t|\n",
            "|\t  --------* Fechamento Caixa *--------\t\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            "| -> Cod: 1 - Fechar Caixa\t\t\t\t|\n",
            "| -> Cod: 2 - Cancelar\t\t\t\t\t|\n",
            "|=======================================================|\n",
            "Opcao..:  ",
            NULL
        };

        for (i = 0; i < pegaTamanho(printMenuFechamentoCaixa); i++) {
            printf("%s", printMenuFechamentoCaixa[i]);
        }
    }

    //==== MENU RELATORIO DE CAIXA ====//
    if (escolhaMenu == 8) {
        const char* printMenuRelatorioCaixa[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t  ------------ Menu Caixa ------------\t\t|\n",
            "|\t  -------* Relatorio de Venda *-------\t\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL
        };

        for (i = 0; i < pegaTamanho(printMenuRelatorioCaixa); i++) {
            printf("%s", printMenuRelatorioCaixa[i]);
        }

        printf("|\t @ --- Valor de abertura: R$ %.2f --- @  \t|\n", vAbre);
        printf("|\t\tFaturamento total: %.2f  \t\t|\n", totalDia);
        printf("|\t\tTotal Limpeza:     %.2f\t\t\t|\n", tLimpeza);
        printf("|\t\tTotal Alimento:    %.2f \t\t|\n", tAlimento);
        printf("|\t\tTotal Padaria:     %.2f\t\t\t|\n", tPadaria);
        printf("|=======================================================|\n");
    }

    //==== MENU ABERTURA PADARIA ====//
    if (escolhaMenu == 9) {
        const char* printMenuAbrePadaria[] = {
            "|=======================================================|\n",
            "|\t\t    Mercado Dona Bere\t\t\t|\n",
            "|=======================================================|\n",
            "|\t------------ Estoque Padaria ------------\t|\n",
            "|\t---* Informe a Quanitdade do Estoque *---\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL
        };

        for (i = 0; i < pegaTamanho(printMenuAbrePadaria); i++ ) {
            printf("%s", printMenuAbrePadaria[i]);
        }
    }

    //==== ERROS EM GERAL ====//
    if (escolhaMenu == 404) {
        const char* printErro[] = {
            "|=======================================================|\n",
            "|\t#\t!!! ----- ERROR ----- !!!\t#\t|\n",
            "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n",
            NULL
        };
        for (i = 0; i < pegaTamanho(printErro); i++) {
            printf("%s", printErro[i]);
        }
    }
}

//==== FUNÇÃO MENU PRINCIPAL ====//
//== Controla a navegação principal entre os menus do sistema ==//
void menuPrincipal() {

    while (opcao != 7) {

        printMenu(0); // 0 == Menu Principal
        scanf("%d", &opcao);

        //=== VERIFICAÇÃO: SE O CAIXA ESTÁ FECHADO ===//
        // Caso o caixa esteja fechado, impede o acesso a menus que dependem da abertura
        if (caixaAberto == false && opcao != 5 && opcao != 7) {
            printMenu(404);
            printf("|\t-> !! ---- CAIXA ESTA FECHADO ---- !! <-\t|\n");
            printf("|=======================================================|\n");
            delay(1);
            for (int j = 0; j <= 2; j++) {
                printf(".");
                delay(0.5);
            }
            menuPrincipal();
        }

        else {
            //=== OPÇÕES DE ACESSO AOS MENUS DO SISTEMA ===//
            switch (opcao) {
                case 1:
                    menuLimpeza();   // Acesso ao menu de Limpeza
                    break;
                case 2:
                    menuAlimento();  // Acesso ao menu de Alimentos
                    break;
                case 3:
                    menuPadaria();   // Acesso ao menu da Padaria
                    break;
                case 4:
                    menuPagamento(); // Acesso ao menu de Pagamento
                    break;
                case 5:
                    menuCaixa();     // Acesso ao menu do Caixa
                    break;
                case 6:
                    menuCancelar();  // Cancelar venda atual
                    break;
                case 7:
                    //=== VERIFICA SE EXISTE CAIXA ABERTO ANTES DE SAIR ===//
                    if (caixaAberto == true) {
                        printMenu(404);
                        printf("|\t-> !! ---- CAIXA ABERTO ---- !! <-\t|\n");
                        printf("|=======================================================|\n");
                        delay(1);
                        for (int j = 0; j <= 2; j++) {
                            printf(".");
                            delay(0.5);
                        }
                        menuPrincipal();
                    } else {
                        printf("ENCERRANDO O SISTEMA");
                        delay(1);
                        for (int j = 0; j <= 2; j++) {
                            printf(".");
                            delay(0.5);
                        }
                        exit(0);
                    }
                default:
                    printMenu(404);
                    printf("|\t-> !! ---- OPCAO INVALIDA ---- !! <-\t|\n");
                    printf("|=======================================================|\n");
                    delay(2);
                    menuPrincipal();
            }
        }
    }
}

//==== FUNÇÃO MENU LIMPEZA ====//
//== Responsável por mostrar os produtos de limpeza e adicionar ao carrinho ==//
void menuLimpeza() {
    float qnt;

    //==== PRINTA O MENU LIMPEZA (1) ====//
    printMenu(1);
    scanf("%d", &opcao);

    if (opcao >= 11 && opcao <= 17) {
        linha = opcao - 11; // Identifica a linha correta da struct

        printf("\n%s\n", limpeza[linha][0].nomeProduto);

        //==== VERIFICA SE HÁ ESTOQUE ====//
        if (limpeza[linha][0].estoqueProduto == 0) {
            printMenu(404);
            printf("|\t-> !! ---- PRODUTO SEM ESTOQUE ---- !! <- \t|\n");
            printf("|=======================================================|\n");
            delay(1);
            for (int j = 0; j <= 2; j++) {
                printf(".");
                delay(0.5);
            }
            menuLimpeza();
        }

        printf("Inserir quantidade: ");
        scanf("%f", &qnt);

        //==== VERIFICA SE HÁ ESTOQUE SUFICIENTE ====//
        while (qnt > limpeza[linha][0].estoqueProduto) {
            printMenu(404);
            printf("| -> ! - ESTOQUE INSUFICIENTE,  DIGITE NOVAMENTE - ! <- |\n");
            printf("|=======================================================|\n");
            delay(1);
            printf("Opcao..:  ");
            scanf("%f", &qnt);
        }

        printf("%.2f x %s adicionado ao carrinho.\n", qnt, limpeza[linha][0].nomeProduto);
        delay(3);

        //==== ATUALIZA ESTOQUE E VALORES ====//
        limpeza[linha][0].estoqueProduto -= qnt;
        vLimpeza += limpeza[linha][0].valorProduto * qnt;
        vCar += limpeza[linha][0].valorProduto * qnt;

        menuLimpeza();
    }
    else if (opcao == 18) {
        printf("\nVoltar ao menu principal\n");
        delay(2);
    }
    else {
        printMenu(404);
        printf("|\t-> !! ---- OPCAO INVALIDA ---- !! <-\t|\n");
        printf("|=======================================================|\n");
        delay(2);
        menuLimpeza();
    }
}

//==== FUNÇÃO MENU ALIMENTO ====//
//== Responsável por mostrar os produtos de alimentos e adicionar ao carrinho ==//
void menuAlimento() {
    float qnt;

    //==== PRINTA O MENU ALIMENTOS (2) ====//
    printMenu(2);

    scanf("%d", &opcao);

    if (opcao >= 21 && opcao <= 27) {
        linha = opcao - 21;
        printf("\n%s:\n", alimentos[linha][0].nomeProduto);

        //==== VERIFICA SE HÁ ESTOQUE ====//
        if (alimentos[linha][0].estoqueProduto == 0) {
            printMenu(404);
            printf("|\t-> !! ---- PRODUTO SEM ESTOQUE ---- !! <- \t|\n");
            printf("|=======================================================|\n");
            delay(1);
            for (int j = 0; j <= 2; j++) {
                printf(".");
                delay(0.5);
            };
            menuAlimento();
        }

        printf("Inserir quantidade: ");
        scanf("%f", &qnt);

        //==== VERIFICA SE HÁ ESTOQUE SUFICIENTE ====//
        while (qnt > alimentos[linha][0].estoqueProduto) {
            printMenu(404);
            printf("| -> ! - ESTOQUE INSUFICIENTE,  DIGITE NOVAMENTE - ! <- |\n");
            printf("|=======================================================|\n");
            delay(1);
            printf("Opcao..:  ");
            scanf("%f", &qnt);
        }

        printf("%.2f x %s adicionado ao carrinho.\n", qnt, alimentos[linha][0].nomeProduto);
        delay(3);

        //==== ATUALIZA ESTOQUE E VALORES ====//
        alimentos[linha][0].estoqueProduto -= qnt;
        vAlimento += alimentos[linha][0].valorProduto * qnt;
        vCar += alimentos[linha][0].valorProduto * qnt;

        menuAlimento();
    }
    else if (opcao == 28) {
        printf("\nVoltar ao menu principal\n");
        delay(2);
    }
    else {
        printMenu(404);
        printf("|\t-> !! ---- OPCAO INVALIDA ---- !! <-\t|\n");
        printf("|=======================================================|\n");
        delay(2);
        menuAlimento();
    }
}

//==== FUNÇÃO MENU PADARIA ====//
//== Responsável por mostrar os produtos da padaria e adicionar ao carrinho ==//
void menuPadaria() {
    float qnt;

    //==== PRINTA O MENU PADARIA (3) ====//
    printMenu(3);

    scanf("%d", &opcao);

    if (opcao >= 31 && opcao <= 37) {
        linha = opcao - 31; // Identifica a linha correta da struct
        printf("\n%s:\n", padaria[linha][0].nomeProduto);

        //==== VERIFICA SE HÁ ESTOQUE ====//
        if (padaria[linha][0].estoqueProduto == 0) {
            printMenu(404);
            printf("|\t-> !! ---- PRODUTO SEM ESTOQUE ---- !! <- \t|\n");
            printf("|=======================================================|\n");
            delay(1);
            for (int j = 0; j <= 2; j++) {
                printf(".");
                delay(0.5);
            };
            menuPadaria();
        }

        printf("Inserir quantidade: ");
        scanf("%f", &qnt);

        //==== VERIFICA SE HÁ ESTOQUE SUFICIENTE ====//
        while (qnt > padaria[linha][0].estoqueProduto) {
            printMenu(404);
            printf("| -> ! - ESTOQUE INSUFICIENTE,  DIGITE NOVAMENTE - ! <- |\n");
            printf("|=======================================================|\n");
            delay(1);
            printf("Opcao..:  ");
            scanf("%f", &qnt);
        }

        printf("%.2f x %s adicionado ao carrinho.\n", qnt, padaria[linha][0].nomeProduto);
        delay(3);

        //==== ATUALIZA ESTOQUE E VALORES ====//
        padaria[linha][0].estoqueProduto -= qnt;
        vPadaria += padaria[linha][0].valorProduto * qnt;
        vCar += padaria[linha][0].valorProduto * qnt;

        menuPadaria();
    }
    else if (opcao == 38) {
        printf("\nVoltar ao menu principal\n");
        delay(2);
    }
    else {
        printMenu(404);
        printf("|\t-> !! ---- OPCAO INVALIDA ---- !! <-\t|\n");
        printf("|=======================================================|\n");
        delay(2);
        menuPadaria();
    }
}


//==== FUNÇÃO MENU PAGAMENTO ====//
//== Responsável por processar pagamentos em dinheiro ou cartão ==//
void menuPagamento() {

    //==== VARIÁVEIS DE CONTROLE DO PAGAMENTO ====//
    float descontoReal = 0;
    float descontoCinco = 0.05;
    float descontoDez = 0.10;
    float descontoInformado = 0;
    float descontoLimpeza = 0, descontoAlimento = 0, descontoPadaria = 0;

    //==== CALCULA VALOR TOTAL SE NÃO HOUVER DÍVIDA ====//
if (!deve && fPagar == 0) {
    vTotal = vLimpeza + vPadaria + vAlimento;
} else {
    printf("|=======================================================|\n");
    printf("|\t#\t!!! ----- ALERTA ----- !!!\t#\t|\n");
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("|\t\t ! - Pagamento Parcial Detectado - ! \t|\n");
    printf("|\t\t Faltam: R$ %.2f \t|\n", vTotal);
}

printMenu(4); // 4 == Menu de Pagamento
scanf("%d", &opcao);

switch (opcao) {

//==== PAGAMENTO EM DINHEIRO ====//
case 41:
    if (!deve && fPagar == 0) {
        printf("\nValor total: %.2f R$\n", vTotal);

        if (vTotal <= 50) {
            printf("Desconto de 5%% aplicado.\n");
            descontoReal = vTotal * descontoCinco;
            descontoLimpeza = vLimpeza * descontoCinco;
            descontoAlimento = vAlimento * descontoCinco;
            descontoPadaria = vPadaria * descontoCinco;
        } else if (vTotal < 200) {
            printf("Desconto de 10%% aplicado.\n");
            descontoReal = vTotal * descontoDez;
            descontoLimpeza = vLimpeza * descontoDez;
            descontoAlimento = vAlimento * descontoDez;
            descontoPadaria = vPadaria * descontoDez;
        } else {
            printf("Informe a porcentagem de desconto: ");
            scanf("%f", &descontoInformado);
            if(descontoInformado <= 50){
                printf("Desconto de %.0f%% aplicado.\n", descontoInformado);
                descontoInformado /= 100;
                descontoReal = vTotal * descontoInformado;
                descontoLimpeza = vLimpeza * descontoInformado;
                descontoAlimento = vAlimento * descontoInformado;
                descontoPadaria = vPadaria * descontoInformado;
            } else {
                printMenu(404);
                printf("|\t !! - DESCONTO MAIOR QUE 50%% - !!\t|\n");
                descontoInformado = 0;
                delay(2);
                menuPagamento();
            }
        }

        vTcdesconto = vTotal - descontoReal;
        vTcdesconto = truncar(vTcdesconto, 2);
        printf("\nTotal com desconto: %.2f R$\n", vTcdesconto);
    }

    printf("Valor recebido: ");
    scanf("%f", &recebido);

    if (!deve) {
        vPagar = vTcdesconto;             // Sem desconto se já havia dívida
    } else {
        vPagar = fPagar;
    }

    //==== PAGAMENTO TOTAL COM SUCESSO ====//
    if (recebido >= vPagar) {
        troco = recebido - vPagar;
        totalDia += vPagar;

        system("cls");
        printf("Processando pagamento");
        delay(1);
        for (int j = 0; j < 3; j++) {
            printf(".");
            delay(1);
        }

        system("cls");
        printf("Troco: %.2f R$\n", troco);
        printf("Pagamento realizado com sucesso!\n");
        delay(2);

        tLimpeza += vLimpeza - descontoLimpeza;
        tAlimento += vAlimento - descontoAlimento;
        tPadaria += vPadaria - descontoPadaria;

        vLimpeza = vAlimento = vPadaria = vCar = 0;
        descontoLimpeza = descontoAlimento = descontoPadaria = 0;
        fPagar = 0;
        deve = false;

        menuPrincipal();
        break;
    }

    //==== PAGAMENTO PARCIAL ====//
    else {
    fPagar = vTotal - recebido;
    fPagar = truncar(fPagar, 2);

    totalDia += recebido;
    deve = true;

    while (fPagar > 0) {
        system("cls");
        printf("Pagamento parcial registrado.\nSera cobrado o valor integral: %.2f R$\n", vTotal);
        printf("Falta pagar: %.2f R$\n\n", fPagar);
        delay(1);

        printf("Efetuar o restante do pagamento em dinheiro?\n\n");
        printf("1: Sim\n");
        printf("2: Voltar ao menu de pagamento\n");
        scanf("%d", &opcao2);

        if (opcao2 != 1) {
            printf("Redirecionando para o menu de pagamento...\n");
            delay(2);
            vCar = fPagar;
            menuPagamento(); // retorna sem finalizar pagamento
            break;
        }

        printf("Digite novo valor recebido: ");
        scanf("%f", &recebido);

        if (recebido >= fPagar) {
            troco = recebido - fPagar;
            totalDia += fPagar;
            fPagar = 0;

            system("cls");
            printf("Processando pagamento");
            delay(1);
            for (int j = 0; j < 3; j++) {
                printf(".");
                delay(1);
            }

            system("cls");
            printf("Troco: %.2f R$\n", troco);
            printf("Pagamento realizado com sucesso!\n");
            delay(2);

            tLimpeza += vLimpeza;
            tAlimento += vAlimento;
            tPadaria += vPadaria;

            vLimpeza = vAlimento = vPadaria = vCar = 0;
            descontoLimpeza = descontoAlimento = descontoPadaria = 0;
            deve = false;

            menuPrincipal();
            break;
        } else {
            fPagar -= recebido;
            fPagar = truncar(fPagar, 2);
            totalDia += recebido;
            vCar = fPagar;
            recebido = 0;
            printf("Valor insuficiente. Faltam %.2f R$\n", fPagar);
            delay(2);
        }
    }
}
        //==== PAGAMENTO EM CARTÃO ====//
        case 42:
            if(deve == false){
            printf("Valor total: %.2f R$\n\n", vTotal);
            }else{
            printf("Falta pagar: %.2f R$\n\n", fPagar);}
            printf("51. Pagamento realizado\n");
            printf("52. Pagamento nao realizado\n");
            printf("53. Voltar ao menu anterior\n");
            scanf("%d", &opcao2);

            if (opcao2 == 51) {
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for (int j = 0; j <= 2; j++) {
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("Pagamento realizado com sucesso!\n");
                delay(2);
                if(deve == false){
                totalDia += vTotal;
                }else{
                totalDia += fPagar;}
                tLimpeza += vLimpeza;
                tAlimento += vAlimento;
                tPadaria += vPadaria;
                vLimpeza = vAlimento = vPadaria = fPagar = vCar = 0;
            }

            else if (opcao2 == 52) {
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for (int j = 0; j <= 2; j++) {
                    printf(".");
                    delay(1);
                }
                system("cls");
                printf("\nPagamento nao realizado!\n");
                delay(3);
                menuPagamento();
            }

            else if (opcao2 == 53) {
                printf("\nVoltar ao menu anterior\n");
                delay(2);
                menuPagamento();
            }

            else {
                printf("\nOpcao invalida... Tente novamente\n");
                delay(2);
            }
            break;

        //==== VOLTAR AO MENU PRINCIPAL ====//
        case 43:
            printf("\nVoltar ao menu principal\n");
            menuPrincipal();
            break;

        //==== OPÇÃO INVÁLIDA ====//
        default:
            printf("\nOpcao invalida... Tente Novamente\n");
            delay(2);
            menuPagamento();
            break;
    }
}

//==== FUNÇÃO MENU CAIXA ====//
//== Gerencia a abertura e fechamento do caixa ==//
void menuCaixa() {

    printMenu(5); // 5 == Menu Caixa
    scanf("%d", &opcao);

    switch (opcao) {

        //==== ABERTURA DO CAIXA ====//
        case 1: {
            if (caixaAberto == true) {
                printMenu(404);
                printf("|\t-> !! ---- CAIXA ABERTO ---- !! <-\t|\n");
                printf("|=======================================================|\n");
                delay(1);
                for (int j = 0; j <= 2; j++) {
                    printf(".");
                    delay(0.5);
                }
                menuCaixa();
            } else {
                printMenu(6); // 6 == Abertura do Caixa
                fflush(stdout);
                scanf("%d", &opcao2);
                printf("|=======================================================|\n");

                switch (opcao2) {

                    case 1:
                        printf("|\t@ ---- Valor de Abertura do Caixa ---- @\t|\n");
                        printf("| -> R$ ");
                        scanf("%f", &vAbre);

                        while (vAbre <= 0) {
                            printMenu(404);
                            printf("|   -> !! ---- ERRO, INFORME UM NOVO VALOR ---- !! <-\t|\n");
                            printf("|=======================================================|\n");
                            printf("Opcao..:  ");
                            scanf("%f", &vAbre);
                        }

                        system("cls");
                        printf("Validando abertura de caixa");
                        delay(0.5);
                        for (int j = 0; j <= 2; j++) {
                            printf(".");
                            delay(0.5);
                        }

                        system("cls");
                        printf("Caixa aberto com sucesso!!!\n");
                        delay(1);
                        printf("Valor de abertura de caixa: R$ %.2f\n", vAbre);
                        delay(1);
                        caixaAberto = true;
                        system("cls");
                        abrePadaria(); // Abre o estoque da padaria
                        menuPrincipal();
                        break;

                    case 2:
                        menuCaixa();
                        break;

                    default:
                        printMenu(404);
                        printf("|\t-> !! ---- OPCAO INVALIDA ---- !! <-\t|\n");
                        printf("|=======================================================|\n");
                        delay(1);
                        menuPrincipal();
                        break;
                }
            }
        }

        //==== FECHAMENTO DO CAIXA ====//
        case 2: {
            printMenu(7);
            scanf("%d", &opcao2);

            switch (opcao2) {

                case 1:
                    if (totalDia <= 0) {
                        totalDia = 0;
                    }

                    printMenu(8);

                    totalDia = tLimpeza = tAlimento = tPadaria = 0;
                    caixaAberto = false;
                    reset();
                    delay(5);
                    break;

                case 2:
                    menuPrincipal();
                    break;

                default:
                    printMenu(404);
                    printf("|\t-> !! ---- OPCAO INVALIDA ---- !! <-\t|\n");
                    printf("|=======================================================|\n");
                    delay(1);
                    menuPrincipal();
                    break;
            }
                case 3:
                    menuPrincipal();
                    break;

                default:
                    printMenu(404);
                    printf("|\t-> !! ---- OPCAO INVALIDA ---- !! <-\t|\n");
                    printf("|=======================================================|\n");
                    delay(1);
                    menuPrincipal();
                    break;
    }
}
}

//==== FUNÇÃO ABERTURA PADARIA ====//
//== Solicita a quantidade inicial de estoque da padaria ==//
void abrePadaria() {
    printf("Abertura padaria");
    delay(0.5);
    for (int j = 0; j <= 2; j++) {
        printf(".");
        delay(0.5);
    }
    system("cls");

    //==== ABERTURA PADARIA ====//
    printMenu(9);

    for (int i = 0; i < 7; i++) {
        printf("| %s: ", padaria[i][0].nomeProduto);
        scanf("%d", &padaria[i][0].estoqueProduto);
    }
    delay(1);
}

//==== FUNÇÃO CANCELAR VENDA ====//
//== Cancela a venda atual e zera os totais acumulados ==//
void menuCancelar() {
    system("cls");
    printf("Deseja cancelar a venda?\n");
    printf("1 - SIM\n");
    printf("2 - NAO\n");

    scanf("%d", &opcao);
    system("cls");

    switch (opcao) {
        case 1:
            vPadaria = 0;
            vAlimento = 0;
            vLimpeza = 0;
            vCar = 0;
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


//==== FUNÇÃO MAIN ====//
//== Ponto de entrada do programa ==//
int main() {
    system("color 0C"); // Estilo: Vermelho
    menuPrincipal();    // Chamada da função principal
    return 0;
}
