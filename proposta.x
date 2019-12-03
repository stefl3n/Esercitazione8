/* 
 * proposta.x
 *	+ definizioni strutture argDir e  resultFileScan 
 *  + definizioni metodi.
 * 	+ rpcgen proposta.x --> genera i seguenti file
 *		- proposta.h --> libreria da includere in proposta_proc.c e proposta_client.c.
 *		- proposta_xdr.c --> routine di conversione.
 * 		- proposta_clnt.c; proposta_svc.c --> stub C e S.
 */

struct argDir{
	char *nomedir;
	int soglia;
};

struct resultFileScan{
    int numchar;
    int numword;
    int numline;
}


program PROPOSTAPROG {
	version PROPOSTAVERS {
		resultFileScan FILE_SCAN(string) = 1;
		int DIR_SCAN(Operandi) = 2;
	} = 1;
} = 0x20000013;


