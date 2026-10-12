#include <stdio.h>
#include <windows.h>

void getPassword(char *password, int maxLength) {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    DWORD count;
    char ch;
    int i = 0;

    // Obtém o modo atual do console
    GetConsoleMode(hStdin, &mode);
    // Desativa o eco
    SetConsoleMode(hStdin, mode & ~(ENABLE_ECHO_INPUT));

    // Lê a senha
    while (i < maxLength - 1 && ReadConsole(hStdin, &ch, 1, &count, NULL) && ch != '\n' && ch != '\r') {
        password[i++] = ch;
        // Opcional: exibe asteriscos para cada caractere digitado
        printf("*");
    }
    password[i] = '\0';

    // Restaura o modo do console
    SetConsoleMode(hStdin, mode);
    printf("\n");
}

int main() {
    char password[128];
    printf("Enter password: ");
    getPassword(password, 128);
    printf("Password entered: %s\n", password);
    return 0;
}
