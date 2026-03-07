/*
 * Autor: Yair Alejandro Erreguin Franco
 * Descripción: Práctica 1 de la materia Selected Topics in Cryptography.
 * Tema: Curvas Elípticas
 * Fecha: 17/02/2026
 * Punto al infinito: (0,1,0)
 * Punto cualquiera : (x,y,1)
 * */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ec.h"

void excercise_one(int P)
{
  mpz_t prime; mpz_init(prime); mpz_set_ui(prime, P);
  get_ecs_from_p(prime);
  mpz_clear(prime);
}

void excercise_two(int bits)
{
  get_ec_from_p_bits(bits);
}

void excercise_three(int aa, int ba, unsigned int Pa)
{
  mpz_t a, b, P; mpz_inits(a, b, P, NULL);
  mpz_set_si(a, aa); mpz_set_si(b, ba); mpz_set_ui(P ,Pa);
  point_counting(a, b, P);
  mpz_clears(a, b, P, NULL);
}

void excercise_four(int x1, int y1, int z1, int x2, int y2, int z2, int a, int b, int Pa)
{
  EC E; init_ec(&E); set_ec_ui(a, b, Pa, &E);
  Point P,Q,R; init_point(&P); init_point(&Q); init_point(&R); set_point_ui(x1, y1, z1, &P); set_point_ui(x2, y2, z2, &Q);
  if(add_point(&R, &P, &Q, &E))
    print_point(R);
  else
    fprintf(stderr, "Error: La suma no se completó :( \n");
  del_point(&P); del_point(&R);
  del_ec(&E);
}

void excercise_five(int x, int y, int z, int a, int b, int Pa)
{
  EC E; init_ec(&E); set_ec_ui(a, b, Pa, &E);
  Point P,R; init_point(&P); init_point(&R); set_point_ui(x, y, z, &P);
  if(double_point(&R, &P, &E))
    print_point(R);
  else
    fprintf(stderr, "Error: EL punto no se duplico :( \n");
  del_point(&P); del_point(&R);
  del_ec(&E);
}

int main(int argc, char **argv)
{
  int opt;
  int mode=0;
  int a=0,b=0,P=0, bits=0, x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;

  /*Parseo de CLI :p*/
  while((opt = getopt(argc, argv, "12345a:b:p:n:x:y:z:X:Y:Z:")) != -1)
  {
    switch(opt)
    {
      case '1':
        mode = 1;
        break;
      case '2':
        mode = 2;
        break;
      case '3':
        mode = 3;
        break;
      case '4':
        mode = 4;
        break;
      case '5':
        mode = 5;
        break;
      case 'p':
        P = atoi(optarg);
        break;
      case 'b':
        b = atoi(optarg);
        break;
      case 'a':
        a = atoi(optarg);
        break;
      case 'n':
        bits = atoi(optarg);
        break;
      case 'x':
        x1 = atoi(optarg);
        break;
      case 'y':
        y1 = atoi(optarg);
        break;
      case 'z':
        z1 = atoi(optarg);
        break;
      case 'X':
        x2 = atoi(optarg);
        break;
      case 'Y':
        y2 = atoi(optarg);
        break;
      case 'Z':
        z2 = atoi(optarg);
        break;
      default:
        exit(EXIT_FAILURE);
        break;
    }
  }

  //Orquestador
  if(mode == 1)
  {
    if(P <= 3)
    {
      fprintf(stderr, "Error: El número primo debe ser: p>3 \n");
      exit(EXIT_FAILURE);
    }
    excercise_one(P);
  }
  else if(mode == 2)
  {
    if(bits < 4)
    {
      fprintf(stderr, "Error: La cantidad de bits debe ser al menos de cuatro.\n");
      exit(EXIT_FAILURE);
    }
    excercise_two(bits);
  }
  else if(mode == 3)
  {
    if(P <= 3)
    {
      fprintf(stderr, "Error: El número primo debe ser: p>3 \n");
      exit(EXIT_FAILURE);
    }
    excercise_three(a, b, P);
  }
  else if(mode == 4)
  {
    if(P <= 3)
    {
      fprintf(stderr, "Error: El número primo debe ser: p>3\n");
      exit(EXIT_FAILURE);
    }
    excercise_four(x1,y1,z1,x2,y2,z2,a,b,P);
  }
  else if(mode == 5)
  {
    if(P <= 3)
    {
      fprintf(stderr, "Error: El número primo debe ser: p>3\n");
      exit(EXIT_FAILURE);
    }
    excercise_five(x1,y1,z1,a,b,P);
  }

  return 0;
}

// Modo1: ./practica -1 -p <primo>
// Modo2: ./practica -2 -n <bits>
// Modo3: ./practica -3 -a <a> -b <b> -p <primo>
// Modo4: ./practica -4 -x <x1> -y <y1> -z <z1> -X <x2> -Y <y2> -Z <z2> -a <a> -b <b> -p <p>
// Modo5: ./practica -5 -x <x> -y <y> -z <z> -a <a> -b <b> -p <p>
