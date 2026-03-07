/*
 * Autor: Yair Alejandro Erreguin Franco
 * Descripción: Práctica 1 de la materia Selected Topics in Cryptography.
 * Tema: Curvas Elípticas
 * Fecha: 17/02/2026
 * Punto al infinito: (0,1,0)
 * Punto cualquiera : (x,y,1)
 .:DEFINICIONES:.
 * */
 
 #include <stdio.h>
 #include "ec.h"
 
 
void init_point(Point *p)
{
  mpz_inits(p->x, p->y, p->z, NULL);
}

void set_point(mpz_t x, mpz_t y, mpz_t z, Point *p)
{
  mpz_set(p->x, x);
  mpz_set(p->y, y);
  mpz_set(p->z, z);
}

void set_point_ui(unsigned int x, unsigned int y, unsigned int z, Point *p)
{
  mpz_set_ui(p->x, x);
  mpz_set_ui(p->y, y);
  mpz_set_ui(p->z, z);
}

void print_point(Point P)
{
  gmp_printf("\n\tR=(%Zd,%Zd,%Zd)\n", P.x, P.y, P.z);
}

int is_infinite_point(Point *P)
{
  if(mpz_cmp_ui(P->x, 0) == 0 && mpz_cmp_ui(P->y, 1) == 0 && mpz_cmp_ui(P->z, 0) == 0) //Podría verificar solo z=0
    return 1;
  return 0;
}

int is_root_point(Point *P)
{
  if(mpz_cmp_ui(P->y,0) == 0 && mpz_cmp_ui(P->z, 1) == 0) //Si la componente y es cero entonces es un punto raiz
    return 1;
  return 0;
}

int is_equal_point(Point *P, Point *Q)
{
  if(mpz_cmp(P->x, Q->x) == 0  && mpz_cmp(P->y, Q->y) == 0 && mpz_cmp(P->z, Q->z) == 0)
    return 1;
  return 0;
}

int is_inverse_point(Point *P, Point *Q, EC *E)
{
  if(mpz_cmp(P->x, Q->x) != 0) return 0;

  mpz_t sum; mpz_init(sum);
  mpz_add(sum, P->y, Q->y); mpz_mod(sum, sum, E->P);

  int result = (mpz_cmp_ui(sum,0) == 0);  //y+(-y) === 0 mod P

  mpz_clear(sum);
  return result;
}

void del_point(Point *p)
{
  mpz_clears(p->x, p->y, p->z, NULL);
}

int add_point(Point *R, Point *P, Point *Q, EC *E)
{
  if(!is_equal_point(P,Q))   //P!=Q
  {
    if(is_infinite_point(P)) //R = INF + Q = Q
    {
      set_point(Q->x, Q->y, Q->z, R);
      return 1;
    }
    if(is_infinite_point(Q)) //R = P + ING = P
    {
      set_point(P->x, P->y, P->z, R);
      return 1;
    }
    if(is_inverse_point(P,Q, E))
    {
      set_point_ui(0,1,0, R);
      return 1;
    }
    //Caso donde no son iguales, no son puntos inversos y ningun sumando es el punto a infinito
    mpz_t num, den, inv_den, lambda, x3, y3; mpz_inits(num, den, inv_den, lambda, x3, y3, NULL);

    mpz_sub(num, Q->y, P->y); // y2-y1
    mpz_sub(den, Q->x, P->x); // x2-x1

    if(mpz_invert(inv_den, den, E->P) == 0) //Si no hay inverso entonces punto al infinito
      set_point_ui(0, 1, 0, R); 
    else
    {
      mpz_mul(lambda, num, inv_den); //(y2-y1)*(x2-x1)⁻¹
      mpz_mod(lambda, lambda, E->P); //(y2-y1)*(x2-x1)⁻¹ mod P

      //Cálculo de x3
      mpz_powm_ui(x3, lambda, 2, E->P); 
      mpz_sub(x3, x3, P->x);
      mpz_sub(x3, x3, Q->x);
      mpz_mod(x3, x3, E->P); //x3=lambda² - x1 - x2 mod P
      
      //Calculode y3
      mpz_sub(y3, P->x, x3);
      mpz_mul(y3, y3, lambda);
      mpz_sub(y3, y3, P->y);
      mpz_mod(y3, y3, E->P); // y3 = lambda(x1 - x3) - y1 mod P

      set_point(x3, y3, P->z, R);
    }
    mpz_clears(num, den, inv_den, lambda, x3, y3, NULL);
    return 1;
  }
  else
    return 0;

}

