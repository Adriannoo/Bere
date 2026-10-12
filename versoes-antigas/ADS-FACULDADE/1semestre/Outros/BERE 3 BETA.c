#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>

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
float totalVendas = 0;
int opcaoMenu;
int caixaAberto = 0;
int proximoNumeroVenda = 1;

// ===== PROTÓTIPOS DE FUNÇÕES ===== //
void menuPrincipal();
void exibirMenu();
void menuCadastro();
void cadastrarCliente(Cliente **clientes, int *quantidadeClientes);
void cadastrarProduto(Produto **produtos, int *quantidadeProdutos);
void adicionarCategoria(const char *categoria);
void liberarCategorias();
void exibirCategorias();
bool categoriaExiste(const char *categoria);
void menuVendas(Produto *produtos, int *quantidadeProdutos);
void menuAberturaCaixa();
void menuFechamentoCaixa();
void menuRelatorios();

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

float truncarValor(float valor, int casasDecimais) {
    float potencia = pow(10.0f, casasDecimais);
    return truncf(valor * potencia) / potencia;
}

void adicionarCategoria(const char *categoria) {
    char **temp = realloc(categoriasGlobais.categorias,(categoriasGlobais.quantidade + 1) * sizeof(char*));
    if (temp == NULL) {
        printf("Erro ao alocar memoria para categorias.\n");
        return;
    }
    categoriasGlobais.categorias = temp;

    categoriasGlobais.categorias[categoriasGlobais.quantidade] = strdup(categoria);
    if (categoriasGlobais.categorias[categoriasGlobais.quantidade] == NULL) {
        printf("Erro ao alocar memoria para nova categoria.\n");
        return;
    }
    categoriasGlobais.quantidade++;
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

    printf("\n|=== CATEGORIAS DISPONIVEIS =========|\n");
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        printf("| %d. %s\n", i+1, categoriasGlobais.categorias[i]);
    }
    printf("|====================================|\n");
}

void liberarCategorias() {
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        free(categoriasGlobais.categorias[i]);
    }
    free(categoriasGlobais.categorias);
    categoriasGlobais.quantidade = 0;
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

void menuPrincipal() {
    static Produto *produtos = NULL;
    static int quantidadeProdutos = 0;

    while (1) {
        exibirMenu();
        scanf("%d", &opcaoMenu);
        getchar();

        switch (opcaoMenu) {
            case 1:
                system("cls");
                menuCadastro();
                break;
            case 2:
                menuVendas(produtos, quantidadeProdutos);
                break;
            /*case 3:
                menuAberturaCaixa();
                break;
            case 4:
                menuFechamentoCaixa();
                break;
            case 5:
                menuRelatorios();
                break;
            case 6:
                if (caixaAberto) {
                    printf("Caixa ainda aberto. Realize o fechamento primeiro.\n");
                    Sleep(2000);
                } else {
                    // Liberar memória antes de sair
                    for (int i = 0; i < quantidadeProdutos; i++) {
                        free(produtos[i].categoriaProduto);
                    }
                    free(produtos);
                    liberarCategorias();
                    printf("Saindo do sistema...\n");
                    Sleep(1000);
                    exit(0);
                }
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                Sleep(1000);*/
        }
    }
}

void menuCadastro() {
    static Cliente *clientes = NULL;
    static int quantidadeClientes = 0;
    static Produto *produtos = NULL;
    static int quantidadeProdutos = 0;

    while (1) {
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
                Sleep(1000);
        }
    }
}

