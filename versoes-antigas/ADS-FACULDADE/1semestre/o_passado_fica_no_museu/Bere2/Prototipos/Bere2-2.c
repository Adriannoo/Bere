#include <stdio.h>
#include <math.h> //Bibliotecas
#include <stdlib.h>

/* Funcoes e variaveis globais */
void menuPrincipal(void);
void menuLimpeza(void);
void menuAlimento(void); // Declaracoes das Funcoes
void menuPadaria(void);
void menuPagamento(void);
void menufPagamento(void);

float vLimpeza = 0, vPadaria = 0, vAlimento = 0, totalDia =0; // Variaveis globais
float fPagar;
float vCar = 0;
float vAbertura = 0;

int main()
{
    menuPrincipal(); // Funcao Principal
    return 0;
}

void menuPrincipal() //Funcao do Menu Principal
{
    int opcaoUm=0; // Variavel opcao do usuario

    while (opcaoUm!=5)
    {
        system("cls");
        printf("\n   <Menu Principal>\n"); //Print do Menu
        printf("1. Material de Limpeza\n");
        printf("2. Alimentos\n");
        printf("3. Padaria\n");
        printf("4. Pagamento\n");
        printf("5. Abertura de caixa\n");
        printf("6. Fechamento de caixa\n");
        printf("7. Sair\n");
        printf("Valor total carrinho: %.2f R$", vCar);

        printf("\nValor total de vendas hoje: %.2f  R$ \n",totalDia); // Atualizacao do valor total das vendas
        printf("\n");

        scanf("%d", &opcaoUm);

        switch(opcaoUm){ // Switch Case para as sessoes do mercadinho
            case 1:
                if(vAbertura <= 0){          // 47 até 51 === VERIFICAÇÃO CAIXA ABERTO CASO ESTIVER FECHADO SOLICITA ABERTURA
                    system("cls");
                    printf("O caixa esta fechado, abra para prosseguir...");
                    sleep(3);
                    menuPrincipal();
                }
                else{
                menuLimpeza();
                break;}
            case 2:
                    if(vAbertura <= 0){    // VERIFICAÇÃO CAIXA ABERTO
                    system("cls");
                    printf("O caixa esta fechado, abra para prosseguir...");
                    sleep(3);
                    menuPrincipal();
                }
                else{
                menuAlimento();
                break;}
            case 3:
                    if(vAbertura <= 0){  // VERIFICAÇÃO CAIXA ABERTO
                    system("cls");
                    printf("O caixa esta fechado, abra para prosseguir...");
                    sleep(3);
                    menuPrincipal();
                }
                else{
                menuPadaria();
                break;}
            case 4:
                    if(vAbertura <= 0){  // VERIFICAÇÃO CAIXA ABERTO
                    system("cls");
                    printf("O caixa esta fechado, abra para prosseguir...");
                    sleep(3);
                    menuPrincipal();
                    }
                else{
                menuPagamento();
                break;}
            case 5:
                menuAbertura();
                break;
            case 6:
                    if(vAbertura <= 0){  // VERIFICAÇÃO CAIXA ABERTO
                    system("cls");
                    printf("O caixa esta fechado, abra para prosseguir...");
                    sleep(3);

                    menuPrincipal();
                }
                else{
                menuFechamento();
                break;}
            case 7:
                printf("Encerrando o sistema...");
                exit (0);
            default:
                printf("Opcao invalida... Tente novamente\n");
                menuPrincipal();
    }
  }
}

