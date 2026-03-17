/* Definición de prototipos y estructuras 
 * para desarrollar la práctica 4 de STIC
 * */

#ifndef DSA_H
#define DSA_H
#include <stdint.h>
#include <sys/random.h>
#include <stdio.h>
#include "field.h"


/*Estructuras para instanciar llaves, firma y mensaje*/
typedef struct
{
  uint64_t p,q,g,B;
}pub_k;

typedef struct
{
  uint64_t d;
}priv_k;

typedef struct
{
  uint64_t r,s;
}signature;

typedef struct
{
  uint64_t m;
}message;

/*Función para generar un número aleatorio usando la entropia de urandom */
uint64_t gen_random_number(void);

/*Función exhaustiva para verificar primalidad O(n) = sqrt(n)*/
int is_prime(uint64_t);

/*Función para obtener el primo 11<=q<=1024. el primer primo es 3 y el último el 1021 */
uint64_t gen_q_prime(void);

/*Funcion para generar numero 1<=h<=p-1*/
uint64_t gen_h_number(uint64_t);

/*Función para generar clave privada*/
uint64_t gen_d_number(uint64_t);

/*Funcion para obtener el primo p=k*q+1*/
uint64_t gen_p_prime(uint64_t);

/*Función para buscar los primos p y q*/
int gen_pq(pub_k*);

/*Funcion para obtener el generador del subgrupo q*/
int gen_g(pub_k*);

/*Funcion para generar el par de llaves dsa*/
int gen_keys(pub_k*, priv_k*);

/*Fución para generar una llave efimera 0<=e_k<=q*/
uint64_t gen_ephimeral_key(uint64_t);

/*Funcion para generar la firma (r,s)*/
int gen_signature(signature*, pub_k*, priv_k*, uint64_t);

/*Funcion que verifica una firma dado un mensaje*/
int verif_signature(pub_k*, signature*, uint64_t );
#endif // !DSA_H
