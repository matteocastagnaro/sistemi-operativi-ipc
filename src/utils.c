/**
 * @file utils.c
 * @author Matteo Castagnaro - VR360261
 * @brief File di libreria personale contenente funzioni utili.
 */

#include "utils.h"

char *itoa(int n)
{
	char *s=malloc(sizeof(char)*10);
	sprintf(s, "%d", n);
	return s;
}

void syserr(char *prog, char *msg)
{
	print("%s: fatal error: %s\n",prog,msg);
	exit(-1);
}

void print( const char *format, ... )
{
	va_list arg;
	char *string = malloc( sizeof(char) * SIZE );
	// memset() NECESSARIA PER PULIRE LA STRINGA IN MANIERA DEFINITIVA
	memset(string,0,SIZE*sizeof(char));
	va_start(arg, format);
	vsprintf(string, format, arg);
	write(STDOUT,string,strlen(string));
	va_end(arg);
	free(string);
}

int num_elements(char *dirname)
{
	int n=0;
	DIR *d;
	struct dirent *dentry;

	if( (d=opendir(dirname)) == 0 )
		return -1;

	 while( (dentry = readdir(d)) )
		 if(dentry->d_name[0]=='c' && dentry->d_name[1]=='p' && dentry->d_name[2]=='u' && dentry->d_name[3]=='_')
			 n++;

	if(closedir(d)<0)
		syserr(P_NAME,"closedir() fallita");

	return n;
}

int ctoi(char *car)
{
	switch(*car)
	{
	case 'W': return 0;break;
	case 'S': return 1;break;
	case 'R': return 2;break;
	default: return atoi(car);
	}
}

char itoc(int op)
{
	switch(op)
	{
	case 0: return 'W';
	case 1: return 'S';
	case 2: return 'R';
	default: return 'e';
	}
}

coda *read_file(char *file)
{
// 	VETTORE CHE CONTIENE UNA RIGA DEL FILE
	char s[128];
// 	BUFFER PER LEGGERE UN SOLO CARATTERE ALLA VOLTA
	char c;
// 	INDICE PER IL VETTORE DI CARATTERI S
	int i;
// 	FILE DESCRIPTOR
	int fd_from;
// 	NUMERO DI CARATTERI LETTI
	int nread;
// 	POSIZIONE ALL'INTERNO DEL FILE
	long where;
// 	CONTATORE PER SALTARE L'ULTIMA RIGA
	int cont=0;
//	ARRAY DI SUPPORTO PER LEGGERE IL FILE
	char tmp[100];
// 	ARRAY PER I DATI DELLA RIGA
	int data[3];
// 	INDICE PER L'ARRAY DEI DATI
	int pos=0;
// 	CODA DOVE SALVARE I COMANDI
	coda *coda = NULL;

//	APRO IL FILE
	if ((fd_from = open(file, O_RDONLY)) == -1)
		syserr(P_NAME,"open() fallita");

// 	SPOSTO IL PUNTATORE DEL FILE ALLA FINE
	if ((where = lseek(fd_from, -1, SEEK_END)) == -1)
		syserr(P_NAME,"lseek() fallita");

	i = sizeof(s) - 1;
	s[i] = '\0';

	while ((nread = read(fd_from, &c, sizeof(char))) == 1)
	{

//		SE RAGGIUNGO LA FINE DELLA RIGA
		if (c == '\n')
		{
			if ( cont > 0)
			{
				sprintf(tmp, "%s", &s[i]); /* stampo la stringa s a partire dall'elemento i */
				char *token = strtok(tmp, " ");
				data[2]=-1;

					while(token)
					{
							data[pos]=ctoi(token);
							token = strtok(NULL, " ");
							pos++;
					}

//					RIPORTO ALL'INIZIO L'INDICE PER I DATI
					pos=0;

// 					COSTRUISCO LA LISTA DEI COMANDI
					coda = construct_coda(itoc(data[0]), data[1], data[2], coda);

			}
// 			INCREMENTO IL CONTATORE
			cont++;

// 			RIPORTO L'INDICE DEL VETTORE S ALL'ULTIMO VALORE
			i = sizeof(s) - 1;
		}
// 		SE L'INDICE I DIVENTA 0 ALLORA HO FINITO LO SPAZIO SUL VETTORE S
		if (i == 0)
			syserr(P_NAME,"riga troppo lunga");

//		MEMORIZZO IL CARATTERE LETTO DOPO AVER DECREMENTATO L'INDICE I.
//		I CARATTERI VENGONO MEMORIZZATI DALLA FINE PER PRESERVARE L'ORDINE DELLA RIGA
		s[--i] = c;

// 		SE LSEEK RITORNA 0 SIGNIFICA CHE SONO ARRIVATO ALL'INIZIO DEL FILE E QUINDI ESCO DAL WHILE
		if (where == 0)
			break;

//		DOPO AVER LETTO UN CARATTERE DEVO RISALIRE DI 2 POSIZIONI, IL CARATTERE CHE HO APPENA LETTO
//		E QUELLO CHE VOGLIO LEGGERE
		if ((where = lseek(fd_from, -2L, SEEK_CUR)) == -1)
			syserr(P_NAME,"lseek() fallita");
	}

// 	RIPORTO IL PUNTATORE ALL'INIZIO DEL FILE PER LEGGERE LA PRIMA RIGA
	if ((where = lseek(fd_from, 0, SEEK_SET)) == -1)
			syserr(P_NAME,"lseek() fallita");

//	VADO A LEGGERE LA PRIMA RIGA DEL FILE
	while ( (nread = read(fd_from, &c, sizeof(char))) == 1)
	{
//		APPENA INCONTRA IL CARATTERE \n ESCE DAL CICLO
		if(c=='\n')
			break;
		sprintf(tmp, "%s", &s[i]);
	}

//	DIVIDO LA STRINGA TROVATA IN TOKEN
	char *r = strtok(tmp, " ");
//	METTO IN POSIZIONE 2 IL VALORE -1. LA POSIZIONE 2 INDICA pos
	data[2] = -1;

	while(r)
	{
		data[pos]=ctoi(r);
		r = strtok(NULL, " ");
		pos++;
	}

//	COSTRUISCO LA LISTA DELLE OPERAZIONI
	coda = construct_coda(itoc(data[0]),data[1],data[2],coda);

	// CHIUDO IL FILE
	if (close(fd_from) == -1)
		syserr(P_NAME,"close");

	return coda;
}

int length_coda(coda *this)
{
	return this ? (1 + length_coda(this->next)) : 0;
}

coda *construct_coda(char c, int value, int pos, coda *next)
{
  coda *this = malloc(sizeof(coda));

  this->sig  = c;
  this->num  = value;
  this->pos  = pos;
  this->next = next;

  return this;
}

coda *coda_creat(int n)
{
	char *s = malloc(sizeof(char)*20);
	sprintf(s,"conf/cpu_%d.txt",n);
	coda *c = read_file(s);
	free(s);
	return c;
}

