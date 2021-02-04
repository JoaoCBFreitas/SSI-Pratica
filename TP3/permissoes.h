#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <time.h>

typedef struct dadosUtilizador
{
    char *username;
    char *email;
} * utilizador;

utilizador *listaUsers;

int NumUsers = 0;

void initUser();
int getAuth();
int getUser(char *userName);
int existeUtilizador(char *username);
void fixStr(char *str, int lenght);
void criaUser(char *userName, char *userEmail);
char *geraPass();
void enviaEmail(char *email, char *password);
