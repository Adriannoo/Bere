#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define ARQUIVO_LOG "relatorios.log"

// Estrutura para armazenar a venda atual
typedef struct {
    int codigoProduto;
    char descricao[100];
    float precoUnitario;
    int quantidade;
    float subtotal;
} ItemVenda;

//=== ESTRUTURAS DE DADOS ===//
typedef struct {
    int codigo;
    char nomeCompleto[100];
    char nomeSocial[100];
    char cpf[15];
    char endereco[100];
    char bairro[50];
    char celular[20];
} Cliente;

typedef struct {
    int codigo;
    char nomeProduto[100];
    char *categoriaProduto;
    float precoCompra;
    float percentual;
    float precoVenda;
    int estoque;
    int estoqueMinimo;
} Produto;

typedef struct {
    char **categorias;
    int quantidade;
} ListaCategorias;

typedef struct {
    Produto *produtos;
    int quantidade;
} SistemaProdutos;

SistemaProdutos sistemaProdutos = {NULL, 0};

// ===== VARIÁVEIS GLOBAIS ===== //
ListaCategorias categoriasGlobais = {NULL, 0};
float totalCaixa = 0;
float totalVenda = 0;
int numeroVenda = 0;
float totalVendas = 0;
int numItens = 0;
int quantidadeProdutos = 0;
int opcaoMenu;
int caixaAberto = 0;
int proximoNumeroVenda = 1;
float vAbre = 0;
int vFechamento = 0;
float totalDinheiroF = 0, totalCartaoF = 0, totalDinheiroCartaoF = 0;

// ===== PROTÓTIPOS DE FUNÇÕES ===== //
float retiradaCaixa(float *totalCaixa);
void menu_principal(void);
void exibirMenu(void);
void salvarClientes(const Cliente *clientes, int quantidade);
void carregarClientes(Cliente **clientes, int *quantidade);
void menuCadastro(void);
void cadastrarCliente(Cliente **clientes, int *quantidadeClientes);
void cadastrarProduto(Produto **produtos, int *quantidadeProdutos);
void menuVendas(void);
void menuAberturaCaixa(void);
void menuFechamentoCaixa(void);
void menuRelatorios(void);
void processarPagamentoCartao(float *totalCartao, float totalOriginal);
void processarPagamentoDinheiro(float *totalDinheiro, float *totalVenda, float totalOriginal);
void finalizarVenda(float totalOriginal, float dinheiro, float cartao);
void menu_pagamento(float totalVenda);
void menu_novaVenda(void);
void exibirRelatorioVendas(void);
void exibirProdutosCadastrados(void);
void exibirClientesCadastrados(Cliente *clientes, int quantidadeClientes);
void registrarLog(const char *mensagem);
void exibirLogs();
void verificarEstoqueMinimo();
void exibirCategorias();
void liberarCategorias();
void salvarCategorias();
void salvarProdutos();
void carregarProdutos();
void carregarCategorias();
bool adicionarCategoria(const char *categoria);
bool categoriaExiste(const char *categoria);

// ===== FUNÇÕES AUXILIARES ===== //

bool validarCPF(const char *cpf) { // Remove caracteres não numéricos
    char cpfLimpo[12]; // 11 dígitos + o Nulo
    int j = 0;
    for (int i = 0; cpf[i] != '\0' && j < 11; i++) {
        if (isdigit(cpf[i])) {
            cpfLimpo[j++] = cpf[i];
        }
    }
    cpfLimpo[j] = '\0';

    // Verificação se tem 11 dígitos
    if (j != 11) {
        return false;
    }

    // Verificação se todos os dígitos são iguais (ex: 11111111111)
    bool todosIguais = true;
    for (int i = 1; i < 11; i++) {
        if (cpfLimpo[i] != cpfLimpo[0]) {
            todosIguais = false;
            break;
        }
    }
    if (todosIguais) {
        return false;
    }

    return true;
}

double truncarValor(double const valor, int const casasDecimais) {
    // Verifica se o número de casas decimais é válido
    if (casasDecimais < 0) {
        return valor; // Retorna o valor original se casas decimais for negativo
    }
    // Calcula o fator de multiplicação/divisão
    const double fator = pow(10.0, casasDecimais);
    // Trunca o valor
    return trunc(valor * fator) / fator;
}

// ===== FUNÇÕES DE MENU ===== //
void exibirMenu() {
    system("cls");
    printf("|=======================================================|\n");
    printf("|\t\t    Mercado Dona Bere\t\t\t|\n");
    printf("|=======================================================|\n");
    printf("|\t ------------ MENU PRINCIPAL ------------\t|\n");
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("| -> 1 - CADASTROS \t\t\t\t\t|\n");
    printf("| -> 2 - VENDAS \t\t\t\t\t|\n");
    printf("|.......................................................|\n");
    printf("| -> 3 - ABERTURA DE CAIXA \t\t\t\t|\n");
    printf("| -> 4 - FECHAMENTO DE CAIXA \t\t\t\t|\n");
    printf("|.......................................................|\n");
    printf("| -> 5 - RELATORIOS \t\t\t\t\t|\n");
    printf("| -> 6 - SAIR \t\t\t\t\t\t|\n");
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("| TOTAL DE VENDAS HOJE: R$ %.2f \t\t\t|\n", totalVendas);
    printf("|=======================================================|\n");
    printf("Opcao: ");
}

void menu_principal() {
    int opcaoMenu;

    // Carrega dados ao iniciar
    carregarCategorias();
    carregarProdutos();

    // Variáveis locais para clientes
    Cliente *clientes = NULL;
    int quantidadeClientes = 0;
    carregarClientes(&clientes, &quantidadeClientes);

    do {
        exibirMenu();
        scanf("%d", &opcaoMenu);
        getchar();

        switch (opcaoMenu) {
            case 1:  // CADASTROS
                system("cls");
                menuCadastro();
                break;

            case 2:  // VENDAS
                if (caixaAberto) {
                    menuVendas();
                } else {
                    printf("Caixa fechado. Efetue a abertura do caixa primeiro.\n");
                    system("pause");
                }
                break;

            case 3:  // ABERTURA DE CAIXA
                if (!caixaAberto) {
                    menuAberturaCaixa();
                } else {
                    printf("Caixa ja esta aberto.\n");
                    system("pause");
                }
                break;

            case 4:  // FECHAMENTO DE CAIXA
                if (caixaAberto) {
                    menuFechamentoCaixa();
                } else {
                    printf("Caixa ja esta fechado.\n");
                    system("pause");
                }
                break;

            case 5:  // RELATÓRIOS
                menuRelatorios();
                break;

            case 6:  // SAIR
                if (caixaAberto) {
                    printf("Caixa ainda aberto. Realize o fechamento primeiro.\n");
                    system("pause");
                } else {
                    // Salva dados antes de sair
                    salvarProdutos();
                    salvarClientes(clientes, quantidadeClientes);
                    salvarCategorias();

                    // Libera memória
                    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
                        free(sistemaProdutos.produtos[i].categoriaProduto);
                    }
                    free(sistemaProdutos.produtos);
                    free(clientes);
                    liberarCategorias();

                    printf("Saindo do sistema...\n");
                    system("pause");
                    exit(0);
                }
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                system("pause");
        }
    } while (opcaoMenu != 6);
}

void salvarClientes(const Cliente *clientes, int quantidade) {
    FILE *arquivo = fopen("clientes.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de clientes para escrita!\n");
        return;
    }

    // Escreve a quantidade de clientes
    fwrite(&quantidade, sizeof(int), 1, arquivo);

    // Escreve cada cliente
    for (int i = 0; i < quantidade; i++) {
        fwrite(&clientes[i], sizeof(Cliente), 1, arquivo);
    }

    fclose(arquivo);
}

