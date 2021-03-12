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
0 FULL: Valor inicial será 0. Cuantos lugares estan
ocupados en el buffer.
1 EMPTY: Valor inicial el tamaño de buffer. Indica cuantos
lugares hay disponibles para consumir.
2 MUTEX: Valor inicial 1. Este da permiso a bloque el acceso
a la región critica.
*/

#define FULL 0
#define EMPTY 1
#define MUTEX 2	
#define BUF_SIZE 1024

void inicializa_valores();
void imprimir();
void borrar();
int TAMANIO_BUFFER;

struct shmseg{
	int buf[BUF_SIZE];
	int full; //lugares ocupados en el buffer
	int empty; //lugares disponibles en el buffer.
};

int main(){
    int opcion;
	printf("******************MONITOR********************\n");
	printf("Dame el valor de n para el tamaño del buffer: ");
	scanf("%d",&TAMANIO_BUFFER);
	
	
    while(1){
        printf("\n1. Inicializar\n2. Imprimir buffer\n3. Borrar\n4. Salir \n");
        printf("\nIntroduce opcion: ");
		scanf("%d",&opcion);
		if(opcion == 1)
			inicializa_valores();
		else if(opcion == 2)
			imprimir();
		else if(opcion == 3)
			borrar();
		else if(opcion == 4)
			exit(0);
	}

}

/*
Se crea la variable compartida y se inicializa en cero
Se crean los semaforos y se inicializan sus valores
*/

void inicializa_valores(){
    int llave;
    int sem_id, shm_id;
    struct shmseg *shmp;
    int *buffer;
    ushort semaforos[3];


    //Creamos la llave
    llave = ftok(".",'G');

    if(llave == -1)
        perror("\nError al general la llave\n");
    else{
        //Inicializamos los semáforos
        sem_id = semget(llave,3,IPC_CREAT|0666); //Se crean los semáforos
        
        if(sem_id == -1)
          perror("\nError al crear los semaforos\n");
        else{
           semaforos[FULL] = 0; // Espacios ocupados inicialmente en el bufer
           semaforos[EMPTY] = TAMANIO_BUFFER;  // Espacios disponibles inicialmente en el bufer
           semaforos[MUTEX] = 1;   // Da o bloquea el acceso a la region critica
           semctl(sem_id, 0, SETALL, semaforos);   // Asigna a los semaforos los valores en el arreglo semaforos
        }

        //Inicializamos la variable compartida
        shm_id = shmget(llave, sizeof(struct shmseg), IPC_CREAT|0666);
        if(shm_id == -1)
            perror("\nError al crear la variable compartida\n");
        else{
            shmp = shmat(shm_id, 0, 0); // Ligamos la memoria creada anteriormente a este proceso)
            buffer = shmp->buf; //Se le asignan los bloques de un buffer a una memoria compartidad
       
            shmp->full=0;
            shmp->empty = TAMANIO_BUFFER;
            
            
            for(int i=0; i<TAMANIO_BUFFER; i++){
            	buffer[i]=0;
            }
            shmdt(shmp);  // Desligamos la memoria creada
        }  
        
        
    } //Fin del if principal
}

// Borra los semaforos y la variable compartida
void borrar(){
	int llave, sem_id, shm_id;
	int *buffer;
	struct shmseg *shmp;
    	
	// Creamos la llave
	llave = ftok(".", 'G');
	
	if( llave == -1)
		perror("\nError al crear la llave\n");
	else{
		sem_id = semget(llave, 3, 0666);
		if(sem_id == -1)
			perror("\nError al encontrar semaforos\n");
		else{
			// Liberar semaforos
			if( (semctl(llave, 0, IPC_RMID)) == -1){
				perror("\nSe borro un espacio\n");
			}
		}
		
		// Borrando memoria
		shm_id = shmget(llave, sizeof(struct shmseg), 0666);
		if(shm_id == -1)
			perror("\nNo se encontro la memoria\n");
		else{
			shmctl(shm_id, IPC_RMID, NULL);
		}
	} // Fin del if principal
	
}

void imprimir(){
	int llave, shm_id, *buffer;
	struct shmseg *shmp;
	int full,empty;
	printf("\nValores del buffer \n");
	// Creamos la llave
	llave = ftok(".", 'G');
	
	if(llave == -1)
		perror("\nNo se pudo crear la llave\n");
	else{
		shm_id = shmget(llave, sizeof(struct shmseg), 0666);
		if(shm_id == -1)
			perror("\nError al encontrar el bufer\n");
		else{
			
			shmp = shmat(shm_id, 0, 0); //Ligamos al buffer a la memoria compartida
			buffer = shmp->buf;
			full = shmp->full;
			empty = shmp->empty;
			for(int i = 0; i<TAMANIO_BUFFER; i++) {
				printf("\nEl valor del arreglo es '%d', en la posicion [%d]",buffer[i],i+1); 
			}
			printf("\nValor de buffer = %d\n",TAMANIO_BUFFER);
			printf("Casillas libres para producir %d\n", empty);
			printf("Casillas llenas para consumir %d\n", full);
			shmdt(shmp);
		}
	}	// Fin del if principal
}

