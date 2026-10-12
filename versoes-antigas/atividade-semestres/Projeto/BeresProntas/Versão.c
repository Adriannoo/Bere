#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

struct CadastroCliente *cliente = NULL;
struct CadastroProduto *produto = NULL;
//Venda *vendas = NULL;
//Pagamento *pagamentos = NULL;
int numClientes = 0, numProdutos = 0, numVendas = 0, numPagamentos = 0, opcao = 0, loopDesconto;
int vendasRealizadas = 0;
float inicioCaixa = 0.0, totalCartao = 0.0, totalDin = 0.0, totalDinCartao = 0.0, totalFat= 0.0, vCaixa = 0.0;
float totalDesc = 0, totalBruto = 0, pagoDin = 0, pagoCartao= 0.0, desc = 0, desconto = 0, troco = 0.0, retirada = 0.0;
char caixa = 'F';
FILE *arquivoCliente, *arquivoProduto;

//declaração de structs
struct CadastroCliente{
    int cod;
    char nome[30];
    char nomesocial[30];
    char cpf[12];
    char rua[30];
    char num[8];
    char bairro[30];
    char cep[8];
    char telefone[30];};

struct CadastroProduto{
    int cod;
    char descricao[30];
    char categoria[30];
    float precoCompra;
    float precoVenda;
    int estoque;};

//função que impede letras onde pede-se números
bool validaNumero(char *numero, int tamanho){
    if (numero==NULL){
        return false;
    }
    int len = strlen(numero);
    if ((tamanho>0) && (len != tamanho)){
        return false;
    }
    for (int i = 0; i < len; i++){
        if (!isdigit(numero[i]))
        {
            return false;
        }
    }
    return true;
}

//função que impede mandar espaço vazio
bool validacaracter(char *caracter){
    int len = strlen(caracter);
    if (len == 0){
        return false;
    }
    return true;
}

int procuraItem(int codigo){
    int i;
    for (int i = 0; i < numProdutos; i++){
       if (produto[i].cod==codigo) {
         break;
       }  
    }
    return i;
}

//=================================================== MENUS
//função para menu principal
void menuPrincipal(){

    system("cls");
    printf("\n|---------------------------------------------|\n");
    printf("|           SISTEMA MERCADO BERE              |\n");
    printf("|---------------------------------------------|\n");
    printf("|           | | Menu Principal | |            |\n");
    printf("|                                             |\n");
    printf("|     1. Cadastros                            |\n");
    printf("|     2. Vendas                               |\n");
    printf("|     3. Abertura de Caixa                    |\n");
    printf("|     4. Fechamento de Caixa                  |\n");
    printf("|     5. Relatorios                           |\n");
    printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
    printf("|     6. Sair                                 |\n");
    printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
    printf("|     Escolha uma das opcoes acima:           |\n");
    printf("|     (Somente numero)                        |\n");
    printf("|_____________________________________________|\n\n");
    scanf("%d", &opcao);
}

//função para menu de cadastros
void menuCadastros(){

    int cadastrar=0;
    while(cadastrar != 3){
        system("cls");
        printf("\n _____________________________________________\n");
        printf("|                                             |\n");
        printf("|           | |   Cadastros   | |             |\n");
        printf("|                                             |\n");
        printf("|     1. Cadastrar clientes                   |\n");
        printf("|     2. Cadastrar produtos                   |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     3. Voltar ao Menu principal             |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     Escolha uma das opcoes acima:           |\n");
        printf("|     (Somente numero)                        |\n");
        printf("|_____________________________________________|\n\n");
        scanf("%d", &cadastrar);

        switch (cadastrar){

        //cadastrar clientes
        case 1:
            cadastroCliente();
            break;

        //cadastrar produtos
        case 2:
            cadastroProduto();
            break;

        //voltar ao menu principal
        case 3:
            voltar();
            break;

        //caso não seja nenhuma das opções de cadastro
        default:
            opinvalida();
        }
    }
}

// função para menu de opção invalida
void opinvalida(){
    printf("\n|----------------------------------------------------|\n");
    printf("|  OPCAO INVALIDA! FAVOR INSERIR UM VALOR VALIDO...  |\n");
    printf("|----------------------------------------------------| \n\n");
    system("pause");
    system("cls");
}

