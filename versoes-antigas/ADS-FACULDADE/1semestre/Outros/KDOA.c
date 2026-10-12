#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Contadores de registros
int user_cont = 1, limpeza_cont = 10, alimentos_cont = 10, padaria_cont = 10, cadastrados_cont = 10, login_code = -1;

// Estrutura para produtos
typedef struct {
    int codigo_produto;
    char nome_produto[20];
    float valor_custo;
    float valor_venda;
    float estoque_produto;
} produtos;

// Estrutura para usuários
typedef struct {
    int id_usuario;
    char login[20];
    char senha[20];
    int nivel_acesso; // 1 - adm, 2 - gerente, 3 - cadastrados
} usuarios;

typedef struct {
    char data[11]; // formato DD/MM/AAAA
    float total_limpeza;
    float total_alimentos;
    float total_padaria;
    float faturamento;
    float custo_total;
    float lucro_total;
    float margem_lucro;
} fechamento_diario;

fechamento_diario *historico_fechamentos = NULL;
int historico_cont = 0;

// Ponteiros para as listas
usuarios **cadastrados = NULL;
produtos **limpeza = NULL;
produtos **alimentos = NULL;
produtos **padaria = NULL;

// Variáveis globais
int opcao = 0, quantidade = 0, estoque_temp[21] = {0};
float carrinho_limpeza = 0, carrinho_alimentos = 0, carrinho_padaria = 0;
float total_limpeza = 0, total_alimentos = 0, total_padaria = 0, total = 0;
float total_desconto = 0, falta = 0, guarda_desconto = 0, desconto_informado = 0;
float pagamento = 0, troco = 0, faturamento = 0, valor_abertura = 0;
bool status_caixa = false;

// Protótipos de funções
void menu_principal(void);
void menu_limpeza(void);
void menu_alimento(void);
void menu_padaria(void);
void menu_pagamento(void);
void menu_falta(void);
void login_acesso(void);
void menu_cancelar(void);
void menu_caixa(void);
void acesso_adm(void);
void registrar_usuarios(void);
void listar_usuarios(void);
void registrar_produtos(void);
void listar_produtos(void);
float aplicar_desconto(float);
void inicializar_sistema(void);
void liberar_memoria(void);

// Função para inicializar o sistema
void inicializar_sistema() {
    // Alocar memória para as listas
    cadastrados = malloc(cadastrados_cont * sizeof(usuarios *));
    limpeza = malloc(limpeza_cont * sizeof(produtos *));
    alimentos = malloc(alimentos_cont * sizeof(produtos *));
    padaria = malloc(padaria_cont * sizeof(produtos *));
    
    // Inicializar admin padrão
    cadastrados[0] = malloc(sizeof(usuarios));
    cadastrados[0]->id_usuario = 0;
    strcpy(cadastrados[0]->login, "admin");
    strcpy(cadastrados[0]->senha, "admin");
    cadastrados[0]->nivel_acesso = 1;
    
    // Inicializar produtos de limpeza
    for (int i = 0; i < 7; i++) {
        limpeza[i] = malloc(sizeof(produtos));
        limpeza[i]->codigo_produto = i+1;
        sprintf(limpeza[i]->nome_produto, "Limpeza %d", i+1);
        limpeza[i]->valor_venda = 5.0 + i;
        limpeza[i]->estoque_produto = 10;
    }
    limpeza_cont = 7;
    
    // Inicializar alimentos
    for (int i = 0; i < 7; i++) {
        alimentos[i] = malloc(sizeof(produtos));
        alimentos[i]->codigo_produto = i+1;
        sprintf(alimentos[i]->nome_produto, "Alimento %d", i+1);
        alimentos[i]->valor_venda = 3.0 + i;
        alimentos[i]->estoque_produto = 10;
    }
    alimentos_cont = 7;
    
    // Inicializar padaria
    for (int i = 0; i < 7; i++) {
        padaria[i] = malloc(sizeof(produtos));
        padaria[i]->codigo_produto = i+1;
        sprintf(padaria[i]->nome_produto, "Padaria %d", i+1);
        padaria[i]->valor_venda = 2.0 + i;
        padaria[i]->estoque_produto = 0;
    }
    padaria_cont = 7;
}

// Função para liberar memória
void liberar_memoria() {
    // Liberar produtos de limpeza
    for (int i = 0; i < limpeza_cont; i++) {
        free(limpeza[i]);
    }
    free(limpeza);
    
    // Liberar alimentos
    for (int i = 0; i < alimentos_cont; i++) {
        free(alimentos[i]);
    }
    free(alimentos);
    
    // Liberar padaria
    for (int i = 0; i < padaria_cont; i++) {
        free(padaria[i]);
    }
    free(padaria);
    
    // Liberar usuários
    for (int i = 0; i < cadastrados_cont; i++) {
        free(cadastrados[i]);
    }
    free(cadastrados);
}

