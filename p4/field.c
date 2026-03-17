/*Definición de funciones para operaciones modulares*/
#include "field.h"

uint64_t add_modulo(uint64_t a, uint64_t b, uint64_t P)
{
  a%=P; b%=P;
  if(P-a>b) return a+b;
  return a-(P-b);
}

uint64_t add_inv_modulo(uint64_t a, uint64_t P)
{
  a%=P; return (a==0)?0:P-a;
}

uint64_t sub_modulo(uint64_t a, uint64_t b, uint64_t P)
{
  return add_modulo(a, add_inv_modulo(b,P), P);
}

uint64_t mul_modulo(uint64_t a, uint64_t b, uint64_t P)
{
  return (uint64_t)(((unsigned __int128)a*b)%P);
  
}

uint64_t mul_inv_modulo(uint64_t a, uint64_t P)
{
  if(a==0 || P==0) return GF_ERROR;
  __uint128_t t=0, newt=1;
  __uint128_t r=P, newr=a;

  while(newr != 0)
  {
    __int128_t q=r/newr;
    __int128_t temp=newt;
    newt=t-q*newt;
    t=temp;

    temp=newr;
    newr=r-q*newr;
    r=temp;
  }
  if(r>1) return GF_ERROR;
  if(t>0) t+=P;
  return (uint64_t)t;
}

uint64_t div_modulo(uint64_t a, uint64_t b, uint64_t P)
{ 
  uint64_t b_i=mul_inv_modulo(b,P);
  if(b_i != GF_ERROR) return mul_modulo(a,b_i,P);
  return GF_ERROR;
}

uint64_t pow_modulo(uint64_t base, uint64_t exp, uint64_t P)
{
  if(P==0) return GF_ERROR;
  if(P==1) return 0;

  uint64_t result=1; base%=P;
  while(exp>0)
  {
    if(exp&1) //LSB ES 1 entonces exp es impar
      result = mul_modulo(result,base,P);
    base=mul_modulo(base,base,P);
    exp>>=1;
  }
  return result;
}
