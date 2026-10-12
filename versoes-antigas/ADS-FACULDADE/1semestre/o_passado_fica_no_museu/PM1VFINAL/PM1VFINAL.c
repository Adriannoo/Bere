#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Fun��es e vari�veis globais */
void menuPrincipal(void);
void menuLimpeza(void);
void menuAlimento(void);
void menuPadaria(void);
void menuPagamento(void);
void menufPagamento(void);
float vLimpeza = 0, vPadaria = 0, vAlimento = 0, totalDia =0;
float fPagar;

int main()
{
    menuPrincipal();
    return 0;
}

void menuPrincipal(){
    int opcaoUm=0;

    while (opcaoUm!=5)
    {
        printf("   <Menu Principal>\n");
        printf("1. Material de Limpeza\n");
        printf("2. Alimentos\n");
        printf("3. Padaria\n");
        printf("4. Pagamento\n");
        printf("5. Sair\n");

        printf("\nValor total de vendas hoje: %.2f \n",totalDia);

        scanf("%d", &opcaoUm);

        switch(opcaoUm){
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
                printf("Encerrando o sistema...");
                exit (0);
            default:
                printf("Opcao invalida... Tente novamente\n");
                menuPrincipal();
    }
  }
}

void menuLimpeza()
{
    int cLimpeza;
    float pDetergente, pSabaopo, pEsponja, pAmaciante; /* <<< PRODUTOS DE LIMPEZA */
    float qnt;
        /* <<<<<<< VALORES DOS PRODUTOS >>>>>>> */
            /* LIMPEZA */
    pDetergente = 1.99;
    pSabaopo = 8.99;
    pEsponja = 1.50;
    pAmaciante = 15.00;

    printf("  <Material de limpeza>\n");
    printf("11. Detergente      R$ 1,99\n");
    printf("12. Sabao em po 1kg R$ 8,99\n");
    printf("13. Esponja         R$ 1,50\n");
    printf("14. Amaciante 1lt   R$ 15,00\n");
    printf("15. Voltar ao menu principal\n");

    scanf("%d", &cLimpeza);

    switch(cLimpeza){
        case 11:    /*<----- DETERGENTE */
            printf("Detergente\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Detergente adicionado ao carrinho.\n",qnt);
            vLimpeza += pDetergente * qnt;
            menuLimpeza();
            break;

        case 12:    /*<----- SABAO EM P� */
            printf("Sabao em po\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Sabao em po adicionado ao carrinho.\n",qnt);
            vLimpeza += pSabaopo * qnt;
            menuLimpeza();
            break;

        case 13:    /*<----- ESPONJA*/
            printf("Esponja\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Esponja adicionado ao carrinho.\n",qnt);
            vLimpeza += pEsponja * qnt;
            menuLimpeza();
            break;

        case 14:    /*<----- AMACIANTE*/
            printf("Amaciante\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Amaciante adicionado ao carrinho.\n",qnt);
            vLimpeza += pAmaciante * qnt;
            menuLimpeza();
            break;

        case 15:    /*<----- VOLTAR*/
            printf("Voltar ao menu principal\n");
            break;

        default:
            printf("Opcao invalida... Tente Novamente\n");
            menuLimpeza();
            break;
        }
}

  void menuAlimento()
{
    int cAlimento;
    float pCafe, pFeijaop, pAcucar, pLeite, pArroz; /* <<< ALIMENTOS */
    float qnt;
            /* ALIMENTOS */
    pCafe = 19.99;
    pLeite = 5.90;
    pArroz = 4.50;
    pFeijaop = 8.00;
    pAcucar = 5.00;

    printf("   <Alimentos>\n");
    printf("21. Cafe             R$ 19,99\n");
    printf("22. Leite Cx         R$ 5,90\n");
    printf("23. Arroz 1KG        R$ 4,50\n");
    printf("24. Feijao Preto 1KG R$ 8,00\n");
    printf("25. Acucar 1KG       R$ 5,00\n");
    printf("26. Voltar ao menu principal\n");

    scanf("%d", &cAlimento);

    switch(cAlimento){
        case 21:    /*<----- CAFE */
            printf("Cafe\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Cafe adicionado ao carrinho.\n",qnt);
            vAlimento += pCafe * qnt;
            menuAlimento();
            break;

        case 22:    /*<----- LEITE */
            printf("Leite\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Leite adicionado ao carrinho.\n",qnt);
            vAlimento += pLeite * qnt;
            menuAlimento();
            break;

        case 23:    /*<----- ARROZ*/
            printf("Arroz 1kg\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Arroz 1kg adicionado ao carrinho.\n",qnt);
            vAlimento += pArroz * qnt;
            menuAlimento();
            break;

        case 24:    /*<----- FEIJAO PRETO*/
            printf("Feijao Preto 1kg\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Feijao preto 1kg adicionado ao carrinho.\n",qnt);
            vAlimento += pFeijaop * qnt;
            menuAlimento();
            break;

        case 25:    /*<----- A��CAR*/
            printf("Acucar 1kg\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Acucar 1kg adicionado ao carrinho.\n",qnt);
            vAlimento += pAcucar * qnt;
            menuAlimento();
            break;

        case 26:    /*<----- VOLTAR*/
            printf("Voltar ao menu principal\n");
            break;

        default:
            printf("Opcao invalida... Tente Novamente\n");
            menuAlimento();
            break;
        }
}


