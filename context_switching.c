#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>


//Global variable declaration
int val = 50;
int val_1 = 1000;
int n = 10;
char alpha='A';
int pat_n = 10;

//Global functions declaration
void *addition(void *arg);
void *fib(void *arg);
void *pat(void *arg);
void *add_5(void *arg);
void *alphabets(void *arg);
void *quit(void *arg);

//Creating structure
struct store
{
 pthread_t thread_id;
 int num;
}  ;

static FILE *fp = NULL;


//When signal from the main function arrives, handler_1 handles the thread
void handler_1(int sig_1)
{
	// Declaring thread 2
	pthread_t thread_id2;

	// Creating second thread
	pthread_create(&thread_id2, NULL , fib, NULL);


	//fprintf(fp,"Interrupt ...\n Thread ID :-%d started from 1000 series \n",thread_id2);

}


//When signal from main function arrives for quiting,handler_2 handles the thread
void handler_2(int sig_2)
{
	// Declaring thread 3
	pthread_t thread_id3;

	// Creating third thread
	pthread_create(&thread_id3,NULL,quit,NULL);

    //	fprintf(fp,"Thread ID quit  %d \n",thread_id3);
}

//When signal from add_5 arrives then this will handle
void handler_3(int sig_3)
{
	// Declaring thread 4
	pthread_t thread_id4;

	// Creating fourth thread
	pthread_create(&thread_id4,NULL,pat,NULL);

	
}

//When signal from alphabets arrives then this will handle
void handler_4(int sig_4)
{
	// Declaring thread 5
	pthread_t thread_id5;

	// Creating fifth thread
	pthread_create(&thread_id5,NULL,add_5,NULL);

}

//When signal from fib arrives then this will handle
void handler_5(int sig_5)
{
	// Declaring thread 6
	pthread_t thread_id6;

	// Creating sixth thread
	pthread_create(&thread_id6,NULL,alphabets,NULL);

}

//main function
void main()
{

  fp = fopen("logfile.txt","a");
  pthread_t thread_id1;

  printf("The process id is:- %d \n",getpid());


  // Signals interrupts which can occur while main thread is running
  signal(SIGINT, handler_1);
  signal(SIGQUIT,handler_2);

  // Creating the main thread i.e. thread 1
  pthread_create(&thread_id1, NULL , addition, NULL);

  // The following function waits for the thread specified by thread to terminate
  pthread_join(thread_id1,NULL);

}

//Main thread which is being called by main function
void *addition(void *arg)
{
    int i;

	fprintf(fp,"Initializing with %d ",val);
	printf("Addition begins from here...\n");

    for(i=0;i<20;i++)
	  {
		    ++val;
		    printf("%d \n",val);	// Printing 51 to 70 as val's starting value is 50
		    sleep(2);

	  }

}

//Thread which will be executed after the main thread i.e. the addition function
void *fib(void *arg)
{
    signal(SIGINT,handler_3);
    fprintf(fp,"\n **Context switching to next process/thread**\t  %d Value stored: \n",val);
    fprintf(fp," %d Value restored \n",n);

    int i, nt, t1 = 0, t2 = 1;

    printf("\nFibonacci Series Begins...\n");

    for (i = 1; i <= n; ++i)
    {
        printf("%d \n ", t1);
        nt = t1 + t2;
        t1 = t2;
        t2 = nt;
	sleep(2);
    }

    fprintf(fp,"\n**Switching again** \t %d Value stored \n",n);
    fprintf(fp," %d Value restored....... \n",val);
}

// Thread which will be executed after the "fib" thread if an interrupt occurs
void *pat(void *arg)
{
    signal(SIGINT,handler_4);
    fprintf(fp,"\n **Context switching to next process/thread**\t  %d Value stored: \n",n);
    fprintf(fp," %d Value restored \n",pat_n);
    printf("\nPATTERN\n");
    int i, j;

    for(i=1; i<=pat_n; ++i)
    {
        for(j=1; j<=i; ++j)
        {
            printf("* ");
	   
        }
        printf("\n");
	sleep(2);
    }
    fprintf(fp,"\n**Switching again** \t %d Value stored \n",pat_n);
    fprintf(fp," %d Value restored.......  \n",n);
}




// Thread which will be executed after the "pat" thread if an interrupt occurs
void *add_5(void *arg)
{
    int i;

	// Signal for thread to inner thread
	signal(SIGINT,handler_5);

	fprintf(fp," \n **Context switching to next process/thread**\t  %d Value stored: \n",pat_n);
	fprintf(fp," %d Value restored \n",val_1);
	printf("\nNumbers having difference of 5\n");
    	printf("%d \n",val_1);
	for(i=0;i<5;i++)
	{
		    val_1 = val_1 + 5;
		    printf("%d \n",val_1);	// Printing values with first value being 1000 and afterwards it gets added by 5
		    sleep(2);
	}

	printf("Final value = %d \n",val_1);
	fprintf(fp," %d Value restored.......  \n",pat_n);
}

// Thread which will be executed after the "add_5" thread if an interrupt occurs
void *alphabets(void *arg)
{

    fprintf(fp,"\n **Context switching to next process/thread**\t  %d Value stored: \n",val_1);
    fprintf(fp," %c Value restored \n",alpha);
    printf("\nAlphabets begins from here\n");
    alpha++;

    printf(" %c \n",alpha);	// Printing A to Z
    fprintf(fp,"\n**Switching again** \t %c Value stored \n",alpha);
    fprintf(fp," %d Value restored.......  \n",val_1);
}


// SIGQUIT comes then handler will call this function
void *quit(void *arg)
{
	fprintf(fp,"**************TERMINATED***************");
	printf("  \n ...End of the process...  \n");
	exit(0);

}