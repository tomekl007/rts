#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>

#include <time.h>

#define t1_22 "1-22" //m1
#define t1_27 "1-27" //m1
#define t2_16 "2-16" //m3
#define t3_23 "3-23" //m10
#define t8_4 "8-4" //m4
#define t9_26 "9-26" //m11
#define t12_21 "12-21" //m5
#define t13_7 "13-7" //m6
#define t13_29 "13-29"//m6
#define t14_24 "14-24"//m7
#define t15_31 "15-31"//m12
#define t18_11 "18-11"//m2
#define t18_5 "18-5"//m2
#define t18_23 "18-23"//m2
#define t20_17 "20-17"//m9
#define t20_19 "20-19"//m9
#define t20_30 "20-30"//m9
#define t25_6 "25-6"//m8
#define t25_10 "25-10"//m8
#define t3_28 "3-28"

int min_pulse_prio;


void * task_4(void * arg);
void * task_5(void * arg);
void * task_6(void * arg);
void * task_7(void * arg);
void * task_10(void * arg);
void * task_11(void * arg);
void * task_16(void * arg);
void * task_17(void * arg);
void * task_19(void * arg);
void * task_21(void * arg);
void * task_22(void * arg);
void * task_23(void * arg);
void * task_24(void * arg);
void * task_26(void * arg);
void * task_27(void * arg);
void * task_28(void * arg);
void * task_29(void * arg);
void * task_30(void * arg);
void * task_31(void * arg);

void * TC(void *);

int main(int argc, char *argv[])
{
	printf("===argc = %d\n", argc );
	int taskToRun[10];
	int j =0;
	while(j < argc){
		taskToRun[j] = atoi(argv[j]);
		j++;
	}

	//printf("taskToRun : %d ", tasksToRun[2]);

	int i;

	min_pulse_prio =  sched_get_priority_min(1);

	int tasks_number = 19;

	int tid[tasks_number];
	pthread_attr_t attr[tasks_number];
//	char * canal_names[19] = { "3-23", "8-4", "9-26", "12-21",
//			"13-7", "13-29", "14-24", "15-31", "18-11",
//			"18-5", "18-23", "20-17", "20-19", "20-30", "25-6", "25-10"};

	char * canal_names[19] = {"8-4", "18-5","25-6","13-7","25-10","18-11","2-16","20-17",
			"20-19","12-21","1-22","18-23","14-24","9-26","1-27","3-28","13-29","20-30","15-31"};

//    void * (*tasks[19]) (void *) = {task_22, task_27, task_16, task_23, task_4, task_26,
//    		                        task_21, task_7, task_29, task_24, task_31, task_11,
//    		                        task_5, task_23, task_17, task_19, task_30, task_6,task_10};
    void * (*tasks[19]) (void *)={task_4,task_5,task_6,task_7,task_10,task_11,task_16,task_17,
                                  task_19,task_21,task_22,task_23,task_24,task_26,task_27,task_28,
                                  task_29,task_30,task_31};


    for(i = 0; i < argc; ++i)
    	{
    		pthread_attr_init(&(attr[taskToRun[i]]));
    		pthread_attr_setdetachstate(&(attr[taskToRun[i]]),PTHREAD_CREATE_DETACHED);
    		pthread_create(&(tid[taskToRun[i]]),&(attr[taskToRun[i]]),
    				       tasks[taskToRun[i]], (void*)canal_names[taskToRun[i]]);

    //		printf("stworzono task %d\n", i);
    	}

//	for(i = 0; i < tasks_number; ++i)
//	{
//		pthread_attr_init(&(attr[i]));
//		pthread_attr_setdetachstate(&(attr[i]),PTHREAD_CREATE_DETACHED);
//		pthread_create(&(tid[i]),&(attr[i]),tasks[i], (void*)canal_names[i]);
//
////		printf("stworzono task %d\n", i);
//	}

	while(1)	//tutaj lepiej czekac na zakonczenie watkow, ale mozna i tak
	{
	    sleep(1);
	}

	for(i = 0; i < argc; ++i)
	{
		pthread_attr_destroy(&(attr[taskToRun[i]]));
	}
}