void carregarClientes(Cliente **clientes, int *quantidade) {
    FILE *arquivo = fopen("clientes.dat", "rb");
    if (arquivo == NULL) {
        *clientes = NULL;
        *quantidade = 0;
        return;
    }

    // Lê a quantidade de clientes
    fread(quantidade, sizeof(int), 1, arquivo);

    // Aloca memória para os clientes
    *clientes = (Cliente*)malloc(*quantidade * sizeof(Cliente));
    if (*clientes == NULL) {
        printf("Erro ao alocar memoria para clientes!\n");
        fclose(arquivo);
        *quantidade = 0;
        return;
    }

    // Lê cada cliente
    for (int i = 0; i < *quantidade; i++) {
        fread(&(*clientes)[i], sizeof(Cliente), 1, arquivo);
    }

    fclose(arquivo);
}

void menuCadastro() {
    static Cliente *clientes = NULL;
    static int quantidadeClientes = 0;
    Produto *produtos = sistemaProdutos.produtos;
    int quantidadeProdutos = sistemaProdutos.quantidade;

    opcaoMenu = 0;
    while (opcaoMenu != 3) {
        system("cls");
        printf("|=======================================================|\n");
        printf("|\t\t    MENU CADASTROS\t\t\t|\n");
        printf("|=======================================================|\n");
        printf("| -> 1 - CADASTRAR CLIENTE \t\t\t\t|\n");
        printf("| -> 2 - CADASTRAR PRODUTO \t\t\t\t|\n");
        printf("| -> 3 - VOLTAR AO MENU PRINCIPAL \t\t\t|\n");
        printf("|=======================================================|\n");
        printf("Opcao: ");

        scanf("%d", &opcaoMenu);
        getchar();

        switch (opcaoMenu) {
            case 1:
                cadastrarCliente(&clientes, &quantidadeClientes);
                break;
            case 2:
                cadastrarProduto(&produtos, &quantidadeProdutos);
                break;
            case 3:
                return;
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }
    }
}

void cadastrarCliente(Cliente **clientes, int *quantidadeClientes) {
    // Realoca memória para o novo cliente
    Cliente *temp = realloc(*clientes, (*quantidadeClientes + 1) * sizeof(Cliente));
    if (temp == NULL) {
        printf("Erro ao alocar memoria para novo cliente!\n");
        return;
    }
    *clientes = temp;

    // Obtém referência para o novo cliente
    Cliente *novoCliente = &((*clientes)[*quantidadeClientes]);
    memset(novoCliente, 0, sizeof(Cliente)); // Inicializa a estrutura

    // Atribui código ao novo cliente
    novoCliente->codigo = *quantidadeClientes + 1;

    printf("\n=== CADASTRO DE CLIENTE ===\n");

    // Nome Completo
    printf("Nome Completo: ");
    fgets(novoCliente->nomeCompleto, sizeof(novoCliente->nomeCompleto), stdin);
    novoCliente->nomeCompleto[strcspn(novoCliente->nomeCompleto, "\n")] = '\0';

    // Nome Social (opcional)
    printf("Nome Social (opcional): ");
    fgets(novoCliente->nomeSocial, sizeof(novoCliente->nomeSocial), stdin);
    novoCliente->nomeSocial[strcspn(novoCliente->nomeSocial, "\n")] = '\0';

    // CPF com validação
    bool cpfValido = false;
    while (!cpfValido) {
        printf("CPF: ");
        fgets(novoCliente->cpf, sizeof(novoCliente->cpf), stdin);
        novoCliente->cpf[strcspn(novoCliente->cpf, "\n")] = '\0';

        cpfValido = validarCPF(novoCliente->cpf);
        if (!cpfValido) {
            printf("\nCPF INVALIDO, DIGITE NOVAMENTE!\n");
        }
    }

    // Endereço
    printf("Endereco: ");
    fgets(novoCliente->endereco, sizeof(novoCliente->endereco), stdin);
    novoCliente->endereco[strcspn(novoCliente->endereco, "\n")] = '\0';

    // Bairro
    printf("Bairro: ");
    fgets(novoCliente->bairro, sizeof(novoCliente->bairro), stdin);
    novoCliente->bairro[strcspn(novoCliente->bairro, "\n")] = '\0';

    // Celular
    printf("Celular: ");
    fgets(novoCliente->celular, sizeof(novoCliente->celular), stdin);
    novoCliente->celular[strcspn(novoCliente->celular, "\n")] = '\0';

    // Atualiza a quantidade de clientes
    (*quantidadeClientes)++;

    // Persistência de dados
    salvarClientes(*clientes, *quantidadeClientes);

    printf("\nCliente cadastrado com sucesso! Codigo: %d\n", novoCliente->codigo);
    system("pause");
}

bool adicionarCategoria(const char *categoria) {
    // Verifica se a categoria já existe
    if (categoriaExiste(categoria)) {
        return false;
    }

    // Realoca memória para a nova categoria
    char **temp = realloc(categoriasGlobais.categorias, (categoriasGlobais.quantidade + 1) * sizeof(char *));
    if (temp == NULL) {
        printf("Erro ao alocar memoria para categorias.\n");
        return false;
    }
    categoriasGlobais.categorias = temp;

    // Aloca e copia a nova categoria
    categoriasGlobais.categorias[categoriasGlobais.quantidade] = strdup(categoria);
    if (categoriasGlobais.categorias[categoriasGlobais.quantidade] == NULL) {
        printf("Erro ao alocar memoria para nova categoria.\n");
        return false;
    }

    categoriasGlobais.quantidade++;
    return true;
}

bool categoriaExiste(const char *categoria) {
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        if (strcmp(categoriasGlobais.categorias[i], categoria) == 0) {
            return true;
        }
    }
    return false;
}

void exibirCategorias() {
    if (categoriasGlobais.quantidade == 0) {
        printf("Nenhuma categoria cadastrada.\n");
        return;
    }

    printf("\n=== CATEGORIAS DISPONIVEIS ===\n");
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        printf("%d - %s\n", i + 1, categoriasGlobais.categorias[i]);
    }
    printf("===============================\n");
}

void liberarCategorias() {
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        free(categoriasGlobais.categorias[i]);
    }
    free(categoriasGlobais.categorias);
    categoriasGlobais.categorias = NULL;
    categoriasGlobais.quantidade = 0;
}

void salvarCategorias() {
    FILE *arquivo = fopen("categorias.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de categorias para escrita!\n");
        return;
    }

    // Escreve a quantidade de categorias
    fwrite(&categoriasGlobais.quantidade, sizeof(int), 1, arquivo);

    // Escreve cada categoria
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        size_t tamanho = strlen(categoriasGlobais.categorias[i]) + 1;
        fwrite(&tamanho, sizeof(size_t), 1, arquivo);
        fwrite(categoriasGlobais.categorias[i], sizeof(char), tamanho, arquivo);
    }

    fclose(arquivo);
}

