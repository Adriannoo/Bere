#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define ARQUIVO_LOG "relatorios.log"
#define MAX_ITENS_CARRINHO 100 // Define o tamanho máximo do carrinho de compras

// Estrutura para armazenar um item da venda (no carrinho)
typedef struct {
    int codigoProduto;
    char descricao[100];
    float precoUnitario;
    int quantidade;
    float subtotal;
} ItemVenda;

// Estrutura para guardar informações de um Cliente
typedef struct {
    int codigo;
    char nomeCompleto[100];
    char nomeSocial[100];
    char cpf[15];
    char endereco[100];
    char bairro[50];
    char celular[20];
} Cliente;

// Estrutura para guardar informações de um Produto
typedef struct {
    int codigo;
    char nomeProduto[100];
    char *categoriaProduto; // Guarda a categoria do produto (ex: "Alimentos", "Bebidas")
    float precoCompra;
    float percentual;
    float precoVenda;
    int estoque;
    int estoqueMinimo;
} Produto;

// Estrutura para organizar a lista de categorias de produtos
typedef struct {
    char **categorias; // Um array de nomes de categorias
    int quantidade;    // Quantas categorias existem
} ListaCategorias;

// Estrutura para organizar todos os produtos do sistema
typedef struct {
    Produto *produtos; // Um array de produtos
    int quantidade;    // Quantos produtos existem
} SistemaProdutos;

// Variáveis globais (acessíveis em qualquer parte do programa)
SistemaProdutos sistemaProdutos = {NULL, 0}; // Começa sem produtos
ListaCategorias categoriasGlobais = {NULL, 0}; // Começa sem categorias

const float SALDO_MINIMO = 50.0f; // Saldo mínimo que deve ficar no caixa
float totalCaixa = 0;             // Dinheiro atual no caixa
float totalVendas = 0;            // Faturamento total das vendas do dia
int caixaAberto = 0;              // Indica se o caixa está aberto (1) ou fechado (0)
int proximoNumeroVenda = 1;       // Número da próxima venda a ser registrada
float vAbre = 0;                  // Valor com que o caixa foi aberto
float totalDinheiroF = 0, totalCartaoF = 0; // Totais de pagamento por tipo no dia

// ===== PROTÓTIPOS DE FUNÇÕES ===== //
// Aqui declaramos as funções antes de usá-las, para o compilador saber que elas existem.
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
void menu_pagamento(float valorAPagarDaVenda, ItemVenda carrinhoDaVenda[], int numItensCarrinho);
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

/*
 * Valida se um CPF tem 11 dígitos numéricos e não são todos iguais.
 * Retorna 'true' se o CPF parecer válido, 'false' caso contrário.
 */
bool validarCPF(const char *cpf) {
    char cpfLimpo[12];
    int j = 0;
    // Remove caracteres não numéricos do CPF
    for (int i = 0; cpf[i] != '\0' && j < 11; i++) {
        if (isdigit(cpf[i])) {
            cpfLimpo[j++] = cpf[i];
        }
    }
    cpfLimpo[j] = '\0'; // Adiciona o terminador de string

    // Verifica se o CPF limpo tem 11 dígitos
    if (j != 11) {
        return false;
    }

    // Verifica se todos os dígitos são iguais (ex: "111.111.111-11" é inválido)
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
    // Esta função não faz a validação completa dos dígitos verificadores do CPF,
    // apenas um cheque de formato básico.
    return true;
}

/*
 * Trunca um valor decimal (float) para um número específico de casas decimais.
 * Ajuda a evitar problemas de precisão com números de ponto flutuante.
 */
double truncarValor(double const valor, int const casasDecimais) {
    if (casasDecimais < 0) {
        return valor;
    }
    const double fator = pow(10.0, casasDecimais);
    return trunc(valor * fator) / fator;
}

// ===== FUNÇÕES DE MENU ===== //
// Exibe o menu principal do sistema.
void exibirMenu() {
    system("cls"); // Limpa a tela do console
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

/*
 * Função principal que controla o fluxo do programa.
 * Carrega os dados iniciais, exibe o menu principal
 * e gerencia a navegação entre as diferentes seções do sistema.
 * Também é responsável por salvar os dados ao sair.
 */
void menu_principal() {
    int opcaoMenuLocal;

    // Carrega categorias e produtos do arquivo ao iniciar
    carregarCategorias();
    carregarProdutos();

    // Carrega clientes do arquivo
    Cliente *clientes = NULL; // Ponteiro para o array de clientes
    int quantidadeClientes = 0;
    carregarClientes(&clientes, &quantidadeClientes); // Passa o endereço do ponteiro e da quantidade

    do {
        exibirMenu(); // Mostra o menu principal
        scanf("%d", &opcaoMenuLocal); // Lê a opção do usuário
        getchar(); // Limpa o buffer do teclado

        switch (opcaoMenuLocal) {
            case 1:
                system("cls");
                menuCadastro(); // Vai para o menu de cadastros
                break;
            case 2:
                if (caixaAberto) { // Só permite vendas se o caixa estiver aberto
                    menuVendas();
                } else {
                    printf("Caixa fechado. Efetue a abertura do caixa primeiro.\n");
                    system("pause");
                }
                break;
            case 3:
                if (!caixaAberto) { // Só permite abrir se o caixa estiver fechado
                    menuAberturaCaixa();
                } else {
                    printf("Caixa ja esta aberto.\n");
                    system("pause");
                }
                break;
            case 4:
                if (caixaAberto) { // Só permite fechar se o caixa estiver aberto
                    menuFechamentoCaixa();
                } else {
                    printf("Caixa ja esta fechado.\n");
                    system("pause");
                }
                break;
            case 5:
                menuRelatorios(); // Vai para o menu de relatórios
                break;
            case 6:
                if (caixaAberto) { // Não permite sair se o caixa estiver aberto
                    printf("Caixa ainda aberto. Realize o fechamento primeiro.\n");
                    system("pause");
                } else {
                    // Salva todos os dados em arquivos antes de sair
                    salvarProdutos();
                    salvarClientes(clientes, quantidadeClientes);
                    salvarCategorias();

                    // Libera a memória que foi alocada dinamicamente
                    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
                        free(sistemaProdutos.produtos[i].categoriaProduto); // Libera categoria de cada produto
                    }
                    free(sistemaProdutos.produtos); // Libera o array de produtos
                    sistemaProdutos.produtos = NULL;
                    sistemaProdutos.quantidade = 0;
                    free(clientes); // Libera o array de clientes
                    clientes = NULL;
                    liberarCategorias(); // Libera as categorias

                    printf("Saindo do sistema...\n");
                    system("pause");
                    exit(0); // Encerra o programa
                }
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                system("pause");
        }
    } while (opcaoMenuLocal != 6); // Continua rodando até o usuário escolher sair (opção 6)
}

/*
 * Salva a lista de clientes em um arquivo binário chamado "clientes.dat".
 * Isso permite que os dados persistam mesmo depois que o programa é fechado.
 */
void salvarClientes(const Cliente *clientes, int quantidade) {
    FILE *arquivo = fopen("clientes.dat", "wb"); // Abre o arquivo para escrita binária
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de clientes para escrita!\n");
        return;
    }
    fwrite(&quantidade, sizeof(int), 1, arquivo); // Salva a quantidade de clientes
    for (int i = 0; i < quantidade; i++) {
        fwrite(&clientes[i], sizeof(Cliente), 1, arquivo); // Salva cada cliente
    }
    fclose(arquivo); // Fecha o arquivo
}

/*
 * Carrega a lista de clientes de um arquivo binário "clientes.dat".
 * A memória para os clientes é alocada dinamicamente conforme necessário.
 */
void carregarClientes(Cliente **clientes, int *quantidade) {
    FILE *arquivo = fopen("clientes.dat", "rb"); // Abre o arquivo para leitura binária
    if (arquivo == NULL) {
        *clientes = NULL;   // Se o arquivo não existe, não há clientes
        *quantidade = 0;
        return;
    }
    fread(quantidade, sizeof(int), 1, arquivo); // Lê a quantidade de clientes
    // Aloca memória para todos os clientes que serão lidos
    *clientes = (Cliente*)malloc(*quantidade * sizeof(Cliente));
    if (*clientes == NULL) {
        printf("Erro ao alocar memoria para clientes!\n");
        fclose(arquivo);
        *quantidade = 0;
        return;
    }
    for (int i = 0; i < *quantidade; i++) {
        fread(&(*clientes)[i], sizeof(Cliente), 1, arquivo); // Lê cada cliente
    }
    fclose(arquivo); // Fecha o arquivo
}

/*
 * Exibe o menu de opções de cadastro (cliente ou produto).
 * Permite ao usuário escolher o que deseja cadastrar.
 */