// função para menu de voltar menu
void voltar(){
    printf("\n|------------------------------------|\n");
    printf("|  VOLTANDO PARA O MENU ANTERIOR...  |\n");
    printf("|------------------------------------|\n\n");
    system("pause");
    system("cls");
}

//função para menu de vendas
void menuVendas(){

    int vender=0;
    while (vender != 4){
        system("cls");
        printf("\n|---------------------------------------------|\n");
        printf("|           SISTEMA MERCADO BERE              |\n");
        printf("|---------------------------------------------|\n");
        printf("|           | | Menu de Vendas | |            |\n");
        printf("|                                             |\n");
        printf("|     1. Nova Venda                           |\n");
        printf("|     2. Retirada de Caixa (Sangria)          |\n");
        printf("|     3. Pagamento                            |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     4. Voltar ao Menu Principal             |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     Escolha uma das opcoes acima:           |\n");
        printf("|     (Somente numero)                        |\n");
        printf("|_____________________________________________|\n\n");
        scanf("%d", &vender);

        switch (vender){

        //nova venda
        case 1:
            novaVenda();
            break;

        //reirada de caixa
        case 2:
            Sangria();
            break;

        //pagamento
        case 3:
            pagamento();
            break;

        //voltar para menu principal
        case 4:
            voltar();
            break;

        //caso não seja nenhuma das opções de venda ou pagamento
        default:
            opinvalida();
        }
    }
}

//função para menu de abertura de caixa
void aberturaCaixa(){

    system("cls");
    printf("\n|--------------------------------------------------|\n");
    printf("|             ABERTURA DE CAIXA:                   |\n");
    printf("|--------------------------------------------------|\n");
    printf("| Digite o valor inicial do caixa                  |\n");
    printf("|--------------------------------------------------|\n\nR$ ");
    scanf("%f", &inicioCaixa);

    //se o inicio do caixa for menor que 0
    if(inicioCaixa<0){
        opinvalida();
    }

    //abrir caixa
    else{
        caixa='A';
        //totalsetor[0]=0.00;
        //totalsetor[1]=0.00;
        //totalsetor[2]=0.00;
        system("cls");
    }
}

// função para menu de caixa já aberto
void caixaAberto(){
    system("cls");
    printf("\n|------------------------------------|\n");
    printf("|         CAIXA ABERTO...            |\n");
    printf("|------------------------------------|\n\n");
    system("pause");
    system("cls");
}

//função para menu de fechamento de caixa
void fechamentoCaixa(){
    //caso já tenha ocorrido um pagamento, não pode fechar o caixa antes de concluir
    if (pagoDin>0||pagoCartao>0){
        system("cls");
        printf("\n|----------------------------------------------------------------|\n");
        printf("|  RECEBIMENTO JA EXISTENTE, VOLTANDO AO MENU PRINCIPAL...       |\n");
        printf("|----------------------------------------------------------------| \n\n");
        system("pause");
        system("cls");
    }

    //fechar caixa
    else{
        system("cls");
        totalFat=totalDin+totalCartao+totalDinCartao;
        totalDinCartao+=pagoDin+pagoCartao;
        printf("\n|--------------------------------------------------|\n");
        printf("|             FECHAMENTO DE CAIXA:                 |\n");
        printf("|--------------------------------------------------|\n");
        printf("  Vendas do dia: %d                                \n", vendasRealizadas);
        printf("|--------------------------------------------------|\n\n");
        printf("  VALOR DE ABERTURA: R$ %.2f \n", inicioCaixa);
        printf("  TOTAL PAGO EM DINHEIRO: R$ %.2f \n", totalDin);
        printf("  TOTAL PAGO EM CARTAO: R$ %.2f \n", totalCartao);
        printf("  TOTAL PAGO EM DINHEIRO / CARTAO: R$ %.2f \n", totalDinCartao);
        printf("  TOTAL FATURADO: R$ %.2f \n", totalFat);
        printf("|--------------------------------------------------|\n\n");

        int fechar = 0;
        while (fechar != 1){
            printf("\n|----------------------------------------------|\n");
            printf("|  TEM CERTEZA QUE DESEJA FECHAR O CAIXA?      |\n");
            printf("|                                              |\n");
            printf("|  1. Sim                                      |\n");
            printf("|  2. Nao                                      |\n");
            printf("|----------------------------------------------|\n\n");
            scanf("%d", &fechar);

            //caso escolha sair
            if (fechar == 1){
                caixaFechado();
                caixa='F';
                vendasRealizadas=0;
                inicioCaixa=0;
                totalDin=0;
                totalCartao=0;
                totalDinCartao=0;
                totalFat=0;
                main();
            }

            //voltar para menu principal
            if (fechar == 2){
                voltar();
                main();
                system("cls");
            }

            //caso não seja nenhuma das opções (sair ou voltar pra menu)
            else{
                opinvalida();
                system("cls");
            }
        }

    }
}

