#include <stdlib.h>
#include <errno.h>
#include <process.h>

#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>

#include <string.h>

#include <pthread.h>

#define TIMER_PULSE_CODE   _PULSE_CODE_MINAVAIL

#define PULSE_PRIORITY 10
#define PULSE_CODE 6

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

#define MESSAGE_CANAL_1 "1-22"
#define MESSAGE_CANAL_2 "1-27"
//int coid;

int min_pulse_prio;	//domyslnie dla RR

void * task_1(void * arg);
void * task_2(void * arg);
void * task_3(void * arg);
void * task_8(void * arg);
void * task_9(void * arg);
void * task_12(void * arg);
void * task_13(void * arg);
void * task_14(void * arg);
void * task_15(void * arg);
void * task_18(void * arg);
void * task_20(void * arg);
void * task_25(void * arg);

int mnoznik_czasu = 100; //ile razy okresy wzbudzania taskow piszacych beda wydluzone (by mozna bylo ladnie ogladac co sie dzieje na konsoli)

void setClock(int timer_chid, int msec, int timer_code)
{
//	puts("set clock");

	struct sigevent         event;
	   struct itimerspec       itime;
	   timer_t                 timer_id;

	   event.sigev_notify = SIGEV_PULSE;
	   event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,
			   	   	   	   	   	   	   	   timer_chid,
	                                    _NTO_SIDE_CHANNEL, 0);

	   event.sigev_priority = getprio(0);
	   event.sigev_code = timer_code;
	   timer_create(CLOCK_REALTIME, &event, &timer_id);

	   itime.it_value.tv_sec = 1;	//start 1.0 sekund
	   itime.it_value.tv_nsec = 0;
	   int a = msec * mnoznik_czasu;
	   itime.it_interval.tv_sec = a / 1000;
	   a %= 1000;
	   itime.it_interval.tv_nsec = (long)a * 1000000;
	   timer_settime(timer_id, 0, &itime, NULL);
}

int main(int argc, char *argv[])
{
	puts("********************");

	int taskToRun[10];
		int j =0;
		while(j < argc){
			taskToRun[j] = atoi(argv[j]);
			j++;
		}


	printf("mnoznik czasu: %d\n", mnoznik_czasu);

	int i;

	min_pulse_prio =  sched_get_priority_min(1);

	int tasks_number = 12;

	int tid[tasks_number];
	pthread_attr_t attr[tasks_number];
	void * (*tasks[12]) (void *) = {task_1, task_2, task_3, task_8, task_9, task_12, task_13, task_14, task_15, task_18, task_20, task_25};

	for(i = 0; i < argc; ++i)
	{
		pthread_attr_init(&(attr[taskToRun[i]]));
		pthread_attr_setdetachstate(&(attr[taskToRun[i]]),PTHREAD_CREATE_DETACHED);
		pthread_create(&(tid[taskToRun[i]]),&(attr[taskToRun[i]]),tasks[taskToRun[i]],NULL);

//		printf("stworzono task %d\n", i);
	}

	while(1)	//tutaj lepiej czekac na zakonczenie watkow, ale mozna i tak
	{
	    sleep(1);
	}

	for(i = 0; i < argc; ++i)
	{
		pthread_attr_destroy(&(attr[taskToRun[i]]));
	}
}

void open_canals(int canals_number, int * server_coids, char ** canal_names)
{
//	puts("open canals");

	int i;

	for(i = 0;  i < canals_number; ++i)
	   {
		   while ((server_coids[i] = name_open(canal_names[i], 0)) == -1)
		   {
			   printf("blad przy otwieraniu kanalu %s\n", canal_names[i]);
			   sleep(2);
		   }

		   printf("otwieram kanal %s\n", canal_names[i]);

		   fflush(stdout);
	   }
}

void main_loop(int task_number, int canals_number, int timer_chid, int * server_coids)
{
//	puts("main loop");

	int rcvid;
	int i;

	struct _pulse pulse;

	   while (1)
	   {
		   rcvid = MsgReceivePulse(timer_chid,&pulse, sizeof(pulse),NULL);

	       if (rcvid == 0)
	       {
	    	   if (pulse.code == TIMER_PULSE_CODE)
	    	   {
//	    		   puts("received timer pulse");

	    		   for(i = 0;  i < canals_number; ++i)
	    		   {
	    			   MsgSendPulse(server_coids[i], min_pulse_prio, _PULSE_CODE_MINAVAIL, task_number);
	    		   }
	    	   }
	       }
	   }
}

void task_body(int task_number, int canals_number, int msec, char ** canal_names)
{
	printf("task %d, okres %d ms\n", task_number, msec);

	int server_coids[canals_number];

	int timer_chid;

	timer_chid = ChannelCreate(0);

	setClock(timer_chid, msec, TIMER_PULSE_CODE);

	open_canals(canals_number, server_coids, canal_names);
	main_loop(task_number, canals_number, timer_chid, server_coids);
}

void * task_1(void * arg)
{
	//printf("task_1pisz jest");
	int task_number = 1;
	int msec = 10;
	int canals_number = 2;

	char * canal_names[canals_number];
	canal_names[0] = t1_22;
	canal_names[1] = t1_27;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_2(void * arg)
{
	int task_number = 2;
	int msec = 20;
	int canals_number = 1;

	char * canal_names[canals_number];
	canal_names[0] = t2_16;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_3(void * arg)
{
	int task_number = 3;
	int msec = 100;
	int canals_number = 1;

	char * canal_names[canals_number];
	canal_names[0] = t3_28;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_8(void * arg)
{
	int task_number = 8;
	int msec = 15;
	int canals_number = 1;

	char * canal_names[canals_number];
	canal_names[0] = t8_4;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_9(void * arg)
{
	int task_number = 9;
	int msec = 50;
	int canals_number = 1;

	char * canal_names[canals_number];
	canal_names[0] = t9_26;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_12(void * arg)
{
	int task_number = 12;
	int msec = 20;
	int canals_number = 1;

	char * canal_names[canals_number];
	canal_names[0] = t12_21;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_13(void * arg)
{
	int task_number = 13;
	int msec = 40;
	int canals_number = 2;

	char * canal_names[canals_number];
	canal_names[0] = t13_7;
	canal_names[1] = t13_29;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_14(void * arg)
{
	int task_number = 14;
	int msec = 15;
	int canals_number = 1;

	char * canal_names[canals_number];
	canal_names[0] = t14_24;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_15(void * arg)
{
	int task_number = 15;
	int msec = 100;
	int canals_number = 1;

	char * canal_names[canals_number];
	canal_names[0] = t15_31;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_18(void * arg)
{
	int task_number = 18;
	int msec = 14;
	int canals_number = 3;

	char * canal_names[canals_number];
	canal_names[0] = t18_5;
	canal_names[1] = t18_11;
	canal_names[2] = t18_23;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_20(void * arg)
{
	int task_number = 20;
	int msec = 20;
	int canals_number = 3;

	char * canal_names[canals_number];
	canal_names[0] = t20_17;
	canal_names[1] = t20_19;
	canal_names[2] = t20_30;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}

void * task_25(void * arg)
{
	int task_number = 25;
	int msec = 50;
	int canals_number = 2;

	char * canal_names[canals_number];
	canal_names[0] = t25_6;
	canal_names[1] = t25_10;

	task_body(task_number, canals_number, msec, canal_names);

	return NULL;
}
