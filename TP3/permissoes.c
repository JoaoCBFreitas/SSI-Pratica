#include "permissoes.h"
#ifndef strlcpy
#define strlcpy(dst, src, sz) snprintf((dst), (sz), "%s", (src))
#endif
int getUser(char *userName)
{
	//Obtem o username do utilizador que está a correr o terminal
	char *user = "";
	struct passwd *pass;
	pass = getpwuid(getuid());
	user = pass->pw_name;
	if (strcmp(user, "") == 0)
	{
		return -1;
	}
	strlcpy(userName, user, 25);
	return 0;
}

int existeUtilizador(char *username)
{
	//Vai á lista de utilizadores e verifica se o username existe
	for (int i = 0; i < NumUsers; i++)
		if (strcmp(username, listaUsers[i]->username) == 0)
			return i;
	return -1;
}

void criaUser(char *userName, char *userEmail)
{
	//cria um novo user
	utilizador user = (utilizador)malloc(sizeof(struct dadosUtilizador));
	//aloca na memoria espaço para o username e email
	user->username = (char *)malloc(sizeof(char) * 25);
	user->email = (char *)malloc(sizeof(char) * 100);
	//copia o username e email para o user
	strlcpy(user->username, userName, 25);
	strlcpy(user->email, userEmail, 100);
	//adiciona user ao da lista de utilizadores (indicado pelo numero de users)
	listaUsers[NumUsers] = user;
	//incrementa o numero de users
	NumUsers++;
}

char *geraPass()
{
	//função cria uma password alfanumerica de 10 caracters
	int i;
	char *pass = (char *)malloc(sizeof(char) * 11);
	srand((unsigned int)(time(NULL)));
	for (i = 0; i < 4; i++)
	{
		//revised logic to generate random characters at all positions (0 - 9)
		pass[3 * i] = '0' + (rand() % 10);	  //generating numeric character
		char capLetter = 'A' + (rand() % 26); //generating upper case alpha character
		pass[(3 * i) + 1] = capLetter;
		char letter = 'a' + (rand() % 26); //generating lower case alpha character
		pass[(3 * i) + 2] = letter;
	}
	pass[3 * i] = '\0'; //placing null terminating character at the end
	return pass;
}

void initUser()
{
	//Aloca memória
	listaUsers = (utilizador *)malloc(sizeof(utilizador) * 10);
}

void enviaEmail(char *email, char *password)
{
	//Email-ssi2020tp3@gmail.com
	//Pass-123456789ASDA
	char comando[1000] = "";
	sprintf(comando, "sendemail -o tls=yes -f ssi2020tp3@gmail.com -t %s -u Password -m %s -s smtp.gmail.com:587 -xu ssi2020tp3@gmail.com -xp 123456789ASDA", email, password);
	system(comando);
}

void fixStr(char *str, int length)
{
	//Muda o ultimo caracter de uma string de \n para \0
	for (int i = 0; i < length; i++)
		if (str[i] == '\n')
			str[i] = '\0';
}

void killhandler(int signum)
{
	//Handler para sinais
	free(listaUsers);
	waitpid(-1, NULL, WNOHANG);
	kill(0, SIGKILL);
}
int getAuth()
{
	signal(SIGINT, killhandler);
	signal(SIGQUIT, killhandler);
	int res = 0;
	char username[25];
	int estado = -1;
	res = getUser(username);
	if (res < 0)
	{
		return -1;
	}

	int pos = existeUtilizador(username);
	while (pos == -1)
	{
		char email[100];
		printf("Utilizador %s não existe na base de dados\n", username);
		printf("Introduza o seu email\n");
		fgets(email, 101, stdin);
		fixStr(email, 100);

		printf("Email Introduzido: %s\n", email);

		criaUser(username, email);
		pos = existeUtilizador(username);
	}
	char *password;
	char passwd[10];
	char *email = listaUsers[pos]->email;
	password = geraPass();
	enviaEmail(email, password);
	printf("Password enviada para %s\n", email);
	//Cria um processo filho que dorme durante 30 segundos e depois morre
	//Se depois do utilizador inserir a pass o filho ainda estiver vivo quer dizer que ainda não tinham passado os 30 segundos.
	//Nesse caso o pai mata o filho passando de seguida à comparação da password gerada com a fornecida pelo utilizador.
	//Se o filho estiver morto então acabou o tempo
	pid_t pid = fork();
	if (pid != 0)
	{
		printf("Insira a password (30 segundos)\n");
		fgets(passwd, 14, stdin);
		fixStr(passwd, 13);
		printf("Password Inserida: %s\n", passwd);
		int status;
		pid_t result = waitpid(pid, &status, WNOHANG);
		if (result == 0)
		{
			kill(pid, SIGTERM);
		}
		else if (result == -1)
		{
			printf("Ocorreu um erro\n");
			return -1;
		}
		else
		{
			printf("TIMEOUT\n");
			return -1;
		}
	}
	else
	{
		sleep(30);
		exit(0);
	}
	if (strcmp(passwd, password) != 0)
	{
		free(password);
		printf("Acesso Negado\n");
		estado = -1;
	}
	else
	{
		free(password);
		printf("Acesso Permitido\n");
		estado = 0;
	}
	return estado;
}