// função para menu de caixa já fechado
void caixaFechado(){
    system("cls");
    printf("\n|------------------------------------|\n");
    printf("|           CAIXA FECHADO..          |\n");
    printf("|------------------------------------|\n\n");
    system("pause");
    system("cls");
}

//função para menu de relatorios
void menuRelatorios(){

    int relatorio=0;
    while (relatorio != 4){
        system("cls");
        printf("\n _____________________________________________\n");
        printf("|                                             |\n");
        printf("|           | |   Relatorios   | |            |\n");
        printf("|                                             |\n");
        printf("|     1. Listagem de clientes                 |\n");
        printf("|     2. Listagem de produtos                 |\n");
        printf("|     3. Listagem de vendas                   |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     4. Retornar ao menu principal           |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     Escolha uma das opcoes acima:           |\n");
        printf("|     (Somente numero)                        |\n");
        printf("|_____________________________________________|\n\n");
        scanf("%d", &relatorio);

        switch (relatorio){

        //listagem de clientes
        case 1:
            listaClientes();
            break;

        //listagem de produtos
        case 2:
            listaProdutos();
            break;

        //listagem de vendas
        case 3:
            //listaVendas();
            break;

        //voltar para menu principal
        case 4:
            voltar();
            break;

        //caso não seja nenhuma das opçõaes de listagem
        default:
            opinvalida();
        }
    }
}

//função pra menu de sair
void saida(){

    int sair = 0;
    while (sair != 1){
        printf("\n|------------------------------------|\n");
        printf("|  TEM CERTEZA QUE DESEJA SAIR?      |\n");
        printf("|                                    |\n");
        printf("|  1. Sim                            |\n");
        printf("|  2. Nao                            |\n");
        printf("|------------------------------------|\n\n");
        scanf("%d", &sair);

        //caso escolha sair
        if (sair == 1){
        //Salva os dados em arquivo
            char salvar;
            while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N')){
            printf("\n|------------------------------------------------------|\n");
            printf("|  DESEJA SALVAR OS DADOS DE CLIENTES E PRODUTOS       |\n");
            printf("|  CADASTRADOS? (s/n)                                  |\n");
            printf("|------------------------------------------------------|\n\n");
            scanf(" %c", &salvar);
            fflush(stdin);

                if (salvar == 's' || salvar == 'S'){
                    arquivoCliente = fopen("clientes.dat", "wb");
                    fwrite(&numClientes, sizeof(int), 1, arquivoCliente);
                    fwrite(cliente, sizeof(struct CadastroCliente), numClientes, arquivoCliente);
                    fclose(arquivoCliente);
                    arquivoProduto = fopen("produtos.dat", "wb");
                    fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                    fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                    fclose(arquivoProduto);
                    system("cls");
                    printf("\nClientes e Produtos salvos, Fechando Programa...\n\n");
                    system("pause");
                    break;
                }

                if (salvar == 'n' || salvar == 'N'){
                    system("cls");
                    printf("\nClientes e produtos nao salvos. Fechando Programa...\n\n");
                    system("pause");
                    break;
                }

                else{
                    opinvalida();
                }
            }
            exit(0);
        }

        //voltar para menu principal
        if (sair == 2){
            voltar();
            main();
            system("cls");
        }

        //caso não seja nenhuma das opções (sair ou voltar pra menu)
        else{
            opinvalida();
            system("cls");
        }
    }
}
//=================================================== FIM MENUS

