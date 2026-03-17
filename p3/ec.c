/*
 * Autor: Yair Alejandro Erreguin Franco
 * Descripción: Práctica 2 STIC.
 * Tema: Curvas Elípticas
 * Fecha: 28/02/2026
 * Punto al infinito: (0,1,0)
 * Punto cualquiera : (x,y,1)
 .:DEFINICIONES:.
 * */


#include <stdio.h>
#include "ec.h"

/*
 * Funciones para estructura
 * de contexto
 * */

void init_ctx(CTX *ctx)
{
  if(ctx != NULL)
    mpz_inits(ctx->temp0, ctx->temp1, ctx->temp2, ctx->temp3, ctx->temp4, ctx->temp5, ctx->temp6, ctx->temp7, ctx->temp8, ctx->temp9, NULL);
}
void del_ctx(CTX *ctx)
{
  if(ctx != NULL)
    mpz_clears(ctx->temp0, ctx->temp1, ctx->temp2, ctx->temp3, ctx->temp4, ctx->temp5, ctx->temp6, ctx->temp7, ctx->temp8, ctx->temp9, NULL);
}


/*
 * Funciones para puntos
 * */
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

void print_point(Point *P)
{
  gmp_printf("\n\tR=(%Zd,%Zd,%Zd)\n", P->x, P->y, P->z);
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

/*
 * Funciones para curvas
 * */
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

int is_singular_ec(EC ec, CTX *ctx) //CTX[7,8,9]
{
  //4a3+27b2 !== 0 mod P
  //r=temp7 | a=temp8 | b=temp9
  mpz_powm_ui(ctx->temp8, ec.a, 3, ec.P);
  mpz_mul_ui(ctx->temp8, ctx->temp8, 4);

  mpz_powm_ui(ctx->temp9, ec.b, 2, ec.P);
  mpz_mul_ui(ctx->temp9, ctx->temp9, 27);

  mpz_add(ctx->temp7, ctx->temp8, ctx->temp9);
  mpz_mod(ctx->temp7, ctx->temp7, ec.P);

  int singular=0;
  if(mpz_cmp_ui(ctx->temp7,0) == 0) singular=1;

  return singular;
}

void del_ec(EC *ec)
{
  mpz_clears(ec->a, ec->b, ec->P, NULL);
}

int is_on_curve(Point *p, EC *ec, CTX *ctx)  //CTX[7,8,9]
{
  if(is_infinite_point(p)) return 1;
  int r=0;
  //mpz_t A,B, aux; mpz_inits(A,B,aux,NULL); temp7=A temp8=B temp9=aux
  //x³+ax+b mod P
  mpz_powm_ui(ctx->temp8, p->x, 3, ec->P); //x³ mod P
  mpz_mul(ctx->temp9, p->x, ec->a);      //ax
  mpz_add(ctx->temp8, ctx->temp8, ctx->temp9);      //x³ +ax
  mpz_add(ctx->temp8, ctx->temp8, ec->b);           //x³+ax+b
  mpz_mod(ctx->temp8, ctx->temp8, ec->P);           //x³+ax+b mod P
  
  //y²
  mpz_powm_ui(ctx->temp7, p->y, 2, ec->P);
  if(mpz_cmp(ctx->temp7, ctx->temp8) == 0) r=1;
  //mpz_clears(A,B,aux,NULL);
  return r;
}

/*
 * Operación de grupo
 * */

int add_point(Point *R, Point *P, Point *Q, EC *E, CTX *ctx) //ctx[0,1,2,3,4,5]
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
    //num->temp0 | den->temp1 | inv_den->temp2 | lambda->temp3 | x3->temp4 | y3->temp5
    //mpz_t num, den, inv_den, lambda, x3, y3; mpz_inits(num, den, inv_den, lambda, x3, y3, NULL);

    mpz_sub(ctx->temp0, Q->y, P->y); // y2-y1
    mpz_sub(ctx->temp1, Q->x, P->x); // x2-x1

    if(mpz_invert(ctx->temp2, ctx->temp1, E->P) == 0) //Si no hay inverso entonces punto al infinito
      set_point_ui(0, 1, 0, R); 
    else
    {
      mpz_mul(ctx->temp3, ctx->temp0, ctx->temp2); //(y2-y1)*(x2-x1)⁻¹
      mpz_mod(ctx->temp3, ctx->temp3, E->P); //(y2-y1)*(x2-x1)⁻¹ mod P

      //Cálculo de x3
      mpz_powm_ui(ctx->temp4, ctx->temp3, 2, E->P); 
      mpz_sub(ctx->temp4, ctx->temp4, P->x);
      mpz_sub(ctx->temp4, ctx->temp4, Q->x);
      mpz_mod(ctx->temp4, ctx->temp4, E->P); //x3=lambda² - x1 - x2 mod P
      
      //Calculode y3
      mpz_sub(ctx->temp5, P->x, ctx->temp4);
      mpz_mul(ctx->temp5, ctx->temp5, ctx->temp3);
      mpz_sub(ctx->temp5, ctx->temp5, P->y);
      mpz_mod(ctx->temp5, ctx->temp5, E->P); // y3 = lambda(x1 - x3) - y1 mod P

      set_point(ctx->temp4, ctx->temp5, P->z, R);
    }
    //mpz_clears(num, den, inv_den, lambda, x3, y3, NULL);
    return 1;
  }
  else
    return 0;
}

