@ -0,0 +1,1143 @@
#include <stdio.h>
#include <math.h> //Bibliotecas
#include <stdlib.h>
#include <time.h>

/* Funcoes e variaveis globais */
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

float vLimpeza = 0, vPadaria = 0, vAlimento = 0, totalDia =0; // Variaveis globais
float fPagar = 0;
float vCar = 0;
float vAbertura = 0;
int totalDetergente = 50, totalsabPo = 30, totalEsponja = 0, totalAmaciante = 20, totalBucha = 100, totalDesinfetante = 15, totalSabao = 60;
int totalCafe = 10, totalLeite = 15, totalArroz = 10, totalFeijao = 5, totalAcucar = 20, totalSal = 6, totalFarinha = 15;
int tpaoForma, tpaoIntegral, tpaoFrances, tSonho, tBiscoito, tpaoDoce, tSalgado;

void delay(int delayemSegundos){    // Funcao utilizando a biblioteca "<time.h>" para delay.
    int mili = 1000 * delayemSegundos;
    clock_t comecoT = clock();
    while (clock() < comecoT + mili);
}

void menuPrincipal(){ // Funcao do Menu Principal

    int opcaoUm=0; // Variavel opcao do usuario

    while (opcaoUm!=8)
    {
        system("cls");
        printf("\n   <Menu Principal>\n"); //Print do Menu
        printf("1. Material de Limpeza\n");
        printf("2. Alimentos\n");
        printf("3. Padaria\n");
        printf("4. Pagamento\n");
        printf("5. Abertura de caixa\n");
        printf("6. Fechamento de caixa\n");
        printf("7. Cancelar venda\n");
        printf("8. Sair\n");
        printf("Valor total carrinho: %.2f R$", vCar);

        printf("\nValor total de vendas hoje: %.2f  R$ \n\n",totalDia); // Atualizacao do valor total das vendas

        scanf("%d", &opcaoUm);

        if (opcaoUm == 5) {
          menuAbertura();
        }

        else if(vAbertura <= 0){
            system("cls");
                printf("O caixa esta fechado, realize a abertura");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                printf(".");
                delay(1);}
            menuPrincipal();

        } else {
            switch(opcaoUm) { // Switch Case para as sessoes do mercadinho
                case 1: // 47 até 51 === VERIFICAÇÃO CAIXA ABERTO CASO ESTIVER FECHADO SOLICITA ABERTURA
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
                    printf("Encerrando o sistema...");
                    exit (0);
                default:
                    printf("Opcao invalida... Tente novamente\n");
                    menuPrincipal();
            }
        }
    }
}