//=================================================== CADASTRO CLIENTES
void cadastroCliente(){

    char confirma;
    while (confirma =! 'n'||confirma != 'N'){


        cliente = realloc(cliente, (numClientes + 1) * sizeof(struct CadastroCliente));
        if (cliente == NULL){
            printf("Erro de alocação de memória.\n");
            exit(1);
        }
        cliente[numClientes].cod = numClientes + 1;

        //NOME DO CLIENTE
        fflush(stdin);
//        printf("Digite o nome do cliente: ");
//        fgets(cliente[numClientes].nome, sizeof(cliente[numClientes].nome), stdin);
//        cliente[numClientes].nome[strcspn(cliente[numClientes].nome, "\n")] = 0;

        while (true) {
          printf("Digite o nome do cliente: ");
          fgets(cliente[numClientes].nome, sizeof(cliente[numClientes].nome), stdin);
          cliente[numClientes].nome[strcspn(cliente[numClientes].nome, "\n")] = 0;
          if (validacaracter(cliente[numClientes].nome)) {
             break;
          } else {
            printf("Nome invalido.\n");
          }
        }
        fflush(stdin);

        //NOME SOCIAL
//        printf("Digite o nome social do cliente: ");
//        fgets(cliente[numClientes].nomesocial, sizeof(cliente[numClientes].nomesocial), stdin);
//        cliente[numClientes].nomesocial[strcspn(cliente[numClientes].nomesocial, "\n")] = 0; //

        while (true) {
          printf("Digite o nome social do cliente: ");
          fgets(cliente[numClientes].nomesocial, sizeof(cliente[numClientes].nomesocial), stdin);
          cliente[numClientes].nomesocial[strcspn(cliente[numClientes].nomesocial, "\n")] = 0;
          if (validacaracter(cliente[numClientes].nomesocial)) {
             break;
          } else {
            printf("Nome social invalido.\n");
          }
        }
        fflush(stdin);

        // CPF
        while (true){

            printf("Digite o CPF do cliente: ");
            scanf("%s", &cliente[numClientes].cpf);
            if (validaNumero(cliente[numClientes].cpf, 11)){
                break;
            }
            else{
                printf("CPF invalido.\n");
            }
        }
        fflush(stdin);

        // RUA
//        printf("Digite a rua do cliente: ");
//        fgets(cliente[numClientes].rua, sizeof(cliente[numClientes].rua), stdin);
//        cliente[numClientes].rua[strcspn(cliente[numClientes].rua, "\n")] = 0;

        while (true) {
          printf("Digite a rua do cliente: ");
          fgets(cliente[numClientes].rua, sizeof(cliente[numClientes].rua), stdin);
          cliente[numClientes].rua[strcspn(cliente[numClientes].rua, "\n")] = 0;
          if (validacaracter(cliente[numClientes].rua)) {
             break;
          } else {
            printf("Rua invalida.\n");
          }
        }
        fflush(stdin);

        //NUMERO DA CASA
        while (true){

            printf("Digite o numero da residencia do cliente: ");
            scanf("%s", &cliente[numClientes].num);

            if (validaNumero(cliente[numClientes].num, 0)){
                break;
            }
            else{
                printf("numero de residencia invalido.\n");
            }
        }
        fflush(stdin);
       
//        printf("%s",produto[procuraItem(2)].descricao);

        //BAIRRO
//        printf("Digite o bairro do cliente: ");
//        fgets(cliente[numClientes].bairro, sizeof(cliente[numClientes].bairro), stdin);
//        cliente[numClientes].bairro[strcspn(cliente[numClientes].bairro, "\n")] = 0;

        while (true) {
          printf("Digite o bairro do cliente: ");
          fgets(cliente[numClientes].bairro, sizeof(cliente[numClientes].bairro), stdin);
          cliente[numClientes].bairro[strcspn(cliente[numClientes].bairro, "\n")] = 0;
          if (validacaracter(cliente[numClientes].bairro)) {
             break;
          } else {
            printf("Rua invalida.\n");
          }
        }
        fflush(stdin);

        //CEP
        while (true){

            printf("Digite o CEP do cliente: ");
            scanf("%s", &cliente[numClientes].cep);

            if (validaNumero(cliente[numClientes].cep, 8)){
                break;
            }
            else{
                printf("CEP invalido.\n");
            }
        }
        fflush(stdin);

        //TELEFONE
//        printf("Digite o telefone/whats do cliente: ");
//        fgets(cliente[numClientes].telefone, sizeof(cliente[numClientes].telefone), stdin);
//        cliente[numClientes].telefone[strcspn(cliente[numClientes].telefone, "\n")] = 0;

        while (true) {
          printf("Digite o telefone/whats do cliente: ");
          fgets(cliente[numClientes].telefone, sizeof(cliente[numClientes].telefone), stdin);
          cliente[numClientes].telefone[strcspn(cliente[numClientes].telefone, "\n")] = 0;
          if (validacaracter(cliente[numClientes].telefone)) {
             break;
          } else {
            printf("Telefone/whats invalida.\n");
          }
        }
        fflush(stdin);

        numClientes++;

        //se quiser cadastrar mais um cliente
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N')){
            system("cls");
            printf("Deseja incluir um novo Cliente? (s/n)? ");
            scanf(" %c", &confirma);
            fflush(stdin);
            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N'){
                break;
            }
            else{
                opinvalida();
            }
        }
        if ( confirma == 'n' || confirma == 'N') {
          system("cls");
          break;
        }
    }
}
//=================================================== FIM CADASTRO CLIENTES

