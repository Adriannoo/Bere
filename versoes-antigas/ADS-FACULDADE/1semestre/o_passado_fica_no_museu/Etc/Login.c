#include <stdio.h>
#include <stdbool.h>


struct registro{

    char login[50];
    char senha[50];
    bool permissaoAdm;

};

struct registro usuario[200]= {
    {"admin", "admin123", true},    // Admin com permissão
    {"usuario1", "senha1", false},  // Usuário comum
    {"usuario2", "senha2", false},
    {"usuario3", "senha3", false},
    {"usuario4", "senha4", false},
    {"usuario5", "senha5", false},
    {"usuario6", "senha6", false},
    {"usuario7", "senha7", false},
    {"usuario8", "senha8", false},
    {"usuario9", "senha9", false},
    {"usuario10", "senha10", false},
    {"usuario11", "senha11", false},
    {"usuario12", "senha12", false},
    {"usuario13", "senha13", false},
    {"usuario14", "senha14", false},
    {"usuario15", "senha15", false},
    {"usuario16", "senha16", false},
    {"usuario17", "senha17", false},
    {"usuario18", "senha18", false},
    {"usuario19", "senha19", false},
    {"usuario20", "senha20", false}  // Usuário comum
};

int registrados = 19;



void registroUsuario(){

    system("cls");
    int permissao = 0;
    printf("Login: ");
    fgets(usuario[registrados].login, sizeof(usuario[registrados].login), stdin);
    usuario[registrados].login[strcspn(usuario[registrados].login, "\n")] = '\0';
    printf("Senha: ");
    fgets(usuario[registrados].senha, sizeof(usuario[registrados].senha), stdin);
    usuario[registrados].senha[strcspn(usuario[registrados].senha, "\n")] = '\0';
    printf("Permissao Administrativa\n");
    printf("1- SIM ou 0- NAO\n");
    printf("Opcao: ");
    scanf("%d", &permissao);

    if(permissao == 1){
        usuario[registrados].permissaoAdm = true;
    } else {
    usuario[registrados].permissaoAdm = false;
    }




    if(usuario[registrados].permissaoAdm == 1){
        printf("Usuario Admnistrativo cadastrado com sucesso!\n");
        printf("Login: %s\n", usuario[registrados].login);
        printf("Senha: %s\n", usuario[registrados].senha);
        system("pause");
        registrados++;
        menuPrincipal();
    } else {
        printf("Usuario Colaborador cadastrado com sucesso!\n");
        printf("Login: %s\n", usuario[registrados].login);
        printf("Senha: %s\n", usuario[registrados].senha);
        system("pause");
        registrados++;
        menuPrincipal();
    }


}
void menuPrincipal(){
    printf("Menu Principal\n");
    exit(0);
}

void login(){

    char logintemp[50];
    char senhatemp[50];
    int i = 0;
    bool logado = false;
while(!logado){
    system("cls");
    printf("Login: ");
    fgets(logintemp, sizeof(logintemp), stdin);
    logintemp[strcspn(logintemp, "\n")] = '\0';
    printf("Senha: ");
    fgets(senhatemp, sizeof(senhatemp), stdin);
    senhatemp[strcspn(senhatemp, "\n")] = '\0';

    for(i = 0; i < registrados; i++){
    if(strcmp(logintemp, usuario[i].login) == 0 && strcmp(senhatemp, usuario[i].senha) == 0){
        printf("Login Realizado com sucesso!\n");
        logado = true;
        system("pause");
        menuPrincipal();
        }
    }
        printf("Login e/ou senha invalidos! Tente novamente..\n");
        system("pause");
  }
}

int main(){
    login();
return 0;
}