int double_point(Point *R, Point *P, EC *E)
{
  if(is_infinite_point(P) || is_root_point(P)) // 2INF = INF o punto raiz con tangente vertica
  {
    set_point_ui(0, 1, 0, R);
    return 1;
  }
  //lambda = 3x²+a /2y
  mpz_t num, den, inv_den, lambda, x3, y3; mpz_inits(num, den, inv_den, lambda, x3, y3, NULL);
  mpz_powm_ui(num, P->x, 2, E->P); //x²
  mpz_mul_ui(num, num, 3);         //3*x²
  mpz_add(num, num, E->a);         //3*x² + a
  mpz_mod(num, num, E->P);         //3*x² + a mod P
  
  mpz_mul_ui(den, P->y, 2);        //2*y
  mpz_mod(den, den, E->P);         //2*y mod P
  
  if(mpz_invert(inv_den, den, E->P) == 0) //Redundante pero para asegurar
    set_point_ui(0,1,0, R);
  else
  {
    mpz_mul(lambda, num, inv_den);
    mpz_mod(lambda, lambda, E->P);

    //Cáluculo de x3  x3 = lambda² - 2x1 mod P
    mpz_powm_ui(x3, lambda, 2, E->P);
    mpz_sub(x3, x3, P->x);
    mpz_sub(x3, x3, P->x);
    mpz_mod(x3, x3,E->P);
    //Cálculo de y3   y3 = lambda(x1 - x3) - y1 mod P
    mpz_sub(y3, P->x, x3);
    mpz_mul(y3, y3, lambda);
    mpz_sub(y3, y3, P->y);
    mpz_mod(y3, y3, E->P);

    set_point(x3, y3, P->z, R);
  }
  mpz_clears(num, den, inv_den, lambda, x3, y3, NULL);
  return 1;
}


void init_ec(EC *ec)
{
  mpz_inits(ec->a, ec->b, ec->P, NULL);
}

int set_ec(mpz_t a, mpz_t b, mpz_t P, EC *ec)
{
  if(mpz_cmp_ui(P, 3) > 0) mpz_set(ec->P, P); 
  else return 0;

  mpz_mod(ec->a, a, ec->P);
  mpz_mod(ec->b, b, ec->P);
  return 1;
}

int set_ec_ui(unsigned int a, unsigned int b, unsigned int P, EC *ec)
{
  if(P>3) mpz_set_ui(ec->P, P);
  else return 0;

  mpz_set_ui(ec->a, a%P);
  mpz_set_ui(ec->b, b%P);
  return 1;
}
void print_ec(EC ec)
{
    gmp_printf("\n\tE: y² === x³ + %Zdx + %Zd (mod %Zd)\n", ec.a, ec.b, ec.P);
}

void del_ec(EC *ec)
{
  mpz_clears(ec->a, ec->b, ec->P, NULL);
}

/*
 * Función para determinar si una Curva Elíptica es singular 
 * */

int is_singular(EC ec)
{
  //4a3+27b2 !== 0 mod P
  mpz_t r, A, B; mpz_inits(r,A,B, NULL);

  mpz_powm_ui(A, ec.a, 3, ec.P);
  mpz_mul_ui(A, A, 4);

  mpz_powm_ui(B, ec.b, 2, ec.P);
  mpz_mul_ui(B, B, 27);

  mpz_add(r, A, B);
  mpz_mod(r, r, ec.P);

  int singular=0;
  if(mpz_cmp_ui(r,0) == 0) singular=1;

  mpz_clears(r, A,B, NULL);
  return singular;
}

/*
 * Punto 1 de la práctica
 * #ec(P) = P(P-1)
 * */

