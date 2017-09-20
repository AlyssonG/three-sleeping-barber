#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <pilha.h>


#define CHAIRS 7
//Oi Alysson eu dei overwrite nas coisas que você fez pq a biblioteca do C para
//semaforo é melhor =P

	
//Lembrete: usar sem_post(a.k.a up) e sem_wait(a.k.a down)
sem_t mutex, barber, client, queue;

int menu = 0;

int clientQueue[CHAIRS];	//Fila de espera do barbeiro/cabelereiro/whatever

int waiting = 0;		//Quantos clientes estao na fila de espera

//Pode pintar cabelo, cortar o cabelo e fazer a barba
pthread_t barPinta, barCabelo, barBa;
//Um barbeiro(thread) para cada funcao
int bId1 = 1, bId2 = 2, bId3 = 3;

//FUNCOES DE BARBEIRO
void pintaCabelo();//essas funcoes sao so printf mesmo ,-,
void cortaCabelo();
void fazBarba();

//Passa o id de um cliente para verificar o serviço a ser realizado
//INCOMPLETO(jura?)
void barber(int id){
	while(1){
		sem_wait(&client);
		sem_wait(&mutex);
		waiting--;
		sem_post(&barbers);
		sem_post(&mutex);
		if(id == 1) cortaCabelo();
		else if(id == 2)	pintaCabelo();
		else fazBarba();
	}
}

int main(void){
	return 0;
}
