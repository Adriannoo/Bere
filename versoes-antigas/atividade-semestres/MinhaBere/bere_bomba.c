#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>


struct CadastroCliente *cliente = NULL;
struct CadastroProduto *produto = NULL;
struct CadastroVenda *venda = NULL;
struct ProdutoVenda *produtovenda = NULL;
struct SituacaoCaixa *posicaoCaixa = NULL;

#define INICIAL_MAX_PRODUTOS 10
#define INICIAL_MAX_VENDAS 10

#define LOGIN_MIN 8
#define LOGIN_MAX 12
#define PASSWORD_MIN 6
#define PASSWORD_MAX 8

//Venda *vendas = NULL;
//Pagamento *pagamentos = NULL;
int numClientes = 0, numProdutos = 0, numVendas = 0, numPagamentos = 0, opcao = 0, loopDesconto, indProduto;
int vendasRealizadas = 0, qtd=0;
float inicioCaixa = 0.0, totalCartao = 0.0, totalDin = 0.0, totalDinCartao = 0.0, totalFat= 0.0, vCaixa = 0.0;
float totalDesc = 0, totalBruto = 0, pagoDin = 0, pagoCartao= 0.0, desc = 0, desconto = 0, troco = 0.0, retirada = 0.0, margem = 0;
char caixa = 'F';
FILE *arquivoCliente, *arquivoProduto , *arquivoCaixa, *arquivoVenda;

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
    char telefone[30];
};

struct Cadastrousuario{
    int cod;
    char login[LOGIN_MAX + 1];
    char password[PASSWORD_MAX + 1];
    int tipo; // 1: Administrador, 2: Usuário
};

struct CadastroProduto{
    int cod;
    char descricao[30];
    char categoria[30];
    float precoCompra;
    float margem;
    float precoVenda;
    int estoque;
    int estoqueminimo;
};

struct Data {
    int dia;
    int mes;
    int ano;
};

struct Data obterDataAtual() {
    struct Data dataAtual;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    dataAtual.dia = tm.tm_mday;
    dataAtual.mes = tm.tm_mon + 1;
    dataAtual.ano = tm.tm_year + 1900;
    return dataAtual;
}

struct ProdutoVenda{
    int num;
    int cod;
    char descricao[30];
    int quantidade;
    float precoVenda;
    float valorTotal;
};

struct CadastroVenda{
    int num;
    int cliente;
    char nomeCliente[30];
    struct Data data;
    float valorTotal;
    int totalProdutos;
    struct ProdutoVenda *produtos;
};
//    int maxProdutos;};

struct SituacaoCaixa{
    char caixa;
    float inicioCaixa;
    float totalDin;
    float totalCartao;
    float vCaixa;
};


