#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "plataforma.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Definicoes de arquivos e constantes
#define ARQUIVO_LOG "dados/relatorios.log"
#define ARQUIVO_USUARIOS "dados/usuarios.dat"
#define ARQUIVO_CLIENTES "dados/clientes.dat"
#define ARQUIVO_CATEGORIAS "dados/categorias.dat"
#define ARQUIVO_PRODUTOS "dados/produtos.dat"
#define MAX_ITENS_CARRINHO 100

// =============================================================================
// ESTRUTURAS DE DADOS (AGORA COM LISTAS LIGADAS)
// =============================================================================

// Níveis de permissão
// enum é um tipo de dado que permite definir um conjunto de constantes nomeadas em C.
// No código, Permissao é um enum que define os níveis de permissão COLABORADOR e ADMINISTRADOR
typedef enum {
    COLABORADOR,
    ADMINISTRADOR
} Permissao;

// Estrutura para guardar informações de um Usuário
typedef struct {
    int codigo;
    char nome[100];
    char login[50];
    char senha[50];
    Permissao permissao; // Adicionado o campo de permissão
} Usuario;

// Nó da lista de Usuários
typedef struct NodeUsuario {
    Usuario dados;
    struct NodeUsuario *proximo;
} NodeUsuario;

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

// Nó da lista de Clientes
typedef struct NodeCliente {
    Cliente dados;
    struct NodeCliente *proximo;
} NodeCliente;

// Estrutura para guardar informações de uma Categoria
typedef struct {
    int codigo;
    char nome[100];
} Categoria;

// Nó da lista de Categorias
typedef struct NodeCategoria {
    Categoria dados;
    struct NodeCategoria *proximo;
} NodeCategoria;

// Estrutura para guardar informações de um Produto
typedef struct {
    int codigo;
    char nomeProduto[100];
    int codigoCategoria; // Referencia a categoria pelo código
    float precoCompra;
    float percentual;
    float precoVenda;
    int estoque;
    int estoqueMinimo;
} Produto;

// Nó da lista de Produtos
typedef struct NodeProduto {
    Produto dados;
    struct NodeProduto *proximo;
} NodeProduto;

// Estrutura para item no carrinho de compras
typedef struct {
    int codigoProduto;
    char descricao[100];
    float precoUnitario;
    int quantidade;
    float subtotal;
} ItemVenda;

// =============================================================================
// VARIÁVEIS GLOBAIS E CABEÇAS DAS LISTAS
// =============================================================================

// Definindo ponteiro para as listas ligadas
NodeUsuario *listaUsuarios = NULL;
NodeCliente *listaClientes = NULL;
NodeCategoria *listaCategorias = NULL;
NodeProduto *listaProdutos = NULL;

Usuario usuarioLogado; // Variável global para o usuário logado

const float SALDO_MINIMO = 50.0f;
float totalCaixa = 0;
float totalVendas = 0;
float totalSangrias = 0;
int caixaAberto = 0;
int proximoNumeroVenda = 1;
float vAbre = 0;
float totalDinheiroF = 0, totalCartaoF = 0;

// Contadores para o tipo de venda
int numVendasDinheiro = 0;
int numVendasCartao = 0;
int numVendasMisto = 0;

// =============================================================================
// PROTÓTIPOS DE FUNÇÕES
// =============================================================================

// Funções de Menu
void menu_principal(void);
void exibirMenuPrincipal(void);
void menuGerenciamento(void);
void menuVendas(void);
void menuAberturaCaixa(void);
void menuFechamentoCaixa(void);
void menuRelatorios(void);
void menu_pagamento(float valorAPagar, ItemVenda carrinho[], int numItens);
void menu_novaVenda(void);

// Funções de Cadastro (Create)
void cadastrarUsuario();
void cadastrarCliente();
void cadastrarCategoria();
void cadastrarProduto();

// Funções de Exclusão (Delete)
void excluirUsuario();
void excluirCliente();
void excluirCategoria();
void excluirProduto();

// Funções de Exibição (Read)
void exibirUsuarios();
void exibirClientes();
void exibirCategorias(bool comPause);
void exibirProdutos();
void exibirRelatorioVendas();
void exibirLogs();

// Funções de Persistência (Salvar/Carregar)
void salvarTudo();
void carregarTudo();
void salvarUsuarios();
void carregarUsuarios();
void salvarClientes();
void carregarClientes();
void salvarCategorias();
void carregarCategorias();
void salvarProdutos();
void carregarProdutos();

// Funções de Liberação de Memória
void liberarListas();
void liberarListaUsuarios();
void liberarListaClientes();
void liberarListaCategorias();
void liberarListaProdutos();

// Funções Auxiliares e de Negócio
void registrarLog(const char *mensagem);
void verificarEstoqueMinimo();
void retiradaCaixa();
bool validarCPF(const char *cpf);
int obterProximoCodigoUsuario();
int obterProximoCodigoCliente();
int obterProximoCodigoCategoria();
int obterProximoCodigoProduto();
const char* obterNomeCategoria(int codCategoria);

// Funções de Login
bool fazerLogin();

// =============================================================================
// FUNÇÃO MAIN
// =============================================================================

static void descartarEntrada(void) {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {}
}

int main() {
    if (!prepararPastaDados()) return 1;
    carregarTudo();     // Carrega todos os dados dos arquivos ao iniciar

    // Criar um usuario administrador padrao se não houver nenhum
    if (listaUsuarios == NULL) {
        printf("\nSistema sem usuarios. Criando usuario administrador padrao...\n");
        NodeUsuario *novo = malloc(sizeof(NodeUsuario));
        if (novo) { // Verifica se a alocação foi bem-sucedida
            novo->dados.codigo = 1;
            strcpy(novo->dados.nome, "Administrador Master");
            strcpy(novo->dados.login, "admin");
            strcpy(novo->dados.senha, "admin123");
            novo->dados.permissao = ADMINISTRADOR; // Define como administrador
            novo->proximo = NULL; // Insere como primeiro usuario
            listaUsuarios = novo;
            salvarUsuarios();
            printf("Usuario 'admin' com senha 'admin123' criado (Administrador).\n");
            aguardarSegundos(2);
        } else {
            printf("Erro ao alocar memoria para usuario padrao.\n");
            return 1;
        }
    }

    // Loop de login
    while (!fazerLogin()) {
        pausarTela(); // Permite ler o erro antes de tentar novamente
    }

    verificarEstoqueMinimo(); // Verifica estoque baixo na inicialização
    menu_principal();   // Inicia o menu principal
    liberarListas();    // Libera toda a memória alocada antes de sair
    return 0;
}

// =============================================================================
// FUNÇÕES DE MENU E FLUXO PRINCIPAL
// =============================================================================

void menu_principal() {
    int opcao;
    bool sair = false;

    do {
        exibirMenuPrincipal();
        if (scanf("%d", &opcao) != 1) {
            opcao = 0; // Previne loop infinito se a entrada não for um número
        }
        descartarEntrada(); // Limpa o buffer de entrada

        bool aguardarLeitura = false;
        switch (opcao) {
            case 1: menuGerenciamento(); break; // Menu Gerenciamento
            case 2:
                if (caixaAberto) menuVendas(); // Menu Vendas
                else { printf("\n>> ERRO: Caixa fechado. Efetue a abertura primeiro.\n"); aguardarLeitura = true; }
                break;
            case 3:
                aguardarLeitura = true;
                if (!caixaAberto) menuAberturaCaixa(); // Menu Abertura de Caixa
                else { printf("\n>> AVISO: O caixa ja esta aberto.\n"); }
                break;
            case 4:
                aguardarLeitura = true;
                if (caixaAberto) menuFechamentoCaixa(); // Menu Fechamento de Caixa
                else { printf("\n>> AVISO: O caixa ja esta fechado.\n"); }
                break;
            case 5: menuRelatorios(); break; // Menu Relatorios
            case 6:
                if (caixaAberto) { // Saida, verifica se ha caixa aberto
                    printf("\n>> ERRO: O caixa ainda esta aberto. Realize o fechamento primeiro.\n");
                    aguardarLeitura = true;
                } else {
                    printf("\nSalvando todos os dados...\n");
                    salvarTudo();
                    printf("Saindo do sistema. Ate logo!\n");
                    sair = true;
                }
                break;
            default:
                aguardarLeitura = true;
                printf("\n>> ERRO: Opcao invalida! Tente novamente.\n");
        }
        if (aguardarLeitura) pausarTela();
    } while (!sair); // Enquanto sair for true
}