void cadastrarCliente(Cliente **clientes, int *quantidadeClientes) {
    Cliente *temp = realloc(*clientes, (*quantidadeClientes + 1) * sizeof(Cliente));
    if (temp == NULL) {
        printf("Erro!\n");
        return;
    }
    *clientes = temp;

    Cliente *novoCliente = &((*clientes)[*quantidadeClientes]);
    novoCliente->codigo = *quantidadeClientes + 1;

    printf("\n=== CADASTRO DE CLIENTE ===\n");
    printf("Nome Completo: ");
    fgets(novoCliente->nomeCompleto, sizeof(novoCliente->nomeCompleto), stdin);
    strtok(novoCliente->nomeCompleto, "\n");

    printf("Nome Social (opcional): ");
    fgets(novoCliente->nomeSocial, sizeof(novoCliente->nomeSocial), stdin);
    strtok(novoCliente->nomeSocial, "\n");

    printf("CPF: ");
    fgets(novoCliente->cpf, sizeof(novoCliente->cpf), stdin);
    strtok(novoCliente->cpf, "\n");
    while (!validarCPF(novoCliente->cpf)) {
        printf("\nCPF INVALIDO, DIGITE NOVAMENTE: ");
        fgets(novoCliente->cpf, sizeof(novoCliente->cpf), stdin);
        strtok(novoCliente->cpf, "\n");
    }

    printf("Endereco: ");
    fgets(novoCliente->endereco, sizeof(novoCliente->endereco), stdin);
    strtok(novoCliente->endereco, "\n");

    printf("Bairro: ");
    fgets(novoCliente->bairro, sizeof(novoCliente->bairro), stdin);
    strtok(novoCliente->bairro, "\n");

    printf("Celular: ");
    fgets(novoCliente->celular, sizeof(novoCliente->celular), stdin);
    strtok(novoCliente->celular, "\n");

    (*quantidadeClientes)++;
    printf("\nCliente cadastrado com sucesso! Codigo: %d\n", novoCliente->codigo);
    Sleep(2000);
}

void cadastrarProduto(Produto **produtos, int *quantidadeProdutos) {
    Produto *temp = realloc(sistemaProdutos.produtos, (sistemaProdutos.quantidade + 1) * sizeof(Produto));
    if (temp == NULL) {
        printf("Erro!\n");
        return;
    }
    sistemaProdutos.produtos = temp;

    Produto *novo = &sistemaProdutos.produtos[sistemaProdutos.quantidade];
    novo->codigo = sistemaProdutos.quantidade + 1;

    printf("\n=== CADASTRO DE PRODUTO ===\n");
    printf("Nome do Produto: ");
    fgets(novo->nomeProduto, sizeof(novo->nomeProduto), stdin);
    strtok(novo->nomeProduto, "\n");

    // Seleção de categoria
    int opcaoCategoria;
    char nomeCategoria[100];

    do {
        system("cls");
        printf("\n|======SELECIONE UMA CATEGORIA ======|\n");
        exibirCategorias();
        printf("\n|-> 1 - Selecionar categoria existente\n");
        printf("|-> 2 - Criar nova categoria\n");
        printf("| Opcao: ");
        scanf("%d", &opcaoCategoria);
        getchar();

        if (opcaoCategoria == 1) {
            if (categoriasGlobais.quantidade == 0) {
                printf("Nenhuma categoria disponivel. Criando nova...\n");
                Sleep(1000);
                opcaoCategoria = 2;
                continue;
            }

            int categoriaSelecionada;
            printf("\nDigite o numero da categoria: ");
            scanf("%d", &categoriaSelecionada);

            if (categoriaSelecionada > 0 && categoriaSelecionada <= categoriasGlobais.quantidade) {
                novo->categoriaProduto = strdup(categoriasGlobais.categorias[categoriaSelecionada-1]);
                break;
            } else {
                printf("Opcao invalida!\n");
                Sleep(1000);
            }
        } else if (opcaoCategoria == 2) {
            printf("\nNome da nova categoria: ");
            fgets(nomeCategoria, sizeof(nomeCategoria), stdin);
            strtok(nomeCategoria, "\n");

            if (!categoriaExiste(nomeCategoria)) {
                adicionarCategoria(nomeCategoria);
                novo->categoriaProduto = strdup(nomeCategoria);
                break;
            } else {
                printf("Categoria ja existe!\n");
                Sleep(1000);
            }
        } else {
            printf("Opcao invalida!\n");
            Sleep(1000);
        }
    } while (1);

    printf("\nPreco de Compra: R$ ");
    scanf("%f", &novo->precoCompra);
    while (novo->precoCompra <= 0){
        printf("\nValor incorreto, digite novamente: ");
        scanf("%f", &novo->precoCompra);
    }
    getchar();

    printf("\nMargem de Lucro (%%): ");
    scanf("%f", &novo->percentual);
    while (novo->percentual <= 0){
        printf("\nValor incorreto, digite novamente: ");
        scanf("%f", &novo->percentual);
    }
    getchar();

    novo->precoVenda = novo->precoCompra * (1 + novo->percentual/100);
    printf("\nPreco de Venda Calculado: R$ %.2f\n", novo->precoVenda);

    printf("\nQuantidade em Estoque: ");
    scanf("%d", &novo->estoque);
    while (novo->estoque <= 0){
        printf("\nValor incorreto, digite novamente: ");
        scanf("%d", &novo->estoque);
    }
    getchar();

    printf("\nEstoque Minimo: ");
    scanf("%d", &novo->estoqueMinimo);
    while (novo->estoqueMinimo <= 0){
        printf("\nValor incorreto, digite novamente: ");
        scanf("%d", &novo->estoqueMinimo);
    }
    getchar();

    sistemaProdutos.quantidade++;
    printf("\nProduto cadastrado com sucesso! Codigo: %d\n", novo->codigo);
    Sleep(2000);
}

