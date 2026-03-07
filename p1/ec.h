/*
 * Autor: Yair Alejandro Erreguin Franco
 * Descripción: Práctica 1 de la materia Selected Topics in Cryptography.
 * Tema: Curvas Elípticas
 * Fecha: 17/02/2026
 * Punto al infinito: (0,1,0)
 * Punto cualquiera : (x,y,1)
 .:PROTOTIPOS Y ESTRUCTURAS:.
 * */

#ifndef EC_H
#define EC_H

#include <gmp.h>
#define ENTROPY_POOL "/dev/urandom"

/*
 * Estructura para los puntos de una Curva Elíptica
 * */
typedef struct
{
  mpz_t x;
  mpz_t y;
  mpz_t z;
}Point;

/*
 * Estructura para una Curva Elíptica
 * */
typedef struct
{
  mpz_t a;
  mpz_t b;
  mpz_t P;
}EC;

/*
 * Funciones para manipulación de puntos
 * */
void init_point(Point*);
void set_point(mpz_t, mpz_t, mpz_t, Point*);
void set_point_ui(unsigned int , unsigned int , unsigned int , Point*);
void print_point(Point );
int is_infinite_point(Point* );
int is_root_point(Point* );
int is_equal_point(Point*, Point* );
int is_inverse_point(Point*, Point*, EC* );
void del_point(Point* );

/*
 * Funciones de suma para puntos
 * */
int add_point(Point*, Point*, Point*, EC* );
int double_point(Point*, Point*,EC* );


/*
 * Funciones para manipulación de curvas
 * */
void init_ec(EC* );
int set_ec(mpz_t , mpz_t , mpz_t , EC* );
int set_ec_ui(unsigned int , unsigned int , unsigned int , EC* );
void print_ec(EC );
void del_ec(EC* );
int is_singular(EC );

void get_ecs_from_p(mpz_t );
void get_ec_from_p_bits(int );
void point_counting(mpz_t , mpz_t , mpz_t );

/*
 * Función para generar números primos de <b> bits
 * */
int gen_prime(mpz_t , gmp_randstate_t , unsigned int );
#endif
