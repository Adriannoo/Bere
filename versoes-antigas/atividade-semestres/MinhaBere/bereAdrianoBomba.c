#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define INICIAL_MAX_PRODUTOS 10
#define INICIAL_MAX_VENDAS 10
#define LOGIN_MIN 8
#define LOGIN_MAX 12
#define PASSWORD_MIN 6
#define PASSWORD_MAX 8

struct CadastroCliente *cliente = NULL;
struct CadastroUsuario *usuario = NULL;
struct CadastroCategoria *categoria = NULL;
struct CadastroProduto *produto = NULL;
struct CadastroVenda *venda = NULL;
struct ProdutoVenda *produtovenda = NULL;
struct SituacaoCaixa *posicaoCaixa = NULL;

int contadorMain = 0, numClientes = 0, numProdutos = 0, numVendas = 0, numPagamentos = 0, opcao = 0, loopDesconto=0, indProduto=0;
int vendasRealizadas = 0, qtd=0;
float inicioCaixa = 0.0, totalCartao = 0.0, totalDin = 0.0, totalDinCartao = 0.0, totalFat= 0.0, vCaixa = 0.0;
float totalDesc = 0, totalBruto = 0, pagoDin = 0, pagoCartao= 0.0, desc = 0, desconto = 0, troco = 0.0, retirada = 0.0, margem = 0;
char caixa = 'F', tipoUsuario = ' ';
char nomeUsuario[LOGIN_MAX + 1];
int numusuarios, numcategorias;

FILE *arquivoCliente, *arquivoProduto, *arquivoCaixa, *arquivoVenda, *arquivoUsuario, *arquivoCategoria, *arquivoCaixa;

//declaração de structs
//struct de cadastro de usuarios
struct CadastroUsuario {
    int cod;
    char login[LOGIN_MAX + 1];
    char password[PASSWORD_MAX + 1];
    char tipo; // 1: Administrador, 2: Usuário
};

//struct de cadastro de clientes
struct CadastroCliente {
    int cod;
    char nome[30];
    char nomesocial[30];
    char cpf[12];
    char rua[30];
    char num[8];
    char bairro[30];
    char cep[9];
    char telefone[30];
};

// Cadastro de categoria
struct CadastroCategoria {
    int cod;
    char descricao[30];
};

//struct de cadastro de produtos
struct CadastroProduto {
    int cod;
    char descricao[30];
    struct CadastroCategoria categoria;
    float precoCompra;
    float margem;
    float precoVenda;
    int estoque;
    int estoqueminimo;
};

struct ProdutoVenda {
    int num;
    int cod;
    char descricao[30];
    int quantidade;
    float precoVenda;
    float valorTotal;
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

struct CadastroVenda {
    int num;
    int cliente;
    char nomeCliente[30];
    struct Data data;
    float valorTotal;
    int totalProdutos;
    struct ProdutoVenda *produtos;
    char metodoPagamento;
};

struct SituacaoCaixa {
    char caixa;
    float inicioCaixa;
    float totalDin;
    float totalCartao;
    float vCaixa;
};

// Função para ler uma data do usuário
struct Data lerData() {
    struct Data data;
    printf("Digite a data no formato DD MM AAAA: ");
    scanf("%d %d %d", &data.dia, &data.mes, &data.ano);
    return data;
}

int compararDatas(const struct Data d1, const struct Data d2) {
    if (d1.ano > d2.ano) return 1;
    if (d1.ano < d2.ano) return -1;
    if (d1.mes > d2.mes) return 1;
    if (d1.mes < d2.mes) return -1;
    if (d1.dia > d2.dia) return 1;
    if (d1.dia < d2.dia) return -1;
    return 0;
}

int dataDentroDoPeriodo(struct Data data, struct Data inicio, struct Data fim) {
    return (compararDatas(data, inicio) >= 0 && compararDatas(data, fim) <= 0);
}

//função que impede letras onde pede-se números
bool validaNumero(const char *numero, const int tamanho) {
    if (numero==NULL) {
        return false;
    }
    const int len = strlen(numero);
    if ((tamanho>0) && (len != tamanho)) {
        return false;
    }
    for (int i = 0; i < len; i++) {
        if (!isdigit(numero[i])) {
            return false;
        }
    }
    return true;
}

//função que impede mandar espaço vazio
bool validacaracter(const char *caracter) {
    const int len = strlen(caracter);
    if (len == 0) {
        return false;
    }
    return true;
}

float truncar(const float valor, const int casas_decimais) {
    const float potencia = pow(10.0f, casas_decimais);
    return truncf(valor * potencia) / potencia;
}

int pesquisaItem(int codigo) {
    int resultado =- 1;
    for (int i = 0; i < numProdutos; i++) {
        if (produto[i].cod == codigo) {
            resultado = i;
        }
    }
    return resultado;
}

int pesquisaCliente(int codigo) {
    int resultado =-1;
    for (int i = 0; i < numClientes; i++) {
        if (cliente[i].cod==codigo) {
            resultado = i;
        }
    }
    return resultado;
}

int pesquisaCategoria(int codigo) {
    int resultado =-1;
    for (int i = 0; i < numcategorias; i++) {
        if (categoria[i].cod == codigo) {
            resultado = i;
        }
    }
    return resultado;
}

//comparar nome de clientes para ordem alfabetica
int compararNome(const void *a, const void *b) {
    return strcmp(((struct CadastroCliente *)a)->nome, ((struct CadastroCliente *)b)->nome);
}

//comparar nome de produtos para ordem alfabetica
int compararProduto(const void *a, const void *b) {
    return strcmp(((struct CadastroProduto *)a)->descricao, ((struct CadastroProduto *)b)->descricao);
}

//=================================================== MENU PRINCIPAL
void menuPrincipal() {
    clearScreen();
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
//=================================================== FIM MENU PRINCIPAL

//=================================================== MENU DE CADASTROS
void menuCadastros() {
    int cadastrar=0;
    while(cadastrar != 5) {
        clearScreen();
        printf("\n|---------------------------------------------|\n");
        printf("|                                             |\n");
        printf("|           | |   Cadastros   | |             |\n");
        printf("|                                             |\n");
        printf("|     1. Cadastrar Usuario                    |\n");
        printf("|     2. Cadastrar Cliente                    |\n");
        printf("|     3. Cadastrar Produto                    |\n");
        printf("|     4. Cadastrar Categoria                  |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     5. Voltar ao Menu principal             |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     Escolha uma das opcoes acima:           |\n");
        printf("|     (Somente numero)                        |\n");
        printf("|_____________________________________________|\n\n");
        scanf("%d", &cadastrar);

        switch (cadastrar) {

        //cadastrar usuarios
        case 1:
            if (tipoUsuario=='1') {
                cadastroUsuario();
            }
            else{
                 printf("\n Usuario sem permissao de acesso\n");
                 getchar();
            }
            break;

        //cadastrar produtos
        case 2:
            cadastroCliente();
            break;

        case 3:
            cadastroProduto();
            break;

        case 4:
            cadastroCategoria();
            break;

        //voltar ao menu principal
        case 5:
            voltar();
            break;

        //caso não seja nenhuma das opções de cadastro
        default:
            opinvalida();
        }
    }
}

void get_password(char *password, const int maxLength) {
    const HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    DWORD count;
    char ch;
    int i = 0;

    // Obtém o modo atual do console
    GetConsoleMode(hStdin, &mode);
    // Desativa o eco
    SetConsoleMode(hStdin, mode & ~(ENABLE_ECHO_INPUT));

    // Lê a senha
    while (i < maxLength - 1) {
        if (ReadConsole(hStdin, &ch, 1, &count, NULL) && ch != '\n' && ch != '\r') {
            if (ch == '\b') {
                // Tratamento para backspace
                if (i > 0) {
                    printf("\b \b");  // Apaga o asterisco
                    i--;
                }
            } else {
                password[i++] = ch;
                printf("*");
                fflush(stdout);  // Garante que o asterisco seja exibido imediatamente
            }
        } else if (ch == '\r') {
            // Se o caractere é '\r', substitua por '\n'
            break;
        }
    }
    password[i] = '\0';

    // Restaura o modo do console
    SetConsoleMode(hStdin, mode);
    printf("\n");

    clear_input_buffer();
}


void get_password1(char *password, int maxLength) {
    const HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    DWORD count;
    char ch;
    int i = 0;

    // Obtém o modo atual do console
    GetConsoleMode(hStdin, &mode);
    // Desativa o eco
    SetConsoleMode(hStdin, mode & ~(ENABLE_ECHO_INPUT));

    // Lê a senha
    while (i < maxLength - 1 && ReadConsole(hStdin, &ch, 1, &count, NULL) && ch != '\n' && ch != '\r') {
        password[i++] = ch;
        // Opcional: exibe asteriscos para cada caractere digitado
        printf("*");
    }
    password[i] = '\0';

    // Restaura o modo do console
    SetConsoleMode(hStdin, mode);
    printf("\n");

    clear_input_buffer();
}

void clearScreen() {
    system("cls");
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

//=================================================== FIM MENU DE CADASTROS

//=================================================== CADASTRAR USUARIOS
void cadastroUsuario() {
    char confirma;
    do {

        usuario = realloc(usuario, (numusuarios + 1) * sizeof(struct CadastroUsuario));
        if (usuario == NULL) {
            printf("\n|-----------------------------------------------------|\n");
            printf("| ERRO NA ALOCACAO DE MEMORIA! ENCERRANDO PROGRAMA... |\n");
            printf("|-----------------------------------------------------|\n");
            exit(1);
        }

        usuario[numusuarios].cod = numusuarios + 1;

        //LOGIN DO usuario
        fflush(stdin);
        while (true) {
            char descricao[30];            
            clearScreen();
            printf("\n|-----------------------------------------------------|\n");
            printf("|                                                     |\n");
            printf("|           | |   Cadastrar Usuario   | |             |\n");
            printf("|                                                     |\n");
            printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  1. Digite o login do usuario: ");
            fgets(descricao, sizeof(descricao), stdin);
            descricao[strcspn(descricao, "\n")] = 0;
            if (validacaracter(descricao)) {
                if ((strlen(descricao)>=LOGIN_MIN) && (strlen(descricao)<=LOGIN_MAX)){
                   strncpy(usuario[numusuarios].login, descricao, sizeof(descricao) - 1);
                   break;
                }
                printf("\n  Erro! login com tamanho invalido. Tente Novamente.");
            } else {
                printf("\n  Erro! login invalido. Tente Novamente.");
            }
        }
        fflush(stdin);

        //SENHA

        while (true) {
            char password[20];

            printf("\n\n  2. Digite a senha do usuario: ");
            get_password(password, sizeof(password));

            if (validacaracter(password)) {

                if ((strlen(password)>=PASSWORD_MIN) && (strlen(password)<=PASSWORD_MAX)) {
                   strncpy(usuario[numusuarios].password, password, sizeof(password) - 1);
                   break;
                }
                printf("\n  Erro! senha com tamanho invalido. Tente Novamente.");
            } else {
                printf("\n  Erro! Senha invalida. Tente Novamente.");
            }
        }

        // Tipo
        while (true) {

            printf("\n\n  3. Digite o Tipo do usuario(1-Administrador ou 2-Funcionario): ");
            scanf(" %c", &usuario[numusuarios].tipo);

            if (usuario[numusuarios].tipo == '1' || usuario[numusuarios].tipo == '2') {
                if ((numusuarios==0) && (usuario[numusuarios].tipo != '1')) {
                  printf("\n  !Erro! Para o cadastro do primeiro usuario tipo deve ser 1-Administrador. Tente Novamente.");
                }
                else {
                  break;
                }
            }
            else {
                printf("\n  !Erro! tipo invalido. Tente Novamente.");
            }
        }
        fflush(stdin);
        numusuarios++;

//se quiser salvar dados do usuario recem cadastrado
//------------------------------------------------------------------------------------------------
        char salvar;
        while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N')) {

            clearScreen();

            printf("\n|---------------------------------------------------------|\n");
            printf("|  DESEJA SALVAR OS DADOS DE usuarios CADASTRADOS? (s/n)  |\n");
            printf("|---------------------------------------------------------|\n\n");
            scanf(" %c", &salvar);
            fflush(stdin);

            if (salvar == 's' || salvar == 'S') {
                arquivoUsuario = fopen("usuarios.dat", "wb");
                fwrite(&numusuarios, sizeof(int), 1, arquivoUsuario);
                fwrite(usuario, sizeof(struct CadastroUsuario), numusuarios, arquivoUsuario);
                fclose(arquivoUsuario);
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   usuario SALVO...                         |\n");
                break;
            }
            if (salvar == 'n' || salvar == 'N') {
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   usuario NAO SALVO...                     |\n");
                break;
            }
            opinvalida();
        }

//se quiser cadastrar mais um usuario
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N')) {
            printf("|- - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("|   DESEJA INCLUIR UM NOVO usuario? (s/n)    |\n");
            printf("|--------------------------------------------|\n\n");
            scanf(" %c", &confirma);
            fflush(stdin);
            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N') {
                clearScreen();
                break;
            }
            opinvalida();
        }
        if ( confirma == 'n' || confirma == 'N') {
            clearScreen();
            break;
        }

    }
    while (confirma != 'n'||confirma != 'N');
}
//=================================================== FIM CADASTRAR USUARIOS

//=================================================== CADASTRAR CATEGORIAS
void cadastroCategoria() {
    char confirma;
    do {
        categoria = realloc(categoria, (numcategorias + 1) * sizeof(struct CadastroCategoria));
        if (categoria == NULL) {
            printf("\n|-----------------------------------------------------|\n");
            printf("| ERRO NA ALOCACAO DE MEMORIA! ENCERRANDO PROGRAMA... |\n");
            printf("|-----------------------------------------------------|\n");
            exit(1);
        }
        categoria[numcategorias].cod = numcategorias + 1;

        //Cadastro de categoria 
        fflush(stdin);
        while (true) {
            clearScreen();
            printf("\n|-----------------------------------------------------|\n");
            printf("|                                                     |\n");
            printf("|           | |   Cadastrar Categoria   | |             |\n");
            printf("|                                                     |\n");
            printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  1. Digite a categoria: ");
            fgets(categoria[numcategorias].descricao, sizeof(categoria[numcategorias].descricao), stdin);
            categoria[numcategorias].descricao[strcspn(categoria[numcategorias].descricao, "\n")] = 0;
            if (validacaracter(categoria[numcategorias].descricao)) {
               break;
            }
            printf("\n  Erro! login invalido. Tente Novamente.");
            getchar();
        }
        fflush(stdin);
        numcategorias++;

//se quiser salvar dados do categoria recem cadastrado
//------------------------------------------------------------------------------------------------
        char salvar;
        while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N')) {
            clearScreen();
            printf("\n|---------------------------------------------------------|\n");
            printf("|  DESEJA SALVAR OS DADOS DE CATEGORIAS CADASTRADOS? (s/n)  |\n");
            printf("|---------------------------------------------------------|\n\n");
            scanf(" %c", &salvar);
            fflush(stdin);

            if (salvar == 's' || salvar == 'S') {
                arquivoCategoria = fopen("categorias.dat", "wb");
                fwrite(&numcategorias, sizeof(int), 1, arquivoCategoria);
                fwrite(categoria, sizeof(struct CadastroCategoria), numcategorias, arquivoCategoria);
                fclose(arquivoCategoria);
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   CATEGORIA SALVO...                         |\n");
                break;
            }
            if (salvar == 'n' || salvar == 'N') {
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   CATEGORIA NAO SALVA...                     |\n");
                break;
            }
            opinvalida();
        }

//se quiser cadastrar mais um categoria
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N')) {
            printf("|- - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("|   DESEJA INCLUIR UMA NOVA CATEGORIA? (s/n)    |\n");
            printf("|--------------------------------------------|\n\n");
            scanf(" %c", &confirma);
            fflush(stdin);
            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N') {
                clearScreen();
                break;
            }
            opinvalida();
        }
        if ( confirma == 'n' || confirma == 'N') {
            clearScreen();
            break;
        }

    }
    while (confirma != 'n'||confirma != 'N');
}
//=================================================== FIM CADASTRAR CATEGORIAS

