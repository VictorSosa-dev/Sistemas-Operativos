/*
* Programa creado por Jorge Duran para Somos Binarios
*/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>


/****************************************************************
Funciones auxiliares para inicializar, hacer wait y hacer Signal
Funcionan con arrays de semaforos, si solo hay uno ese parametro es 0
*******************************************************************/
void error(char* errorInfo) {
    fprintf(stderr,"%s",errorInfo);
    exit(1);
}

/*
sembuf tiene los siguientes campos
struct sembuf{
	unsigned short sem_num;   // número del semáforo dentro del conjunto
	short       sem_op;             // clase de operación. 0 para wait y 1 para signal o incrementar
}
*/

void doSignal(int semid, int numSem) {
    struct sembuf sops; //Signal
    sops.sem_num = numSem;
    sops.sem_op = 1; // Signal o ++
    sops.sem_flg = 0;

    if (semop(semid, &sops, 1) == -1) {
        perror(NULL);
        error("Error al hacer Signal");
    }
}


void doWait(int semid, int numSem) {
    struct sembuf sops;
    sops.sem_num = numSem; /* Sobre el primero, ... */
    sops.sem_op = -1; /* ... un wait (resto 1) */
    sops.sem_flg = 0;

    if (semop(semid, &sops, 1) == -1) {
        perror(NULL);
        error("Error al hacer el Wait");
    }
}

void initSem(int semid, int numSem, int valor) { //iniciar un semaforo
	// semctl: para operaciones de lectura y escitura del estado del semáforo. Incluye la destrucción del semáforo
	// También le da un valor inicial al semáforo
	// argumentos: id del semáforo,  numSem es el número de semáforo con el que se va a trabajar (si hay más de uno), y 
	// el tercero es csm, y es la operación aplicada. El cuarto es opcional
    if (semctl(semid, numSem, SETVAL, valor) < 0) {        
        perror(NULL);
        error("Error iniciando semaforo");
    }
}

int main() {
    puts("Sincronizacion con Semaforos ");
    int semaforo;    

    //Creamos un semaforo y damos permisos para compartirlo
	// La función semget devuelve el semáforo identificado con una llave asociada.
	// El primer argumento: key en este caso tiene como valor IPC, obliga a semget a crear un identificador único.
	// El segundo argumento indica el número de semáforos
	// Tercer argumento indica que se crea el semáforo y los permisos
    if((semaforo=semget(IPC_PRIVATE,1,IPC_CREAT | 0666))<0) {
        perror(NULL);
        error("Semaforo: semget");
     }

	// Inicializamos el semáforo
    initSem(semaforo,0,1);
    puts("Hay una plaza libre");  

    switch (fork())
    {
        case -1:
            error("Error en el fork"); 

        case 0:  /* Hijo */
            doWait(semaforo,0);
            puts("Entro el hijo a su RC, el padre espera");
            sleep(5);
            puts("El hijo sale de RC");
            doSignal(semaforo,0);
            exit(0);

        default: /* Padre */
            doWait(semaforo,0);
            puts("Entro el padre a la RC, el hijo espera");
            sleep(5);
            puts("El padre sale de RC");
            doSignal(semaforo,0);
    }       
    
    sleep(20);
    
    //Liberacion del semaforo
    if ((semctl(semaforo, 0, IPC_RMID)) == -1) {
        perror(NULL);
        error("Semaforo borrando");
    }
    return 0;
}