/* echo_client.c
 *	+include proposta.h
 */

#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>
#include "proposta.h"
#define DIM 100

int main(int argc, char *argv[]){

	CLIENT *cl;
	ResultFileScan *f_ris;
    int * d_ris;
	char *server;
	char msg;
	char ok[5];
	int i=0,j=0;

	
	if (argc < 2) {
		fprintf(stderr, "uso: %s host\n", argv[0]);
		exit(1);
	}

	server = argv[1];

	cl = clnt_create(server, PROPOSTAPROG, PROPOSTAVERS, "udp");
	
	if (cl == NULL) {
		clnt_pcreateerror(server);
		exit(1);
	}


	/* CORPO DEL CLIENT:
	/* ciclo di accettazione di richieste da utente ------- */

	printf("Digitare F per file_scan, digitare D per dir_scan, EOF per terminare: ");

	while ((msg=getchar())!=EOF){
        
		if(msg=='F'){
			i++;
			
            char *nomefile;
			char temp[200];
			
			nomefile=(char*)malloc((sizeof(char))*50);
			
			getchar();
			
            printf("Inserire il nome del file (i=%d): ",i);
			
            if((gets(temp)) != NULL){
				
				strcpy(nomefile,temp);
				
                f_ris = file_scan_1(&nomefile, cl);
			
		        if (f_ris == NULL) {
			        fprintf(stderr, "%s: %s fallisce la rpc\n", argv[0], server);
			        clnt_perror(cl, server);
			        j++;
					if(j==5){
						printf("Ho provato 5 volte ma restituisce sempre NULL\n Ciao Ciao\n");
						exit(1);
					}
					
		        }else if(f_ris->numchar == -1){
					
						printf("Errore apertura file %s\n",nomefile);
						j=0;
					
					}else if(f_ris->numchar == -2){
					
						printf("Errore lettura file %s\n",nomefile);
						j=0;
					
					}else{
						j=0;
						printf("Operazione completata con successo -> \n Numero di caratteri: %d \n Numero di parole: %d \n Numero di righe: %d.\n", f_ris->numchar, f_ris->numword, f_ris->numline);
				}
			}
        }else if(msg=='D'){
			
				i++;
				
				argDir *argdir;
				char nomedir[100];
				int soglia;
				
				argdir=(argDir*)malloc(sizeof(argDir));
				
				printf("Inserire il nome della directory (i=%d): ",i);
				
				if((scanf("%s",argdir->nomedir))==1){
					
					printf("Nome dir :%s Inserire il valore di soglia: ",argdir->nomedir);
					
					if(scanf("%d", &argdir->soglia)==1){
						
						printf("nomedir:= %s e soglia:=%d\n", argdir->nomedir, argdir->soglia);
					
						d_ris = dir_scan_1(argdir, cl);
						
						if (d_ris == NULL) {
							fprintf(stderr, "%s: %s fallisce la rpc\n", argv[0], server);
							clnt_perror(cl, server);
							j++;
							if(j==5){
								printf("Ho provato 5 volte ma restituisce sempre NULL\n Ciao Ciao\n");
								exit(1);
							}
						}else {
							
							j=0;
							printf("Operazione completata con successo -> \n Numero file contenuti in %s con dimensione maggiore di %d: %d.\n", argdir->nomedir, argdir->soglia, *d_ris);
						}
					}
				}
			getchar();
			
			free(argdir);
			
        }
		if(i==0){
            printf("Codice inserito non corretto.\n"); 
		}   
		i=0;
        printf("Digitare F per file_scan, digitare D per dir_scan, EOF per terminare: ");
	} // while gets(msg)

	// Libero le risorse: memoria allocata con malloc e gestore di trasporto
	clnt_destroy(cl);
	printf("Termino...\n");
	exit(0);
}