//=================================================== CADASTRAR CLIENTES
void cadastroCliente() {
    char confirma;
    do {
        cliente = realloc(cliente, (numClientes + 1) * sizeof(struct CadastroCliente));
        if (cliente == NULL) {
            printf("\n|-----------------------------------------------------|\n");
            printf("| ERRO NA ALOCACAO DE MEMORIA! ENCERRANDO PROGRAMA... |\n");
            printf("|-----------------------------------------------------|\n");
            exit(1);
        }

        cliente[numClientes].cod = numClientes + 1;

        //NOME DO CLIENTE
        fflush(stdin);
        while (true) {
            clearScreen();
            printf("\n|-------------------------------------------------------------|\n");
            printf("|                                                             |\n");
            printf("|               | |   Cadastrar Cliente   | |                 |\n");
            printf("|                                                             |\n");
            printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  1. Digite o nome do cliente: ");
            fgets(cliente[numClientes].nome, sizeof(cliente[numClientes].nome), stdin);
            cliente[numClientes].nome[strcspn(cliente[numClientes].nome, "\n")] = 0;
            if (validacaracter(cliente[numClientes].nome)) {
                break;
            }
            printf("\n  !Erro! Nome invalido. Tente Novamente.");
        }
        fflush(stdin);

        //NOME SOCIAL
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  2. Digite o nome social do cliente: ");
            fgets(cliente[numClientes].nomesocial, sizeof(cliente[numClientes].nomesocial), stdin);
            cliente[numClientes].nomesocial[strcspn(cliente[numClientes].nomesocial, "\n")] = 0;
            if (validacaracter(cliente[numClientes].nomesocial)) {
                break;
            }
            printf("\n  !Erro! Nome social invalido. Tente Novamente.");
        }
        fflush(stdin);

        // CPF
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  3. Digite o CPF do cliente: ");
            scanf("%s", &cliente[numClientes].cpf);
            if (validaNumero(cliente[numClientes].cpf, 11)) {
                break;
            }
            printf("\n  !Erro! CPF invalido. Tente Novamente.");
        }
        fflush(stdin);

        // RUA
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  4. Digite a rua do cliente: ");
            fgets(cliente[numClientes].rua, sizeof(cliente[numClientes].rua), stdin);
            cliente[numClientes].rua[strcspn(cliente[numClientes].rua, "\n")] = 0;
            if (validacaracter(cliente[numClientes].rua))
            {
                break;
            }
            printf("\n  !Erro! Rua invalida. Tente Novamente.");
        }
        fflush(stdin);

        //NUMERO DA CASA
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  5. Digite o numero da residencia do cliente: ");
            scanf("%s", &cliente[numClientes].num);

            if (validaNumero(cliente[numClientes].num, 0)) {
                break;
            }
            fflush(stdin);
            printf("\n  !Erro! Numero de residencia invalido. Tente Novamente.");
        }
        fflush(stdin);

        //BAIRRO
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  6. Digite o bairro do cliente: ");
            fgets(cliente[numClientes].bairro, sizeof(cliente[numClientes].bairro), stdin);
            cliente[numClientes].bairro[strcspn(cliente[numClientes].bairro, "\n")] = 0;
            if (validacaracter(cliente[numClientes].bairro)) {
                break;
            }
            printf("\n  !Erro! Rua invalida. Tente Novamente.");
        }
        fflush(stdin);

        //CEP
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  7. Digite o CEP do cliente: ");
            fgets(cliente[numClientes].cep, sizeof(cliente[numClientes].cep), stdin);
            cliente[numClientes].cep[strcspn(cliente[numClientes].cep, "\n")] = 0;

            if (validacaracter(cliente[numClientes].cep)) {
              if (validaNumero(cliente[numClientes].cep, 8)) {
                 break;
              }
              printf("\n  !Erro! CEP invalido. Tente Novamente1.");
              fflush(stdin);
            } else {
                printf("\n  !Erro! CEP invalido. Tente Novamente2.");
                fflush(stdin);
            }
        }
        fflush(stdin);

        //TELEFONE
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  8. Digite o telefone/whats do cliente: ");
            fgets(cliente[numClientes].telefone, sizeof(cliente[numClientes].telefone), stdin);
            cliente[numClientes].telefone[strcspn(cliente[numClientes].telefone, "\n")] = 0;
            if (validacaracter(cliente[numClientes].telefone)) {
                break;
            }
            printf("\n  !Erro! Telefone/whats invalida. Tente Novamente.");
        }
        fflush(stdin);

        numClientes++;

