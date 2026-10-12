#ifndef PLATAFORMA_H
#define PLATAFORMA_H
#include <stdbool.h>
bool prepararPastaDados(void);
void limparTela(void);
void pausarTela(void);
void aguardarSegundos(unsigned int segundos);
void exibirSeparador(void);
void exibirLinha(const char *formato, ...);
void exibirCabecalho(const char *titulo);
void exibirValor(const char *rotulo, float valor);
#endif