void exibirMenuPrincipal() {
    limparTela();
    exibirCabecalho("MENU PRINCIPAL");
    exibirLinha("Usuario: %s", usuarioLogado.nome);
    exibirLinha("Perfil: %s | Caixa: %s",
                usuarioLogado.permissao == ADMINISTRADOR ? "Administrador" : "Colaborador",
                caixaAberto ? "ABERTO" : "FECHADO");
    exibirSeparador();
    exibirLinha("1  Gerenciamento     Cadastros e exclusoes");
    exibirLinha("2  Vendas            Carrinho, pagamento e sangria");
    exibirLinha("3  Abrir caixa");
    exibirLinha("4  Fechar caixa");
    exibirLinha("5  Relatorios        Vendas, produtos, clientes e logs");
    exibirLinha("6  Salvar e sair");
    exibirSeparador();
    exibirValor("Vendas desta sessao", totalVendas);
    exibirSeparador();
    printf("\n  Escolha uma opcao: ");
}

void menuGerenciamento() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se o usuario e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem acessar o menu de gerenciamento.\n");
        pausarTela();
        return;
    }

    int opcao = 0;
    while (opcao != 9) {
        limparTela();
        exibirCabecalho("GERENCIAMENTO");
        exibirLinha("CLIENTES          PRODUTOS          CATEGORIAS        USUARIOS");
        exibirSeparador();
        exibirLinha("1  Cadastrar      3  Cadastrar       5  Cadastrar       7  Cadastrar");
        exibirLinha("2  Excluir        4  Excluir         6  Excluir         8  Excluir");
        exibirSeparador();
        exibirLinha("9  Voltar ao menu principal");
        exibirSeparador();
        printf("\n  Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) opcao = 0;
        descartarEntrada();

        switch (opcao) {
            case 1: cadastrarCliente(); break;
            case 2: excluirCliente(); break;
            case 3: cadastrarProduto(); break;
            case 4: excluirProduto(); break;
            case 5: cadastrarCategoria(); break;
            case 6: excluirCategoria(); break;
            case 7: cadastrarUsuario(); break;
            case 8: excluirUsuario(); break;
            case 9: break; // Sai do loop e volta ao menu principal
            default: printf("\n>> ERRO: Opcao invalida!\n");
        }
        if (opcao != 9) pausarTela();
    }
}

void menuVendas() {
    int opcao;
    while (true) {
        limparTela();
        exibirCabecalho("VENDAS");
        exibirProdutos();
        exibirLinha("1  Nova venda");
        exibirLinha("2  Retirada de caixa (sangria)");
        exibirLinha("3  Voltar ao menu principal");
        exibirSeparador();
        printf("\n  Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) opcao = 0; // Loop para garantir que a entrada seja um número
        descartarEntrada(); // Limpa o buffer

        switch (opcao) {
            case 1:
                if (listaProdutos != NULL) menu_novaVenda(); // Verifica se a lista de produtos nao esta vazia
                else { printf("\nNao ha produtos cadastrados para vender!\n"); }
                break;
            case 2:
                retiradaCaixa();
                break;
            case 3:
                return; // Volta ao menu principal
            default:
                printf("\nOpcao invalida!\n");
        }
        pausarTela();
    }
}

void menu_novaVenda() {
    ItemVenda carrinho[MAX_ITENS_CARRINHO]; // Cria struct para o carrinho de compras
    int numItens = 0;
    float totalVenda = 0;
    int codProd, qtd;

    while(true) {
        limparTela();
        exibirCabecalho("NOVA VENDA");
        exibirLinha("Venda #%d", proximoNumeroVenda);
        exibirProdutos();

        exibirLinha("CARRINHO DE COMPRAS");
        exibirSeparador();
        if(numItens == 0){
            exibirLinha("Carrinho vazio.");
        } else {
            for(int i = 0; i < numItens; i++){ // Exibe os itens do carrinho
                exibirLinha("%3d x %-28.28s | R$ %9.2f | Total R$ %9.2f", carrinho[i].quantidade, carrinho[i].descricao, carrinho[i].precoUnitario, carrinho[i].subtotal);
            }
        }
        exibirSeparador();
        exibirValor("Total do carrinho", totalVenda); // Total da venda
        exibirSeparador();

        printf("\nDigite o codigo do produto (0 para finalizar): "); // Codigo do produto para a venda
        if(scanf("%d", &codProd) != 1) codProd = -1; // Loop para garantir que a entrada seja um número
        descartarEntrada();

        if(codProd == 0) break;

        NodeProduto* p_aux = listaProdutos; // Cria No auxiliar de produtos
        bool encontrado = false; // Booleano para verificar se o produto foi encontrado
        while(p_aux){ // Loop enquanto o No auxiliar nao for nulo
            if(p_aux->dados.codigo == codProd){ // Verifica se o codigo e igual do produto
                encontrado = true;
                break; // Sai do loop se o produto for encontrado
            }
            p_aux = p_aux->proximo; // passa para o proximo produto
        }

        if(!encontrado){ // Se o produto nao for encontrado
            printf(">> ERRO: Produto nao encontrado!\n");
            pausarTela();
            continue; // 'continue' para voltar ao inicio do loop
        }

        printf("Digite a quantidade: ");
        if(scanf("%d", &qtd) != 1 || qtd <= 0) { // Verifica se a quantidade for valida
            descartarEntrada();
            printf(">> ERRO: Quantidade invalida!\n");
            pausarTela();
            continue;
        }
        descartarEntrada(); // Limpa o buffer

        int quantidadeNoCarrinho = 0;
        for (int i = 0; i < numItens; i++) {
            if (carrinho[i].codigoProduto == codProd) quantidadeNoCarrinho += carrinho[i].quantidade;
        }
        int disponivel = p_aux->dados.estoque - quantidadeNoCarrinho;
        if(disponivel < qtd){ // Verifica se o estoque e suficiente
            printf(">> ERRO: Estoque insuficiente! Disponivel: %d\n", disponivel);
            pausarTela();
            continue;
        }

        if (numItens >= MAX_ITENS_CARRINHO) { // Verifica se o carrinho esta cheio
            printf(">> ERRO: Carrinho cheio!\n");
            pausarTela();
            break;
        }

        // Adiciona item ao carrinho
        carrinho[numItens].codigoProduto = codProd; // Armazena o codigo do produto no carrinho
        strcpy(carrinho[numItens].descricao, p_aux->dados.nomeProduto); // Copia o nome do produto para o carrinho
        carrinho[numItens].precoUnitario = p_aux->dados.precoVenda; // Armazena o preco de venda do produto no carrinho
        carrinho[numItens].quantidade = qtd; // Armazena a quantidade do produto no carrinho
        carrinho[numItens].subtotal = p_aux->dados.precoVenda * qtd; // Calcula o subtotal do item
        totalVenda += carrinho[numItens].subtotal; // Atualiza o total da venda
        numItens++; // Acrescenta o numero de itens no carrinho
    }

    if(numItens > 0){ // Se o carrinho nao estiver vazio
        menu_pagamento(totalVenda, carrinho, numItens); // Vai para o menu de pagamento
    } else {
        printf("\nVenda cancelada.\n");
    }
}

void menuAberturaCaixa() {
    limparTela();
    exibirCabecalho("ABERTURA DE CAIXA");
    float valorAbertura;
    do {
        printf("Digite o valor de abertura do caixa: R$ ");
        if (scanf("%f", &valorAbertura) != 1 || valorAbertura < 0) { // Verifica se a entrada e valida
            printf(">> ERRO: Valor invalido! Digite um valor positivo.\n");
            descartarEntrada(); // Limpa o buffer
            valorAbertura = -1; // Força o loop a continuar
        }
    } while (valorAbertura < 0);
    descartarEntrada();

    vAbre = valorAbertura; // atribui o valor de abertura
    totalCaixa = vAbre; // total do caixa
    caixaAberto = 1; // Marca que o caixa esta aberto

    char logMsg[100]; // Array de caracteres para armazenar a mensagem de log
    sprintf(logMsg, "Caixa aberto com R$ %.2f", vAbre); // sprintf para formatar a mensagem, guarda o vAbre no logMsg
    registrarLog(logMsg); // Registra o log

    printf("\n>> Caixa aberto com sucesso com R$ %.2f!\n", vAbre);
}

void menuFechamentoCaixa() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Se nao for administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem fechar o caixa.\n");
        return;
    }

    limparTela();
    exibirCabecalho("FECHAMENTO DE CAIXA");

    float valorEsperado = vAbre + totalDinheiroF - totalSangrias; // Valor esperado caixa + dinheiro
    float diferenca = totalCaixa - valorEsperado; // Calcula a diferenca entre o total do caixa e o esperado

    exibirValor("Abertura", vAbre);
    exibirValor("Faturamento", totalVendas);
    exibirSeparador();
    exibirValor("Recebido em dinheiro", totalDinheiroF);
    exibirValor("Recebido em cartao", totalCartaoF);
    exibirValor("Retiradas (sangrias)", totalSangrias);
    exibirSeparador();
    exibirLinha("Vendas: %d em dinheiro | %d em cartao | %d mistas",
                numVendasDinheiro, numVendasCartao, numVendasMisto);
    exibirSeparador();
    exibirValor("Saldo esperado", valorEsperado);
    exibirValor("Saldo apurado", totalCaixa);
    exibirValor("Diferenca", diferenca);
    exibirSeparador();
    printf("\n");

    if (fabs(diferenca) > 0.01) { // Verifica se a diferenca e maior que 0.01. fabs para obter o valor absoluto
        printf("!! ATENCAO: DIVERGENCIA ENCONTRADA NO CAIXA !!\n");
    }

    printf("Confirmar o fechamento do caixa? (s/n): ");
    char confirma;
    scanf(" %c", &confirma);
    descartarEntrada();

    if (tolower(confirma) == 's') { // Funcao tolower para converter a letra para minuscula
        caixaAberto = 0;

        char logMsg[256];
        sprintf(logMsg, "Caixa fechado. Esperado: R$%.2f, Apurado: R$%.2f, Diferenca: R$%.2f", valorEsperado, totalCaixa, diferenca);
        registrarLog(logMsg); // Registra o fechamento

        // Reseta os valores para o proximo dia
        totalVendas = 0;
        totalDinheiroF = 0;
        totalCartaoF = 0;
        totalSangrias = 0;
        vAbre = 0;
        proximoNumeroVenda = 1;
        numVendasDinheiro = 0;
        numVendasCartao = 0;
        numVendasMisto = 0;

        printf("\n>> Caixa fechado com sucesso!\n");
    } else {
        printf("\nFechamento cancelado.\n");
    }
}