void menuCadastro() {
    // Estas variáveis estáticas mantêm os clientes carregados uma única vez.
    static Cliente *clientes_cad = NULL;
    static int quantidadeClientes_cad = 0;
    if (clientes_cad == NULL) {
        carregarClientes(&clientes_cad, &quantidadeClientes_cad);
    }

    // Ponteiros locais para produtos, que na verdade manipulam a estrutura global sistemaProdutos
    Produto *produtos_locais_ptr = sistemaProdutos.produtos;
    int quantidade_produtos_local = sistemaProdutos.quantidade;

    int opcaoMenuCadastro = 0;
    while (opcaoMenuCadastro != 3) {
        system("cls");
        printf("|=======================================================|\n");
        printf("|\t\t    MENU CADASTROS\t\t\t|\n");
        printf("|=======================================================|\n");
        printf("| -> 1 - CADASTRAR CLIENTE \t\t\t\t|\n");
        printf("| -> 2 - CADASTRAR PRODUTO \t\t\t\t|\n");
        printf("| -> 3 - VOLTAR AO MENU PRINCIPAL \t\t\t|\n");
        printf("|=======================================================|\n");
        printf("Opcao: ");

        scanf("%d", &opcaoMenuCadastro);
        getchar(); // Limpa o buffer

        switch (opcaoMenuCadastro) {
            case 1:
                cadastrarCliente(&clientes_cad, &quantidadeClientes_cad); // Chama a função de cadastro de cliente
                salvarClientes(clientes_cad, quantidadeClientes_cad); // Salva clientes após o cadastro
                break;
            case 2:
                cadastrarProduto(&produtos_locais_ptr, &quantidade_produtos_local); // Chama a função de cadastro de produto
                break;
            case 3:
                return; // Volta ao menu principal
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }
    }
}

/*
 * Permite ao usuário cadastrar um novo cliente.
 * Solicita os dados do cliente e os adiciona ao array de clientes,
 * realocando a memória se necessário.
 * O CPF é validado antes de ser salvo.
 */
void cadastrarCliente(Cliente **clientes, int *quantidadeClientes) {
    // Tenta aumentar o espaço para mais um cliente
    Cliente *temp = realloc(*clientes, (*quantidadeClientes + 1) * sizeof(Cliente));
    if (temp == NULL) {
        printf("Erro ao alocar memoria para novo cliente!\n");
        return;
    }
    *clientes = temp; // Atualiza o ponteiro para o novo bloco de memória

    Cliente *novoCliente = &((*clientes)[*quantidadeClientes]); // Pega o endereço do novo cliente
    memset(novoCliente, 0, sizeof(Cliente)); // Limpa a memória do novo cliente
    novoCliente->codigo = *quantidadeClientes + 1; // Atribui um código sequencial

    printf("\n=== CADASTRO DE CLIENTE ===\n");
    printf("Nome Completo: ");
    fgets(novoCliente->nomeCompleto, sizeof(novoCliente->nomeCompleto), stdin);
    novoCliente->nomeCompleto[strcspn(novoCliente->nomeCompleto, "\n")] = '\0'; // Remove o '\n'

    printf("Nome Social (opcional): ");
    fgets(novoCliente->nomeSocial, sizeof(novoCliente->nomeSocial), stdin);
    novoCliente->nomeSocial[strcspn(novoCliente->nomeSocial, "\n")] = '\0';

    bool cpfValido = false;
    while (!cpfValido) { // Loop para validar o CPF
        printf("CPF: ");
        fgets(novoCliente->cpf, sizeof(novoCliente->cpf), stdin);
        novoCliente->cpf[strcspn(novoCliente->cpf, "\n")] = '\0';
        cpfValido = validarCPF(novoCliente->cpf);
        if (!cpfValido) {
            printf("\nCPF INVALIDO, DIGITE NOVAMENTE!\n");
        }
    }
    printf("Endereco: ");
    fgets(novoCliente->endereco, sizeof(novoCliente->endereco), stdin);
    novoCliente->endereco[strcspn(novoCliente->endereco, "\n")] = '\0';
    printf("Bairro: ");
    fgets(novoCliente->bairro, sizeof(novoCliente->bairro), stdin);
    novoCliente->bairro[strcspn(novoCliente->bairro, "\n")] = '\0';
    printf("Celular: ");
    fgets(novoCliente->celular, sizeof(novoCliente->celular), stdin);
    novoCliente->celular[strcspn(novoCliente->celular, "\n")] = '\0';

    (*quantidadeClientes)++; // Aumenta o número total de clientes

    printf("\nCliente cadastrado com sucesso! Codigo: %d\n", novoCliente->codigo);
    system("pause");
}

/*
 * Tenta adicionar uma nova categoria à lista global de categorias.
 * Não adiciona se a categoria já existir.
 * Retorna 'true' se adicionou, 'false' caso contrário.
 */
bool adicionarCategoria(const char *categoria) {
    if (categoriaExiste(categoria)) { // Verifica se a categoria já está na lista
        return false;
    }
    // Aumenta o espaço para mais uma categoria
    char **temp = realloc(categoriasGlobais.categorias, (categoriasGlobais.quantidade + 1) * sizeof(char *));
    if (temp == NULL) {
        printf("Erro ao alocar memoria para categorias.\n");
        return false;
    }
    categoriasGlobais.categorias = temp;
    // Duplica a string da categoria para armazenar no array
    categoriasGlobais.categorias[categoriasGlobais.quantidade] = strdup(categoria);
    if (categoriasGlobais.categorias[categoriasGlobais.quantidade] == NULL) {
        printf("Erro ao alocar memoria para nova categoria.\n");
        return false;
    }
    categoriasGlobais.quantidade++; // Aumenta o contador de categorias
    return true;
}

/*
 * Verifica se uma categoria com o nome dado já existe na lista global.
 * Retorna 'true' se encontrar, 'false' caso contrário.
 */
bool categoriaExiste(const char *categoria) {
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        if (strcmp(categoriasGlobais.categorias[i], categoria) == 0) { // Compara os nomes
            return true;
        }
    }
    return false;
}

/*
 * Exibe todas as categorias de produtos que estão cadastradas no sistema.
 */
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

/*
 * Libera toda a memória que foi alocada para as categorias.
 * Importante para evitar "vazamento de memória" quando o programa termina.
 */
void liberarCategorias() {
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        free(categoriasGlobais.categorias[i]); // Libera a memória de cada nome de categoria
    }
    free(categoriasGlobais.categorias); // Libera o array de ponteiros
    categoriasGlobais.categorias = NULL;
    categoriasGlobais.quantidade = 0;
}

/*
 * Salva a lista de categorias em um arquivo binário "categorias.dat".
 */
void salvarCategorias() {
    FILE *arquivo = fopen("categorias.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de categorias para escrita!\n");
        return;
    }
    fwrite(&categoriasGlobais.quantidade, sizeof(int), 1, arquivo); // Salva a quantidade
    for (int i = 0; i < categoriasGlobais.quantidade; i++) {
        size_t tamanho = strlen(categoriasGlobais.categorias[i]) + 1; // Pega o tamanho da string
        fwrite(&tamanho, sizeof(size_t), 1, arquivo); // Salva o tamanho
        fwrite(categoriasGlobais.categorias[i], sizeof(char), tamanho, arquivo); // Salva a string
    }
    fclose(arquivo);
}

/*
 * Salva a lista de produtos em um arquivo binário "produtos.dat".
 */
void salvarProdutos() {
    FILE *arquivo = fopen("produtos.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de produtos para escrita!\n");
        return;
    }

    fwrite(&sistemaProdutos.quantidade, sizeof(int), 1, arquivo); // Salva a quantidade de produtos

    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
        Produto *p = &sistemaProdutos.produtos[i];

        // Salva cada campo do produto
        fwrite(&p->codigo, sizeof(p->codigo), 1, arquivo);
        fwrite(p->nomeProduto, sizeof(p->nomeProduto), 1, arquivo);
        fwrite(&p->precoCompra, sizeof(p->precoCompra), 1, arquivo);
        fwrite(&p->percentual, sizeof(p->percentual), 1, arquivo);
        fwrite(&p->precoVenda, sizeof(p->precoVenda), 1, arquivo);
        fwrite(&p->estoque, sizeof(p->estoque), 1, arquivo);
        fwrite(&p->estoqueMinimo, sizeof(p->estoqueMinimo), 1, arquivo);

        // Salva o tamanho da categoria e a categoria em si (pois é uma string alocada dinamicamente)
        if (p->categoriaProduto != NULL) {
            size_t tamanho = strlen(p->categoriaProduto) + 1;
            fwrite(&tamanho, sizeof(size_t), 1, arquivo);
            fwrite(p->categoriaProduto, sizeof(char), tamanho, arquivo);
        } else {
            size_t tamanho = 0; // Se não tem categoria, salva tamanho 0
            fwrite(&tamanho, sizeof(size_t), 1, arquivo);
        }
    }
    fclose(arquivo);
}