// Função de login
void login_acesso() {
    login_code = -1;
    char login_temp[40];
    char senha_temp[40];

    do {
        system("cls");
        printf("Usuario:\n");
        fgets(login_temp, sizeof(login_temp), stdin);
        printf("Senha:\n");
        fgets(senha_temp, sizeof(senha_temp), stdin);

        // Remove \n
        login_temp[strcspn(login_temp, "\n")] = '\0';
        senha_temp[strcspn(senha_temp, "\n")] = '\0';

        for (int i = 0; i < cadastrados_cont; i++) {
            if (cadastrados[i] != NULL && 
                strcmp(login_temp, cadastrados[i]->login) == 0 &&
                strcmp(senha_temp, cadastrados[i]->senha) == 0) {
                login_code = i;
                printf("Login realizado com sucesso!\n");
                system("pause");
                menu_principal();
                return;
            }
        }

        printf("Login e/ou senha invalidos! Tente novamente..\n");
        system("pause");
    } while (login_code == -1);
}

// Função para registrar produtos
void registrar_produtos() {
    do {
        system("cls");
        printf("Selecione a categoria\n");
        printf("1- Limpeza\n");
        printf("2- Padaria\n");
        printf("3- Alimentos\n");
        printf("4- Voltar\n");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer
        
        switch (opcao) {
            case 1: {
                produtos **temp = realloc(limpeza, (limpeza_cont + 1) * sizeof(produtos *));
                if (temp == NULL) {
                    printf("Erro ao alocar produtos\n");
                    break;
                }
                limpeza = temp;
                limpeza[limpeza_cont] = malloc(sizeof(produtos));
                if (limpeza[limpeza_cont] == NULL) {
                    printf("Erro ao alocar produto\n");
                    break;
                }
                
                limpeza[limpeza_cont]->codigo_produto = limpeza_cont + 1;
                printf("Informe o nome do produto:\n");
                fgets(limpeza[limpeza_cont]->nome_produto, 20, stdin);
                limpeza[limpeza_cont]->nome_produto[strcspn(limpeza[limpeza_cont]->nome_produto, "\n")] = '\0';
                
                do {
                    printf("Informe o valor do produto:\n");
                    scanf("%f", &limpeza[limpeza_cont]->valor_venda);
                } while (limpeza[limpeza_cont]->valor_venda < 0);
                
                do {
                    printf("Informe o estoque do produto:\n");
                    scanf("%f", &limpeza[limpeza_cont]->estoque_produto);
                } while (limpeza[limpeza_cont]->estoque_produto < 0);
                
                limpeza_cont++;
                getchar();
                break;
            }
            case 2: {
                produtos **temp = realloc(padaria, (padaria_cont + 1) * sizeof(produtos *));
                if (temp == NULL) {
                    printf("Erro ao alocar produtos\n");
                    break;
                }
                padaria = temp;
                padaria[padaria_cont] = malloc(sizeof(produtos));
                if (padaria[padaria_cont] == NULL) {
                    printf("Erro ao alocar produto\n");
                    break;
                }
                
                padaria[padaria_cont]->codigo_produto = padaria_cont + 1;
                printf("Informe o nome do produto:\n");
                fgets(padaria[padaria_cont]->nome_produto, 20, stdin);
                padaria[padaria_cont]->nome_produto[strcspn(padaria[padaria_cont]->nome_produto, "\n")] = '\0';
                
                do {
                    printf("Informe o valor do produto:\n");
                    scanf("%f", &padaria[padaria_cont]->valor_venda);
                } while (padaria[padaria_cont]->valor_venda < 0);
                
                do {
                    printf("Informe o estoque do produto:\n");
                    scanf("%f", &padaria[padaria_cont]->estoque_produto);
                } while (padaria[padaria_cont]->estoque_produto < 0);
                
                padaria_cont++;
                getchar();
                break;
            }
            case 3: {
                produtos **temp = realloc(alimentos, (alimentos_cont + 1) * sizeof(produtos *));
                if (temp == NULL) {
                    printf("Erro ao alocar produtos\n");
                    break;
                }
                alimentos = temp;
                alimentos[alimentos_cont] = malloc(sizeof(produtos));
                if (alimentos[alimentos_cont] == NULL) {
                    printf("Erro ao alocar produto\n");
                    break;
                }
                
                alimentos[alimentos_cont]->codigo_produto = alimentos_cont + 1;
                printf("Informe o nome do produto:\n");
                fgets(alimentos[alimentos_cont]->nome_produto, 20, stdin);
                alimentos[alimentos_cont]->nome_produto[strcspn(alimentos[alimentos_cont]->nome_produto, "\n")] = '\0';
                
                do {
                    printf("Informe o valor do produto:\n");
                    scanf("%f", &alimentos[alimentos_cont]->valor_venda);
                } while (alimentos[alimentos_cont]->valor_venda < 0);
                
                do {
                    printf("Informe o estoque do produto:\n");
                    scanf("%f", &alimentos[alimentos_cont]->estoque_produto);
                } while (alimentos[alimentos_cont]->estoque_produto < 0);
                
                alimentos_cont++;
                getchar();
                break;
            }
            case 4:
                return;
            default:
                printf("Opcao invalida\n");
                break;
        }
    } while (opcao != 4);
}