void * task_body(void * arg)	//arg to nazwa kanalu
{
	char * canal_name = (char*)arg;
	printf("nowy task nasluchujacy na kanale %s\n", canal_name);

	name_attach_t *attach;
	int rcvid;

   /* Create a local name (/dev/name/local/...) */
   if ((attach = name_attach(NULL, canal_name, 0)) == NULL)
   {
	   puts("blad przy otwieraniu kanalu");
	   return EXIT_FAILURE;
   }

   struct _pulse pulse;



   while (1)
   {

	   rcvid = MsgReceivePulse(attach->chid, &pulse, sizeof(pulse), NULL);

	  if (rcvid == -1)
	  {/* Error condition, exit */
		  puts("blad");
		  break;
	  }

	  if (rcvid == 0)
	  {/* Pulse received */
		  if(pulse.code == _PULSE_CODE_DISCONNECT)
			  puts("klient rozlaczony");
		  else
			  printf("kanal %s, puls o wartosci %d\n", canal_name, pulse.value);	//wartosc pulsu to numer wysylajacego tasku

		  fflush(stdout);
	  }
   }

   return EXIT_SUCCESS;
}

void * task_4(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 4;

    //int canals_number = 1;//zawsze odbiera tylko z jednego kanalu

	//char * canal_name[canals_number];
	//canal_name[0] = t1_22;


	task_body(canal_name);

	return NULL;
}

void * task_5(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 5;

    //int canals_number = 1;//zawsze odbiera tylko z jednego kanalu

	//char * canal_name[canals_number];
	//canal_name[0] = t1_22;


	task_body(canal_name);

	return NULL;
}

void * task_6(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 6;

    //int canals_number = 1;//zawsze odbiera tylko z jednego kanalu

	//char * canal_name[canals_number];
	//canal_name[0] = t1_22;


	task_body(canal_name);

	return NULL;
}

void * task_7(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 7;

    //int canals_number = 1;//zawsze odbiera tylko z jednego kanalu

	//char * canal_name[canals_number];
	//canal_name[0] = t1_22;


	task_body(canal_name);

	return NULL;
}

void * task_10(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 10;

    //int canals_number = 1;//zawsze odbiera tylko z jednego kanalu

	//char * canal_name[canals_number];
	//canal_name[0] = t1_22;


	task_body(canal_name);

	return NULL;
}

void * task_11(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 11;

    task_body(canal_name);

	return NULL;
}

void * task_16(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 16;

    task_body(canal_name);

	return NULL;
}

void * task_17(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 17;

    task_body(canal_name);

	return NULL;
}

void * task_19(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 19;

    task_body(canal_name);

	return NULL;
}

void * task_21(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 21;

    task_body(canal_name);

	return NULL;
}


void * task_22(void * arg)
{
	char * canal_name = (char*)arg;

   // int canals_number = 1;//zawsze odbiera tylko z jednego kanalu

	//char * canal_name[canals_number];
	//canal_name[0] = t1_22;


	task_body(canal_name);

	return NULL;
}

void * task_23(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 17;

    task_body(canal_name);

	return NULL;
}

void * task_24(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 17;

    task_body(canal_name);

	return NULL;
}

void * task_26(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 26;

    task_body(canal_name);

	return NULL;
}

void * task_27(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 27;

    task_body(canal_name);

	return NULL;
}

void * task_28(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 28;

    task_body(canal_name);

	return NULL;
}

void * task_29(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 29;

    task_body(canal_name);

	return NULL;
}

void * task_30(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 30;

    task_body(canal_name);

	return NULL;
}

void * task_31(void * arg)
{
	char * canal_name = (char*)arg;
	int task_number = 31;

    task_body(canal_name);

	return NULL;
}
