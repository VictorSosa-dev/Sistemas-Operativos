#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*

i: es el indice para recorrer el arreglo de peticiones
j: es el indice para recorrer el arreglo de marcos
marcos[]:  los marcos donde se agregaran las paginas entrantes
peticiones[]:  va a contener las paginas que piden memoria
*/

/*************VARIABLES QUE SE VAN A OCUPAR**************/
int num_peticiones, num_marcos, num_fallas = 0;
int bandera1, bandera2, lugar_a_ocupar = 0;
int i, j, menos_usada;

/************DECLARACION DE LOS METODOS********************/

void fifo(int *peticiones, int *marcos);
int num_aleatorio(int lower,int upper);
void aleatorio(int *peticiones, int *marcos);
void segunda_oportunidad(int *peticiones, int *marcos);
void lru(int *peticiones, int *marcos);
void optimo(int *peticiones, int *marcos);

int main(){
	int opcion=0; 
	srand(time(NULL));
	printf("\nIntroduce el numero de peticiones: ");
			scanf("%d",&num_peticiones);
			int peticiones[num_peticiones];
			
			printf("\nIntroduce las peticiones:\n");
			for(i = 0; i < num_peticiones; i++){
				printf("Peticion %d: ",i+1);
				scanf("%d",&peticiones[i]);
			}

			printf("\nIntroduce el numero de marcos: ");
			scanf("%d",&num_marcos);
			int marcos[num_marcos];
			for(i = 0; i < num_marcos; i++)
				marcos[i] = -1;  // -1 significa que ese marco esta desocupado
 
 	while(1){
        printf("Elige una opcion.\n1.- Inicializa valores\n2.- FIFO\n3.- RANDOM\n4.- SEGUNDA OPORTUNIDAD\n5.- OPTIMO\n6.- LRU\n7.- SALIR\n");
        printf("Introduce la opcion: ");
        scanf("%d",&opcion);
        if(opcion == 1){
			printf("\nIntroduce el numero de peticiones: ");
			scanf("%d",&num_peticiones);
			peticiones[num_peticiones];
			
			printf("\nIntroduce las peticiones:\n");
			for(i = 0; i < num_peticiones; i++){
				printf("Peticion %d: ",i+1);
				scanf("%d",&peticiones[i]);
			}

			printf("\nIntroduce el numero de marcos: ");
			scanf("%d",&num_marcos);
			marcos[num_marcos];
			for(i = 0; i < num_marcos; i++)
				marcos[i] = -1;  // -1 significa que ese marco esta desocupado
			num_fallas = 0;
		}else if(opcion == 2){
			printf("\n******************FIFO*******************\n");
            fifo(peticiones,marcos);
		}else if(opcion == 3){
			printf("\n******************RANDOM******************\n");
			aleatorio(peticiones,marcos);
		}else if(opcion ==4){
			printf("\n*********SEGUNDA OPORTUNIDAD FIFO*********\n");
			segunda_oportunidad(peticiones,marcos);
		}else if(opcion == 5){
			printf("\n*****************OPTIMO*******************\n");
			optimo(peticiones,marcos);
		}else if(opcion == 6){
			printf("\n*******************LRU********************\n");
			lru(peticiones,marcos);	     
		}else if(opcion == 7)
                return 1;
  
    }
}	

/************ALGORITMOS DE PAGINACION********************/

/**
* @param *peticiones apuntador a las peticiones de pagina
* @param *marcos apuntador a los marcos
*
*/
void fifo(int *peticiones, int *marcos){
	num_fallas = 0;
	for(int k=0; k < num_marcos; k++)
		marcos[k] = -1;

    for(i = 0; i < num_peticiones; i++){
		bandera1 = 0;
		
		for(j = 0; j < num_marcos; j++){
			if(marcos[j] == peticiones[i])
				bandera1 = 1; // Si esta la pagina en memoria y entonces NO hay reemplazo
		}
		
		// Si bandera == 0 la pagina entrante no esta en memoria y SI hay reemplazo
		if(bandera1 == 0){
			printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[lugar_a_ocupar]);
			num_fallas++;
			marcos[lugar_a_ocupar] = peticiones[i];
			lugar_a_ocupar = (lugar_a_ocupar+1)%num_marcos;
		}		
		
	} // For principal
	
	printf("\nNumero de fallas = %d con %d marcos\n",num_fallas, num_marcos);		
}