//função que impede letras onde pede-se números
bool validaNumero(const char *numero, int tamanho){
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
bool validacaracter(const char *caracter){
    int len = strlen(caracter);
    if (len == 0){
        return false;
    }
    return true;
}

float truncar(float valor, int casas_decimais) {
    float potencia = pow(10.0f, casas_decimais);
    return truncf(valor * potencia) / potencia;
}

int pesquisaItem(int codigo){
    int i;
    int resultado =-1;
    for (int i = 0; i < numProdutos; i++){
        if (produto[i].cod==codigo){
          resultado = i;
        }
    }
    return resultado;
}

int pesquisaCliente(int codigo){
    int i;
    int resultado =-1;
    for (int i = 0; i < numClientes; i++){
        if (cliente[i].cod==codigo){
          resultado = i;
        }
    }
    return resultado;
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
        vCaixa = vCaixa + inicioCaixa;
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
//                main();
                break;
            }

            //voltar para menu principal
            if (fechar == 2){
                voltar();
//                main();
                system("cls");
                break;
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
            system("pause");
            break;

        //listagem de vendas
        case 3:
            listaVendas();
            system("pause");
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
                    printf("\nPasso 1...\n\n");

                    arquivoCliente = fopen("clientes.dat", "wb");
                    fwrite(&numClientes, sizeof(int), 1, arquivoCliente);
                    fwrite(cliente, sizeof(struct CadastroCliente), numClientes, arquivoCliente);
                    fclose(arquivoCliente);

                    arquivoProduto = fopen("produtos.dat", "wb");
                    fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                    fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                    fclose(arquivoProduto);

                    arquivoVenda = fopen("vendas.dat", "wb");
                    fwrite(&numVendas, sizeof(int), 1, arquivoVenda);
                    for (int i = 0; i < numVendas; i++) {
                      fwrite(&venda[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);
                      fwrite(venda[i].produtos, sizeof(struct ProdutoVenda), venda[i].totalProdutos, arquivoVenda);
                    }
                    fclose(arquivoVenda);

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

        if (sair == 2){
            voltar();
            main();
            system("cls");
        }

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
                fflush(stdin);
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
              fflush(stdin);
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
}//=================================================== FIM CADASTRO CLIENTES

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

        produto[numProdutos].precoCompra=0;
        produto[numProdutos].precoVenda=0;
        produto[numProdutos].margem=0;

        produto[numProdutos].cod = numProdutos + 1;

        //NOME DO PRODUTO
        fflush(stdin);
        while (true) {
          printf("Digite o nome do produto: ");
          fgets(produto[numProdutos].descricao, sizeof(produto[numProdutos].descricao), stdin);
          produto[numProdutos].descricao[strcspn(produto[numProdutos].descricao, "\n")] = 0;
          if (validacaracter(produto[numProdutos].descricao)) {
             break;
          } else {
            printf("Nome invalido.\n");
          }
        }
        fflush(stdin);

        while (true) {
          printf("Digite a categoria do produto: ");
          fgets(produto[numProdutos].categoria, sizeof(produto[numProdutos].categoria), stdin);
          produto[numProdutos].categoria[strcspn(produto[numProdutos].categoria, "\n")] = 0; //
          if (validacaracter(produto[numProdutos].categoria)) {
             break;
          } else {
            printf("Categoria invalida.\n");
          }
        }

        fflush(stdin);

        //PRECO DE COMPRA
        while (true) {
          printf("Digite o preco de compra do produto: ");
          if (scanf("%f", &produto[numProdutos].precoCompra) == 1) {
            break;
          }
          printf("preco de compra invalido\n");
          fflush(stdin);
        }

        fflush(stdin);

        //MARGEM DE LUCRO
        while (true) {
          printf("Digite a margem de lucro do produto: ");
          if (scanf("%f", &produto[numProdutos].margem) == 1) {
            break;
          }
          printf("margem invalida\n");
          fflush(stdin);
        }

        fflush(stdin);

        margem = produto[numProdutos].margem / 100;

        produto[numProdutos].precoVenda = produto[numProdutos].precoCompra + (produto[numProdutos].precoCompra * margem);

        produto[numProdutos].precoVenda=truncar(produto[numProdutos].precoVenda, 2);

        printf("Preco de venda do produto: %.2f\n", produto[numProdutos].precoVenda);

    // Calcular o preço de venda com a margem de lucro

/*
        //PRECO DE VENDA
        while (true) {
          printf("Digite o preco de venda do produto: ");
          if (scanf("%f", &produto[numProdutos].precoVenda) == 1) {
            break;
          }
          printf("preco de venda invalido\n");
          fflush(stdin);
        }

        fflush(stdin);
*/
        //ESTOQUE
        printf("Digite o estoque do produto: ");
        scanf("%d", &produto[numProdutos].estoque);
        fflush(stdin);

        printf("Digite o estoque minimo do produto: ");
        scanf("%d", &produto[numProdutos].estoqueminimo);
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

//=================================================== CARREGAR DADOS DE PRODUTOS
carregarDadosCaixa(){

    arquivoCaixa = fopen("caixa.dat", "rb");
    if (arquivoCaixa){
//        fread(&numProdutos, sizeof(int), 1, arquivoCaixa);
//        produto = malloc(numProdutos * sizeof(struct CadastroProduto));
        fread(posicaoCaixa, sizeof(struct SituacaoCaixa), 1, arquivoCaixa);
        fclose(arquivoCaixa);
        caixa=posicaoCaixa->caixa;
        inicioCaixa=posicaoCaixa->inicioCaixa;
        totalCartao=posicaoCaixa->totalCartao;
        totalDin=posicaoCaixa->totalDin;
        vCaixa=posicaoCaixa->vCaixa;
    }
}
//=================================================== FIM CARREGAR DADOS PRODUTOS

int carregarDadosVendas(struct CadastroVenda **venda){
    arquivoVenda = fopen("vendas.dat", "rb");
    numVendas = 0;
    if (arquivoVenda)
    {
        fread(&numVendas, sizeof(int), 1, arquivoVenda);
//        venda = malloc(numVendas * sizeof(struct CadastroVenda));
//        fread(venda, sizeof(struct CadastroVenda), numVendas, arquivoVenda);

       *venda = (struct CadastroVenda *)malloc(numVendas * sizeof(struct CadastroVenda));
       if (*venda == NULL) {
         perror("Erro ao alocar memória para vendas");
         exit(EXIT_FAILURE);
       }

       for (int i = 0; i < numVendas; i++) {
          fread(&(*venda)[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);

//          printf("| Qtd Itens: %d\n", (*venda)[i].totalProdutos);
//          system("pause");

          (*venda)[i].produtos = (struct ProdutoVenda *)malloc((*venda)[i].totalProdutos * sizeof(struct ProdutoVenda));
          if ((*venda)[i].produtos == NULL) {
            perror("Erro ao alocar memória para produtos");
            exit(EXIT_FAILURE);
          }
          fread((*venda)[i].produtos, sizeof(struct ProdutoVenda), (*venda)[i].totalProdutos, arquivoVenda);
       }

       fclose(arquivoVenda);

    }
    return numVendas;
}

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
           //     char valorString[10]; // Buffer para armazenar a string formatada
           //     sprintf(valorString, "%.2f", desconto);
           //     desconto=atof(valorString);

                desconto=truncar(desconto, 2);

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
//        char valorString[10]; // Buffer para armazenar a string formatada
//        sprintf(valorString, "%.2f", totalDesc);
//        totalDesc=atof(valorString);
        totalDesc=truncar(totalDesc,2);
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
    if (caixa=='F') {
      caixaFechado();
    }
    else{
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
      else{
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
    }
}
//=================================================== FIM SANGRIA

//=================================================== VENDAS
/*
novaVenda1(){
  if (caixa=='F') {
    caixaFechado();
  }
  else{

    while (true) {
      listaProdutos();
      int codigo;
      printf("\nDigite o codigo do produto desejado(0 para sair): ");
      scanf("%d", &codigo);
      if (codigo==0){
        system("cls");
        break;
      }

      indProduto=pesquisaItem(codigo);
      if (indProduto==-1){
        printf("\nProduto nao cadastrado.\n\n");
        system("pause");
        system("cls");
        continue;
      }

      printf("Produto Escolhido: %s\n", produto[indProduto].descricao);
      printf("Preco: %.2f\n", produto[indProduto].precoVenda);

      printf("Insira a Quantidade Desejada: ");
      scanf("%d", &qtd);
      if ((produto[indProduto].estoque-qtd)<0){
        printf("\nEssa quantidade excede o estoque disponivel para compra... Tente Novamente.\n\n");
//        scanf("%d", &qtd);
        system("pause");
        system("cls");
        continue;
      }

      if (produto[indProduto].estoque<=0){
        printf("\nO produto esta sem estoque.\n\n");
        system("pause");
        system("cls");
        continue;
      }
      if (produto[indProduto].estoque<=produto[indProduto].estoqueminimo){
        printf("\nEstoque minimo alcancado.\n\n");
      }
//        if ((produto[indProduto].estoque-qtd)>0){
//produto[numProdutos-1].estoque=produto[numProdutos-1].estoque-qtd;
//          printf("Produto Escolhido: %d", produto[indProduto].estoque);
//          system("pause");

      venda = realloc(venda, (numVendas + 1) * sizeof(struct CadastroVenda));
      if (venda == NULL){
         printf("Erro de alocação de memória.\n");
         exit(1);
      }

      venda[numVendas].num = numVendas + 1;
      venda[numVendas].produtos.cod=produto[indProduto].cod;
 //     venda[numVendas].descricao=produto[indProduto].descricao;
      strcpy(venda[numVendas].descricao , produto[indProduto].descricao);
      venda[numVendas].precoVenda=produto[indProduto].precoVenda;
      venda[numVendas].quantidade=qtd;

//      printf("Qtd do Item: %d\n", venda[numVendas].quantidade);
//      printf("Preco do Item: %.2f\n", venda[numVendas].precoVenda);

      produto[indProduto].estoque=produto[indProduto].estoque-qtd;

      printf("Total do Item: %.2f\n", venda[numVendas].precoVenda*venda[numVendas].quantidade);

      numVendas++;

      system("pause");
      system("cls");

    }
  }
}
*/

void adicionarProduto(struct ProdutoVenda *produtovenda, int num, int cod, const char *descricao, int quantidade, float precoVenda) {
    produtovenda->num = num;
    produtovenda->cod = cod;
    strncpy(produtovenda->descricao, descricao, sizeof(produtovenda->descricao) - 1);
    produtovenda->descricao[sizeof(produto->descricao) - 1] = '\0';
    produtovenda->quantidade = quantidade;
    produtovenda->precoVenda = precoVenda;
    produtovenda->valorTotal = quantidade * precoVenda;
}

void adicionarVenda(struct CadastroVenda *venda, int num, int cliente, const char *nomeCliente, struct Data data) {
    venda->num = num;
    venda->cliente = cliente;
    strncpy(venda->nomeCliente, nomeCliente, sizeof(venda->nomeCliente) - 1);
    venda->data = data;
    venda->valorTotal = 0.0f;
    venda->totalProdutos = 0;
//    venda->maxProdutos = INICIAL_MAX_PRODUTOS;
//    venda->produtos = (struct ProdutoVenda *)malloc(venda->maxProdutos * sizeof(struct ProdutoVenda));
//    venda->produtos = (struct ProdutoVenda *)malloc((venda->totalProdutos+1) * sizeof(struct ProdutoVenda));
    venda->produtos = NULL;
//    if (venda->produtos == NULL) {
//        perror("Erro ao alocar memoria para produtos");
//        exit(EXIT_FAILURE);
//    }
}

novaVenda(){
  if (caixa=='F') {
    caixaFechado();
  }
  else{
    int continuarVenda = 1;
    while (continuarVenda) {
//        if (numVendas >= INICIAL_MAX_VENDAS) {
            venda = (struct CadastroVenda *)realloc(venda, (numVendas + 1) * sizeof(struct CadastroVenda));
            if (venda == NULL) {
                perror("Erro ao alocar memoria para vendas");
                exit(EXIT_FAILURE);
            }
//        }
        int codcliente;
        struct Data data = obterDataAtual();

        printf("Digite o codigo do cliente: ");
        scanf("%d", &codcliente);

        indProduto=pesquisaCliente(codcliente);
        if (indProduto==-1){
          printf("\nCliente nao cadastrado.\n\n");
          system("pause");
          system("cls");
          continue;
        }

        printf("Nome Cliente: %s\n", cliente[indProduto].nome);

        char nomeCliente[30];
        strncpy(nomeCliente, cliente[indProduto].nome, sizeof(cliente[indProduto].nome) - 1);

        adicionarVenda(&venda[numVendas], numVendas+1, codcliente, nomeCliente, data);

        int continuarProduto = 1;
        while (continuarProduto) {
//            if (venda[numVendas].totalProdutos >= venda[numVendas].maxProdutos) {
//                venda[numVendas].maxProdutos *= 2;
//                venda[numVendas].produtos = (struct ProdutoVenda *)realloc(venda[numVendas].produtos, venda[numVendas].maxProdutos * sizeof(struct ProdutoVenda));
                venda[numVendas].produtos = (struct ProdutoVenda *)realloc(venda[numVendas].produtos, (venda[numVendas].totalProdutos+1) * sizeof(struct ProdutoVenda));
                if (venda[numVendas].produtos == NULL) {
                    perror("Erro ao alocar memoria para produtos");
                    exit(EXIT_FAILURE);
                }
//            }

            int cod;
            char descricao[30];
            int quantidade;
            float precoVenda;

            printf("Digite o codigo do produto: ");
            scanf("%d", &cod);

            indProduto=pesquisaItem(cod);
            if (indProduto==-1){
              printf("\nProduto nao cadastrado.\n\n");
              system("pause");
              system("cls");
              continue;
            }
//            descricao = produto[indProduto].descricao;
            strncpy(descricao, produto[indProduto].descricao, sizeof(produto[indProduto].descricao) - 1);
            precoVenda = produto[indProduto].precoVenda;

            printf("Produto Escolhido: %s\n", produto[indProduto].descricao);
            printf("Preco: %.2f\n", produto[indProduto].precoVenda);

//------------------ Incluido ----------------------------------------------------------------------
            if (produto[indProduto].estoque<=0){
              printf("\nO produto esta sem estoque.\n\n");
              system("pause");
              system("cls");
              continue;
            }
            if (produto[indProduto].estoque<=produto[indProduto].estoqueminimo){
              printf("\nEstoque minimo alcancado.\n\n");
            }
//----------------------------------------------------------------------------------------

//            printf("Digite a descrição do produto: ");
//            scanf(" %[^\n]", descricao);

            printf("Digite a quantidade: ");
            scanf("%d", &quantidade);
//            printf("Digite o preço de venda: ");
//            scanf("%f", &precoVenda);

//------------------ Incluido ----------------------------------------------------------------------
            if ((produto[indProduto].estoque-quantidade)<0){
              printf("\nEssa quantidade excede o estoque disponivel para compra... Tente Novamente.\n\n");
//              scanf("%d", &qtd);
              system("pause");
              system("cls");
              continue;
            }
//----------------------------------------------------------------------------------------

            adicionarProduto(&venda[numVendas].produtos[venda[numVendas].totalProdutos], numVendas+1, cod, descricao, quantidade, precoVenda);
            venda[numVendas].valorTotal += quantidade * precoVenda;
            venda[numVendas].totalProdutos++;

//-------------------- Incluido --------------------------------------------------------------------
            produto[indProduto].estoque=produto[indProduto].estoque-quantidade;
//----------------------------------------------------------------------------------------

            printf("Deseja adicionar outro produto? (1-Sim, 0-Nao): ");
            scanf("%d", &continuarProduto);
        }

        numVendas++;
//        totalVendas++;
        printf("Deseja adicionar outra venda? (1-Sim, 0-Nao): ");
        scanf("%d", &continuarVenda);
    }
  }
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
        printf("| Preco de Compra...: %.2f\n", produto[i].precoCompra);
        printf("| Preco de Venda...: %.2f\n", produto[i].precoVenda);
        printf("| Estoque Atual...: %d\n", produto[i].estoque);
    }
}

//função de listar vendas
void listaVendas()
{
    int i;
    int j;
    for (int i = 0; i < numVendas; i++){
        printf("\n|----------------------------------------------------------|\n");
        printf("| Numero.: %d\n", venda[i].num);
        printf("| Data...: %02d/%02d/%d\n", venda[i].data.dia, venda[i].data.mes, venda[i].data.ano);
        printf("| Cliente: %s\n", venda[i].nomeCliente);
        printf("| Total..: %.2f\n", venda[i].valorTotal);

        printf("| Qtd Itens: %d\n", venda[i].totalProdutos);
//        printf("|   Codigo........: %d\n", venda[i].produtos[0].cod);
//        printf("|   Descricao.....: %s\n", venda[i].produtos[0].descricao);

        for (int j = 0; j < venda[i].totalProdutos; j++){
          printf("|   Codigo........: %d\n", venda[i].produtos[j].cod);
          printf("|   Descricao.....: %s\n", venda[i].produtos[j].descricao);
          printf("|   Quantidade....: %d\n", venda[i].produtos[j].quantidade);
          printf("|   Preco de Venda: %.2f\n", venda[i].produtos[j].precoVenda);
          printf("|   Total.........: %.2f\n", venda[i].produtos[j].valorTotal);
        }

    }
}
//=================================================== FIM RELATÓRIOS

//=================================================== FUNÇÃO PRINCIPAL
void main(){
//    carregarDadosCaixa();
    carregarDadosClientes();
    carregarDadosProdutos();
    carregarDadosVendas(&venda);
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
}
