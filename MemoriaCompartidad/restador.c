#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define TAMANIO 1


void main(){
	int shmid;   // ID del segmento de memoria
	key_t key;   // Nombre del segmento de memoria
	int *shm, *s;
	
	key = 56789;
	
	
	// Creamos el segmento de memoria
	if(   (shmid = shmget(key, TAMANIO, IPC_CREAT|0666)) < 0){
		perror("Error al crear el segmento de memoria");
		exit(1);
	}
	
	// Ligamos el segmento al espacio de direcciones
	if( (shm = shmat(shmid, NULL, 0)) == (int *)-1){
		perror("Error al ligar la memoria");
		exit(1);
	}
	
	// se le pasa la direccion del espacio de memoria 
	s = shm;
	
//	fork();
//	fork();
//	fork();
	
	*s = 0;
	int i = 0;
	while(i < 20000){
		*s = *s - 1;
		printf("\nValor de variable en restador = %d",*s);
		i++;
	}
	
	
}