void menuPadaria()
{
    int cPadaria;
    float pPaoforma, pPaointegral, pSonho, pPaofrances; /* <<< PADARIA */
    float qnt;
    /* PADARIA */
    pPaoforma = 9.50;
    pPaointegral = 12.50;
    pPaofrances = 1.90;
    pSonho = 8.50;

    printf("        <Padaria>  \n");
    printf("31. Pao de Forma    R$ 9,50\n");
    printf("32. Pao Integral    R$ 12,50\n");
    printf("33. Pao Frances und R$ 1,90\n");
    printf("34. Sonho           R$ 8,50\n");
    printf("35. Voltar ao menu principal\n");

    scanf("%d", &cPadaria);

    switch(cPadaria){
        case 31:   /*<----- PAO DE FORMA */
            printf("Pao de Forma\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Pao de Forma adicionado ao carrinho.\n",qnt);
            vPadaria += pPaoforma * qnt;
            menuPadaria();
            break;

        case 32:    /*<----- PAO INTEGRAL */
            printf("Pao Integral\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Pao integral adicionado ao carrinho.\n",qnt);
            vPadaria += pPaointegral * qnt;
            menuPadaria();
            break;

        case 33:    /*<----- PAO FRANCES*/
            printf("Pao Frances\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Pao frances adicionado ao carrinho.\n",qnt);
            vPadaria += pPaofrances * qnt;
            menuPadaria();
            break;

        case 34:    /*<----- SONHO*/
            printf("Sonho\n");
            printf("Inserir quantidade: ");
            scanf("%f", &qnt);
            printf("%.2f x Sonho adicionado ao carrinho.\n",qnt);
            vPadaria += pSonho * qnt;
            menuPadaria();
            break;

        case 35:    /*<----- VOLTAR*/
            printf("Voltar ao menu principal\n");
            break;

        default:
            printf("Opcao invalida... Tente Novamente\n");
            menuPadaria();
            break;
    }
}

