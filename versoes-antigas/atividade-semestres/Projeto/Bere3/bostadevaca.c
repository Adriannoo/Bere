#include <stdio.h>

char opcao;

void menuPrincipal(void);
void cadastrar(void);

void cadastrar() {

}

void menuPrincipal() {
    printf("\nMenu Principal\n\n");
    printf("1. CADASTROS\n");
    printf("2. VENDAS\n");
    printf("3. ABERTURA DE CAIXA\n");
    printf("4. FECHAMENTO DE CAIXA\n");
    printf("5. RELATORIOS\n");
    printf("6. SAIR\n");

    switch(opcao) {
        case 1:
            cadastrar();
            break;

        case 2:
            break;

        case 3:
            break;

        default:
            printf("ERRO");
            break;
     }

}

int main() {

    return 0;
}