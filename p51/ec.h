#ifndef EC_H
#define EC_H

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

#define ENTROPY_POOL "/dev/urandom"

/* Estructura de contexto para variables temporales */
typedef struct {
    mpz_t temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, temp9;
} CTX;

typedef struct {
    mpz_t x, y, z;
} Point;

typedef struct {
    mpz_t a, b, P;
} EC;

typedef struct {
    mpz_t a, b, P, n; 
} ECDSA_Params;

typedef struct {
    mpz_t r, s;
} Signature;

/* Prototipos de Manejo de Memoria */
void init_ctx(CTX*);
void del_ctx(CTX*);
void init_point(Point*);
void del_point(Point*);
void init_ec(EC*);
void del_ec(EC*);
void init_signature(Signature*);
void del_signature(Signature*);

/* Operaciones de Curva */
void set_point(mpz_t, mpz_t, mpz_t, Point*);
void set_point_ui(unsigned int, unsigned int, unsigned int, Point*);
int is_infinite_point(Point*);
int is_on_curve(Point*, EC*, CTX*);
int is_singular_ec(EC, CTX*);

int add_point(Point*, Point*, Point*, EC*, CTX*);
int double_point(Point*, Point*, EC*, CTX*);
void scalar_multiply(Point*, Point*, mpz_t, EC*, CTX*);

/* Funciones ECDSA */
void generate_keypair(mpz_t, Point*, Point*, ECDSA_Params*, CTX*);
void ecdsa_sign(Signature*, mpz_t, mpz_t, Point*, ECDSA_Params*, CTX*);
int ecdsa_verify(Signature*, Point*, mpz_t, Point*, ECDSA_Params*, CTX*);

#endif
