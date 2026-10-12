//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
//#include <termios.h>
#include <windows.h>
#include <unistd.h>

//declaracao de structs
struct CadastroCliente *cliente = NULL;
struct CadastroUsuario *usuario = NULL;
struct CadastroCategoria *categoria = NULL;
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
int contadorMain = 0, numClientes = 0, numProdutos = 0, numVendas = 0, numPagamentos = 0, opcao = 0, loopDesconto=0, indProduto=0, numusuarios=0, numcategorias=0;
int vendasRealizadas = 0, quantidade=0;
float inicioCaixa = 0.0, totalCartao = 0.0, totalDin = 0.0, totalFat= 0.0, vCaixa = 0.0, totalSangria = 0.0;
float totalDesc = 0, totalBruto = 0, pagoDin = 0, pagoCartao= 0.0, desconto = 0, troco = 0.0, retirada = 0.0, margem = 0, aPagar = 0;
char caixa = 'F', tipoUsuario = ' ';
char nomeUsuario[LOGIN_MAX + 1];
FILE *arquivoCliente, *arquivoProduto, *arquivoCaixa, *arquivoVenda, *arquivoUsuario, *arquivoCategoria, *arquivoCaixa;

//=================================================== STRUCTS
//struct de cadastro de usuarios
struct CadastroUsuario
{
    int cod;
    char login[LOGIN_MAX + 1];
    char password[PASSWORD_MAX + 1];
    char tipo; // 1: Administrador, 2: Usuário
};
//struct de cadastro de clientes
struct CadastroCliente
{
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
struct CadastroCategoria
{
    int cod;
    char descricao[30];
};
//struct de cadastro de produtos
struct CadastroProduto
{
    int cod;
    char descricao[30];
    struct CadastroCategoria categoria;
    float precoCompra;
    float margem;
    float precoVenda;
    int estoque;
    int estoqueminimo;
};

struct ProdutoVenda
{
    int num;
    int cod;
    char descricao[30];
    int quantidade;
    float precoVenda;
    float valorTotal;
};

struct Data
{
    int dia;
    int mes;
    int ano;
};

struct Data obterDataAtual()
{
    struct Data dataAtual;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    dataAtual.dia = tm.tm_mday;
    dataAtual.mes = tm.tm_mon + 1;
    dataAtual.ano = tm.tm_year + 1900;
    return dataAtual;
}

struct CadastroVenda
{
    int num;
    int cliente;
    char nomeCliente[30];
    struct Data data;
    float valorTotal;
    float desconto;
    int totalProdutos;
    char metodoPagamento[3];
    struct ProdutoVenda *produtos;
};

struct SituacaoCaixa
{
    char caixa;
    float inicioCaixa;
    float totalDin;
    float totalCartao;
    float vCaixa;
};

// Função para ler uma data do usuário
struct Data lerData()
{
    struct Data data;
    printf("Digite a data no formato DD MM AAAA: ");
    scanf("%d %d %d", &data.dia, &data.mes, &data.ano);
    return data;
}
//=================================================== FIM STRUCTS

int compararDatas(struct Data d1, struct Data d2)
{
    if (d1.ano > d2.ano) return 1;
    if (d1.ano < d2.ano) return -1;
    if (d1.mes > d2.mes) return 1;
    if (d1.mes < d2.mes) return -1;
    if (d1.dia > d2.dia) return 1;
    if (d1.dia < d2.dia) return -1;
    return 0;
}

int dataDentroDoPeriodo(struct Data data, struct Data inicio, struct Data fim)
{
    return (compararDatas(data, inicio) >= 0 && compararDatas(data, fim) <= 0);
}

//função que impede letras onde pede-se números
bool validaNumero(char *numero, int tamanho)
{
    if (numero==NULL)
    {
        return false;
    }
    int len = strlen(numero);
    if ((tamanho>0) && (len != tamanho))
    {
        return false;
    }
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(numero[i]))
        {
            return false;
        }
    }
    return true;
}

//função que impede mandar espaço vazio
bool validacaracter(char *caracter)
{
    int len = strlen(caracter);
    if (len == 0)
    {
        return false;
    }
    return true;
}

float truncar(float valor, int casas_decimais)
{
    float potencia = pow(10.0f, casas_decimais);
    return truncf(valor * potencia) / potencia;
}

int pesquisaCliente(int codigo)
{
    int i;
    int resultado =-1;
    for (int i = 0; i < numClientes; i++)
    {
        if (cliente[i].cod==codigo)
        {
            resultado = i;
        }
    }
    return resultado;
}

int pesquisaItem(int codigo)
{
    int i;
    int resultado =-1;
    for (int i = 0; i < numProdutos; i++)
    {
        if (produto[i].cod==codigo)
        {
            resultado = i;
        }
    }
    return resultado;
}

int pesquisaCategoria(int codigo)
{
    int i;
    int resultado =-1;
    for (int i = 0; i < numcategorias; i++)
    {
        if (categoria[i].cod==codigo)
        {
            resultado = i;
        }
    }
    return resultado;
}

//comparar nome de clientes para ordem de codigo
int compararCodCliente(const void *a, const void *b) {
    struct CadastroCliente *cliente_a = (struct CadastroCliente *)a;
    struct CadastroCliente *cliente_b = (struct CadastroCliente *)b;
    return cliente_a->cod - cliente_b->cod;
}

//comparar nome de produto para ordem de codigo
int compararCodProduto(const void *a, const void *b) {
    struct CadastroProduto *produto_a = (struct CadastroProduto *)a;
    struct CadastroProduto *produto_b = (struct CadastroProduto *)b;
    return produto_a->cod - produto_b->cod;
}

//comparar nome de categoria de produtos para ordem de codigo
int compararCodCategoria(const void *a, const void *b) {
    struct CadastroCategoria *categoria_a = (struct CadastroCategoria *)a;
    struct CadastroCategoria *categoria_b = (struct CadastroCategoria *)b;
    return categoria_a->cod - categoria_b->cod;
}

//comparar nome de clientes para ordem alfabetica
int compararNome(const void *a, const void *b)
{
    return strcmp(((struct CadastroCliente *)a)->nome, ((struct CadastroCliente *)b)->nome);
}

//comparar nome de produtos para ordem alfabetica
int compararProduto(const void *a, const void *b)
{
    return strcmp(((struct CadastroProduto *)a)->descricao, ((struct CadastroProduto *)b)->descricao);
}

//comparar nome de categorias para ordem alfabetica
int compararCategoria(const void *a, const void *b)
{
    return strcmp(((struct CadastroCategoria *)a)->descricao, ((struct CadastroCategoria *)b)->descricao);
}