//=================================================== CARREGAR DADOS CLIENTES
carregarDadosClientes(){
    arquivoCliente = fopen("clientes.dat", "rb");
    if (arquivoCliente)
    {
        fread(&numClientes, sizeof(int), 1, arquivoCliente);
        cliente = malloc(numClientes * sizeof(struct CadastroCliente));
        fread(cliente, sizeof(struct CadastroCliente), numClientes, arquivoCliente);
        fclose(arquivoCliente);
    }
}
//=================================================== FIM CARREGAR DADOS CLIENTES

//=================================================== CADASTRO PRODUTOS
void cadastroProduto(){

    char confirma;
    while (confirma =! 'n'||confirma != 'N'){

        produto = realloc(produto, (numProdutos + 1) * sizeof(struct CadastroProduto));
        if (produto == NULL){
            printf("Erro de alocação de memória.\n");
            exit(1);
        }


        produto[numProdutos].cod = numProdutos + 1;

        //NOME DO PRODUTO
        fflush(stdin);
        printf("Digite o nome do produto: ");
        fgets(produto[numProdutos].descricao, sizeof(produto[numProdutos].descricao), stdin);
        produto[numProdutos].descricao[strcspn(produto[numProdutos].descricao, "\n")] = 0; //
        fflush(stdin);

        //CATEGORIA DO PRODUTO
        printf("Digite a categoria do produto: ");
        fgets(produto[numProdutos].categoria, sizeof(produto[numProdutos].categoria), stdin);
        produto[numProdutos].categoria[strcspn(produto[numProdutos].categoria, "\n")] = 0; //
        fflush(stdin);

        //PRECO DE COMPRA
        while (true) {
          printf("Digite o preco de compra do produto:");
          if (scanf("%f", &produto[numProdutos].precoCompra) == 1) {
            break;
          }
          printf("preco de compra invalido\n");
          fflush(stdin);
        }

        fflush(stdin);

        //PRECO DE VENDA
        while (true) {
          printf("Digite o preco de venda do produto:");
          if (scanf("%f", &produto[numProdutos].precoVenda) == 1) {
            break;
          }
          printf("preco de venda invalido\n");
          fflush(stdin);
        }

        fflush(stdin);

        //ESTOQUE
        printf("Digite o estoque do produto:");
        scanf("%d", &produto[numProdutos].estoque);
        fflush(stdin);
        numProdutos++;

        //se quiser cadastrar mais um produto
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N')){

            system("cls");
            printf("Deseja incluir um novo Produto? (s/n)? ");
            scanf(" %c", &confirma);
            fflush(stdin);

            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N'){
                break;
            }
            else{
                opinvalida();
            }
        }
        if ( confirma == 'n' || confirma == 'N') {
          system("cls");
          break;
        }
     }
}
//=================================================== FIM CADASTRO PRODUTOS

