#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int main(int argc, char **argv){

//	MI SALVO ALL'INTERNO DELLE VARIABILI TUTTI GLI ELEMENTI PASSATI DAL MAIN AL PROCESSO
//	DELLA CPU. LI CONVERTO DA STRINGHE A INT
	int proc_id  = atoi(argv[1]);
	int shmid    = atoi(argv[2]);
	int semid    = atoi(argv[3]);
	int ramDim	 = atoi(argv[4]);

//	DICHIARO LA STRUTTURA DA UTILIZZARE NELLA MEMORIA CONDIVISA
	shms *shmptr = malloc(sizeof(shms));

//	COLLEGO LA CPU ALLA MEMORIA CONDIVISA CREATA NEL MAIN
	shmptr = (shms*)shm_attach(shmid);

//	CREO LA LISTA DI OPERAZIONI DA ESEGUIRE PER OGNI FILE DI CONFIGURAZIONE
	coda *c = coda_creat(proc_id);

//	METTO IN ATTESA TUTTI I PROCESSI DELLA CPU IN MODO DA FARLI AVVIARE IN ORDINE DA 1 A 4
	sem_wait(proc_id+3,semid);

//	VADO A LEGGERE LA LISTA CREATA IN PRECEDENZA
	do
	{
//		CONTROLLO IL SEGNALE DELLA LISTA
		switch(c->sig)
		{
		case 'S':	print("CPU #%d:\t%c %d\t sleep(%d)\n",proc_id,c->sig,c->num,c->num);
					sleep(c->num);
					break;
		case 'R':
//					SE IL NUMERO (num) LETTO DALLA LISTA È MAGGIORE DELLA DIMENSIONE DELLA
//					RAM STAMPO A VIDEO UN ERRORE DI LETTURA. L'ARRAY CREATO DALLA RAM NON È
//					SUFFICIENTEMENTE GRANDE PER ESEGUIRE L'OPERAZIONE LETTA DALLA LISTA
					if(c->num >= ramDim)
						print("CPU #%d:\t%c %d\t ERRORE DI LETTURA!\n",proc_id,c->sig,c->num);
					else
					{
//						ENTRO NELLA SEZIONE CRITICA.
//						ACCEDO IN MODO ESCLUSIVO ALLA MEMORIA CONDIVISA
						sem_wait(SEM_SEZCR,semid);
							shmptr->sig=c->sig;
							shmptr->num=c->num;
							shmptr->sem_id = proc_id+3;
							sem_signal(SEM_RAM,semid);
							sem_wait(proc_id+3,semid);
							print("CPU #%d:\t%c %d\t LETTURA   -> array[%d] = %d\n",proc_id,shmptr->sig,shmptr->num,shmptr->num,shmptr->read);
//							ESCO DALLA SEZIONE CRITICA
						sem_signal(SEM_SEZCR,semid);
					}
					break;
		case 'W':
//					SE IL NUMERO (pos) LETTO DALLA LISTA È MAGGIORE DELLA DIMENSIONE DELLA
//					RAM STAMPO A VIDEO UN ERRORE DI LETTURA. L'ARRAY CREATO DALLA RAM NON È
//					SUFFICIENTEMENTE GRANDE PER ESEGUIRE L'OPERAZIONE LETTA DALLA LISTA
					if(c->pos >= ramDim)
						print("CPU #%d:\t%c %d %d\t ERRORE DI SCRITTURA!\n",proc_id,c->sig,c->num,c->pos);
					else
					{
//						ENTRO NELLA SEZIONE CRITICA.
//						ACCEDO IN MODO ESCLUSIVO ALLA MEMORIA CONDIVISA
						sem_wait(SEM_SEZCR,semid);
							shmptr->sig=c->sig;
							shmptr->num=c->num;
							shmptr->pos=c->pos;
							shmptr->sem_id = proc_id+3;
							sem_signal(SEM_RAM,semid);
							sem_wait(proc_id+3,semid);
							print("CPU #%d:\t%c %d %d\t SCRITTURA -> array[%d] = %d\n",shmptr->sem_id - 2,shmptr->sig,shmptr->num,shmptr->pos,shmptr->pos, shmptr->num);
//							ESCO DALLA SEZIONE CRITICA
						sem_signal(SEM_SEZCR,semid);
					}
					break;
		}


//		LEGGO L'ELEMENTO SUCCESSIVO DELLA LISTA
		c = c->next;
	}
	while(c);

//	INCREMENTO counter. SIGNIFICA CHE LA CPU HA TERMINATO LA LETTURA DEL FILE DI
//	CONFIGURAZIONE
	shmptr->counter++;

//	DÒ LA SIGNAL ALLA RAM
	sem_signal(SEM_RAM,semid);

//	SCOLLEGO LA MEMORIA CONDIVISA DALLA CPU E TERMINO L'ESECUZIONE
	shm_detach(shmptr);

	return 0;
}
