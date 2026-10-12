#include "plataforma.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

bool prepararPastaDados(void) {
#ifdef _WIN32
    int resultado = _mkdir("dados");
#else
    int resultado = mkdir("dados", 0700);
#endif
    if (resultado == 0) return true;
    if (errno == EEXIST) {
        struct stat pasta;
        if (stat("dados", &pasta) == 0 && (pasta.st_mode & S_IFDIR)) return true;
    }
    perror("Nao foi possivel preparar a pasta de dados");
    return false;
}

void limparTela(void) {
    fflush(stdout);
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO tela;
    if (!GetConsoleScreenBufferInfo(console, &tela)) return;
    DWORD tamanho = (DWORD)tela.dwSize.X * tela.dwSize.Y;
    DWORD escritos;
    COORD inicio = {0, 0};
    FillConsoleOutputCharacterA(console, ' ', tamanho, inicio, &escritos);
    FillConsoleOutputAttribute(console, tela.wAttributes, tamanho, inicio, &escritos);
    SetConsoleCursorPosition(console, inicio);
#else
    if (isatty(STDOUT_FILENO)) printf("\033[2J\033[H");
#endif
}

void pausarTela(void) {
    printf("\nPressione Enter para continuar...");
    fflush(stdout);
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {}
}

void aguardarSegundos(unsigned int segundos) {
#ifdef _WIN32
    Sleep(segundos * 1000);
#else
    sleep(segundos);
#endif
}


static void corDestaque(bool ativar) {
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO tela;
    if (!GetConsoleScreenBufferInfo(console, &tela)) return;
    static WORD corOriginal;
    static bool inicializada = false;
    if (!inicializada) { corOriginal = tela.wAttributes; inicializada = true; }
    WORD destaque = (corOriginal & 0xF0) | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    SetConsoleTextAttribute(console, ativar ? destaque : corOriginal);
#else
    if (isatty(STDOUT_FILENO)) printf(ativar ? "\033[36m" : "\033[0m");
#endif
}

void exibirSeparador(void) {
    corDestaque(true);
    printf("+----------------------------------------------------------------------------+\n");
    corDestaque(false);
}

void exibirLinha(const char *formato, ...) {
    char texto[512];
    va_list argumentos;
    va_start(argumentos, formato);
    vsnprintf(texto, sizeof(texto), formato, argumentos);
    va_end(argumentos);
    printf("| %-74.74s |\n", texto);
}

void exibirCabecalho(const char *titulo) {
    exibirSeparador();
    corDestaque(true);
    exibirLinha("MERCADO DONA BERE");
    corDestaque(false);
    exibirLinha("%s", titulo);
    exibirSeparador();
}

void exibirValor(const char *rotulo, float valor) {
    exibirLinha("%-49.49s R$ %20.2f", rotulo, valor);
}