void get_ecs_from_p(mpz_t P)
{
  if(mpz_probab_prime_p(P,55) == 2)
  {
    EC E;
    init_ec(&E);
    mpz_t a,b, counter; mpz_inits(a, b, counter, NULL);
    mpz_set_ui(a, 0);
    mpz_set_ui(b, 0);
    mpz_set_ui(counter, 0);
    set_ec(a, b, P, &E);

    while(mpz_cmp(a, P) < 0)
    {
      while(mpz_cmp(b, P) < 0)
      {
        set_ec(a, b, P, &E);
        if(!is_singular(E))
        {
          mpz_add_ui(counter, counter, 1);
          print_ec(E);
        }
        mpz_add_ui(b,b,1);
      }
      mpz_set_ui(b,0);
      mpz_add_ui(a,a,1);
    }

    gmp_printf("\n\tTotal de curvas no-singulares: %Zd\n", counter);

    mpz_clears(a, b, counter, NULL);
    del_ec(&E);
  }
  else 
    gmp_printf("El número %Zd propablemente no es primo :(\n", P);
}

void get_ec_from_p_bits(int bits)
{
  //Estructura para instanciar la curva
  EC E;
  init_ec(&E);

  //Preparando state para generar el número primo
  gmp_randstate_t state;
  gmp_randinit_default(state);

  mpz_t P,a,b; mpz_inits(P, a, b, NULL);
  gen_prime(P, state, bits);
  set_ec(a, b, P, &E);

  do
  {
    mpz_urandomm(a, state, P);
    mpz_urandomm(b, state, P);
    set_ec(a ,b ,P ,&E);

  }
  while(is_singular(E));
  print_ec(E); printf("\n");

  mpz_clears(P, a, b, NULL);
  gmp_randclear(state);
  del_ec(&E);
}

void point_counting(mpz_t a, mpz_t b, mpz_t P)
{
  EC E;
  init_ec(&E);
  set_ec(a,b,P, &E);
  if(!is_singular(E))
  {
    mpz_t x,y, counter, A, B, aux;
    mpz_inits(x,y, counter, A, B, aux, NULL);
    mpz_set_ui(x, 0);
    mpz_set_ui(y, 0);
    mpz_set_ui(counter, 1); //Punto a infinito

    while(mpz_cmp(x, E.P) < 0)
    {
      //x³+ax+b mod P
      mpz_powm_ui(B, x, 3, E.P);
      mpz_add(B, B, E.b);
      mpz_mod(B, B, E.P);
      mpz_mul(aux, x, E.a);
      mpz_add(B, B, aux);
      mpz_mod(B, B, E.P);

      while(mpz_cmp(y, E.P) < 0)
      {
        //y² mod P
        mpz_powm_ui(A, y, 2, E.P);

        if(mpz_cmp(A,B) == 0)
        {
          gmp_printf("(%Zd, %Zd, 1)\n", x, y);
          mpz_add_ui(counter, counter, 1);
        }
        mpz_add_ui(y, y, 1);
      }
      mpz_set_ui(y, 0);
      mpz_add_ui(x, x, 1);
    }
    gmp_printf("(0, 1, 0)\n");
    gmp_printf("||E|| = %Zd", counter);
    mpz_clears(x, y, counter, A, B, aux, NULL);
  }
  else 
  {
    print_ec(E);
    printf("\tCurva Elíptica Singular :(\n");
  }
  del_ec(&E);
}

/*
 * Función que genera un número aleatorio a partir de una semilla tomada 
 * de /dev/urandom. Al tomar la semilla del pozo de entropia se usa para
 * generar un número aleatorio para despues tomar el número primo siguiente
 * al número generado*/

int gen_prime(mpz_t r, gmp_randstate_t state, unsigned int bits)
{
  //Obtención de seed
  unsigned int seed;
  FILE *f = fopen(ENTROPY_POOL,"rb");
  if(f == NULL) return 0;
  else
  {
    fread(&seed, sizeof(seed) ,1 ,f);
    fclose(f);
  }

  //Generar número aleatorio
  gmp_randseed_ui(state, seed);
  mpz_urandomb(r, state, bits);
  mpz_nextprime(r, r);
  return 1;
}
