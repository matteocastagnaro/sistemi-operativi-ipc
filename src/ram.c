#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

// NUMERO SEMAFORO -> 2

int main(int argc, char **argv) {

//	MI SALVO ALL'INTERNO DELLE VARIABILI TUTTI GLI ELEMENTI PASSATI DAL MAIN AL PROCESSO
//	DELLA RAM. LI CONVERTO DA STRINGHE A INT
	int ramDim = atoi(argv[1]);
	int shmid = atoi(argv[2]);
	int semid = atoi(argv[3]);
	int numCpu = atoi(argv[4]);

//	COSTRUISCO L'ARRAY DI INTERI DI DIMENSIONE ramDim
	int array[ramDim], i;

//	DICHIARO LA STRUTTURA PER LA MEMORIA CONDIVISA
	shms *shmptr = malloc(sizeof(shms));

//	INIZIALIZZO TUTTI GLI ELEMENTI DELL'ARRAY A 0
	for (i = 0; i < ramDim; i++)
		array[i] = 0;

//	COLLEGO LA MEMORIA CONDIVISA ALLA RAM
	shmptr = (shms*) shm_attach(shmid);

//	SE counter È UGUALE A numCpu SIGNIFICA CHE TUTTE LE CPU HANNO TERMINATO L'ESECUZIONE,
//	QUINDI POSSO USCIRE DAL CICLO
	 do
	 {
//		RESTO IN ATTESA DI UN SEGNALE DALLA CPU
		sem_wait(SEM_RAM, semid);

//		CONTROLLO IL SEGNALE PASSATO ALLA MEMORIA CONDIVISA
		switch (shmptr->sig) {
		case 'W':
				array[shmptr->pos] = shmptr->num;
//				DÒ IL SEGNALE ALLA CPU CHE HA CHIAMATO LA RAM
				sem_signal(shmptr->sem_id,semid);
				break;
		case 'R':
				shmptr->read = array[shmptr->num];
//				DÒ IL SEGNALE ALLA CPU CHE HA CHIAMATO LA RAM
				sem_signal(shmptr->sem_id, semid);
				break;
		}
	}
	while (shmptr->counter < numCpu);

//	HO TERMINATO CON LA RAM. DÒ IL SEGNALE AL MAIN
	sem_signal(SEM_MAIN, semid);

//	SCOLLEGO LA RAM DALLA MEMORIA CONDIVISA
	shm_detach(shmptr);

	return 0;
}
