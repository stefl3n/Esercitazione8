/* proposta_proc.c
 * 	+implementazione della procedura remota "proposta".
 * 	+include proposta.h.
 */

#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <rpc/rpc.h>
#include "proposta.h"

ResultFileScan * file_scan_1_svc(char **nomefile, struct svc_req *rp){
	
	ResultFileScan* result = (ResultFileScan*) malloc(sizeof(ResultFileScan));
	int fd, nchar=0, words=0, line=0, check;
	char c, c1;
	
	if((fd=open(*nomefile,O_RDONLY))<0){
		perror("Errore apertura file :");
		result->numchar=-1;
		result->numword=-1;
		result->numline=-1;
		return result;
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
		words++;
		line++;
	}
	
	if(check<0){
		perror("Errore lettura file:");
		result->numchar=-2;
		result->numword=-2;
		result->numline=-2;
		return result;
	}
	
	close(fd);
	
	result->numchar=nchar;
	result->numword=words;
	result->numline=line;
	return result;
}

int * dir_scan_1_svc(argDir *argdir, struct svc_req *rp)
{
	DIR *dir;  struct dirent * dd;
	static int count = 0;
	int temp=0;
	int size;
	char nomedir[256], path[256];
	struct stat* stato;
	
	if((getcwd(path,256)) == NULL){
			perror("Errore nella getcwd :");
			exit(1);
	}
	
	dprintf(2,"contenurto di argdir nomedir: %s     soglia : %d",argdir->nomedir,argdir->soglia);
	
	if((dir = opendir(argdir->nomedir))==NULL){
		
		printf("Errore apertura direttorio: %s ", argdir->nomedir);
		perror("Errore :");
		count = -1;
		return (&count);
	}
	
	stato=(struct stat*)malloc(sizeof(struct stat));
	
	while ((dd = readdir(dir)) != NULL){
		
		chdir(argdir->nomedir);
		
		if(dd->d_name[0]!='.'){
		    printf("Trovato file %s\n", dd->d_name);
			
			if((stat(dd->d_name,stato))==0){
				printf("Il file %s ha dim : %jd\n",dd->d_name,stato->st_size);
		
				if((long)stato->st_size > argdir->soglia)
					temp++;
			
			}else
				perror("Errore apertura stat di un file:");
		}
	}
	chdir(path);
	closedir (dir);
	free(stato);
	count=temp;
	return (&count);
}