// Função para registrar usuários
void registrar_usuarios() {
    usuarios **temp = realloc(cadastrados, (cadastrados_cont + 1) * sizeof(usuarios *));
    if (temp == NULL) {
        printf("Erro ao alocar usuarios\n");
        return;
    }
    cadastrados = temp;
    
    cadastrados[cadastrados_cont] = malloc(sizeof(usuarios));
    if (cadastrados[cadastrados_cont] == NULL) {
        printf("Erro ao alocar usuario\n");
        return;
    }
    
    int permissao = 0;
    system("cls");
    getchar(); // Limpar buffer
    
    printf("Informe o login: ");
    fgets(cadastrados[cadastrados_cont]->login, sizeof(cadastrados[cadastrados_cont]->login), stdin);
    cadastrados[cadastrados_cont]->login[strcspn(cadastrados[cadastrados_cont]->login, "\n")] = '\0';
    
    printf("Informe a senha: ");
    fgets(cadastrados[cadastrados_cont]->senha, sizeof(cadastrados[cadastrados_cont]->senha), stdin);
    cadastrados[cadastrados_cont]->senha[strcspn(cadastrados[cadastrados_cont]->senha, "\n")] = '\0';
    
    system("cls");
    while (permissao != 1 && permissao != 2 && permissao != 3) {
        printf("Permissao de acesso?\n");
        printf("1- ADMINISTRATIVO\n");
        printf("2- GERENTE\n");
        printf("3- COLABORADOR\n");
        scanf("%d", &permissao);
        
        if (permissao == 1) {
            cadastrados[cadastrados_cont]->nivel_acesso = 1;
        } else if (permissao == 2) {
            cadastrados[cadastrados_cont]->nivel_acesso = 2;
        } else if (permissao == 3) {
            cadastrados[cadastrados_cont]->nivel_acesso = 3;
        } else {
            printf("Opcao invalida!\n");
        }
    }
    
    cadastrados[cadastrados_cont]->id_usuario = cadastrados_cont;
    cadastrados_cont++;
    user_cont++;
}

// Função para listar usuários
void listar_usuarios() {
    system("cls");
    printf("<Lista de usuarios>\n");
    printf("Codigo\tUsuario\tPermissao\n");
    for (int i = 0; i < cadastrados_cont; i++) {
        if (cadastrados[i] != NULL) {
            printf("%d\t%s\t", cadastrados[i]->id_usuario, cadastrados[i]->login);
            switch(cadastrados[i]->nivel_acesso) {
                case 1: printf("ADMIN\n"); break;
                case 2: printf("GERENTE\n"); break;
                case 3: printf("COLABORADOR\n"); break;
            }
        }
    }
    system("pause");
}

// Função para listar produtos
void listar_produtos() {
    do {
        system("cls");
        printf("<Lista de produtos>\n");
        printf("1 - Limpeza\n");
        printf("2 - Alimentos\n");
        printf("3 - Padaria\n");
        printf("4 - Voltar\n");
        printf("Selecione a listagem que deseja\n");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1: // limpeza
                system("cls");
                printf("Cod | \tNome\t|\tValor\t| Estoque \n");
                for (int i = 0; i < limpeza_cont; i++) {
                    printf("%d - \t%s \t%.2f \t    (%.0f)\n", 
                           limpeza[i]->codigo_produto, 
                           limpeza[i]->nome_produto, 
                           limpeza[i]->valor_venda, 
                           limpeza[i]->estoque_produto);
                }
                system("pause");
                break;
            case 2: // alimentos
                system("cls");
                printf("Cod | \tNome\t|\tValor\t| Estoque \n");
                for (int i = 0; i < alimentos_cont; i++) {
                    printf("%d - \t%s \t%.2f \t    (%.0f)\n", 
                           alimentos[i]->codigo_produto, 
                           alimentos[i]->nome_produto, 
                           alimentos[i]->valor_venda, 
                           alimentos[i]->estoque_produto);
                }
                system("pause");
                break;
            case 3: // padaria
                system("cls");
                printf("Cod | \tNome\t|\tValor\t| Estoque \n");
                for (int i = 0; i < padaria_cont; i++) {
                    printf("%d - \t%s \t%.2f \t    (%.0f)\n", 
                           padaria[i]->codigo_produto, 
                           padaria[i]->nome_produto, 
                           padaria[i]->valor_venda, 
                           padaria[i]->estoque_produto);
                }
                system("pause");
                break;
            case 4: 
                return;
            default: 
                printf("Opcao invalida\n"); 
                break;
        }
    } while (opcao != 4);
}

