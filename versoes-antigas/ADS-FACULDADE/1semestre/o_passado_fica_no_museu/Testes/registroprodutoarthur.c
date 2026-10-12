#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> // Para system()
#include <unistd.h> // Para sleep()

// Contadores de usuários e produtos
int cadastradosLimpeza = 1, cadastradosAlimentos = 1, cadastradosPadaria = 1, contasAdm = 1, contasCaixa = 1;
bool administrativo = false;

// Estrutura de usuários
struct usuarios {
    char login[50], senha[50];
};

// Arrays de administradores e colaboradores
struct usuarios caixa[100] = {{"caixa1", "123"}};
struct usuarios adm[100] = {{"admin", "admin"}};

// Estrutura de produtos
struct produto {
    int codigoProduto;
    char nomeProduto[50];
    float valorProduto;
    int estoqueProduto;
};

// Arrays de produtos por categoria
struct produto limpeza[100] = {{11, "Detergente", 1.99, 50}};
struct produto alimentos[100] = {{21, "Cafe", 19.99, 10}};
struct produto padaria[100] = {{31, "Pao de forma PCT", 9.50, 0}};

// Prototipação
void inicio();
void menuAdministrativo();
void acessoAdministrativo();
void listarProdutos();
void registrarProduto();
void registrarUsuario();
void loginSistema();

// Cadastro de novo usuário
void registrarUsuario() {
    int codigoUsuario;
    system("cls");
    printf("Registro de usuarios\n");
    printf("1 - Registrar novo colaborador\n");
    printf("2 - Registrar novo administrador\n");
    printf("3 - Voltar\n");
    printf("Selecione a opcao: ");
    scanf("%d", &codigoUsuario);
    getchar(); // Limpa o buffer do Enter

    switch (codigoUsuario) {
        case 1:
            printf("Cadastro de colaborador\n");
            printf("Informe o usuario: ");
            fgets(caixa[contasCaixa].login, sizeof(caixa[contasCaixa].login), stdin);
            caixa[contasCaixa].login[strcspn(caixa[contasCaixa].login, "\n")] = '\0';

            printf("Informe a senha: ");
            fgets(caixa[contasCaixa].senha, sizeof(caixa[contasCaixa].senha), stdin);
            caixa[contasCaixa].senha[strcspn(caixa[contasCaixa].senha, "\n")] = '\0';

            contasCaixa++;
            printf("\nNovo colaborador cadastrado!");
            sleep(2);
            registrarUsuario();
            break;

        case 2:
            printf("Cadastro de administrador\n");
            printf("Informe o usuario: ");
            fgets(adm[contasAdm].login, sizeof(adm[contasAdm].login), stdin);
            adm[contasAdm].login[strcspn(adm[contasAdm].login, "\n")] = '\0';

            printf("Informe a senha: ");
            fgets(adm[contasAdm].senha, sizeof(adm[contasAdm].senha), stdin);
            adm[contasAdm].senha[strcspn(adm[contasAdm].senha, "\n")] = '\0';

            contasAdm++;
            printf("\nNovo administrador cadastrado!");
            sleep(2);
            registrarUsuario();
            break;

        case 3:
            inicio();
            break;

        default:
            printf("Opcao invalida!\n");
            sleep(1);
            registrarUsuario();
            break;
    }
}

// Cadastro de novo produto
void registrarProduto() {
    int codigoCategoria;
    system("cls");
    printf("Registro de produtos\n");
    printf("1 - Limpeza\n");
    printf("2 - Alimentos\n");
    printf("3 - Padaria\n");
    printf("4 - Lista de produtos\n");
    printf("5 - Voltar\n");
    printf("Selecione a categoria: ");
    scanf("%d", &codigoCategoria);
    getchar(); // limpa buffer

    switch (codigoCategoria) {
        case 1:
            printf("Informe o codigo do produto: ");
            scanf("%d", &limpeza[cadastradosLimpeza].codigoProduto);
            getchar();

            printf("Informe o nome do produto: ");
            fgets(limpeza[cadastradosLimpeza].nomeProduto, sizeof(limpeza[cadastradosLimpeza].nomeProduto), stdin);
            limpeza[cadastradosLimpeza].nomeProduto[strcspn(limpeza[cadastradosLimpeza].nomeProduto, "\n")] = '\0';

            printf("Informe o valor do produto: ");
            scanf("%f", &limpeza[cadastradosLimpeza].valorProduto);

            printf("Informe o estoque do produto: ");
            scanf("%d", &limpeza[cadastradosLimpeza].estoqueProduto);

            cadastradosLimpeza++;
            printf("Produto registrado com sucesso!\n");
            sleep(2);
            registrarProduto();
            break;

        case 2:
            printf("Informe o codigo do produto: ");
            scanf("%d", &alimentos[cadastradosAlimentos].codigoProduto);
            getchar();

            printf("Informe o nome do produto: ");
            fgets(alimentos[cadastradosAlimentos].nomeProduto, sizeof(alimentos[cadastradosAlimentos].nomeProduto), stdin);
            alimentos[cadastradosAlimentos].nomeProduto[strcspn(alimentos[cadastradosAlimentos].nomeProduto, "\n")] = '\0';

            printf("Informe o valor do produto: ");
            scanf("%f", &alimentos[cadastradosAlimentos].valorProduto);

            printf("Informe o estoque do produto: ");
            scanf("%d", &alimentos[cadastradosAlimentos].estoqueProduto);

            cadastradosAlimentos++;
            sleep(2);
            registrarProduto();
            break;

        case 3:
            printf("Informe o codigo do produto: ");
            scanf("%d", &padaria[cadastradosPadaria].codigoProduto);
            getchar();

            printf("Informe o nome do produto: ");
            fgets(padaria[cadastradosPadaria].nomeProduto, sizeof(padaria[cadastradosPadaria].nomeProduto), stdin);
            padaria[cadastradosPadaria].nomeProduto[strcspn(padaria[cadastradosPadaria].nomeProduto, "\n")] = '\0';

            printf("Informe o valor do produto: ");
            scanf("%f", &padaria[cadastradosPadaria].valorProduto);

            printf("Informe o estoque do produto: ");
            scanf("%d", &padaria[cadastradosPadaria].estoqueProduto);

            cadastradosPadaria++;
            sleep(2);
            registrarProduto();
            break;

        case 4:
            listarProdutos();
            break;

        case 5:
            inicio();
            break;

        default:
            printf("Opcao invalida!\n");
            sleep(1);
            registrarProduto();
            break;
    }
}

