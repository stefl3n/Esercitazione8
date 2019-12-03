* echo_client.c
 *	+include echo.h
 */

#include <stdio.h>
#include <rpc/rpc.h>
#include "proposta.h"
#define DIM 100

int main(int argc, char *argv[]){

	CLIENT *cl;
	char **echo_msg; // il risultato è un char*
    resultFileScan * f_ris;
    int * d_ris;
	char *server;
	char *msg;
	char ok[5];

	if (argc < 2) {
		fprintf(stderr, "uso: %s host\n", argv[0]);
		exit(1);
	}

	server = argv[1];

	cl = clnt_create(server, ECHOPROG, ECHOVERS, "udp");
	if (cl == NULL) {
		clnt_pcreateerror(server);
		exit(1);
	}


	/* CORPO DEL CLIENT:
	/* ciclo di accettazione di richieste da utente ------- */
	msg=(char*)malloc(DIM+1);

	printf("Digitare F per file_scan, digitare D per dir_scan, EOF per terminare: ");

	while (gets(msg))
	{
        if(msg[0]=='F')
        {
            char nomefile[100];
            printf("Inserire il nome del file: ");
            if(gets(nomefile))
            {
                f_ris = file_scan(&nomefile, cl);
		        if (f_ris == NULL) {
			        fprintf(stderr, "%s: %s fallisce la rpc\n", argv[0], server);
			        clnt_perror(cl, server);
			        exit(1);
		        }
                if (*f_ris == NULL) {
			        fprintf(stderr, "%s: %s restituisce una struttura nulla\n", argv[0], server);
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

                d_ris = dir_scan(&argdir, cl);
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
	free(msg);
	clnt_destroy(cl);
	printf("Termino...\n");
	exit(0);
}