// Função de acesso administrativo
void acesso_adm() {
    system("cls");
    if (cadastrados[login_code]->nivel_acesso != 1) {
        printf("Voce nao tem permissao para acessar esta area\n");
        system("pause");
        return;
    }
    
    do {
        system("cls");
        printf("1 - Listar usuarios\n");
        printf("2 - Registrar usuario\n");
        printf("3 - Listar produtos\n");
        printf("4 - Registrar produtos\n");
        printf("5 - Voltar\n");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                listar_usuarios();
                break;
            case 2:
                registrar_usuarios();
                break;
            case 3:
                listar_produtos();
                break;
            case 4:
                registrar_produtos();
                break;
            case 5:
                return;
            default:
                system("cls");
                printf("Valor invalido, tente novamente...\n");
                system("pause");
        }
    } while (opcao != 5);
}

void consultar_historico() {
    system("cls");
    printf("=== HISTORICO DE FECHAMENTOS ===\n\n");

    for (int i = 0; i < historico_cont; i++) {
        printf("%d - %s\n", i+1, historico_fechamentos[i].data);
    }

    printf("\nSelecione o fechamento para detalhes (0 para voltar): ");
    int opcao;
    scanf("%d", &opcao);

    if (opcao > 0 && opcao <= historico_cont) {
        system("cls");
        printf("=== FECHAMENTO %s ===\n", historico_fechamentos[opcao-1].data);
        printf("Total Limpeza: R$%.2f\n", historico_fechamentos[opcao-1].total_limpeza);
        printf("Total Alimentos: R$%.2f\n", historico_fechamentos[opcao-1].total_alimentos);
        printf("Total Padaria: R$%.2f\n", historico_fechamentos[opcao-1].total_padaria);
        printf("Faturamento: R$%.2f\n", historico_fechamentos[opcao-1].faturamento);
        printf("Custo Total: R$%.2f\n", historico_fechamentos[opcao-1].custo_total);
        printf("Lucro: R$%.2f (%.1f%%)\n",
               historico_fechamentos[opcao-1].lucro_total,
               historico_fechamentos[opcao-1].margem_lucro);

        printf("\n1 - Ver relatorio completo\n");
        printf("2 - Voltar\n");
        printf("Opcao: ");
        int sub_opcao;
        scanf("%d", &sub_opcao);

        if (sub_opcao == 1) {
            char nome_arquivo[50];
            sprintf(nome_arquivo, "fechamento_%s.txt", historico_fechamentos[opcao-1].data);
            system(nome_arquivo); // Abre o arquivo no visualizador padrão
        }
    }
    system("pause");
}

