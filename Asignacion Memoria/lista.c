#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#define MEMORIA 500

// ocupado + desocupado = particion
struct Nodo{
	int tamanio_particion; // Tamanio del hueco
	int tamanio_ocupado;   // Tamanio ocupado
	int tamanio_desocupado; // Tamanio para ocupar
	int index;              // indice del nodo 
	int contador_proceso;   // contador del proceso  
	int procesos[4];			// numero de procesos dentro
	struct Nodo *siguiente_nodo; // Referencia la siguente nodo
};

// Creación de nuevo nodo
// struct Nodo *siguiente_nodo  es  la flecha que apunta a otro nodo o a null
// En Java este sería el constructor para un Nodo
struct Nodo *crea_nodo(int tamanio_particion, struct Nodo *siguiente_nodo){
	struct Nodo *nuevo_nodo = (struct Nodo*) malloc(sizeof(struct Nodo));
	if(nuevo_nodo == NULL){
		printf("\nProblema al crear el nuevo nodo\n");
		exit(0);
	}
	
	nuevo_nodo->tamanio_particion = tamanio_particion;
	nuevo_nodo->tamanio_ocupado = 0;
	nuevo_nodo->tamanio_desocupado = tamanio_particion;
	nuevo_nodo->index = 0;
	nuevo_nodo->contador_proceso = 0;
	nuevo_nodo->siguiente_nodo = siguiente_nodo;
	
	return nuevo_nodo;
}


struct Nodo *inserta_inicio(struct Nodo *primer_nodo, int tamanio_particion){
	struct Nodo *nuevo_nodo = crea_nodo(tamanio_particion, primer_nodo);
	primer_nodo = nuevo_nodo;
	return primer_nodo;	
}


// Si el primer nodo es nulo entonces la lista está vacia
int esta_vacia(struct Nodo *primer_nodo){
	return primer_nodo == NULL;
}


void imprime_lista(struct Nodo *primer_nodo){
	// Si la lista está vacia, solo mandamos un mensaje
	if(esta_vacia(primer_nodo)){
		printf("\nLa lista esta vacia\n");
		return;
	}
	
	printf("\nElementos de la lista ligada\n");
	while(primer_nodo != NULL){
		printf(" %d ",primer_nodo->tamanio_particion);
		primer_nodo = primer_nodo->siguiente_nodo; // avanza al siguiente nodo
	}
}
	
	
int buscar_nodo(struct Nodo *primer_nodo, int tamanio_particion){
	if(esta_vacia(primer_nodo)){
		printf("\nNo hay nodos\n");
		return -1;
	}
	
	while(primer_nodo != NULL){
		if(tamanio_particion == primer_nodo->tamanio_particion)
			return 1;
		primer_nodo = primer_nodo->siguiente_nodo;
	}
	return 0;	
}

struct Nodo *borrar_inicio(struct Nodo *primer_nodo){
    if(esta_vacia(primer_nodo)){
        printf("\nNo hay nodos\n");
        return NULL;
    }
    struct Nodo *aux = primer_nodo;
    primer_nodo = primer_nodo->siguiente_nodo;
    free(aux);
    return primer_nodo;
}


struct Nodo *inserta_final(struct Nodo *primer_nodo, int tamanio_particion){
    if(esta_vacia(primer_nodo))
        return inserta_inicio(primer_nodo,tamanio_particion);

    //Creamos nueno nodo
    struct Nodo *nuevo_nodo = crea_nodo(tamanio_particion,NULL);
    //Creamos el aux y lo posicionamos en el ultimo nodo actual
    struct Nodo *aux = primer_nodo;
    while(aux->siguiente_nodo != NULL)
        aux = aux->siguiente_nodo;

    aux->siguiente_nodo = nuevo_nodo;
    return primer_nodo;
}

struct Nodo *borra_final(struct Nodo *primer_nodo){
    if(esta_vacia(primer_nodo)){
        printf("\nNo hay nodos\n");
        return NULL;
    }

    //Si solo hay un nodo
    if(primer_nodo->siguiente_nodo == NULL)
        return borrar_inicio(primer_nodo);
    