void menuRelatorios() {
    int opcao = 0;
    while(opcao != 5){
        limparTela();
        exibirCabecalho("RELATORIOS");
        exibirLinha("1  Vendas desta sessao");
        exibirLinha("2  Produtos cadastrados");
        exibirLinha("3  Clientes cadastrados");
        exibirLinha("4  Logs do sistema");
        exibirLinha("5  Voltar ao menu principal");
        exibirSeparador();
        printf("\n  Escolha uma opcao: ");

        if(scanf("%d", &opcao)!=1) opcao = 0; // Loop para garantir que a entrada seja um numero
        descartarEntrada();

        switch(opcao){
            case 1: exibirRelatorioVendas(); break; // Relatorios de venda
            case 2: limparTela(); exibirProdutos(); pausarTela(); break; // Lista de produtos
            case 3: limparTela(); exibirClientes(); pausarTela(); break; // Lista de clientes
            case 4: exibirLogs(); break; // Logs
            case 5: break;
            default: printf("Opcao invalida!\n"); pausarTela();
        }
    }
}

// =============================================================================
// FUNÇÕES DE CADASTRO (CREATE)
// =============================================================================

void cadastrarCliente() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem cadastrar clientes.\n");
        return;
    }
    limparTela();
    NodeCliente *novo = malloc(sizeof(NodeCliente)); // Aloca memoria para um novo cliente
    if (!novo) { printf("Erro de alocacao de memoria!\n"); return; } // Verifica se a alocacao foi bem-sucedida

    novo->dados.codigo = obterProximoCodigoCliente(); // Obtem o proximo codigo de cliente

    printf("=== CADASTRO DE CLIENTE (Codigo: %d) ===\n", novo->dados.codigo);
    printf("Nome Completo: ");
    fgets(novo->dados.nomeCompleto, 100, stdin); // Le o nome completo do cliente
    novo->dados.nomeCompleto[strcspn(novo->dados.nomeCompleto, "\n")] = 0; // Remove o \n no final e guarda

    printf("Nome Social (opcional): ");
    fgets(novo->dados.nomeSocial, 100, stdin);
    novo->dados.nomeSocial[strcspn(novo->dados.nomeSocial, "\n")] = 0;

    while (true) { // Loop para validar o CPF
        printf("CPF (formato XXX.XXX.XXX-XX): ");
        fgets(novo->dados.cpf, 15, stdin);
        novo->dados.cpf[strcspn(novo->dados.cpf, "\n")] = 0; // Remove o \n do final e guarda os dados
        if (validarCPF(novo->dados.cpf)) break; // Chama a funcao para validar, se sim, sai do loop
        printf(">> CPF invalido! Tente novamente.\n"); // Se nao, erro e volta o Loop
    }

    printf("Endereco (Rua, Numero): ");
    fgets(novo->dados.endereco, 100, stdin);
    novo->dados.endereco[strcspn(novo->dados.endereco, "\n")] = 0; // Remove o \n do final e guarda os dados

    printf("Bairro: ");
    fgets(novo->dados.bairro, 50, stdin);
    novo->dados.bairro[strcspn(novo->dados.bairro, "\n")] = 0; // Remove o \n do final e guarda os dados

    printf("Celular (XX) XXXXX-XXXX: ");
    fgets(novo->dados.celular, 20, stdin);
    novo->dados.celular[strcspn(novo->dados.celular, "\n")] = 0; // Remove o \n do final e guarda os dados

    // Adiciona no início da lista
    novo->proximo = listaClientes; // O novo cliente aponta para o antigo primeiro cliente
    listaClientes = novo; // A lista de clientes agora aponta para o novo cliente

    printf("\n>> Cliente cadastrado com sucesso!\n");
    salvarClientes(); // Salva a alteração no arquivo
}

void cadastrarProduto() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem cadastrar produtos.\n");
        return;
    }
    limparTela();
    if (listaCategorias == NULL) { // Se nao houver categorias cadastradas
        printf(">> ERRO: Nenhuma categoria cadastrada. Cadastre uma categoria primeiro.\n");
        return; // Sai da funcao
    }

    NodeProduto *novo = malloc(sizeof(NodeProduto)); // Aloca um novo No para produto
    if (!novo) { printf("Erro de alocacao!\n"); return; } // Verifica se a alocacao foi bem-sucedida

    novo->dados.codigo = obterProximoCodigoProduto(); // Pega o proximo codigo de produto
    printf("=== CADASTRO DE PRODUTO (Codigo: %d) ===\n", novo->dados.codigo);

    printf("Nome do Produto: ");
    fgets(novo->dados.nomeProduto, 100, stdin);
    novo->dados.nomeProduto[strcspn(novo->dados.nomeProduto, "\n")] = 0; // Remove o \n do final e guarda os dados

    while (true) {
        exibirCategorias(false);
        printf("\nDigite o codigo da categoria para o produto: ");
        if (scanf("%d", &novo->dados.codigoCategoria) != 1) novo->dados.codigoCategoria = -1; // Verifica se a entrada e valida
        descartarEntrada(); // Limpa o buffer

        if (obterNomeCategoria(novo->dados.codigoCategoria) != NULL) break; // Verifica se o codigo da categoria existe
        printf(">> Codigo de categoria invalido! Tente novamente.\n");
        pausarTela();
        limparTela();
    }

    printf("Preco de Compra: R$ ");
    scanf("%f", &novo->dados.precoCompra);
    descartarEntrada(); // Limpa o buffer
    printf("Margem de Lucro (%%): ");
    scanf("%f", &novo->dados.percentual);
    novo->dados.precoVenda = novo->dados.precoCompra * (1 + novo->dados.percentual / 100); // Calcula o preco de venda e guarda
    printf("Preco de Venda Calculado: R$ %.2f\n", novo->dados.precoVenda);
    printf("Quantidade em Estoque: ");
    scanf("%d", &novo->dados.estoque);
    descartarEntrada(); // Limpa o buffer
    printf("Estoque Minimo: ");
    scanf("%d", &novo->dados.estoqueMinimo);
    descartarEntrada(); // Limpa o buffer

    novo->proximo = listaProdutos; // Insere o novo produto no inicio da lista
    listaProdutos = novo; // A lista de produtos agora aponta para o novo produto

    printf("\n>> Produto cadastrado com sucesso!\n");
    salvarProdutos();
}