void gerar_relatorio_fechamento(int index) {
    char nome_arquivo[50];
    sprintf(nome_arquivo, "fechamento_%s.txt", historico_fechamentos[index].data);

    FILE *file = fopen(nome_arquivo, "w");
    if (file == NULL) {
        printf("Erro ao criar relatorio de fechamento!\n");
        return;
    }

    fprintf(file, "=== RELATORIO DE FECHAMENTO ===\n");
    fprintf(file, "Data: %s\n\n", historico_fechamentos[index].data);

    fprintf(file, "=== TOTAIS POR CATEGORIA ===\n");
    fprintf(file, "Limpeza: R$%.2f\n", historico_fechamentos[index].total_limpeza);
    fprintf(file, "Alimentos: R$%.2f\n", historico_fechamentos[index].total_alimentos);
    fprintf(file, "Padaria: R$%.2f\n\n", historico_fechamentos[index].total_padaria);

    fprintf(file, "=== RESUMO FINANCEIRO ===\n");
    fprintf(file, "Faturamento Bruto: R$%.2f\n", historico_fechamentos[index].faturamento);
    fprintf(file, "Custo Total: R$%.2f\n", historico_fechamentos[index].custo_total);
    fprintf(file, "Lucro Líquido: R$%.2f\n", historico_fechamentos[index].lucro_total);
    fprintf(file, "Margem de Lucro: %.1f%%\n\n", historico_fechamentos[index].margem_lucro);

    fprintf(file, "=== PRODUTOS VENDIDOS ===\n");

    // Listar produtos de limpeza vendidos
    fprintf(file, "\nLimpeza:\n");
    for (int i = 0; i < limpeza_cont; i++) {
        int vendidos = limpeza[i]->estoque_inicial - limpeza[i]->estoque_produto;
        if (vendidos > 0) {
            fprintf(file, "%s - %d un - R$%.2f (Lucro: R$%.2f)\n",
                   limpeza[i]->nome_produto,
                   vendidos,
                   vendidos * limpeza[i]->valor_venda,
                   vendidos * (limpeza[i]->valor_venda - limpeza[i]->valor_custo));
        }
    }

    // Repetir para alimentos e padaria

    fclose(file);
    printf("Relatorio de fechamento gerado: %s\n", nome_arquivo);
}

// Menu do caixa
void menu_caixa() {
    float acrescentaEstoque = 0;

    do {
        system("cls");
        printf("< Menu Caixa >\n");
        printf("1 - Abrir\n");
        printf("2 - Fechar\n");
        printf("3 - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                if (status_caixa == true) {
                    printf("\nO caixa ja esta aberto, feche para realizar uma nova abertura");
                    system("pause");
                    break;
                }
                printf("\nInforme o valor de abertura\n");
                printf("Valor: ");
                scanf("%f", &valor_abertura);
                if(valor_abertura < 0){
                    printf("Valor inserido invalido\n");
                    system("pause");
                    break;
                }
                printf("\nInforme o estoque da padaria\n");
                for (int i = 0; i < padaria_cont; i++) {
                    do {
                        printf("\nInforme a quantidade de %s em estoque: ", padaria[i]->nome_produto);
                        scanf("%f", &acrescentaEstoque);
                        if (acrescentaEstoque < 0)
                            printf("Informe um valor valido\n");
                    } while (acrescentaEstoque < 0);
                    padaria[i]->estoque_produto = acrescentaEstoque;
                }
                status_caixa = true;
                printf("O caixa foi aberto!\n");
                system("pause");
                break;
                case 2: // Fechar caixa
        if (status_caixa == true) {
            // Calcular custos e lucros
            float custo_total = 0;
            float lucro_total = 0;

            // Calcular para limpeza
            for (int i = 0; i < limpeza_cont; i++) {
                float vendidos = limpeza[i]->estoque_inicial - limpeza[i]->estoque_produto;
                custo_total += vendidos * limpeza[i]->valor_custo;
                lucro_total += vendidos * (limpeza[i]->valor_venda - limpeza[i]->valor_custo);
            }

            // Calcular para alimentos e padaria (mesma lógica)

            float margem = (lucro_total / custo_total) * 100;

            // Registrar no histórico
            historico_cont++;
            historico_fechamentos = realloc(historico_fechamentos, historico_cont * sizeof(fechamento_diario));

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(historico_fechamentos[historico_cont-1].data, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

            historico_fechamentos[historico_cont-1].total_limpeza = total_limpeza;
            historico_fechamentos[historico_cont-1].total_alimentos = total_alimentos;
            historico_fechamentos[historico_cont-1].total_padaria = total_padaria;
            historico_fechamentos[historico_cont-1].faturamento = faturamento;
            historico_fechamentos[historico_cont-1].custo_total = custo_total;
            historico_fechamentos[historico_cont-1].lucro_total = lucro_total;
            historico_fechamentos[historico_cont-1].margem_lucro = margem;

            // Exibir resumo
            system("cls");
            printf("=== FECHAMENTO DO DIA ===\n");
            printf("Data: %s\n", historico_fechamentos[historico_cont-1].data);
            printf("Total Limpeza: R$%.2f\n", total_limpeza);
            printf("Total Alimentos: R$%.2f\n", total_alimentos);
            printf("Total Padaria: R$%.2f\n", total_padaria);
            printf("Faturamento Total: R$%.2f\n", faturamento);
            printf("Custo Total: R$%.2f\n", custo_total);
            printf("Lucro Total: R$%.2f\n", lucro_total);
            printf("Margem de Lucro: %.1f%%\n", margem);

            // Gerar relatório
            gerar_relatorio_fechamento(historico_cont-1);

            // Resetar valores
            total_limpeza = total_alimentos = total_padaria = faturamento = valor_abertura = 0;
            status_caixa = false;

            system("pause");
        }
        break;
            case 3:
                return;
            default:
                printf("Opcao invalida\n");
                system("pause");
                break;
        }
    } while(opcao != 3);
}

