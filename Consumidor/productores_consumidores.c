#include <stdio.h>
#include <stdlib.h>

void productor();  // Product un elemento y lo agrega al buffer
void consumidor(); // Consume un elemento del buffer
int wait(int);           // Devuelve el valor menos 1 del semáforo que se le pasó como parámetro
int signal(int);        // Devuelve el valor más 1 del semáforo que se le pasó como parámetro

int mutex = 1, lleno = 0, vacio = 3, x = 0;

int main(){
	int n;
	printf("\n1. Productor \n2. Consumidor \n3. Salir\n");
	while(1){
		printf("\nHay %d lugares vacios\nHay %d lugares ocupados\n",vacio, lleno);
		printf("\nIntroduce tu seleccion: ");
		scanf("%d",&n);
		switch(n){
			case 1: if( (mutex == 1) && (vacio != 0))
							productor();
						else
							printf("\nBuffer lleno\n");
						break;
			case 2: if( (mutex == 1) && (lleno != 0))
							consumidor();
						else
							printf("\nBuffer vacio\n");
						break;
			case 3: exit(0);
						break;
		}
	}
} // Fin del main



int wait(int s){
	return --s;
}

int signal(int s){
	return ++s;
}

void productor(){
	vacio = wait(vacio);
	mutex = wait(mutex);
	x++; // producir un elemento : RC
	mutex = signal(mutex);
	lleno = signal(lleno);
}

void consumidor(){
	lleno = wait(lleno);
	mutex = wait(mutex);
	x--; // consumir un elemento: RC
	mutex = signal(mutex);
	vacio = signal(vacio);
}