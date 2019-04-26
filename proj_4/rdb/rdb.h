/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RDB_H_RPCGEN
#define _RDB_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define DATABASE "personnel.dat" /* '%' passes it through */
#define MAX_STR 256

struct record {
	char *firstName;
	char *middleInitial;
	char *lastName;
	int phone;
	char *location;
};
typedef struct record record;

#define RDBPROG 0x20000001
#define RDBVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define FIRSTNAME_KEY 1
extern  record * firstname_key_1(char **, CLIENT *);
extern  record * firstname_key_1_svc(char **, struct svc_req *);
#define LASTNAME_KEY 2
extern  record * lastname_key_1(char **, CLIENT *);
extern  record * lastname_key_1_svc(char **, struct svc_req *);
#define PHONE_KEY 3
extern  record * phone_key_1(int *, CLIENT *);
extern  record * phone_key_1_svc(int *, struct svc_req *);
#define LOCATION_KEY 4
extern  record * location_key_1(char **, CLIENT *);
extern  record * location_key_1_svc(char **, struct svc_req *);
#define ADD_RECORD 5
extern  int * add_record_1(record *, CLIENT *);
extern  int * add_record_1_svc(record *, struct svc_req *);
extern int rdbprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define FIRSTNAME_KEY 1
extern  record * firstname_key_1();
extern  record * firstname_key_1_svc();
#define LASTNAME_KEY 2
extern  record * lastname_key_1();
extern  record * lastname_key_1_svc();
#define PHONE_KEY 3
extern  record * phone_key_1();
extern  record * phone_key_1_svc();
#define LOCATION_KEY 4
extern  record * location_key_1();
extern  record * location_key_1_svc();
#define ADD_RECORD 5
extern  int * add_record_1();
extern  int * add_record_1_svc();
extern int rdbprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_record (XDR *, record*);

#else /* K&R C */
extern bool_t xdr_record ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RDB_H_RPCGEN */
