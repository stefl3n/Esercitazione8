
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <rpc/rpc.h>
#include "proposta.h"
#define DIM 100
#define PROPOSTAPROG 0x20000013
#define PROPOSTAVERS 1
int main(int argc, char *argv[]){

	CLIENT *cl;
	char **echo_msg; // il risultato è un char*
    resultFileScan * f_ris;
    int * d_ris;
	char *server, *nomefile;
	char *msg;
	char ok[5];

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

	while (scanf("%s",msg)!=EOF)
	{
        if(msg[0]=='F')
            printf("Inserire il nome del file: ");
            if(scanf("%s",nomefile)!=1)
            {
                f_ris = file_scan_1(&nomefile, cl);
		        if (f_ris == NULL) {
			        fprintf(stderr, "%s: %s fallisce la rpc\n", argv[0], server);
			        clnt_perror(cl, server);
			        exit(1);
		        }
		        //TODO logica ancora da implementare.
                if (f_ris->numchar==-1) {
			        fprintf(stderr, "%s: %s file vuoto\n", argv[0], server);
		        }
		        else if(f_ris->numchar==-2){
                    fprintf(stderr, "%s: %s file non trovato\n", argv[0], server);
                }
		        else
                    printf("Operazione completata con successo -> \n Numero di caratteri: %d \n Numero di parole: %d \n Numero di righe: %d.\n", f_ris->numchar, f_ris->numword, f_ris->numline);
            }
        }else if(msg[0]=='D')
        {
            char nomedir[100];
            int soglia;
            printf("Inserire il nome della directory: ");
            if(gets(nomedir))
            {
                printf("Inserire il valore di soglia: ");
                if(scanf("%d", soglia) > 0)
                argDir argdir;
                strcpy(argDir->nomedir, nomedir);
                argDir->soglia = soglia;

                d_ris = dir_scan_1(&argdir, cl);
		        if (d_ris == NULL) {
			        fprintf(stderr, "%s: %s fallisce la rpc\n", argv[0], server);
			        clnt_perror(cl, server);
			        exit(1);
		        }
                if (*d_ris == NULL) {
			        fprintf(stderr, "%s: %s restituisce intero nullo\n", argv[0], server);
		        }
		        else
                    printf("Operazione completata con successo -> \n Numero file contenuti in %s con dimensione maggiore di %d: %d.\n", nomedir, soglia, d_ris);
            }
        }else
        {
            printf("Codice inserito non corretto.\n");    
        }
        printf("Digitare F per file_scan, digitare D per dir_scan, EOF per terminare: ");
	} // while gets(msg)

	// Libero le risorse: memoria allocata con malloc e gestore di trasporto
	clnt_destroy(cl);
	printf("Termino...\n");
	exit(0);
}
