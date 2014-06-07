/**
 * @file utils.h
 * @author Matteo Castagnaro - VR360261
 * @brief File dove vengono dichiarate le funzioni, incluse le librerie e definite le costanti globali.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <stddef.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "sem.h"
#include "shm.h"

#define STDIN	0
#define STDOUT 	1
#define P_NAME 	"prog"
#define BUFLEN 	512
#define SIZE 	1024

#define SEM_MAIN	1
#define SEM_RAM		2
#define SEM_SEZCR	3

#define TRUE 1
#define FALSE !TRUE

/**
 * @struct coda
 * @brief Struttura per la lettura dei file di configurazione.
 */
struct coda{
	///Carattere che indica il comando da eseguire
	char sig;
	int num;
	int pos;
	///Coda della lista
	struct coda *next;
};
typedef struct coda coda;

/**
 * @struct shm_struct
 * @brief Struttura per la scrittura sulla memoria condivisa
 */
struct shm_struct{
	///ID del semaforo
	int sem_id;
	///Carattere che indica il comando da eseguire
	char sig;
	int num;
	int pos;
	int read;
	///Contatore per segnalare alla RAM che tutte le CPU hanno terminato il proprio lavoro
	int counter;
};
typedef struct shm_struct shms;
/**
 * @fn void syserr(char *prog, char *msg)
 * @brief Stampa a video l'errore di sistema. Utilizza la funzione <i><b>print()</b></i>
 * @param prog - Nome del programma
 * @param msg - Messaggio di errore
 * @return void
 */
void syserr(char *prog, char *msg);
/**
 * @fn char *itoa(int n)
 * @brief Converte il numero passato in array di caratteri (stringa).
 * @param n - Numero passato da convertire in stringa
 * @return char*
 */
char *itoa(int n);
/**
 * @fn void print(const char *format, ... )
 * @brief Stampa a video la stringa in input. Utilizza la system call <i><b>write()</b></i>
 * @param format - Stringa da stampare su STDOUT.
 * @return void
 */
void print( const char *format, ... );
/**
 * @fn int num_elements(char *dirname)
 * @brief Ritorna un intero contenente il numero di elementi della directory <i>dirname</i>. Considera soltanto i file che iniziano con <i>cpu_</i>
 * @param dirname - Nome della directory di cui contare gli elementi
 * @return int
 */
int num_elements(char *dirname);
/**
 * @fn int ctoi(char *car)
 * @brief Funzione che converte il carattere passato in input in intero
 * @param c - Carattere da convertire in intero
 * @return int
 */
int ctoi(char *car);
/**
 * @fn char itoc(int op)
 * @brief Funzione che converte un intero in carattere. Opposta alla char_to_int()
 * @param op - Intero da convertire in carattere
 * @return int
 */
char itoc(int op);
/**
 * @fn coda *read_file(char *file)
 * @brief Funzione necessaria per il recupero degli elementi scritti nei file di configurazione
 * @param *file - Nome del file di cui leggere il contenuto
 * @return coda*
 */
coda *read_file(char *file);
/**
 * @fn coda *construct_coda(char c, int value, int pos, coda *next)
 * @brief Funzione che costruisce la lista generata dalla funzione read_file()
 * @param c - Comando da eseguire
 * @param value - Valore da considerare
 * @param pos - Posizione su cui scrivere <i>value</i>. Valido solo per il comando <i>W</i>
 * @param *next - Coda della lista
 * @return *coda
 */
coda *construct_coda(char c, int value, int pos, coda *next);
/**
 * @fn int length_coda(coda *this)
 * @brief Funzione che ritorna la lunghezza della lista
 * @param *this - Lista di cui contarne la lunghezza
 * @return int
 */
int length_coda(coda *this);
/**
 * @fn coda *coda_creat(int n)
 * @brief Funzione che crea la lista dal numero del processo
 * @param n - Numero del processo
 * @return coda*
 */
coda *coda_creat(int n);