//se quiser salvar dados do cliente recem cadastrado
//------------------------------------------------------------------------------------------------
        char salvar;
        while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N')) {
            clearScreen();
            printf("\n|---------------------------------------------------------|\n");
            printf("|  DESEJA SALVAR OS DADOS DE CLIENTES CADASTRADOS? (s/n)  |\n");
            printf("|---------------------------------------------------------|\n\n");
            scanf(" %c", &salvar);
            fflush(stdin);

            if (salvar == 's' || salvar == 'S') {
                arquivoCliente = fopen("clientes.dat", "wb");
                fwrite(&numClientes, sizeof(int), 1, arquivoCliente);
                fwrite(cliente, sizeof(struct CadastroCliente), numClientes, arquivoCliente);
                fclose(arquivoCliente);
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   CLIENTE SALVO...                         |\n");
                break;
            }
            if (salvar == 'n' || salvar == 'N') {
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   CLIENTE NAO SALVO...                     |\n");
                break;
            }
            opinvalida();
        }

//se quiser cadastrar mais um cliente
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N')) {
            printf("|- - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("|   DESEJA INCLUIR UM NOVO CLIENTE? (s/n)    |\n");
            printf("|--------------------------------------------|\n\n");
            scanf(" %c", &confirma);
            fflush(stdin);
            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N') {
                clearScreen();
                break;
            }
            opinvalida();
        }
        if ( confirma == 'n' || confirma == 'N') {
            clearScreen();
            break;
        }

    }
    while (confirma != 'n'||confirma != 'N');
}
//=================================================== FIM CADASTRAR CLIENTES

//=================================================== CADASTRAR PRODUTOS
void cadastroProduto() {

    char confirma;
    while (confirma != 'n'||confirma != 'N') {
        produto = realloc(produto, (numProdutos + 1) * sizeof(struct CadastroProduto));
        if (produto == NULL) {
            printf("\n|-----------------------------------------------------|\n");
            printf("| ERRO NA ALOCACAO DE MEMORIA! ENCERRANDO PROGRAMA... |\n");
            printf("|-----------------------------------------------------|\n");
            exit(1);
        }

        produto[numProdutos].precoCompra=0;
        produto[numProdutos].precoVenda=0;
        produto[numProdutos].margem=0;

        produto[numProdutos].cod = numProdutos + 1;

        //NOME DO PRODUTO
        fflush(stdin);
        while (true) {
            clearScreen();
            printf("\n|-------------------------------------------------------------|\n");
            printf("|                                                             |\n");
            printf("|               | |   Cadastrar Produto   | |                 |\n");
            printf("|                                                             |\n");
            printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  1. Digite o nome do produto: ");
            fgets(produto[numProdutos].descricao, sizeof(produto[numProdutos].descricao), stdin);
            produto[numProdutos].descricao[strcspn(produto[numProdutos].descricao, "\n")] = 0;
            if (validacaracter(produto[numProdutos].descricao)) {
                break;
            }
            printf("\n  !Erro! Nome invalido. Tente Novamente.\n");
        }
        fflush(stdin);

        //CATEGORIA
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  2. Digite a categoria do produto: ");
            if (scanf("%d", &produto[numProdutos].categoria.cod) == 1) {

               indProduto=pesquisaCategoria(produto[numProdutos].categoria.cod);
               if (indProduto==-1) {
                  printf("\n  Categoria nao cadastrada.\n\n");
                  getchar();
                  clearScreen();
                  continue;
               }
               printf("  Categoria: %s\n", categoria[indProduto].descricao);
               strncpy(produto[numProdutos].categoria.descricao, categoria[indProduto].descricao, sizeof(categoria[indProduto].descricao) - 1); 

               break;
            }
            printf("\n !Erro! Categoria invalida. Tente Novamente.\n");
            fflush(stdin);
        }


        //PRECO DE COMPRA
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  3. Digite o preco de compra do produto: ");
            if (scanf("%f", &produto[numProdutos].precoCompra) == 1) {
                break;
            }
            printf("\n  !Erro! Preco de compra invalido. Tente Novamente.\n");
            fflush(stdin);
        }

        fflush(stdin);

        //MARGEM DE LUCRO
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  4. Digite a porcentagem da margem de lucro do produto: ");
            if (scanf("%f", &produto[numProdutos].margem) == 1) {
                break;
            }
            printf("\n  !Erro! Margem invalida. Tente Novamente.\n");
            fflush(stdin);
        }
        fflush(stdin);

        //preço de venda calculado com base na margem de lucro desejada
        margem = produto[numProdutos].margem / 100;
        produto[numProdutos].precoVenda = produto[numProdutos].precoCompra + (produto[numProdutos].precoCompra * margem);
        produto[numProdutos].precoVenda=truncar(produto[numProdutos].precoVenda, 2);

        printf("\n  - Preco de venda do produto: %.2f\n", produto[numProdutos].precoVenda);

        //ESTOQUE
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  5. Digite o estoque do produto: ");
            if (scanf("%d", &produto[numProdutos].estoque) == 1)
            {
                break;
            }
            printf("\n  !Erro! Estoque invalido. Tente Novamente.\n");
            fflush(stdin);
        }

        //ESTOQUE MINIMO
        while (true) {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  6. Digite o estoque minimo do produto: ");
            if (scanf("%d", &produto[numProdutos].estoqueminimo) == 1) {
                break;
            }
            printf("\n  !Erro! Estoque minimo invalido. Tente Novamente.\n");
            fflush(stdin);
        }
        numProdutos++;

//se quiser salvar dados do cliente recem cadastrado
//------------------------------------------------------------------------------------------------
        char salvar;
        while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N')) {
            clearScreen();
            printf("\n|---------------------------------------------------------|\n");
            printf("|  DESEJA SALVAR OS DADOS DO PRODUTO CADASTRADO? (s/n)    |\n");
            printf("|---------------------------------------------------------|\n\n");
            scanf(" %c", &salvar);
            fflush(stdin);

            if (salvar == 's' || salvar == 'S') {
                arquivoProduto = fopen("produtos.dat", "wb");
                fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                fclose(arquivoProduto);
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   PRODUTO SALVO...                         |\n");
                break;
            }
            if (salvar == 'n' || salvar == 'N') {
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   PRODUTO NAO SALVO...                     |\n");
                break;
            }
            opinvalida();
        }


        //se quiser cadastrar mais um produto
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N')) {

            printf("|- - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("|   DESEJA INCLUIR UM NOVO PRODUTO? (s/n)    |\n");
            printf("|--------------------------------------------|\n\n");
            scanf(" %c", &confirma);
            fflush(stdin);

            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N') {
                break;
            }
            opinvalida();
        }
        if ( confirma == 'n' || confirma == 'N') {
            clearScreen();
            break;
        }
    }
}
//=================================================== FIM CADASTRAR PRODUTOS

//=================================================== CADASTRAR CATEGORIA
void carregarDadosCategoria()
{
    arquivoCategoria = fopen("categorias.dat", "rb");
    if (arquivoCategoria) {
        fread(&numcategorias, sizeof(int), 1, arquivoCategoria);
        categoria = malloc(numcategorias * sizeof(struct CadastroCategoria));
        fread(categoria, sizeof(struct CadastroCategoria), numcategorias, arquivoCategoria);
        fclose(arquivoCategoria);
    }
}
//=================================================== FIM CADASTRAR CATEGORIA


void carregarDadosCaixa() {
    arquivoCaixa = fopen("caixa.dat", "rb");
    if (arquivoCaixa) {
        fread(&caixa, sizeof(char), 1, arquivoCaixa);
        fread(&inicioCaixa, sizeof(float), 1, arquivoCaixa);
        fclose(arquivoCaixa);
    }
}


//=================================================== CARREGAR DADOS DE...
//USUARIOS
carregarDadosUsuarios()
{
    arquivoUsuario = fopen("usuarios.dat", "rb");
    if (arquivoUsuario) {
        fread(&numusuarios, sizeof(int), 1, arquivoUsuario);
        usuario = malloc(numusuarios * sizeof(struct CadastroUsuario));
        fread(usuario, sizeof(struct CadastroUsuario), numusuarios, arquivoUsuario);
        fclose(arquivoUsuario);
    }
}
//CLIENTES
carregarDadosClientes()
{
    arquivoCliente = fopen("clientes.dat", "rb");
    if (arquivoCliente) {
        fread(&numClientes, sizeof(int), 1, arquivoCliente);
        cliente = malloc(numClientes * sizeof(struct CadastroCliente));
        fread(cliente, sizeof(struct CadastroCliente), numClientes, arquivoCliente);
        fclose(arquivoCliente);
    }
}