/*
 * Carrega a lista de categorias de um arquivo binário.
 * Se já houver categorias na memória, elas são liberadas antes.
 */
void carregarCategorias() {
    FILE *arquivo = fopen("categorias.dat", "rb");
    if (arquivo == NULL) {
        return; // Arquivo não existe, sem categorias para carregar
    }
    liberarCategorias(); // Libera a memória das categorias atuais antes de carregar novas

    int quantidade;
    fread(&quantidade, sizeof(int), 1, arquivo); // Lê a quantidade de categorias no arquivo

    for (int i = 0; i < quantidade; i++) {
        size_t tamanho;
        fread(&tamanho, sizeof(size_t), 1, arquivo); // Lê o tamanho da string da categoria
        char *categoria = (char *)malloc(tamanho * sizeof(char)); // Aloca memória para a string
        if (categoria == NULL) {
            printf("Erro ao alocar memoria para categoria!\n");
            break;
        }
        fread(categoria, sizeof(char), tamanho, arquivo); // Lê a string da categoria

        // Adiciona a categoria lida à lista global
        char **temp = realloc(categoriasGlobais.categorias,
                            (categoriasGlobais.quantidade + 1) * sizeof(char *));
        if (temp == NULL) {
            free(categoria); // Libera a categoria que acabamos de ler se não houver memória para o array
            break;
        }
        categoriasGlobais.categorias = temp;
        categoriasGlobais.categorias[categoriasGlobais.quantidade++] = categoria; // Adiciona e incrementa o contador
    }
    fclose(arquivo);
}

/*
 * Carrega a lista de produtos de um arquivo binário.
 * Libera a memória dos produtos atuais antes de carregar os novos.
 */
void carregarProdutos() {
    FILE *arquivo = fopen("produtos.dat", "rb");
    if (arquivo == NULL) {
        return; // Arquivo não existe, sem produtos para carregar
    }

    // Libera a memória de produtos e suas categorias antes de carregar novos
    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
        free(sistemaProdutos.produtos[i].categoriaProduto);
    }
    free(sistemaProdutos.produtos);
    sistemaProdutos.produtos = NULL;
    sistemaProdutos.quantidade = 0;

    int quantidade_no_arquivo;
    fread(&quantidade_no_arquivo, sizeof(int), 1, arquivo); // Lê a quantidade de produtos no arquivo

    for (int i = 0; i < quantidade_no_arquivo; i++) {
        // Aumenta o espaço para mais um produto
        Produto *temp = realloc(sistemaProdutos.produtos,
                              (sistemaProdutos.quantidade + 1) * sizeof(Produto));
        if (temp == NULL) {
            printf("Erro ao alocar memoria para produtos!\n");
            break;
        }
        sistemaProdutos.produtos = temp;
        Produto *novo = &sistemaProdutos.produtos[sistemaProdutos.quantidade]; // Pega o endereço do novo produto
        memset(novo, 0, sizeof(Produto)); // Limpa a memória do novo produto

        // Lê cada campo do produto do arquivo
        fread(&novo->codigo, sizeof(novo->codigo), 1, arquivo);
        fread(novo->nomeProduto, sizeof(novo->nomeProduto), 1, arquivo);
        fread(&novo->precoCompra, sizeof(novo->precoCompra), 1, arquivo);
        fread(&novo->percentual, sizeof(novo->percentual), 1, arquivo);
        fread(&novo->precoVenda, sizeof(novo->precoVenda), 1, arquivo);
        fread(&novo->estoque, sizeof(novo->estoque), 1, arquivo);
        fread(&novo->estoqueMinimo, sizeof(novo->estoqueMinimo), 1, arquivo);

        size_t tamanho_categoria;
        fread(&tamanho_categoria, sizeof(size_t), 1, arquivo); // Lê o tamanho da string da categoria

        if (tamanho_categoria > 0) {
            novo->categoriaProduto = (char *)malloc(tamanho_categoria * sizeof(char)); // Aloca memória para a categoria
            if (novo->categoriaProduto == NULL) {
                printf("Erro ao alocar memoria para categoria do produto!\n");
                continue;
            }
            fread(novo->categoriaProduto, sizeof(char), tamanho_categoria, arquivo); // Lê a categoria
        } else {
            novo->categoriaProduto = NULL; // Se o tamanho é 0, não há categoria
        }
        sistemaProdutos.quantidade++; // Incrementa o contador de produtos
    }
    fclose(arquivo);
}

/*
 * Permite ao usuário cadastrar um novo produto.
 * O usuário pode escolher uma categoria existente ou criar uma nova.
 * Calcula o preço de venda com base no preço de compra e percentual de lucro.
 * Define o estoque e o estoque mínimo.
 * Salva os produtos e categorias em arquivos após o cadastro.
 */