void cadastrarCategoria() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se o usuario e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem cadastrar categorias.\n");
        return;
    }
    limparTela();
    NodeCategoria *novo = malloc(sizeof(NodeCategoria));
    if (!novo) { printf("Erro de alocacao!\n"); return; } // Verifica se a alocacao foi bem-sucedida

    novo->dados.codigo = obterProximoCodigoCategoria(); // Pega o proximo codigo de categoria

    printf("=== CADASTRO DE CATEGORIA (Codigo: %d) ===\n", novo->dados.codigo);
    printf("Nome da Categoria: ");
    fgets(novo->dados.nome, 100, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = 0; // Remove o \n do final e guarda os dados

    novo->proximo = listaCategorias; // Insere o novo no inicio da lista de categorias
    listaCategorias = novo; // A lista de categorias agora aponta para o novo No

    printf("\n>> Categoria cadastrada com sucesso!\n");
    salvarCategorias();
}

void cadastrarUsuario() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se o usuario e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem cadastrar usuarios.\n");
        return;
    }
    limparTela();
    NodeUsuario *novo = malloc(sizeof(NodeUsuario));
    if (!novo) { printf("Erro de alocacao!\n"); return; } // Verifica se a alocacao foi bem-sucedida

    novo->dados.codigo = obterProximoCodigoUsuario(); // Pega o proximo codigo de usuario

    printf("=== CADASTRO DE USUARIO (Codigo: %d) ===\n", novo->dados.codigo);
    printf("Nome Completo do Usuario: ");
    fgets(novo->dados.nome, 100, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = 0; // Remove o \n do final e guarda os dados

    printf("Login de Acesso: ");
    fgets(novo->dados.login, 50, stdin);
    novo->dados.login[strcspn(novo->dados.login, "\n")] = 0; // Remove o \n do final e guarda os dados

    printf("  Senha: ");
    fgets(novo->dados.senha, 50, stdin);
    novo->dados.senha[strcspn(novo->dados.senha, "\n")] = 0; // Remove o \n do final e guarda os dados

    int op_permissao;
    do {
        printf("Nivel de Permissao (1-COLABORADOR, 2-ADMINISTRADOR): ");
        if(scanf("%d", &op_permissao) != 1) op_permissao = 0; // Verifica se a entrada e valida
        descartarEntrada();
        if(op_permissao == 1) { // Se for 1, define como colaborador
            novo->dados.permissao = COLABORADOR;
        } else if (op_permissao == 2) { // Se for 2, define como administrador
            novo->dados.permissao = ADMINISTRADOR;
        } else {
            printf(">> Opcao de permissao invalida! Digite 1 ou 2.\n"); // Se a opcao for invalida, avisa o usuario
        }
    } while(op_permissao < 1 || op_permissao > 2); // Loop enquanto a opcao for invalida


    novo->proximo = listaUsuarios; // Insere o novo usuario no inicio da lista
    listaUsuarios = novo; // A lista de usuarios agora aponta para o novo usuario

    printf("\n>> Usuario cadastrado com sucesso!\n");
    salvarUsuarios();
}

// =============================================================================
// FUNÇÕES DE EXCLUSÃO (DELETE)
// =============================================================================

void excluirCliente() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se o usuario e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem excluir clientes.\n");
        return;
    }
    limparTela();
    exibirClientes(); // Exibe a lista de clientes cadastrados

    if (!listaClientes) { // Verifica se a lista de clientes esta vazia
        printf("\nNenhum cliente para excluir.\n");
        return;
    }

    int cod;
    printf("\nDigite o codigo do cliente a ser excluido (0 para cancelar): ");
    scanf("%d", &cod);
    descartarEntrada(); // Limpa o buffer de entrada

    if (cod == 0) return; // Se o codigo for 0, cancela a exclusao

    NodeCliente *atual = listaClientes, *anterior = NULL; // Cria No auxiliar para percorrer a lista

    while (atual != NULL && atual->dados.codigo != cod) { // Loop para encontrar o cliente
        anterior = atual; // Guarda o cliente anterior
        atual = atual->proximo; // Atual para o proximo cliente
    }

    if (atual == NULL) { // Se o cliente nao for encontrado
        printf("\n>> ERRO: Cliente com codigo %d nao encontrado.\n", cod);
        return;
    }

    printf("\nTem certeza que deseja excluir o cliente '%s'? (s/n): ", atual->dados.nomeCompleto);
    char confirma;
    scanf(" %c", &confirma);
    descartarEntrada();

    if (tolower(confirma) == 's') {
        if (anterior == NULL) { // Se e o primeiro elemento
            listaClientes = atual->proximo; // Pula para o proximo cliente
        } else { // Se esta no meio ou fim
            anterior->proximo = atual->proximo; // anterior recebe o atual proximo
        }
        free(atual); // Libera a memoria do No
        salvarClientes();
        printf("\n>> Cliente excluido com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
}

void excluirProduto() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se o usuario e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem excluir produtos.\n");
        return;
    }
    limparTela();
    exibirProdutos();

    if (!listaProdutos) { // Verifica se a lista de produtos esta vazia
        printf("\nNenhum produto para excluir.\n");
        return;
    }

    int cod;
    printf("\nDigite o codigo do produto a ser excluido (0 para cancelar): ");
    scanf("%d", &cod);
    descartarEntrada(); // Limpa o buffer

    if (cod == 0) return; // Se for 0 cancela a exclusao

    NodeProduto *atual = listaProdutos, *anterior = NULL; // Cria um ponteiro atual e anterior para percorrer a lista

    while(atual != NULL && atual->dados.codigo != cod) { // Loop para encontrar o produto
        anterior = atual; // Anterior recebe atual
        atual = atual->proximo; // Atual para o proximo produto
    }

    if (atual == NULL) { // Se o produto nao for encontrado
        printf("\n>> ERRO: Produto com codigo %d nao encontrado.\n", cod);
        return;
    }

    printf("\nTem certeza que deseja excluir o produto '%s'? (s/n): ", atual->dados.nomeProduto);
    char confirma;
    scanf(" %c", &confirma);
    descartarEntrada();

    if (tolower(confirma) == 's') {
        if (anterior == NULL) { // Se for o primeiro produto
            listaProdutos = atual->proximo; // Vai para o proximo
        } else {
            anterior->proximo = atual->proximo; // Anterior recebe o proximo do atual
        }
        free(atual); // Libera o Produto
        salvarProdutos();
        printf("\n>> Produto excluido com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
}

void excluirCategoria() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se o usuario e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem excluir categorias.\n");
        return;
    }
    limparTela();
    exibirCategorias(false); // Exibe a lista de categorias cadastradas

    if (!listaCategorias) { // Verifica se a lista de categorias esta vazia
        printf("\nNenhuma categoria para excluir.\n");
        return;
    }

    int cod;
    printf("\nDigite o codigo da categoria a ser excluida (0 para cancelar): ");
    scanf("%d", &cod);
    descartarEntrada(); // Limpa o buffer de entrada

    if (cod == 0) return;

    NodeProduto *p_aux = listaProdutos; // Cria um No auxiliar para percorrer a lista de produtos
    while(p_aux != NULL){ // Enquanto o No auxiliar nao for nulo
        if(p_aux->dados.codigoCategoria == cod){ // Verifica se a categoria esta em uso
            printf("\n>> ERRO: Categoria em uso pelo produto '%s' (Cod: %d).\n", p_aux->dados.nomeProduto, p_aux->dados.codigo);
            printf("Nao e possivel excluir uma categoria em uso.\n");
            return;
        }
        p_aux = p_aux->proximo; // Vai para o proximo
    }

    NodeCategoria *atual = listaCategorias, *anterior = NULL; // Cria os auxiliares para percorrer a lista
    while(atual != NULL && atual->dados.codigo != cod) { // Enquanto nao encontrar a categoria
        anterior = atual; // Recebe o presente
        atual = atual->proximo; // Vai para o proximo
    }

    if (atual == NULL) { // Se a categoria nao for encontrada
        printf("\n>> ERRO: Categoria com codigo %d nao encontrada.\n", cod);
        return;
    }

    printf("\nTem certeza que deseja excluir a categoria '%s'? (s/n): ", atual->dados.nome);
    char confirma;
    scanf(" %c", &confirma);
    descartarEntrada();

    if (tolower(confirma) == 's') {
        if (anterior == NULL) { // Se for o primeiro elemento
            listaCategorias = atual->proximo;
        } else {
            anterior->proximo = atual->proximo; // Anterior recebe o proximo do atual
        }
        free(atual); // Libera a memoria do No atual
        salvarCategorias();
        printf("\n>> Categoria excluida com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
}

void excluirUsuario() {
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se o usuario e Administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem excluir usuarios.\n");
        return;
    }
    limparTela();
    exibirUsuarios();

    if (!listaUsuarios) { // Verifica se a lista de usuarios esta vazia
        printf("\nNenhum usuario para excluir.\n");
        return;
    }

    int cod;
    printf("\nDigite o codigo do usuario a ser excluido (0 para cancelar): ");
    scanf("%d", &cod);
    descartarEntrada(); // Limpa o buffer de entrada

    if (cod == 0) return;

    if (cod == 1) { // Verifica se e o usuário principal (admin padrao)
        printf("\n>> ERRO: O usuario principal (Cod: 1) nao pode ser excluido.\n");
        return;
    }

    NodeUsuario *atual = listaUsuarios, *anterior = NULL; // Cria ponteiros auxiliares para percorrer a lista de usuarios
    while(atual != NULL && atual->dados.codigo != cod) { // Enquanto nao encontrar o usuario
        anterior = atual; // Recebe o presente
        atual = atual->proximo; // Vai para o proximo usuario
    }

    if (atual == NULL) { // Se o usuario nao for encontrado
        printf("\n>> ERRO: Usuario com codigo %d nao encontrado.\n", cod);
        return;
    }

    printf("\nTem certeza que deseja excluir o usuario '%s'? (s/n): ", atual->dados.nome);
    char confirma;
    scanf(" %c", &confirma);
    descartarEntrada();

    if (tolower(confirma) == 's') {
        if (anterior == NULL) {
            listaUsuarios = atual->proximo; // Se for o primeiro usuario, pula para o proximo
        } else {
            anterior->proximo = atual->proximo; // Se esta no meio ou fim, anterior recebe o proximo do atual
        }
        free(atual); // Libera a memoria do No atual
        salvarUsuarios();
        printf("\n>> Usuario excluido com sucesso!\n");
    } else {
        printf("\nExclusao cancelada.\n");
    }
}

