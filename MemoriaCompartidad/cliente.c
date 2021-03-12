#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TAMANIO 27

void main(){
	int shmid;
	key_t key;
	char *shm, *s;
	
	key = 56789;
	
	// Localizamos el segmento de memoria
	if( (shmid = shmget(key, TAMANIO, 0666)) < 0){
		perror("No se encontró el segmento de memoria");
		exit(1);
	}
	
	// Ligamos el segmento
	if( (shm = shmat(shmid, NULL, 0)) == (char *) -1){
		perror("Error al ligar el segmento");
		exit(1);
	}
	
	// Se leen los caracteres puestos por el servidor en
	// la memoria compartida
	for(s = shm;  *s != '\0'; s++)
		putchar(*s);
	
	putchar('\n');
	
	// Cambiamos el primer caracter por '*'
	*shm = '*';
	exit(0);
	
}
