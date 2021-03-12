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
#define TAMANIO_BUFFER 3


void consumir();
void waitS(int semaforo); // Recibe el indice del semaforo al que se le aplicara wait: -1
void signalS(int semaforo); // Recibe el indice del semaforo al que se le aplicara el signal: +1


int main(){
	
	printf("\tP: Esperando por espacio disponible... \n");
	waitS(FULL);
	printf("\tP: Esperando acceso a la Recion Critica... \n");
	waitS(MUTEX);
	consumir(); // Region Critica
	printf("\tP: Liberando acceso a bufer\n");
	signalS(MUTEX);
	printf("\tP: Hay un elemento mas en bufer\n");
	signalS(EMPTY);
		
}

// Es el equivalente al x++ del programa sin
// memoria compartida y sin semaforos de C
void consumir(){
	int llave, shm_id, *buffer;
	
	// Creamos la llave
	llave = ftok(".", 'G');
	
	if( llave == -1 )
		perror("\nError al crear la llave\n");
	else{
		shm_id = shmget(llave, sizeof(int), 0666);
		if(shm_id == -1)
			perror("\nError al encontrar la variable\n");
		else{
			buffer = (int *)shmat(shm_id, 0, 0);
			*buffer = *buffer - 1;
			shmdt((char *) buffer);
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