// =============================================================================
// FUNÇÕES DE PAGAMENTO (REFINADAS)
// =============================================================================

void menu_pagamento(float valorAPagar, ItemVenda carrinho[], int numItens) {
    float totalPagoDinheiroNaVenda = 0, totalPagoCartaoNaVenda = 0;
    float restanteAPagar = valorAPagar;
    int opcaoPagamento;
    bool vendaConcluida = false;

    do {
        limparTela();
        exibirCabecalho("PAGAMENTO");
        exibirValor("Restante a pagar", restanteAPagar);
        exibirValor("Ja pago em dinheiro", totalPagoDinheiroNaVenda);
        exibirValor("Ja pago em cartao", totalPagoCartaoNaVenda);
        exibirSeparador();
        exibirLinha("1  Dinheiro    2  Cartao    3  Cancelar venda");
        exibirSeparador();
        printf("\n  Escolha uma opcao: ");

        if(scanf("%d", &opcaoPagamento) != 1) opcaoPagamento = 0; // Verifica se a entrada e valida
        descartarEntrada();

        switch(opcaoPagamento) {
            case 1: { // Pagamento em Dinheiro
                float valorRecebido;
                printf("Informe o valor recebido em dinheiro: R$ ");
                if (scanf("%f", &valorRecebido) != 1 || valorRecebido <= 0) { // Verifica se o valor e valido
                    descartarEntrada(); printf("Valor invalido!\n"); pausarTela(); continue;
                }
                descartarEntrada();

                if (valorRecebido >= restanteAPagar) {
                    float troco = valorRecebido - restanteAPagar; // Calcula o troco
                    if (troco > totalCaixa) { // Verifica se o caixa tem troco suficiente
                        printf(">> ERRO: Caixa nao tem troco suficiente (R$ %.2f)! Disponivel: R$ %.2f\n", troco, totalCaixa);
                        pausarTela(); continue; // Pula para a proxima iteracao do loop
                    }
                    totalPagoDinheiroNaVenda += restanteAPagar;
                    totalCaixa += restanteAPagar; // Adiciona o valor que 'deveria' ter sido recebido em dinheiro para a venda
                    // O restante a pagar ja representa o valor recebido menos o troco.
                    restanteAPagar = 0;
                    printf("Troco: R$ %.2f\n", troco);
                } else { // Se o valor recebido for menor que o restante a pagar
                    totalPagoDinheiroNaVenda += valorRecebido; // Adiciona o valor recebido ao total pago em dinheiro
                    totalCaixa += valorRecebido; // Adiciona o valor recebido ao caixa
                    restanteAPagar -= valorRecebido; // Atualiza o restante a pagar
                }
                break;
            }
            case 2: { // Pagamento com Cartão
                float valorPagamentoCartao;
                printf("Quanto deseja pagar no cartao? (0 para pagar o total restante R$ %.2f): R$ ", restanteAPagar);
                if (scanf("%f", &valorPagamentoCartao) != 1 || valorPagamentoCartao < 0) { // Verifica se o valor e valido
                    descartarEntrada(); printf("Valor invalido!\n"); pausarTela(); continue;
                }
                descartarEntrada();

                if (valorPagamentoCartao == 0) valorPagamentoCartao = restanteAPagar; // Se o usuario digitar 0, paga o restante total

                // Pequeno ajuste para evitar problemas de ponto flutuante
                if (valorPagamentoCartao > restanteAPagar + 0.01f) { // Verifica se o valor do cartao e maior que o restante a pagar
                    printf(">> ERRO: Valor no cartao (R$ %.2f) maior que o devido (R$ %.2f)!\n", valorPagamentoCartao, restanteAPagar);
                    pausarTela(); continue;
                }

                printf("Processando R$ %.2f no cartao...\n", valorPagamentoCartao);
                printf("Pagamento com cartao APROVADO!\n");
                totalPagoCartaoNaVenda += valorPagamentoCartao; // Adiciona o valor pago no cartao ao total pago na venda
                restanteAPagar -= valorPagamentoCartao; // Atualiza o restante a pagar
                break;
            }
            case 3:
                printf("Pagamento/Venda cancelada pelo usuario.\n");
                // Em caso de cancelamento, o dinheiro já adicionado ao caixa deve ser removido.
                // Isso garante que o caixa nao seja inflacionado por vendas canceladas apos pagamentos parciais em dinheiro.
                totalCaixa -= totalPagoDinheiroNaVenda; // Atualiza o total do caixa removendo o dinheiro pago
                return; // Sai da função, a venda nao e concluida
            default:
                printf("Opcao invalida!\n");
                pausarTela();
        }

        if (restanteAPagar < 0.01f) { // Considera 0.01f como "zero" para float
            vendaConcluida = true;
        }

    } while (!vendaConcluida);

    // Finalizacao da venda e recibo
    limparTela();
    exibirSeparador();
    exibirCabecalho("VENDA FINALIZADA");
    exibirLinha("Recibo #%d | Venda concluida com sucesso", proximoNumeroVenda);
    exibirSeparador();

    // Logica para determinar o tipo de venda (Dinheiro, Cartao, Misto)
    if (totalPagoDinheiroNaVenda > 0 && totalPagoCartaoNaVenda > 0) { // Se ambos os tipos de pagamento foram usados
        exibirLinha("Forma de pagamento: Misto");
        numVendasMisto++; // Incrementa o contador de vendas mistas
    } else if (totalPagoDinheiroNaVenda > 0) { // Se apenas dinheiro foi usado
        exibirLinha("Forma de pagamento: Dinheiro");
        numVendasDinheiro++; // Incrementa o contador de vendas em dinheiro
    } else { // totalPagoCartaoNaVenda > 0
        exibirLinha("Forma de pagamento: Cartao");
        numVendasCartao++; // Incrementa o contador de vendas no cartao
    }

    exibirSeparador();
    exibirValor("Total da venda", valorAPagar);
    if (totalPagoDinheiroNaVenda > 0) {
        exibirValor("Pago em dinheiro", totalPagoDinheiroNaVenda);
    }
    if (totalPagoCartaoNaVenda > 0) {
        exibirValor("Pago em cartao", totalPagoCartaoNaVenda);
    }
    exibirSeparador();

    totalVendas += valorAPagar; // Atualiza o total de vendas do dia
    totalDinheiroF += totalPagoDinheiroNaVenda; // Atualiza
    totalCartaoF += totalPagoCartaoNaVenda; // Atualiza o total de vendas no cartao

    // Atualiza o estoque dos produtos vendidos
    for (int i = 0; i < numItens; i++) {
        NodeProduto *p_aux = listaProdutos; // Cria um ponteiro auxiliar para percorrer a lista de produtos
        while(p_aux) {
            if (p_aux->dados.codigo == carrinho[i].codigoProduto) { // Verifica se o codigo do produto corresponde ao do carrinho
                p_aux->dados.estoque -= carrinho[i].quantidade; // Atualiza o estoque do produto
                break;
            }
            p_aux = p_aux->proximo; // Vai para o proximo produto
        }
    }
    salvarProdutos(); // Salva as alterações de estoque
    proximoNumeroVenda++; // Incrementa o numero da proxima venda
    registrarLog("Venda finalizada com sucesso.");
}