void salvarProdutos() {
    FILE *arquivo = fopen("produtos.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de produtos para escrita!\n");
        return;
    }

    // Escreve a quantidade de produtos
    fwrite(&sistemaProdutos.quantidade, sizeof(int), 1, arquivo);

    // Escreve cada produto
    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
        Produto *p = &sistemaProdutos.produtos[i];

        // Escreve os dados básicos do produto
        fwrite(p, sizeof(Produto) - sizeof(char *), 1, arquivo);

        // Escreve a categoria (se existir)
        if (p->categoriaProduto != NULL) {
            size_t tamanho = strlen(p->categoriaProduto) + 1;
            fwrite(&tamanho, sizeof(size_t), 1, arquivo);
            fwrite(p->categoriaProduto, sizeof(char), tamanho, arquivo);
        } else {
            size_t tamanho = 0;
            fwrite(&tamanho, sizeof(size_t), 1, arquivo);
        }
    }

    fclose(arquivo);
}

// ===== FUNÇÕES DE CARREGAMENTO ===== //

void carregarCategorias() {
    FILE *arquivo = fopen("categorias.dat", "rb");
    if (arquivo == NULL) {
        return; // Arquivo não existe ainda
    }

    // Lê a quantidade de categorias
    int quantidade;
    fread(&quantidade, sizeof(int), 1, arquivo);

    // Carrega cada categoria
    for (int i = 0; i < quantidade; i++) {
        size_t tamanho;
        fread(&tamanho, sizeof(size_t), 1, arquivo);

        char *categoria = (char *)malloc(tamanho * sizeof(char));
        if (categoria == NULL) {
            printf("Erro ao alocar memoria para categoria!\n");
            break;
        }

        fread(categoria, sizeof(char), tamanho, arquivo);

        // Adiciona a categoria ao sistema
        char **temp = realloc(categoriasGlobais.categorias,
                            (categoriasGlobais.quantidade + 1) * sizeof(char *));
        if (temp == NULL) {
            free(categoria);
            break;
        }

        categoriasGlobais.categorias = temp;
        categoriasGlobais.categorias[categoriasGlobais.quantidade++] = categoria;
    }

    fclose(arquivo);
}

void carregarProdutos() {
    FILE *arquivo = fopen("produtos.dat", "rb");
    if (arquivo == NULL) {
        return; // Arquivo não existe ainda
    }

    // Lê a quantidade de produtos
    int quantidade;
    fread(&quantidade, sizeof(int), 1, arquivo);

    // Carrega cada produto
    for (int i = 0; i < quantidade; i++) {
        // Realoca memória para o novo produto
        Produto *temp = realloc(sistemaProdutos.produtos,
                              (sistemaProdutos.quantidade + 1) * sizeof(Produto));
        if (temp == NULL) {
            printf("Erro ao alocar memoria para produtos!\n");
            break;
        }
        sistemaProdutos.produtos = temp;

        Produto *novo = &sistemaProdutos.produtos[sistemaProdutos.quantidade];

        // Lê os dados básicos do produto
        fread(novo, sizeof(Produto) - sizeof(char *), 1, arquivo);

        // Lê a categoria
        size_t tamanho;
        fread(&tamanho, sizeof(size_t), 1, arquivo);

        if (tamanho > 0) {
            novo->categoriaProduto = (char *)malloc(tamanho * sizeof(char));
            if (novo->categoriaProduto == NULL) {
                printf("Erro ao alocar memoria para categoria do produto!\n");
                continue;
            }
            fread(novo->categoriaProduto, sizeof(char), tamanho, arquivo);
        } else {
            novo->categoriaProduto = NULL;
        }

        sistemaProdutos.quantidade++;
    }

    fclose(arquivo);
}

