#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int pid1, pid2;	//dzieci

void sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		puts("--- sygnal SIGINT --- zabijanie dzieci p6");
		kill(pid1, SIGINT);
		kill(pid2, SIGINT);
		exit(0);	//nie potrzeba informacji o tym ze zakonczyl na SIGINT, wiec mozna go skonczyc normalnie
	}
}

int main(int argc, char *argv[])
{
	if(signal(SIGINT, sig_handler) == SIG_ERR)
	    printf("Parent: Unable to create handler for SIGINT\n");

	const char* taskiCzytajace[6]= {"13","14","15","16","17","18"};
	const char* taskiPiszace[1]={"11"};
	int pid;

	if( (pid1 = fork()) == 0)
		execl("pC2", taskiCzytajace[0],taskiCzytajace[1],taskiCzytajace[2],taskiCzytajace[3],
				taskiCzytajace[4],taskiCzytajace[5], NULL);

	if( (pid2 = fork()) == 0)
		execl("pP", taskiPiszace[0],NULL);
	else
		while(1)	//dzieci nie beda sie konczyc same z siebie, czekaj na signal ctrl-c
			sleep(1);

	return 0;
}