// =============================================================================
// FUNÇÕES DE EXIBIÇÃO (READ)
// =============================================================================

void exibirProdutos() {
    exibirSeparador();
    exibirLinha("%-4s | %-24s | %-16s | %9s | %7s", "COD", "PRODUTO", "CATEGORIA", "PRECO R$", "ESTOQUE");
    exibirSeparador();
    if (listaProdutos == NULL) {
        exibirLinha("Nenhum produto cadastrado.");
    } else {
        for (NodeProduto *aux = listaProdutos; aux != NULL; aux = aux->proximo) {
            const char *categoria = obterNomeCategoria(aux->dados.codigoCategoria);
            exibirLinha("%-4d | %-24.24s | %-16.16s | %9.2f | %7d", aux->dados.codigo,
                        aux->dados.nomeProduto, categoria ? categoria : "N/A",
                        aux->dados.precoVenda, aux->dados.estoque);
        }
    }
    exibirSeparador();
}

void exibirCategorias(bool comPause) {
    if (!comPause) limparTela();
    exibirCabecalho("CATEGORIAS DISPONIVEIS");
    exibirLinha("%-6s | %s", "CODIGO", "CATEGORIA");
    exibirSeparador();
    if (listaCategorias == NULL) exibirLinha("Nenhuma categoria cadastrada.");
    for (NodeCategoria *aux = listaCategorias; aux != NULL; aux = aux->proximo) {
        exibirLinha("%-6d | %.60s", aux->dados.codigo, aux->dados.nome);
    }
    exibirSeparador();
}

void exibirClientes() {
    limparTela();
    exibirCabecalho("CLIENTES CADASTRADOS");
    exibirLinha("%-4s | %-29s | %-14s | %-17s", "COD", "NOME", "CPF", "CELULAR");
    exibirSeparador();
    if (listaClientes == NULL) exibirLinha("Nenhum cliente cadastrado.");
    for (NodeCliente *aux = listaClientes; aux != NULL; aux = aux->proximo) {
        exibirLinha("%-4d | %-29.29s | %-14.14s | %-17.17s", aux->dados.codigo,
                    aux->dados.nomeCompleto, aux->dados.cpf, aux->dados.celular);
    }
    exibirSeparador();
}

void exibirUsuarios() {
    limparTela();
    exibirCabecalho("USUARIOS CADASTRADOS");
    exibirLinha("%-4s | %-29s | %-17s | %-13s", "COD", "NOME", "LOGIN", "PERMISSAO");
    exibirSeparador();
    if (listaUsuarios == NULL) exibirLinha("Nenhum usuario cadastrado.");
    for (NodeUsuario *aux = listaUsuarios; aux != NULL; aux = aux->proximo) {
        exibirLinha("%-4d | %-29.29s | %-17.17s | %-13s", aux->dados.codigo,
                    aux->dados.nome, aux->dados.login,
                    aux->dados.permissao == ADMINISTRADOR ? "Administrador" : "Colaborador");
    }
    exibirSeparador();
}

// =============================================================================
// FUNÇÕES DE PERSISTÊNCIA (SALVAR/CARREGAR)
// =============================================================================

void carregarTudo() { // Chama todas as funcoes de carregar
    carregarUsuarios();
    carregarClientes();
    carregarCategorias();
    carregarProdutos();
}

void salvarTudo() { // Chama todas as funcoes de salvar
    salvarUsuarios();
    salvarClientes();
    salvarCategorias();
    salvarProdutos();
}

void salvarUsuarios() {
    FILE *f = fopen(ARQUIVO_USUARIOS, "wb"); // Cria um arquivo binario para escrita
    if (!f) { // Verifica se o arquivo foi aberto corretamente
        perror("Erro ao abrir arquivo de usuarios para escrita!");
        return;
    }
    // Cria um ponteiro auxiliar para percorrer a lista de usuarios enquanto nao for NULL
    for (NodeUsuario *aux = listaUsuarios; aux != NULL; aux = aux->proximo) { // Percorre a lista de usuarios
        if (fwrite(&aux->dados, sizeof(Usuario), 1, f) != 1) { // Verifica se foi escrito no arquivo corretamente
            perror("Erro ao escrever usuario no arquivo"); // 'perror' exibe a mensagem de erro do sistema
            break;
        }
    }
    fclose(f);
}

void carregarUsuarios() {
    FILE *f = fopen(ARQUIVO_USUARIOS, "rb"); // Cria um arquivo binario para leitura
    if (!f) {
        // Arquivo nao existe ou nao pode ser aberto
        // A funcao perror exibe a mensagem de erro do sistema junto com a string passada
        perror("Erro ao abrir arquivo de usuarios!");
        return;
    }
    liberarListaUsuarios(); // Libera qualquer dado existente antes de carregar

    Usuario u; // Define uma variavel Usuario para armazenar os dados lidos do arquivo
    while(fread(&u, sizeof(Usuario), 1, f)) { // Le os dados do arquivo enquanto houver dados
        NodeUsuario *novo = malloc(sizeof(NodeUsuario)); // Aloca memoria para o novo No
        if(!novo) { // Se a alocacao falhar
            perror("Erro de alocacao de memoria ao carregar usuario"); // 'perror' exibe a mensagem de erro do sistema
            continue; // 'continue' pula para a proxima iteracao se falhar na alocacao
        }
        // Se a alocacao foi bem-sucedida, preenche o novo no
        novo->dados = u; // Copia os dados lidos do arquivo para o novo no
        novo->proximo = listaUsuarios; // Insere o novo no inicio da lista de usuarios
        listaUsuarios = novo; // A lista de usuarios agora aponta para o novo no
    }
    fclose(f);
}

void salvarClientes() {
    FILE *f = fopen(ARQUIVO_CLIENTES, "wb"); // Cria um arquivo binario para escrita
    if (!f) {
        // A funcao perror exibe a mensagem de erro do sistema junto com a string passada
        perror("Erro ao abrir arquivo de clientes para escrita");
        return;
    }
    for(NodeCliente *aux = listaClientes; aux != NULL; aux = aux->proximo) { // Cria um ponteiro auxiliar para percorrer a lista de clientes
        if (fwrite(&aux->dados, sizeof(Cliente), 1, f) != 1) { // Le os dados do cliente e escreve no arquivo
            perror("Erro ao escrever cliente no arquivo"); // Se houver erro ao escrever, exibe a mensagem de erro
            break;
        }
    }
    fclose(f);
}

void carregarClientes() {
    FILE *f = fopen(ARQUIVO_CLIENTES, "rb");
    if (!f) {
        // Arquivo nao existe ou nao pode ser aberto
        return;
    }
    liberarListaClientes();
    Cliente c;
    while(fread(&c, sizeof(Cliente), 1, f)) { // Le os dados do arquivo
        NodeCliente *novo = malloc(sizeof(NodeCliente)); // Aloca memoria para o novo no
        if(!novo) {
            perror("Erro de alocacao de memoria ao carregar cliente"); // 'perror' exibe a mensagem de erro do sistema
            continue;
        }
        novo->dados = c;
        novo->proximo = listaClientes;
        listaClientes = novo;
    }
    fclose(f);
}

void salvarCategorias() {
    FILE *f = fopen(ARQUIVO_CATEGORIAS, "wb"); // Cria um arquivo binario para escrita
    if (!f) {
        // Se o arquivo nao puder ser aberto, exibe a mensagem de erro
        perror("Erro ao abrir arquivo de categorias para escrita");
        return;
    }
    for(NodeCategoria *aux = listaCategorias; aux != NULL; aux = aux->proximo) { // Cria um ponteiro auxiliar para percorrer a lista de categorias
        if (fwrite(&(aux->dados), sizeof(Categoria), 1, f) != 1) { // Le os dados da categoria e escreve no arquivo
            perror("Erro ao escrever categoria no arquivo"); // Se houver erro ao escrever, exibe a mensagem de erro
            break;
        }
    }
    fclose(f);
}