    //Si hay mas nodos
    struct Nodo *aux1=primer_nodo;
    struct Nodo *aux2=primer_nodo->siguiente_nodo;

    while(aux2->siguiente_nodo != NULL){
        aux1 = aux1->siguiente_nodo;
        aux2 = aux2->siguiente_nodo;
    }
    aux1->siguiente_nodo = NULL;
    free(aux2);

    return primer_nodo;
}

//Genera un numero aleatorio
int num_aleatorio(int lower, int upper) {  
	return rand() % (upper - lower + 1) + lower; 
} 

//Metodo para crear lista.
struct Nodo *crea_lista(struct Nodo *primer_nodo){
	int total=0, tamanio_particion, index=0;
	struct Nodo *aux = NULL;
	while( total <= MEMORIA){
		tamanio_particion = num_aleatorio(5,10);
		total += tamanio_particion;
		primer_nodo = inserta_inicio(primer_nodo,tamanio_particion);
	}
	aux = primer_nodo;
	//Se le asigna un indice a cada nodo de la lista
	while(aux != NULL){
		aux->contador_proceso = 0;
		aux->index = index;
		index++;
		aux = aux->siguiente_nodo;
	}
	printf("Total: %d",total);
	return primer_nodo;
}

//Metodo que imprime los valores del segmento.
//El tamaño del la particion,
//el espacio ocupado y 
//el espacio ocupado
void muestra_particion(struct Nodo *primer_nodo){

    printf("%d	\t%d       \t[  %d \t  %d \t   %d  ]\n", primer_nodo->contador_proceso,primer_nodo->index, primer_nodo->tamanio_particion,primer_nodo->tamanio_ocupado,primer_nodo->tamanio_desocupado); 
	
}

//Metodo que imprime el estado de la memoria
void estado_memoria(struct Nodo *lista){
	// Si la lista está vacia, solo mandamos un mensaje
	if(esta_vacia(lista)){
		printf("\nLa lista esta vacia\n");
		return;
	}
	
	printf("\n-------------------ESTADO DE LA MEMORIA-------------------------\n");
	printf("Cont           Indice.     \tTotal.  Ocupado.  Disponible.\n\n");
	while(lista != NULL){
		muestra_particion(lista);
		lista = lista->siguiente_nodo; // avanza al siguiente nodo
	}
	printf("\n");
	printf("------------------------------------------------------------------\n");
}

//Primer metodo de administracion de memoria
void firt_fit(struct Nodo *lista ){
    int i,tam_proceso, tiempo_proceso = 5;
    struct Nodo *primer_nodo = lista;
   	if(esta_vacia(lista)) {
		printf("\nLa lista esta vacia\n");
		return;
	}
	
	for(i = 0; i < 100; i++) {
		tam_proceso = num_aleatorio(3,10); //Se crea un numero aleatorio entre 3 y 10
		
		if(lista == NULL)
			lista = primer_nodo;

		while(lista != NULL){
			if(lista->tamanio_ocupado != 0){
				lista->contador_proceso++;
				if(lista->contador_proceso > tiempo_proceso){ //El proceso que esta dentro sale y el bloque
					lista->tamanio_ocupado = 0;  // regresa a sus valores originales
					lista->tamanio_desocupado = lista->tamanio_particion;
					lista->contador_proceso = 0;
				}
			}else{
				//Se revisa el espacio para el proceso.
				if(tam_proceso <= lista->tamanio_desocupado && lista->tamanio_desocupado != 0) {
					lista->tamanio_ocupado += tam_proceso;
					lista->tamanio_desocupado -= tam_proceso;
					break;
				} 
			}//fin de else
			lista = lista->siguiente_nodo;
		}//fin fr while
	} //fin de for
}