void menuVendas(Produto *produtos, int *quantidadeProdutos) {
    while (1) {
        system("cls");
        printf("|====================================================================|\n");
        printf("|\t\t    MENU VENDAS\t\t\t\t\t     |\n");
        printf("|====================================================================|\n");

        // Exibe produtos cadastrados
        if (sistemaProdutos.quantidade > 0) {
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
        } else {
            printf("\n|AVISO: Nenhum produto cadastrado!\n");
        }

        printf("|====================================================================|\n");
        printf("\n|-> 1 - Nova venda\t\t\t\t\t\t     |\n");
        printf("|-> 2 - Retirada de caixa (Sangria)\t\t\t\t     |\n");
        printf("|-> 3 - Pagamento\t\t\t\t\t\t     |\n");
        printf("|-> 4 - Voltar ao menu principal\t\t\t\t     |\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("|Opcao: ");

        int opcao;
        scanf("%d", &opcao);
        getchar();

        if (opcao == 4) {
            return;
        }

        switch (opcao) {
            case 1:
            if (sistemaProdutos.quantidade == 0) {
                printf("Nao ha produtos para vender!\n");
                Sleep(1500);
                break;
            }

    // Estrutura para armazenar a venda atual
    typedef struct {
        int codigoProduto;
        char descricao[100];
        float precoUnitario;
        int quantidade;
        float subtotal;
    } ItemVenda;

    ItemVenda carrinho[100];
    int numItens = 0;
    float totalVenda = 0;
    static int numeroVenda = 1; // Contador estático para número da venda

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
        scanf("%d", &codigoProduto);
        getchar();

        if (codigoProduto == 0) {
            break;
        }

        printf("Informe a Quantidade: ");
        scanf("%d", &quantidade);
        getchar();

        bool produtoEncontrado = false;
        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
            if (sistemaProdutos.produtos[i].codigo == codigoProduto) {
                if (sistemaProdutos.produtos[i].estoque >= quantidade) {
                    // Adiciona ao carrinho
                    carrinho[numItens].codigoProduto = codigoProduto;
                    strcpy(carrinho[numItens].descricao, sistemaProdutos.produtos[i].nomeProduto);
                    carrinho[numItens].precoUnitario = sistemaProdutos.produtos[i].precoVenda;
                    carrinho[numItens].quantidade = quantidade;
                    carrinho[numItens].subtotal = sistemaProdutos.produtos[i].precoVenda * quantidade;

                    // Atualiza totais
                    totalVenda += carrinho[numItens].subtotal;
                    numItens++;

                    // Atualiza estoque
                    sistemaProdutos.produtos[i].estoque -= quantidade;

                    printf("Adicionado: %d x %s (R$ %.2f cada)\n", quantidade,
                           sistemaProdutos.produtos[i].nomeProduto,
                           sistemaProdutos.produtos[i].precoVenda);
                } else {
                    printf("Estoque insuficiente! Disponivel: %d\n", sistemaProdutos.produtos[i].estoque);
                }
                produtoEncontrado = true;
                break;
            }
        }

        if (!produtoEncontrado) {
            printf("Produto nao encontrado!\n");
        }
        Sleep(1500);
    }

    if (numItens > 0) {
        char opcao;
        printf("\nDeseja adicionar mais itens ao carrinho? (s/n): ");
        scanf(" %c", &opcao);
        getchar();

        if (tolower(opcao) == 'n') {
            int opcaoPagamento;
            system("cls");
            printf("|====================================================================|\n");
            printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
            printf("|====================================================================|\n");
            printf("|MENU FORMA DE PAGAMENTO:                                            |\n");
            printf("|1. PAGAMENTO EM CARTAO                                              |\n");
            printf("|2. PAGAMENTO EM DINHEIRO                                            |\n");
            printf("|3. RETORNAR AO MENU PRINCIPAL                                       |\n");
            printf("|--------------------------------------------------------------------|\n");
            printf("OPCAO: ");
            scanf("%d", &opcaoPagamento);
            do {
                if (opcaoPagamento == 1){
                    int opcaoCartao;
                    system("cls");
                    printf("|====================================================================|\n");
                    printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVenda);
                    printf("|====================================================================|\n");
                    printf("|PAGAMENTO EM CARTAO:                                                |\n");
                    printf("|1) PAGAMENTO NA MAQUININHA OK!                                      |\n");
                    printf("|0) PAGAMENTO NO CARTAO NAO OK                                       |\n");
                    printf("|--------------------------------------------------------------------|\n");
                    printf("OPCAO: ");
                    scanf("%d", &opcaoCartao);
                    system("cls");
                    if (opcaoCartao == 0){
                        printf("\n\nPAGAMENTO NAO APROVADO, INFORME NOVA FORMA DE PAGAMENTO: ");
                        Sleep(2000);
                        return 0;
                    }
                    printf("PAGAMENTO REALIZADO COM SUCESSO!! ");
                    Sleep(2000);
                    char formaPagamento;
                    printf("\n|Forma de pagamento (A - Aberto / P - Pago): ");
                    printf("\n|VENDA FINALIZADA COM SUCESSO!\n");
                    printf("  |Numero da venda: %d\n", numeroVenda);
                    printf("  |Total: R$ %.2f\n", totalVenda);
                    printf("  |Status: %s\n", formaPagamento == 'P' ? "Pago" : "Aberto");

                    // Atualiza o contador de vendas
                    numeroVenda++;
                    totalVendas += totalVenda;
                }
            } while (opcaoPagamento != 3);
        }
    }

        else {
        printf("Nenhum produto foi vendido.\n");
        }
        Sleep(3000);
        break;
}
            /*case 2:

                Sleep(1500);
                break;
            case 3:

                Sleep(1500);
                break;
            default:
                printf("Opção invalida!\n");
                Sleep(1000);*/
        }
    }