void menuLimpeza() // Funcao Menu Limpeza
{
    system("cls");
    printf("\n");
    int cLimpeza; // Opcao do usuario
    float pDetergente, pSabaopo, pEsponja, pAmaciante; /* <<< PRODUTOS DE LIMPEZA */
    float qnt;
        /* <<<<<<< VALORES DOS PRODUTOS >>>>>>> */
            /* LIMPEZA */
    pDetergente = 1.99;
    pSabaopo = 8.99;
    pEsponja = 1.50; // Valores dos produtos
    pAmaciante = 15.00;

    printf("  <Material de limpeza>\n");
    printf("11. Detergente      R$ 1,99\n");
    printf("12. Sabao em po 1kg R$ 8,99\n");
    printf("13. Esponja         R$ 1,50\n");
    printf("14. Amaciante 1lt   R$ 15,00\n");
    printf("15. Voltar ao menu principal\n");
    printf("Carrinho materiais de limpeza: %.2f R$", vLimpeza);
    printf("\n");

    scanf("%d", &cLimpeza);

    switch(cLimpeza){ // Switch case para selecao dos produtos
        case 11:    /*<----- DETERGENTE */
            printf("\n");
            printf("Detergente\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Detergente adicionado ao carrinho.\n",qnt);
            vLimpeza += pDetergente * qnt;
            vCar += pDetergente * qnt;
            menuLimpeza();
            break;

        case 12:    /*<----- SABAO EM P� */
            printf("\n");
            printf("Sabao em po\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Sabao em po adicionado ao carrinho.\n",qnt);
            vLimpeza += pSabaopo * qnt;
            vCar += pSabaopo * qnt;
            menuLimpeza();
            break;

        case 13:    /*<----- ESPONJA*/
            printf("\n");
            printf("Esponja\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Esponja adicionado ao carrinho.\n",qnt);
            vLimpeza += pEsponja * qnt;
            vCar += pEsponja * qnt;
            menuLimpeza();
            break;

        case 14:    /*<----- AMACIANTE*/
            printf("\n");
            printf("Amaciante\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Amaciante adicionado ao carrinho.\n",qnt);
            vLimpeza += pAmaciante * qnt;
            vCar += pAmaciante * qnt;
            menuLimpeza();
            break;

        case 15:    /*<----- VOLTAR*/
            printf("\n");
            printf("Voltar ao menu principal\n");
            break;

        default:
            printf("\n");
            printf("Opcao invalida... Tente Novamente\n");
            menuLimpeza();
            break;
        }
}
  void menuAlimento() // Funcao Menu Alimentos
{
    system("cls");
    printf("\n");
    int cAlimento;
    float pCafe, pFeijaop, pAcucar, pLeite, pArroz; /* <<< ALIMENTOS */
    float qnt;
            /* ALIMENTOS */
    pCafe = 19.99;
    pLeite = 5.90;
    pArroz = 4.50; // Valores dos produtos
    pFeijaop = 8.00;
    pAcucar = 5.00;

    printf("   <Alimentos>\n");
    printf("21. Cafe             R$ 19,99\n");
    printf("22. Leite Cx         R$ 5,90\n");
    printf("23. Arroz 1KG        R$ 4,50\n");
    printf("24. Feijao Preto 1KG R$ 8,00\n");
    printf("25. Acucar 1KG       R$ 5,00\n");
    printf("26. Voltar ao menu principal\n");
    printf("Carrinho alimentos: %.2f R$", vAlimento);
    printf("\n");

    scanf("%d", &cAlimento);

    switch(cAlimento){ // Switch case para selecao dos produtos
        case 21:    /*<----- CAFE */
            printf("\n");
            printf("Cafe\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Cafe adicionado ao carrinho.\n",qnt);
            vAlimento += pCafe * qnt;
            vCar += pCafe * qnt;
            menuAlimento();
            break;

        case 22:    /*<----- LEITE */
            printf("\n");
            printf("Leite\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Leite adicionado ao carrinho.\n",qnt);
            vAlimento += pLeite * qnt;
            vCar += pLeite * qnt;
            menuAlimento();
            break;

        case 23:    /*<----- ARROZ*/
            printf("\n");
            printf("Arroz 1kg\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Arroz 1kg adicionado ao carrinho.\n",qnt);
            vAlimento += pArroz * qnt;
            vCar += pArroz * qnt;
            menuAlimento();
            break;

        case 24:    /*<----- FEIJAO PRETO*/
            printf("\n");
            printf("Feijao Preto 1kg\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Feijao preto 1kg adicionado ao carrinho.\n",qnt);
            vAlimento += pFeijaop * qnt;
            vCar += pFeijaop * qnt;
            menuAlimento();
            break;

        case 25:    /*<----- ACUCAR*/
            printf("\n");
            printf("Acucar 1kg\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Acucar 1kg adicionado ao carrinho.\n",qnt);
            vAlimento += pAcucar * qnt;
            vCar += pAcucar * qnt;
            menuAlimento();
            break;

        case 26:    /*<----- VOLTAR*/
            printf("\n");
            printf("Voltar ao menu principal\n");
            break;

        default:
            printf("\n");
            printf("Opcao invalida... Tente Novamente\n");
            menuAlimento();
            break;
        }
}
void menuPadaria() // Funcao Menu Padaria
{
    system("cls");
    printf("\n");
    int cPadaria;
    float pPaoforma, pPaointegral, pSonho, pPaofrances; /* <<< PADARIA */
    float qnt;
    /* PADARIA */
    pPaoforma = 9.50;
    pPaointegral = 12.50; // Valores dos produtos
    pPaofrances = 1.90;
    pSonho = 8.50;

    printf("        <Padaria>  \n");
    printf("31. Pao de Forma    R$ 9,50\n");
    printf("32. Pao Integral    R$ 12,50\n");
    printf("33. Pao Frances und R$ 1,90\n");
    printf("34. Sonho           R$ 8,50\n");
    printf("35. Voltar ao menu principal\n");
    printf("Carrinho padaria: %.2f R$ ", vPadaria);
    printf("\n");

    scanf("%d", &cPadaria);

    switch(cPadaria){ // Switch case para selecao dos protudos
        case 31:   /*<----- PAO DE FORMA */
            printf("\n");
            printf("Pao de Forma\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Pao de Forma adicionado ao carrinho.\n",qnt);
            vPadaria += pPaoforma * qnt;
            vCar += pPaoforma * qnt;
            menuPadaria();
            break;

        case 32:    /*<----- PAO INTEGRAL */
            printf("\n");
            printf("Pao Integral\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Pao integral adicionado ao carrinho.\n",qnt);
            vPadaria += pPaointegral * qnt;
            vCar += pPaointegral * qnt;
            menuPadaria();
            break;

        case 33:    /*<----- PAO FRANCES*/
            printf("\n");
            printf("Pao Frances\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Pao frances adicionado ao carrinho.\n",qnt);
            vPadaria += pPaofrances * qnt;
            vCar += pPaofrances * qnt;
            menuPadaria();
            break;

        case 34:    /*<----- SONHO*/
            printf("\n");
            printf("Sonho\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Sonho adicionado ao carrinho.\n",qnt);
            vPadaria += pSonho * qnt;
            vCar += pSonho * qnt;
            menuPadaria();
            break;

        case 35:    /*<----- VOLTAR*/
            printf("\n");
            printf("Voltar ao menu principal\n");
            break;

        default:
            printf("\n");
            printf("Opcao invalida... Tente Novamente\n");
            menuPadaria();
            break;
    }
}
void menuPagamento() // Funcao Menu Pagamento
{
    system("cls");
    printf("\n");
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
    printf("Forma de Pagamento:\n");
    printf("41. Dinheiro\n");
    printf("42. Cartao\n");
    printf("43. Voltar ao menu principal\n");

    scanf("%d", &cPagamento);   /* <--------------- PAGAMENTO EM DINHEIRO */

    switch(cPagamento){ // Switch case para verificar valor com desconto
        case 41:
            printf("\n");
            printf("Valor total: %.2f R$\n",vTotal);

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
            printf("\n");
            printf("Valor com desconto: %.2f R$\n", vTcdesconto);
            printf("Inserir valor recebido em dinheiro\n");
            scanf("%f", &valorCobrado);

            if (valorCobrado > vTcdesconto)
            {
                troco= valorCobrado- vTcdesconto;
                totalDia= totalDia + vTcdesconto;  /* <----------- Soma do total diario */
                system("cls");
                printf("Processando pagamento.");
                sleep(1);
                system("cls");
                printf("Processando pagamento..");
                sleep(1);
                system("cls");
                printf("Processando pagamento...");
                sleep(1);
                system("cls");
                printf("Processando pagamento....");
                sleep(1);
                system("cls");
                printf("\n");
                printf("Troco: %.2f R$\n", troco);
                printf("Pagamento realizado com sucesso!\n");
                sleep(3);
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
                totalDia= totalDia + vTcdesconto;
                printf("\n");
                printf("Pagamento realizado com sucesso!\n");
                vLimpeza= 0;
                vAlimento= 0;
                vPadaria= 0;
                descontoReal = 0;
                fPagar= 0;
                vCar = 0;/*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                break;
            }
            else
            {
                printf("\n");
                printf("O valor integral sera cobrado!\n");
                fPagar = vTotal - valorCobrado;
                totalDia= totalDia + valorCobrado;
                printf("Falta pagar %.2f R$", fPagar);
                menufPagamento();
                break;
            }

        case 42:       /* <--------- PAGAMENTO EM CART�O */
            printf("Valor total: %.2f R$\n",vTotal);
            printf("\n");
            printf("51. Pagamento realizado\n");
            printf("52. Pagamento nao realizado\n");
            printf("53. Voltar ao  menu anterior\n");
            scanf("%d", &cCartao);

            if (cCartao == 51)
                {
                    printf("\n");
                    printf("Pagamento realizado com sucesso!\n");
                    totalDia = totalDia + vTotal; /* <----------- Soma do total di�rio */
                }
                else if (cCartao == 52)
                {
                    printf("\n");
                    printf("Pagamento nao realizado!\n");
                }

                else if (cCartao == 53)
                {
                    printf("\n");
                    printf("Voltar ao menu anterior\n");
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
            printf("\n");
            printf("Voltar ao menu principal\n");
            menuPrincipal();
            break;

        default:
            printf("\n");
            printf("Opcao invalida... Tente Novamente\n");
            menuPagamento();
            break;
    }
}

void menufPagamento() //FUNCAO CRIADA PARA CASO A PESSOA NAO DE DINHEIRO SUFICIENTE NO PAGAMENTO NO MENUPRINCIPAL
{
    system("cls");
    int cPagamento= 0, cCartao= 0;
    float troco = 0, valorCobrado = 0; /* <<< VARI�VEIS DE ARMAZENAMENTO TEMPOR�RIO */

    float descontoCinco, descontoDez, descontoDezoito ; /* <<< VARI�VEIS DE ARMAZENAMENTO FIXO */
                /* DESCONTOS */
    descontoCinco = 0.05;   /* <<<- 5% desconto */
    descontoDez = 0.10;  /* <<<- 10% desconto */
    descontoDezoito = 0.18;  /* <<<- 18% desconto */

    printf("\nForma de Pagamento:\n");
    printf("\n");
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
                totalDia= totalDia + fPagar;  /* <----------- Soma do total di�rio */
                troco= valorCobrado- fPagar;
                printf("\n");
                printf("Troco: %.2f R$\n", troco);
                printf("Pagamento realizado com sucesso!\n");
                printf("\n");
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
                printf("Falta pagar %.2f R$ \n", fPagar);
                printf("\n");
                menufPagamento();
                break;
            }

        case 42:       /* <--------- PAGAMENTO EM CART�O */
            printf("Valor total: %.2f R$\n",fPagar);
            printf("51. Pagamento realizado\n");
            printf("52. Pagamento nao realizado\n");
            printf("53. Voltar ao  menu anterior\n");
            scanf("%d", &cCartao);
            if (cCartao == 51)
            {
                printf("Pagamento realizado com sucesso!\n");
                printf("\n");
                totalDia= totalDia + fPagar; /* <----------- Soma do total di�rio */
                menuPrincipal();
            }

            else if (cCartao == 52)
            {
                printf("Pagamento nao realizado!\n");
            }

            else if (cCartao == 53)
            {
                printf("Voltar ao menu anterior\n");
                printf("\n");
                menuPagamento();
            }

            else
            {
                printf("Opcao invalida... Tente novamente\n");
                printf("\n");
            }

            vPadaria = 0; vAlimento= 0; vLimpeza =0; vCar=0;   /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
            break;

        case 43:
            printf("Voltar ao menu principal\n");
            printf("\n");
            menuPrincipal();
            break;

        default:
            printf("Opcao invalida... Tente Novamente\n");
            printf("\n");
            menufPagamento();
            break;
    }
}


