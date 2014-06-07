/**
 *@file sem.c
 *@author Matteo Castagnaro - VR360261
*/

#include "utils.h"

void sem_wait (int sem_number, int semid)
{
	static struct sembuf wait_w;
	wait_w.sem_op  = -1;
	wait_w.sem_num = sem_number;
	wait_w.sem_flg = 0;

	if (semop(semid,&wait_w,1)==-1)
		syserr(P_NAME,"*sem_wait* semop");

}

void sem_signal (int sem_number, int semid)
{
	static struct sembuf signal_s;
	signal_s.sem_op  = 1;
	signal_s.sem_num = sem_number;
	signal_s.sem_flg = 0;

	if (semop(semid,&signal_s,1)==-1)
		syserr(P_NAME,"*sem_signal* semop");


}

int sem_creat(int num_sem, char key)
{
	static int semid,i;

//	GENERO UN ID UNIVOCO PER IL SEMAFORO
	semid = semget(ftok("/usr", key), num_sem ,IPC_CREAT | 0666 |IPC_EXCL);

//	CONTROLLO CHE IL SEMAFORO SIA STATO CREATO, ALTRIMENTI DEALLOCO IL SEMAFORO
	if(semid == -1)
	{
		semctl(semid, 0, IPC_RMID, 0);
		syserr(P_NAME,"*init_sem* semget");
	}

//	INIZIALIZZO I SEMAFORI A 0
	st_sem.val = 0;
	for(i = 0 ; i < num_sem ; i++)
	    if (semctl(semid, i, SETVAL, st_sem) == -1)
	    {
	    	if (semctl(semid, 0, IPC_RMID, 0) == -1)
	    		 syserr(P_NAME,"semctl");

	    	syserr(P_NAME,"semctl");
	    }

	return semid;
}

void sem_set(int semid, int semnum, int val)
{
//	SETTO IL VALORE
	st_sem.val = val;

    if (semctl(semid, semnum, SETVAL, st_sem) == -1)
    {
    	if (semctl(semid, 0, IPC_RMID, 0) == -1)
    		syserr(P_NAME,"semctl");

    	syserr(P_NAME,"semctl");
    }
}

void sem_kill(int semid)
{
	if(semctl(semid, 0, IPC_RMID, 0) == -1)
		syserr(P_NAME,"semctl() fallita");
}
