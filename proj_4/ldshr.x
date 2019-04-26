/*
 * ldshr.x: remote database access protocol
 */

/* structure for FINDMAX_GPU */
struct max_para {
	int n;
	int mean;
	int seed;
}

/* structure for UPDATE_LST */
struct node {
	int num;
	struct node *next;
}

/* program definition no union or typdef definitions needed */
program RDBPROG { /* could manage multiple servers */
	version RDBVERS {
		string GET_LOAD(string) = 1;
		double FINDMAX_GPU(struct max_para) = 2;
		node * UPDATE_LST(struct node*) = 3;
	} = 1;
} = 0x20000001;	/* program number ranges estblished by ONC */