//Genera un numero aleatorio entre un intervalo
int num_aleatorio(int lower, int upper) {  
	return rand() % (upper - lower + 1) + lower; 
} 
/**
* El algoritmo cuando los marcos ya estan llenos se 
* elige aleatoriamente un marco y ese va ser en 
* donde se va a insertar la peticion de pagina actual.
* @param *peticiones apuntador a las peticiones de pagina
* @param *marcos apuntador a los marcos
*/
void aleatorio(int *peticiones, int *marcos){
	num_fallas = 0;
	for(int k=0; k < num_marcos; k++)
		marcos[k] = -1;

	int indice; // Variable para guardar el marco donde se va remplazar su pagina
	for(i = 0; i < num_peticiones; i++){
		bandera1 = 0; bandera2 = 0;
		for(j = 0; j < num_marcos; j++){
			if(marcos[j] == peticiones[i]){
				bandera1 = 1; // Si esta la pagina en memoria y entonces NO hay reemplazo
				bandera2 = 1;
			}
		}

		// Si bandera1 == 0 la pagina entrante no esta en memoria
		// en esta condicion se llenan primero todos los marcos
		if(bandera1 == 0){
			for(int k = 0; k<num_marcos; k++){
				if(marcos[k] == -1){
					printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[k]);
					num_fallas++;
					bandera2 = 1;
					marcos[k] = peticiones[i];
					break;
				}
			}
		}
		// Si bandera2 == 0 la pagina entrante no esta en memoria y SI hay reemplazo
		// en esta condicion es cuando los marcos ya tienen una pagina y se debe de 
		// elegir uno aleatoriamente.
		if(bandera2 == 0){
			indice = num_aleatorio(0,num_marcos-1);// el metodo devulve un marco aleatoriamente
			printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[indice]);
			num_fallas++;
			marcos[indice] = peticiones[i];		
		}		
	} // fin del for principal
	
	printf("\n\nNumero de fallas = %d con %d marcos\n",num_fallas, num_marcos);	
}
/**
* EL metodo busca a la izquierda la peticion menos pedida
* y esa va ser la que es candidata a salir.
* @param *peticiones apuntador a las peticiones de pagina
* @param *marcos apuntador a los marcos
*
*/
void lru(int *peticiones, int *marcos){
	num_fallas = 0;
	for(int k=0; k < num_marcos; k++)
		marcos[k] = -1;
	//Este arreglo se usa para saber cual de los paginas
	//en los marcos ha sido la menos usada.
	//Por eso en el arreglo menos_usado se incrementa 
	//en  uno cada que hay una peticion solo si no 
	//hay referencia si hay referencia en esa posicion 
	//se pone en cero en el arrglo menos_usado y asi 
	//sabemos en que marco se va a hacer el remplazo
	//porque buscamos en el arreglo menos_usado
	//la posicion que tenga el numero mas grande,
	//porque esa pagina es la menos usada
	 int menos_usado[num_marcos]; 
	 for(i = 0; i < num_marcos; i++)
 	 	menos_usado[i] = -1;

	// Este ciclo se usa para recorrer el 
	// arreglo de las peticiones
	 for(i=0; i < num_peticiones; i++){
        bandera1=0; // bandera para llenar los arreglos
		bandera2=0; // bandera para realizar un remplazo
		// Este ciclo se usa para recorrer los marcos
        for(j=0; j < num_marcos; j++){
            if(marcos[j] == peticiones[i]){ // hay una referencia a esa pagina
                bandera1=1; // si hay refeencia de la pagina
				bandera2=1; // las dos bandaras se ponenen a uno.
				 //se incrementa el valor de las pagina
                for(int x = 0; x < num_marcos; x++){
					if(menos_usado[x] != -1)
						menos_usado[x]++;
				}
                menos_usado[j]=0;   // Como la pagina ya tenia una referencia
									// entonces su valor en el arreglo cambia a cero 
            }
        }
		// Cuando los marcos estan vacios 
        if(bandera1 == 0){
            for(j = 0; j < num_marcos; j++){
                if(marcos[j] == -1){
					printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[lugar_a_ocupar]);
                    marcos[lugar_a_ocupar] = peticiones[i];
					lugar_a_ocupar = (lugar_a_ocupar+1)%num_marcos;
                    bandera2=1;  // la bandera dos se pone a 1 y ya no se entra en el siguiente if
                    num_fallas++; //Es una falla porque no hay referencia a la pagina
					// se incrementan los valores en arreglo menos usado
                    for(int x = 0; x < num_marcos; x++){
						if(menos_usado[x] != -1) 
							menos_usado[x]++;
					}
                    menos_usado[j]=0; // como la
                	break;
                }
            }
        } 
		//Cuando estan llenos los marcos 
		//debemos ver cual es el que menos 
		//se usado a la izaquierda
        if(bandera2 == 0){
            int mayor=-1,posicion=0;
			//se busca en que marco se va a hacer el remplazo
    		for(int k = 0; k < num_marcos; k++){
      			if(menos_usado[k] > mayor){
           	  		mayor = menos_usado[k];
              		posicion=k; 
        		}
    		}

			printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[posicion]);
            marcos[posicion] = peticiones[i];
            for(int x = 0; x < num_marcos; x++){
				if(menos_usado[x] != -1)
					menos_usado[x]++;
			}
			// como la pagina acaba de entrar su valor en el arrglo menos_usado es cero
            menos_usado[posicion]=0; 
            num_fallas++;
        }
    }
    printf("\nNumero de fallas = %d con %d marcos\n",num_fallas, num_marcos);
}