int double_point(Point *R, Point *P, EC *E, CTX *ctx) //CTX [0,1,2,3,4,5]
{
  if(is_infinite_point(P) || is_root_point(P)) // 2INF = INF o punto raiz con tangente vertica
  {
    set_point_ui(0, 1, 0, R);
    return 1;
  }
  //lambda = 3x²+a /2y
  //num->temp0 | den->temp1 | inv_den->temp2 | lambda->temp3 | x3->temp4 | y3->temp5
  //mpz_t num, den, inv_den, lambda, x3, y3; mpz_inits(num, den, inv_den, lambda, x3, y3, NULL);
  mpz_powm_ui(ctx->temp0, P->x, 2, E->P); //x²
  mpz_mul_ui(ctx->temp0, ctx->temp0, 3);         //3*x²
  mpz_add(ctx->temp0, ctx->temp0, E->a);         //3*x² + a
  mpz_mod(ctx->temp0, ctx->temp0, E->P);         //3*x² + a mod P
  
  mpz_mul_ui(ctx->temp1, P->y, 2);        //2*y
  mpz_mod(ctx->temp1, ctx->temp1, E->P);         //2*y mod P
  
  if(mpz_invert(ctx->temp2, ctx->temp1, E->P) == 0) //Redundante pero para asegurar
    set_point_ui(0,1,0, R);
  else
  {
    mpz_mul(ctx->temp3, ctx->temp0, ctx->temp2);
    mpz_mod(ctx->temp3, ctx->temp3, E->P);

    //Cáluculo de x3  x3 = lambda² - 2x1 mod P
    mpz_powm_ui(ctx->temp4, ctx->temp3, 2, E->P);
    mpz_sub(ctx->temp4, ctx->temp4, P->x);
    mpz_sub(ctx->temp4, ctx->temp4, P->x);
    mpz_mod(ctx->temp4, ctx->temp4,E->P);
    //Cálculo de y3   y3 = lambda(x1 - x3) - y1 mod P
    mpz_sub(ctx->temp5, P->x, ctx->temp4);
    mpz_mul(ctx->temp5, ctx->temp5, ctx->temp3);
    mpz_sub(ctx->temp5, ctx->temp5, P->y);
    mpz_mod(ctx->temp5, ctx->temp5, E->P);

    set_point(ctx->temp4, ctx->temp5, P->z, R);
  }
  //mpz_clears(num, den, inv_den, lambda, x3, y3, NULL);
  return 1;
}

void scalar_multiply_ltor(Point *R, Point *P, mpz_t k, EC *ec, CTX *ctx) //CTX   CUIDADDOOOOOOO SE PUEDE COLISIONAR EL CTX con is_on_curve y en add_point
{
    if(is_infinite_point(P) || mpz_cmp_ui(k, 0) == 0 || !is_on_curve(P, ec, ctx)) //is_on_curve()[7,8,9]
    {
        set_point_ui(0, 1, 0, R);
        return;
    }
    
    Point Temp; 
    init_point(&Temp); 
    set_point_ui(0, 1, 0, R);
    
    size_t no_bits = mpz_sizeinbase(k, 2);

    for(long int i = no_bits - 1; i >= 0; i--)
    {
        double_point(&Temp, R, ec, ctx); // double_point()[0,1,2,3,4,5]
        set_point(Temp.x, Temp.y, Temp.z, R);

        if(mpz_tstbit(k, i) == 1)
        {
            add_point(&Temp, R, P, ec, ctx);
            set_point(Temp.x, Temp.y, Temp.z, R);
        }
    }
    del_point(&Temp);
}

void scalar_multiply_rtol(Point *R, Point *P, mpz_t k, EC *ec, CTX *ctx)
{
  if(is_infinite_point(P) || mpz_cmp_ui(k, 0) == 0 || !is_on_curve(P, ec, ctx)) //is_on_curve()[7,8,9]
  {
      set_point_ui(0, 1, 0, R);
      return;
  }

  Point Temp;
  init_point(&Temp);
  set_point_ui(0,1,0, R);

  size_t no_bits = mpz_sizeinbase(k,2);

  for(long int i=0; i<no_bits; i++)
  {
    if(mpz_tstbit(k,i) == 1)
    {
      add_point(&Temp, R, P, ec, ctx);
      set_point(Temp.x, Temp.y, Temp.z, R);
    }
    double_point(&Temp, P, ec, ctx);
    set_point(Temp.x, Temp.y, Temp.z, P);
  }
  del_point(&Temp);

}