//=================================================== MENU PRINCIPAL
void menuPrincipal()
{

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
void menuCadastros()
{

    int cadastrar=0;
    while(cadastrar != 5)
    {
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

        switch (cadastrar)
        {

        //cadastrar usuarios
        case 1:
            if (tipoUsuario=='1')
            {
                cadastroUsuario();
            }
            else
            {
                printf("\n|------------------------------------|\n");
                printf("|  Usuario sem permissao de acesso.  |\n");
                printf("|------------------------------------|\n");
                _getch();
            }
            break;

        //cadastrar produtos
        case 2:
            cadastroCliente();
            break;

        //cadastrar produtos
        case 3:
            cadastroProduto();
            break;

        //cadastrar categorias
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
//=================================================== FIM MENU DE CADASTROS

//=================================================== SENHA DO LOGIN
//função de senha
void get_password(char *password, int maxLength)
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    DWORD count;
    char ch;
    int i = 0;

    // Obtém o modo atual do console
    GetConsoleMode(hStdin, &mode);
    // Desativa o eco
    SetConsoleMode(hStdin, mode & ~(ENABLE_ECHO_INPUT));

    // Lê a senha
    while (i < maxLength - 1 && ReadConsole(hStdin, &ch, 1, &count, NULL) && ch != '\n' && ch != '\r')
    {
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

//função para limpar a tela
void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hConsole == INVALID_HANDLE_VALUE) return;

    // Obtém a informação da tela do console
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // Preenche a tela com espaços
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', cellCount, homeCoords, &count)) return;

    // Define os atributos
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) return;

    // Move o cursor para o topo da tela
    SetConsoleCursorPosition(hConsole, homeCoords);
}

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
//=================================================== FIM SENHA DO LOGIN

//=================================================== CADASTRAR USUARIOS
void cadastroUsuario()
{
    char confirma;
    do
    {

        usuario = realloc(usuario, (numusuarios + 1) * sizeof(struct CadastroUsuario));
        if (usuario == NULL)
        {
            printf("\n|-----------------------------------------------------|\n");
            printf("| ERRO NA ALOCACAO DE MEMORIA! ENCERRANDO PROGRAMA... |\n");
            printf("|-----------------------------------------------------|\n");
            exit(1);
        }

        usuario[numusuarios].cod = numusuarios + 1;

        //LOGIN DO USUARIO
        fflush(stdin);
        while (true)
        {
            char descricao[30];
            clearScreen();
            printf("\n|-----------------------------------------------------|\n");
            printf("|                                                     |\n");
            printf("|           | |   Cadastrar Usuario   | |             |\n");
            printf("|                                                     |\n");
            printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  1. Digite o login do usuario: \n  (minimo de 8 caracteres, maximo de 12 caracteres)\n  - ");
            fgets(descricao, sizeof(descricao), stdin);
            printf("\n| - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
//            usuario[numusuarios].login[strcspn(usuario[numusuarios].login, "\n")] = 0;
            descricao[strcspn(descricao, "\n")] = 0;
            if (validacaracter(descricao))
            {
                if ((strlen(descricao)>=LOGIN_MIN) && (strlen(descricao)<=LOGIN_MAX))
                {
                    strncpy(usuario[numusuarios].login, descricao, sizeof(descricao) - 1);
                    break;
                }
                else
                {
                    printf("\n  Erro! login com tamanho invalido. Tente Novamente.");

                    _getch();
                }

            }
            else
            {
                printf("\n  Erro! login invalido. Tente Novamente.");
                _getch();
            }
        }
        fflush(stdin);

        //SENHA
        while (true)
        {

            char password[20];
            printf("\n  2. Digite a senha do usuario: \n  (minimo de 6 caracteres, maximo de 8 caracteres)  ");
            get_password(password, sizeof(password));

            if (validacaracter(password))
            {
                if ((strlen(password)>=PASSWORD_MIN) && (strlen(password)<=PASSWORD_MAX))
                {
                    strncpy(usuario[numusuarios].password, password, sizeof(password) - 1);
                    break;
                }
                else
                {
                    printf("\n  Erro! senha com tamanho invalido. Tente Novamente.");
                }
            }
            else
            {
                printf("\n  Erro! Senha invalida. Tente Novamente.");
            }
        }
//        fflush(stdin);

        // Tipo
        while (true)
        {
            printf("\n| - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  3. Digite o Tipo do usuario\n  (1-Administrador ou 2-Funcionario): ");
            scanf(" %c", &usuario[numusuarios].tipo);

            if (usuario[numusuarios].tipo == '1' || usuario[numusuarios].tipo == '2')
            {
                if ((numusuarios==0) && (usuario[numusuarios].tipo != '1'))
                {
                    printf("\n  !Erro! Para o cadastro do primeiro usuario \n  tipo deve ser 1-Administrador. Tente Novamente.");
                }
                else
                {
                    break;
                }
            }
            else
            {
                printf("\n  !Erro! tipo invalido. Tente Novamente.");
            }
        }
        fflush(stdin);

        numusuarios++;

//se quiser salvar dados do usuario recem cadastrado
//------------------------------------------------------------------------------------------------
        char salvar;

        while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N'))
        {

            clearScreen();

            printf("\n|---------------------------------------------------------|\n");
            printf("|  DESEJA SALVAR OS DADOS DE usuarios CADASTRADOS? (s/n)  |\n");
            printf("|---------------------------------------------------------|\n\n");
            scanf(" %c", &salvar);
            fflush(stdin);

            if (salvar == 's' || salvar == 'S')
            {
                arquivoUsuario = fopen("usuarios.dat", "wb");
                fwrite(&numusuarios, sizeof(int), 1, arquivoUsuario);
                fwrite(usuario, sizeof(struct CadastroUsuario), numusuarios, arquivoUsuario);
                fclose(arquivoUsuario);
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   USUARIO SALVO...                         |\n");
                break;
            }
            else
            {
                if (salvar == 'n' || salvar == 'N')
                {
                    clearScreen();
                    printf("\n|--------------------------------------------|\n");
                    printf("|   USUARIO NAO SALVO...                     |\n");
                    break;
                }
                else
                {
                    opinvalida();
                }
            }
        }

//se quiser cadastrar mais um usuario
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N'))
        {
            printf("|- - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("|   DESEJA INCLUIR UM NOVO usuario? (s/n)    |\n");
            printf("|--------------------------------------------|\n\n");
            scanf(" %c", &confirma);
            fflush(stdin);
            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N')
            {
                clearScreen();
                break;
            }
            else
            {
                opinvalida();
            }
        }
        if ( confirma == 'n' || confirma == 'N')
        {
            clearScreen();
            break;
        }
    }
    while (confirma =! 'n'||confirma != 'N');
}
//=================================================== FIM CADASTRAR USUARIOS

//=================================================== CADASTRAR CLIENTES
void cadastroCliente()
{
    char confirma;
    do
    {
        cliente = realloc(cliente, (numClientes + 1) * sizeof(struct CadastroCliente));
        if (cliente == NULL)
        {
            printf("\n|-----------------------------------------------------|\n");
            printf("| ERRO NA ALOCACAO DE MEMORIA! ENCERRANDO PROGRAMA... |\n");
            printf("|-----------------------------------------------------|\n");
            exit(1);
        }
        cliente[numClientes].cod = numClientes + 1;

        //NOME DO CLIENTE
        fflush(stdin);
        while (true)
        {
            clearScreen();
            printf("\n|-------------------------------------------------------------|\n");
            printf("|                                                             |\n");
            printf("|               | |   Cadastrar Cliente   | |                 |\n");
            printf("|                                                             |\n");
            printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  1. Digite o nome do cliente: ");
            fgets(cliente[numClientes].nome, sizeof(cliente[numClientes].nome), stdin);
            cliente[numClientes].nome[strcspn(cliente[numClientes].nome, "\n")] = 0;
            if (validacaracter(cliente[numClientes].nome))
            {
                break;
            }
            else
            {
                printf("\n  !Erro! Nome invalido. Tente Novamente.");
            }
        }
        fflush(stdin);

        //NOME SOCIAL
        while (true)
        {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  2. Digite o nome social do cliente: ");
            fgets(cliente[numClientes].nomesocial, sizeof(cliente[numClientes].nomesocial), stdin);
            cliente[numClientes].nomesocial[strcspn(cliente[numClientes].nomesocial, "\n")] = 0;
            if (validacaracter(cliente[numClientes].nomesocial))
            {
                break;
            }
            else
            {
                printf("\n  !Erro! Nome social invalido. Tente Novamente.");
            }
        }
        fflush(stdin);

        // CPF
        while (true)
        {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  3. Digite o CPF do cliente: ");
            scanf("%s", &cliente[numClientes].cpf);
            if (validaNumero(cliente[numClientes].cpf, 11))
            {
                break;
            }
            else
            {
                printf("\n  !Erro! CPF invalido. Tente Novamente.");
            }
        }
        fflush(stdin);

        // RUA
        while (true)
        {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  4. Digite a rua do cliente: ");
            fgets(cliente[numClientes].rua, sizeof(cliente[numClientes].rua), stdin);
            cliente[numClientes].rua[strcspn(cliente[numClientes].rua, "\n")] = 0;
            if (validacaracter(cliente[numClientes].rua))
            {
                break;
            }
            else
            {
                printf("\n  !Erro! Rua invalida. Tente Novamente.");
            }
        }
        fflush(stdin);

        //NUMERO DA CASA
        while (true)
        {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  5. Digite o numero da residencia do cliente: ");
            scanf("%s", &cliente[numClientes].num);

            if (validaNumero(cliente[numClientes].num, 0))
            {
                break;
            }
            else
            {
                fflush(stdin);
                printf("\n  !Erro! Numero de residencia invalido. Tente Novamente.");
            }
        }
        fflush(stdin);

        //BAIRRO
        while (true)
        {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  6. Digite o bairro do cliente: ");
            fgets(cliente[numClientes].bairro, sizeof(cliente[numClientes].bairro), stdin);
            cliente[numClientes].bairro[strcspn(cliente[numClientes].bairro, "\n")] = 0;
            if (validacaracter(cliente[numClientes].bairro))
            {
                break;
            }
            else
            {
                printf("\n  !Erro! Rua invalida. Tente Novamente.");
            }
        }
        fflush(stdin);

        //CEP
        while (true)
        {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  7. Digite o CEP do cliente: ");
//            scanf("%s", &cliente[numClientes].cep);
            fgets(cliente[numClientes].cep, sizeof(cliente[numClientes].cep), stdin);
            cliente[numClientes].cep[strcspn(cliente[numClientes].cep, "\n")] = 0;

            if (validacaracter(cliente[numClientes].cep))
            {
                if (validaNumero(cliente[numClientes].cep, 8))
                {
                    break;
                }
                else
                {
                    printf("\n  !Erro! CEP invalido. Tente Novamente1.");
                    fflush(stdin);
                }
            }
            else
            {
                printf("\n  !Erro! CEP invalido. Tente Novamente2.");
                fflush(stdin);
            }
        }
        fflush(stdin);

        //TELEFONE
        while (true)
        {
            printf("\n|-------------------------------------------------------------|\n");
            printf("\n  8. Digite o telefone/whats do cliente: ");
            fgets(cliente[numClientes].telefone, sizeof(cliente[numClientes].telefone), stdin);
            cliente[numClientes].telefone[strcspn(cliente[numClientes].telefone, "\n")] = 0;
            if (validacaracter(cliente[numClientes].telefone))
            {
                break;
            }
            else
            {
                printf("\n  !Erro! Telefone/whats invalida. Tente Novamente.");
            }
        }
        fflush(stdin);

        numClientes++;

//se quiser salvar dados do cliente recem cadastrado
//------------------------------------------------------------------------------------------------
        char salvar;
        while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N'))
        {
            clearScreen();
            printf("\n|---------------------------------------------------------|\n");
            printf("|  DESEJA SALVAR OS DADOS DE CLIENTES CADASTRADOS? (s/n)  |\n");
            printf("|---------------------------------------------------------|\n\n");
            scanf(" %c", &salvar);
            fflush(stdin);

            if (salvar == 's' || salvar == 'S')
            {
                arquivoCliente = fopen("clientes.dat", "wb");
                fwrite(&numClientes, sizeof(int), 1, arquivoCliente);
                fwrite(cliente, sizeof(struct CadastroCliente), numClientes, arquivoCliente);
                fclose(arquivoCliente);
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   CLIENTE SALVO...                         |\n");
                break;
            }
            else
            {
                if (salvar == 'n' || salvar == 'N')
                {
                    clearScreen();
                    printf("\n|--------------------------------------------|\n");
                    printf("|   CLIENTE NAO SALVO...                     |\n");
                    break;
                }
                else
                {
                    opinvalida();
                }
            }
        }
//se quiser cadastrar mais um cliente
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N'))
        {
            printf("|- - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("|   DESEJA INCLUIR UM NOVO CLIENTE? (s/n)    |\n");
            printf("|--------------------------------------------|\n\n");
            scanf(" %c", &confirma);
            fflush(stdin);
            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N')
            {
                clearScreen();
                break;
            }
            else
            {
                opinvalida();
            }
        }
        if ( confirma == 'n' || confirma == 'N')
        {
            clearScreen();
            break;
        }

    }
    while (confirma =! 'n'||confirma != 'N');
}
//=================================================== FIM CADASTRAR CLIENTES

//=================================================== CADASTRAR PRODUTOS
void cadastroProduto()
{
    if(numcategorias==0)
    {
        printf("\n|---------------------------------------------------------------------|\n");
        printf("| NENHUMA CATEGORIA CADASTRADA, CADASTRE ANTES DE CRIAR UM PRODUTO... |\n");
        printf("|---------------------------------------------------------------------|\n");
        _getch();
    }
    else
    {

        char confirma;
        while (confirma =! 'n'||confirma != 'N')
        {

            produto = realloc(produto, (numProdutos + 1) * sizeof(struct CadastroProduto));
            if (produto == NULL)
            {
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
            while (true)
            {
                clearScreen();
                printf("\n|-------------------------------------------------------------|\n");
                printf("|                                                             |\n");
                printf("|               | |   Cadastrar Produto   | |                 |\n");
                printf("|                                                             |\n");
                printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
                printf("\n  1. Digite o nome do produto: ");
                fgets(produto[numProdutos].descricao, sizeof(produto[numProdutos].descricao), stdin);
                produto[numProdutos].descricao[strcspn(produto[numProdutos].descricao, "\n")] = 0;
                if (validacaracter(produto[numProdutos].descricao))
                {
                    break;
                }
                else
                {
                    printf("\n  !Erro! Nome invalido. Tente Novamente.\n");
                }
            }
            fflush(stdin);

            //CATEGORIA
            while (true)
            {
                printf("\n|-------------------------------------------------------------|\n");
                printf("\n  2. Digite o codigo da categoria do produto: ");
                if (scanf("%d", &produto[numProdutos].categoria.cod) == 1)
                {

                    indProduto=pesquisaCategoria(produto[numProdutos].categoria.cod);
                    if (indProduto==-1)
                    {
                        printf("\n  Categoria nao cadastrada.\n\n");
                        _getch();
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
            while (true)
            {
                printf("\n|-------------------------------------------------------------|\n");
                printf("\n  3. Digite o preco de compra do produto: ");
                if (scanf("%f", &produto[numProdutos].precoCompra) == 1)
                {
                    if  (produto[numProdutos].precoCompra>0)
                    {
                      break;
                    }
                }
                printf("\n  !Erro! Preco de compra invalido. Tente Novamente.\n");
                fflush(stdin);
            }
            fflush(stdin);

            //MARGEM DE LUCRO
            while (true)
            {
                printf("\n|-------------------------------------------------------------|\n");
                printf("\n  4. Digite a porcentagem da margem de lucro do produto: ");
                if (scanf("%f", &produto[numProdutos].margem) == 1)
                {
                    if  (produto[numProdutos].margem>0)
                    {
                      break;
                    }
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
            while (true)
            {
                printf("\n|-------------------------------------------------------------|\n");
                printf("\n  5. Digite o estoque do produto: ");
                if (scanf("%d", &produto[numProdutos].estoque) == 1)
                {
                    if  (produto[numProdutos].estoque>0)
                    {
                      break;
                    }
                }
                printf("\n  !Erro! Estoque invalido. Tente Novamente.\n");
                fflush(stdin);
            }

            //ESTOQUE MINIMO
            while (true)
            {
                printf("\n|-------------------------------------------------------------|\n");
                printf("\n  6. Digite o estoque minimo do produto: ");
                if (scanf("%d", &produto[numProdutos].estoqueminimo) == 1)
                {
                    if  (produto[numProdutos].estoqueminimo>0)
                    {
                      break;
                    }
                }
                printf("\n  !Erro! Estoque minimo invalido. Tente Novamente.\n");
                fflush(stdin);
            }
            numProdutos++;

//se quiser salvar dados do cliente recem cadastrado
//------------------------------------------------------------------------------------------------
            char salvar;
            while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N'))
            {
                clearScreen();
                printf("\n|---------------------------------------------------------|\n");
                printf("|  DESEJA SALVAR OS DADOS DO PRODUTO CADASTRADO? (s/n)    |\n");
                printf("|---------------------------------------------------------|\n\n");
                scanf(" %c", &salvar);
                fflush(stdin);

                if (salvar == 's' || salvar == 'S')
                {
                    arquivoProduto = fopen("produtos.dat", "wb");
                    fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                    fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                    fclose(arquivoProduto);
                    clearScreen();
                    printf("\n|--------------------------------------------|\n");
                    printf("|   PRODUTO SALVO...                         |\n");
                    break;
                }
                else
                {
                    if (salvar == 'n' || salvar == 'N')
                    {
                        clearScreen();
                        printf("\n|--------------------------------------------|\n");
                        printf("|   PRODUTO NAO SALVO...                     |\n");
                        break;
                    }
                    else
                    {
                        opinvalida();
                    }
                }
            }
//se quiser cadastrar mais um produto
//------------------------------------------------------------------------------------------------
            while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N'))
            {
                printf("|- - - - - - - - - - - - - - - - - - - - - - |\n");
                printf("|   DESEJA INCLUIR UM NOVO PRODUTO? (s/n)    |\n");
                printf("|--------------------------------------------|\n\n");
                scanf(" %c", &confirma);
                fflush(stdin);

                if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N')
                {
                    break;
                }
                else
                {
                    opinvalida();
                }
            }
            if ( confirma == 'n' || confirma == 'N')
            {
                clearScreen();
                break;
            }
        }
    }
}
//=================================================== FIM CADASTRAR PRODUTOS

//=================================================== CADASTRAR CATEGORIAS
void cadastroCategoria()
{

    char confirma;
    do
    {
        categoria = realloc(categoria, (numcategorias + 1) * sizeof(struct CadastroCategoria));
        if (categoria == NULL)
        {
            printf("\n|-----------------------------------------------------|\n");
            printf("| ERRO NA ALOCACAO DE MEMORIA! ENCERRANDO PROGRAMA... |\n");
            printf("|-----------------------------------------------------|\n");
            exit(1);
        }
        categoria[numcategorias].cod = numcategorias + 1;

        //LOGIN DO categoria
        fflush(stdin);
        while (true)
        {
            clearScreen();
            printf("\n|-----------------------------------------------------|\n");
            printf("|                                                     |\n");
            printf("|           | |   Cadastrar Categoria   | |           |\n");
            printf("|                                                     |\n");
            printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("\n  1. Digite a categoria: ");
            fgets(categoria[numcategorias].descricao, sizeof(categoria[numcategorias].descricao), stdin);
            categoria[numcategorias].descricao[strcspn(categoria[numcategorias].descricao, "\n")] = 0;
            if (validacaracter(categoria[numcategorias].descricao))
            {
                break;
            }
            else
            {
                printf("\n  Erro! login invalido. Tente Novamente.");
                _getch();
            }
        }
        fflush(stdin);
        numcategorias++;

//se quiser salvar dados do categoria recem cadastrado
//------------------------------------------------------------------------------------------------
        char salvar;
        while ((salvar !='S' || salvar!='s')||(salvar!='n'||salvar!='N'))
        {
            clearScreen();
            printf("\n|-----------------------------------------------------------|\n");
            printf("|  DESEJA SALVAR OS DADOS DE CATEGORIAS CADASTRADOS? (s/n)  |\n");
            printf("|-----------------------------------------------------------|\n\n");
            scanf(" %c", &salvar);
            fflush(stdin);

            if (salvar == 's' || salvar == 'S')
            {
                arquivoCategoria = fopen("categorias.dat", "wb");
                fwrite(&numcategorias, sizeof(int), 1, arquivoCategoria);
                fwrite(categoria, sizeof(struct CadastroCategoria), numcategorias, arquivoCategoria);
                fclose(arquivoCategoria);
                clearScreen();
                printf("\n|--------------------------------------------|\n");
                printf("|   CATEGORIA SALVA...                       |\n");
                break;
            }
            else
            {
                if (salvar == 'n' || salvar == 'N')
                {
                    clearScreen();
                    printf("\n|--------------------------------------------|\n");
                    printf("|   CATEGORIA NAO SALVA...                   |\n");
                    break;
                }
                else
                {
                    opinvalida();
                }
            }
        }
//se quiser cadastrar mais um categoria
//------------------------------------------------------------------------------------------------
        while ((confirma != 'S' || confirma!= 's')||(confirma != 'n' || confirma != 'N'))
        {
            printf("|- - - - - - - - - - - - - - - - - - - - - - |\n");
            printf("|   DESEJA INCLUIR UMA NOVA CATEGORIA? (s/n) |\n");
            printf("|--------------------------------------------|\n\n");
            scanf(" %c", &confirma);
            fflush(stdin);
            if (confirma == 's' || confirma == 'S' || confirma == 'n' || confirma == 'N')
            {
                clearScreen();
                break;
            }
            else
            {
                opinvalida();
            }
        }
        if ( confirma == 'n' || confirma == 'N')
        {
            clearScreen();
            break;
        }
    }
    while (confirma =! 'n'||confirma != 'N');
}
//=================================================== FIM CADASTRAR CATEGORIAS

//=================================================== CARREGAR DADOS DE...
//USUARIOS
carregarDadosUsuarios()
{
    arquivoUsuario = fopen("usuarios.dat", "rb");
    if (arquivoUsuario)
    {
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
    if (arquivoCliente)
    {
        fread(&numClientes, sizeof(int), 1, arquivoCliente);
        cliente = malloc(numClientes * sizeof(struct CadastroCliente));
        fread(cliente, sizeof(struct CadastroCliente), numClientes, arquivoCliente);
        fclose(arquivoCliente);
    }
}

//PRODUTOS
carregarDadosProdutos()
{

    arquivoProduto = fopen("produtos.dat", "rb");
    if (arquivoProduto)
    {
        fread(&numProdutos, sizeof(int), 1, arquivoProduto);
        produto = malloc(numProdutos * sizeof(struct CadastroProduto));
        fread(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
        fclose(arquivoProduto);
    }
}

//CATEGORIA
void carregarDadosCategoria()
{
    arquivoCategoria = fopen("categorias.dat", "rb");
    if (arquivoCategoria)
    {
        fread(&numcategorias, sizeof(int), 1, arquivoCategoria);
        categoria = malloc(numcategorias * sizeof(struct CadastroCategoria));
        fread(categoria, sizeof(struct CadastroCategoria), numcategorias, arquivoCategoria);
        fclose(arquivoCategoria);
    }
}

//CAIXA
void carregarDadosCaixa()
{
    arquivoCaixa = fopen("caixa.dat", "rb");
    if (arquivoCaixa)
    {
        fread(&caixa, sizeof(char), 1, arquivoCaixa);
        fread(&inicioCaixa, sizeof(float), 1, arquivoCaixa);  
        fread(&totalDin, sizeof(float), 1, arquivoCaixa);
        fread(&totalCartao, sizeof(float), 1, arquivoCaixa);
        fread(&totalSangria, sizeof(float), 1, arquivoCaixa);
        fclose(arquivoCaixa);
    }
}

//VENDA
int carregarDadosVendas(struct CadastroVenda **venda)
{
    arquivoVenda = fopen("vendas.dat", "rb");
    numVendas = 0;
    if (arquivoVenda)
    {
        fread(&numVendas, sizeof(int), 1, arquivoVenda);
        *venda = (struct CadastroVenda *)malloc(numVendas * sizeof(struct CadastroVenda));
        if (*venda == NULL)
        {
            perror("Erro ao alocar memória para vendas");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < numVendas; i++)
        {
            fread(&(*venda)[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);
            (*venda)[i].produtos = (struct ProdutoVenda *)malloc((*venda)[i].totalProdutos * sizeof(struct ProdutoVenda));
            if ((*venda)[i].produtos == NULL)
            {
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
void menuVendas()
{

    int vender=0;
    while (vender != 4)
    {
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

        switch (vender)
        {

        //nova venda
        case 1:
            if(numcategorias==0){
                    printf("\n|---------------------------------------------------------------|\n");
                    printf("|  NENHUMA CATEGORIA CADASTRADA. REDIRECIONANDO PARA CADASTRO.  |\n");
                    printf("|---------------------------------------------------------------|\n\n");
                    _getch();
                    clearScreen();
                    cadastroCategoria();
            }
            else{
            if (numProdutos==0){
                    printf("\n|-----------------------------------------------------------|\n");
                    printf("|  NENHUM PRODUTO CADASTRADO. REDIRECIONANDO PARA CADASTRO. |\n");
                    printf("|-----------------------------------------------------------|\n\n");
                    _getch();
                    clearScreen();
                    cadastroProduto();
            }
            else{
            if(numClientes==0){
                    printf("\n|------------------------------------------------------------|\n");
                    printf("|  NENHUM CLIENTE CADASTRADO. REDIRECIONANDO PARA CADASTRO.  |\n");
                    printf("|------------------------------------------------------------|\n\n");
                    _getch();
                    clearScreen();
                    cadastroCliente();
            }
            else{
            novaVenda();

            }break;

        //reirada de caixa
        case 2:
            if (tipoUsuario=='1')
            {
                Sangria();
            }
            else
            {
                printf("\n|------------------------------------|\n");
                printf("|  Usuario sem permissao de acesso.  |\n");
                printf("|------------------------------------|\n");
                _getch();
            }
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
    }
}
//=================================================== FIM MENU DE VENDAS

//=================================================== NOVA VENDA
novaVenda()
{
    if (caixa=='F')
    {
        caixaFechado();
    }
    else
    {
        int continuarVenda = 1;
        while (continuarVenda)
        {
//        if (numVendas >= INICIAL_MAX_VENDAS) {
            venda = (struct CadastroVenda *)realloc(venda, (numVendas + 1) * sizeof(struct CadastroVenda));
            if (venda == NULL)
            {
                perror("Erro ao alocar memoria para vendas");
                exit(EXIT_FAILURE);
            }
//        }
            int codcliente;
            struct Data data = obterDataAtual();
            clearScreen();
            printf("\n|-------------------------------------------------------------|");
            printf("\n|                          CLIENTE                            |");
            printf("\n|-------------------------------------------------------------|");
            printf("\n| CODIGO DO CLIENTE: ");
            scanf("%d", &codcliente);

            indProduto=pesquisaCliente(codcliente);
            if (indProduto==-1)
            {
                printf("\n  Cliente nao cadastrado.\n\n");
                _getch();
                clearScreen();
                continue;
            }

            printf("  Nome do Cliente: %s\n", cliente[indProduto].nome);

            char nomeCliente[30];
            strncpy(nomeCliente, cliente[indProduto].nome, sizeof(cliente[indProduto].nome) - 1);

            adicionarVenda(&venda[numVendas], numVendas+1, codcliente, nomeCliente, data);

            int continuarProduto = 1;
            while (continuarProduto)
            {
                venda[numVendas].produtos = (struct ProdutoVenda *)realloc(venda[numVendas].produtos, (venda[numVendas].totalProdutos+1) * sizeof(struct ProdutoVenda));
                if (venda[numVendas].produtos == NULL)
                {
                    perror("  Erro ao alocar memoria para produtos");
                    exit(EXIT_FAILURE);
                }

                int cod;
                char descricao[30];
                float precoVenda = 0.0f;

                printf("\n|-------------------------------------------------------------|");
                printf("\n|                          PRODUTO                            |");
                printf("\n|-------------------------------------------------------------|");
                printf("\n| CODIGO DO PRODUTO: ");
                scanf("%d", &cod);
                indProduto=pesquisaItem(cod);
                if (indProduto==-1)
                {
                    printf("\nProduto nao cadastrado.\n\n");
                    _getch();
                    clearScreen();
                    continue;
                }
                strncpy(descricao, produto[indProduto].descricao, sizeof(produto[indProduto].descricao) - 1);
                precoVenda = produto[indProduto].precoVenda;

                printf("\n  Produto Escolhido: %s\n", produto[indProduto].descricao);
                printf("  Preco: %.2f\n", produto[indProduto].precoVenda);
                printf("  Estoque Atual: %d\n", produto[indProduto].estoque);
                printf("  Estoque Minimo do Produto: %d\n", produto[indProduto].estoqueminimo);
                printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |\n");

                printf("  Digite a quantidade: ");
                scanf("%d", &quantidade);
                if ((produto[indProduto].estoque-quantidade)<0)
            {
                clearScreen();
                printf("\n|------------------------------------------------------------------------------|");
                printf("\n|  COMPRA MAIOR DO QUE ESTOQUE EXISTENTE, FAVOR ESCOHER UMA QUANTIDADE MENOR.  |");
                printf("\n|------------------------------------------------------------------------------|\n\n");
                _getch();
            }
            else
            {
                if (quantidade<=0)
                {
                    clearScreen();
                    printf("\n|------------------------------------------------------|");
                    printf("\n|  QUANTIDADE ESCOLHIDA IMPOSSIVEL, TENTE NOVAMENTE.   |");
                    printf("\n|------------------------------------------------------|\n\n");
                    _getch();
                }

                else
                {
                    if ((produto[indProduto].estoque-quantidade)<=produto[indProduto].estoqueminimo)
                    {
                        clearScreen();
                        printf("\n|-------------------------------------------|");
                        printf("\n|  ATENCAO! ESTOQUE MINIMO DE %d ATINGIDO.  |", produto[indProduto].estoqueminimo);
                        printf("\n|-------------------------------------------|\n\n");
                        _getch();
                    }

                venda[numVendas].produtos[venda[numVendas].totalProdutos].num = numVendas+1;
                venda[numVendas].produtos[venda[numVendas].totalProdutos].cod = cod;
                strncpy(venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao, descricao, sizeof(venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao) - 1);
                venda[numVendas].produtos[venda[numVendas].totalProdutos].descricao[sizeof(produto->descricao) - 1] = '\0';
                venda[numVendas].produtos[venda[numVendas].totalProdutos].quantidade = quantidade;

                venda[numVendas].produtos[venda[numVendas].totalProdutos].precoVenda = precoVenda;
                venda[numVendas].produtos[venda[numVendas].totalProdutos].valorTotal = quantidade*precoVenda;
                produto[indProduto].estoque -= quantidade;

                printf("  Total do Item.: %.2f\n", venda[numVendas].produtos[venda[numVendas].totalProdutos].valorTotal);

                venda[numVendas].valorTotal += quantidade * precoVenda;

                printf("  Total da Venda: %.2f\n", venda[numVendas].valorTotal);

                venda[numVendas].totalProdutos++;
                totalBruto=venda[numVendas].valorTotal;
                }
            }
                printf("\n|--------------------------------------------|");
                printf("\n|  ADICIONAR OUTRO PRODUTO? (1-Sim, 0-Nao)   |");
                printf("\n|--------------------------------------------|\n\n");
                scanf("%d", &continuarProduto);
            }

            numVendas++;
            continuarVenda = 0;
        }
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
        if (inicioCaixa<50)
        {
            printf("\n|---------------------------------------|\n");
            printf("|  O CAIXA ESTA COM MENOS DE R$ 50.00.  |\n");
            printf("|---------------------------------------|\n");
            _getch();
        }
        else
        {
/*            
        printf("  VALOR DE ABERTURA: R$ %.2f \n", inicioCaixa);
        printf("  TOTAL PAGO EM DINHEIRO: R$ %.2f \n", totalDin);
        printf("  TOTAL PAGO EM CARTAO: R$ %.2f \n", totalCartao);
        printf("  TOTAL FATURADO: R$ %.2f \n", totalFat);
        printf("  SANGRIA: R$ %.2f \n", totalSangria);
*/
            printf("\n|----------------------------------------------------|\n");
            printf("|               |  | SANGRIA | |                     |\n");
            printf("|                                                    |\n");
            printf("|  Digite a quantia em dinheiro a ser retirado:      |\n");
            printf("|----------------------------------------------------|\n");
            printf("  TOTAL EM DINHEIRO NO CAIXA: R$ %.2f\n", inicioCaixa+totalDin-totalSangria);
            printf("|----------------------------------------------------|\n");
            scanf("%f", &retirada);

            if (retirada==0)
            {
                printf("\n  NADA FOI RETIRADO, SEU CAIXA CONTINUA COM R$ %.2f\n", (inicioCaixa+totalDin-totalSangria-retirada));
                _getch();
            }
            else
            {
                if ((inicioCaixa+totalDin-totalSangria-retirada)<50)
                {
                    printf("\n|--------------------------------------------------------------|\n");
                    printf("|  O CAIXA  FICARA COM MENOS DE R$ 50.00, SAQUE IMPOSSIVEL...    |\n");
                    printf("|----------------------------------------------------------------|\n");
                    _getch();
                }

                if ((inicioCaixa+totalDin-totalSangria-retirada)>=50)
                {
//                    totalDin=inicioCaixa+totalDin-retirada;
                    totalSangria += retirada; 

                    arquivoCaixa = fopen("caixa.dat", "wb");
                    fwrite(&caixa, sizeof(char), 1, arquivoCaixa);
                    fwrite(&inicioCaixa, sizeof(float), 1, arquivoCaixa);
                    fwrite(&totalDin, sizeof(float), 1, arquivoCaixa);
                    fwrite(&totalCartao, sizeof(float), 1, arquivoCaixa);
                    fwrite(&totalSangria, sizeof(float), 1, arquivoCaixa);
                    fclose(arquivoCaixa);

                    printf("  DINHEIRO RETIRADO, SEU CAIXA AGORA ESTA COM R$ %.2f\n", (inicioCaixa+totalDin-totalSangria));
                    _getch();

                }
            }
        }
    }
}
//=================================================== FIM SANGRIA

//=================================================== PAGAMENTOS
void pagamento()
{
    //venda[numVendas].produtos[venda[numVendas].totalProdutos].quantidade = quantidade;
    float desc=0;
    clearScreen();

    printf("NUMVENDAS: %.d    \n", numVendas);

    //caso não haja compras no carrinho
    if(totalBruto<=0)
    {
        printf("\n|-----------------------------------------------|\n");
        printf("|  NENHUMA COMPRA REALIZADA, FAVOR ADICIONAR    |\n");
        printf("|  ITENS PARA PAGAMENTO.                        |\n");
        printf("|                                               |\n");
        printf("|  VOLTANDO PARA O MENU ANTERIOR...             |\n");
        printf("|-----------------------------------------------| \n\n");
        _getch();
        clearScreen();
    }
    else
    {
        //caso o carrinho tenha mais que R$ 0
        //forma de pagamento
        int pag=0;
        while (pag!=4)
        {

            printf("\n|---------------------------------------------------|\n");
            printf("|                     PAGAMENTO                     |\n");
            printf("|---------------------------------------------------|\n");
            printf("|  Escolha sua forma de pagamento (somente numero)  |\n");
            printf("|                                                   |\n");
            printf("|  1. Dinheiro/PIX                                  |\n");
            printf("|  2. Cartao de Debito/Credito                      |\n");
            printf("|  3. Pagamento Misto / Parcial                     |\n");
            printf("|  4. Voltar ao menu principal                      |\n");
            printf("|---------------------------------------------------|\n");
            printf("   TOTAL A PAGAR: R$ %.2f\n\n   ", totalBruto);
            scanf("%d", &pag);

            switch (pag)
            {

            case 1:
                clearScreen();
                printf("\n Digite a porcentagem de desconto (somente numero): ");
                scanf("%f", &desc);

                if ((desc<0)||(desc>100))
                {
                    printf("\n|------------------------------------------------------------|\n");
                    printf("|  NAO E POSSIVEL REALIZAR ESTE DESCONTO, TENTE NOVAMENTE.   |\n");
                    printf("|------------------------------------------------------------|\n\n");
                    _getch();
                    clearScreen();
                }
                if((desc>=0)||(desc<=100))
                {


                    desc = desc / 100;
                    desconto = totalBruto * desc;
                    desconto=truncar(desconto, 2);
                    totalDesc=totalBruto-desconto;
                    aPagar=totalDesc;

                    clearScreen();
                    printf("\n|------------------------------------------------|\n");
                    printf("|                   PAGAMENTO                    |\n");
                    printf("|------------------------------------------------|\n");
                    printf("|  Pagamento em dinheiro                         |\n");
                    printf("|                                                |\n");
                    printf("|  Digite a quantia em dinheiro entregue:        |\n");
                    printf("|------------------------------------------------|\n");
                    printf("|  TOTAL BRUTO...........: R$ %.2f\n", totalBruto);
                    printf("|  COMPRA COM DESCONTO DE: R$ %.2f\n", desconto);
                    printf("|  TOTAL A PAGAR.........: R$ %.2f\n", totalDesc);
                    printf("|------------------------------------------------|\n");
                    printf("|  R$ ");
                    scanf("%f", &pagoDin);

                    //pagou a mais, realizar troco
                    if (pagoDin>aPagar)
                    {
                        troco = pagoDin-aPagar;
                        printf("| - - - - - - - - - - - - - - - - - - - - - - - -|\n");
                        printf("|  TROCO A PAGAR: R$ %.2f    \n", troco);
                        printf("|------------------------------------------------|\n\n");
                        pagoDin=aPagar;
                        _getch();
                        clearScreen();
                    }

                    //pagamento bem sucedido
                    if (pagoDin==aPagar)
                    {
                        printf("\n|-------------------------------------------------|\n");
                        printf("|  PAGAMENTO REALIZADO COM SUCESSO,               |\n");
                        printf("|  VOLTANDO PARA O MENU PRINCIPAL...              |\n");
                        printf("|-------------------------------------------------| \n\n");
                        venda[numVendas-1].desconto =desconto;
                        totalDin+=pagoDin;
                    //    totalDin-=troco;
                        totalCartao+=pagoCartao;
                        vCaixa+=totalCartao+totalDin;
                        aPagar=0;
                        totalDesc=0;
                        totalBruto=0;
                        desconto=0;
                        desc=0;
                        pagoDin=0;
                        pagoCartao=0;
                        strncpy(venda[numVendas-1].metodoPagamento, "d", sizeof(venda[numVendas-1].metodoPagamento));

                        arquivoProduto = fopen("produtos.dat", "wb");
                        fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                        fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                        fclose(arquivoProduto);

                        arquivoVenda = fopen("vendas.dat", "wb");
                        fwrite(&numVendas, sizeof(int), 1, arquivoVenda);
                        for (int i = 0; i < numVendas; i++)
                        {
                            fwrite(&venda[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);
                            fwrite(venda[i].produtos, sizeof(struct ProdutoVenda), venda[i].totalProdutos, arquivoVenda);
                        }
                        fclose(arquivoVenda);

                        _getch();
                        clearScreen();
                    }
                    //pagou menos, volta e pede pra pagar misto
                    if (pagoDin<aPagar)
                    {
                        printf("\n|----------------------------------------------|\n");
                        printf("|  PAGAMENTO INSUFICIENTE, TENTE NOVAMENTE OU  |\n");
                        printf("|  REALIZE O PAGAMENTO PARCIAL...              |\n");
                        printf("|----------------------------------------------| \n\n");
                        _getch();
                        clearScreen();
                    }
                }
                break;

            //pagamento em cartao
            case 2:
                clearScreen();
                printf("\n|------------------------------------------------|\n");
                printf("|                   PAGAMENTO                    |\n");
                printf("|------------------------------------------------|\n");
                printf("|  Pagamento em Cartao de Credito/Debito         |\n");
                printf("|                                                |\n");
                printf("|  Digite a quantia em cartao:                   |\n");
                printf("|------------------------------------------------|\n");
                printf("|  TOTAL A PAGAR: R$ %.2f \n", totalBruto);
                printf("|------------------------------------------------|\n");
                printf("   R$ ");
                scanf("%f", &pagoCartao);

                //se pagou a quantia correta
                if (pagoCartao==totalBruto)
                {
                    printf("\n|-------------------------------------------------|\n");
                    printf("|  PAGAMENTO REALIZADO COM SUCESSO,               |\n");
                    printf("|  VOLTANDO PARA O MENU PRINCIPAL...              |\n");
                    printf("|-------------------------------------------------| \n\n");
                    totalDin+=pagoDin;
                    totalCartao+=pagoCartao;
                    vCaixa+=totalCartao+totalDin;
                    aPagar=0;
                    totalDesc=0;
                    totalBruto=0;
                    desconto=0;
                    desc=0;
                    pagoCartao=0;
                    pagoDin=0;
                //    venda[numVendas-1].metodoPagamento='c';
                //    strcpy(venda[numVendas-1].metodoPagamento,'c');
                //    strncpy(venda[numVendas].metodoPagamento, "c", sizeof("c") - 1);
                    strncpy(venda[numVendas-1].metodoPagamento, "c", sizeof(venda[numVendas-1].metodoPagamento));

                    arquivoProduto = fopen("produtos.dat", "wb");
                    fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                    fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                    fclose(arquivoProduto);

                    arquivoVenda = fopen("vendas.dat", "wb");
                    fwrite(&numVendas, sizeof(int), 1, arquivoVenda);
                    for (int i = 0; i < numVendas; i++)
                    {
                        fwrite(&venda[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);
                        fwrite(venda[i].produtos, sizeof(struct ProdutoVenda), venda[i].totalProdutos, arquivoVenda);
                    }
                    fclose(arquivoVenda);
                    
                    _getch();
                    clearScreen();
                }

                //pagou a mais, tenta pagar de novo pois cartao não tem troco
                if (pagoCartao>totalBruto)
                {
                    printf("\n|--------------------------------------------------------------|\n");
                    printf("|  PAGAMENTO MAIOR QUE O TOTAL A PAGAR, FAVOR TENTE            |\n");
                    printf("|  REALIZAR O PAGAMENTO NOVAMENTE...                           |\n");
                    printf("|--------------------------------------------------------------| \n\n");
                    _getch();
                    clearScreen();
                }

                //pagou a menos, pede para realizar pagamento parcial
                if (pagoCartao<totalBruto)
                {
                    printf("\n|----------------------------------------------|\n");
                    printf("|  PAGAMENTO INSUFICIENTE, TENTE REALIZAR O    |\n");
                    printf("|  REALIZE O PAGAMENTO PARCIAL...              |\n");
                    printf("|----------------------------------------------| \n\n");
                    _getch();
                    clearScreen();
                }
                break;

            //pagamento parcial
            case 3:
                do
                {
                    clearScreen();
                    printf("\n|---------------------------------------------------|\n");
                    printf("|                 PAGAMENTO PARCIAL                 |\n");
                    printf("|---------------------------------------------------|\n");
                    printf("|  Escolha sua forma de pagamento (somente numero)  |\n");
                    printf("|                                                   |\n");
                    printf("|  1. Dinheiro/PIX                                  |\n");
                    printf("|  2. Cartao de Debito/Credito                      |\n");
                    printf("|  3. Voltar ao menu principal                      |\n");
                    printf("|---------------------------------------------------|\n");
                    printf("   TOTAL A PAGAR: R$ %.2f\n\n   ", totalBruto);
                    scanf("%d", &pag);
                    switch (pag)
                    {

                    //pagamento parcial (dinheiro e cartao respectivamente)
                    case 1:
                        aPagar=totalBruto;
                        do
                        {
                            clearScreen();
                            printf("\n|------------------------------------------------|\n");
                            printf("|               PAGAMENTO PARCIAL                |\n");
                            printf("|------------------------------------------------|\n");
                            printf("|  Pagamento em dinheiro                         |\n");
                            printf("|                                                |\n");
                            printf("|  Digite a quantia em dinheiro entregue:        |\n");
                            printf("|------------------------------------------------|\n");
                            printf("|  TOTAL A PAGAR: R$ %.2f\n", aPagar);
                            printf("|------------------------------------------------|\n");
                            printf("   R$ ");
                            scanf("%f", &pagoDin);

                            //nao pode pagar o total pois escolheu a opcao de pagar parcial
                            if (pagoDin>=totalBruto)
                            {
                                printf("\n|--------------------------------------------------------------|\n");
                                printf("|  O PAGAMENTO PARCIAL NAO PODE SER MAIOR OU IGUAL AO          |\n");
                                printf("|  TOTAL, TENTE NOVAMENTE...                                   |\n");
                                printf("|--------------------------------------------------------------| \n\n");
                                _getch();
                                clearScreen();
                            }

                            else
                            {
                                //nao pode pagar menos do que 0 reais
                                if (pagoDin<0)
                                {
                                    printf("\n|------------------------------------------|\n");
                                    printf("|  PAGAMENTO IMPOSSIVEL, TENTE NOVAMENTE.  |\n");
                                    printf("|------------------------------------------| \n\n");
                                    _getch();
                                    clearScreen();
                                }

                                else
                                {
                                    //se o pagamento for maior que 0, porem menor que o total, realiza o pagamento parcial
                                    if ((pagoDin>=0)&&(pagoDin<totalBruto))
                                    {
                                        aPagar-=pagoDin;
                                        clearScreen();
                                        printf("\n|------------------------------------------------|\n");
                                        printf("|               PAGAMENTO PARCIAL                |\n");
                                        printf("|------------------------------------------------|\n");
                                        printf("|  Pagamento em cartao de credito/debito         |\n");
                                        printf("|                                                |\n");
                                        printf("|  Digite a quantia paga em cartao:              |\n");
                                        printf("|------------------------------------------------|\n");
                                        printf("|  TOTAL BRUTO: R$ %.2f\n", totalBruto);
                                        printf("|  QUANTIA PAGA EM DINHEIRO: R$ %.2f\n", pagoDin);
                                        printf("|  RESTANTE A PAGAR........: R$ %.2f\n", aPagar);
                                        printf("|------------------------------------------------|\n");
                                        printf("   R$ ");
                                        scanf("%f", &pagoCartao);

                                        //pagamento em cartao misto bem sucedido
                                        if (pagoCartao==aPagar)
                                        {
                                            printf("\n|-------------------------------------------------|\n");
                                            printf("|  PAGAMENTO REALIZADO COM SUCESSO,               |\n");
                                            printf("|  VOLTANDO PARA O MENU PRINCIPAL...              |\n");
                                            printf("|-------------------------------------------------| \n\n");
                                            totalDin+=pagoDin;
                                            totalCartao+=pagoCartao;
                                            vCaixa+=totalCartao+totalDin;
                                            aPagar=0;
                                            totalDesc=0;
                                            totalBruto=0;
                                            desconto=0;
                                            desc=0;
                                            pagoCartao=0;
                                    //        venda[numVendas-1].metodoPagamento='dc';
                                    //        strcpy(venda[numVendas-1].metodoPagamento,'dc');
                                    //        strncpy(venda[numVendas].metodoPagamento, "dc", sizeof("dc") - 1);
                                            strncpy(venda[numVendas-1].metodoPagamento, "dc", sizeof(venda[numVendas-1].metodoPagamento));

                                            arquivoProduto = fopen("produtos.dat", "wb");
                                            fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                                            fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                                            fclose(arquivoProduto);

                                            arquivoVenda = fopen("vendas.dat", "wb");
                                            fwrite(&numVendas, sizeof(int), 1, arquivoVenda);
                                            for (int i = 0; i < numVendas; i++)
                                            {
                                                fwrite(&venda[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);
                                                fwrite(venda[i].produtos, sizeof(struct ProdutoVenda), venda[i].totalProdutos, arquivoVenda);
                                            }
                                            fclose(arquivoVenda);

                                            _getch();
                                            clearScreen();
                                            voltar(); 
                                        }
                                        //pagou a menos no restante do pagamento parcial, tente novamente
                                        if (pagoCartao<aPagar)
                                        {
                                            printf("\n|--------------------------------------------|\n");
                                            printf("|  PAGAMENTO INSUFICIENTE, TENTE NOVAMENTE.  |\n");
                                            printf("|--------------------------------------------| \n\n");
                                            _getch();
                                            clearScreen();
                                        }
                                        //pagou a mais no restante do pagamento em cartao, tente novamente pois nao tem troco
                                        if (pagoCartao>aPagar)
                                        {
                                            printf("\n|--------------------------------------------------------------------------|\n");
                                            printf("|  O PAGAMENTO EM CARTAO NAO PODE SER MAIOR QUE O TOTAL, TENTE NOVAMENTE.  |\n");
                                            printf("|--------------------------------------------------------------------------| \n\n");
                                            _getch();
                                            clearScreen();
                                        }
                                    }
                                }
                            }
                        }
                        while(aPagar!=0);
                        break;

                    //pagamento parcial (cartao e dinheiro respectivamente)
                    case 2:
                        aPagar=totalBruto;
                        do
                        {
                            clearScreen();
                            printf("\n|------------------------------------------------|\n");
                            printf("|               PAGAMENTO PARCIAL                |\n");
                            printf("|------------------------------------------------|\n");
                            printf("|  Pagamento em Cartao de Credito/Debito:        |\n");
                            printf("|                                                |\n");
                            printf("|  Digite a quantia em dinheiro entregue:        |\n");
                            printf("|------------------------------------------------|\n");
                            printf("|  TOTAL A PAGAR: R$ %.2f\n", aPagar);
                            printf("|------------------------------------------------|\n");
                            printf("   R$ ");
                            scanf("%f", &pagoCartao);

                            //nao pode pagar o total pois escolheu pagar parcial
                            if (pagoCartao>=totalBruto)
                            {
                                printf("\n|--------------------------------------------------------------|\n");
                                printf("|  O PAGAMENTO PARCIAL NAO PODE SER MAIOR OU IGUAL AO          |\n");
                                printf("|  TOTAL, TENTE NOVAMENTE...                                   |\n");
                                printf("|--------------------------------------------------------------| \n\n");
                                _getch();
                                clearScreen();
                            }

                            else
                            {
                                //nao pode pagar menos que 0 reais
                                if (pagoCartao<0)
                                {
                                    printf("\n|------------------------------------------|\n");
                                    printf("|  PAGAMENTO IMPOSSIVEL, TENTE NOVAMENTE.  |\n");
                                    printf("|------------------------------------------| \n\n");
                                    _getch();
                                    clearScreen();
                                }

                                else
                                {
                                    //se o pagamento for maior que 0, porem menor que o total, realiza o pagamento parcial em cartao
                                    if ((pagoCartao>=0)&&(pagoCartao<totalBruto))
                                    {
                                        aPagar-=pagoCartao;
                                        clearScreen();
                                        printf("\n|------------------------------------------------|\n");
                                        printf("|               PAGAMENTO PARCIAL                |\n");
                                        printf("|------------------------------------------------|\n");
                                        printf("|  Pagamento em dinheiro                         |\n");
                                        printf("|                                                |\n");
                                        printf("|  Digite a quantia de dinheiro entregue:        |\n");
                                        printf("|------------------------------------------------|\n");
                                        printf("|  TOTAL BRUTO...........: R$ %.2f\n", totalBruto);
                                        printf("|  QUANTIA PAGA EM CARTAO: R$ %.2f\n", pagoCartao);
                                        printf("|  RESTANTE A PAGAR......: R$ %.2f\n", aPagar);
                                        printf("|------------------------------------------------|\n");
                                        printf("|  R$ ");
                                        scanf("%f", &pagoDin);

                                        //pagou a mais no restante em dinheiro, pagamento bem sucedido com troco
                                        if (pagoDin>aPagar)
                                        {
                                            troco = pagoDin-aPagar;
                                            printf("| - - - - - - - - - - - - - - - - - - - - - - - -|\n");
                                            printf("| TROCO A PAGAR: R$ %.2f    \n", troco);
                                            printf("|------------------------------------------------|\n\n");
                                            pagoDin=aPagar;
                                            _getch();
                                            clearScreen();
                                        }

                                        //se pagar o total restante em dinheiro, pagamento bem sucedido
                                        if (pagoDin==aPagar)
                                        {
                                            printf("\n|-------------------------------------------------|\n");
                                            printf("|  PAGAMENTO REALIZADO COM SUCESSO,               |\n");
                                            printf("|  VOLTANDO PARA O MENU PRINCIPAL...              |\n");
                                            printf("|-------------------------------------------------| \n\n");
                                            totalDin+=pagoDin;
                                            totalDin-=troco;
                                            totalCartao+=pagoCartao;
                                            vCaixa+=totalCartao+totalDin;
                                            aPagar=0;
                                            totalDesc=0;
                                            totalBruto=0;
                                            desconto=0;
                                            desc=0;
                                            pagoCartao=0;
                                     //       venda[numVendas].metodoPagamento='cd';
                                     //       strcpy(venda[numVendas-1].metodoPagamento,'cd');
                                     //       strncpy(venda[numVendas].metodoPagamento, "cd", sizeof("cd") - 1);
                                            strncpy(venda[numVendas-1].metodoPagamento, "cd", sizeof(venda[numVendas-1].metodoPagamento));
                                           
                                            arquivoProduto = fopen("produtos.dat", "wb");
                                            fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                                            fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                                            fclose(arquivoProduto);

                                            arquivoVenda = fopen("vendas.dat", "wb");
                                            fwrite(&numVendas, sizeof(int), 1, arquivoVenda);
                                            for (int i = 0; i < numVendas; i++)
                                            {
                                                fwrite(&venda[i], sizeof(struct CadastroVenda) - sizeof(struct ProdutoVenda *), 1, arquivoVenda);
                                                fwrite(venda[i].produtos, sizeof(struct ProdutoVenda), venda[i].totalProdutos, arquivoVenda);
                                            }
                                            fclose(arquivoVenda);
                                    
                                     //       produto[indProduto].estoque-=quantidade;
                                            _getch();
                                            clearScreen();
                                        }

                                        //nao pode pagar a menos no restante do pagamento parcial em dinheiro
                                        if (pagoDin<aPagar)
                                        {
                                            printf("\n|--------------------------------------------|\n");
                                            printf("|  PAGAMENTO INSUFICIENTE, TENTE NOVAMENTE.  |\n");
                                            printf("|--------------------------------------------| \n\n");
                                            _getch();
                                            clearScreen();
                                        }
                                    }
                                }
                            }
                        }
                        while(aPagar!=0);
                        break;

                    case 3:
                        continue;

                    default:
                        opinvalida();

                        break;
                    }
                }
                while (pag!=3);

            case 4:
                voltar();
                break;

            default:
                opinvalida();
            }
            break;
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
                fwrite(&totalDin, sizeof(float), 1, arquivoCaixa);
                fwrite(&totalCartao, sizeof(float), 1, arquivoCaixa);
                fwrite(&totalSangria, sizeof(float), 1, arquivoCaixa);
                fclose(arquivoCaixa);

//                printf("Caixa Gravado\n");
//                _getch();

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
    _getch();
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
        _getch();
        clearScreen();
    }

    //fechar caixa
    else
    {
        clearScreen();
        totalFat=totalDin+totalCartao;
        printf("\n|--------------------------------------------------|\n");
        printf("|               FECHAMENTO DE CAIXA                |\n");
        printf("|--------------------------------------------------|\n");
        printf("  Vendas Realizadas: %d                                \n", numVendas);
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - -|\n");
        printf("  VALOR DE ABERTURA: R$ %.2f \n", inicioCaixa);
        printf("  TOTAL PAGO EM DINHEIRO: R$ %.2f \n", totalDin);
        printf("  TOTAL PAGO EM CARTAO: R$ %.2f \n", totalCartao);
        printf("  TOTAL FATURADO: R$ %.2f \n", totalFat);
        printf("  SANGRIA: R$ %.2f \n", totalSangria);
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
                totalFat=0;
                totalSangria=0;

                arquivoCaixa = fopen("caixa.dat", "wb");
                fwrite(&caixa, sizeof(char), 1, arquivoCaixa);
                fwrite(&inicioCaixa, sizeof(float), 1, arquivoCaixa);
                fwrite(&totalDin, sizeof(float), 1, arquivoCaixa);
                fwrite(&totalCartao, sizeof(float), 1, arquivoCaixa);
                fwrite(&totalSangria, sizeof(float), 1, arquivoCaixa);
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
    _getch();
    clearScreen();
}
//=================================================== FIM FECHAMENTO DE CAIXA

//=================================================== MENU DE RELATORIOS
void menuRelatorios()
{

    int relatorio=0;
    while (relatorio != 5)
    {
        clearScreen();
        printf("\n|---------------------------------------------|\n");
        printf("|                                             |\n");
        printf("|           | |   Relatorios   | |            |\n");
        printf("|                                             |\n");
        printf("|     1. Listagem de clientes                 |\n");
        printf("|     2. Listagem de produtos                 |\n");
        printf("|     3. Listagem de categorias               |\n");
        printf("|     4. Listagem de vendas                   |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     5. Retornar ao menu principal           |\n");
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

        //listagem de categorias criadas
        case 3:
            listaCategorias();
            break;

        //listagem de vendas
        case 4:
            listaVendas();
            break;

        //voltar para menu principal
        case 5:
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

void listarComprasPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim)
{
    struct
    {
        int cliente;
        char nomeCliente[30];
        float totalCompras;
    } totalPorCliente[100]; // Assumindo um máximo de 100 clientes únicos
    int numClientes = 0;

    for (int i = 0; i < numVendas; i++)
    {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim))
        {
            int clienteIndex = -1;
            for (int j = 0; j < numClientes; j++)
            {
                if (totalPorCliente[j].cliente == vendas[i].cliente)
                {
                    clienteIndex = j;
                    break;
                }
            }

            if (clienteIndex == -1)
            {
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
    for (int i = 0; i < numClientes; i++)
    {
        printf("Codigo: %d Cliente: %s Total Compras: %.2f\n",
               totalPorCliente[i].cliente, totalPorCliente[i].nomeCliente, totalPorCliente[i].totalCompras);
    }
}

// Função para listar os clientes que mais compraram por período
void listarClientesMaisCompraramPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim)
{
    struct
    {
        int cliente;
        char nomeCliente[30];
        float valorTotalCompras;
    } totalPorCliente[100]; // Assumindo um máximo de 100 clientes diferentes
    int numClientes = 0;

    // Inicializar totalPorCliente
    for (int i = 0; i < 100; i++)
    {
        totalPorCliente[i].cliente = 0;
        strcpy(totalPorCliente[i].nomeCliente, "");
        totalPorCliente[i].valorTotalCompras = 0.0;
    }

    // Calcular valor total de compras por cliente no período
    for (int i = 0; i < numVendas; i++)
    {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim))
        {
            int clienteIndex = -1;
            // Procurar cliente na lista totalPorCliente
            for (int j = 0; j < numClientes; j++)
            {
                if (totalPorCliente[j].cliente == vendas[i].cliente)
                {
                    clienteIndex = j;
                    break;
                }
            }
            // Se cliente não estiver na lista, adicioná-lo
            if (clienteIndex == -1)
            {
                clienteIndex = numClientes++;
                totalPorCliente[clienteIndex].cliente = vendas[i].cliente;
                strcpy(totalPorCliente[clienteIndex].nomeCliente, vendas[i].nomeCliente);
            }
            // Adicionar valor total da venda ao cliente correspondente
            totalPorCliente[clienteIndex].valorTotalCompras += vendas[i].valorTotal;
        }
    }

    // Ordenar clientes por valor total de compras em ordem decrescente
    for (int i = 0; i < numClientes - 1; i++)
    {
        for (int j = i + 1; j < numClientes; j++)
        {
            if (totalPorCliente[i].valorTotalCompras < totalPorCliente[j].valorTotalCompras)
            {
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
    for (int i = 0; i < numClientes; i++)
    {
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
            qsort(cliente, numClientes, sizeof(struct CadastroCliente), compararCodCliente);
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
            _getch();
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
            _getch();
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

// Função para listar os itens mais vendidos por período
void listarItensMaisVendidosPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim)
{
    struct
    {
        int cod;
        char descricao[30];
        int quantidadeVendida;
    } totalPorProduto[100]; // Assumindo um máximo de 100 produtos únicos
    int numProdutos = 0;

    for (int i = 0; i < numVendas; i++)
    {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim))
        {
            for (int j = 0; j < vendas[i].totalProdutos; j++)
            {
                int produtoIndex = -1;
                for (int k = 0; k < numProdutos; k++)
                {
                    if (totalPorProduto[k].cod == vendas[i].produtos[j].cod)
                    {
                        produtoIndex = k;
                        break;
                    }
                }

                if (produtoIndex == -1)
                {
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
    for (int i = 0; i < numProdutos - 1; i++)
    {
        for (int j = i + 1; j < numProdutos; j++)
        {
            if (totalPorProduto[i].quantidadeVendida < totalPorProduto[j].quantidadeVendida)
            {
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
    for (int i = 0; i < numProdutos; i++)
    {
        printf("Codigo: %d Produto: %s Quantidade Vendida: %d\n",
               totalPorProduto[i].cod, totalPorProduto[i].descricao, totalPorProduto[i].quantidadeVendida);
    }
}

//=================================================== RELATÓRIOS DE PRODUTOS
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
            qsort(produto, numProdutos, sizeof(struct CadastroProduto), compararCodProduto);
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
            _getch();
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
            _getch();
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
            _getch();
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

//=================================================== RELATÓRIO DE CATEGORIAS
listaCategorias()
{
    int lista = 0;
    while(lista!=3)
    {
        clearScreen();
        printf("\n|---------------------------------------------------|\n");
        printf("|        | |   Relatorio de Categorias   | |        |\n");
        printf("|                                                   |\n");
        printf("|  1. Ordem de Codigo                               |\n");
        printf("|  2. Ordem Alfabetica (A-Z)                        |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|  3. Voltar para menu de relatorios                |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|     Escolha uma das opcoes acima:                 |\n");
        printf("|     (Somente numero)                              |\n");
        printf("|___________________________________________________|\n\n");
        scanf("%d", &lista);

        switch (lista)
        {
        case 1:

            clearScreen();
            printf("\n|------------------------------------------------------------|");
            printf("\n|             CATEGORIAS EM ORDEM DE CODIGO                  |\n");
            qsort(categoria, numcategorias, sizeof(struct CadastroCategoria), compararCodCategoria);

            for (int i = 0; i < numcategorias; i++)
            {
                printf("|------------------------------------------------------------|\n");
                printf("|  Codigo...........: %d\n", categoria[i].cod);
                printf("|  Nome da Categoria: %s\n", categoria[i].descricao);
            }
            printf("|------------------------------------------------------------|\n");
            printf("|- - - - - - - - FIM DA LISTA DE CATEGORIAS - - - - - - - - -|\n\n");

            _getch();
            break;

        case 2:
            clearScreen();
            printf("\n|------------------------------------------------------------|");
            printf("\n|            CATEGORIAS EM ORDEM ALFABETICA                  |\n");
            qsort(categoria, numcategorias, sizeof(struct CadastroCategoria), compararCategoria);
            for (int i = 0; i < numcategorias; i++)
            {
                printf("|------------------------------------------------------------|\n");
                printf("|  Codigo...........: %d\n", categoria[i].cod);
                printf("|  Nome da Categoria: %s\n", categoria[i].descricao);
            }
            printf("|------------------------------------------------------------|\n");
            printf("|- - - - - - - - FIM DA LISTA DE CATEGORIAS - - - - - - - - -|\n\n");

            _getch();
            break;

        case 3:
            voltar();
            break;
        }
    }
}
//=================================================== FIM RELATÓRIO DE CATEGORIAS

//=================================================== RELATORIO DE VENDAS

// Função para listar as vendas por período
void listarVendasPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim)
{
    printf("Listagem de vendas no período de %d/%d/%d a %d/%d/%d:\n",
           inicio.dia, inicio.mes, inicio.ano, fim.dia, fim.mes, fim.ano);

    for (int i = 0; i < numVendas; i++)
    {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim))
        {
            printf("--------------------------------------------------\n");
            printf("Número da Venda: %d\n", vendas[i].num);
            printf("Cliente: %d - %s\n", vendas[i].cliente, vendas[i].nomeCliente);
            printf("Data da Venda: %d/%d/%d\n", vendas[i].data.dia, vendas[i].data.mes, vendas[i].data.ano);
            printf("Valor Total: %.2f\n", vendas[i].valorTotal);
            printf("Método de Pagamento: %s\n", vendas[i].metodoPagamento);

            printf("Produtos:\n");
            for (int j = 0; j < vendas[i].totalProdutos; j++)
            {
                printf("   Codigo: %d, Descricao: %s, Quantidade: %d, Preço Unitario: %.2f, Valor Total: %.2f\n",
                       vendas[i].produtos[j].cod, vendas[i].produtos[j].descricao,
                       vendas[i].produtos[j].quantidade, vendas[i].produtos[j].precoVenda,
                       vendas[i].produtos[j].valorTotal);
            }
        }
    }
    printf("--------------------------------------------------\n");
}

void listarFaturamentoPorPeriodo(struct CadastroVenda *vendas, int numVendas, struct Data inicio, struct Data fim)
{
    float dinheiro=0,cartao=0,dincar=0,cardin=0;
    printf("Listagem de faturamento no periodo de %d/%d/%d a %d/%d/%d:\n",
           inicio.dia, inicio.mes, inicio.ano, fim.dia, fim.mes, fim.ano);

    for (int i = 0; i < numVendas; i++)
    {
        if (dataDentroDoPeriodo(vendas[i].data, inicio, fim))
        {
            if (strcmp(venda[i].metodoPagamento, "d") == 0) {
                dinheiro+=vendas[i].valorTotal-vendas[i].desconto;
            }
            if (strcmp(venda[i].metodoPagamento, "c") == 0) {
                cartao+=vendas[i].valorTotal;
            }
            if (strcmp(venda[i].metodoPagamento, "cd") == 0) {
                cardin+=vendas[i].valorTotal;
            }
            if (strcmp(venda[i].metodoPagamento, "dc") == 0) {
                dincar+=vendas[i].valorTotal;
            }
      }
    }
    printf("--------------------------------------------------\n");
    printf("Dinheiro.........: %.2f\n", dinheiro);
    printf("Cartao...........: %.2f\n", cartao);
    printf("Cartao e Dinheiro: %.2f\n", cardin);
    printf("Dinheiro e Cartao: %.2f\n", dincar);
    printf("--------------------------------------------------\n");
}

void listaVendas()
{
    int lista = 0;
    int i;
    int j;
    struct Data inicio;
    struct Data fim;

    while(lista!=3)
    {
        clearScreen();
        printf("\n|---------------------------------------------------|\n");
        printf("|          | |   Relatorio de Vendas   | |          |\n");
        printf("|                                                   |\n");
        printf("|  1. Ordem de Codigo                               |\n");
        printf("|  2. Periodo de Venda                              |\n");
        printf("|  3. Faturamento Consolidado                       |\n");
        printf("| - - - - - - - - - - - - - - - - - - - - - - - - - |\n");
        printf("|  4. Voltar para menu de relatorios                |\n");
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
                printf("|  Total bruto....: %.2f\n", venda[i].valorTotal);
                printf("|  Valor desconto.: %.2f\n", venda[i].desconto);
                printf("|  Total Pago.....: %.2f\n", venda[i].valorTotal-venda[i].desconto);
                printf("|  Qtd Itens......: %d\n", venda[i].totalProdutos);
                printf("|  Pago em........: %s\n", venda[i].metodoPagamento);
                printf("| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -|\n");

                for (int j = 0; j < venda[i].totalProdutos; j++)
                {
                    printf("|  Codigo.........: %d\n", venda[i].produtos[j].cod);
                    printf("|  Descricao......: %s\n", venda[i].produtos[j].descricao);
                    printf("|  Quantidade.....: %d\n", venda[i].produtos[j].quantidade);
                    printf("|  Preco unitario.: %.2f\n", venda[i].produtos[j].precoVenda);
                    printf("|  Total do item..: %.2f\n", venda[i].produtos[j].valorTotal);
                }
            }
            printf("|------------------------------------------------------------|\n");
            printf("|- - - - - - - - - FIM DA LISTA DE VENDAS - - - - - - - - - -|\n\n");

            _getch();
            break;

        //vendas em determinado periodo
        case 2:
            // Lendo o período
            printf("Digite a data de inicio do periodo:\n");
            inicio = lerData();
            printf("Digite a data de fim do periodo:\n");
            fim = lerData();
            listarVendasPorPeriodo(venda, numVendas, inicio, fim);

        case 3:
            printf("Digite a data de inicio do periodo:\n");
            inicio = lerData();
            printf("Digite a data de fim do periodo:\n");
            fim = lerData();
            listarFaturamentoPorPeriodo(venda, numVendas, inicio, fim);
       
        //voltar para menu de vendas
        case 4:
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
                    arquivoCaixa = fopen("caixa.dat", "wb");
                    fwrite(&caixa, sizeof(char), 1, arquivoCaixa);
                    fwrite(&inicioCaixa, sizeof(float), 1, arquivoCaixa);
                    fwrite(&totalDin, sizeof(float), 1, arquivoCaixa);
                    fwrite(&totalCartao, sizeof(float), 1, arquivoCaixa);
                    fwrite(&totalSangria, sizeof(float), 1, arquivoCaixa);
                    fclose(arquivoCaixa);

                    arquivoProduto = fopen("produtos.dat", "wb");
                    fwrite(&numProdutos, sizeof(int), 1, arquivoProduto);
                    fwrite(produto, sizeof(struct CadastroProduto), numProdutos, arquivoProduto);
                    fclose(arquivoProduto);

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
    _getch();
    clearScreen();
}

// PRINT DE VOLTAR PARA MENU ANTERIOR
void voltar()
{
    printf("\n|------------------------------------|\n");
    printf("|  VOLTANDO PARA O MENU ANTERIOR...  |\n");
    printf("|------------------------------------|\n\n");
    _getch();
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
    venda->desconto = 0.0f;
    venda->totalProdutos = 0;
    venda->produtos = NULL;
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

        if (numusuarios==0)
        {
            printf("\n|----------------------------------------------------|\n");
            printf("|  Nao existem usuarios cadastrados, redirecionando  |\n");
            printf("|  para o cadastro de usuarios...                    |\n");
            printf("|----------------------------------------------------|\n");

            _getch();

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
            if (strcmp(nomeUsuario, " ") == 0)
            {
                printf("\n|--------------------------------------------------------|\n");
                printf("|  Usuario e/ou senha nao cadastrados. Tente novamente.  |\n");
                printf("|--------------------------------------------------------|\n");
                _getch();
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
                if (tipoUsuario=='1')
                {
                    aberturaCaixa();
                }
                else
                {
                    printf("\n|------------------------------------|\n");
                    printf("|  Usuario sem permissao de acesso.  |\n");
                    printf("|------------------------------------|\n");
                    _getch();
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
                if (tipoUsuario=='1')
                {
                    fechamentoCaixa();
                }
                else
                {
                    printf("\n|------------------------------------|\n");
                    printf("|  Usuario sem permissao de acesso.  |\n");
                    printf("|------------------------------------|\n");
                    _getch();
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

    // Liberação de memória alocada
    free(venda[numVendas].produtos);
    free(venda);

}
