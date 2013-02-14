#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <signal.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int pid1, pid2, pid3, pid4, pid5, pid6;	//dzieci

void sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		puts("--- sygnal SIGINT --- zabijanie dzieci p1 - p6");

		kill(pid1, SIGINT);
		kill(pid2, SIGINT);
		kill(pid3, SIGINT);
		kill(pid4, SIGINT);
		kill(pid5, SIGINT);
		kill(pid6, SIGINT);
		exit(0);	//nie potrzeba informacji o tym ze zakonczyl na SIGINT, wiec mozna go skonczyc normalnie
	}
}

void redirect_output(char * file)
{
	int fd;
/*
	fd = open( "myfile.dat",
	        O_WRONLY | O_CREAT | O_TRUNC,
	        S_IRUSR | S_IWUSR );
*/
//	if((fd = open(file, O_WRONLY | O_CREAT))==-1){ /*open the file */
	if((fd = open( file,
	        O_WRONLY | O_CREAT | O_TRUNC,
	        S_IRUSR | S_IWUSR ))==-1){ /*open the file */
	  perror("open");
	  return 1;
	}

	dup2(fd,STDOUT_FILENO); /*copy the file descriptor fd into standard output*/
	dup2(fd,STDERR_FILENO); /* same, for the standard error */
	close(fd); /* close the file descriptor as we don't need it more  */
}

int main(int argc, char *argv[])
{
	puts("start");

	/*
	int spawnl( int mode,
	            const char * path,
	            const char * arg0,
	            const char * arg1...,
	            const char * argn,
	            NULL );
	*/

	if(signal(SIGINT, sig_handler) == SIG_ERR)
	    printf("Parent: Unable to create handler for SIGINT\n");
/*
	pid1 = spawnl(P_NOWAIT, "Proces1", "Proces1", NULL);
	pid2 = spawnl(P_NOWAIT, "Proces2", "Proces2", NULL);
	pid3 = spawnl(P_NOWAIT, "Proces3", "Proces3", NULL);
	pid4 = spawnl(P_NOWAIT, "Proces4", "Proces4", NULL);
	pid5 = spawnl(P_NOWAIT, "Proces5", "Proces5", NULL);
	pid6 = spawnl(P_NOWAIT, "Proces6", "Proces6", NULL);
*/

	//trzeba pouruchamiac procesy i przekierowac ich wyjscia
	//niestety nie da sie tego zrobic spawnem, trzeba forkowac

	if( (pid1 = fork()) == 0)
	{
		redirect_output("p1.txt");
		execl("Proces1", "Proces1", NULL);
	}
	else if( (pid2 = fork()) == 0)
	{
		redirect_output("p2.txt");
		execl("Proces2", "Proces2", NULL);
	}
	else if( (pid3 = fork()) == 0)
	{
		redirect_output("p3.txt");
		execl("Proces3", "Proces3", NULL);
	}
	else if( (pid4 = fork()) == 0)
	{
		redirect_output("p4.txt");
		execl("Proces4", "Proces4", NULL);
	}
	else if( (pid5 = fork()) == 0)
	{
		redirect_output("p5.txt");
		execl("Proces5", "Proces5", NULL);
	}
	else if( (pid6 = fork()) == 0)
	{
		redirect_output("p6.txt");
		execl("Proces6", "Proces6", NULL);
	}

	puts("------------ nacisnij ctrl-c by zakonczyc wszystko");

	while(1)
	{
		sleep(1);
	}

	return EXIT_SUCCESS;
}