// Menu para cancelar venda
void menu_cancelar() {
    system("cls");
    printf("Deseja realmente cancelar as vendas?\n");
    printf("1 - Confirmar\n");
    printf("2 - Voltar\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    
    switch (opcao) {
        case 1:
            // Devolver produtos ao estoque
            for (int i = 0; i < limpeza_cont; i++) {
                limpeza[i]->estoque_produto += estoque_temp[i];
                estoque_temp[i] = 0;
            }
            
            for (int i = 0; i < alimentos_cont; i++) {
                alimentos[i]->estoque_produto += estoque_temp[i + 7];
                estoque_temp[i + 7] = 0;
            }
            
            for (int i = 0; i < padaria_cont; i++) {
                padaria[i]->estoque_produto += estoque_temp[i + 14];
                estoque_temp[i + 14] = 0;
            }
            
            carrinho_limpeza = carrinho_padaria = carrinho_alimentos = 0;
            printf("\nVenda cancelada! Produtos devolvidos ao estoque\n");
            system("pause");
            break;
        case 2:
            return;
        default:
            printf("Opcao invalida\n");
            system("pause");
            break;
    }
}

// Função para aplicar desconto
float aplicar_desconto(float x) {
    if (x <= 50) {
        guarda_desconto = x * 0.05;
        printf("\nDesconto de 5%% aplicado\n");
    } else if (x <= 200) {
        guarda_desconto = x * 0.10;
        printf("\nDesconto de 10%% aplicado\n");
    } else {
        printf("Informe o desconto a ser aplicado (em %%): ");
        scanf("%f", &desconto_informado);
        guarda_desconto = x * (desconto_informado / 100);
        printf("\nDesconto de %.0f%% aplicado\n", desconto_informado);
    }
    return total_desconto = x - guarda_desconto;
}

// Menu para pagamento pendente
void menu_falta() {
    do {
        system("cls");
        printf("Menu Pagamento Pendente\n");
        printf("1 - Dinheiro\n");
        printf("2 - Cartao\n");
        printf("3 - Voltar\n");
        printf("Opcao:");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("\nFalta pagar: %.2f R$\n", falta);
                printf("Informe o valor pago: ");
                scanf("%f", &pagamento);
                
                if (pagamento > 0) {
                    if (pagamento >= falta) {
                        printf("\nPagamento realizado com sucesso!\n");
                        total_alimentos += carrinho_alimentos;
                        total_padaria += carrinho_padaria;
                        total_limpeza += carrinho_limpeza;
                        faturamento += falta;
                        troco = pagamento - falta;
                        carrinho_alimentos = carrinho_padaria = carrinho_limpeza = total = total_desconto = falta = 0;
                        if (troco != 0)
                            printf("Troco: %.2f R$\n", troco);
                        system("pause");
                        return;
                    } else if (pagamento < falta && pagamento > 0) {
                        faturamento += pagamento;
                        falta = falta - pagamento;
                        printf("Ainda restou valores\n");
                        system("pause");
                    }
                } else {
                    printf("Valor invalido, tente novamente...\n");
                    system("pause");
                }
                break;
            case 2:
                printf("1 - Pagamento realizado\n");
                printf("2 - Pagamento nao realizado\n");
                printf("3 - Voltar\n");
                printf("Opcao: ");
                scanf("%d", &opcao);
                
                if (opcao == 1) {
                    printf("Pagamento Realizado!\n");
                    total_alimentos += carrinho_alimentos;
                    total_padaria += carrinho_padaria;
                    total_limpeza += carrinho_limpeza;
                    faturamento += falta;
                    carrinho_alimentos = carrinho_padaria = carrinho_limpeza = total = total_desconto = falta = 0;
                    system("pause");
                    return;
                } else if (opcao == 2) {
                    printf("Pagamento Nao Realizado\n");
                    system("pause");
                } else if (opcao == 3) {
                    continue;
                } else {
                    printf("Opcao invalida\n");
                    system("pause");
                }
                break;
            case 3:
                return;
            default:
                printf("Opcao invalida\n");
                system("pause");
                break;
        }
    } while (opcao != 3);
}