void menuVenda(float *totalVendas){

}
/*void menuAberturaCaixa() {
    if (caixaAberto) {
        printf("O caixa ja esta aberto!\n");
    } else {
        caixaAberto = 1;
        printf("Caixa aberto com sucesso!\n");
    }
    Sleep(1000);
}

void menuFechamentoCaixa() {
    if (!caixaAberto) {
        printf("O caixa ja esta fechado!\n");
    } else {
        caixaAberto = 0;
        printf("Caixa fechado com sucesso!\n");
        printf("Total de vendas do dia: R$ %.2f\n", totalVendas);
    }
    Sleep(2000);
}

void menuRelatorios() {
    system("cls");
    printf("|=======================================================|\n");
    printf("|\t\t    RELATORIOS\t\t\t|\n");
    printf("|=======================================================|\n");
    printf("| -> 1 - RELATORIO DE PRODUTOS\t\t\t|\n");
    printf("| -> 2 - RELATORIO DE VENDAS\t\t\t|\n");
    printf("| -> 3 - VOLTAR AO MENU PRINCIPAL\t\t|\n");
    printf("|=======================================================|\n");
    printf("Opcao: ");

    scanf("%d", &opcaoMenu);
    getchar();

    switch (opcaoMenu) {
        case 1:
            // Implementar relatório de produtos
            break;
        case 2:
            // Implementar relatório de vendas
            break;
        case 3:
            return;
        default:
            printf("Opcao invalida!\n");
            Sleep(1000);
    }
}*/

//==== FUNÇÃO PRINCIPAL ====//
int main() {
    system("color 0C"); // Cor vermelha no terminal;
    adicionarCategoria("LIMPEZA");

    // Inicializar com categorias padrão

    menuPrincipal();;

    liberarCategorias();
    return 0;
}
