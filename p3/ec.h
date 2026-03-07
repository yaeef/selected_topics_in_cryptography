/*
 * Autor: Yair Alejandro Erreguin Franco
 * Descripción: Práctica 2 STIC.
 * Tema: Curvas Elípticas
 * Fecha: 28/02/2026
 * Punto al infinito: (0,1,0)
 * Punto cualquiera : (x,y,1)
 .:PROTOTIPOS Y ESTRUCTURAS:.
 * */

#ifndef EC_H
#define EC_H

#include <gmp.h>
#define ENTROPY_POOL "/dev/urandom"

/*
 * Estructura para puntos
 * */
typedef struct
{
  mpz_t x,y,z;
}Point;

/*
 * Estructura para una Curva Elíptica
 * */
typedef struct
{
  mpz_t a,b,P;
}EC;

/*
 * Estructura de contexto para memoria dinámica
 * */
typedef struct
{
  Point p_ctx;
  EC ec_ctx;
  mpz_t num, den, inv_den, lambda, x3, y3;
}ctx;

/*
 * Funciones para puntos
 * */
void init_point(Point*);
void set_point(mpz_t, mpz_t, mpz_t, Point*);
void set_point_ui(unsigned int , unsigned int , unsigned int , Point*);
void print_point(Point*);
int is_infinite_point(Point*);
int is_root_point(Point*);
int is_equal_point(Point*, Point*);
int is_inverse_point(Point*, Point*, EC*);
void del_point(Point*);


/*
 * Funciones para curvas
 * */
void init_ec(EC*);
int set_ec(mpz_t , mpz_t , mpz_t , EC*);
int set_ec_ui(unsigned int , unsigned int , unsigned int , EC*);
void print_ec(EC);
int is_singular_ec(EC);
void del_ec(EC*);

/*
 * Función  que verifica si un punto pertenece a una curva
 * */
int is_on_curve(Point*, EC*);

/*
 * Operación de grupo
 * */

int add_point(Point*, Point*, Point*, EC*);
int double_point(Point*, Point*, EC*);
void scalar_multiply(Point* , Point* , mpz_t , EC*);


#endif
