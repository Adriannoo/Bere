#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>


int cadastradosLimpeza = 1, cadastradosAlimentos = 1, cadastradosPadaria = 1;
bool administrativo = false;
char loginadm[50], senhaadm[50];

struct produto{
int codigoProduto;
char nomeProduto[50];
float valorProduto;
int estoqueProduto;
};

struct produto limpeza[100] = {{11,"Detergente",1.99,50}};
struct produto alimentos[100] = {{21,"Cafe",19.99,10}};
struct produto padaria[100] = {{31,"Pao de forma PCT",9.50,0}};

void registrarProduto(){
    int codigoCategoria;
    system("cls");
    printf("Registro de produtos\n");
    printf("1 - Limpeza\n");
    printf("2 - Alimentos\n");
    printf("3 - Padaria\n");
    printf("4 - Voltar\n");
    printf("Selecione a categoria: ");
    scanf("%d", &codigoCategoria);

    switch(codigoCategoria){
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
                printf("Produtos registrado com sucesso!!!");
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
            inicio();
    }
}

void listarProdutos(){
    int codigoLista;
    system("cls");
    printf("Lista de produtos\n");
    printf("1 - Limpeza\n");
    printf("2 - Alimentos\n");
    printf("3 - Padaria\n");
    scanf("%d", &codigoLista);
    switch(codigoLista){
    case 1:
        printf("Limpeza\n");
            for(int i = 0; i < cadastradosLimpeza; i++){
                    printf("Codigo: %d Nome: %-23s Valor: %.2f Estoque: (%d)\n",
                       limpeza[i].codigoProduto, limpeza[i].nomeProduto, limpeza[i].valorProduto, limpeza[i].estoqueProduto);
    }
        sleep(2);
        menuAdministrativo();
        break;
    case 2:
        printf("Alimentos\n");
            for(int j = 0; j < cadastradosAlimentos; j++){
                printf("Codigo: %d Nome: %-23s Valor: %.2f Estoque: (%d)\n", alimentos[j].codigoProduto, alimentos[j].nomeProduto, alimentos[j].valorProduto, alimentos[j].estoqueProduto);
    }
        sleep(2);
        menuAdministrativo();
        break;
    case 3:
        printf("Padaria\n");
            for(int k = 0; k < cadastradosPadaria; k++){
                printf("Codigo: %d Nome: %s Valor: %.2f Estoque: (%d)\n", padaria[k].codigoProduto, padaria[k].nomeProduto, padaria[k].valorProduto, padaria[k].estoqueProduto);
        }
        sleep(2);
        menuAdministrativo();
        break;
    }
}

void menuAdministrativo(){
    int codigoMenu;
    system("cls");
    printf("Menu Administrativo\n");
    printf("1 - Registrar Produtos\n");
    printf("2 - Listar Produtos\n");
    printf("3 - Voltar\n");
    scanf("%d", &codigoMenu);
    switch(codigoMenu){
        case 1:
            registrarProduto();
        break;
        case 2:
            listarProdutos();
        break;
        case 3:
            administrativo = false;
            acessoAdministrativo();
        break;
    }
}

void acessoAdministrativo(){
    system("cls");
    printf("<Login adminstrativo>\n");
        printf("Login: ");
            fgets(loginadm, sizeof(loginadm), stdin);
            loginadm[strcspn(loginadm, "\n")] = '\0';
        printf("Senha: ");
            fgets(senhaadm, sizeof(senhaadm), stdin);
            senhaadm[strcspn(senhaadm, "\n")] = '\0';
            if (strcmp(loginadm, "admin") == 0 && strcmp(senhaadm, "admin") == 0){
                printf("Login realizado com sucesso!\n");
                printf("Liberado acesso adminstrativo");
                administrativo = true;
                sleep(2);
                menuAdministrativo();
                }else{
                    printf("Login e/ou senha invalidos!\n");
                    printf("Tente novamente...");
                    sleep(2);
                    acessoAdministrativo();
                }

}

void inicio(){
    acessoAdministrativo();
}

int main(){
    inicio();
    return 0;
}