// Lista produtos por categoria
void listarProdutos() {
    int codigoLista;
    system("cls");
    printf("Lista de produtos\n");
    printf("1 - Limpeza\n");
    printf("2 - Alimentos\n");
    printf("3 - Padaria\n");
    scanf("%d", &codigoLista);

    switch (codigoLista) {
        case 1:
            printf("Limpeza\n");
            for (int i = 0; i < cadastradosLimpeza; i++) {
                printf("Codigo: %d | Nome: %-23s | Valor: %.2f | Estoque: (%d)\n",
                    limpeza[i].codigoProduto, limpeza[i].nomeProduto, limpeza[i].valorProduto, limpeza[i].estoqueProduto);
            }
            break;

        case 2:
            printf("Alimentos\n");
            for (int i = 0; i < cadastradosAlimentos; i++) {
                printf("Codigo: %d | Nome: %-23s | Valor: %.2f | Estoque: (%d)\n",
                    alimentos[i].codigoProduto, alimentos[i].nomeProduto, alimentos[i].valorProduto, alimentos[i].estoqueProduto);
            }
            break;

        case 3:
            printf("Padaria\n");
            for (int i = 0; i < cadastradosPadaria; i++) {
                printf("Codigo: %d | Nome: %-23s | Valor: %.2f | Estoque: (%d)\n",
                    padaria[i].codigoProduto, padaria[i].nomeProduto, padaria[i].valorProduto, padaria[i].estoqueProduto);
            }
            break;

        default:
            printf("Opcao invalida!\n");
            break;
    }

    sleep(2);
    menuAdministrativo();
}

// Menu administrativo
void menuAdministrativo() {
    int codigoMenu;
    system("cls");
    printf("Menu Administrativo\n");
    printf("1 - Produtos\n");
    printf("2 - Usuarios\n");
    printf("3 - Voltar\n");
    scanf("%d", &codigoMenu);

    switch (codigoMenu) {
        case 1:
            registrarProduto();
            break;
        case 2:
            registrarUsuario();
            break;
        case 3:
            administrativo = false;
            acessoAdministrativo();
            break;
        default:
            printf("Opcao invalida!\n");
            sleep(1);
            menuAdministrativo();
            break;
    }
}

// Login de administrador
void acessoAdministrativo() {
    char loginadm[50];
    char senhaadm[50];
    system("cls");
    printf("<Login administrativo>\n");
    printf("Login: ");
    fgets(loginadm, sizeof(loginadm), stdin);
    loginadm[strcspn(loginadm, "\n")] = '\0';

    printf("Senha: ");
    fgets(senhaadm, sizeof(senhaadm), stdin);
    senhaadm[strcspn(senhaadm, "\n")] = '\0';

    bool loginValido = false;

    for (int i = 0; i < contasAdm; i++) {
        if (strcmp(loginadm, adm[i].login) == 0 && strcmp(senhaadm, adm[i].senha) == 0) {
            loginValido = true;
            break;
        }
    }

    if (loginValido) {
        printf("Login realizado com sucesso!\n");
        administrativo = true;
        sleep(2);
        menuAdministrativo();
    } else {
        printf("Login ou senha invalidos!\n");
        sleep(2);
        acessoAdministrativo();
    }
}

// Login de colaborador
void loginSistema() {
    char loginCaixa[50];
    char senhaCaixa[50];

    printf("Acesso colaborador\n");
    printf("Login: ");
    fgets(loginCaixa, sizeof(loginCaixa), stdin);
    loginCaixa[strcspn(loginCaixa, "\n")] = '\0';

    printf("Senha: ");
    fgets(senhaCaixa, sizeof(senhaCaixa), stdin);
    senhaCaixa[strcspn(senhaCaixa, "\n")] = '\0';

    bool loginValido = false;

    for (int j = 0; j < contasCaixa; j++) {
        if (strcmp(loginCaixa, caixa[j].login) == 0 && strcmp(senhaCaixa, caixa[j].senha) == 0) {
            loginValido = true;
            break;
        }
    }

    if (loginValido) {
        printf("Login realizado com sucesso!\n");
        sleep(2);
        acessoAdministrativo();
    } else {
        printf("Login ou senha invalidos!\n");
        sleep(2);
        loginSistema();
    }
}

// Início do programa
void inicio() {
    loginSistema();
}

// Função principal
int main() {
    inicio();
    return 0;
}