//=================================================== CARREGAR DADOS DE PRODUTOS
carregarDadosProdutos(){

    arquivoProduto = fopen("produtos.dat", "rb");
    if (arquivoProduto){
        fread(&numProdutos, sizeof(int), 1, arquivoProduto);
        produto = malloc(numProdutos * sizeof(struct CadastroProduto));
        fread(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
        fclose(arquivoProduto);
    }
}
//=================================================== FIM CARREGAR DADOS PRODUTOS

//=================================================== PAGAMENTO
void pagamento(){
    totalBruto=50;

    system("cls");
    //caso não haja compras no carrinho
    if(totalBruto==0){
        printf("\n|-----------------------------------------------|\n");
        printf("|  NENHUMA COMPRA REALIZADA, FAVOR ADICIONAR    |\n");
        printf("|  ITENS PARA PAGAMENTO.                        |\n");
        printf("|                                               |\n");
        printf("|  VOLTANDO PARA O MENU PRINCIPAL...            |\n");
        printf("|-----------------------------------------------| \n\n");
        system("pause");
        system("cls");
        main();
    }
    //caso o carrinho tenha mais que R$ 0
    //forma de pagamento
    while (totalBruto>0){

        if (totalBruto>0){
            int pag=0;
            printf("\n|--------------------------------------------------|\n");
            printf("|  Escolha sua forma de pagamento:                 |\n");
            printf("|                                                  |\n");
            printf("|  1. Dinheiro/PIX                                 |\n");
            printf("|  2. Cartao de Debito/Credito                     |\n");
            printf("|  3. Voltar ao menu principal                     |\n");
            printf("|--------------------------------------------------|\n\n");

            //parte que vai printar os produtos comprados
            printf("\n\n|--------------------------------------------------|\n\n");
            scanf("%d", &pag);
            system("cls");

            switch (pag){

            case 1:
                if (loopDesconto==0){
                printf("\n Digite a porcentagem de desconto: ");
                scanf("%f", &desc);
                desc = desc / 100;
                desconto = totalBruto * desc;
                }
                // Trunca o valor do desconto em 2 decimais
                char valorString[10]; // Buffer para armazenar a string formatada
                sprintf(valorString, "%.2f", desconto);
                desconto=atof(valorString);

                totalDesc=totalBruto-desconto;
                totalDesc-=pagoDin;
                totalDesc-=pagoCartao;

                printf("\n|------------------------------------------------|\n");
                printf("|  Pagamento em dinheiro:                        |\n");
                printf("|                                                |\n");
                printf("|  Digite a quantia em dinheiro entregue:        |\n");
                printf("|------------------------------------------------|\n");
                printf("  TOTAL BRUTO: R$ %.2f                        \n", totalBruto);
                printf("  COMPRA COM DESCONTO DE: R$ %.2f\n", desconto);
                printf("  TOTAL A PAGAR: R$ %.2f\n", totalDesc);
                printf("|------------------------------------------------|\n");
                printf("  R$ ");
                scanf("%f", &pagoDin);

                if (totalDesc<pagoDin){
                    troco = pagoDin-totalDesc;
                    printf("|------------------------------------------------|\n");
                    printf("  TROCO A PAGAR: R$ %.2f                        \n", troco);
                    printf("|------------------------------------------------|\n\n");
                    pagoDin=totalDesc;
                    system("pause");
                    system("cls");
                }

                if (totalDesc==pagoDin){
                    printf("\n|-------------------------------------------------|\n");
                    printf("|  PAGAMENTO REALIZADO COM SUCESSO,               |\n");
                    printf("|  VOLTANDO PARA O MENU PRINCIPAL...              |\n");
                    printf("|-------------------------------------------------| \n\n");
                    system("pause");
                    system("cls");
                if (loopDesconto==0){
                    vendasRealizadas++;
                    vCaixa=inicioCaixa+totalDesc;
                    totalDin=totalDin+pagoDin;
                    totalBruto=0;
                    totalDesc=0;
                    desconto=0;
                    troco=0;
                    pagoDin=0;
                    pagoCartao=0;
                    loopDesconto=0;
                    main();
                }
                if (loopDesconto==1){
                    vendasRealizadas++;
                    vCaixa=inicioCaixa+totalDesc;
                    totalDinCartao=totalDinCartao+pagoDin+pagoCartao;
                    totalBruto=0;
                    totalDesc=0;
                    desconto=0;
                    troco=0;
                    pagoDin=0;
                    pagoCartao=0;
                    loopDesconto=0;
                    system("pause");
                    main();
                }
                }

                else if (totalDesc>pagoDin){
                    troco = totalDesc-pagoDin;
                    printf("\n|--------------------------------------------------------------|\n");
                    printf("|  PAGAMENTO INSUFICIENTE, PAGAR RESTANTE DE OUTRA MANEIRA?    |\n");
                    printf("|                                                              |\n");
                    printf("|  1. Sim                                                      |\n");
                    printf("|  2. Voltar ao Menu Principal (Cancelar Pagamento Atual)      |\n");
                    printf("|--------------------------------------------------------------| \n");
                    printf("   RESTANTE A PAGAR: R$ %.2f\n\n", troco);

                    scanf("%d", &pag);
                    system("cls");

                    if (pag==1){
                        totalDesc-=pagoDin;
                        loopDesconto=1;
                    }
                    else{
                    if (pag==2){
                        voltar();
                        loopDesconto=0;
                        pagoDin=0;
                        break;
                    }
                }
                }
                break;

        case 2:

        totalDesc=totalBruto-desconto;
        while(totalDesc!=pagoCartao){
        totalDesc-=pagoDin;
        totalDesc-=pagoCartao;


        printf("\n|------------------------------------------------|\n");
        printf("|  Pagamento em Cartao de Credito/Debito         |\n");
        printf("|                                                |\n");
        printf("|  Digite a quantia em cartao:                   |\n");
        printf("|------------------------------------------------|\n");
        printf("  TOTAL A PAGAR: R$ %.2f \n", totalDesc);
        printf("|------------------------------------------------|\n");
        printf("   R$ ");
        scanf("%f", &pagoCartao);

        // Trunca o valor do total em 2 decimais
        char valorString[10]; // Buffer para armazenar a string formatada
        sprintf(valorString, "%.2f", totalDesc);
        totalDesc=atof(valorString);

        if (totalDesc==pagoCartao){
            printf("\n\n|-------------------------------------------------|\n");
            printf("|  PAGAMENTO REALIZADO COM SUCESSO,               |\n");
            printf("|  VOLTANDO PARA O MENU PRINCIPAL...              |\n");
            printf("|-------------------------------------------------| \n\n");
            system("pause");
            system("cls");

        if (loopDesconto==0){
            vendasRealizadas++;
            vCaixa=inicioCaixa+totalDesc;
            totalCartao=totalCartao+pagoCartao;
            totalBruto=0;
            totalDesc=0;
            desconto=0;
            pagoDin=0;
            pagoCartao=0;
            loopDesconto=0;
            main();
        }


        if (loopDesconto==1){
                vendasRealizadas++;
                vCaixa=inicioCaixa+totalDesc;
                totalDinCartao=totalDinCartao+pagoDin+pagoCartao;
                totalBruto=0;
                totalDesc=0;
                desconto=0;
                troco=0;
                pagoDin=0;
                pagoCartao=0;
                loopDesconto=0;
                main();
            }
        }


        else if (totalDesc>pagoCartao){
            printf("\n|--------------------------------------------------------------|\n");
            printf("|  PAGAMENTO INSUFICIENTE, PAGAR RESTANTE DE OUTRA MANEIRA?    |\n");
            printf("|                                                              |\n");
            printf("|  1. Sim                                                      |\n");
            printf("|  2. Voltar ao Menu Principal (Cancelar Pagamento Atual)      |\n");
            printf("|--------------------------------------------------------------| \n\n");
            scanf("%d", &pag);
            system("cls");

            if (pag==1){
                totalDesc-=pagoCartao;
                loopDesconto=1;
            }
            if (pag==2){
                voltar();
                loopDesconto=0;
                pagoCartao=0;
                break;
            }
            else{
                if (pag==2){
                    voltar();
                }
            }
            break;
        }

        else if (totalDesc<pagoCartao){
            printf("\n|--------------------------------------------------------------|\n");
            printf("|  PAGAMENTO MAIOR QUE O TOTAL A PAGAR, FAVOR TENTE            |\n");
            printf("|  REALIZAR O PAGAMENTO NOVAMENTE...                           |\n");
            printf("|--------------------------------------------------------------| \n\n");
            system("pause");
            system("cls");
        }
    }


                break;
            case 3:
                voltar();
                main();
                break;

            default:
                opinvalida();
            }
        }
    }
}
//=================================================== FIM PAGAMENTOS

//=================================================== SANGRIA
void Sangria(){

    if (vCaixa<50){
    printf("\n|----------------------------------------------------|\n");
    printf("|  O CAIXA ESTA COM MENOS DE R$ 50.00...             |\n");
    printf("|----------------------------------------------------|\n");
    system("pause");
    }
    else{
    printf("\n|----------------------------------------------------|\n");
    printf("|                    SANGRIA                         |\n");
    printf("|                                                    |\n");
    printf("|  Digite a quantia em dinheiro a ser retirado:      |\n");
    printf("|----------------------------------------------------|\n");
    printf("  TOTAL EM DINHEIRO NO CAIXA: R$ %.2f\n", vCaixa);
    printf("|----------------------------------------------------|\n");
    scanf("%f", &retirada);

    if (retirada==0){
    printf("\n  NADA FOI RETIRADO, SEU CAIXA CONTINUA COM R$ %.2f\n", vCaixa);
    system("pause");
    }

    if ((vCaixa-retirada)<50){
    printf("\n|---------------------------------------------------------------|\n");
    printf("|  O CAIXA  FICARA COM MENOS DE R$ 50.00, SAQUE IMPOSSIVEL...    |\n");
    printf("|----------------------------------------------------------------|\n");
    system("pause");
    }
    if ((vCaixa-retirada)>=50){
    vCaixa=vCaixa-retirada;
    printf("  DINHEIRO RETIRADO, SEU CAIXA AGORA ESTA COM R$ %.2f\n", vCaixa);
    system("pause");
    }

    }
}
//=================================================== FIM SANGRIA

//=================================================== VENDAS
novaVenda(){

}
//=================================================== FIM VENDAS

//=================================================== RELATÓRIOS
//função de listar clientes
void listaClientes(){
    int i;
    for (int i = 0; i < numClientes; i++){
        printf("\n|----------------------------------------------------------|\n");
        printf("| Codigo.....: %d\n", cliente[i].cod);
        printf("| Nome.......: %s\n", cliente[i].nome);
        printf("| Nome social: %s\n", cliente[i].nomesocial);
        printf("| CPF........: %s\n", cliente[i].cpf);
        printf("| Rua........: %s\n", cliente[i].rua);
        printf("| Numero.....: %s\n", cliente[i].num);
        printf("| Bairro.....: %s\n", cliente[i].bairro);
        printf("| CEP........: %s\n", cliente[i].cep);
        printf("| Telefone...: %s\n", cliente[i].telefone);
    }
    system("pause");
}

//função de listar produtos
void listaProdutos(){

    int i;
    for (int i = 0; i < numProdutos; i++){
        printf("\n|----------------------------------------------------------|\n");
        printf("| Codigo.....: %d\n", produto[i].cod);
        printf("| Descricao.......: %s\n", produto[i].descricao);
        printf("| Categoria: %s\n", produto[i].categoria);
        printf("| Preco de Compra...: %f\n", produto[i].precoCompra);
        printf("| Preco de Venda...: %f\n", produto[i].precoVenda);
        printf("| Estoque Atual...: %d\n", produto[i].estoque);
    }
    system("pause");
}

//função de listar vendas
void listaVendas()
{
}
//=================================================== FIM RELATÓRIOS

//=================================================== FUNÇÃO PRINCIPAL
void main(){
    carregarDadosClientes();
    carregarDadosProdutos();
    do{
        //menu principal
        menuPrincipal();

        switch(opcao){

        //menu de cadastros
        case 1:
            menuCadastros();
            break;

        //menu de vendas
        case 2:
            menuVendas();
            break;

        //abertura de caixa
        case 3:
            //se o caixa já estiver aberto
            if (caixa == 'A'){
                caixaAberto();
            }

            else{
                aberturaCaixa();
            }
            break;

        //fechamento de caixa
        case 4:

            //se o caixa já estiver fechado
            if (caixa == 'F'){
                caixaFechado();
            }

            else{
                fechamentoCaixa();
            }
            break;

        //relatorios
        case 5:
            menuRelatorios();
            break;

        //sair do programa
        case 6:
            break;

        //caso não seja nenhuma das opções de menu
        default:
            opinvalida();
            break;
        }
    }
    while(opcao != 6);
    saida();

    // Liberação de memória
    free(cliente);
    free(produto);
    //free(vendas);
    //free(pagamentos);

}