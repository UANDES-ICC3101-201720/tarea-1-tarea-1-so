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




// TODO: implement







typedef struct args_struct {

	UINT argL;
	UINT argR;
	UINT argX;
	UINT *arg_arreglo;
} as_type;


int serial_binsearch(struct args_struct arguments) {
	struct args_struct as = arguments;
	UINT l = as.argL;
	UINT r = as.argR;
	UINT x = as.argX;
	UINT *arr = as.arg_arreglo;

	if (x > arr[r]){
		return -1;
	}
	
	if(r >= 1){
		UINT mid = l + (r-l)/2;
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

void* serial_binsearch_void (void *argumentos){
	as_type *as = (as_type *) argumentos;
	UINT l = as->argL;
	UINT r = as->argR;
	UINT x = as->argX;
	UINT *arr = as->arg_arreglo;
	printf("l: %d, r: %d, x: %d\n", l, r, x);

	if ((x > arr[r]) || (l-r == 1) || (x < arr[x])){
		size_t ret = -1;
		return  (void*) ret;
	}


	if(r >= 1){
		UINT mid = l + (r-l)/2;
		printf("mid = %d\n", mid);
		if(arr[mid] == x) {
			printf("---> %d\n",mid);
			size_t ret = mid;
			return (void*) ret;
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

	size_t ret = -1;
	return (void*) ret; 
}

// TODO: implement

// 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56-63, 64-71, 72-79

void * parallel_binsearch(struct args_struct arguments){

	struct args_struct as = arguments;
	UINT x = as.argX;
	UINT *arr = as.arg_arreglo;

	int cantidad_nucleos = sysconf(_SC_NPROCESSORS_ONLN);

	pthread_t arreglo_threads[cantidad_nucleos];


	for (int i = 0; i < cantidad_nucleos; i++){
		as.argL = i*cantidad_nucleos;
		if (i != cantidad_nucleos-1){
			as.argR = ((i+1)*cantidad_nucleos)-1;
		}
		else {
			as.argR = 100;
		}
		as.argX = x;
		as.arg_arreglo = arr;
		size_t * respuesta;
		printf("creando threads...\n");
		pthread_create(&arreglo_threads[i], NULL, serial_binsearch_void , &as);	
		pthread_join(arreglo_threads[i], (void **) &respuesta);

		//printf("respuesta: %d\n", respuesta);
		if (*respuesta == x){
			return respuesta;
		}
	}
	size_t ret = -1;
	return (void*) ret;

}

int main(int argc, char** argv) {

    /* TODO: move this time measurement to right before the execution of each binsearch algorithms
     * in your experiment code. It now stands here just for demonstrating time measurement. */
    int experiments = 0 ;
    int t = 0;
    int position = 0;
    int c;
    clock_t cbegin = clock();
    char charT= "";

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
				strcpy(charT, optarg);
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
    	char * myargs[3];
    	myargs[0] = "./datagen";
    	myargs[1] = "inicio";
    	myargs[2] = NULL;
    	execvp(myargs[0], myargs);
    }
    else if(dtgnid>0){
    	printf("%s%d\n", "Bnsearch PID: ", getpid());
    }

    else if (dtgnid<0){
    	fprintf(stderr, "%s\n", "Can't create Datagen as child process");
    }
    printf("%s%d\n","hola: ", getpid() );
	
    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */

	struct sockaddr_un addr;
	int fd;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd ==-1) perror("[binsearch] Error creating socket. \n");

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, DSOCKET_PATH, sizeof(addr.sun_path));

	int r;
	if((r= connect(fd, (struct sockaddr*)&addr, sizeof(addr)))==-1)
		perror("Binsearch no se pudo conectar");


//Aqui comienza la generacion de datos en base a los flags
	char instruction[]= "BEGIN S 6";


	
	write(fd, instruction, 10);
	/*
	UINT readvalues = 0;
	size_t numvalues = pow(10, 6);
	size_t readbytes = 0;

	UINT *readbuf = malloc(sizeof(UINT) * numvalues);

	while (readvalues < numvalues) {
	    readbytes = read(fd, readbuf + readvalues, sizeof(UINT) * 1000);
	    readvalues += readbytes / 4;
	}

	UINT arreglo[1000000];

	UINT * index = readbuf[0];
	for(size_t i = 0; i<numvalues; i++){
		arreglo[i]= index + i;
		printf("%lu\n", arreglo[i]);
	}

	free(readbuf);
	
	if(write(fd, DATAGEN_END_CMD, sizeof(DATAGEN_END_CMD)) == -1){
		perror("[Binsearch]Datagen not terminated");
	} */



	
	UINT readvalues = 0;
	size_t numvalues = pow(10, 8);
	size_t readbytes = 0;

	UINT *readbuf = malloc(sizeof(UINT) * numvalues);

	while (readvalues < numvalues) {
	    /* read the bytestream */
	    readbytes = read(fd, readbuf + readvalues, sizeof(UINT) * 1000);
	    readvalues += readbytes / 4;
	}

	if(write(fd, DATAGEN_END_CMD, sizeof(DATAGEN_END_CMD)) ==-1)
		perror("Cant stop datagen");

	exit(0);


	
		
		
		
	





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
