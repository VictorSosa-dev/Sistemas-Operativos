#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

/*
Claves para semaforos. Estos valores son INDICES.
0 LLENO: Valor inicial será 0. Cuantos lugares estan ocupados en el buffer
1 VACIO:  Valor inicial el tamaño de buffer. Indica cuantos lugares hay disponibles para consumir
2 MUTEX: Valor inicial 1. Este da permiso o bloquea el acceso a la región critica
*/
#define FULL 0
#define EMPTY 1
#define MUTEX 2	
#define BUF_SIZE 1024

void consumir();
void waitS(int semaforo); // Recibe el indice del semaforo al que se le aplicara wait: -1
void signalS(int semaforo); // Recibe el indice del semaforo al que se le aplicara el signal: +1


struct shmseg{
	int buf[BUF_SIZE];
	int full;
	int empty;
};

int main(){
	int t=0;  //Consumidor
	int k=0;  //Numero de lugares a consumir
	pid_t pid;
	printf("************************Soy el consumidor*****************************\n");
	printf("\nDame el numero de consumidores: ");
	scanf("%d",&t);
	printf("\nDame el valor de k ques es numero casillas a consumir por proceso: ");
	scanf("%d",&k);
	
	
	for(int i=0; i<t; i++){
		pid = fork();
		if(pid){
			//printf("\nSoy el padre %d\n",getpid());
			sleep(1);
		}else if(pid==0){	
			//printf("\nSoy el hijo %d con padre %d\n",getpid(),getppid());
			sleep(3);
			printf("\nSoy el proceso  %d, y consumo %d elementos.\n\n",i+1,k);
			for(int j=0; j<k; j++){
				printf("Consumidor %d: Esperando por espacio disponible... \n",i+1);
				waitS(FULL);
				printf("Consumidor %d: Esperando acceso a la Recion Critica... \n",i+1);
				waitS(MUTEX);
				consumir(); // Region Critica
				printf("Consumidor %d: Liberando acceso a bufer\n",i+1);
				signalS(MUTEX);
				printf("Consumidor %d: Hay un elemento menos en el buffer\n\n",i+1);
				signalS(EMPTY);

			}
			break;
						
		}
		
	} //Fin del for
	
}

// Es el equivalente al x++ del programa sin
// memoria compartida y sin semaforos de C
void consumir(){
	int llave, shm_id, *buffer;
	struct shmseg *shmp;
	// Creamos la llave
	llave = ftok(".", 'G');
	
	if( llave == -1 )
		perror("\nError al crear la llave\n");
	else{
		shm_id = shmget(llave, sizeof(int), 0666);
		if(shm_id == -1)
			perror("\nError al encontrar la variable\n");
		else{
			shmp = shmat(shm_id, 0, 0);
			buffer = shmp->buf;
			buffer[shmp->full-1] = 0;
			shmp->full--;
			shmp->empty++;
			shmdt(shmp);
			//(printf("Soy el consumidor y conzumo el elemento '%c', en la posicion [%d] \n",a, *buffer);
	 	
		}
	}
}


// Se le pasa el indice del semaforo al que se aplicara wait
void waitS(int semaforo){
	int llave, sem_id;
	struct sembuf s_ops;
	
	llave = ftok(".", 'G');
	
	if( llave == -1)
		perror("\nError al crear llave\n");
	else{
		sem_id = semget(llave, 3, 0666);
		if( sem_id == -1)
			perror("\nError al encontrar los semaforos\n");
		else{
			s_ops.sem_num = semaforo; // numero del semaforo a actualizar
			s_ops.sem_op = -1;    // operacion wait
			s_ops.sem_flg = 0;
			semop(sem_id, &s_ops, 1);  // Se le aplica la operacion wait a semaforo
		}
	}	
} 





// Se le pasa el indice del semaforo al que se aplicara signal
void signalS(int semaforo){
	int llave, sem_id;
	struct sembuf s_ops;
	
	llave = ftok(".", 'G');
	
	if( llave == -1)
		perror("\nError al crear llave\n");
	else{
		sem_id = semget(llave, 3, 0666);
		if( sem_id == -1)
			perror("\nError al encontrar los semaforos\n");
		else{
			s_ops.sem_num = semaforo; // numero del semaforo a actualizar
			s_ops.sem_op = 1;    // operacion signal
			s_ops.sem_flg = 0;
			semop(sem_id, &s_ops, 1);  // Se le aplica la operacion wait a semaforo
		}
	}
	
	
}