// Menu de pagamento
void menu_pagamento() {
    total = carrinho_alimentos + carrinho_padaria + carrinho_limpeza;
    
    do {
        system("cls");
        printf("<Menu Pagamento>\n");
        printf("1 - Dinheiro\n");
        printf("2 - Cartao\n");
        printf("3 - Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (falta == 0) {
                    if (total > 0) {
                        total_desconto = aplicar_desconto(total);
                        printf("Valor total: %.2f R$\n", total);
                        printf("Valor com desconto: %.2f R$\n", total_desconto);
                        printf("Informe o valor recebido: ");
                        scanf("%f", &pagamento);
                    } else {
                        printf("Nenhum item no carrinho!\n");
                        system("pause");
                        return;
                    }
                } else {
                    menu_falta();
                    return;
                }
                
                if (pagamento < 0) {
                    printf("Valor invalido, tente novamente...\n");
                    system("pause");
                    continue;
                }

                if (pagamento >= total_desconto) {
                    printf("\nPagamento Realizado!\n");
                    total_alimentos += carrinho_alimentos;
                    total_padaria += carrinho_padaria;
                    total_limpeza += carrinho_limpeza;
                    faturamento += total_desconto;
                    troco = pagamento - total_desconto;
                    carrinho_alimentos = carrinho_padaria = carrinho_limpeza = total = total_desconto = 0;
                    if (troco != 0)
                        printf("Troco: %.2f R$\n", troco);
                    system("pause");
                    return;
                } else if (pagamento > 0 && pagamento < total_desconto) {
                    printf("\nO valor integral sera cobrado\n");
                    faturamento += pagamento;
                    falta = total - pagamento;
                    system("pause");
                    menu_falta();
                    return;
                }
                break;
            case 2:
                printf("1 - Pagamento realizado\n");
                printf("2 - Pagamento nao realizado\n");
                printf("3 - Voltar\n");
                printf("Opcao: ");
                scanf("%d", &opcao);
                
                if (opcao == 1) {
                    printf("Pagamento Realizado!\n");
                    total_alimentos += carrinho_alimentos;
                    total_padaria += carrinho_padaria;
                    total_limpeza += carrinho_limpeza;
                    faturamento += total;
                    carrinho_alimentos = carrinho_padaria = carrinho_limpeza = total = total_desconto = 0;
                    system("pause");
                    return;
                } else if (opcao == 2) {
                    printf("Pagamento Nao Realizado\n");
                    system("pause");
                } else if (opcao == 3) {
                    continue;
                } else {
                    printf("Opcao invalida\n");
                    system("pause");
                }
                break;
            case 3:
                return;
            default:
                printf("Opcao invalida\n");
                system("pause");
                break;
        }
    } while(opcao != 3);
}

// Menu da padaria
void menu_padaria() {
    do {
        system("cls");
        printf("<Menu Padaria>\n");
        printf("Cod | \tNome\t|\tValor\t| Estoque \n");
        for (int i = 0; i < padaria_cont; i++) {
            printf("%d - \t%s \t%.2f \t    (%.0f)\n", 
                   padaria[i]->codigo_produto, 
                   padaria[i]->nome_produto, 
                   padaria[i]->valor_venda, 
                   padaria[i]->estoque_produto);
        }
        printf("%d - Voltar\n", padaria_cont + 1);
        printf("Carrinho Padaria: %.2f R$\n", carrinho_padaria);
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao >= 1 && opcao <= padaria_cont) {
            int index = opcao - 1;
            printf("\n%s\n", padaria[index]->nome_produto);
            printf("Informe a quantidade: ");
            scanf("%d", &quantidade);
            
            if (quantidade < 0) {
                printf("Quantidade invalida... tente novamente");
                system("pause");
                continue;
            }
            
            if (quantidade > padaria[index]->estoque_produto) {
                printf("\nEstoque insuficiente, tente novamente\n");
                system("pause");
                continue;
            }
            
            padaria[index]->estoque_produto -= quantidade;
            estoque_temp[14 + index] += quantidade;
            carrinho_padaria += padaria[index]->valor_venda * quantidade;
            printf("Adicionado %d x %s ao carrinho\n", quantidade, padaria[index]->nome_produto);
            system("pause");
        } else if (opcao == padaria_cont + 1) {
            return;
        } else {
            printf("Opcao invalida... tente novamente\n");
            system("pause");
        }
    } while(1);
}

