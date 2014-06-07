/**
 *@file sem.h
 *@author Matteo Castagnaro - VR360261
 */
#ifndef SEM_H_
#define SEM_H_

/**
 * @union semun
 * @brief Struttura per il semaforo
 */
union semun {
	///Usato per IPC_STAT e IPC_SET
	int val;
	/// IPC_INFO
	struct semid_ds *buf;
	/// Usato per GET_ALL E SET_ALL
	ushort *array;
} st_sem; /**< union variable*/
/**
 * @fn void sem_wait (int sem_number, int semid)
 * @brief Esegue una wait sul semaforo specificato
 * @param sem_number - Numero del semaforo
 * @param semid - ID del semaforo
 * @return void
 */
void sem_wait (int sem_number, int semid);
/**
 * @fn void sem_signal (int sem_number, int semid)
 * @brief Esegue una signal sul semaforo specificato
 * @param sem_number - Numero del semaforo
 * @param semid - ID del semaforo
 * @return void
 */
void sem_signal (int sem_number, int semid);
/**
 * @fn int sem_creat(int num_sem, char key)
 * @brief Crea un semaforo e ritorna l'id del semaforo creato
 * @param num_sem - Lunghezza dell'array di semafori
 * @param key - Chiave per il semaforo
 * @return int
 */
int sem_creat(int num_sem, char key);
/**
 * @fn void sem_set(int semid, int semnum, int val)
 * @brief Setta il semaforo specificato con il valore specificato in val
 * @param semid - ID del semaforo
 * @param semnum - Numero del semaforo
 * @param val - Valore che si vuole settare
 * @return void
 */
void sem_set(int semid, int semnum, int val);
/**
 * @fn void sem_kill(int semid)
 * @brief Deallocazione del semaforo
 * @param semid - ID del semaforo da deallocare
 * @return void
 */
void sem_kill(int semid);

#endif