void carregarCategorias() {
    FILE *f = fopen(ARQUIVO_CATEGORIAS, "rb"); // Cria um arquivo binario para leitura
    if (!f) {
        // Se o arquivo nao puder ser aberto, exibe a mensagem de erro
        perror("Erro ao abrir arquivo de categorias para leitura");
        return;
    }

    liberarListaCategorias(); // Libera qualquer dado existente antes de carregar
    Categoria c; // Cria uma variavel Categoria para armazenar os dados lidos do arquivo

    while(fread(&c, sizeof(Categoria), 1, f)) { // Enquanto houver dados no arquivo
        NodeCategoria *novo = malloc(sizeof(NodeCategoria)); // Aloca memoria para o novo No
        if(!novo) { // Se a alocacao falhar
            perror("Erro de alocacao de memoria ao carregar categoria");
            continue; // Pula para a proxima iteracao se falhar na alocacao
        }
        novo->dados = c; // Copia os dados lidos do arquivo para o novo no
        novo->proximo = listaCategorias; // Insere o novo no inicio da lista de categorias
        listaCategorias = novo; // A lista de categorias agora aponta para o novo no
    }
    fclose(f);
}

void salvarProdutos() {
    FILE *f = fopen(ARQUIVO_PRODUTOS, "wb"); // Cria um arquivo binario para escrita
    if (!f) { // Se o arquivo nao puder ser aberto, exibe a mensagem de erro
        // 'perror' exibe a mensagem de erro do sistema junto com a string passada
        perror("Erro ao abrir arquivo de produtos para escrita");
        return;
    }
    for(NodeProduto *aux = listaProdutos; aux; aux = aux->proximo) { // Cria um ponteiro auxiliar para percorrer a lista de produtos
        if (fwrite(&(aux->dados), sizeof(Produto), 1, f) != 1) { // Le os dados do produto e escreve no arquivo
            perror("Erro ao escrever produto no arquivo"); // Se houver erro ao escrever, exibe a mensagem de erro
            break;
        }
    }
    fclose(f);
}

void carregarProdutos() {
    FILE *f = fopen(ARQUIVO_PRODUTOS, "rb"); // Cria um arquivo binario para leitura
    if (!f) {
        // Se o arquivo nao puder ser aberto, exibe a mensagem de erro
        perror("Erro ao abrir arquivo de produtos para leitura"); // 'perror' exibe a mensagem de erro do sistema junto com a string passada
        return;
    }
    liberarListaProdutos(); // Libera qualquer dado existente antes de carregar

    Produto p; // Define uma variavel Produto para armazenar os dados lidos do arquivo

    while(fread(&p, sizeof(Produto), 1, f)) { // Enquanto houver dados no arquivo
        NodeProduto *novo = malloc(sizeof(NodeProduto)); // Aloca memoria para o novo No
        if(!novo) { // Se houver erro de alocacao
            perror("Erro de alocacao de memoria ao carregar produto");
            continue;
        }
        novo->dados = p; // Copia os dados lidos do arquivo para o novo no
        novo->proximo = listaProdutos; // Insere o novo no inicio da lista de produtos
        listaProdutos = novo; // A lista de produtos agora aponta para o novo No
    }
    fclose(f);
}

// =============================================================================
// FUNÇÕES DE LIBERAÇÃO DE MEMÓRIA
// =============================================================================

void liberarListas() { // Funcao para liberar todas as listas
    liberarListaUsuarios();
    liberarListaClientes();
    liberarListaCategorias();
    liberarListaProdutos();
}

void liberarListaUsuarios() {
    NodeUsuario *atual = listaUsuarios; // Cria um ponteiro auxiliar para percorrer a lista de usuarios
    while (atual != NULL) { // Enquanto nao chegar no fim
        NodeUsuario *temp = atual; // Temporario recebe atual
        atual = atual->proximo; // Atual recebe o proximo usuario
        free(temp); // Libera a memoria do No temporario
    }
    listaUsuarios = NULL; // A lista de usuarios agora aponta para NULL
}

void liberarListaClientes() {
    NodeCliente *atual = listaClientes; // Cria um ponteiro auxiliar para percorrer a lista de clientes
    while (atual != NULL) {
        NodeCliente *temp = atual; // Temporario recebe atual
        atual = atual->proximo; // Atual recebe o proximo cliente
        free(temp); // Libera a memoria do No temporario
    }
    listaClientes = NULL; // A lista agora aponta para NULL
}

void liberarListaCategorias() {
    NodeCategoria *atual = listaCategorias; // Cria um ponteiro auxiliar para percorrer a lista de categorias
    while (atual != NULL) {
        NodeCategoria *temp = atual; // Temporario recebe atual
        atual = atual->proximo; // Atual recebe o proximo categoria
        free(temp); // Libera memoria do No temporario
    }
    listaCategorias = NULL; // A lista de categorias agora aponta para NULL
}

void liberarListaProdutos() {
    NodeProduto *atual = listaProdutos; // Cria um ponteiro auxiliar para percorrer a lista de produtos
    while (atual != NULL) {
        NodeProduto *temp = atual; // Temporario recebe atual
        atual = atual->proximo; // Atual recebe o proximo produto
        free(temp); // Libera a memoria do No temporario
    }
    listaProdutos = NULL; // A lista de produto aponta para NULL
}

// =============================================================================
// FUNÇÕES AUXILIARES
// =============================================================================

const char* obterNomeCategoria(int codCategoria) { // FUncao para obter o nome da categoria pelo codigo
    NodeCategoria *aux = listaCategorias; // Cria um ponteiro auxiliar para percorrer a lista de categorias
    while (aux != NULL) {
        if (aux->dados.codigo == codCategoria) { // Se o codigo da categoria for igual ao passado
            return aux->dados.nome; // Retorna o nome da categoria
        }
        aux = aux->proximo; // Vai para o proximo No
    }
    return NULL; // Se nao encontrar, retorna NULL
}

int obterProximoCodigoUsuario() { // Funcao para obter o proximo codigo de usuario
    if (listaUsuarios == NULL) return 1; // Se a lista de usuarios estiver vazia, o proximo codigo sera 1
    int maxCod = 0;
    for(NodeUsuario* aux = listaUsuarios; aux; aux = aux->proximo) { // Cria um ponteiro auxiliar para percorrer a lista de usuarios
        if(aux->dados.codigo > maxCod) maxCod = aux->dados.codigo; // Se o codigo do usuario for maior que o maximo encontrado, atualiza o maximo
    }
    return maxCod + 1; // Retorna o valor do maximo encontrado + 1, que sera o proximo codigo disponivel
}

int obterProximoCodigoCliente() { // Funcao para obter o proximo codigo de cliente
    if (listaClientes == NULL) return 1; // Se a lista de clientes estiver vazia, o proximo codigo sera 1
    int maxCod = 0;
    for(NodeCliente* aux = listaClientes; aux; aux = aux->proximo) { // Cria um ponteiro auxiliar para percorrer a lista de clientes
        if(aux->dados.codigo > maxCod) maxCod = aux->dados.codigo; // Se o codigo do cliente for maior que o maximo encontrado, atualiza o maximo
    }
    return maxCod + 1; // Retorna o valor do maximo encontrado + 1, que sera o proximo codigo disponivel
}

int obterProximoCodigoCategoria() { // Funcao para obter o proximo codigo de categoria
    if (listaCategorias == NULL) return 1; // Se a lista de categorias estiver vazia, o proximo codigo sera 1
    int maxCod = 0;
    for(NodeCategoria* aux = listaCategorias; aux; aux = aux->proximo) { // Cria um ponteiro auxiliar para percorrer a lista de categorias
        if(aux->dados.codigo > maxCod) maxCod = aux->dados.codigo; // Se o codigo da categoria for maior que o maximo encontrado, atualiza o maximo
    }
    return maxCod + 1; // Retona o valor do maximo encontrado + 1, que sera o proximo codigo disponivel
}

int obterProximoCodigoProduto() {
    if (listaProdutos == NULL) return 1; // Verifica se a lista de produtos esta vazia, se sim, o proximo codigo sera 1
    int maxCod = 0;
    for(NodeProduto* aux = listaProdutos; aux; aux = aux->proximo) { // Cria um ponteiro auxiliar para percorrer a lista de produtos
        if(aux->dados.codigo > maxCod) maxCod = aux->dados.codigo; // Se o codigo do produto for maior que o maximo encontrado, atualiza o maximo
    }
    return maxCod + 1; // Retorna o valor do maximo encontrado + 1, que sera o proximo codigo disponivel
}