void menuAbertura()
{

    int cAbertura = 0;
    if (vAbertura > 0){
            system("cls");
    printf("Existe um caixa em aberto, finalize para realizar uma nova abertura\n");
    for ( int i = 5; i != 0;i--){
        printf("Retornando em %d segundos \n",i);
        sleep(1);
    }
    menuPrincipal();}
    else{
         system("cls");
         printf("    <Abertura de caixa>\n");
         printf("1 - Inserir valor de abertura\n");
         printf("2 - Cancelar\n");
         scanf("%d",&cAbertura);
switch (cAbertura){
      case 1:
      printf("Informe o valor: ");
      scanf("%f",&vAbertura);
      system("cls");
      printf("Validando abertura de caixa.");
      sleep(1);
      system("cls");
      printf("Validando abertura de caixa..");
      sleep(1);
      system("cls");
      printf("Validando abertura de caixa...");
      sleep(1);
      system("cls");
      printf("Validando abertura de caixa....");
      sleep(1);
      system("cls");
      printf("Caixa aberto com sucesso...\n");
      sleep(1);
      printf("Valor de abertura de caixa %.2f R$", vAbertura);
      sleep(4);
      menuPrincipal();
      break;
      case 2:
      menuPrincipal();
      break;
      default:
      printf("Opcao invalida, tente novamente...");
      menuAbertura();
      break;}

}
}

void menuFechamento(){
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
                vFechageral = vFechamentoD+ vFechamentoC+ vAbertura;
                totalDia += vAbertura;
                if (vFechageral >= totalDia ){
                    printf("Caixa fechado  com sucesso\n");
                    printf("Valor total em dinheiro: %.2f R$\n", vFechamentoD);
                    printf("Valor total em cartao: %.2f R$\n", vFechamentoC);
                    printf("Valor de abertura de caixa: %.2f R$\n", vAbertura);
                    printf("Sistema finalizando em 5 segundos");
                    sleep(5);
                    exit(0);}
            case 2:
                menuPrincipal();
                break;



     }


}