//PRODUTOS
carregarDadosProdutos() {
    arquivoProduto = fopen("produtos.dat", "rb");
    if (arquivoProduto) {
        fread(&numProdutos, sizeof(int), 1, arquivoProduto);
        produto = malloc(numProdutos * sizeof(struct CadastroProduto));
        fread(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
        fclose(arquivoProduto);
    }
}

//VENDA
int carregarDadosVendas(struct CadastroVenda **venda) {
    arquivoVenda = fopen("vendas.dat", "rb");
    numVendas = 0;
    if (arquivoVenda) {
        fread(&numVendas, sizeof(int), 1, arquivoVenda);

        *venda = (struct CadastroVenda *)malloc(numVendas * sizeof(struct CadastroVenda));
        if (*venda == NULL) {
            perror("Erro ao alocar memória para vendas");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < numVendas; i++) {
            fread(&(*venda)[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);
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
//=================================================== FIM CARREGAR DADOS

//=================================================== MENU DE VENDAS
void menuVendas() {
    int vender=0;
    while (vender != 4) {
        clearScreen();
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
        switch (vender) {
        //nova venda
        case 1:
            novaVenda();
            break;

        //reirada de caixa
        case 2:
           if (tipoUsuario=='1') {
                Sangria();
           } else {
               printf("\n Usuario sem permissao de acesso\n");
               getchar();
           } break;

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
//=================================================== FIM MENU DE VENDAS

//=================================================== NOVA VENDA
novaVenda() {
    if (caixa=='F') {
        caixaFechado();
    } else {
        int continuarVenda = 1;
        while (continuarVenda) {
            venda = (struct CadastroVenda *)realloc(venda, (numVendas + 1) * sizeof(struct CadastroVenda));
            if (venda == NULL) {
                perror("Erro ao alocar memoria para vendas");
                exit(EXIT_FAILURE);
            }
            int codcliente;
            struct Data data = obterDataAtual();

            printf("Digite o codigo do cliente: ");
            scanf("%d", &codcliente);

            indProduto=pesquisaCliente(codcliente);
            if (indProduto==-1) {
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
                venda[numVendas].produtos = (struct ProdutoVenda *)realloc(venda[numVendas].produtos, (venda[numVendas].totalProdutos+1) * sizeof(struct ProdutoVenda));

                if (venda[numVendas].produtos == NULL) {
                    perror("Erro ao alocar memoria para produtos");
                    exit(EXIT_FAILURE);
                }

                int cod;
                char descricao[30];
                int quantidade;
                float precoVenda = 0.0f;

                printf("Digite o codigo do produto: ");
                scanf("%d", &cod);

                indProduto=pesquisaItem(cod);
                if (indProduto==-1) {
                    printf("\nProduto nao cadastrado.\n\n");
                    system("pause");
                    system("cls");
                    continue;
                }
                strncpy(descricao, produto[indProduto].descricao, sizeof(produto[indProduto].descricao) - 1);
                precoVenda = produto[indProduto].precoVenda;

                printf("\n  Produto Escolhido: %s\n", produto[indProduto].descricao);
                printf("  Preco: %.2f\n", produto[indProduto].precoVenda);
                printf("  Estoque Atual: %d\n", produto[indProduto].estoque);
                printf("  Estoque Minimo do Produto: %d\n", produto[indProduto].estoqueminimo);
                printf("\n| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
                printf("Digite a quantidade: ");
                scanf("%d", &quantidade);

                venda[numVendas].produtos[venda[numVendas].totalProdutos].num = numVendas+1;
                venda[numVendas].produtos[venda[numVendas].totalProdutos].cod = cod;
                strncpy(venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao, descricao, sizeof(venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao) - 1);
                venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao[sizeof(produto->descricao) - 1] = '\0';
                venda[numVendas].produtos[venda[numVendas].totalProdutos].quantidade = quantidade;

                venda[numVendas].produtos[venda[numVendas].totalProdutos].precoVenda = precoVenda;
                venda[numVendas].produtos[venda[numVendas].totalProdutos].valorTotal = quantidade*precoVenda;

                printf("Total do Item.: %.2f\n", venda[numVendas].produtos[venda[numVendas].totalProdutos].valorTotal);

                venda[numVendas].valorTotal += quantidade * precoVenda;

                printf("Total da Venda: %.2f\n", venda[numVendas].valorTotal);

                venda[numVendas].totalProdutos++;
                totalBruto=venda[numVendas].valorTotal;

                printf("Deseja adicionar outro produto? (1-Sim, 0-Nao): ");
                scanf("%d", &continuarProduto);
            }
            numVendas++;
            continuarVenda = 0;
        }
    }
}
//=================================================== FIM NOVA VENDA



novaVendaBruno() {
    if (caixa=='F') {
        caixaFechado();
    } else {
        int continuarVenda = 1;
        while (continuarVenda!=0) {
            venda = (struct CadastroVenda *)realloc(venda, (numVendas + 1) * sizeof(struct CadastroVenda));
            if (venda == NULL) {
                perror("Erro ao alocar memoria para vendas");
                exit(EXIT_FAILURE);
            }
            struct Data data = obterDataAtual();
            int codcliente=0;

            clearScreen();
            printf("\n|-------------------------------------------------------------|");
            printf("\n|                          CLIENTE                            |");
            printf("\n|-------------------------------------------------------------|");
            printf("\n| Digite o codigo do cliente: ");
            scanf("%d", &codcliente);

            indProduto=pesquisaCliente(codcliente);
            if (indProduto==-1) {
                printf("\n  Cliente nao cadastrado.\n\n");
                getchar();
                clearScreen();
                continue;
            }
            printf("  Nome do Cliente: %s\n", cliente[indProduto].nome);

            char nomeCliente[30];
            strncpy(nomeCliente, cliente[indProduto].nome, sizeof(cliente[indProduto].nome) - 1);
            adicionarVenda(&venda[numVendas], numVendas+1, codcliente, nomeCliente, data);

            int continuarProduto = 1;
            while (continuarProduto) {

            venda[numVendas].produtos = (struct ProdutoVenda *)realloc(venda[numVendas].produtos, (venda[numVendas].totalProdutos+1) * sizeof(struct ProdutoVenda));

            if (venda[numVendas].produtos == NULL) {
                perror("  Erro ao alocar memoria para produtos");
                exit(EXIT_FAILURE);
            }

            int cod;
            char descricao[30];
            int quantidade;
            float precoVenda = 0.0f;

            printf("\n|-------------------------------------------------------------|");
            printf("\n|                          PRODUTO                            |");
            printf("\n|-------------------------------------------------------------|");
            printf("\n| CODIGO DO PRODUTO: ");
            scanf("%d", &cod);

            indProduto=pesquisaItem(cod);
            if (indProduto==-1) {
                printf("\n  Produto nao cadastrado.\n\n");
                getchar();
                clearScreen();
                break;
            }
            strncpy(descricao, produto[indProduto].descricao, sizeof(produto[indProduto].descricao) - 1);
            precoVenda = produto[indProduto].precoVenda;

            printf("\n  Produto Escolhido: %s\n", produto[indProduto].descricao);
            printf("  Preco: %.2f\n", produto[indProduto].precoVenda);
            printf("  Estoque Atual: %d\n", produto[indProduto].estoque);
            printf("  Estoque Minimo do Produto: %d\n", produto[indProduto].estoqueminimo);
            printf("\n| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  Digite a quantidade desejada: ");
            scanf("%d", &quantidade);

            if (produto[indProduto].estoque-quantidade<=0) {
                clearScreen();
                printf("\n|------------------------------------------------------------------------------|");
                printf("\n|  COMPRA MAIOR DO QUE ESTOQUE EXISTENTE, FAVOR ESCOHER UMA QUANTIDADE MENOR.  |");
                printf("\n|------------------------------------------------------------------------------|\n\n");
                getchar();
            } else {
                if (quantidade<=0) {
                    clearScreen();
                    printf("\n|------------------------------------------------------|");
                    printf("\n|  QUANTIDADE ESCOLHIDA IMPOSSIVEL, TENTE NOVAMENTE.   |");
                    printf("\n|------------------------------------------------------|\n\n");
                    getchar();
                } else {
                    if (produto[indProduto].estoque-quantidade<produto[indProduto].estoqueminimo) {
                        clearScreen();
                        printf("\n|----------------------------------------------------------------------|");
                        printf("\n|  ATENCAO! ESTOQUE MINIMO DE %d ULTRAPASSADO, FAVOR ESCOLHER           |", produto[indProduto].estoqueminimo);
                        printf("\n|  OUTRO ITEM, OU UMA QUANTIA MENOR.                                   |");
                        printf("\n|----------------------------------------------------------------------|\n\n");
                        getchar();
                    } else {
                    if ((produto[indProduto].estoque-quantidade)==produto[indProduto].estoqueminimo) {
                        clearScreen();
                        printf("\n|-------------------------------------------|");
                        printf("\n|  ATENCAO! ESTOQUE MINIMO DE %d ATINGIDO.  |", produto[indProduto].estoqueminimo);
                        printf("\n|-------------------------------------------|\n\n");
                        getchar();
                    }

                    venda[numVendas].produtos[venda[numVendas].totalProdutos].num = numVendas+1;
                    venda[numVendas].produtos[venda[numVendas].totalProdutos].cod = cod;
                    strncpy(venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao, descricao, sizeof(venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao) - 1);
                    venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao[sizeof(produto->descricao) - 1] = '\0';
                    venda[numVendas].produtos[venda[numVendas].totalProdutos].quantidade = quantidade;
                    venda[numVendas].produtos[venda[numVendas].totalProdutos].precoVenda = precoVenda;
                    venda[numVendas].produtos[venda[numVendas].totalProdutos].valorTotal = quantidade*precoVenda;
                    venda[numVendas].valorTotal += quantidade * precoVenda;
                    venda[numVendas].totalProdutos++;
                    produto[indProduto].estoque-=quantidade;
                    totalBruto+=venda[numVendas].valorTotal;
                    numVendas++;
                    vendasRealizadas=numVendas;

                    printf("\n   TOTAL NO CARRINHO: %.2f\n\n", totalBruto);
                    getchar();
                }
            }
        }

        clearScreen();
        printf("\n|--------------------------------------------|");
        printf("\n|  ADICIONAR OUTRO PRODUTO? (1-Sim, 0-Nao)   |");
        printf("\n|--------------------------------------------|\n\n");
        scanf("%d", &continuarProduto);

        if (continuarProduto==1) {
            continue;
        }

        if(continuarProduto==0) {
            break;
        }
        opinvalida();
    }  // End produtos

            clearScreen();
            printf("\n|--------------------------------------------|");
            printf("\n|  ADICIONAR OUTRA VENDA? (1-Sim, 0-Nao)   |");
            printf("\n|--------------------------------------------|\n\n");
            scanf("%d", &continuarVenda);

            if (continuarVenda==1) {
                continue;
            }
            if(continuarVenda==0) {
                break;
            }
            opinvalida();
        } // End vendas  
    }
}

//=================================================== FIM NOVA VENDA

//=================================================== SANGRIA
void Sangria()
{
    if (caixa=='F')
    {
        caixaFechado();
    }
    else
    {
        if (vCaixa<50)
        {
            printf("\n|---------------------------------------|\n");
            printf("|  O CAIXA ESTA COM MENOS DE R$ 50.00.  |\n");
            printf("|---------------------------------------|\n");
            getchar();
        }
        else
        {
            printf("\n|----------------------------------------------------|\n");
            printf("|               |  | SANGRIA | |                     |\n");
            printf("|                                                    |\n");
            printf("|  Digite a quantia em dinheiro a ser retirado:      |\n");
            printf("|----------------------------------------------------|\n");
            printf("  TOTAL EM DINHEIRO NO CAIXA: R$ %.2f\n", vCaixa);
            printf("|----------------------------------------------------|\n");
            scanf("%f", &retirada);

            if (retirada==0)
            {
                printf("\n  NADA FOI RETIRADO, SEU CAIXA CONTINUA COM R$ %.2f\n", vCaixa);
                getchar();
            }
            else
            {
                if ((vCaixa-retirada)<50)
                {
                    printf("\n|---------------------------------------------------------------|\n");
                    printf("|  O CAIXA  FICARA COM MENOS DE R$ 50.00, SAQUE IMPOSSIVEL...    |\n");
                    printf("|----------------------------------------------------------------|\n");
                    getchar();
                }

                if ((vCaixa-retirada)>=50)
                {
                    vCaixa=vCaixa-retirada;
                    printf("  DINHEIRO RETIRADO, SEU CAIXA AGORA ESTA COM R$ %.2f\n", vCaixa);
                    getchar();
                }
            }
        }
    }
}
//=================================================== FIM SANGRIA

//=================================================== PAGAMENTOS
void pagamento()
{
    clearScreen();
    //caso não haja compras no carrinho
    if(totalBruto==0)
    {
        printf("\n|-----------------------------------------------|\n");
        printf("|  NENHUMA COMPRA REALIZADA, FAVOR ADICIONAR    |\n");
        printf("|  ITENS PARA PAGAMENTO.                        |\n");
        printf("|                                               |\n");
        printf("|  VOLTANDO PARA O MENU ANTERIOR...             |\n");
        printf("|-----------------------------------------------| \n\n");
        getchar();
        clearScreen();
    }
    else
    {
        //caso o carrinho tenha mais que R$ 0
        //forma de pagamento
        while (totalBruto>0)
        {
            if (totalBruto>0)
            {
                totalDesc=totalBruto-desconto;
                troco = totalDesc-pagoDin-pagoCartao;

                int pag=0;
                printf("\n|---------------------------------------------------|\n");
                printf("|                     PAGAMENTO                     |\n");
                printf("|---------------------------------------------------|\n");
                printf("|  Escolha sua forma de pagamento (somente numero)  |\n");
                printf("|                                                   |\n");
                printf("|  1. Dinheiro/PIX                                  |\n");
                printf("|  2. Cartao de Debito/Credito                      |\n");
                printf("|  3. Voltar ao menu principal                      |\n");
                printf("|---------------------------------------------------|\n");
                printf("   TOTAL A PAGAR: R$ %.2f\n\n   ", troco);
                scanf("%d", &pag);
                clearScreen();

                switch (pag)
                {

                case 1:
                    if (loopDesconto==0){
                        printf("\n Digite a porcentagem de desconto (somente numero): ");
                        scanf("%f", &desc);

                    if ((desc<0)||(desc>100))
                    {
                        printf("\n|------------------------------------------------------------|\n");
                        printf("|  NAO E POSSIVEL REALIZAR ESTE DESCONTO, TENTE NOVAMENTE.   |\n");
                        printf("|------------------------------------------------------------|\n\n");
                        getchar();
                        clearScreen();
                    }
                    }
                    if((desc>=0)&(desc<=100))
                    {
                        desc = desc / 100;
                        desconto = totalBruto * desc;
                        desconto=truncar(desconto, 2);

                        totalDesc=totalBruto-desconto;
                        totalDesc-=pagoDin;
                        totalDesc-=pagoCartao;

                        printf("\n|------------------------------------------------|\n");
                        printf("|  Pagamento em dinheiro:                        |\n");
                        printf("|                                                |\n");
                        printf("|  Digite a quantia em dinheiro entregue:        |\n");
                        printf("|------------------------------------------------|\n");
                        printf("  TOTAL BRUTO...........: R$ %.2f\n", totalBruto);
                        printf("  COMPRA COM DESCONTO DE: R$ %.2f\n", desconto);
                        printf("  TOTAL A PAGAR.........: R$ %.2f\n", totalDesc);
                        printf("|------------------------------------------------|\n");
                        printf("  R$ ");
                        scanf("%f", &pagoDin);

                        if (totalDesc<pagoDin)
                        {
                            troco = pagoDin-totalDesc;
                            printf("|------------------------------------------------|\n");
                            printf("  TROCO A PAGAR: R$ %.2f                        \n", troco);
                            printf("|------------------------------------------------|\n\n");
                            pagoDin=totalDesc;
                            getchar();
                            clearScreen();
                        }

                        if (totalDesc==pagoDin)
                        {
                            printf("\n|-------------------------------------------------|\n");
                            printf("|  PAGAMENTO REALIZADO COM SUCESSO,               |\n");
                            printf("|  VOLTANDO PARA O MENU PRINCIPAL...              |\n");
                            printf("|-------------------------------------------------| \n\n");
                            getchar();
                            clearScreen();
                            if (totalBruto-desconto==pagoDin)
                            {
                                venda[indProduto].valorTotal=pagoDin;
                                venda[indProduto].metodoPagamento='d';
                                vCaixa=inicioCaixa+totalDesc;
                                totalDin=totalDin+pagoDin;
                                totalBruto=0;
                                totalDesc=0;
                                desconto=0;
                                troco=0;
                                pagoDin=0;
                                pagoCartao=0;
                                loopDesconto=0;
                            }
                            if(loopDesconto==1){
                                if (loopDesconto=1){
                                venda[indProduto].valorTotal=pagoCartao+pagoDin;
                                venda[indProduto].metodoPagamento='p';
                                vCaixa=inicioCaixa+totalDesc;
                                totalCartao=totalCartao+pagoCartao;
                                totalBruto=0;
                                totalDesc=0;
                                desconto=0;
                                troco=0;
                                pagoDin=0;
                                pagoCartao=0;
                                loopDesconto=0;
                                }
                            }
                        }

                        else if (totalDesc>pagoDin)
                        {
                            troco = totalDesc-pagoDin;
                            clearScreen();
                            printf("\n|--------------------------------------------------------------|\n");
                            printf("|  PAGAMENTO INSUFICIENTE, PAGAR RESTANTE DE OUTRA MANEIRA?    |\n");
                            printf("|                                                              |\n");
                            printf("|  1. Sim                                                      |\n");
                            printf("|  2. Voltar ao Menu Principal (Cancelar Pagamento Atual)      |\n");
                            printf("|--------------------------------------------------------------| \n");
                            printf("   TOTAL DA COMPRA.: R$ %.2f\n", totalDesc);
                            printf("   QUANTIA PAGA EM DINHEIRO: R$ %.2f\n", pagoDin);
                            printf("   QUANTIA PAGA EM CARTAO..: R$ %.2f\n", pagoCartao);
                            printf("   RESTANTE A PAGAR: R$ %.2f\n", troco);
                            printf("|--------------------------------------------------------------|\n");

                            scanf("%d", &pag);
                            clearScreen();

                            if (pag==1)
                            {
                                totalDesc-=pagoDin;
                                loopDesconto=1;
                            }
                            else
                            {
                                if (pag==2)
                                {
                                    voltar();
                                    loopDesconto=0;
                                    pagoDin=0;
                                    break;
                                }
                            }
                        }
                    }
                    break;

                case 2:

                    totalDesc=totalBruto-desconto;
                    while(totalDesc!=pagoCartao)
                    {
                        totalDesc-=pagoDin;
                        totalDesc-=pagoCartao;

                        printf("\n|------------------------------------------------|\n");
                        printf("|                   PAGAMENTO                    |\n");
                        printf("|------------------------------------------------|\n");
                        printf("|  Pagamento em Cartao de Credito/Debito         |\n");
                        printf("|                                                |\n");
                        printf("|  Digite a quantia em cartao:                   |\n");
                        printf("|------------------------------------------------|\n");
                        printf("  TOTAL A PAGAR: R$ %.2f \n", totalDesc);
                        printf("|------------------------------------------------|\n");
                        printf("   R$ ");
                        scanf("%f", &pagoCartao);

                        totalDesc=truncar(totalDesc,2);
                        if (totalDesc==pagoCartao)
                        {
                            printf("\n\n|-------------------------------------------------|\n");
                            printf("|  PAGAMENTO REALIZADO COM SUCESSO,               |\n");
                            printf("|  VOLTANDO PARA O MENU PRINCIPAL...              |\n");
                            printf("|-------------------------------------------------| \n\n");
                            getchar();
                            clearScreen();

                            if (totalBruto-desconto==pagoCartao)
                            {
                                venda[indProduto].valorTotal=pagoCartao;
                                venda[indProduto].metodoPagamento='c';
                                vCaixa=inicioCaixa+totalDesc;
                                totalCartao=totalCartao+pagoCartao;
                                totalBruto=0;
                                totalDesc=0;
                                desconto=0;
                                troco=0;
                                pagoDin=0;
                                pagoCartao=0;
                                loopDesconto=0;
                            }
                            if (loopDesconto=1){
                                venda[indProduto].valorTotal=pagoCartao+pagoDin;
                                venda[indProduto].metodoPagamento='p';
                                vCaixa=inicioCaixa+totalDesc;
                                totalCartao=totalCartao+pagoCartao;
                                totalBruto=0;
                                totalDesc=0;
                                desconto=0;
                                troco=0;
                                pagoDin=0;
                                pagoCartao=0;
                                loopDesconto=0;
                            }
                        }


                        else if (totalDesc>pagoCartao)
                        {
                            troco = totalDesc-pagoCartao;
                            clearScreen();
                            printf("\n|--------------------------------------------------------------|\n");
                            printf("|  PAGAMENTO INSUFICIENTE, PAGAR RESTANTE DE OUTRA MANEIRA?    |\n");
                            printf("|                                                              |\n");
                            printf("|  1. Sim                                                      |\n");
                            printf("|  2. Voltar ao Menu Principal (Cancelar Pagamento Atual)      |\n");
                            printf("|--------------------------------------------------------------| \n");
                            printf("   TOTAL DA COMPRA.........: R$ %.2f\n", totalBruto-desconto);
                            printf("   QUANTIA PAGA EM DINHEIRO: R$ %.2f\n", pagoDin);
                            printf("   QUANTIA PAGA EM CARTAO..: R$ %.2f\n", pagoCartao);
                            printf("   RESTANTE A PAGAR........: R$ %.2f\n", troco);
                            printf("|--------------------------------------------------------------|\n");

                            scanf("%d", &pag);
                            clearScreen();

                            if (pag==1)
                            {
                                totalDesc-=pagoCartao;
                                loopDesconto=1;
                            }
                            else
                            {
                                if (pag==2)
                                {
                                    voltar();
                                    loopDesconto=0;
                                    pagoCartao=0;
                                    break;
                                }
                            }
                        break;
                        }




                        else if (totalDesc<pagoCartao)
                        {
                            printf("\n|--------------------------------------------------------------|\n");
                            printf("|  PAGAMENTO MAIOR QUE O TOTAL A PAGAR, FAVOR TENTE            |\n");
                            printf("|  REALIZAR O PAGAMENTO NOVAMENTE...                           |\n");
                            printf("|--------------------------------------------------------------| \n\n");
                            getchar();
                            clearScreen();
                        }
                    }


                    break;
                case 3:
                    voltar();
                    break;

                default:
                    opinvalida();

                }
            }
            //break;
        }
    }
}
//=================================================== FIM PAGAMENTOS

//=================================================== ABERTURA DE CAIXA
void aberturaCaixa()
{
    int abrir=0;
    while (abrir !=2)
    {
        clearScreen();
        printf("\n|---------------------------------------|\n");
        printf("| TEM CERTEZA QUE DESEJA ABRIR O CAIXA? |\n");
        printf("|                                       |\n");
        printf("|  1. Sim                               |\n");
        printf("|  2. Nao (Voltar ao Menu Principal)    |\n");
        printf("|                                       |\n");
        printf("| - - - - - - - - - - - - - - - - - - - |\n");
        printf("|                                       |\n");
        printf("| Escolha uma das opcoes acima:         |\n");
        printf("| (Somente numero)                      |\n");
        printf("|_______________________________________|\n\n");
        scanf("%d", &abrir);

        if (abrir ==1)
        {
            clearScreen();
            printf("\n|--------------------------------------------------|\n");
            printf("|               ABERTURA DE CAIXA                  |\n");
            printf("|--------------------------------------------------|\n");
            printf("| Digite o valor inicial do caixa:                 |\n");
            printf("|--------------------------------------------------|\n\n  R$ ");
            scanf("%f", &inicioCaixa);

            //se o inicio do caixa for menor que 0
            if(inicioCaixa<0)
            {
                opinvalida();
            }

            //abrir caixa
            else
            {
                if (inicioCaixa==-0)
                {
                    inicioCaixa=0;
                }
                vCaixa = vCaixa + inicioCaixa;
                caixa = 'A';

                arquivoCaixa = fopen("caixa.dat", "wb");
                fwrite(&caixa, sizeof(char), 1, arquivoCaixa);
                fwrite(&inicioCaixa, sizeof(float), 1, arquivoCaixa);
                fclose(arquivoCaixa);

                clearScreen();
                break;


            }
        }
        else
        {
            if (abrir ==2)
            {
                voltar();
            }
            else
            {
                opinvalida();
            }
        }
    }

}

// função para menu de caixa já aberto
void caixaAberto()
{
    clearScreen();
    printf("\n|------------------------------------|\n");
    printf("|         CAIXA ABERTO...            |\n");
    printf("|------------------------------------|\n\n");
    getchar();
    clearScreen();
}

//=================================================== FIM ABERTURA DE CAIXA

//=================================================== FECHAMENTO DE CAIXA
void fechamentoCaixa()
{
    //caso já tenha ocorrido um pagamento, não pode fechar o caixa antes de concluir
    if (pagoDin>0||pagoCartao>0)
    {
        clearScreen();
        printf("\n|------------------------------------------------|\n");
        printf("|  CONCLUA O PAGAMENTO PARA FECHAR O CAIXA...    |\n");
        printf("|------------------------------------------------| \n\n");
        getchar();
        clearScreen();
    }

    //fechar caixa
    else
    {
        clearScreen();
        totalFat=totalDin+totalCartao+totalDinCartao;
        totalDinCartao+=pagoDin+pagoCartao;
        printf("\n|--------------------------------------------------|\n");
        printf("|               FECHAMENTO DE CAIXA                |\n");
        printf("|--------------------------------------------------|\n");
        printf("  Vendas Realizadas: %d                                \n", numVendas);
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - -|\n");
        printf("  VALOR DE ABERTURA: R$ %.2f \n", inicioCaixa);
        printf("  TOTAL PAGO EM DINHEIRO: R$ %.2f \n", totalDin);
        printf("  TOTAL PAGO EM CARTAO: R$ %.2f \n", totalCartao);
        printf("  TOTAL FATURADO: R$ %.2f \n", totalFat);
        printf("|--------------------------------------------------|\n\n");

        int fechar = 0;
        while (fechar != 1)
        {
            printf("\n|----------------------------------------------|\n");
            printf("|  TEM CERTEZA QUE DESEJA FECHAR O CAIXA?      |\n");
            printf("|                                              |\n");
            printf("|  1. Sim                                      |\n");
            printf("|  2. Nao                                      |\n");
            printf("|----------------------------------------------|\n\n");
            scanf("%d", &fechar);

            //caso escolha sair
            if (fechar == 1)
            {
                caixaFechado();
                caixa='F';
                vendasRealizadas=0;
                inicioCaixa=0;
                totalDin=0;
                totalCartao=0;
                totalDinCartao=0;
                totalFat=0;

                arquivoCaixa = fopen("caixa.dat", "wb");
                fwrite(&caixa, sizeof(char), 1, arquivoCaixa);
                fwrite(&inicioCaixa, sizeof(float), 1, arquivoCaixa);
                fclose(arquivoCaixa);

                break;
            }

            //voltar para menu principal
            if (fechar == 2)
            {
                voltar();
                clearScreen();
                break;
            }

            //caso não seja nenhuma das opções (sair ou voltar para menu)
            else
            {
                opinvalida();
                clearScreen();
            }
        }

    }
}

// função de caixa já fechado
void caixaFechado()
{
    clearScreen();
    printf("\n|------------------------------------|\n");
    printf("|           CAIXA FECHADO..          |\n");
    printf("|------------------------------------|\n\n");
    getchar();
    clearScreen();
}
//=================================================== FIM FECHAMENTO DE CAIXA

//=================================================== MENU DE RELATORIOS
void menuRelatorios()
{

    int relatorio=0;
    while (relatorio != 4)
    {
        clearScreen();
        printf("\n|---------------------------------------------|\n");
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

        switch (relatorio)
        {

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
            listaVendas();
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
//=================================================== FIM MENU DE RELATORIOS

//=================================================== RELATÓRIOS DE CLIENTES
//função de listar clientes

void listarComprasPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim) {
    struct {
        int cliente;
        char nomeCliente[30];
        float totalCompras;
    } totalPorCliente[100]; // Assumindo um máximo de 100 clientes únicos
    int numClientes = 0;

    for (int i = 0; i < numVendas; i++) {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim)) {
            int clienteIndex = -1;
            for (int j = 0; j < numClientes; j++) {
                if (totalPorCliente[j].cliente == vendas[i].cliente) {
                    clienteIndex = j;
                    break;
                }
            }

            if (clienteIndex == -1) {
                clienteIndex = numClientes++;
                totalPorCliente[clienteIndex].cliente = vendas[i].cliente;
                strcpy(totalPorCliente[clienteIndex].nomeCliente, vendas[i].nomeCliente);
                totalPorCliente[clienteIndex].totalCompras = 0;
            }

            totalPorCliente[clienteIndex].totalCompras += vendas[i].valorTotal;
        }
    }

    printf("Clientes que compraram no periodo de %d/%d/%d a %d/%d/%d:\n",
           inicio.dia, inicio.mes, inicio.ano, fim.dia, fim.mes, fim.ano);
    for (int i = 0; i < numClientes; i++) {
        printf("Codigo: %d Cliente: %s Total Compras: %.2f\n",
               totalPorCliente[i].cliente, totalPorCliente[i].nomeCliente, totalPorCliente[i].totalCompras);
    }
}

// Função para listar os clientes que mais compraram por período
void listarClientesMaisCompraramPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim) {
    struct {
        int cliente;
        char nomeCliente[30];
        float valorTotalCompras;
    } totalPorCliente[100]; // Assumindo um máximo de 100 clientes diferentes
    int numClientes = 0;

    // Inicializar totalPorCliente
    for (int i = 0; i < 100; i++) {
        totalPorCliente[i].cliente = 0;
        strcpy(totalPorCliente[i].nomeCliente, "");
        totalPorCliente[i].valorTotalCompras = 0.0;
    }

    // Calcular valor total de compras por cliente no período
    for (int i = 0; i < numVendas; i++) {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim)) {
            int clienteIndex = -1;
            // Procurar cliente na lista totalPorCliente
            for (int j = 0; j < numClientes; j++) {
                if (totalPorCliente[j].cliente == vendas[i].cliente) {
                    clienteIndex = j;
                    break;
                }
            }
            // Se cliente não estiver na lista, adicioná-lo
            if (clienteIndex == -1) {
                clienteIndex = numClientes++;
                totalPorCliente[clienteIndex].cliente = vendas[i].cliente;
                strcpy(totalPorCliente[clienteIndex].nomeCliente, vendas[i].nomeCliente);
            }
            // Adicionar valor total da venda ao cliente correspondente
            totalPorCliente[clienteIndex].valorTotalCompras += vendas[i].valorTotal;
        }
    }

    // Ordenar clientes por valor total de compras em ordem decrescente
    for (int i = 0; i < numClientes - 1; i++) {
        for (int j = i + 1; j < numClientes; j++) {
            if (totalPorCliente[i].valorTotalCompras < totalPorCliente[j].valorTotalCompras) {
                // Trocar os clientes de lugar
                int tempCliente = totalPorCliente[i].cliente;
                totalPorCliente[i].cliente = totalPorCliente[j].cliente;
                totalPorCliente[j].cliente = tempCliente;

                float tempValor = totalPorCliente[i].valorTotalCompras;
                totalPorCliente[i].valorTotalCompras = totalPorCliente[j].valorTotalCompras;
                totalPorCliente[j].valorTotalCompras = tempValor;

                char tempNome[30];
                strcpy(tempNome, totalPorCliente[i].nomeCliente);
                strcpy(totalPorCliente[i].nomeCliente, totalPorCliente[j].nomeCliente);
                strcpy(totalPorCliente[j].nomeCliente, tempNome);
            }
        }
    }

    // Imprimir relatório dos clientes que mais compraram
    printf("Clientes que mais compraram no período de %d/%d/%d a %d/%d/%d:\n",
           inicio.dia, inicio.mes, inicio.ano, fim.dia, fim.mes, fim.ano);
    for (int i = 0; i < numClientes; i++) {
        printf("--------------------------------------------------\n");
        printf("Cliente: %d - %s\n", totalPorCliente[i].cliente, totalPorCliente[i].nomeCliente);
        printf("Valor Total de Compras: %.2f\n", totalPorCliente[i].valorTotalCompras);
    }
    printf("--------------------------------------------------\n");
}

void listaClientes()
{
    int lista = 0;
    while(lista!=4)
    {
        clearScreen();
        printf("\n|---------------------------------------------------|\n");
        printf("|         | |   Relatorio de Clientes   | |         |\n");
        printf("|                                                   |\n");
        printf("|  1. Ordem de Codigo                               |\n");
        printf("|  2. Ordem Alfabetica (A-Z)                        |\n");
        printf("|  3. Periodo de compra                             |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|  4. Voltar para menu de relatorios                |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     Escolha uma das opcoes acima:                 |\n");
        printf("|     (Somente numero)                              |\n");
        printf("|___________________________________________________|\n\n");
        scanf("%d", &lista);

        switch (lista)
        {

        //clientes em ordem de codigo
        case 1:
            clearScreen();
            printf("\n|------------------------------------------------------------|");
            printf("\n|                CLIENTES EM ORDEM DE CODIGO                 |\n");
            for (int i = 0; i < numClientes; i++)
            {
                printf("|------------------------------------------------------------|\n");
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
            printf("|------------------------------------------------------------|\n");
            printf("| - - - - - - - - FIM DA LISTA DE CLIENTES - - - - - - - - - |\n\n");
            getchar();
            break;

        //clientes em ordem alfabetica
        case 2:
            clearScreen();
            printf("\n|------------------------------------------------------------|");
            printf("\n|               CLIENTES EM ORDEM ALFABETICA                 |\n");
            qsort(cliente, numClientes, sizeof(struct CadastroCliente), compararNome);

            for (int i = 0; i < numClientes; i++)
            {
                printf("|------------------------------------------------------------|\n");
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
            printf("|------------------------------------------------------------|\n");
            printf("| - - - - - - - - FIM DA LISTA DE CLIENTES - - - - - - - - - |\n\n");
            getchar();
            break;

        //clientes que compraram em determinado periodo
        case 3:

    // Lendo o período
            printf("Digite a data de inicio do periodo:\n");
            struct Data inicio = lerData();
            printf("Digite a data de fim do periodo:\n");
            struct Data fim = lerData();
      //      listarComprasPorPeriodo(venda, numVendas, inicio, fim);
            listarClientesMaisCompraramPorPeriodo(venda, numVendas, inicio, fim);
        //voltar para menu de relatorios
        case 4:
            voltar();
            break;
        }
    }
}
//=================================================== FIM RELATÓRIOS DE CLIENTES

//=================================================== RELATÓRIOS DE PRODUTOS

// Função para listar os itens mais vendidos por período
void listarItensMaisVendidosPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim) {
    struct {
        int cod;
        char descricao[30];
        int quantidadeVendida;
    } totalPorProduto[100]; // Assumindo um máximo de 100 produtos únicos
    int numProdutos = 0;

    for (int i = 0; i < numVendas; i++) {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim)) {
            for (int j = 0; j < vendas[i].totalProdutos; j++) {
                int produtoIndex = -1;
                for (int k = 0; k < numProdutos; k++) {
                    if (totalPorProduto[k].cod == vendas[i].produtos[j].cod) {
                        produtoIndex = k;
                        break;
                    }
                }

                if (produtoIndex == -1) {
                    produtoIndex = numProdutos++;
                    totalPorProduto[produtoIndex].cod = vendas[i].produtos[j].cod;
                    strcpy(totalPorProduto[produtoIndex].descricao, vendas[i].produtos[j].descricao);
                    totalPorProduto[produtoIndex].quantidadeVendida = 0;
                }

                totalPorProduto[produtoIndex].quantidadeVendida += vendas[i].produtos[j].quantidade;
            }
        }
    }

    // Ordenar os produtos pela quantidade vendida em ordem decrescente
    for (int i = 0; i < numProdutos - 1; i++) {
        for (int j = i + 1; j < numProdutos; j++) {
            if (totalPorProduto[i].quantidadeVendida < totalPorProduto[j].quantidadeVendida) {
                // Troca os produtos
                int tempQuantidade = totalPorProduto[i].quantidadeVendida;
                totalPorProduto[i].quantidadeVendida = totalPorProduto[j].quantidadeVendida;
                totalPorProduto[j].quantidadeVendida = tempQuantidade;

                int tempCod = totalPorProduto[i].cod;
                totalPorProduto[i].cod = totalPorProduto[j].cod;
                totalPorProduto[j].cod = tempCod;

                char tempDescricao[30];
                strcpy(tempDescricao, totalPorProduto[i].descricao);
                strcpy(totalPorProduto[i].descricao, totalPorProduto[j].descricao);
                strcpy(totalPorProduto[j].descricao, tempDescricao);
            }
        }
    }

    printf("Itens mais vendidos no período de %d/%d/%d a %d/%d/%d:\n",
           inicio.dia, inicio.mes, inicio.ano, fim.dia, fim.mes, fim.ano);
    for (int i = 0; i < numProdutos; i++) {
        printf("Codigo: %d Produto: %s Quantidade Vendida: %d\n",
               totalPorProduto[i].cod, totalPorProduto[i].descricao, totalPorProduto[i].quantidadeVendida);
    }
}


void listaProdutos()
{
    int lista = 0;
    while(lista!=5)
    {
        clearScreen();
        printf("\n|---------------------------------------------------|\n");
        printf("|         | |   Relatorio de Produtos   | |         |\n");
        printf("|                                                   |\n");
        printf("|  1. Ordem de Codigo                               |\n");
        printf("|  2. Ordem Alfabetica (A-Z)                        |\n");
        printf("|  3. Estoque Minimo / Sem Estoque                  |\n");
        printf("|  4. Mais Vendidos                                 |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|  5. Voltar para menu de relatorios                |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     Escolha uma das opcoes acima:                 |\n");
        printf("|     (Somente numero)                              |\n");
        printf("|___________________________________________________|\n\n");
        scanf("%d", &lista);

        switch (lista)
        {

        //produto em ordem de codigo
        case 1:
            clearScreen();
            printf("\n|------------------------------------------------------------|");
            printf("\n|                PRODUTOS EM ORDEM DE CODIGO                 |\n");
            for (int i = 0; i < numProdutos; i++)
            {
                printf("|------------------------------------------------------------|\n");
                printf("| Codigo.........: %d\n", produto[i].cod);
                printf("| Descricao......: %s\n", produto[i].descricao);
                printf("| Categoria......: %s\n", produto[i].categoria.descricao);
                printf("| Preco de Compra: %.2f\n", produto[i].precoCompra);
                printf("| Preco de Venda.: %.2f\n", produto[i].precoVenda);
                printf("| Estoque Atual..: %d\n", produto[i].estoque);
            }
            printf("|------------------------------------------------------------|\n");
            printf("| - - - - - - - - FIM DA LISTA DE PRODUTOS - - - - - - - - - |\n\n");
            getchar();
            break;

        //produto em ordem alfabetica
        case 2:
            clearScreen();
            printf("\n|------------------------------------------------------------|");
            printf("\n|               PRODUTOS EM ORDEM ALFABETICA                 |\n");
            qsort(produto, numProdutos, sizeof(struct CadastroProduto), compararProduto);

            for (int i = 0; i < numProdutos; i++)
            {
                printf("|------------------------------------------------------------|\n");
                printf("| Codigo.........: %d\n", produto[i].cod);
                printf("| Descricao......: %s\n", produto[i].descricao);
                printf("| Categoria......: %s\n", produto[i].categoria.descricao);
                printf("| Preco de Compra: %.2f\n", produto[i].precoCompra);
                printf("| Preco de Venda.: %.2f\n", produto[i].precoVenda);
                printf("| Estoque Atual..: %d\n", produto[i].estoque);
                printf("| Estoque Minimo.: %d\n", produto[i].estoqueminimo);
            }
            printf("|------------------------------------------------------------|\n");
            printf("| - - - - - - - - FIM DA LISTA DE PRODUTOS - - - - - - - - - |\n\n");
            getchar();
            break;

        //produtos sem estoque ou estoque minimo
        case 3:
            clearScreen();
            printf("\n|------------------------------------------------------------|");
            printf("\n|        PRODUTOS COM ESTOQUE MINIMO / SEM ESTOQUE           |\n");
            for (int i = 0; i < numProdutos; i++)
            {
                if((produto[i].estoque==produto[i].estoqueminimo)||(produto[i].estoque==0))
                {
                    printf("|------------------------------------------------------------|\n");
                    printf("| Codigo.........: %d\n", produto[i].cod);
                    printf("| Descricao......: %s\n", produto[i].descricao);
                    printf("| Categoria......: %s\n", produto[i].categoria.descricao);
                    printf("| Preco de Compra: %.2f\n", produto[i].precoCompra);
                    printf("| Preco de Venda.: %.2f\n", produto[i].precoVenda);
                    printf("| Estoque Atual..: %d\n", produto[i].estoque);
                    printf("| Estoque Minimo.: %d\n", produto[i].estoqueminimo);
                }
            }
            printf("|------------------------------------------------------------|\n");
            printf("| - - - - - - - - FIM DA LISTA DE PRODUTOS - - - - - - - - - |\n\n");
            getchar();
            break;

        //produtos mais vendidos
        case 4:
    // Lendo o período
            printf("Digite a data de inicio do periodo:\n");
            struct Data inicio = lerData();
            printf("Digite a data de fim do periodo:\n");
            struct Data fim = lerData();
            listarItensMaisVendidosPorPeriodo(venda, numVendas, inicio, fim);

        //voltar para menu de relatorios
        case 5:
            voltar();
            break;
        }
    }
}
//=================================================== FIM RELATÓRIOS DE PRODUTOS

//=================================================== RELATORIO DE VENDAS

// Função para listar as vendas por período
void listarVendasPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim) {
    printf("Listagem de vendas no período de %d/%d/%d a %d/%d/%d:\n",
           inicio.dia, inicio.mes, inicio.ano, fim.dia, fim.mes, fim.ano);
    
    for (int i = 0; i < numVendas; i++) {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim)) {
            printf("--------------------------------------------------\n");
            printf("Número da Venda: %d\n", vendas[i].num);
            printf("Cliente: %d - %s\n", vendas[i].cliente, vendas[i].nomeCliente);
            printf("Data da Venda: %d/%d/%d\n", vendas[i].data.dia, vendas[i].data.mes, vendas[i].data.ano);
            printf("Valor Total: %.2f\n", vendas[i].valorTotal);
            printf("Método de Pagamento: %c\n", vendas[i].metodoPagamento);
            
            printf("Produtos:\n");
            for (int j = 0; j < vendas[i].totalProdutos; j++) {
                printf("   Codigo: %d, Descricao: %s, Quantidade: %d, Preço Unitario: %.2f, Valor Total: %.2f\n",
                       vendas[i].produtos[j].cod, vendas[i].produtos[j].descricao,
                       vendas[i].produtos[j].quantidade, vendas[i].produtos[j].precoVenda,
                       vendas[i].produtos[j].valorTotal);
            }
        }
    }
    printf("--------------------------------------------------\n");
}

void listaVendas()
{
    int lista = 0;
    int i;
    int j;
    while(lista!=3)
    {
        clearScreen();
        printf("\n|---------------------------------------------------|\n");
        printf("|          | |   Relatorio de Vendas   | |          |\n");
        printf("|                                                   |\n");
        printf("|  1. Ordem de Codigo                               |\n");
        printf("|  2. Periodo de Venda                              |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|  3. Voltar para menu de relatorios                |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     Escolha uma das opcoes acima:                 |\n");
        printf("|     (Somente numero)                              |\n");
        printf("|___________________________________________________|\n\n");
        scanf("%d", &lista);

        switch (lista)
        {
        //vendas por ordem de codigo
        case 1:

            clearScreen();
            printf("\n|------------------------------------------------------------|");
            printf("\n|               VENDAS EM ORDEM DE CODIGO                    |\n");

            for (int i = 0; i < numVendas; i++)
            {
                printf("|------------------------------------------------------------|\n");
                printf("|  Numero de Venda: %d\n", venda[i].num);
                printf("|  Data...........: %02d/%02d/%d\n", venda[i].data.dia, venda[i].data.mes, venda[i].data.ano);
                printf("|  Cliente........: %s\n", venda[i].nomeCliente);
                printf("|  Total Pago.....: %.2f\n", venda[i].valorTotal);
                printf("|  Qtd Itens......: %d\n", venda[i].totalProdutos);
                printf("|------------------------------------------------------------|\n");

                for (int j = 0; j < venda[i].totalProdutos; j++)
                {
                    printf("|  Codigo.........: %d\n", venda[i].produtos[j].cod);
                    printf("|  Descricao......: %s\n", venda[i].produtos[j].descricao);
                    printf("|  Quantidade.....: %d\n", venda[i].produtos[j].quantidade);
                    printf("|  Preco de Venda.: %.2f\n", venda[i].produtos[j].precoVenda);
                    printf("|  Total Bruto....: %.2f\n", venda[i].produtos[j].valorTotal);
                    printf("|  Pago em........: %c\n", venda[i].metodoPagamento);
                }
            }
            printf("|------------------------------------------------------------|\n");
            printf("|- - - - - - - - - FIM DA LISTA DE VENDAS - - - - - - - - - -|\n\n");

            getchar();
            break;

        //vendas em determinado periodo
        case 2:
    // Lendo o período
            printf("Digite a data de inicio do periodo:\n");
            struct Data inicio = lerData();
            printf("Digite a data de fim do periodo:\n");
            struct Data fim = lerData();
            listarVendasPorPeriodo(venda, numVendas, inicio, fim);

        //voltar para menu de vendas
        case 3:
            voltar();
            break;

        }
    }
}
//=================================================== FIM RELATÓRIO DE VENDAS

//=================================================== FECHAR PROGRAMA
void saida()
{

    int sair = 0;
    while (sair != 1)
    {
        printf("\n|------------------------------------|\n");
        printf("|  TEM CERTEZA QUE DESEJA SAIR?      |\n");
        printf("|                                    |\n");
        printf("|  1. Sim                            |\n");
        printf("|  2. Nao                            |\n");
        printf("|------------------------------------|\n\n");
        scanf("%d", &sair);

        //caso escolha sair
        if (sair == 1)
        {
            //Salva os dados em arquivo
            char salvar;
            while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N'))
            {
                printf("\n|------------------------------------------------------|\n");
                printf("|  DESEJA SALVAR OS DADOS DE CLIENTES E PRODUTOS       |\n");
                printf("|  CADASTRADOS, E DADOS DE VENDAS? (s/n)               |\n");
                printf("|------------------------------------------------------|\n\n");
                scanf(" %c", &salvar);
                fflush(stdin);

                if (salvar == 's' || salvar == 'S')
                {

/*
                //    posicaoCaixa->caixa=caixa;
                    printf("Caixa: %c\n", caixa);
                    printf("\nPasso 1...\n\n");
                    posicaoCaixa->caixa = caixa;
                    printf("\nPasso 2...\n\n");
                    posicaoCaixa->inicioCaixa=inicioCaixa;
                    printf("\nPasso 3...\n\n");
                    posicaoCaixa->totalCartao=totalCartao;
                    printf("\nPasso 4...\n\n");
                    posicaoCaixa->totalDin=totalDin;
                    printf("\nPasso 5...\n\n");
                    posicaoCaixa->vCaixa=vCaixa;
                    printf("\nPasso 6...\n\n");
                    arquivoCaixa = fopen("caixa.dat", "wb");
                    printf("\nPasso 7...\n\n");
                    fwrite(posicaoCaixa, sizeof(struct SituacaoCaixa), 1, arquivoCaixa);
                    printf("\nPasso 8...\n\n");
                    fclose(arquivoCaixa);
                    printf("\nPasso 9...\n\n");
*/
                    /*arquivoCliente = fopen("clientes.dat", "wb");
                    fwrite(&numClientes, sizeof(int), 1, arquivoCliente);
                    fwrite(cliente, sizeof(struct CadastroCliente), numClientes, arquivoCliente);
                    fclose(arquivoCliente);

                    arquivoProduto = fopen("produtos.dat", "wb");
                    fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                    fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                    fclose(arquivoProduto);*/

                    arquivoVenda = fopen("vendas.dat", "wb");
                    fwrite(&numVendas, sizeof(int), 1, arquivoVenda);
                    for (int i = 0; i < numVendas; i++)
                    {
                        fwrite(&venda[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);
                        fwrite(venda[i].produtos, sizeof(struct ProdutoVenda), venda[i].totalProdutos, arquivoVenda);
                    }
                    fclose(arquivoVenda);

                    clearScreen();
                    printf("\nClientes e Produtos salvos, Fechando Programa...\n\n");
                    break;
                }

                if (salvar == 'n' || salvar == 'N')
                {
                    clearScreen();
                    printf("\nClientes e produtos nao salvos. Fechando Programa...\n\n");
                    break;
                }

                else
                {
                    opinvalida();
                }
            }
            exit(0);
        }

        //voltar para menu principal
        if (sair == 2)
        {
            voltar();
            main();
            clearScreen();
        }

        //caso não seja nenhuma das opções (sair ou voltar pra menu)
        else
        {
            opinvalida();
            clearScreen();
        }
    }
}
//=================================================== FIM FECHAR PROGRAMA



// PRINT DE OPÇÃO INVÁLIDA
void opinvalida()
{
    printf("\n|----------------------------------------------------|\n");
    printf("|  OPCAO INVALIDA! FAVOR INSERIR UM VALOR VALIDO...  |\n");
    printf("|----------------------------------------------------| \n\n");
    getchar();
    clearScreen();
}

// PRINT DE VOLTAR PARA MENU ANTERIOR
void voltar()
{
    printf("\n|------------------------------------|\n");
    printf("|  VOLTANDO PARA O MENU ANTERIOR...  |\n");
    printf("|------------------------------------|\n\n");
    getchar();
    clearScreen();
}

void adicionarProduto(struct ProdutoVenda *produtovenda, int num, int cod, const char *descricao, int quantidade, float precoVenda)
{
    printf("Mostra preco2: %.2f\n", precoVenda);

    produtovenda->num = num;
    produtovenda->cod = cod;
    strncpy(produtovenda->descricao, descricao, sizeof(produtovenda->descricao) - 1);
    produtovenda->descricao[sizeof(produto->descricao) - 1] = '\0';
    produtovenda->quantidade = quantidade;

//    produtovenda->precoVenda = 0.0f;
//    produtovenda->valorTotal = 0.0f;


    produtovenda->precoVenda = precoVenda;
    produtovenda->valorTotal = quantidade * precoVenda;
}

void adicionarVenda(struct CadastroVenda *venda, int num, int cliente, const char *nomeCliente, struct Data data)
{
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

//=================================================== FUNÇÃO PRINCIPAL
void main()
{
//    carregarDadosCaixa();
    contadorMain++;
    if (contadorMain==1)   //  Primeira vez
    {
        numusuarios = 0;
        carregarDadosCaixa();
        carregarDadosCategoria();
        carregarDadosUsuarios();
        carregarDadosClientes();
        carregarDadosProdutos();
        carregarDadosVendas(&venda);
        
        if (numusuarios==0){
          printf("\n Nao existem usuarios cadastrados, redirecionando para o cadastro de usuarios...");
          getchar();
        
          cadastroUsuario();
        }
         //LOGIN DO USUARIO
        while (true)
        {
            char descricao[30];            
            clearScreen();
            printf("\n|-----------------------------------------------------|\n");
            printf("|                                                     |\n");
            printf("|                  | |   LOGIN   | |                  |\n");
            printf("|                                                     |\n");
            printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  Usuario: ");
            fgets(descricao, sizeof(descricao), stdin);
            descricao[strcspn(descricao, "\n")] = 0;
            fflush(stdin);

            char password[20];
            printf("\n\n  Senha: ");
            get_password(password, sizeof(password));
            password[strcspn(password, "\n")] = 0;

            strcpy(nomeUsuario, " ");
            for (int i = 0; i < numusuarios; i++)
            {
               if (strcmp(descricao, usuario[i].login) == 0 && strcmp(password, usuario[i].password) == 0) 
               {
                 strncpy(nomeUsuario, usuario[i].login, sizeof(usuario[i].login) - 1);
                 strcpy(&tipoUsuario, &usuario[i].tipo);
                 break;  
               }
            }
            if (strcmp(nomeUsuario, " ") == 0) {
                 printf("\n Usuario e/ou senha nao cadastrados");
                 getchar();
            }
            else
            {
              break;    
            }
        }
        
    }

    do
    {
        //menu principal
        menuPrincipal();

        switch(opcao)
        {

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
            if (caixa == 'A')
            {
                caixaAberto();
            }

            else
            {
                if (tipoUsuario=='1') {
                  aberturaCaixa();
                }
                else{
                 printf("\n Usuario sem permissao de acesso\n");
                 getchar();
                }
            }
            break;

        //fechamento de caixa
        case 4:

            //se o caixa já estiver fechado
            if (caixa == 'F')
            {
                caixaFechado();
            }

            else
            {
              if (tipoUsuario=='1') {
                fechamentoCaixa();
              }
              else{
                printf("\n Usuario sem permissao de acesso\n");
                getchar();
              }   
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
    // Liberação de memória alocada
    free(venda[numVendas].produtos);
    free(venda);

}