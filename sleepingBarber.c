#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"


#define CHAIRS 7

//Semaforos para barbeiros, clientes e exclusao mutua
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
//O sleep serve para randomizar o tempo que cada cliente leva para cortar o cabelo
//Assim a execucao fica mais realista e a fila consegue encher/esvaziar mais facilmente.
void pintaCabelo(){printf("O barbeiro está pintando o cabelo!\n");sleep(rand()%25);}
void cortaCabelo(){printf("O barbeiro está cortando o cabelo!\n");sleep(rand()%15);}
void fazBarba(){printf("O barbeiro está fazendo a barba!\n");sleep(rand()%7);}

//FUNCOES DE CLIENTE
void cabelo(){printf("O cliente está cortando o cabelo!\n");}
void pinta(){printf("O cliente está pintando o cabelo!\n");}
void barba(){printf("O cliente está fazendo a barba!\n");}

//Passa o id de um cliente para verificar o serviço a ser realizado
void barber(int * id){
	while(1){
		//Checa se existe algum cliente do tipo de servico desejado na fila
		if(* id == 1) sem_wait(&client1);
		else if(* id == 2) sem_wait(&client2);
		else if(* id == 3) sem_wait(&client3);

		sem_wait(&mutex);//Entra na regiao critica para tal barbeiro
		waiting--;
		sem_post(&mutex);//Sai da regiao critica

		//Libera o barbeiro/cabelereiro/sei la e faz o servico
		if(* id == 1){sem_post(&barber1); cortaCabelo();}
		else if(* id == 2){sem_post(&barber2);pintaCabelo();}
		else if(* id == 3){sem_post(&barber3); fazBarba();}
	}
	pthread_exit(NULL);
}

void client(int * id) {
	sem_wait(&mutex);

	if(waiting < CHAIRS) {
		printf("Chegou um cliente %d!\n", * id);
		waiting++;//Coloca o cliente na fila

		//Verifica se o barbeiro esta dormindo. Se estiver, acorda-o.
		if(* id == 1) sem_post(&client1);
		if(* id == 2)	sem_post(&client2);
		if(* id == 3)	sem_post(&client3);

		sem_post(&mutex);//Sai da exclusao mutua

		//Atende o cliente e coloca o barbeiro pra dormir
		if(* id == 1){sem_wait(&barber1);cabelo();}
		if(* id == 2){sem_wait(&barber2);pinta();}
		if(* id == 3){sem_wait(&barber3);barba();}

	}
	else{
		sem_post(&mutex);//Sai da exclusao mutua se a fila esta cheia
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

	pthread_t c1;
	
	srand(time(NULL));
	int vetor[3] = {1, 2, 3};//Vetor para tipos de clientes

	//Cria as threads de barbeiro
	pthread_create(&barCabelo, NULL, (void *) barber, &bId1);
	pthread_create(&barPinta, NULL, (void *) barber, &bId2);
	pthread_create(&barBa, NULL, (void *) barber, &bId3);

	while(1){
		//Cria  threads de cliente a partir do vetor de id
 		pthread_create(&c1, NULL, (void *) client, &vetor[rand()%3]);
		sleep(rand()%5);
	}
	return 0;
}