// Menu de alimentos
void menu_alimento() {
    do {
        system("cls");
        printf("<Menu Alimento>\n");
        printf("Cod | \tNome\t|\tValor\t| Estoque \n");
        for (int i = 0; i < alimentos_cont; i++) {
            printf("%d - \t%s \t%.2f \t    (%.0f)\n", 
                   alimentos[i]->codigo_produto, 
                   alimentos[i]->nome_produto, 
                   alimentos[i]->valor_venda, 
                   alimentos[i]->estoque_produto);
        }
        printf("%d - Voltar\n", alimentos_cont + 1);
        printf("Carrinho Alimentos: %.2f R$\n", carrinho_alimentos);
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao >= 1 && opcao <= alimentos_cont) {
            int index = opcao - 1;
            printf("\n%s\n", alimentos[index]->nome_produto);
            printf("Informe a quantidade: ");
            scanf("%d", &quantidade);
            
            if (quantidade < 0) {
                printf("Quantidade invalida... tente novamente");
                system("pause");
                continue;
            }
            
            if (quantidade > alimentos[index]->estoque_produto) {
                printf("\nEstoque insuficiente, tente novamente\n");
                system("pause");
                continue;
            }
            
            alimentos[index]->estoque_produto -= quantidade;
            estoque_temp[7 + index] += quantidade;
            carrinho_alimentos += alimentos[index]->valor_venda * quantidade;
            printf("Adicionado %d x %s ao carrinho\n", quantidade, alimentos[index]->nome_produto);
            system("pause");
        } else if (opcao == alimentos_cont + 1) {
            return;
        } else {
            printf("Opcao invalida... tente novamente\n");
            system("pause");
        }
    } while(1);
}

// Menu de limpeza
void menu_limpeza() {
    do {
        system("cls");
        printf("<Menu Limpeza>\n");
        printf("Cod | \tNome\t|\tValor\t| Estoque \n");
        for (int i = 0; i < limpeza_cont; i++) {
            printf("%d - \t%s \t%.2f \t    (%.0f)\n", 
                   limpeza[i]->codigo_produto, 
                   limpeza[i]->nome_produto, 
                   limpeza[i]->valor_venda, 
                   limpeza[i]->estoque_produto);
        }
        printf("%d - Voltar\n", limpeza_cont + 1);
        printf("Carrinho Limpeza: %.2f R$\n", carrinho_limpeza);
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao >= 1 && opcao <= limpeza_cont) {
            int index = opcao - 1;
            printf("\n%s\n", limpeza[index]->nome_produto);
            printf("Informe a quantidade: ");
            scanf("%d", &quantidade);
            
            if (quantidade < 0) {
                printf("Quantidade invalida... tente novamente");
                system("pause");
                continue;
            }
            
            if (quantidade > limpeza[index]->estoque_produto) {
                printf("\nEstoque insuficiente, tente novamente\n");
                system("pause");
                continue;
            }
            
            limpeza[index]->estoque_produto -= quantidade;
            estoque_temp[index] += quantidade;
            carrinho_limpeza += limpeza[index]->valor_venda * quantidade;
            printf("Adicionado %d x %s ao carrinho\n", quantidade, limpeza[index]->nome_produto);
            system("pause");
        } else if (opcao == limpeza_cont + 1) {
            return;
        } else {
            printf("Opcao invalida... tente novamente\n");
            system("pause");
        }
    } while(1);
}

// Menu principal
void menu_principal() {
    do {
        system("cls");
        printf("<Menu Principal>\n");
        printf("1 - Limpeza\n");
        printf("2 - Alimento\n");
        printf("3 - Padaria\n");
        printf("4 - Pagamento\n");
        printf("5 - Caixa\n");
        printf("6 - Cancelar\n");
        printf("7 - Consultar Historico de Fechamentos\n");
        printf("8 - Gerar Relatorio de Vendas Atual\n");
        printf("9 - Voltar\n");
        printf("Carrinho total: %.2f R$\n", (carrinho_alimentos + carrinho_padaria + carrinho_limpeza));
        printf("Opcao: ");
        scanf("%d", &opcao);
        
        if (!status_caixa && opcao != 5 && opcao != 7 && opcao != 8) {
            printf("O caixa esta fechado, realize a abertura!\n");
            system("pause");
            continue;
        }
        
        switch (opcao) {
            case 1: menu_limpeza(); break;
            case 2: menu_alimento(); break;
            case 3: menu_padaria(); break;
            case 4: menu_pagamento(); break;
            case 5: menu_caixa(); break;
            case 6: menu_cancelar(); break;
            case 7: consultar_historico(); break;
            case 8: gerar_relatorio_vendas(); break;
            case 9: return;
            default:
                printf("Opcao invalida... tente novamente\n");
                system("pause");
                break;
        }
    } while(opcao != 8);
}

// Função principal
int main() {
    system("color 0a");
    inicializar_sistema();
    login_acesso();
    liberar_memoria();
    return 0;
}