/**
* Este metodo esta basado en fifo y da una segunda opotunidad 
* si la pagina ya tiene referencia a en los marcos su bit 
* cambia a uno, cuando la pagina estra por primera vez
* su bit tiene valor 0 y si es remplazada igual el valor es 0.
* @param *peticiones apuntador a las peticiones de pagina
* @param *marcos apuntador a los marcos
*/
void segunda_oportunidad(int *peticiones, int *marcos){
	num_fallas = 0;
	for(int k=0; k < num_marcos; k++)
		marcos[k] = -1;
	int bit[num_marcos];
	// Se inicializa el arreglo de bits
	for(int x = 0; x < num_marcos; x++)
		bit[x] = -1;

	// Con este ciclo recorremos el arreglo de peticiones de pagina
	for(i = 0; i < num_peticiones; i++){
		bandera1 = 0;
		bandera2 = 0;
		// Este ciclo recorremos el arreglo de los marcos
		// y solo se usa cuando tenemos una referencia 
		// una pagina en los marcos y lo unico que hacemos
		// es poner las banderas a 1 y el bit a 1 	
		for(j = 0; j < num_marcos; j++){
			if(marcos[j] == peticiones[i]){
				bandera1 = 1; // Si esta la pagina en memoria y entonces NO hay reemplazo
				bandera2 = 1;
				bit[j] = 1; // Como ya hay una referencia a la pagina su bit cambia a uno
			}
		}
		
		// Si bandera == 0 la pagina entrante no esta en memoria.
		if(bandera1 == 0){
			for(int m=0; m < num_marcos; m++){
				// Aqui se meten las paginas cuando hay marcos disponibles
				if(marcos[m] == -1){
					printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[lugar_a_ocupar]);
					num_fallas++;
					marcos[lugar_a_ocupar] = peticiones[i];
					bit[m] = 0;
					lugar_a_ocupar = (lugar_a_ocupar+1)%num_marcos;
					bandera2 = 1;
					break;
				} //fin del if
			}// fin del for
		}// fin del if

		// Cuando ya tenemos los marcos llenos se revisa cual pagina 
		// va de los marcos va ser remplazada
		while(bandera2 == 0){
			//Si el bit la pagina se remplaza
			if(bit[lugar_a_ocupar] == 0){
				printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[lugar_a_ocupar]);
				marcos[lugar_a_ocupar] = peticiones[i];
				bit[lugar_a_ocupar] = 0;
				lugar_a_ocupar = (lugar_a_ocupar+1)%num_marcos;
				num_fallas++;
				bandera2 = 1;
			}else if(bit[lugar_a_ocupar] == 1){ // Como el bit es cero se le da una segunda oportunidad
					bit[lugar_a_ocupar] = 0;
					lugar_a_ocupar = (lugar_a_ocupar+1)%num_marcos;	
			}// fin del else
			
		}//fin de while	
		
	} // For principal
	
	printf("\nNumero de fallas = %d con %d marcos\n",num_fallas, num_marcos);

}