//Metodo para buscar un bloque para el tam_proceso
//y regresa el indice del bloque
int buscar_bloque_best(struct Nodo *lista, int tam_proceso){
	int best_idx = -1, block_size=0, tiempo_proceso = 5;

	while(lista !=  NULL){
		if(lista->tamanio_ocupado != 0){
			lista->contador_proceso++;
			if(lista->contador_proceso > tiempo_proceso){
				lista->contador_proceso = 0;
				lista->tamanio_ocupado = 0;
				lista->tamanio_desocupado = lista->tamanio_particion;
			}
		}else{
			// En esta parte se utiliza el algoritmo best-fit
			if(lista->tamanio_desocupado >= tam_proceso){
				if(best_idx == -1){
					block_size = lista->tamanio_desocupado;
					best_idx = lista->index;
				}else {
					if(block_size > lista->tamanio_desocupado){
						best_idx = lista->index;
						block_size = lista->tamanio_desocupado;
					}	
				}	 
			}
		}	
		lista = lista->siguiente_nodo;
	}

	return best_idx;
}
//Primer metodo de asignacion de memoria.
//Metodo del mejor ajuste busca el hueco que mejor se ajuste.
//al tamanio del proceso
void best_fit(struct Nodo *lista) {
	int tam_proceso;
	if(esta_vacia(lista)){
		printf("\nLa lista esta vacia\n");
		return;
	}

	for(int i=0; i<100; i++){
		tam_proceso = num_aleatorio(3,10); //Se crea un numero aleatorio entre 3 y 10
		int best_idx = -1;
		//Se busca un hueco para el tamanio del proceso
		best_idx = buscar_bloque_best(lista,tam_proceso);
		if(best_idx != -1){
			struct Nodo *aux = lista;
			//Se bsuca el indice devuelto por el metodo busca_bloque
			while(aux != NULL){
				if(best_idx == aux->index){
					aux->tamanio_desocupado -= tam_proceso;
					aux->tamanio_ocupado += tam_proceso;
					aux->contador_proceso = 1;
					break;
				}
				aux = aux->siguiente_nodo;
			} //fin de while
		} // fin de if grande	
	} //fin de for
}


//Metodo para buscar un bloque para el tam_proceso
//y regresa el indice del bloque
int buscar_bloque_worst(struct Nodo *lista, int tam_proceso){
	int worst_idx = -1, block_size=0;;

	while(lista !=  NULL){
		if(lista->tamanio_ocupado != 0){
			lista->contador_proceso++;
			if(lista->contador_proceso == 5){
				lista->contador_proceso = 0;
				lista->tamanio_ocupado = 0;
				lista->tamanio_desocupado = lista->tamanio_particion;
				
			} // fin de if
		}else{
			// En esta parte se utiliza el algoritmo worst-fit
			if(lista->tamanio_desocupado >= tam_proceso){
				if(worst_idx == -1){
					block_size = lista->tamanio_desocupado;
					worst_idx = lista->index;
				}else {
					if(block_size < lista->tamanio_desocupado){
						worst_idx = lista->index;
						block_size = lista->tamanio_desocupado;
					} //fin if
				} // fin de else	 
			} //fin de if
		} //fin de else	
		lista = lista->siguiente_nodo;
	} //fin de while

	return worst_idx;
}

//Tercer método de asignacion de memoria.
//Metodo que da al proceso el bloque con mayor espacio.
void worst_fit(struct Nodo *lista) {
	int tam_proceso;
	if(esta_vacia(lista)){
		printf("\nLa lista esta vacia\n");
		return;
	}

	for(int i=0; i<100; i++){
		tam_proceso = num_aleatorio(3,10); //Se crea un numero aleatorio entre 3 y 10
		int worst_idx = -1;
		//Se busca un hueco para el proceso
		worst_idx = buscar_bloque_worst(lista,tam_proceso);
		if(worst_idx != -1){
			struct Nodo *aux = lista;
			//Se busca el nodo devulto por el metodo busca_bloque
			while(aux != NULL){
				if(worst_idx == aux->index){
					aux->tamanio_desocupado -= tam_proceso;
					aux->tamanio_ocupado += tam_proceso;
					aux->contador_proceso = 1;
					break;
				}
				aux = aux->siguiente_nodo;
			} //fin while
		} //fin de if grande	

	} //fin de for
}

