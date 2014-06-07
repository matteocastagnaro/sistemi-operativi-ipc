/**
 * @file shm.h
 * @author Matteo Castagnaro - VR360261
 * @brief Header file relativo alla gestione della memoria condivisa
 */


/**
 * @fn int shm_creat(int size, int tok)
 * @brief Funzione che crea la memoria condivisa
 * @param size - Dimensione della memoria condivisa
 * @param tok - Valore casuale
 * @return int
 */
int shm_creat(int size, int tok);
/**
 * @fn int *shm_attach(int shmid)
 * @brief Funzione che collega un processo alla memoria condivisa
 * @param shmid - ID della memoria condivisa
 * @return int*
 */
int *shm_attach(int shmid);
/**
 * @fn void shm_detach(const void *shmaddr)
 * @brief Funzione che scollega il processo dalla memoria condivisa
 * @param shmadd - Indirizzo della memoria condivisa
 * @return void
 */
void shm_detach(const void *shmaddr);
/**
 * @fn void shm_kill(int shmid)
 * @brief Funzione che dealloca la memoria condivisa
 * @param shmid - ID della memoria condivisa
 * @return void
 */
void shm_kill(int shmid);