void cadastrarProduto(Produto **produtos, int *quantidadeProdutos) {
    // Realoca memória para o novo produto
    Produto *temp = realloc(sistemaProdutos.produtos, (sistemaProdutos.quantidade + 1) * sizeof(Produto));
    if (temp == NULL) {
        printf("Erro ao alocar memoria para novo produto!\n");
        return;
    }
    sistemaProdutos.produtos = temp;
    *produtos = sistemaProdutos.produtos;
    *quantidadeProdutos = sistemaProdutos.quantidade;

    // Obtém referência para o novo produto
    Produto *novo = &sistemaProdutos.produtos[sistemaProdutos.quantidade];
    memset(novo, 0, sizeof(Produto)); // Inicializa a estrutura

    // Atribui código ao novo produto
    novo->codigo = sistemaProdutos.quantidade + 1;

    printf("\n=== CADASTRO DE PRODUTO ===\n");

    // Nome do produto
    printf("Nome do Produto: ");
    fgets(novo->nomeProduto, sizeof(novo->nomeProduto), stdin);
    novo->nomeProduto[strcspn(novo->nomeProduto, "\n")] = '\0';

    // Seleção de categoria
    int opcaoCategoria;
    char nomeCategoria[100];
    bool categoriaDefinida = false;

    while (!categoriaDefinida) {
        system("cls");
        printf("\n=== SELECIONE UMA CATEGORIA ===\n");
        exibirCategorias();
        printf("\n1 - Selecionar categoria existente\n");
        printf("2 - Criar nova categoria\n");
        printf("0 - Cancelar cadastro\n");
        printf("Opcao: ");
        scanf("%d", &opcaoCategoria);
        getchar();

        switch (opcaoCategoria) {
            case 1: {
                if (categoriasGlobais.quantidade == 0) {
                    printf("Nenhuma categoria disponivel. Voce precisa criar uma nova.\n");
                    system("pause");
                    break;
                }

                int categoriaSelecionada;
                printf("\nDigite o numero da categoria: ");
                scanf("%d", &categoriaSelecionada);
                getchar();

                if (categoriaSelecionada > 0 && categoriaSelecionada <= categoriasGlobais.quantidade) {
                    novo->categoriaProduto = strdup(categoriasGlobais.categorias[categoriaSelecionada - 1]);
                    if (novo->categoriaProduto == NULL) {
                        printf("Erro ao alocar memoria para categoria!\n");
                        break;
                    }
                    categoriaDefinida = true;
                } else {
                    printf("Opcao invalida!\n");
                    system("pause");
                }
                break;
            }
            case 2: {
                printf("\nNome da nova categoria: ");
                fgets(nomeCategoria, sizeof(nomeCategoria), stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = '\0';

                // Validação do nome da categoria
                bool nomeValido = true;
                for (size_t i = 0; nomeCategoria[i] != '\0'; i++) {
                    if (!isalnum(nomeCategoria[i]) && nomeCategoria[i] != ' ') {
                        nomeValido = false;
                        break;
                    }
                }

                if (!nomeValido) {
                    printf("Nome de categoria invalido! Use apenas letras, numeros e espacos.\n");
                    system("pause");
                    break;
                }

                if (!categoriaExiste(nomeCategoria)) {
                    if (adicionarCategoria(nomeCategoria)) {
                        novo->categoriaProduto = strdup(nomeCategoria);
                        if (novo->categoriaProduto == NULL) {
                            printf("Erro ao alocar memoria para categoria!\n");
                            break;
                        }
                        salvarCategorias();
                        categoriaDefinida = true;
                    } else {
                        printf("Falha ao adicionar nova categoria!\n");
                        system("pause");
                    }
                } else {
                    printf("Categoria ja existe!\n");
                    system("pause");
                }
                break;
            }
            case 0:
                printf("Cadastro cancelado.\n");
                free(temp);
                return;
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }
    }

    // Preço de compra
    do {
        printf("\nPreco de Compra: R$ ");
        if (scanf("%f", &novo->precoCompra) != 1 || novo->precoCompra <= 0) {
            printf("Valor incorreto! Digite um valor positivo.\n");
            while (getchar() != '\n') // Limpa o buffer
            break;
        }
        break;
    } while (1);
    getchar();

    // Margem de lucro
    do {
        printf("\nMargem de Lucro (%%): ");
        if (scanf("%f", &novo->percentual) != 1 || novo->percentual < 0) {
            printf("Valor incorreto! Digite um percentual nao negativo.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);
    getchar();

    // Calcula preço de venda
    novo->precoVenda = novo->precoCompra * (1 + novo->percentual / 100);
    printf("\nPreco de Venda Calculado: R$ %.2f\n", novo->precoVenda);

    // Quantidade em estoque
    do {
        printf("\nQuantidade em Estoque: ");
        if (scanf("%d", &novo->estoque) != 1 || novo->estoque < 0) {
            printf("Valor incorreto! Digite um número inteiro nao negativo.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);
    getchar();

    // Estoque mínimo
    do {
        printf("\nEstoque Minimo: ");
        if (scanf("%d", &novo->estoqueMinimo) != 1 || novo->estoqueMinimo < 0) {
            printf("Valor incorreto! Digite um numero inteiro nao negativo.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);
    getchar();

    // Atualiza a quantidade de produtos
    sistemaProdutos.quantidade++;
    *quantidadeProdutos = sistemaProdutos.quantidade;

    // Persistência de dados
    salvarProdutos();

    printf("\nProduto cadastrado com sucesso! Codigo: %d\n", novo->codigo);
    system("pause");
}

void menuVendas() {
    while (1) {
        system("cls");
        printf("|====================================================================|\n");
        printf("|\t\t    MENU VENDAS\t\t\t\t\t     |\n");
        printf("|====================================================================|\n");

        // Exibe produtos disponíveis usando sistemaProdutos global
        if (sistemaProdutos.quantidade > 0) {
            printf("|====================================================================|\n");
            printf("|PRODUTOS DISPONIVEIS:\t\t\t\t\t\t     |\n");
            printf("|====================================================================|\n");
            printf("|COD   | NOME                     | CATEGORIA    | PRECO   | ESTOQUE |\n");
            printf("|======|==========================|==============|=========|=========|\n");
            for (int i = 0; i < sistemaProdutos.quantidade; i++) {
                printf("|%-5d | %-24s | %-12s | %-7.2f| %-8d |\n",
                      sistemaProdutos.produtos[i].codigo,
                      sistemaProdutos.produtos[i].nomeProduto,
                      sistemaProdutos.produtos[i].categoriaProduto,
                      sistemaProdutos.produtos[i].precoVenda,
                      sistemaProdutos.produtos[i].estoque);
                printf("|------|--------------------------|--------------|---------|----------|\n");
            }
        } else {
            printf("\n|AVISO: Nenhum produto cadastrado!\n");
        }

        printf("|====================================================================|\n");
        printf("\n|-> 1 - NOVA VENDA\n");
        printf("|-> 2 - RETIRADA DE CAIXA (SANGRIA)\n");
        printf("|-> 3 - VOLTAR AO MENU PRINCIPAL\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("|OPCAO: ");

        int opcao;
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:  // NOVA VENDA
                if (sistemaProdutos.quantidade > 0) {
                    menu_novaVenda();
                } else {
                    printf("\nNao ha produtos cadastrados para vender!\n");
                    system("pause");
                }
                break;

            case 2:  // RETIRADA DE CAIXA
                if (caixaAberto) {
                    retiradaCaixa(&totalCaixa);
                } else {
                    printf("\nCaixa fechado. Efetue a abertura primeiro.\n");
                    system("pause");
                }
                break;

            case 3:  // VOLTAR
                return;

            default:
                printf("\nOpcao invalida!\n");
                system("pause");
        }
    }
}

void menu_novaVenda() {
    if (sistemaProdutos.quantidade == 0) {
        printf("NAO HA PRODUTOS PARA VENDER!\n");
        system("pause");
        return;
    }

    ItemVenda carrinho[100];
    int numItens = 0;
    float totalVenda = 0;
    static int numeroVenda = 1;

    while (1) {
        system("cls");
        // Mostra produtos disponíveis
        printf("|====================================================================|\n");
        printf("|PRODUTOS DISPONIVEIS:\t\t\t\t\t\t     |\n");
        printf("|====================================================================|\n");
        printf("|COD   | NOME                     | CATEGORIA    | PRECO   | ESTOQUE |\n");
        printf("|======|==========================|==============|=========|=========|\n");
        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
            printf("|%-5d | %-24s | %-12s | %-7.2f | %-8d|\n",
                   sistemaProdutos.produtos[i].codigo,
                   sistemaProdutos.produtos[i].nomeProduto,
                   sistemaProdutos.produtos[i].categoriaProduto,
                   sistemaProdutos.produtos[i].precoVenda,
                   sistemaProdutos.produtos[i].estoque);
            printf("|------|--------------------------|--------------|---------|---------|\n");
        }

        // Mostra carrinho atual
        printf("\n| CARRINHO DE COMPRAS - VENDA #%d |\n", numeroVenda);
        printf("|====================================================================|\n");
        printf("|COD   | DESCRICAO              | QTD | PRECO UN. | SUBTOTAL         |\n");
        printf("|======|========================|=====|===========|==================|\n");
        for (int i = 0; i < numItens; i++) {
            printf("|%-5d | %-22s | %-3d | R$ %-7.2f | R$ %-12.2f |\n",
                   carrinho[i].codigoProduto,
                   carrinho[i].descricao,
                   carrinho[i].quantidade,
                   carrinho[i].precoUnitario,
                   carrinho[i].subtotal);
            printf("|------|------------------------|-----|-----------|-----------------|\n");
        }
        printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
        printf("|====================================================================|\n");

        int codigoProduto, quantidade;
        printf("\nInforme o codigo do produto a ser comprado (0 para finalizar): ");
        if (scanf("%d", &codigoProduto) != 1) {
            printf("Codigo invalido!\n");
            while (getchar() != '\n');
            system("pause");
            continue;
        }
        getchar();

        if (codigoProduto == 0) {
            break;
        }

        printf("Informe a Quantidade: ");
        if (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
            printf("Quantidade invalida!\n");
            while (getchar() != '\n');
            system("pause");
            continue;
        }
        getchar();

        bool produtoEncontrado = false;
        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
            if (sistemaProdutos.produtos[i].codigo == codigoProduto) {
                produtoEncontrado = true;

                if (sistemaProdutos.produtos[i].estoque < quantidade) {
                    printf("Estoque insuficiente! Disponivel: %d\n", sistemaProdutos.produtos[i].estoque);
                    system("pause");
                    break;
                }

                // Adiciona ao carrinho
                carrinho[numItens].codigoProduto = codigoProduto;
                strcpy(carrinho[numItens].descricao, sistemaProdutos.produtos[i].nomeProduto);
                carrinho[numItens].precoUnitario = sistemaProdutos.produtos[i].precoVenda;
                carrinho[numItens].quantidade = quantidade;
                carrinho[numItens].subtotal = sistemaProdutos.produtos[i].precoVenda * quantidade;

                // Atualiza totais
                totalVenda += carrinho[numItens].subtotal;
                sistemaProdutos.produtos[i].estoque -= quantidade;
                numItens++;

                printf("Adicionado: %d x %s (R$ %.2f cada)\n", quantidade,
                       sistemaProdutos.produtos[i].nomeProduto,
                       sistemaProdutos.produtos[i].precoVenda);
                system("pause");
                break;
            }
        }

        if (!produtoEncontrado) {
            printf("Produto nao encontrado!\n");
            system("pause");
        }
    }

    if (numItens > 0) {
        // Aplica desconto se solicitado
        char opcaoDesconto;
        float totalDesconto = totalVenda;

        system("cls");
        printf("\n| CARRINHO DE COMPRAS - VENDA #%d |\n", numeroVenda);
        printf("|====================================================================|\n");
        printf("|COD   | DESCRICAO              | QTD | PRECO UN. | SUBTOTAL         |\n");
        printf("|======|========================|=====|===========|==================|\n");
        for (int i = 0; i < numItens; i++) {
            printf("|%-5d | %-22s | %-3d | R$ %-7.2f | R$ %-12.2f |\n",
                   carrinho[i].codigoProduto,
                   carrinho[i].descricao,
                   carrinho[i].quantidade,
                   carrinho[i].precoUnitario,
                   carrinho[i].subtotal);
            printf("|------|------------------------|-----|-----------|-----------------|\n");
        }
        printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
        printf("|====================================================================|\n");

        printf("Deseja aplicar desconto de 5%%? (s/n): ");
        scanf(" %c", &opcaoDesconto);
        getchar();

        if (tolower(opcaoDesconto) == 's') {
            totalDesconto = totalVenda * 0.95f;
            printf("Desconto aplicado. Novo total: R$ %.2f\n", totalDesconto);
            system("pause");
        }

        // Chama menu de pagamento com o valor correto
        menu_pagamento(totalDesconto);

        // Atualiza o número da venda após conclusão
        numeroVenda++;
    }
}

void menu_pagamento(float totalVenda) {
    static float totalDinheiro = 0, totalCartao = 0;
    const float totalOriginal = totalVenda;
    int opcaoPagamento;

    do {
        system("cls");
        printf("|====================================================================|\n");
        printf("| TOTAL A PAGAR: R$ %-40.2f |\n", totalVenda);
        printf("|====================================================================|\n");
        printf("| FORMAS DE PAGAMENTO:\t\t\t\t\t     |\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("| 1. CARTAO\t\t\t\t\t\t     |\n");
        printf("| 2. DINHEIRO\t\t\t\t\t\t     |\n");
        printf("| 3. VOLTAR\t\t\t\t\t\t     |\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("OPCAO: ");

        if (scanf("%d", &opcaoPagamento) != 1) {
            printf("Opcao invalida!\n");
            while (getchar() != '\n');
            system("pause");
            continue;
        }
        getchar();

        switch(opcaoPagamento) {
            case 1: // Cartão
                totalCartao += totalVenda;
                printf("Pagamento com cartao no valor de R$ %.2f confirmado.\n", totalVenda);
                totalVenda = 0;
                system("pause");
                break;

            case 2: // Dinheiro
                float valorRecebido;
                printf("Informe o valor recebido em dinheiro: R$ ");
                if (scanf("%f", &valorRecebido) != 1 || valorRecebido <= 0) {
                    printf("Valor invalido!\n");
                    while (getchar() != '\n');
                    system("pause");
                    continue;
                }
                getchar();

                if (valorRecebido >= totalVenda) {
                    float troco = valorRecebido - totalVenda;
                    if (troco > totalCaixa) {
                        printf("Caixa nao tem troco suficiente!\n");
                        system("pause");
                        continue;
                    }
                    totalDinheiro += totalVenda;
                    totalCaixa -= troco;
                    printf("Troco: R$ %.2f\n", troco);
                    totalVenda = 0;
                } else {
                    totalDinheiro += valorRecebido;
                    totalVenda -= valorRecebido;
                    printf("Falta pagar: R$ %.2f\n", totalVenda);
                }
                system("pause");
                break;

            case 3: // Voltar
                return;

            default:
                printf("Opcao invalida!\n");
                system("pause");
        }

        // Se o valor foi totalmente pago
        if (totalVenda <= 0.01f) {
            system("cls");
            printf("|====================================================================|\n");
            printf("| VENDA FINALIZADA COM SUCESSO!\t\t\t\t     |\n");
            printf("|====================================================================|\n");
            printf("| NUMERO DA VENDA: %-45d |\n", numeroVenda);
            printf("| TOTAL: R$ %-52.2f |\n", totalOriginal);
            printf("| FORMA DE PAGAMENTO:\t\t\t\t\t     |\n");
            printf("| - DINHEIRO: R$ %-45.2f |\n", totalDinheiro);
            printf("| - CARTAO: R$ %-46.2f |\n", totalCartao);
            printf("|====================================================================|\n");

            // Atualiza totais globais
            totalVendas += totalOriginal;
            totalDinheiroF += totalDinheiro;
            totalCartaoF += totalCartao;
            totalCaixa += totalDinheiro;

            system("pause");
            return;
        }
    } while (opcaoPagamento);
}

// Função auxiliar para processar pagamento com cartão
void processarPagamentoCartao(float *totalCartao, float totalOriginal) {
    int opcaoCartao;

    system("cls");
    printf("|====================================================================|\n");
    printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
    printf("|====================================================================|\n");
    printf("|MENU PAGAMENTO CARTAO:                                              |\n");
    printf("|1. PAGAMENTO NA MAQUININHA OK!                                      |\n");
    printf("|2. PAGAMENTO NO CARTAO NAO OK.                                      |\n");
    printf("|3. RETORNAR AO MENU ANTERIOR                                        |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("OPCAO: ");
    scanf("%d", &opcaoCartao);
    getchar();

    switch(opcaoCartao) {
        case 1: // PAGAMENTO APROVADO
            *totalCartao += totalVenda;
            finalizarVenda(totalOriginal, 0, *totalCartao);
            break;

        case 2: // PAGAMENTO NEGADO
            printf("ERRO AO EFETUAR PAGAMENTO EM CARTAO!\n");
            system("pause");
            break;

        case 3: // VOLTAR
            break;

        default:
            printf("Opcao invalida!\n");
            system("pause");
    }
}

// Função auxiliar para processar pagamento em dinheiro
void processarPagamentoDinheiro(float *totalDinheiro, float *totalVenda, float totalOriginal) {
    float pagamento;
    system("cls");
    printf("|====================================================================|\n");
    printf("| TOTAL DA VENDA: R$ %-40.2f |\n", *totalVenda);
    printf("|====================================================================|\n");
    printf("DIGITE O VALOR RECEBIDO: R$ ");
    scanf("%f", &pagamento);
    getchar();

    if (pagamento > *totalVenda) {
        // Pagamento maior que o valor devido (dar troco)
        float troco = pagamento - *totalVenda;
        if (troco > totalCaixa) {
            printf("CAIXA NAO TEM TROCO SUFICIENTE!\n");
            system("pause");
            return;
        }
        *totalDinheiro += *totalVenda;
        totalCaixa -= troco;
        printf("TROCO: R$ %.2f\n", troco);
        system("pause");
        finalizarVenda(totalOriginal, *totalDinheiro, 0);
    }
    else if (pagamento == *totalVenda) {
        // Pagamento exato
        *totalDinheiro += pagamento;
        finalizarVenda(totalOriginal, *totalDinheiro, 0);
    }
    else {
        // Pagamento parcial
        *totalVenda -= pagamento;
        *totalDinheiro += pagamento;
        printf("FALTA PAGAR: R$ %.2f\n", *totalVenda);
        system("pause");
    }
}

// Função auxiliar para finalizar a venda
void finalizarVenda(float totalOriginal, float dinheiro, float cartao) {
    system("cls");
    printf("PAGAMENTO REALIZADO COM SUCESSO!\n");
    system("pause");

    printf("\n|VENDA FINALIZADA COM SUCESSO!\n");
    printf("  |Numero da venda: %d\n", numeroVenda);
    printf("  |Total: R$ %.2f\n", totalOriginal);
    printf("  |DINHEIRO: R$ %.2f\n", dinheiro);
    printf("  |CARTAO: R$ %.2f\n", cartao);
    system("pause");

    // Atualiza totais
    numeroVenda++;
    totalVendas += totalOriginal;
    totalDinheiroF += dinheiro;
    totalCartaoF += cartao;
    totalCaixa += dinheiro;

    // Limpa carrinho e volta ao menu principal
    menu_principal();
}

float retiradaCaixa(float *totalCaixa) {
    const float SALDO_MINIMO = 50.0f;
    const int MAX_TENTATIVAS = 3;
    int tentativas = 0;
    float valorRetirada;

    do {
        system("cls");
        printf("|===========================================================|\n");
        printf("|\t\t   RETIRADA DE CAIXA (SANGRIA)\t\t\t   |\n");
        printf("|===========================================================|\n");
        printf("| SALDO ATUAL EM CAIXA: R$ %.2f\n", *totalCaixa);
        printf("| SALDO MINIMO OBRIGATORIO: R$ %.2f\n", SALDO_MINIMO);
        printf("| DIGITE O VALOR A SER RETIRADO (0 para cancelar): R$ ");

        // Validação da entrada
        if (scanf("%f", &valorRetirada) != 1) {
            printf("\n| VALOR INVALIDO! Digite apenas numeros.\n");
            while (getchar() != '\n'); // Limpa buffer
            system("pause");
            tentativas++;
            if (tentativas >= MAX_TENTATIVAS) {
                printf("\n| MUITAS TENTATIVAS INVALIDAS. OPERACAO CANCELADA.\n");
                system("pause");
                return *totalCaixa;
            }
            continue;
        }
        getchar(); // Limpa o \n

        // Verifica se o usuário quer cancelar
        if (valorRetirada == 0) {
            printf("\n| OPERACAO CANCELADA PELO USUARIO.\n");
            system("pause");
            return *totalCaixa;
        }

        // Validações do valor
        if (valorRetirada <= 0) {
            printf("\n| VALOR DEVE SER POSITIVO!\n");
            system("pause");
            tentativas++;
            continue;
        }

        if (valorRetirada > *totalCaixa) {
            printf("\n| SALDO INSUFICIENTE!\n");
            printf("| SALDO ATUAL: R$ %.2f\n", *totalCaixa);
            system("pause");
            tentativas++;
            continue;
        }

        if (*totalCaixa - valorRetirada < SALDO_MINIMO) {
            printf("\n| NAO E POSSIVEL RETIRAR ESTE VALOR!\n");
            printf("| SALDO MINIMO DE R$ %.2f DEVE SER MANTIDO.\n", SALDO_MINIMO);
            printf("| VALOR MAXIMO QUE PODE SER RETIRADO: R$ %.2f\n",
                  *totalCaixa - SALDO_MINIMO);
            system("pause");
            tentativas++;
            continue;
        }

        // Confirmação da operação
        printf("\n| CONFIRMAR RETIRADA DE R$ %.2f? (s/n): ", valorRetirada);
        char confirmacao = getchar();
        getchar(); // Limpa buffer

        if (tolower(confirmacao) != 's') {
            printf("\n| OPERACAO CANCELADA.\n");
            system("pause");
            return *totalCaixa;
        }

        // Efetua a retirada
        *totalCaixa -= valorRetirada;

        // Registra a operação (poderia ser gravado em log)
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);

        printf("\n| RETIRADA REALIZADA COM SUCESSO!\n");
        printf("| DATA/HORA: %02d/%02d/%04d %02d:%02d\n",
               local->tm_mday, local->tm_mon+1, local->tm_year+1900,
               local->tm_hour, local->tm_min);
        printf("| VALOR RETIRADO: R$ %.2f\n", valorRetirada);
        printf("| NOVO SALDO: R$ %.2f\n", *totalCaixa);
        printf("|===========================================================|\n");
        system("pause");

        return *totalCaixa;

    } while (tentativas < MAX_TENTATIVAS);

    printf("\n| MUITAS TENTATIVAS INVALIDAS. OPERACAO CANCELADA.\n");
    system("pause");
    return *totalCaixa;
}

void menuAberturaCaixa() {
    int opcaoAbre;
    int tentativas = 0;
    const int MAX_TENTATIVAS = 3;

    do {
        system("cls");
        printf("|====================================================================|\n");
        printf("|\t\t    MENU ABERTURA\t\t\t\t     |\n");
        printf("|====================================================================|\n");
        printf("|-> 1. ABRIR CAIXA\n");
        printf("|-> 2. RETORNAR AO MENU PRINCIPAL\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("| OPCAO: ");

        // Corrigido: removido o "|" do scanf que causava problemas
        if (scanf("%d", &opcaoAbre) != 1) {
            // Limpa buffer de entrada em caso de erro
            while (getchar() != '\n');
            printf("Entrada invalida! Digite um numero.\n");
            system("pause");
            tentativas++;
            continue;
        }
        getchar(); // Limpa o newline

        switch(opcaoAbre) {
            case 1: {
                if (caixaAberto == 1) {
                    printf("JA HA UM CAIXA EM ABERTO, REALIZE O FECHAMENTO\n");
                    system("pause");
                    return; // Retorna ao invés de chamar menu_principal recursivamente
                }

                printf("REALIZANDO ABERTURA DE CAIXA...\n");
                Sleep(2000);

                int entradaValida = 0;
                do {
                    system("cls");
                    printf("DIGITE O VALOR DE ABERTURA: \n");
                    printf("VALOR R$ ");

                    if (scanf("%f", &vAbre) != 1) {
                        while (getchar() != '\n');
                        printf("Valor invalido! Digite um numero.\n");
                        system("pause");
                        tentativas++;
                        if (tentativas >= MAX_TENTATIVAS) {
                            printf("Muitas tentativas invalidas. Retornando...\n");
                            system("pause");
                            return;
                        }
                        continue;
                    }
                    getchar(); // Limpa o newline

                    if (vAbre < 0) {
                        printf("VALOR DE ABERTURA INCORRETO, DIGITE UM VALOR POSITIVO\n");
                        system("pause");
                        tentativas++;
                        if (tentativas >= MAX_TENTATIVAS) {
                            printf("Muitas tentativas invalidas. Retornando...\n");
                            system("pause");
                            return;
                        }
                    } else {
                        entradaValida = 1;
                    }
                } while (!entradaValida);

                system("cls");
                printf("ABERTURA REALIZADA COM SUCESSO!\n");
                printf("VALOR DE ABERTURA: R$ %.2f\n", vAbre);
                totalCaixa = vAbre;
                caixaAberto = 1;
                system("pause");
                return;
            }
            case 2:
                return; // Simplesmente retorna ao menu principal
            default:
                printf("OPCAO INVALIDA, TENTE NOVAMENTE\n");
                system("pause");
                tentativas++;
        }
    } while (tentativas < MAX_TENTATIVAS);

    printf("Muitas tentativas invalidas. Retornando ao menu principal...\n");
    system("pause");
}

void menuFechamentoCaixa() {
    int opcaoFechamento;
    float vFechamento = 0;
    const int MAX_TENTATIVAS = 3;
    int tentativas = 0;

    do {
        system("cls");
        printf("|====================================================================|\n");
        printf("|\t\t    MENU FECHAMENTO\t\t\t\t     |\n");
        printf("|====================================================================|\n");
        printf("|-> 1. FECHAR CAIXA\n");
        printf("|-> 2. RETORNAR AO MENU PRINCIPAL\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("| OPCAO: ");

        // Validação da entrada
        if (scanf("%d", &opcaoFechamento) != 1) {
            printf("Entrada invalida! Digite 1 ou 2.\n");
            while (getchar() != '\n'); // Limpa buffer
            system("pause");
            tentativas++;
            if (tentativas >= MAX_TENTATIVAS) {
                printf("Muitas tentativas invalidas. Retornando...\n");
                system("pause");
                return;
            }
            continue;
        }
        getchar(); // Limpa o \n

        switch(opcaoFechamento) {
            case 1: {
                system("cls");
                if (caixaAberto == 0) {
                    printf("NAO HA CAIXA ABERTO, RETORNANDO...\n");
                    system("pause");
                    return; // Retorna ao invés de chamar menu_principal()
                }

                printf("REALIZANDO FECHAMENTO DE CAIXA...\n");
                Sleep(1000);
                system("cls");

                // Relatório detalhado
                printf("|================ RESUMO DO DIA ================|\n");
                printf("| VENDAS REALIZADAS: %-26d |\n", numeroVenda);
                printf("| FATURAMENTO TOTAL: R$ %-23.2f |\n", totalVendas);
                printf("|-----------------------------------------------|\n");
                printf("| VALOR ABERTURA: R$ %-26.2f |\n", vAbre);
                printf("| TOTAL DINHEIRO: R$ %-26.2f |\n", totalDinheiroF);
                printf("| TOTAL CARTAO: R$ %-27.2f |\n", totalCartaoF);
                printf("| DINHEIRO/CARTAO: R$ %-24.2f |\n", totalDinheiroCartaoF);
                printf("|===============================================|\n");

                // Cálculo do fechamento
                vFechamento = totalCaixa - (vAbre + totalDinheiroF);

                printf("| SALDO EM CAIXA: R$ %-27.2f |\n", totalCaixa);
                printf("| VALOR ESPERADO: R$ %-26.2f |\n", vAbre + totalDinheiroF);
                printf("| DIFERENCA: R$ %-31.2f |\n", vFechamento);
                printf("|===============================================|\n");

                if (fabsf(vFechamento) > 0.01) { // Considera pequenas diferenças
                    printf("\nATENCAO: DIVERGENCIA ENCONTRADA!\n");
                    printf("Verifique os valores informados.\n");

                    // Sugere possíveis problemas
                    if (vFechamento > 0) {
                        printf("Possivel valor nao registrado ou troco incorreto.\n");
                    } else {
                        printf("Possivel sangria nao registrada ou erro nas vendas.\n");
                    }

                    printf("Deseja forcar o fechamento? (s/n): ");
                    char resposta = getchar();
                    if (tolower(resposta) != 's') {
                        system("pause");
                        return;
                    }
                }

                // Confirmação de fechamento
                printf("\nCONFIRMAR FECHAMENTO DE CAIXA? (s/n): ");
                char confirmacao = getchar();
                getchar(); // Limpa buffer

                if (tolower(confirmacao) == 's') {
                    caixaAberto = 0;
                    printf("\nCAIXA FECHADO COM SUCESSO!\n");

                    // Zera os valores do dia
                    totalVendas = 0;
                    numeroVenda = 0;
                    totalDinheiroF = 0;
                    totalCartaoF = 0;
                    totalDinheiroCartaoF = 0;
                    vAbre = 0;
                } else {
                    printf("Fechamento cancelado.\n");
                }
                system("pause");
                return;
            }
            case 2:
                return; // Retorna ao menu principal
            default:
                printf("OPCAO INVALIDA! Digite 1 ou 2.\n");
                system("pause");
                tentativas++;
        }
    } while (tentativas < MAX_TENTATIVAS);

    printf("Muitas tentativas invalidas. Retornando ao menu principal...\n");
    system("pause");
}

// Implementação da função principal de relatórios
void menuRelatorios() {
    Cliente *clientes = NULL;
    int quantidadeClientes = 0;
    carregarClientes(&clientes, &quantidadeClientes);

    int opcao;
    do {
        system("cls");
        printf("|=======================================================|\n");
        printf("|\t\t    MENU RELATORIOS\t\t\t|\n");
        printf("|=======================================================|\n");
        printf("| -> 1 - RELATORIO DE VENDAS POR DIA \t\t\t|\n");
        printf("| -> 2 - RELATORIO DE PRODUTOS CADASTRADOS \t\t|\n");
        printf("| -> 3 - RELATORIO DE CLIENTES CADASTRADOS \t\t|\n");
        printf("| -> 4 - VISUALIZAR LOGS DO SISTEMA \t\t\t|\n");
        printf("| -> 5 - VOLTAR AO MENU PRINCIPAL \t\t\t|\n");
        printf("|=======================================================|\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            registrarLog("Erro: Tentativa de entrada invalida no menu de relatorios");
            while (getchar() != '\n');
            printf("Entrada invalida!\n");
            system("pause");
            continue;
        }
        getchar();

        char logMensagem[100];

        switch(opcao) {
            case 1:
                snprintf(logMensagem, sizeof(logMensagem), "Relatorio de vendas gerado - %d vendas", numeroVenda);
                registrarLog(logMensagem);
                exibirRelatorioVendas();
                break;
            case 2:
                snprintf(logMensagem, sizeof(logMensagem), "Relatorio de produtos acessado - %d produtos", sistemaProdutos.quantidade);
                registrarLog(logMensagem);
                exibirProdutosCadastrados();
                break;
            case 3:
                snprintf(logMensagem, sizeof(logMensagem), "Relatorio de clientes acessado - %d clientes", quantidadeClientes);
                registrarLog(logMensagem);
                exibirClientesCadastrados(clientes, quantidadeClientes);
                break;
            case 4:
                registrarLog("Visualizacao de logs do sistema");
                exibirLogs();
                break;
            case 5:
                free(clientes);
                registrarLog("Saida do menu de relatorios");
                return;
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }
    } while(opcao != 5);
}

void registrarLog(const char *mensagem) {
    time_t now;
    time(&now);
    const struct tm *local = localtime(&now);

    FILE *logFile = fopen(ARQUIVO_LOG, "a");
    if (logFile == NULL) {
        printf("Erro ao abrir arquivo de log!\n");
        return;
    }

    fprintf(logFile, "[%02d/%02d/%04d %02d:%02d:%02d] %s\n",
            local->tm_mday, local->tm_mon+1, local->tm_year+1900,
            local->tm_hour, local->tm_min, local->tm_sec,
            mensagem);

    fclose(logFile);
}

// Função para exibir ‘logs’
void exibirLogs() {
    system("cls");
    printf("|====================================================================|\n");
    printf("|\t\t    LOGS DO SISTEMA\t\t\t\t     |\n");
    printf("|====================================================================|\n");

    FILE *logFile = fopen(ARQUIVO_LOG, "r");
    if (logFile == NULL) {
        printf("| Nenhum registro de log encontrado.\n");
        printf("|====================================================================|\n");
        system("pause");
        return;
    }

    char linha[200];
    int contador = 0;

    while (fgets(linha, sizeof(linha), logFile) != NULL) {
        // Remove a quebra de linha
        linha[strcspn(linha, "\n")] = '\0';
        printf("| %-66s |\n", linha);
        contador++;

        // Pausa a cada 20 registros
        if (contador % 20 == 0) {
            printf("|====================================================================|\n");
            printf("| Pressione qualquer tecla para continuar... (%d registros)           |\n", contador);
            printf("|====================================================================|\n");
            getchar();
            system("cls");
            printf("|====================================================================|\n");
            printf("|\t\t    LOGS DO SISTEMA (CONT.)\t\t\t     |\n");
            printf("|====================================================================|\n");
        }
    }

    if (contador == 0) {
        printf("| Nenhum registro de log encontrado.\n");
    } else {
        printf("|--------------------------------------------------------------------|\n");
        printf("| TOTAL DE REGISTROS: %-46d |\n", contador);
    }

    printf("|====================================================================|\n");
    fclose(logFile);
    system("pause");
}

// Implementação da função para exibir relatório de vendas
void exibirRelatorioVendas() {
    char logMensagem[200];
    snprintf(logMensagem, sizeof(logMensagem), "Relatorio de vendas detalhado - Total: R$ %.2f, Vendas: %d",
             totalVendas, numeroVenda);
    registrarLog(logMensagem);
    system("cls");
    printf("|====================================================================|\n");
    printf("|\t\t    RELATORIO DE VENDAS\t\t\t\t     |\n");
    printf("|====================================================================|\n");
    printf("| DATA: %s\n", __DATE__); // Usa a data atual do sistema
    printf("|--------------------------------------------------------------------|\n");
    printf("| TOTAL DE VENDAS HOJE: %d\n", numeroVenda);
    printf("| FATURAMENTO TOTAL: R$ %.2f\n", totalVendas);
    printf("|--------------------------------------------------------------------|\n");
    printf("| FORMA DE PAGAMENTO\t| QUANTIDADE\t\t| VALOR TOTAL        |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("| DINHEIRO\t\t|\t\t\t|\tR$ %.2f\t     |\n", totalDinheiroF);
    printf("| CARTAO\t\t|\t\t\t|\tR$ %.2f      |\n", totalCartaoF);
    printf("|====================================================================|\n");
    system("pause");
}

// Implementação da função para exibir produtos cadastrados
void exibirProdutosCadastrados() {
    system("cls");
    printf("|====================================================================|\n");
    printf("|\t\t    PRODUTOS CADASTRADOS\t\t\t     |\n");
    printf("|====================================================================|\n");
    printf("|COD | NOME PRODUTO          | CATEGORIA     | ESTOQUE | PRECO VENDA |\n");
    printf("|====|=======================|===============|=========|=============|\n");


    for(int i = 0; i < sistemaProdutos.quantidade; i++) {
        printf("|%-3d | %-21s | %-13s | %-7d | R$ %-8.2f |\n",
               sistemaProdutos.produtos[i].codigo,
               sistemaProdutos.produtos[i].nomeProduto,
               sistemaProdutos.produtos[i].categoriaProduto,
               sistemaProdutos.produtos[i].estoque,
               sistemaProdutos.produtos[i].precoVenda);
        printf("|----|-----------------------|---------------|---------|-------------|\n");
    }

    printf("| TOTAL DE PRODUTOS: %-47d |\n", sistemaProdutos.quantidade);
    printf("|====================================================================|\n");
    system("pause");
}

// Implementação da função para exibir clientes cadastrados
void exibirClientesCadastrados(Cliente *clientes, const int quantidadeClientes) {
    system("cls");
    printf("|====================================================================|\n");
    printf("|\t\t    CLIENTES CADASTRADOS\t\t\t     |\n");
    printf("|====================================================================|\n");
    printf("|COD | NOME COMPLETO         | CPF           | CELULAR               |\n");
    printf("|====|=======================|===============|=======================|\n");

    for(int i = 0; i < quantidadeClientes; i++) {
        printf("|%-3d | %-21s | %-13s | %-19s   |\n",
               clientes[i].codigo,
               clientes[i].nomeCompleto,
               clientes[i].cpf,
               clientes[i].celular);
        printf("|----|-----------------------|---------------|-----------------------|\n");
    }

    printf("| TOTAL DE CLIENTES: %-47d |\n", quantidadeClientes);
    printf("|====================================================================|\n");
    system("pause");
}

void verificarEstoqueMinimo() {
    // Verifica se o sistema de produtos foi inicializado corretamente
    if (sistemaProdutos.produtos == NULL || sistemaProdutos.quantidade <= 0) {
        return;
    }

    int produtosAbaixoDoMinimo = 0;

    // Primeira passada: contar produtos abaixo do mínimo com dados válidos
    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
        // Verifica se os valores de estoque são válidos (não negativos)
        if (sistemaProdutos.produtos[i].estoque >= 0 &&
            sistemaProdutos.produtos[i].estoqueMinimo >= 0) {
            if (sistemaProdutos.produtos[i].estoque <= sistemaProdutos.produtos[i].estoqueMinimo) {
                produtosAbaixoDoMinimo++;
            }
        }
    }

    // Se encontrou produtos abaixo do mínimo, exibe o alerta
    if (produtosAbaixoDoMinimo > 0) {
        system("cls");
        printf("\n==============================================================\n");
        printf("                ALERTA DE ESTOQUE BAIXO                    \n");
        printf("==============================================================\n");
        printf(" Existem %d produto(s) com estoque no nivel minimo ou abaixo\n", produtosAbaixoDoMinimo);
        printf("--------------------------------------------------------------\n");
        printf(" COD  NOME DO PRODUTO           ESTOQUE  ESTOQUE MIN  FALTAM \n");
        printf("--------------------------------------------------------------\n");

        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
            // Verifica novamente para garantir consistência
            if (sistemaProdutos.produtos[i].estoque >= 0 &&
                sistemaProdutos.produtos[i].estoqueMinimo >= 0 &&
                sistemaProdutos.produtos[i].estoque <= sistemaProdutos.produtos[i].estoqueMinimo) {

                // Calcula a quantidade que falta com segurança
                int faltam = sistemaProdutos.produtos[i].estoqueMinimo - sistemaProdutos.produtos[i].estoque;
                faltam = (faltam < 0) ? 0 : faltam;  // Garante que não seja negativo

                // Formatação segura do nome do produto
                char nomeSeguro[25];
                if (sistemaProdutos.produtos[i].nomeProduto[0] != '\0') {
                    strncpy(nomeSeguro, sistemaProdutos.produtos[i].nomeProduto, sizeof(nomeSeguro)-1);
                    nomeSeguro[sizeof(nomeSeguro)-1] = '\0';
                } else {
                    strcpy(nomeSeguro, "N/A");
                }

                // Exibe os valores formatados e validados
                printf(" %-4d %-24s %-8d %-12d %-9d \n",
                       sistemaProdutos.produtos[i].codigo,
                       nomeSeguro,
                       sistemaProdutos.produtos[i].estoque,
                       sistemaProdutos.produtos[i].estoqueMinimo,
                       faltam);
            }
        }

        printf("==============================================================\n");
        printf("\nPressione ENTER para continuar...\n");

        // Limpa o buffer de entrada completamente
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

// função main() para chamar corretamente:
int main() {
    system("color 0A"); // Cor verde no terminal (sem caracteres especiais)

    // Inicialização de variáveis
    Cliente *clientes = NULL;
    int quantidadeClientes = 0;

    // Carrega dados ao iniciar o programa
    carregarCategorias();
    carregarProdutos();
    carregarClientes(&clientes, &quantidadeClientes);

    // Verificação de estoque mínimo (adicionado aqui)
    verificarEstoqueMinimo();

    // Menu principal
    menu_principal();

    // Libera memória ao sair
    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
        free(sistemaProdutos.produtos[i].categoriaProduto);
    }
    free(sistemaProdutos.produtos);
    free(clientes);
    liberarCategorias();

    // Salva dados antes de sair
    salvarCategorias();
    salvarProdutos();
    salvarClientes(clientes, quantidadeClientes);

    return 0;
}