bool validarCPF(const char *cpf) {
    char digitos[12];
    int quantidade = 0;
    for (int i = 0; cpf[i] != '\0'; i++) {
        unsigned char caractere = (unsigned char)cpf[i];
        if (isdigit(caractere)) {
            if (quantidade == 11) return false;
            digitos[quantidade++] = (char)caractere;
        } else if (caractere != '.' && caractere != '-') {
            return false;
        }
    }
    if (quantidade != 11) return false;
    bool iguais = true;
    for (int i = 1; i < 11; i++) {
        if (digitos[i] != digitos[0]) iguais = false;
    }
    if (iguais) return false;
    for (int tamanho = 9; tamanho <= 10; tamanho++) {
        int soma = 0;
        for (int i = 0; i < tamanho; i++) soma += (digitos[i] - '0') * (tamanho + 1 - i);
        int verificador = (soma * 10) % 11;
        if (verificador == 10) verificador = 0;
        if (digitos[tamanho] - '0' != verificador) return false;
    }
    return true;
}
void registrarLog(const char *mensagem) { // Funcao de registro de logs, parametro mensagem deve ser uma string
    time_t now; // time_t e usado para armazenar o tempo atual, biblioteca time.h
    time(&now); // Funcao time() preenche a variavel 'now' com o tempo atual em segundos desde 1 de janeiro de 1970 (Epoch)
    // Cria uma struct tm que converte o tempo em segundos para uma estrutura de tempo local, com campos como ano, mes, dia, hora, minuto e segundo
    struct tm *local = localtime(&now);
    if (local == NULL) return; // Se a conversao falhar, retorna sem fazer nada

    FILE *logFile = fopen(ARQUIVO_LOG, "a"); // Cria o arquivo log como append, para acrescentar no final
    if (logFile == NULL) return; // Se o arquivo nao puder ser aberto, retorna sem fazer nada

    // Formata a mensagem de log com a data e hora atual, e guarda no arquivo!
    fprintf(logFile, "[%02d/%02d/%04d %02d:%02d:%02d] %s\n",
            local->tm_mday, local->tm_mon + 1, local->tm_year + 1900,
            local->tm_hour, local->tm_min, local->tm_sec,
            mensagem);
    fclose(logFile);
}

void retiradaCaixa() { // Funcao Sangria
    if (usuarioLogado.permissao != ADMINISTRADOR) { // Verifica se o usuario logado e um administrador
        printf("\n>> ACESSO NEGADO: Apenas administradores podem realizar retiradas de caixa.\n");
        return;
    }
    limparTela();
    exibirCabecalho("RETIRADA DE CAIXA");
    printf("Saldo atual em caixa: R$ %.2f\n", totalCaixa);

    float valorRetirada;
    printf("Digite o valor a ser retirado (0 para cancelar): R$ ");

    if(scanf("%f", &valorRetirada) != 1 || valorRetirada < 0){ // Verifica se o valor e valido
        descartarEntrada();
        printf(">> ERRO: Valor invalido!\n"); // Se nao for, exibe mensagem de erro e sai da funcao
        return;
    }
    descartarEntrada(); // Limpa buffer

    if(valorRetirada == 0) {
        printf("Operacao cancelada.\n");
        return;
    }

    if(totalCaixa - valorRetirada < SALDO_MINIMO){ // Se o saldo apos a retirada for menor que o minimo
        printf(">> ERRO: Saldo insuficiente! A retirada deixaria o caixa abaixo do minimo de R$ %.2f\n", SALDO_MINIMO);
        printf("Saldo atual: R$ %.2f | Saldo minimo apos retirada: R$ %.2f\n", totalCaixa, SALDO_MINIMO);
        return;
    }

    totalCaixa -= valorRetirada; // Se for sucesso, totalCaixa e atualizado
    totalSangrias += valorRetirada;
    char logMsg[100]; // Cria um array para o log

    // sprintf para mostrar a mensagem e armazenar na variavel logMsg os valores das variaveis.
    sprintf(logMsg, "Retirada de caixa (sangria) de R$ %.2f. Novo saldo: R$ %.2f", valorRetirada, totalCaixa);
    registrarLog(logMsg); // Registra o log da retirada junto com a mensagem

    printf("\n>> Retirada de R$ %.2f realizada com sucesso.\n", valorRetirada);
    printf("Novo saldo do caixa: R$ %.2f\n", totalCaixa);
}

void verificarEstoqueMinimo() {
    bool alerta = false;
    for(NodeProduto* aux = listaProdutos; aux; aux = aux->proximo){ // Cria um ponteiro auxiliar para percorrer a lista de produtos
        if(aux->dados.estoque <= aux->dados.estoqueMinimo){ // Se o estoque do produto for menor ou igual ao estoque minimo
            alerta = true; // Define alerta como verdadeiro
            break;
        }
    }
    if(alerta){ // Se o alerta for verdadeiro, exibe a mensagem de alerta
        printf("\n\n/!\\ ALERTA DE ESTOQUE BAIXO /!\\\n");
        printf("==============================================================\n");
        printf("Os seguintes produtos estao com estoque baixo ou zerado:\n");
        for(NodeProduto* aux = listaProdutos; aux; aux = aux->proximo){ // Cria um ponteiro auxiliar para percorrer a lista de produtos
            if(aux->dados.estoque <= aux->dados.estoqueMinimo){ // Se o estoque do produto for menor ou igual ao estoque minimo
                printf("- %s (Cod: %d) | Estoque: %d | Minimo: %d\n", aux->dados.nomeProduto, aux->dados.codigo, aux->dados.estoque, aux->dados.estoqueMinimo);
            } // Mostra o nome do produto, codigo, estoque atual e estoque minimo do produto alerta
        }
        printf("==============================================================\n");
        printf("Pressione ENTER para continuar...\n");
        pausarTela();
    }
}

void exibirRelatorioVendas() {
    limparTela();
    exibirCabecalho("VENDAS DESTA SESSAO");
    exibirLinha("Quantidade de vendas: %d", proximoNumeroVenda - 1);
    exibirLinha("Dinheiro: %d | Cartao: %d | Misto: %d",
                numVendasDinheiro, numVendasCartao, numVendasMisto);
    exibirSeparador();
    exibirValor("Faturamento", totalVendas);
    exibirValor("Recebido em dinheiro", totalDinheiroF);
    exibirValor("Recebido em cartao", totalCartaoF);
    exibirSeparador();
    pausarTela();
}

void exibirLogs() {
    limparTela();
    exibirCabecalho("LOGS DO SISTEMA");
    FILE *arquivo = fopen(ARQUIVO_LOG, "r");
    if (arquivo == NULL) {
        exibirLinha("Nenhum log encontrado.");
    } else {
        char linha[256];
        while (fgets(linha, sizeof(linha), arquivo)) {
            linha[strcspn(linha, "\n")] = '\0';
            exibirLinha("%s", linha);
        }
        fclose(arquivo);
    }
    exibirSeparador();
    pausarTela();
}

// =============================================================================
// FUNÇÕES DE LOGIN
// =============================================================================

bool fazerLogin() {
    char login[50];
    char senha[50];
    bool logado = false;

    limparTela();
    exibirCabecalho("ACESSO AO SISTEMA");

    exibirLinha("Informe seu usuario e senha para continuar.");
    exibirSeparador();
    printf("\n  Login: ");
    fgets(login, 50, stdin);
    login[strcspn(login, "\n")] = 0; // Remove o '\n' do final da string

    printf("  Senha: ");
    fgets(senha, 50, stdin);
    senha[strcspn(senha, "\n")] = 0; // Remove o '\n' do final da string

    NodeUsuario *aux = listaUsuarios; // Cria um ponteiro auxiliar para percorrer a lista de usuarios
    while (aux != NULL) { // Loop enquanto nao chegar ao final da lista
        // Verifica por copia se o login e senha correspondem ao usuario
        if (strcmp(aux->dados.login, login) == 0 && strcmp(aux->dados.senha, senha) == 0) {
            usuarioLogado = aux->dados; // Copia os dados do usuário para a global
            logado = true; // Atualiza o logado
            printf("\nBem-vindo, %s!\n", usuarioLogado.nome);
            break;
        }
        aux = aux->proximo; // Vai para o proximo usuario
    }

    if (!logado) { // Se nao encontrar o usuario
        printf("\n>> ERRO: Login ou senha incorretos. Tente novamente.\n");
    }
    return logado; // Retorna true se logado, false se nao logado
}