void menuPagamento()
{
    int cPagamento, cCartao;
    float vTotal = 0, vTcdesconto = 0, qnt = 0, troco = 0, valorCobrado = 0, descontoReal =0; /* <<< VARI�VEIS DE ARMAZENAMENTO TEMPOR�RIO */
    float descontoCinco, descontoDez, descontoDezoito ; /* <<< VARI�VEIS DE ARMAZENAMENTO FIXO */
        /* DESCONTOS */
    descontoCinco = 0.05;   /* <<<- 5% desconto */
    descontoDez = 0.10;  /* <<<- 10% desconto */
    descontoDezoito = 0.18;  /* <<<- 18% desconto */

    vTotal = vLimpeza + vPadaria + vAlimento;
    printf("Forma de Pagamento:\n");
    printf("41. Dinheiro\n");
    printf("42. Cartao\n");
    printf("43. Voltar ao menu principal\n");

    scanf("%d", &cPagamento);   /* <--------------- PAGAMENTO EM DINHEIRO */

    switch(cPagamento){
        case 41:
            printf("Valor total: %.2f\n",vTotal);

            if (vTotal <= 50)
            {
                descontoReal = vTotal * descontoCinco;
            }

            else if (vTotal > 50 && vTotal< 100)
            {
                descontoReal = vTotal * descontoDez;
            }

            else if (vTotal >=100)
            {
                    descontoReal = vTotal * descontoDezoito;
            }

            vTcdesconto = vTotal - descontoReal;
            printf("Valor com desconto: %.2f R$\n", vTcdesconto);
            printf("Inserir valor recebido em dinheiro\n");
            scanf("%f", &valorCobrado);

            if (valorCobrado >= vTcdesconto)
            {
                troco= valorCobrado- vTcdesconto;
                totalDia= totalDia + vTcdesconto;  /* <----------- Soma do total di�rio */
                printf("Troco: %.2f R$\n", troco);
                printf("Pagamento realizado com sucesso!\n");
                vLimpeza= 0;
                vAlimento= 0;
                vPadaria= 0;
                descontoReal = 0;
                fPagar= 0;  /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                break;
            }
            else if (valorCobrado < vTcdesconto)
            {
                printf("O valor integral sera cobrado!\n");
                fPagar = vTotal - valorCobrado;
                totalDia= totalDia + valorCobrado;
                printf("Falta pagar %.2f R$", fPagar);
                menufPagamento();
                break;
            }

        case 42:       /* <--------- PAGAMENTO EM CART�O */
            printf("Valor total: %.2f R$\n",vTotal);
            printf("51. Pagamento realizado\n");
            printf("52. Pagamento nao realizado\n");
            printf("53. Voltar ao  menu anterior\n");
            scanf("%d", &cCartao);

            if (cCartao == 51)
                {
                    printf("Pagamento realizado com sucesso!\n");
                    totalDia = totalDia + vTotal; /* <----------- Soma do total di�rio */
                }
                else if (cCartao == 52)
                {
                    printf("Pagamento nao realizado!\n");
                }

                else if (cCartao == 53)
                {
                    printf("Voltar ao menu anterior\n");
                    menuPagamento();
                }

                else
                {
                    printf("Opcao invalida... Tente novamente\n");
                }

                vPadaria = 0; vAlimento= 0; vLimpeza =0;   /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
                break;

        case 43:
            printf("Voltar ao menu principal\n");
            menuPrincipal();
            break;

        default:
            printf("Opcao invalida... Tente Novamente\n");
            menuPagamento();
            break;
    }
}

void menufPagamento() //FUN��O CRIADA PARA CASO A PESSOA N�O DE DINHEIRO SUFICIENTE NO PAGAMENTO NO MENUPRINCIPAL
{
    int cPagamento= 0, cCartao= 0;
    float troco = 0, valorCobrado = 0; /* <<< VARI�VEIS DE ARMAZENAMENTO TEMPOR�RIO */

    float descontoCinco, descontoDez, descontoDezoito ; /* <<< VARI�VEIS DE ARMAZENAMENTO FIXO */
                /* DESCONTOS */
    descontoCinco = 0.05;   /* <<<- 5% desconto */
    descontoDez = 0.10;  /* <<<- 10% desconto */
    descontoDezoito = 0.18;  /* <<<- 18% desconto */

    printf("\nForma de Pagamento:\n");
    printf("41. Dinheiro\n");
    printf("42. Cartao\n");
    printf("43. Voltar ao menu principal\n");

    scanf("%d", &cPagamento);   /* <--------------- PAGAMENTO EM DINHEIRO */
    switch(cPagamento){
        case 41:
            printf("Valor total: %.2f R$\n",fPagar);
            printf("Inserir valor recebido em dinheiro\n");
            scanf("%f", &valorCobrado);

            if (valorCobrado >= fPagar)
            {
                totalDia= totalDia + fPagar;  /* <----------- Soma do total di�rio */
                troco= valorCobrado- fPagar;
                printf("Troco: %.2f R$\n", troco);
                printf("Pagamento realizado com sucesso!\n");
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
                menuPagamento();
            }

            else
            {
                printf("Opcao invalida... Tente novamente\n");
            }

            vPadaria = 0; vAlimento= 0; vLimpeza =0;   /*<----------- ZERAR VALORES PARA EFETUAR NOVA VENDA */
            break;

        case 43:
            printf("Voltar ao menu principal\n");
            menuPrincipal();
            break;

        default:
            printf("Opcao invalida... Tente Novamente\n");
            menufPagamento();
            break;
    }
}