void cadastrarProduto(Produto **produtos_param, int *quantidadeProdutos_param) {
    // Tenta aumentar o espaço para mais um produto no array global
    Produto *temp = realloc(sistemaProdutos.produtos, (sistemaProdutos.quantidade + 1) * sizeof(Produto));
    if (temp == NULL) {
        printf("Erro ao alocar memoria para novo produto!\n");
        return;
    }
    sistemaProdutos.produtos = temp; // Atualiza o ponteiro global

    Produto *novo = &sistemaProdutos.produtos[sistemaProdutos.quantidade]; // Pega o endereço do novo produto
    memset(novo, 0, sizeof(Produto)); // Limpa a memória do novo produto

    novo->codigo = sistemaProdutos.quantidade + 1; // Atribui um código sequencial

    printf("\n=== CADASTRO DE PRODUTO ===\n");
    printf("Nome do Produto: ");
    fgets(novo->nomeProduto, sizeof(novo->nomeProduto), stdin);
    novo->nomeProduto[strcspn(novo->nomeProduto, "\n")] = '\0'; // Remove o '\n'

    int opcaoCategoria;
    char nomeCategoria[100];
    bool categoriaDefinida = false; // Flag para saber se a categoria foi definida

    while (!categoriaDefinida) {
        system("cls");
        printf("\n=== SELECIONE UMA CATEGORIA ===\n");
        exibirCategorias(); // Mostra as categorias existentes
        printf("\n1 - Selecionar categoria existente\n");
        printf("2 - Criar nova categoria\n");
        printf("0 - Cancelar cadastro de produto\n");
        printf("Opcao: ");
        scanf("%d", &opcaoCategoria);
        getchar(); // Limpa o buffer

        switch (opcaoCategoria) {
            case 1: { // Escolher categoria existente
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
                    // Copia o nome da categoria selecionada
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
            case 2: { // Criar nova categoria
                printf("\nNome da nova categoria: ");
                fgets(nomeCategoria, sizeof(nomeCategoria), stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = '\0';
                bool nomeValido = true;
                // Validação simples para o nome da categoria (apenas letras, números e espaços)
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
                if (!categoriaExiste(nomeCategoria)) { // Verifica se a categoria já existe
                    if (adicionarCategoria(nomeCategoria)) { // Tenta adicionar a nova categoria
                        novo->categoriaProduto = strdup(nomeCategoria); // Copia o nome para o produto
                        if (novo->categoriaProduto == NULL) {
                            printf("Erro ao alocar memoria para categoria!\n");
                            break;
                        }
                        salvarCategorias(); // Salva a nova categoria no arquivo
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
                printf("Cadastro de produto cancelado.\n");
                return; // Volta sem cadastrar o produto
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }
    }

    // Leitura e validação do Preço de Compra (deve ser positivo)
    do {
        printf("\nPreco de Compra: R$ ");
        if (scanf("%f", &novo->precoCompra) != 1 || novo->precoCompra <= 0) {
            printf("Valor incorreto! Digite um valor positivo.\n");
            while (getchar() != '\n'); // Limpa o buffer
            continue;
        }
        break;
    } while (1);
    getchar(); // Limpa o buffer

    // Leitura e validação da Margem de Lucro (não negativo)
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

    // Calcula o preço de venda
    novo->precoVenda = novo->precoCompra * (1 + novo->percentual / 100);
    printf("\nPreco de Venda Calculado: R$ %.2f\n", novo->precoVenda);

    // Leitura e validação da Quantidade em Estoque (não negativo)
    do {
        printf("\nQuantidade em Estoque: ");
        if (scanf("%d", &novo->estoque) != 1 || novo->estoque < 0) {
            printf("Valor incorreto! Digite um numero inteiro nao negativo.\n");
            while (getchar() != '\n');
            continue;
        }
        break;
    } while (1);
    getchar();

    // Leitura e validação do Estoque Mínimo (não negativo)
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

    sistemaProdutos.quantidade++; // Aumenta o contador global de produtos

    // Atualiza os ponteiros passados como parâmetro (se for o caso)
    *produtos_param = sistemaProdutos.produtos;
    *quantidadeProdutos_param = sistemaProdutos.quantidade;

    salvarProdutos(); // Salva todos os produtos no arquivo

    printf("\nProduto cadastrado com sucesso! Codigo: %d\n", novo->codigo);
    system("pause");
}

/*
 * Exibe o menu de vendas, mostrando os produtos disponíveis.
 * Permite iniciar uma nova venda ou fazer uma retirada de caixa (sangria).
 */
void menuVendas() {
    while (1) {
        system("cls");
        printf("|====================================================================|\n");
        printf("|\t\t    MENU VENDAS\t\t\t\t\t     |\n");
        printf("|====================================================================|\n");

        if (sistemaProdutos.quantidade > 0) {
            printf("|PRODUTOS DISPONIVEIS:\t\t\t\t\t\t     |\n");
            printf("|====================================================================|\n");
            printf("|COD   | NOME                     | CATEGORIA    | PRECO   | ESTOQUE |\n");
            printf("|======|==========================|==============|=========|=========|\n");
            for (int i = 0; i < sistemaProdutos.quantidade; i++) {
                printf("|%-5d | %-24.24s | %-12.12s | %-7.2f | %-8d|\n",
                      sistemaProdutos.produtos[i].codigo,
                      sistemaProdutos.produtos[i].nomeProduto,
                      sistemaProdutos.produtos[i].categoriaProduto ? sistemaProdutos.produtos[i].categoriaProduto : "N/A",
                      sistemaProdutos.produtos[i].precoVenda,
                      sistemaProdutos.produtos[i].estoque);
                printf("|------|--------------------------|--------------|---------|---------|\n");
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
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                if (sistemaProdutos.quantidade > 0) { // Só inicia venda se houver produtos
                    menu_novaVenda();
                } else {
                    printf("\nNao ha produtos cadastrados para vender!\n");
                    system("pause");
                }
                break;
            case 2:
                if (caixaAberto) { // Só faz sangria se o caixa estiver aberto
                    retiradaCaixa(&totalCaixa);
                } else {
                    printf("\nCaixa fechado. Efetue a abertura primeiro.\n");
                    system("pause");
                }
                break;
            case 3:
                return; // Volta ao menu principal
            default:
                printf("\nOpcao invalida!\n");
                system("pause");
        }
    }
}

/*
 * Inicia o processo de uma nova venda.
 * Permite adicionar produtos ao carrinho, verifica estoque, e aplica desconto.
 * Em seguida, chama a função de pagamento para finalizar a transação.
 */
void menu_novaVenda() {
    if (sistemaProdutos.quantidade == 0) {
        printf("NAO HA PRODUTOS PARA VENDER!\n");
        system("pause");
        return;
    }

    ItemVenda carrinho[MAX_ITENS_CARRINHO]; // Cria um carrinho temporário para esta venda
    int numItensLocal = 0;   // Contador de itens no carrinho
    float totalVendaLocal = 0; // Soma total dos produtos no carrinho

    int numeroDaVendaAtual = proximoNumeroVenda; // Usa o próximo número de venda disponível

    while (1) {
        system("cls"); // Limpa a tela para exibir o carrinho e produtos
        printf("|====================================================================|\n");
        printf("|PRODUTOS DISPONIVEIS:\t\t\t\t\t\t     |\n");
        printf("|====================================================================|\n");
        printf("|COD   | NOME                     | CATEGORIA    | PRECO   | ESTOQUE |\n");
        printf("|======|==========================|==============|=========|=========|\n");
        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
             printf("|%-5d | %-24.24s | %-12.12s | %-7.2f | %-8d|\n",
                   sistemaProdutos.produtos[i].codigo,
                   sistemaProdutos.produtos[i].nomeProduto,
                   sistemaProdutos.produtos[i].categoriaProduto ? sistemaProdutos.produtos[i].categoriaProduto : "N/A",
                   sistemaProdutos.produtos[i].precoVenda,
                   sistemaProdutos.produtos[i].estoque);
            printf("|------|--------------------------|--------------|---------|---------|\n");
        }

        printf("\n| CARRINHO DE COMPRAS - VENDA #%d |\n", numeroDaVendaAtual);
        printf("|====================================================================|\n");
        printf("|COD   | DESCRICAO              | QTD | PRECO UN. | SUBTOTAL         |\n");
        printf("|======|========================|=====|===========|==================|\n");
        // Exibe os itens já adicionados ao carrinho
        for (int i = 0; i < numItensLocal; i++) {
            printf("|%-5d | %-22.22s | %-3d | R$ %-7.2f | R$ %-12.2f |\n",
                   carrinho[i].codigoProduto,
                   carrinho[i].descricao,
                   carrinho[i].quantidade,
                   carrinho[i].precoUnitario,
                   carrinho[i].subtotal);
            printf("|------|------------------------|-----|-----------|-----------------|\n");
        }
        printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVendaLocal);
        printf("|====================================================================|\n");

        int codigoProduto, quantidade;
        printf("\nInforme o codigo do produto (0 para finalizar selecao de produtos): ");
        if (scanf("%d", &codigoProduto) != 1) { // Lê o código do produto
            printf("Codigo invalido!\n");
            while (getchar() != '\n'); // Limpa o buffer
            system("pause");
            continue; // Volta para o início do loop
        }
        getchar();

        if (codigoProduto == 0) { // Se o usuário digitar 0, finaliza a seleção de produtos
            if (numItensLocal == 0) {
                printf("Nenhum item no carrinho. Venda cancelada.\n");
                system("pause");
                return; // Volta ao menu anterior
            }
            break; // Sai do loop para ir para a etapa de pagamento
        }

        printf("Informe a Quantidade: ");
        if (scanf("%d", &quantidade) != 1 || quantidade <= 0) { // Lê a quantidade
            printf("Quantidade invalida!\n");
            while (getchar() != '\n');
            system("pause");
            continue;
        }
        getchar();

        bool produtoEncontrado = false;
        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
            if (sistemaProdutos.produtos[i].codigo == codigoProduto) { // Encontra o produto pelo código
                produtoEncontrado = true;
                if (sistemaProdutos.produtos[i].estoque < quantidade) { // Verifica o estoque
                    printf("Estoque insuficiente! Disponivel: %d\n", sistemaProdutos.produtos[i].estoque);
                    system("pause");
                    break;
                }
                if (numItensLocal >= MAX_ITENS_CARRINHO) { // Verifica se o carrinho está cheio
                    printf("Carrinho cheio! Nao e possivel adicionar mais itens nesta venda.\n");
                    system("pause");
                    break;
                }

                // Adiciona o produto ao carrinho local
                carrinho[numItensLocal].codigoProduto = codigoProduto;
                strncpy(carrinho[numItensLocal].descricao, sistemaProdutos.produtos[i].nomeProduto, sizeof(carrinho[numItensLocal].descricao) - 1);
                carrinho[numItensLocal].descricao[sizeof(carrinho[numItensLocal].descricao) - 1] = '\0';
                carrinho[numItensLocal].precoUnitario = sistemaProdutos.produtos[i].precoVenda;
                carrinho[numItensLocal].quantidade = quantidade;
                carrinho[numItensLocal].subtotal = sistemaProdutos.produtos[i].precoVenda * quantidade;

                totalVendaLocal += carrinho[numItensLocal].subtotal; // Atualiza o total da venda
                numItensLocal++; // Incrementa o número de itens no carrinho

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

    if (numItensLocal > 0) { // Se há itens no carrinho, procede para o pagamento
        char opcaoDesconto;
        float totalComDesconto = totalVendaLocal;

        system("cls");
        printf("\n| CARRINHO DE COMPRAS - VENDA #%d |\n", numeroDaVendaAtual);
        printf("|====================================================================|\n");
        printf("|COD   | DESCRICAO              | QTD | PRECO UN. | SUBTOTAL         |\n");
        printf("|======|========================|=====|===========|==================|\n");
        for (int i = 0; i < numItensLocal; i++) {
            printf("|%-5d | %-22.22s | %-3d | R$ %-7.2f | R$ %-12.2f |\n",
                   carrinho[i].codigoProduto,
                   carrinho[i].descricao,
                   carrinho[i].quantidade,
                   carrinho[i].precoUnitario,
                   carrinho[i].subtotal);
            printf("|------|------------------------|-----|-----------|-----------------|\n");
        }
        printf("| TOTAL DA VENDA: R$ %-40.2f |\n", totalVendaLocal);
        printf("|====================================================================|\n");

        printf("Deseja aplicar desconto de 5%%? (s/n): ");
        scanf(" %c", &opcaoDesconto); // Lê a opção de desconto
        getchar();

        if (tolower(opcaoDesconto) == 's') { // Aplica o desconto se o usuário desejar
            totalComDesconto = totalVendaLocal * 0.95f; // 5% de desconto
            printf("Desconto aplicado. Novo total: R$ %.2f\n", totalComDesconto);
            system("pause");
        }

        // Chama a função de pagamento, passando o valor final e o carrinho
        menu_pagamento(totalComDesconto, carrinho, numItensLocal);
    }
}

/*
 * Gerencia o processo de pagamento de uma venda.
 * Permite pagamentos em dinheiro e cartão, calcula o troco.
 * Após a conclusão do pagamento, atualiza o estoque dos produtos vendidos.
 */
void menu_pagamento(float valorAPagarDaVenda, ItemVenda carrinhoDaVenda[], int numItensCarrinho) {
    float totalPagoDinheiroNestaVenda = 0; // Quanto foi pago em dinheiro nesta venda específica
    float totalPagoCartaoNestaVenda = 0;   // Quanto foi pago em cartão nesta venda específica
    float restanteAPagar = valorAPagarDaVenda; // Valor que ainda precisa ser pago
    int opcaoPagamento;
    bool vendaConcluida = false; // Sinaliza se a venda foi finalizada

    do {
        system("cls");
        printf("|====================================================================|\n");
        printf("| TOTAL A PAGAR: R$ %.2f                                          |\n", restanteAPagar);
        // Mostra o que já foi pago, se houver
        if ((totalPagoDinheiroNestaVenda + totalPagoCartaoNestaVenda) > 0) {
            printf("| JA PAGO DINHEIRO: R$ %.2f                                        |\n", totalPagoDinheiroNestaVenda);
            printf("| JA PAGO CARTAO: R$ %.2f                                         |\n", totalPagoCartaoNestaVenda);
        }
        printf("|====================================================================|\n");
        printf("| FORMAS DE PAGAMENTO:\t\t\t\t\t     |\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("| 1. CARTAO\t\t\t\t\t\t     |\n");
        printf("| 2. DINHEIRO\t\t\t\t\t\t     |\n");
        printf("| 3. CANCELAR PAGAMENTO/VENDA\t\t\t\t     |\n");
        printf("|--------------------------------------------------------------------|\n");
        printf("OPCAO: ");

        if (scanf("%d", &opcaoPagamento) != 1) { // Lê a opção de pagamento
            printf("Opcao invalida!\n");
            while (getchar() != '\n');
            system("pause");
            continue; // Volta ao início do loop
        }
        getchar();

        switch(opcaoPagamento) {
            case 1: { // Pagamento com Cartão
                float valorPagamentoCartao;
                printf("Informe o valor a ser pago no cartao (ou 0 para pagar o total restante R$ %.2f): R$ ", restanteAPagar);
                if (scanf("%f", &valorPagamentoCartao) != 1 || valorPagamentoCartao < 0) {
                    printf("Valor invalido!\n");
                    while(getchar()!='\n'); system("pause"); continue;
                }
                getchar();

                if (valorPagamentoCartao == 0) valorPagamentoCartao = restanteAPagar; // Se 0, paga o restante total
                // Pequena tolerância para comparar valores decimais
                if (valorPagamentoCartao > restanteAPagar + 0.009) {
                     printf("Valor no cartao (R$ %.2f) maior que o devido (R$ %.2f)!\n", valorPagamentoCartao, restanteAPagar);
                     system("pause"); continue;
                }

                printf("Processando R$ %.2f no cartao...\n", valorPagamentoCartao);
                printf("Pagamento com cartao APROVADO!\n"); // Simulação de aprovação
                totalPagoCartaoNestaVenda += valorPagamentoCartao;
                restanteAPagar -= valorPagamentoCartao; // Diminui o que falta pagar
                system("pause");
                break;
            }
            case 2: { // Pagamento em Dinheiro
                float valorRecebido;
                printf("Informe o valor recebido em dinheiro: R$ ");
                if (scanf("%f", &valorRecebido) != 1 || valorRecebido <= 0) {
                    printf("Valor invalido!\n");
                    while (getchar() != '\n');
                    system("pause");
                    continue;
                }
                getchar();

                if (valorRecebido >= restanteAPagar) { // Se o valor recebido é suficiente ou maior
                    float troco = valorRecebido - restanteAPagar;

                    // Verifica se o caixa tem dinheiro para o troco
                    if (troco > totalCaixa) {
                        printf("Caixa nao tem troco suficiente (R$ %.2f)! Disponivel: R$ %.2f\n", troco, totalCaixa);
                        system("pause");
                        continue;
                    }
                    totalPagoDinheiroNestaVenda += restanteAPagar; // Adiciona o valor exato da dívida
                    totalCaixa += restanteAPagar;                  // Adiciona o valor exato da dívida ao caixa
                    totalCaixa -= troco;                           // Subtrai o troco do caixa
                    restanteAPagar = 0;                            // Nada mais a pagar
                    printf("Troco: R$ %.2f\n", troco);
                } else { // Pagamento parcial em dinheiro
                    totalPagoDinheiroNestaVenda += valorRecebido;
                    totalCaixa += valorRecebido; // Adiciona o valor recebido ao caixa
                    restanteAPagar -= valorRecebido; // Diminui o que falta pagar
                    printf("Valor recebido. Falta pagar: R$ %.2f\n", restanteAPagar);
                }
                system("pause");
                break;
            }
            case 3: // Cancelar Pagamento/Venda
                printf("Pagamento/Venda cancelada pelo usuario.\n");
                // Devolve ao caixa o que foi pago em dinheiro nesta venda antes de cancelar
                totalCaixa -= totalPagoDinheiroNestaVenda;
                system("pause");
                return; // Sai da função, a venda não é concluída
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }

        // Se o valor restante a pagar é muito pequeno, considera a venda paga
        if (restanteAPagar < 0.01f) {
            restanteAPagar = 0;
            vendaConcluida = true;
            break; // Sai do loop de pagamento
        }
    } while (opcaoPagamento != 3 && !vendaConcluida);

    // Se a venda foi concluída com sucesso
    if (vendaConcluida) {
        system("cls");
        printf("|====================================================================|\n");
        printf("| VENDA #%d FINALIZADA COM SUCESSO!\t\t\t\t     |\n", proximoNumeroVenda);
        printf("|====================================================================|\n");
        printf("| TOTAL ORIGINAL DA VENDA: R$ %-33.2f |\n", valorAPagarDaVenda);
        printf("| PAGO EM DINHEIRO: R$ %-40.2f |\n", totalPagoDinheiroNestaVenda);
        printf("| PAGO EM CARTAO: R$ %-42.2f |\n", totalPagoCartaoNestaVenda);
        printf("|====================================================================|\n");

        // Atualiza os totais globais do dia para relatórios
        totalVendas += valorAPagarDaVenda;
        totalDinheiroF += totalPagoDinheiroNestaVenda;
        totalCartaoF += totalPagoCartaoNestaVenda;

        // Processa a baixa de estoque dos produtos vendidos
        printf("\nProcessando baixa de estoque...\n");
        for (int i = 0; i < numItensCarrinho; i++) {
            bool produtoEncontrado = false;
            for (int j = 0; j < sistemaProdutos.quantidade; j++) {
                if (carrinhoDaVenda[i].codigoProduto == sistemaProdutos.produtos[j].codigo) {
                    // Garante que o estoque não fique negativo
                    if (sistemaProdutos.produtos[j].estoque >= carrinhoDaVenda[i].quantidade) {
                        sistemaProdutos.produtos[j].estoque -= carrinhoDaVenda[i].quantidade;
                        printf("  - Baixa: Produto '%s' (Cod: %d) - %d unidades. Novo estoque: %d\n",
                               sistemaProdutos.produtos[j].nomeProduto, sistemaProdutos.produtos[j].codigo,
                               carrinhoDaVenda[i].quantidade, sistemaProdutos.produtos[j].estoque);
                    } else {
                        printf("  - AVISO: Estoque insuficiente para %s (Cod: %d)! Solicitado %d, disponivel %d.\n",
                               sistemaProdutos.produtos[j].nomeProduto, sistemaProdutos.produtos[j].codigo,
                               carrinhoDaVenda[i].quantidade, sistemaProdutos.produtos[j].estoque);
                        sistemaProdutos.produtos[j].estoque = 0; // Zera o estoque se houver mais demanda que disponível
                    }
                    produtoEncontrado = true;
                    break;
                }
            }
            if (!produtoEncontrado) {
                printf("  - ERRO: Produto com codigo %d do carrinho nao encontrado no sistema de produtos!\n", carrinhoDaVenda[i].codigoProduto);
            }
        }
        salvarProdutos(); // Salva o estoque atualizado
        printf("Baixa de estoque concluida.\n");

        proximoNumeroVenda++; // Prepara para a próxima venda
        registrarLog("Venda finalizada com sucesso."); // Registra o evento no log
        system("pause");
    }
}

/*
 * Permite fazer uma retirada de dinheiro do caixa (sangria).
 * Verifica se o valor a ser retirado é válido e se não ultrapassa o saldo mínimo.
 * Atualiza o saldo do caixa e registra a operação.
 */
float retiradaCaixa(float *totalCaixaPtr) {
    const int MAX_TENTATIVAS = 3;
    int tentativas = 0;
    float valorRetirada;

    do {
        system("cls");
        printf("|===========================================================|\n");
        printf("|\t\t   RETIRADA DE CAIXA (SANGRIA)\t\t\t|\n");
        printf("|===========================================================|\n");
        printf("| SALDO ATUAL EM CAIXA: R$ %.2f\n", *totalCaixaPtr);
        printf("| SALDO MINIMO OBRIGATORIO: R$ %.2f\n", SALDO_MINIMO);
        printf("| DIGITE O VALOR A SER RETIRADO (0 para cancelar): R$ ");

        if (scanf("%f", &valorRetirada) != 1) { // Lê o valor da retirada
            printf("\n| VALOR INVALIDO! Digite apenas numeros.\n");
            while (getchar() != '\n');
            system("pause");
            tentativas++;
            if (tentativas >= MAX_TENTATIVAS) {
                printf("\n| MUITAS TENTATIVAS INVALIDAS. OPERACAO CANCELADA.\n");
                system("pause");
                return *totalCaixaPtr;
            }
            continue;
        }
        getchar();

        if (valorRetirada == 0) {
            printf("\n| OPERACAO CANCELADA PELO USUARIO.\n");
            system("pause");
            return *totalCaixaPtr;
        }
        if (valorRetirada < 0) {
            printf("\n| VALOR DEVE SER POSITIVO!\n");
            system("pause");
            tentativas++; continue;
        }
        if (valorRetirada > *totalCaixaPtr) { // Não pode retirar mais do que tem no caixa
            printf("\n| SALDO INSUFICIENTE!\n");
            printf("| SALDO ATUAL: R$ %.2f\n", *totalCaixaPtr);
            system("pause");
            tentativas++; continue;
        }
        // Verifica se a retirada não deixaria o caixa abaixo do saldo mínimo
        if ((*totalCaixaPtr - valorRetirada) < SALDO_MINIMO) {
            printf("\n| NAO E POSSIVEL RETIRAR ESTE VALOR!\n");
            printf("| SALDO MINIMO DE R$ %.2f DEVE SER MANTIDO.\n", SALDO_MINIMO);
            float maxRetirada = *totalCaixaPtr - SALDO_MINIMO;
            if (maxRetirada < 0) maxRetirada = 0;
            printf("| VALOR MAXIMO QUE PODE SER RETIRADO: R$ %.2f\n", maxRetirada);
            system("pause");
            tentativas++; continue;
        }

        printf("\n| CONFIRMAR RETIRADA DE R$ %.2f? (s/n): ", valorRetirada);
        char confirmacao = getchar();
        getchar();

        if (tolower(confirmacao) != 's') { // Se não confirmar, cancela
            printf("\n| OPERACAO CANCELADA.\n");
            system("pause");
            return *totalCaixaPtr;
        }

        *totalCaixaPtr -= valorRetirada; // Diminui o valor do caixa

        // Pega a data e hora atual para o log
        time_t now;
        time(&now);
        struct tm *local = localtime(&now);
        char msgLog[100];
        snprintf(msgLog, sizeof(msgLog), "Retirada de caixa (sangria) no valor de R$ %.2f. Novo saldo: R$ %.2f", valorRetirada, *totalCaixaPtr);
        registrarLog(msgLog); // Registra a operação no log

        printf("\n| RETIRADA REALIZADA COM SUCESSO!\n");
        printf("| DATA/HORA: %02d/%02d/%04d %02d:%02d\n",
               local->tm_mday, local->tm_mon+1, local->tm_year+1900,
               local->tm_hour, local->tm_min);
        printf("| VALOR RETIRADO: R$ %.2f\n", valorRetirada);
        printf("| NOVO SALDO: R$ %.2f\n", *totalCaixaPtr);
        printf("|===========================================================|\n");
        system("pause");
        return *totalCaixaPtr;

    } while (tentativas < MAX_TENTATIVAS);

    printf("\n| MUITAS TENTATIVAS INVALIDAS. OPERACAO CANCELADA.\n");
    system("pause");
    return *totalCaixaPtr;
}

/*
 * Gerencia o processo de abertura do caixa.
 * Solicita o valor inicial para o caixa e atualiza o estado do sistema.
 */
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

        if (scanf("%d", &opcaoAbre) != 1) { // Lê a opção do usuário
            while (getchar() != '\n');
            printf("Entrada invalida! Digite um numero.\n");
            system("pause");
            tentativas++;
            continue;
        }
        getchar();

        switch(opcaoAbre) {
            case 1: {
                if (caixaAberto) { // Verifica se o caixa já está aberto
                    printf("JA HA UM CAIXA EM ABERTO, REALIZE O FECHAMENTO\n");
                    system("pause");
                    return;
                }

                printf("REALIZANDO ABERTURA DE CAIXA...\n");
                Sleep(1000); // Pausa para simular carregamento
                system("cls");

                float valorAberturaTemp = -1.0f;
                int inputTentativas = 0;
                do {
                    system("cls");
                    printf("DIGITE O VALOR DE ABERTURA: \n");
                    printf("VALOR R$ ");
                    if (scanf("%f", &valorAberturaTemp) != 1) { // Lê o valor de abertura
                        while (getchar() != '\n');
                        printf("Valor invalido! Digite um numero.\n");
                        system("pause");
                        inputTentativas++;
                        if (inputTentativas >= MAX_TENTATIVAS) {
                            printf("Muitas tentativas invalidas. Retornando...\n");
                            system("pause"); return;
                        }
                        continue;
                    }
                    getchar();
                    if (valorAberturaTemp < 0) { // Valor de abertura não pode ser negativo
                        printf("VALOR DE ABERTURA INCORRETO, DIGITE UM VALOR POSITIVO\n");
                        system("pause");
                        inputTentativas++;
                        if (inputTentativas >= MAX_TENTATIVAS) {
                            printf("Muitas tentativas invalidas. Retornando...\n");
                            system("pause"); return;
                        }
                        valorAberturaTemp = -1.0f; // Reinicia a tentativa
                    }
                } while (valorAberturaTemp < 0);

                vAbre = valorAberturaTemp; // Guarda o valor de abertura
                totalCaixa = vAbre;        // Define o saldo inicial do caixa
                caixaAberto = 1;           // Marca o caixa como aberto

                system("cls");
                printf("ABERTURA REALIZADA COM SUCESSO!\n");
                printf("VALOR DE ABERTURA: R$ %.2f\n", vAbre);
                printf("SALDO ATUAL DO CAIXA: R$ %.2f\n", totalCaixa);

                char msgLog[100];
                snprintf(msgLog, sizeof(msgLog), "Caixa aberto com R$ %.2f", vAbre);
                registrarLog(msgLog); // Registra a abertura no log

                system("pause");
                return;
            }
            case 2:
                return; // Volta ao menu principal
            default:
                printf("OPCAO INVALIDA, TENTE NOVAMENTE\n");
                system("pause");
                tentativas++;
        }
    } while (tentativas < MAX_TENTATIVAS);

    printf("Muitas tentativas invalidas. Retornando ao menu principal...\n");
    system("pause");
}

/*
 * Gerencia o processo de fechamento do caixa.
 * Apresenta um resumo financeiro do dia e verifica a existência de divergências.
 * Permite fechar o caixa e zera os totais do dia para o próximo ciclo.
 */
void menuFechamentoCaixa() {
    int opcaoFechamento;
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

        if (scanf("%d", &opcaoFechamento) != 1) { // Lê a opção
            printf("Entrada invalida! Digite 1 ou 2.\n");
            while (getchar() != '\n');
            system("pause");
            tentativas++;
            if (tentativas >= MAX_TENTATIVAS) {
                printf("Muitas tentativas invalidas. Retornando...\n");
                system("pause"); return;
            }
            continue;
        }
        getchar();

        switch(opcaoFechamento) {
            case 1: {
                system("cls");
                if (!caixaAberto) { // Verifica se o caixa está aberto
                    printf("NAO HA CAIXA ABERTO, RETORNANDO...\n");
                    system("pause");
                    return;
                }

                printf("REALIZANDO FECHAMENTO DE CAIXA...\n");
                Sleep(1000); // Simula processamento
                system("cls");

                // Calcula o valor que deveria estar no caixa (abertura + dinheiro das vendas)
                float valorEsperadoEmCaixa = vAbre + totalDinheiroF;

                // Calcula a diferença entre o que está no caixa e o que era esperado
                float diferencaCaixa = totalCaixa - valorEsperadoEmCaixa;

                printf("|================ RESUMO DO DIA ================|\n");
                printf("| VENDAS REALIZADAS (HOJE): %-19d |\n", proximoNumeroVenda -1);
                printf("| FATURAMENTO TOTAL (HOJE): R$ %-19.2f |\n", totalVendas);
                printf("|-----------------------------------------------|\n");
                printf("| VALOR ABERTURA CAIXA: R$ %-20.2f |\n", vAbre);
                printf("| TOTAL DINHEIRO (VENDAS): R$ %-17.2f |\n", totalDinheiroF);
                printf("| TOTAL CARTAO (VENDAS): R$ %-18.2f |\n", totalCartaoF);
                printf("|===============================================|\n");
                printf("| VALOR ESPERADO EM CAIXA: R$ %-18.2f |\n", valorEsperadoEmCaixa);
                printf("| SALDO ATUAL EM CAIXA (APURADO): R$ %-11.2f |\n", totalCaixa);
                printf("|-----------------------------------------------|\n");
                printf("| DIFERENCA (APURADO - ESPERADO): R$ %-11.2f |\n", diferencaCaixa);
                printf("|===============================================|\n");

                // Se houver uma diferença significativa (maior que 1 centavo)
                if (fabsf(diferencaCaixa) > 0.01) { // fabsf compara valores decimais
                    printf("\nATENCAO: DIVERGENCIA ENCONTRADA NO CAIXA!\n");
                    if (diferencaCaixa > 0) {
                        printf("Possivel SOBRA em caixa ou sangria nao registrada corretamente.\n");
                    } else {
                        printf("Possivel FALTA em caixa, valor nao registrado ou troco incorreto.\n");
                    }
                    printf("Deseja forcar o fechamento mesmo assim? (s/n): ");
                    char resposta = getchar(); getchar();
                    if (tolower(resposta) != 's') { // Se não confirmar, cancela o fechamento
                        printf("Fechamento cancelado devido a divergencia.\n");
                        system("pause");
                        return;
                    }
                }

                printf("\nCONFIRMAR FECHAMENTO DE CAIXA? (s/n): ");
                char confirmacao = getchar();
                getchar();

                if (tolower(confirmacao) == 's') { // Se confirmar
                    caixaAberto = 0; // Fecha o caixa
                    printf("\nCAIXA FECHADO COM SUCESSO!\n");

                    // Registra o fechamento no log com todos os detalhes
                    char msgLog[200];
                    snprintf(msgLog, sizeof(msgLog),
                             "Caixa fechado. Abertura: R$%.2f, Dinheiro Vendas: R$%.2f, Cartao Vendas: R$%.2f, Esperado: R$%.2f, Apurado: R$%.2f, Diferenca: R$%.2f",
                             vAbre, totalDinheiroF, totalCartaoF, valorEsperadoEmCaixa, totalCaixa, diferencaCaixa);
                    registrarLog(msgLog);

                    // Reseta os totais do dia para que o próximo dia comece "do zero"
                    totalVendas = 0;
                    totalDinheiroF = 0;
                    totalCartaoF = 0;
                    vAbre = 0; // Zera o valor de abertura para o próximo dia
                } else {
                    printf("Fechamento cancelado pelo usuario.\n");
                }
                system("pause");
                return;
            }
            case 2:
                return; // Volta ao menu principal
            default:
                printf("OPCAO INVALIDA! Digite 1 ou 2.\n");
                system("pause");
                tentativas++;
        }
    } while (tentativas < MAX_TENTATIVAS);

    printf("Muitas tentativas invalidas. Retornando ao menu principal...\n");
    system("pause");
}

/*
 * Exibe o menu de relatórios, permitindo ao usuário escolher qual relatório visualizar.
 */
void menuRelatorios() {
    // Carrega clientes para exibição no relatório, se for necessário
    Cliente *clientes_rel = NULL;
    int quantidadeClientes_rel = 0;
    carregarClientes(&clientes_rel, &quantidadeClientes_rel);

    int opcaoRelatorio;
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

        if (scanf("%d", &opcaoRelatorio) != 1) {
            registrarLog("Erro: Tentativa de entrada invalida no menu de relatorios");
            while (getchar() != '\n');
            printf("Entrada invalida!\n");
            system("pause");
            continue;
        }
        getchar(); // Limpa o buffer

        char logMensagem[150]; // Mensagem para registrar no log

        switch(opcaoRelatorio) {
            case 1:
                snprintf(logMensagem, sizeof(logMensagem), "Relatorio de vendas gerado - %d vendas no dia, Total R$ %.2f", (proximoNumeroVenda > 0 ? proximoNumeroVenda -1 : 0) , totalVendas);
                registrarLog(logMensagem); // Registra que o relatório de vendas foi gerado
                exibirRelatorioVendas();
                break;
            case 2:
                snprintf(logMensagem, sizeof(logMensagem), "Relatorio de produtos acessado - %d produtos", sistemaProdutos.quantidade);
                registrarLog(logMensagem); // Registra que o relatório de produtos foi acessado
                exibirProdutosCadastrados();
                break;
            case 3:
                snprintf(logMensagem, sizeof(logMensagem), "Relatorio de clientes acessado - %d clientes", quantidadeClientes_rel);
                registrarLog(logMensagem); // Registra que o relatório de clientes foi acessado
                exibirClientesCadastrados(clientes_rel, quantidadeClientes_rel);
                break;
            case 4:
                registrarLog("Visualizacao de logs do sistema"); // Registra o acesso aos logs
                exibirLogs();
                break;
            case 5:
                free(clientes_rel); // Libera a memória dos clientes carregados para o relatório
                clientes_rel = NULL;
                registrarLog("Saida do menu de relatorios"); // Registra a saída
                return;
            default:
                printf("Opcao invalida!\n");
                system("pause");
        }
    } while(opcaoRelatorio != 5);
}

/*
 * Grava uma mensagem no arquivo de log do sistema, adicionando a data e hora.
 * Isso é útil para rastrear eventos importantes e erros.
 */
void registrarLog(const char *mensagem) {
    time_t now;
    time(&now); // Pega o tempo atual
    struct tm *local = localtime(&now); // Converte para a hora local
    if (local == NULL) {
        printf("Erro ao obter hora local para log!\n");
        return;
    }

    FILE *logFile = fopen(ARQUIVO_LOG, "a"); // Abre o arquivo em modo "append" (adiciona no final)
    if (logFile == NULL) {
        printf("Erro ao abrir arquivo de log!\n");
        return;
    }

    // Escreve a data, hora e a mensagem no arquivo de log
    fprintf(logFile, "[%02d/%02d/%04d %02d:%02d:%02d] %s\n",
            local->tm_mday, local->tm_mon+1, local->tm_year+1900,
            local->tm_hour, local->tm_min, local->tm_sec,
            mensagem);

    fclose(logFile); // Fecha o arquivo de log
}

/*
 * Exibe o conteúdo do arquivo de log do sistema na tela.
 * Implementa uma paginação simples para não sobrecarregar a tela com muitos registros.
 */
void exibirLogs() {
    system("cls");
    printf("|====================================================================|\n");
    printf("|\t\t    LOGS DO SISTEMA\t\t\t\t     |\n");
    printf("|====================================================================|\n");

    FILE *logFile = fopen(ARQUIVO_LOG, "r"); // Abre o arquivo de log para leitura
    if (logFile == NULL) {
        printf("| Nenhum registro de log encontrado ou erro ao abrir.\n");
    } else {
        char linha[256];
        int contador = 0;
        bool imprimiuAlgo = false; // Indica se algum log foi impresso

        while (fgets(linha, sizeof(linha), logFile) != NULL) { // Lê o arquivo linha por linha
            imprimiuAlgo = true;
            linha[strcspn(linha, "\n")] = '\0'; // Remove a quebra de linha
            printf("| %-66.66s |\n", linha);
            contador++;

            // Se o contador atingir 20, pausa para o usuário pressionar Enter
            if (contador > 0 && contador % 20 == 0) {
                printf("|====================================================================|\n");
                printf("| Pressione ENTER para continuar... (%d registros exibidos)          |\n", contador);
                printf("|====================================================================|\n");
                getchar(); // Aguarda Enter
                system("cls"); // Limpa a tela
                printf("|====================================================================|\n");
                printf("|\t\t    LOGS DO SISTEMA (continuacao)\t\t     |\n");
                printf("|====================================================================|\n");
            }
        }
        fclose(logFile);

        if (!imprimiuAlgo) {
             printf("| Nenhum registro de log encontrado.\n");
        } else {
            printf("|--------------------------------------------------------------------|\n");
            printf("| FIM DOS LOGS. TOTAL DE REGISTROS: %-30d |\n", contador);
        }
    }
    printf("|====================================================================|\n");
    system("pause");
}

/*
 * Exibe um relatório com o resumo das vendas do dia, incluindo o faturamento total
 * e o detalhamento dos pagamentos em dinheiro e cartão.
 */
void exibirRelatorioVendas() {
    time_t t; time(&t); struct tm *dataAtualTm = localtime(&t); // Pega a data e hora atual
    char dataFormatada[11];
    if(dataAtualTm) {
        strftime(dataFormatada, sizeof(dataFormatada), "%d/%m/%Y", dataAtualTm); // Formata a data
    } else {
        strcpy(dataFormatada, "N/A");
    }

    system("cls");
    printf("|====================================================================|\n");
    printf("|\t\t    RELATORIO DE VENDAS DO DIA\t\t\t     |\n");
    printf("|====================================================================|\n");
    printf("| DATA: %s\t\t\t\t\t\t     |\n", dataFormatada);
    printf("|--------------------------------------------------------------------|\n");
    // proximoNumeroVenda -1 porque proximoNumeroVenda já foi incrementado após a última venda
    printf("| TOTAL DE VENDAS HOJE: %-38d   |\n", (proximoNumeroVenda > 0 ? proximoNumeroVenda -1 : 0) );
    printf("| FATURAMENTO TOTAL HOJE: R$ %-33.2f    |\n", totalVendas);
    printf("|--------------------------------------------------------------------|\n");
    printf("| DETALHAMENTO POR FORMA DE PAGAMENTO (VENDAS DO DIA):\t\t     |\n");
    printf("| - DINHEIRO: R$ %-48.2f    |\n", totalDinheiroF);
    printf("| - CARTAO: R$ %-50.2f      |\n", totalCartaoF);
    printf("|====================================================================|\n");
    system("pause");
}

/*
 * Exibe uma lista de todos os produtos que estão cadastrados no sistema,
 * mostrando informações como código, nome, categoria, estoque e preço de venda.
 */
void exibirProdutosCadastrados() {
    system("cls");
    printf("|==================================================================================|\n");
    printf("|\t\t\t    PRODUTOS CADASTRADOS\t\t\t\t   |\n");
    printf("|==================================================================================|\n");
    printf("|COD | NOME PRODUTO\t\t| CATEGORIA\t     | ESTOQUE | PRECO VENDA R$ |\n");
    printf("|====|=======================|====================|=========|================|\n");

    if (sistemaProdutos.quantidade == 0) {
        printf("| NENHUM PRODUTO CADASTRADO AINDA.\t\t\t\t\t   |\n");
    } else {
        for(int i = 0; i < sistemaProdutos.quantidade; i++) {
            printf("|%-3d | %-21.21s | %-18.18s | %-7d | %-14.2f |\n",
                   sistemaProdutos.produtos[i].codigo,
                   sistemaProdutos.produtos[i].nomeProduto,
                   sistemaProdutos.produtos[i].categoriaProduto ? sistemaProdutos.produtos[i].categoriaProduto : "N/A", // Exibe "N/A" se a categoria for nula
                   sistemaProdutos.produtos[i].estoque,
                   sistemaProdutos.produtos[i].precoVenda);
            printf("|----|-----------------------|--------------------|---------|----------------|\n");
        }
    }
    printf("| TOTAL DE PRODUTOS CADASTRADOS: %-40d |\n", sistemaProdutos.quantidade);
    printf("|==================================================================================|\n");
    system("pause");
}

/*
 * Exibe uma lista de todos os clientes que estão cadastrados no sistema,
 * mostrando informações como código, nome completo, CPF e celular.
 */
void exibirClientesCadastrados(Cliente *clientes, const int quantidadeClientes) {
    system("cls");
    printf("|=======================================================================================|\n");
    printf("|\t\t\t\tCLIENTES CADASTRADOS\t\t\t\t|\n");
    printf("|=======================================================================================|\n");
    printf("|COD | NOME COMPLETO\t\t\t| CPF\t\t   | CELULAR\t\t   |\n");
    printf("|====|=======================================|==================|=======================|\n");

    if (quantidadeClientes == 0) {
        printf("| NENHUM CLIENTE CADASTRADO AINDA.\t\t\t\t\t\t|\n");
    } else {
        for(int i = 0; i < quantidadeClientes; i++) {
            printf("|%-3d | %-37.37s | %-16.16s | %-21.21s |\n",
                   clientes[i].codigo,
                   clientes[i].nomeCompleto,
                   clientes[i].cpf,
                   clientes[i].celular);
            printf("|----|---------------------------------------|------------------|-----------------------|\n");
        }
    }
    printf("| TOTAL DE CLIENTES CADASTRADOS: %-47d |\n", quantidadeClientes);
    printf("|=======================================================================================|\n");
    system("pause");
}

/*
 * Verifica todos os produtos para identificar aqueles com estoque no nível mínimo
 * ou abaixo. Se encontrar, exibe um alerta com os detalhes desses produtos,
 * indicando quantos faltam para atingir o estoque mínimo.
 */
void verificarEstoqueMinimo() {
    if (sistemaProdutos.produtos == NULL || sistemaProdutos.quantidade <= 0) {
        return; // Não há produtos para verificar
    }
    int produtosAbaixoDoMinimo = 0;
    // Conta quantos produtos estão com estoque baixo
    for (int i = 0; i < sistemaProdutos.quantidade; i++) {
        if (sistemaProdutos.produtos[i].estoque >= 0 &&
            sistemaProdutos.produtos[i].estoqueMinimo >= 0) { // Garante que os valores são válidos
            if (sistemaProdutos.produtos[i].estoque <= sistemaProdutos.produtos[i].estoqueMinimo) {
                produtosAbaixoDoMinimo++;
            }
        }
    }

    if (produtosAbaixoDoMinimo > 0) { // Se houver produtos com estoque baixo, exibe o alerta
        printf("\n\n================ ALERTA DE ESTOQUE BAIXO ================\n");
        printf(" Existem %d produto(s) com estoque no nivel minimo ou abaixo.\n", produtosAbaixoDoMinimo);
        printf("--------------------------------------------------------------\n");
        printf(" COD  NOME DO PRODUTO           ESTOQUE  EST. MIN  FALTAM \n");
        printf("--------------------------------------------------------------\n");

        for (int i = 0; i < sistemaProdutos.quantidade; i++) {
            if (sistemaProdutos.produtos[i].estoque >= 0 &&
                sistemaProdutos.produtos[i].estoqueMinimo >= 0 &&
                sistemaProdutos.produtos[i].estoque <= sistemaProdutos.produtos[i].estoqueMinimo) {
                int faltam = sistemaProdutos.produtos[i].estoqueMinimo - sistemaProdutos.produtos[i].estoque;
                faltam = (faltam < 0) ? 0 : faltam; // Garante que 'faltam' não seja negativo
                char nomeSeguro[26]; // Buffer para garantir que o nome não ultrapasse o limite
                if (sistemaProdutos.produtos[i].nomeProduto[0] != '\0') {
                    strncpy(nomeSeguro, sistemaProdutos.produtos[i].nomeProduto, sizeof(nomeSeguro)-1);
                    nomeSeguro[sizeof(nomeSeguro)-1] = '\0';
                } else {
                    strcpy(nomeSeguro, "N/A"); // Se o nome estiver vazio
                }
                printf(" %-4d %-24s %-8d %-9d %-6d \n",
                       sistemaProdutos.produtos[i].codigo,
                       nomeSeguro,
                       sistemaProdutos.produtos[i].estoque,
                       sistemaProdutos.produtos[i].estoqueMinimo,
                       faltam);
            }
        }
        printf("==============================================================\n");
        printf("Pressione ENTER para continuar...\n");
        while(getchar() != '\n' && getchar() != EOF); // Limpa o buffer do teclado
    }
}

/*
 * Função principal do programa.
 * Configura a cor do console, carrega os dados iniciais
 * e inicia o loop principal do sistema.
 */
int main() {
    system("color 0A"); // Define a cor do console (verde claro no preto)
    carregarCategorias(); // Carrega as categorias salvas
    carregarProdutos();   // Carrega os produtos salvos
    verificarEstoqueMinimo(); // Verifica se há produtos com estoque baixo logo no início
    menu_principal(); // Inicia o menu principal do sistema
    return 0; // Retorna 0 para indicar que o programa terminou com sucesso
}