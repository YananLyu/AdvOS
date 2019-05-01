/*
 * ldshr.x: remote database access protocol
 */
/* preprocessor directives */
%#define MACHINEFILE "machinefile"	/* '%' passes it through */

/* structure for FINDMAX_GPU */
struct max_para {
	int n;
	int mean;
	int seed;
};

/* structure for UPDATE_LST */
struct Node {
	double value;
	struct Node *next;
};


/* program definition no union or typdef definitions needed */
program RDBPROG { /* could manage multiple servers */
	version RDBVERS {
		double GET_LOAD() = 1;
		double FINDMAX_GPU(struct max_para) = 2;
		struct Node UPDATE_LST(struct Node) = 3;
	} = 1;
} = 0x20000001;	/* program number ranges estblished by ONC */
