#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define TAMANIO 27

void main(){
	char c;
	int shmid; // ID del segmento de memoria compartida
	key_t key;
	char *shm, *s;

	key = 56789;  // Nombramos el segmento de memoria compartida

	// Creamos el segmento
	if( (shmid = shmget(key, TAMANIO, IPC_CREAT|0666)) < 0){
		perror("Error al crear el segmento de memoria");
		exit(1);
	}

	// Ligamos el segmento al espacio de direcciones
	if( (shm = shmat(shmid, NULL, 0)) == (char *)-1){
		perror("Error al ligar la memoria");
		exit(1);
	}

	// pasamos a s el apuntador a la memoria compartida
	s = shm;
	
	// Ponemos información sobre la memoria compartida
	for(c = 'a'; c <= 'z'; c++)
		*s++ = c;
	
	/*
	ponemos en espera el servidor hasta que el proceso
	cliente cambie el primer caracter de la memoria por
	'*', lo cual, indicará que ha leído la información que
	el servidor puso en la memoria
	*/
	while(*shm != '*') // shm[0] != '*'
		sleep(1);
	exit(0);
}
