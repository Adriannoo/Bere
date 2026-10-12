#include <stdio.h>
#include <string.h>
#include <windows.h>


void menuPrincipal(void);
void listarProduto(void);
void registroProduto(void);
int quantidadeProdutos;

struct produto{
int codigoProduto;
char nomeProduto[50];
float valorProduto;
int estoqueProduto;
};
struct produto novo[300];

void menuPrincipal(){
    int codigoMenu= 0;
    printf("1- Cadastrar Produto\n");
    printf("2- Listar Produtos\n");
    printf("3- Retornar\n");
        scanf("%d",&codigoMenu);
        switch(codigoMenu){
        case 1:
            registroProduto();
            break;
        case 2:
            listarProduto();
            break;
        case 3:
            return;
            break;
        }

}

void listarProduto(){
    for(int i = 0; i < quantidadeProdutos; i++){
     printf("Codigo: %d \tNome: %s \tValor: %.2f \tEstoque: %d\n", novo[i].codigoProduto, novo[i].nomeProduto, novo[i].valorProduto, novo[i].estoqueProduto);}
     sleep(5);
     menuPrincipal();

}

void registroProduto(){

    int i = quantidadeProdutos;
    printf("Informe o codigo: ");
        scanf("%d",&novo[i].codigoProduto);
        getchar();
    printf("Informe o nome do produto: ");
        fgets(novo[i].nomeProduto, sizeof(novo[i].nomeProduto), stdin);
        novo[i].nomeProduto[strcspn(novo[i].nomeProduto, "\n")] = '\0';
    printf("Informe o valor do produto: ");
        scanf("%f",&novo[i].valorProduto);
    printf("Informe a quantidade de estoque: ");
        scanf("%d",&novo[i].estoqueProduto);

        printf("Produto registrado!\n");
        printf("Codigo: %d \nNome: %s \nValor: %.2f \nEstoque: %d\n", novo[i].codigoProduto, novo[i].nomeProduto, novo[i].valorProduto, novo[i].estoqueProduto);
        quantidadeProdutos++;
        menuPrincipal();


}


int main(){
    menuPrincipal();

    return 0;
}
