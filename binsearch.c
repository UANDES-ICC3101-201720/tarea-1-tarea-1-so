#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <pthread.h>
#include "types.h"
#include "const.h"
#include "util.h"
#define _POSIX_C_Source 2

int *numero = 0;

// TODO: implement

struct arg_struct{
	int arg2;
	int arg3;
	int arg4;
	int* arg1;
};

typedef struct arg_struct2{
	int argL;
	int argR;
	int argX;
	int *array;
} arguments2;



int serial_binsearch(int arr[], int l, int r, int x) {
	if(r>=1){
		int mid = l +(r-l)/2;
		printf("mid = %d\n",mid);
		if(arr[mid] == x) return mid;
		if(arr[mid]>x) {
			//printf("izq\n");
			return serial_binsearch(arr, l, mid-1, x);
			}
		//printf("der\n");
		return serial_binsearch(arr, mid+1, r, x);
			
	}
    return 0;
}

// TODO: implement
int parallel_binsearch(void * arguments) {

	struct arg_struct *args = arguments;


	//printf("%s%d\n", "arg2: ", args->arg2);
	//printf("%s%d\n", "arg3: ", args->arg3);
	//printf("%s%d\n", "arg4: ", args->arg4);

	printf("%s%d\n", "arg2", args->arg2);
    printf("%s%d\n", "arg3", args->arg3);
    printf("%s%d\n", "arg4", args->arg4);

	//printf("%s%d\n", "arg2: ", args->arg2);
	//printf("%s%d\n", "arg3: ", args->arg3);
	//printf("%s%d\n", "arg4: ", args->arg4);
	printf("________________\n");

	if (args->arg3>=1){
		int mid = args->arg2 + (args->arg3-1)/2;
		printf("mid = %d\n",mid);
		struct arg_struct args1;
		args1.arg1 = args->arg1;
		printf("args: %ls\n", args1.arg1);
		args1.arg2 = args->arg2;
		args1.arg3 = mid-1;
		args1.arg4 = args->arg4;
		
		struct arg_struct args2;
		args2.arg1 = args->arg1;
		args2.arg2 = mid+1;
		args2.arg3 = args->arg3;
		args2.arg4 = args->arg4;
		//printf("(args->arg1)[mid] = %d\n", (args->arg1));
		//printf("(args->arg4)[mid] = %d\n", (args->arg4));
		if(((args->arg1)[mid]) != (args->arg4)){
			pthread_t thread1, thread2;

			pthread_create(&thread1, NULL, parallel_binsearch,&args1);
			//pthread_join(thread1,NULL);
			pthread_create(&thread2, NULL, parallel_binsearch,&args2);
			//pthread_join(thread2,NULL);

			pthread_create(&thread1, NULL, parallel_binsearch, &args1);
			pthread_create(&thread2, NULL, parallel_binsearch, &args2);

		}

		if(args->arg1[mid] == args->arg4) return mid;
		return 0;

		if((args->arg1)[mid] == (args->arg4)) return mid;

	}
    
}

int main(int argc, char** argv) {

    /* TODO: move this time measurement to right before the execution of each binsearch algorithms
     * in your experiment code. It now stands here just for demonstrating time measurement. */
    int experiments = 0 ;
    int t = 0;
    int position = 0;
    int c;
    clock_t cbegin = clock();

    printf("[binsearch] Starting up...\n");

    /* Get the number of CPU cores available */
    printf("[binsearch] Number of cores available: '%ld'\n",
           sysconf(_SC_NPROCESSORS_ONLN));

    /* TODO: parse arguments with getopt */
    
    while((c = getopt (argc, argv, "T:E:P:")) !=-1){
    	switch(c){
    		perror("[datagen] Bind error.\n");
			case 'T':
			t = atoi(optarg);
			if(t<3 || t>9){
				fprintf(stderr, "%s\n", "T must be betweeen 3 and 9");
				return 0;
			}
			
			case 'E':
			experiments = atoi(optarg);
			if(experiments<1){
				fprintf(stderr, "%s\n","Number of experiments must be greater or equal than 1" );
				return 0;
			}
			case 'P':
			position = atoi(optarg);
			if(position<0 || position>pow(10, t)-1){
				fprintf(stderr, "%s\n", "Position must be betweeen 0 and 10^T - 1");
				return 0;
			}
    	}
    }
	printf("%d %d %d\n",t, experiments, position);

    /* TODO: start datagen here as a child process. */
    
    int dtgnid = fork();
    
    if(dtgnid == 0){
    	printf("%s%d\n","Datagen PID: ", getpid());
    	char * name= "./datagen";
    	execvp(name, &name);
    }
    else if (dtgnid<0){
    	fprintf(stderr, "%s\n", "Can't create Datagen as child process");
    }
	
    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */
    struct arg_struct prueba;
    int arreglo[100];
    for (int i =0; i<100; i++) arreglo[i]=i+77;
    prueba.arg1 = arreglo;
    prueba.arg2 = 0;

    prueba.arg3 = 99;
    prueba.arg4 = 77;

    prueba.arg3 =999;
    prueba.arg4 = 708;
    printf("%s: %d\n","Se encuentra en la posicion:",parallel_binsearch(&prueba));
	pthread_t serial, parallel;
    prueba.arg3 = 999;
    prueba.arg4 = 56;

    
	//printf("%s%d\n","posicion serial: ", serial_binsearch(arreglo,0,99,81));
	printf("Se encuentra en la posicion: %d\n",parallel_binsearch(&prueba));
	//pthread_t serial, parallel;

	//pthread_create(&serial, NULL, (serial_binsearch), NULL); // El ultimo parametro esta por ver porque son los argumento de la fn
	//pthread_create(&parallel, NULL, (parallel_binsearch), );//Lo mismo de arriba
	//pthread_join(serial, NULL);
	//pthread_join(parallel, NULL);
    /* TODO: connect to datagen and ask for the necessary data in each experiment round.
     * Create a Unix domain socket with DSOCKET_PATH (see const.h).
     * Talk to datagen using the messages specified in the assignment description document.
     * Read the values generated by datagen from the socket and use them to run your
     * experiments
     * */

    /* Probe time elapsed. */
    clock_t cend = clock();

    // Time elapsed in miliseconds.
    double time_elapsed = ((double) (cend - cbegin) / CLOCKS_PER_SEC) * 1000;

    printf("Time elapsed '%lf' [ms].\n", time_elapsed);

    exit(0);
}