/**
* El metodo regresa cual de los marcos va ser donde se va a 
* insertar la pagina 
* @param *m Apuntadaor a los marco.
* @param n_m Numero de marcos.
* @param *p Apuntador a las peticiones.
* @param n_p Numeros de paginas.
* @param p_actual Posicion acrual de la pagina que va a ser insertada
*/
int buscar_optimo(int *m,int n_m,int *p,int n_p,int p_actual){
    int posicion=-1;//Variable donde se guarda la posicion si se encrentra la pagina mas aljada 
	int longitud=0; // Variable para calrcular a longitud de las paginas
	int mayor=999;  // Variable para comparar las distancias de la paginas e ir guardando el valor 
					// de la mas alejada
    // Se recorren los marcos para ver en 
	// cual se va a a insertar la pagina
	for(int x = 0; x < n_m; x++){
        longitud=0;
		// Se recorreren las peticiones a partir
		// de la posicion de la peticion actual 
		// de la pagina
        for(int y = p_actual; y < n_p; y++){
			// Se compara la pagina del 
			// marco con las peticiones, cuando se encuentra
			// se pasa el valor a la longitud a mayor 
			// y se continua con la otra pagina de los 
			// marcos
            if(p[y] == m[x] && posicion == -1){ 
				mayor = longitud;
                posicion = x;
                break;
            }
			// Se compara el valor de longitud y mayor 
			// para saber si la pagina el marco que se 
			// esta comparando esta mas alejado que el anterior.
            if(p[y] == m[x] && longitud > mayor){
				mayor = longitud;
                posicion = x;
            	break;
            }
			// Cada que son diferentes la pagina del marco y 
			// la que se esta comparando se incrementa en uno 
			// la longitud
            if(p[y] != m[x])
                longitud++;
        }
		// Si la pagina ya no aparece en las peticiones, se devuelve 
		// el marco donde se va a insertar la pagina, porque ya no 
		// abra referencia a esa pagina
        if(longitud >= (n_p - p_actual))
            return x;
    }
	// Se devulve la posicion del marco donde se va
	// a insertar la pagina
    return posicion;
}
/**
* EL metodo busca a la derecha de la lista de peticion
* la pagina que tiene en cada uno de los marcos y elige el 
* marco que tiene la pagina que esta mas a derecha o que ya 
* no aparece.
* y esa va ser la que es candidata a salir.
* @param *peticiones apuntador a las peticiones de pagina
* @param *marcos apuntador a los marcos
*
*/
void optimo(int *peticiones, int *marcos){
	num_fallas = 0;
	for(int k=0; k < num_marcos; k++)
		marcos[k] = -1;
	int posicion; //Variable donde estara la posicion de la pagina que va salir			  
	 for(i=0; i < num_peticiones; i++){
        bandera1=0;
		bandera2=0;
        for(j=0; j < num_marcos; j++){
            if(marcos[j] == peticiones[i]){ // hay una referencia a esa pagina
                bandera1=1; // Si se encontro la pagina
				bandera2=1; // y no se hace nada mas
            }
        }
		// Cuando los marcos estan vacios 
        if(bandera1 == 0){
            for(j = 0; j < num_marcos; j++){
                if(marcos[j] == -1){
					printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[lugar_a_ocupar]);
                    marcos[lugar_a_ocupar] = peticiones[i];
					lugar_a_ocupar = (lugar_a_ocupar+1)%num_marcos;
                    bandera2=1; 
                    num_fallas++; //Es una falla porque no hay referencia a la pagina
                	break;
                }// fin del id
            }
        }
		// Cuando estan llenos los marcos 
		// debemos ver en la lista de peticiones  
		// cual es que esta mas elejado a la derecha 
		// se vulve a aparece
        if(bandera2 == 0){
		   posicion = buscar_optimo(marcos,num_marcos,peticiones,num_peticiones,i); 
		   printf("\nFallo de pagina detectado, reemplazamos la pagina %d",marcos[posicion]);
		   marcos[posicion] = peticiones[i];
		   num_fallas++;
        }
    }
    printf("\nNumero de fallas = %d con %d marcos\n",num_fallas, num_marcos);
}
