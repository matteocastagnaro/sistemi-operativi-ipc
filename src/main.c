/**
 * @file main.c
 * @author Matteo Castagnaro - VR360261
 * @brief File principale del progetto.
 */

#include "utils.h"

/**
 * @file main.c
 * @author Matteo Castagnaro - VR360261
 * @mainpage
 * @author Matteo Castagnaro - VR360261
 * @date 20 Maggio 2014
 *
 * <hr>
 * @section intro_sec TESTO ELABORATO
 * Si vuole realizzare un programma in C che utilizzi le system call (IPC), ove possibile, per
 * implementare lo scheletro di un simulatore multiprocessore con memoria RAM. Il progetto deve
 * essere commentato in formato Doxygen, e corredato da uno script configurazione per tale tool e da
 * uno script Makefile per la sua compilazione. Inoltre si devono allegare al progetto anche eventuali file
 * di supporto.\n\n
 * I file di traccia avranno la seguente struttura:
 * 	-# <b>S</b> <num>: indica una operazione di sleep pari a <i>num</i> secondi. Serve per simulare una
 * 	computazione della durata di <i>num</i> secondi.
 * 	-# <b>W</b> <num> <pos>: indica una operazione di scrittura in memoria RAM, del valore intero <i>num</i>,
 * 	all'indirizzo <i>pos</i>.
 * 	-# <b>R</b> <pos>: indica una operazione di lettura di numero intero dalla memoria RAM, all'indirizzo
 * 	<i>pos</i>.
 *
 *	Ad esempio, alcuni file di configurazione potrebbero essere:
 * 	<table align="center" border=3 width="80%">
 * 		<tr>
 *			<td align="center">FILE 1</td>
 *			<td align="center">FILE 2</td>
 *			<td align="center">FILE 3</td>
 *			<td align="center">FILE 4</td>
 * 		</tr>
 * 		<tr>
 *			<td>S 1<br>W 2 4<br>S 3<br>R 7</td>
 *			<td>S 2<br>R 4<br>S 1<br>W 2 7</td>
 *			<td>W 1 2<br>R 2<br>S 1<br>R 1</td>
 *			<td>W 3 2<br>W 20 1<br>R 2<br>S 2</td>
 * 		</tr>
 * 	</table>\n
 *
 *	@subsection funz Funzionamento del programma
 *	Il programma prenderà due interi dall'utente, <i>numCpu</i> e <i>ramDim</i>. Gli interi possono essere passati da
 *	linea di comando oppure in modo interattivo.\n
 *	Il programma crea quindi:
 *	 -# Tutte le eventuali risorse che saranno necessarie per la simulazione (es. semafori e memoria
 *	 condivisa).
 *	 -# Il processo che simula la RAM.
 *	 -# Un numero di processi pari a <i>numCpu</i> per simulare i processori.
 *	 Attende quindi la fine della simulazione aspettando la terminazione dei processi che simulano i
 *	 processori, fa terminare il processo che simula la RAM, libera le risorse allocate, e termina.
 *
 *	 Il processo che simula la RAM eseguirà in questo modo:
 *	  -# Alloca un array di interi, pari a <i>ramDim</i>, per simulare la RAM, e lo inizializza a zero.
 *	  -# Entra in un loop:
 *	   -# Attende che arrivi una richiesta. Tutte le richieste saranno passate tramite memoria
 *	   condivisa.
 *	   -# Se la richiesta è di tipo W (scrittura), scrive il valore <i>num</i> all'interno della cella <i>pos</i>
 *	   dell'array precedentemente allocato.
 *	   -# Se la richiesta è di tipo R (lettura), legge la cella <i>pos</i> dell'array e ne scrive il valore in
 *	   memoria condivisa.
 *
 *	 Ogni processo che simula un processore eseguirà in questo modo:
 *	  -# Legge il file di traccia che deve simulare. Il file sarà nominato <i>cpu_<id>.txt</i>, dove <i>id</i> è il
 *	  numero del processore considerato (a partire da 1).
 *	  -# Entra in un loop per simulare la traccia:
 *	   -# Se l'operazione è di tipo S (sleep), si addormenta tramite una sleep per un numero di
 *	   secondi pari a <i>num</i>.
 *	   -# Se l'operazione è di tipo W (scrittura), scrive in memoria condivisa la richiesta, e segnala
 *	   al processo che simula la RAM che una nuova richiesta è disponibile.
 *	   -# Se l'operazione è di tipo R (lettura), scrive la richiesta in memoria condivisa, segnala al
 *	   processo che simula la RAM che una nuova richiesta è disponibile, ed attende la risposta
 *	   (cioè attende il valore che ha richiesto di leggere).
 *	  -# Quando ha finito di simulare tutta la traccia, termina.
 *
 *	 Quando i processi devono attendere, non devono fare attese attive: si devono bloccare su semafori.
 *	 Allo stesso modo, la “segnalazione” di una richiesta o che un dato e' pronto va sempre fatta tramite
 *	 semafori.\n\n
 *	 Si aggiungano ai processi delle stampe a video per poter seguire l'esecuzione delle tracce.\n\n
 *	 Per ogni chiamata ad una system call, si deve controllare che tale funzione abbia successo.
 *	 Ove possibile, si devono usare le system call al posto delle equivalenti chiamate a funzioni di libreria.
 *	 Tutte le stampe a video, le letture e le scritture su file devono avvenire tramite system call
 *	 (quindi ad esempio non si possono utilizzare <i>printf</i>, <i>fprintf</i>, <i>scanf</i>, <i>fscanf</i>, <i>perror</i>).
*/
int main(int argc, char **argv){

	int numCpu,ramDim,shmid,semid,i;

//	SE IL NUMERO DI ELEMENTI PASSATI DA RIGA DI COMANDO NON È CORRETTO, CHIAMO L'ERRORE
//	DI SISTEMA CON LA syserr
	if(argc!=3)
		syserr(P_NAME,"Insert 2 parameters!");

//	SALVO NELLE VARIABILI numCpu E ramDim IL NUMERO DI CPU E LA DIMENSIONE DELLA RAM
//	PASSATA DA RIGA DI COMANDO
	numCpu=atoi(argv[1]);
	ramDim=atoi(argv[2]);

	print("\nNumero di CPU:\t\t%d\nDimensione della RAM:\t%d\n\n",numCpu,ramDim);

// 	pid[0] 	-> PID del processo che simula la RAM
// 	pid[da 1 a numCpu] -> PID dei processi che simulano le cpu
	int pid[numCpu+1];

	if(numCpu!=num_elements("conf/"))
	{
		char *s=malloc(sizeof(char)*30);
		sprintf(s,"le CPU da gestire sono %d!",num_elements("conf/"));
		syserr(P_NAME,s);
	}

// 	CREO LA MEMORIA CONDIVISA
	shmid = shm_creat(ramDim,'a');

//	COLLEGO IL MAIN ALLA MEMORIA CONDIVISA
	shms *shmptr = (shms*)shm_attach(shmid);
//	INIZIALIZZO IL COUNTER DELLA STRUTTURA DELLA MEMORIA CONDIVISA A 0
	shmptr->counter = 0;

//	CREO L'ARRAY DI SEMAFORI.
//	1	->	MAIN
//	2	->	RAM
//	3	->	SEZIONE CRITICA
//	4-n	->	PROCESSI DELLE CPU
	semid = sem_creat(numCpu+4,'b');

	if((pid[0]=fork())<0)
		syserr(P_NAME,"fork() fallita");

//	GENERO IL PROCESSO RELATIVO ALLA RAM
	if(pid[0]==0)
	{
		char *arg[]={"./ram",itoa(ramDim),itoa(shmid),itoa(semid),itoa(numCpu),NULL};
		execv(arg[0],arg);
//		SE NON VA A BUON FINE LA execv, VIENE CHIAMATO L'ERRORE DI SISTEMA CON syserr
		syserr(P_NAME,"errore nella exec()");
	}

//	GENERO numCpu PROCESSI RELATIVI ALLE CPU
	for(i=1;i<=numCpu;i++)
	{
		if((pid[i]=fork())<0)
			syserr(P_NAME,"fork() processi fallita");
		if(pid[i]==0)
		{
			char *arg[]={"./cpu",itoa(i),itoa(shmid),itoa(semid),itoa(ramDim),NULL};
			execv(arg[0],arg);
//			SE NON VA A BUON FINE LA execv, VIENE CHIAMATO L'ERRORE DI SISTEMA CON syserr
			syserr(P_NAME,"errore nella exec()");
		}
	}

//	SETTO IL VALORE DEL SEMAFORO RELATIVO ALLA SEZIONE CRITICA A 1
	sem_set(semid,SEM_SEZCR,1);

//	SEGNALI AI SEMAFORI DELLE CPU
	for(i=4;i<numCpu+4;i++)
		sem_signal(i,semid);

//	ATTENDO CHE TERMININO TUTTI I PROCESSI FIGLI
	sem_wait(SEM_MAIN,semid);

//	SCOLLEGO IL MAIN DALLA MEMORIA CONDIVISA
	shm_detach(shmptr);

//	CANCELLO L'ARRAY DI SEMAFORI E LA MEMORIA CONDIVISA. TERMINO IL PROGRAMMA
	sem_kill(semid);
	shm_kill(shmid);

	print("\nEliminati semafori e memoria condivisa\n\n");

	return 0;
}