void menuLimpeza(){ // Funcao Menu Limpeza
    int cLimpeza; // Opcao do usuario
    float pDetergente, pSabaopo, pAmaciante, pBucha, pSabao, pDesinfetante; /* <<< PRODUTOS DE LIMPEZA */
    float qnt;
        /* <<<<<<< VALORES DOS PRODUTOS >>>>>>> */
            /* LIMPEZA */
    pDetergente = 1.99;
    pSabaopo = 8.99;
    pAmaciante = 15.00; // Valores dos produtos
    pBucha = 4.99;
    pSabao = 1.00;
    pDesinfetante = 7.99;

    system("cls"); //Funcao de limpar tela (CLEAR SYSTEM)
    printf("  <Material de limpeza>\n");
    printf("11. Detergente                  R$ 1,99    (%d) \n", totalDetergente);
    printf("12. Sabao em po 1kg             R$ 8,99    (%d) \n", totalsabPo);
    printf("13. Esponja                     R$ 1,50    (%d) \n", totalEsponja);
    printf("14. Amaciante 1lt               R$ 15,00   (%d) \n", totalAmaciante);   // Menu Limpeza
    printf("15. Bucha de Pia (Kit c/3)      R$ 4.99    (%d) \n", totalBucha);
    printf("16. Desinfetante (1Lt)          R$ 7.99    (%d) \n", totalDesinfetante);
    printf("17. Sabao em Barra (UND)         R$ 1.00    (%d) \n", totalSabao);
    printf("18. Voltar ao menu principal\n");
    printf("Carrinho materiais de limpeza: %.2f\n", vLimpeza);

    scanf("%d", &cLimpeza);

    switch(cLimpeza){ // Switch case para selecao dos produtos
        case 11:    /*<----- DETERGENTE */
            printf("\nDetergente: \n");
            if (totalDetergente == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalDetergente) {
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Detergente adicionado ao carrinho.\n",qnt);
            delay(3);
            totalDetergente = totalDetergente - qnt;
            vLimpeza += pDetergente * qnt;
            vCar += pDetergente * qnt;
            menuLimpeza();
            break;

        case 12:    /*<----- SABAO EM PO */
            printf("\nSabao em po: \n");
            if (totalsabPo == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalsabPo) {
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Sabao em po adicionado ao carrinho.\n",qnt);
            delay(3);
            totalsabPo = totalsabPo - qnt;
            vLimpeza += pSabaopo * qnt;
            vCar += pSabaopo * qnt;
            menuLimpeza();
            break;

        case 13:    /*<----- ESPONJA*/
            printf("\nEsponja: \n");
            printf("Infelizmente o item nao registra estoque, digite um novo item\n");
            delay(3);
            menuLimpeza();
            break;

        case 14:    /*<----- AMACIANTE*/
            printf("\nAmaciante: \n");
            if (totalAmaciante == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalAmaciante){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Amaciante adicionado ao carrinho.\n",qnt);
            delay(3);
            totalAmaciante = totalAmaciante - qnt;
            vLimpeza += pAmaciante * qnt;
            vCar += pAmaciante * qnt;
            menuLimpeza();
            break;

        case 15:    /*<----- BUCHA DE PIA*/
            printf("Bucha de Pia (Kit c/3): \n");
            if (totalBucha == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalBucha){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Bucha de Pia adicionada ao carrinho.\n",qnt);
            delay(3);
            totalBucha = totalBucha - qnt;
            vLimpeza += pBucha * qnt;
            vCar += pBucha * qnt;
            menuLimpeza();
            break;

        case 16:    /*<----- DESINFETANTE*/
            printf("\n");
            printf("Desinfetante (1LT): \n");
            if (totalDesinfetante == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }

            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalDesinfetante){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }

            printf("%.2f x Desinfetante adicionado ao carrinho.\n",qnt);
            delay(3);
            totalDesinfetante = totalDesinfetante - qnt;
            vLimpeza += pDesinfetante * qnt;
            vCar += pDesinfetante * qnt;
            menuLimpeza();
            break;

        case 17:    /*<----- SABAO EM BARRA*/
            printf("\n");
            printf("Sabao em Barra (UN): \n");
            if (totalSabao == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuLimpeza();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalSabao){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sabao em Barra adicionado ao carrinho.\n",qnt);
            delay(3);
            totalSabao = totalSabao - qnt;
            vLimpeza += pSabao * qnt;
            vCar += pSabao * qnt;
            menuLimpeza();
            break;

        case 18:    /*<----- VOLTAR*/
            printf("\n");
            printf("Voltar ao menu principal\n");
            break;

        default:
            printf("\n");
            printf("Opcao invalida... Tente Novamente\n");
            delay(3);
            menuLimpeza();
            break;
        }
}

void menuAlimento(){ // Funcao Menu Alimentos
    int cAlimento;
    float pCafe, pFeijaop, pAcucar, pLeite, pArroz, pSal, pFarinha; /* <<< ALIMENTOS */
    float qnt;
            /* ALIMENTOS */
    pCafe = 19.99;
    pLeite = 5.90;
    pArroz = 4.50; // Valores dos produtos
    pFeijaop = 8.00;
    pAcucar = 5.00;
    pSal = 2.00;
    pFarinha = 5.00;

    system("cls");
    printf("   <Alimentos>\n");
    printf("21. Cafe                    R$ 19,99        (%d)\n", totalCafe);
    printf("22. Leite Cx                R$ 5,90         (%d)\n", totalLeite);
    printf("23. Arroz 1KG               R$ 4,50         (%d)\n", totalArroz);
    printf("24. Feijao Preto 1KG        R$ 8,00         (%d)\n", totalFeijao);
    printf("25. Acucar 1KG              R$ 5,00         (%d)\n", totalAcucar);
    printf("26. Sal(1KG)                R$ 2,00         (%d)\n", totalSal);
    printf("27. Farinha de Trigo (1kg)  R$ 5.00         (%d)\n", totalFarinha);
    printf("28. Voltar ao menu principal\n");
    printf("Carrinho alimentos: %.2f\n", vAlimento);

    scanf("%d", &cAlimento);

    switch(cAlimento){ // Switch case para selecao dos produtos
        case 21:    /*<----- CAFE */
            printf("\nCafe:\n");
            if (totalCafe == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalCafe){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Cafe adicionado ao carrinho.\n",qnt);
            delay(3);
            totalCafe = totalCafe - qnt;
            vAlimento += pCafe * qnt;
            vCar += pCafe * qnt;
            menuAlimento();
            break;

        case 22:    /*<----- LEITE */
            printf("\n");
            printf("\nLeite:\n");
            if (totalLeite == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalLeite){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Leite adicionado ao carrinho.\n",qnt);
            delay(3);
            totalLeite = totalLeite - qnt;
            vAlimento += pLeite * qnt;
            vCar += pLeite * qnt;
            menuAlimento();
            break;

        case 23:    /*<----- ARROZ*/
            printf("Arroz 1kg:\n");
            if (totalArroz == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalArroz){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Arroz 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            totalArroz = totalArroz - qnt;
            vAlimento += pArroz * qnt;
            vCar += pArroz * qnt;
            menuAlimento();
            break;

        case 24:    /*<----- FEIJAO PRETO*/
            printf("\nFeijao Preto 1kg:\n");
            if (totalFeijao == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalFeijao){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Feijao preto 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            totalFeijao = totalFeijao - qnt;
            vAlimento += pFeijaop * qnt;
            vCar += pFeijaop * qnt;
            menuAlimento();
            break;

        case 25:    /*<----- ACUCAR*/
            printf("\nAcucar 1kg:\n");
            if (totalAcucar == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalAcucar){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Acucar 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            totalAcucar = totalAcucar - qnt;
            vAlimento += pAcucar * qnt;
            vCar += pAcucar * qnt;
            menuAlimento();
            break;

        case 26:    /*<----- ACUCAR*/
            printf("\nSal 1kg:\n");
            if (totalSal == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalSal){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sal 1kg adicionado ao carrinho.\n",qnt);
            delay(3);
            totalSal = totalSal - qnt;
            vAlimento += pSal * qnt;
            vCar += pSal * qnt;
            menuAlimento();
            break;

        case 27:    /*<----- ACUCAR*/
            printf("\nFarinha de Trigo 1kg:\n");
            if (totalFarinha == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuAlimento();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > totalFarinha){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Farinha de Trigo 1kg adicionada ao carrinho.\n",qnt);
            delay(3);
            totalFarinha = totalFarinha - qnt;
            vAlimento += pFarinha * qnt;
            vCar += pFarinha * qnt;
            menuAlimento();
            break;

        case 28:    /*<----- VOLTAR*/
            printf("\nVoltar ao menu principal\n");
            break;

        default:
            printf("\nOpcao invalida... Tente Novamente\n");
            menuAlimento();
            break;
        }
}

void menuPadaria(){ // Funcao Menu Padaria
    int cPadaria;
    float pPaoforma, pPaointegral, pSonho, pPaofrances, pBiscoito, pPaodoce, pSalgado; /* <<< PADARIA */
    float qnt;
    /* PADARIA */
    pPaoforma = 9.50;
    pPaointegral = 12.50; // Valores dos produtos
    pPaofrances = 1.90;
    pSonho = 8.50;
    pBiscoito = 12.50;
    pPaodoce = 2.50;
    pSalgado = 17.50;

    system("cls");
    printf("        <Padaria>  \n");
    printf("31. Pao de Forma (PCT)  R$ 9,50         (%d)\n", tpaoForma);
    printf("32. Pao Integral (PCT)  R$ 12,50        (%d)\n", tpaoIntegral);
    printf("33. Pao Frances (UND)   R$ 1,90         (%d)\n", tpaoFrances);
    printf("34. Sonho (UND)         R$ 8,50         (%d)\n", tSonho);
    printf("35. Biscoito (KG)       R$ 12,50        (%d)\n", tBiscoito);
    printf("36. Pao Doce (UND)      R$ 2,50         (%d)\n", tpaoDoce);
    printf("37. Salgado (UND)       R$ 17,50        (%d)\n", tSalgado);
    printf("38. Voltar ao menu principal\n");
    printf("Carrinho padaria: %.2f", vPadaria);
    printf("\n");

    scanf("%d", &cPadaria);

    switch(cPadaria){ // Switch case para selecao dos protudos
        case 31:   /*<----- PAO DE FORMA */
            printf("\nPao de Forma(PCT):\n");
            if (tpaoForma == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > tpaoForma){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao de Forma adicionado ao carrinho.\n",qnt);
            delay(2);
            tpaoForma = tpaoForma - qnt;
            vPadaria += pPaoforma * qnt;
            vCar += pPaoforma * qnt;
            menuPadaria();
            break;

        case 32:    /*<----- PAO INTEGRAL */
            printf("\nPao Integral(PCT):\n");
            if (tpaoIntegral == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > tpaoIntegral){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao integral adicionado ao carrinho.\n",qnt);
            delay(2);
            tpaoIntegral = tpaoIntegral - qnt;
            vPadaria += pPaointegral * qnt;
            vCar += pPaointegral * qnt;
            menuPadaria();
            break;

        case 33:    /*<----- PAO FRANCES*/
            printf("\nPao Frances(UND):\n");
            if (tpaoFrances == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > tpaoFrances){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao frances adicionado ao carrinho.\n",qnt);
            delay(2);
            tpaoFrances = tpaoFrances - qnt;
            vPadaria += pPaofrances * qnt;
            vCar += pPaofrances * qnt;
            menuPadaria();
            break;

        case 34:    /*<----- SONHO*/
            printf("\nSonho(UND):\n");
            if (tSonho == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > tSonho){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Sonho adicionado ao carrinho.\n",qnt);
            delay(2);
            tSonho = tSonho - qnt;
            vPadaria += pSonho * qnt;
            vCar += pSonho * qnt;
            menuPadaria();
            break;

        case 35:    /*<----- SONHO*/
            printf("\nBiscoito(KG): \n");
            if (tBiscoito == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > tBiscoito){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Biscoito adicionado ao carrinho.\n",qnt);
            delay(2);
            tBiscoito = tBiscoito - qnt;
            vPadaria += pBiscoito * qnt;
            vCar += pBiscoito * qnt;
            menuPadaria();
            break;

        case 36:    /*<----- SONHO*/
            printf("\nPao Doce(UND):\n");
            if (tpaoDoce == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > tpaoDoce){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Pao Doce adicionado ao carrinho.\n",qnt);
            delay(2);
            tpaoDoce = tpaoDoce - qnt;
            vPadaria += pPaodoce * qnt;
            vCar += pPaodoce * qnt;
            menuPadaria();
            break;

        case 37:    /*<----- SONHO*/
            printf("\nSalgado(UND):\n");
            if (tSalgado == 0) {
                printf("Infelizmente o item nao registra estoque, digite um novo item\n");
                delay(3);
                menuPadaria();
                break;
            }
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            while (qnt > tSalgado){
                printf("Quantidade insuficiente, digite um novo valor: \n");
                delay(3);
                scanf("%f", &qnt);
            }
            printf("%.2f x Salgado adicionado ao carrinho.\n",qnt);
            delay(2);
            tSalgado = tSalgado - qnt;
            vPadaria += pSalgado * qnt;
            vCar += pSalgado * qnt;
            menuPadaria();
            break;

        case 38:    /*<----- VOLTAR*/
            printf("\nVoltar ao menu principal\n");
            break;

        default:
            printf("\nOpcao invalida... Tente Novamente\n");
            menuPadaria();
            break;
    }
}

void menuPagamento() // Funcao Menu Pagamento
{
    int cPagamento, cCartao;

    /* <<< VARIAVEIS DE ARMAZENAMENTO TEMPORARIO */
    float vTotal = 0, vTcdesconto = 0, qnt = 0, troco = 0, valorCobrado = 0, descontoReal =0;

    /* <<< VARIAVEIS DE ARMAZENAMENTO FIXO */
    float descontoCinco, descontoDez, descontoDezoito ;

        /* DESCONTOS */
    descontoCinco = 0.05;   /* <<<- 5% desconto */
    descontoDez = 0.10;  /* <<<- 10% desconto */
    descontoDezoito = 0.18;  /* <<<- 18% desconto */

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
                descontoReal = vTotal * descontoCinco;
            }

            else if (vTotal > 50 && vTotal < 100)
            {
                descontoReal = vTotal * descontoDez;
            }

            else if (vTotal >=100)
            {
                    descontoReal = vTotal * descontoDezoito;
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
                delay(1);}
                system("cls");
                printf("\nTroco: %.2f R$\n", troco);
                printf("Pagamento realizado com sucesso!\n");
                delay(3);
                vLimpeza= 0;
                vAlimento= 0;
                vPadaria= 0;
                descontoReal = 0;
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
                delay(1);}
                system("cls");
                printf("Pagamento realizado com sucesso!\n");
                delay(3);
                vLimpeza = 0;
                vAlimento = 0;
                vPadaria = 0;
                descontoReal = 0;
                fPagar = 0;
                vCar = 0;/*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                break;
            } else {
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                printf(".");
                delay(1);}
                system("cls");
                printf("O valor integral sera cobrado!\n");
                delay(4);
                system("cls");
                fPagar = vTotal - valorCobrado;
                totalDia= totalDia + valorCobrado;
                printf("Falta pagar %.2f R$", fPagar);
                delay(3);
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
                delay(1);}
                system("cls");
                printf("Pagamento realizado com sucesso!\n");
                delay(3);
                totalDia = totalDia + vTotal; /* <----------- Soma do total di�rio */
            }

            else if (cCartao == 52)
            {
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                printf(".");
                delay(1);}
                system("cls");
                printf("\nPagamento nao realizado!\n");
                delay(3);
                menufPagamento();
            }

            else if (cCartao == 53)
            {
                printf("\nVoltar ao menu anterior\n");
                menuPagamento();
            }

            else
            {
                printf("\n");
                printf("Opcao invalida... Tente novamente\n");
            }

            vPadaria = 0; vAlimento= 0; vLimpeza =0; vCar = 0;  /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
            break;

        case 43:
            printf("\nVoltar ao menu principal\n");
            menuPrincipal();
            break;

        default:
            printf("\nOpcao invalida... Tente Novamente\n");
            menuPagamento();
            break;
    }
}

void menufPagamento() //FUNCAO CRIADA PARA CASO A PESSOA NAO DE DINHEIRO SUFICIENTE NO PAGAMENTO NO MENUPRINCIPAL
{
    int cPagamento= 0, cCartao= 0;
    float troco = 0, valorCobrado = 0; /* <<< VARIAVEIS DE ARMAZENAMENTO TEMPOR�RIO */

    float descontoCinco, descontoDez, descontoDezoito ; /* <<< VARIAVEIS DE ARMAZENAMENTO FIXO */
                /* DESCONTOS */
    descontoCinco = 0.05;   /* <<<- 5% desconto */
    descontoDez = 0.10;  /* <<<- 10% desconto */
    descontoDezoito = 0.18;  /* <<<- 18% desconto */

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
                delay(1);}
                system("cls");
                printf("\nTroco: %.2f R$\n", troco);
                printf("Pagamento realizado com sucesso!\n\n");
                delay(3);
                vLimpeza= 0;
                vAlimento= 0;
                vPadaria= 0;
                fPagar= 0;  /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
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
                delay(1);}
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
                delay(1);}
                system("cls");
                printf("\nPagamento realizado com sucesso!\n\n");
                delay(3);
                totalDia= totalDia + fPagar; /* <----------- Soma do total diario */
                menuPrincipal();
            }

            else if (cCartao == 52)
            {
                system("cls");
                printf("Processando pagamento");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                printf(".");
                delay(1);}
                system("cls");
                printf("\nPagamento nao realizado!\n");
                delay(3);
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

            vPadaria = 0; vAlimento= 0; vLimpeza =0; vCar=0;   /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
            break;

        case 43:
            printf("Voltar ao menu principal\n\n");
            menuPrincipal();
            break;

        default:
            printf("Opcao invalida... Tente Novamente\n\n");
            menufPagamento();
            break;
    }
}

void menuAbertura(){ // Funcao do Menu de abertura do caixa
    int cAbertura = 0;

    if (vAbertura > 0)
    {
        system("cls");
        printf("Existe um caixa em aberto, finalize para realizar uma nova abertura\n");
        for ( int i = 5; i != 0; i--) {
            printf("Retornando em %d segundos \n",i);
            delay(1);
        }
        menuPrincipal();

    } else {

        system("cls");
        printf("    <Abertura de caixa>\n");
        printf("1 - Inserir valor de abertura\n");
        printf("2 - Cancelar\n");
        scanf("%d",&cAbertura);

        switch (cAbertura){
            case 1:
                printf("Informe o valor: ");
                scanf("%f",&vAbertura);

               while (vAbertura <=0){
                printf ("\nErro.. digite um novo valor:\n");
                scanf ("%f", &vAbertura);}

               system("cls");
                printf("Validando abertura de caixa");
                delay(1);
                for ( int j = 0; j <= 2; j++){
                printf(".");
                delay(1);}
                system("cls");
                printf("Caixa aberto com sucesso!!!\n");
                delay(1);
                printf("Valor de abertura de caixa %.2f R$", vAbertura);
                delay(2);
                system("cls");
                abrePadaria();
                menuPrincipal();
                break;

            case 2:
                menuPrincipal();
                break;

            default:
                printf("Opcao invalida, tente novamente...");
                delay(2);
                menuAbertura();
                break;
        }
    }
}

void abrePadaria(){ // Funcao da Abertura da Padaria
    printf("Abertura padaria");
    delay(1);
    for ( int j = 0; j <= 2; j++){
    printf(".");
    delay(1);}
    system("cls");

    printf("Digite o estoque de Pao de Forma (pacote): ");
    scanf("%d", &tpaoForma);
    system("cls");
    printf("Digite o estoque de Pao Integral (pacote): ");
    scanf("%d", &tpaoIntegral);
    system("cls");
    printf("Digite o estoque de pao Frances (Unidade): ");
    scanf("%d", &tpaoFrances);
    system("cls");
    printf("Digite o estoque de Sonho (Unidade): ");
    scanf("%d", &tSonho);
    system("cls");
    printf("Digite o estoque de Biscoito (KG): ");
    scanf("%d", &tBiscoito);
    system("cls");
    printf("Digite o estoque de Pao Doce (Unidade): ");
    scanf("%d", &tpaoDoce);
    system("cls");
    printf("Digite o estoque de Salgado (Unidade): ");
    scanf("%d", &tSalgado);
    system("cls");
}

void menuFechamento() { // Funcao do menu de Fechamento do caixa
    int cFechamento = 0;
    float vFechamentoD = 0;
    float vFechamentoC = 0;
    float vFechageral = 0;

    printf("1 - Inserir valor de fechamento\n");
    printf("2 - Cancelar\n");
    scanf("%d",&cFechamento);

    switch (cFechamento){
    case 1:
        printf("Informe valor em dinheiro: ");
        scanf("%f",&vFechamentoD);
        printf("\nInforme valor em cartao: ");
        scanf("%f",&vFechamentoC);
        vFechageral = vFechamentoD + vFechamentoC + vAbertura;
        totalDia += vAbertura;
        if (vFechageral >= totalDia ) {
            printf("Caixa fechado  com sucesso\n");
            printf("Valor total em dinheiro: %.2f R$\n", vFechamentoD);
            printf("Valor total em cartao: %.2f R$\n", vFechamentoC);
            printf("Valor de abertura de caixa: %.2f R$\n", vAbertura);
            printf("Sistema finalizando em 5 segundos");
            delay(5);
            exit(0);
        }

    case 2:
        menuPrincipal();
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
        vPadaria = 0; vAlimento= 0; vLimpeza =0; vCar=0;
        printf("Venda cancelada com sucesso...\n");
        delay(3);
        menuPrincipal();
    case 2:
        menuPrincipal();
    }
}

int main(){
    menuPrincipal(); // Funcao Principal
    return 0;
}