//Cuarto método de asignacion de memoria.
//Se le asigna al proceso el bloque donde quepa
//y si llega otro proceso y cabe en el espacio libre
//lo toma.
void block_splitting(struct Nodo *lista){
	int tam_proceso=0; //Es tamaño del proceso
	int tiempo_proceso = 5;
	struct Nodo *primer_nodo = lista;
	//Si la lista esta vacia envia un mensaje y termina el
	//programa su ejecuciom
	if(esta_vacia(lista)){
		printf("\nLa lista esta vacia\n");
		return;
	}

	for(int i=0; i<100; i++){
		tam_proceso = num_aleatorio(3,10); // Se genera el numero aleatorio
		//Si la lista llega al final y aun no termina el ciclo
		//for se vulve a empezar desde el inicio
		if(lista == NULL)
			lista = primer_nodo;

		while(lista != NULL){
			if(lista->tamanio_ocupado != 0){
				lista->contador_proceso++;
				if(lista->contador_proceso > tiempo_proceso){
					lista->tamanio_ocupado = 0;
					lista->tamanio_desocupado = lista->tamanio_particion;
					lista->contador_proceso = 0;
				}else if(lista->tamanio_desocupado >= tam_proceso){
						lista->tamanio_desocupado -= tam_proceso;
						lista->tamanio_ocupado += tam_proceso;
						lista->contador_proceso = 1;
						break;
					} //fin de else-if
			}else if(lista->tamanio_desocupado >= tam_proceso){
					lista->tamanio_desocupado -= tam_proceso;
					lista->tamanio_ocupado += tam_proceso;
					lista->contador_proceso = 1;
					break;
				}//fin del else-if
			lista=lista->siguiente_nodo; //Se pasa al siguienubte nodo
		}//fin del while
	}
}

int main(){
	// Creamos la lista ligada
	srand(time(NULL));
	struct Nodo *primer_nodo = NULL;
	int opcion;

	//primer_nodo = crea_lista(primer_nodo);
	printf("\t\n**TECNICAS DE ASIGNACION DE MEMORIA**\n");
	while(1){
	    printf("\n1. Crear la lista\n2. Estado de la memoria\n3. First-fit \n4. best-fit \n5. Worst-fit \n6. Blocks splitting \n7. Salir\n");
        printf("\nIntroduce opcion: ");
		scanf("%d",&opcion);
		if(opcion == 1)
			primer_nodo = crea_lista(primer_nodo);
		else if(opcion == 2)
			estado_memoria(primer_nodo);
		else if(opcion == 3){
			firt_fit(primer_nodo);
			estado_memoria(primer_nodo);
		}
		else if(opcion == 4){
			best_fit(primer_nodo);
			estado_memoria(primer_nodo);
		}
		else if(opcion == 5){
			worst_fit(primer_nodo);
			estado_memoria(primer_nodo);
		}
		else if(opcion == 6){
			block_splitting(primer_nodo);
			estado_memoria(primer_nodo);
		}
		else if(opcion == 7)
			exit(0);
	}


    /*
	primer_nodo = inserta_inicio(primer_nodo, 30);
	primer_nodo = inserta_inicio(primer_nodo, 75);
	primer_nodo = inserta_inicio(primer_nodo, 76);
	primer_nodo = inserta_inicio(primer_nodo, 34);
	primer_nodo = inserta_inicio(primer_nodo, 56);
	primer_nodo = inserta_inicio(primer_nodo, 94);
	
	imprime_lista(primer_nodo);
	
	if(buscar_nodo(primer_nodo, 200))
		printf("\nSi existe\n");
	else
		printf("\nNo existe\n");
	*/
    //printf("\nEl estado de la particion es:");
   
  //  muestra_particion(primer_nodo);

    
	/*
    primer_nodo = borrar_inicio(primer_nodo);

    primer_nodo = inserta_final(primer_nodo,135);

	imprime_lista(primer_nodo);

    primer_nodo = borra_final(primer_nodo);

    imprime_lista(primer_nodo);
	*/
}