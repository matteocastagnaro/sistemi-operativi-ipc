/**
 * @file shm.c
 * @author Matteo Castagnaro - VR360261
 * @brief File contenente tutte le funzioni relative alla gestione della memoria condivisa
 */

#include "utils.h"

int shm_creat(int size, int tok)
{
	static int shmid;

	if((shmid = shmget(ftok("/tmp", tok), size, IPC_CREAT | 0666)) < 0)
	{
		if (shmctl(shmid, IPC_RMID, (struct shmid_ds *) NULL) < 0)
			syserr(P_NAME,"shmctl");

		syserr(P_NAME,"shmget");
	}
	return shmid;
}

int *shm_attach(int shmid)
{
	int *shm;
	if((shm = (int *) shmat(shmid , 0 , 0)) == (int *) -1)
		syserr(P_NAME,"shmat");

	return shm;
}

void shm_detach(const void *shmaddr)
{
	if (shmdt(shmaddr))
		syserr(P_NAME,"Shmdt");
}

void shm_kill(int shmid)
{
	if (shmctl(shmid, IPC_RMID, (struct shmid_ds *) NULL) < 0)
		syserr(P_NAME,"shmctl");
}
