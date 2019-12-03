#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/select.h>
#include <dirent.h>
#include <rpc/rpc.h>
#include "proposta.h"
//IMPLEMENTAZIONE FUNZIONI SERVER


// Questo è il tipo che deve restituire la funzione file_scan richiesto dalle specifiche



/* Questa funzione prende in ingresso il nome di un file presente nella cartella del server
 * conta il numero di caratteri,parole,righe e restituisce una struttura ResultFileScan.
 * in caso di errore in apertura di file ritorna una struttura con tutti i parametri=-1
 * in caso di errore in lettura ritorna una struttura con tutti i parametri=-2
 * */
resultFileScan* file_scan_1_svc(char **nomefile , struct svc_req *rp){
	static resultFileScan result; //(ResultFileScan*) malloc(sizeof(ResultFileScan));
	int fd, nchar=0, words=0, line=0, check;
	char c, c1;
	if((fd=open(*nomefile,O_RDONLY))<0){
		perror("Errore apertura file :");
		return NULL;
	}
	while((check=read(fd,&c,sizeof(char)))>0){
		c1=c;
		nchar++;
		if(c==' ' || c==',' || c=='.'){
			words++;
		}
		if(c=='\n'){
			words++;
			line++;
		}
	}
	if(check==0 && c1!='\n'){
		line++;
	}
	if(check<0){
		perror("Errore lettura file:");
		return NULL;
	}
	close(fd);
	result.numchar=nchar;
	result.numword=words;
	result.numline=line;
	return (&result);
}

int* dir_scan_1_svc (argDir *argdir, struct svc_req *rp){
	DIR *dir;  struct dirent * dd;
	static int count = 0;
    static int error=-1;
	int size;
	struct stat* stato;
	if((dir = opendir(argdir->nomedir))==NULL){
		perror("Errore apertura direttorio:");
		return &error;
	}
	stato=(struct stat*)malloc(sizeof(struct stat));
	while ((dd = readdir(dir)) != NULL){
		chdir(argdir->nomedir);
		
		if(dd->d_name[0]!='.'){
		
		printf("Trovato file %s\n", dd->d_name);
		
			
			if((stat(dd->d_name,stato))==0){
				
				//size=(int)stato->st_size;
				
				printf("Il file %s ha dim : %jd\n",dd->d_name,stato->st_size);
				
				if((long)stato->st_size>argdir->soglia)
					count++;
			}else
				perror("Errore apertura stat di un file:");

		}
	}
	closedir (dir);
	free(stato);
	return (&count);
}
