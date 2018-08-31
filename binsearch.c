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


/*
int parallel_binsearch2(void *function_args){
	struct two_args_struct *tas = function_args;
	int buscame;
	int *arreglo;
	buscame = tas -> num_a_buscar;
	arreglo = tas -> arreglo;
	printf("buscame = %d\n", buscame);
	int largo = sizeof(arreglo)/sizeof(int);
	int arr_izq[largo/2];
	int arr_der[largo/2];
	for (int i = 0; i < largo/2; i++){
		arr_izq[i] = arreglo[i];
		arr_der[i] = arreglo[i+(largo/2)];
	}
}*/

typedef struct args_struct {
	int argL;
	int argR;
	int argX;
	int *arg_arreglo;
} as_type;

/*
int serial_binsearch(int arr[], int l, int r, int x) {
	if(r >= 1){
		int mid = l + (r-l)/2;
		printf("mid = %d\n", mid);
		if(arr[mid] == x) return mid;
		if(arr[mid] > x) {
			return serial_binsearch(arr, l, mid-1, x);
			}
		return serial_binsearch(arr, mid+1, r, x);	
	}
	return -1;
}*/

int serial_binsearch(struct args_struct arguments) {
	struct args_struct as = arguments;
	int l = as.argL;
	int r = as.argR;
	int x = as.argX;
	int *arr = as.arg_arreglo;

	if (x > arr[r]){
		return -1;
	}
	
	if(r >= 1){
		int mid = l + (r-l)/2;
		printf("mid = %d\n", mid);
		if(arr[mid] == x) {
			printf("---> %d\n",mid);
			return mid;
			}
		if(arr[mid] > x) {
			printf("Entrando a la izquierda...\n");
			struct args_struct as2;
			as2.argL = l;
			as2.argR = mid-1;
			as2.argX = x;
			as2.arg_arreglo = arr;
			return serial_binsearch(as2);
			}
		else{
			printf("Entrando a la derecha...\n");
			struct args_struct as3;
			as3.argL = mid+1;
			as3.argR = r;
			as3.argX = x;
			as3.arg_arreglo = arr;
			return serial_binsearch(as3);	
		}
		
	}
	return -1;
}

int serial_binsearch_void(void *argumentos){
	as_type *as = (as_type *) argumentos;
	int l = as->argL;
	int r = as->argR;
	int x = as->argX;
	int *arr = as->arg_arreglo;
	printf("l: %d, r: %d, x: %d\n", l, r, x);

	if ((x > arr[r]) || (l-r == 1) || (x < arr[x])){
		return -1;
	}


	if(r >= 1){
		int mid = l + (r-l)/2;
		printf("mid = %d\n", mid);
		if(arr[mid] == x) {
			printf("---> %d\n",mid);
			return (void **)mid;
			}
		if(arr[mid] > x) {
			printf("Entrando a la izquierda...\n");
			struct args_struct as2;
			as2.argL = l;
			as2.argR = mid-1;
			as2.argX = x;
			as2.arg_arreglo = arr;
			return serial_binsearch_void(&as2);
			}
		else{
			printf("Entrando a la derecha...\n");
			struct args_struct as3;
			as3.argL = mid+1;
			as3.argR = r;
			as3.argX = x;
			as3.arg_arreglo = arr;
			return serial_binsearch_void(&as3);	
		}	
	}
}

// TODO: implement

// 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56-63, 64-71, 72-79

void * parallel_binsearch(struct args_struct arguments){

	struct args_struct as = arguments;
	int l = as.argL;
	int r = as.argR;
	int x = as.argX;
	int *arr = as.arg_arreglo;

	int cantidad_nucleos = sysconf(_SC_NPROCESSORS_ONLN);

	pthread_t arreglo_threads[cantidad_nucleos];


	for (int i = 0; i < cantidad_nucleos; i++){
		as_type as;
		as.argL = i*cantidad_nucleos;
		if (i != cantidad_nucleos-1){
			as.argR = ((i+1)*cantidad_nucleos)-1;
		}
		else {
			as.argR = 100;
		}
		as.argX = x;
		as.arg_arreglo = arr;
		int * respuesta;
		printf("creando threads...\n");
		pthread_create(&arreglo_threads[i], NULL, serial_binsearch_void , &as);	
		pthread_join(arreglo_threads[i], (void **) &respuesta);

		//printf("respuesta: %d\n", respuesta);
		if (respuesta == x){
			return respuesta;
		}
	}
	return -1;

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


	//int cantidad_de_nucleos = sysconf(_SC_NPROCESSORS_ONLN);
	//int largo = 100;
	//printf("Se crean %d arreglos de largo %d c/u.\n", cantidad_de_nucleos, largo/(cantidad_de_nucleos));
	//printf("%d\n", cantidad_de_nucleos*(largo/(cantidad_de_nucleos)));
    /* TODO: start datagen here as a child process. */
    /*
    int dtgnid = fork();
    
    if(dtgnid == 0){
    	printf("%s%d\n","Datagen PID: ", getpid());
    	char * name= "./datagen";
    	execvp(name, &name);
    }
    else if (dtgnid<0){
    	fprintf(stderr, "%s\n", "Can't create Datagen as child process");
    }
	*/
    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */
	
    int arreglo[100];
    for (int i = 0; i < 100; i++) {
    	arreglo[i] = i;
    }
	//printf("%d\n",sizeof(arreglo)/sizeof(int));

    struct args_struct as;
	as.argL = 0;
	as.argR = 99;
	as.argX = 74;
	as.arg_arreglo = arreglo;

	printf("Empezando\n");

	printf("posicion: %d\n", (int)parallel_binsearch(as)); 

	printf("-------------------------------\n");
 
	

	//printf("posicion serial: %d\n", serial_binsearch(as));

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