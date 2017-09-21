#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"


#define CHAIRS 7

//Lembrete: usar sem_post(a.k.a up) e sem_wait(a.k.a down)
sem_t mutex, barber1, barber2, barber3;
sem_t client1, client2, client3;
int menu = 0;

int clientQueue[CHAIRS];	//Fila de espera do barbeiro/cabelereiro/whatever
int waiting = 0;		//Quantos clientes estao na fila de espera

//Pode pintar cabelo, cortar o cabelo e fazer a barba
pthread_t barPinta, barCabelo, barBa;

//Um barbeiro(thread) para cada funcao
int bId1 = 1, bId2 = 2, bId3 = 3;

//FUNCOES DE BARBEIRO
void pintaCabelo(){printf("O barbeiro está pintando o cabelo!\n");sleep(rand()%25);}//essas funcoes sao so printf mesmo ,-,
void cortaCabelo(){printf("O barbeiro está cortando o cabelo!\n");sleep(rand()%15);}
void fazBarba(){printf("O barbeiro está fazendo a barba!\n");sleep(rand()%7);}

//FUNCOES DE CLIENTE
void cabelo(){printf("O cliente está cortando o cabelo!\n");}
void pinta(){printf("O cliente está pintando o cabelo!\n");}
void barba(){printf("O cliente está fazendo a barba!\n");}

//Passa o id de um cliente para verificar o serviço a ser realizado
void barber(int * id){
	while(1){
		if(* id == 1) sem_wait(&client1);
		else if(* id == 2) sem_wait(&client2);
		else if(* id == 3) sem_wait(&client3);

		sem_wait(&mutex);
		waiting--;
		sem_post(&mutex);

		if(* id == 1){sem_post(&barber1); cortaCabelo();}
		else if(* id == 2){sem_post(&barber2);pintaCabelo();}
		else if(* id == 3){sem_post(&barber3); fazBarba();}
	}
	pthread_exit(NULL);
}

void client(int * id) {
	sem_wait(&mutex);           /* entra na região crítica */

	if(waiting < CHAIRS) {      /* se não houver cadeiras vazias, saia */
		printf("Chegou um cliente %d!\n", * id);
		waiting++;

		if(* id == 1){
			sem_post(&client1);
		}

		if(* id == 2){
			sem_post(&client2);
		}

		if(* id == 3){
			sem_post(&client3);
		}

		sem_post(&mutex);
		if(* id == 1){sem_wait(&barber1);cabelo();}
		if(* id == 2){sem_wait(&barber2);pinta();}
		if(* id == 3){sem_wait(&barber3);barba();}

	}
	else{
		sem_post(&mutex);       /* a barbearia está cheia; não espera */
		printf("A fila estava muito grande e o cliente foi embora!\n");
	}
	pthread_exit(NULL);
}


int main(void){

	sem_init(&client1, 1, 0);
	sem_init(&client2, 1, 0);
	sem_init(&client3, 1, 0);
	sem_init(&barber1, 1, 0);
	sem_init(&barber2, 1, 0);
	sem_init(&barber3, 1, 0);
	sem_init(&mutex, 1, 1);

	pthread_t c1,c2,c3;
	int idCliente;
	srand(time(NULL));
	int vetor[3] = {1, 2, 3};

	pthread_create(&barCabelo, NULL, (void *) barber, &bId1);
	pthread_create(&barPinta, NULL, (void *) barber, &bId2);
	pthread_create(&barBa, NULL, (void *) barber, &bId3);

	while(1){
 		pthread_create(&c1, NULL, (void *) client, &vetor[rand()%3]);
		sleep(rand()%5);
	}
	return 0;
}
