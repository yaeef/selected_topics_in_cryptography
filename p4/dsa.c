/*Definición de funciones
 * */
#include "dsa.h"
#include "field.h"

uint64_t gen_random_number(void) 
{
  uint64_t n;
  ssize_t m = getrandom(&n, sizeof(uint64_t),GRND_NONBLOCK);
  if(m == -1) return -1;
  return n; 
}

int is_prime(uint64_t n)
{
  if(n <= 1) return 0;                      //caso n=0,1,2
  if(n <= 3) return 1;                      //caso n=3
  if(n%2 == 0 || n%3 == 0) return 0;         //caso n=par,impar

  for(uint64_t i=5; i*i<=n; i+=6)
  {
    if(n%i == 0 || n%(i+2) == 0) return 0;
  }
  return 1;
}

uint64_t gen_q_prime(void)
{
  uint64_t q;
  do{q=gen_random_number()%(1024-11+1)+11;}
  while(!is_prime(q));
  return q;
}

uint64_t gen_p_prime(uint64_t q)
{
  uint64_t k=1;
  uint64_t p=k*q+1;
  while(!is_prime(p))
  {
    k++;
    p=k*q+1;
  }
  return p;
}

uint64_t gen_h_number(uint64_t p)
{
  uint64_t h=0;
  do{h=gen_random_number()%((p-1)-11+1)+11;}
  while(h==0);
  return h;
}

uint64_t gen_d_number(uint64_t q)
{
  uint64_t d=0;
  do{d=gen_random_number()%(q-11+1)+11;}
  while(d==0);
  return d;
}

int gen_pq(pub_k *pubkey)
{
  if(pubkey != NULL)
  {
    pubkey->q = gen_q_prime();
    pubkey->p = gen_p_prime(pubkey->q);
    return 1;
  }
  return 0;
}


int gen_g(pub_k *pubkey)
{
  if(pubkey != NULL)
  {
    if(pubkey->p>3 && pubkey->q>3)
    {
      //Se verifica que se tenga un apuntador válido y que se tengan primos válidos :p
      uint64_t e = (pubkey->p-1)/pubkey->q;
      uint64_t h = gen_h_number(pubkey->p);
      uint64_t g;
      do{g = pow_modulo(h,e,pubkey->p);}
      while(g<2 || g>pubkey->p);
      if(pow_modulo(g,pubkey->q, pubkey->p) == 1)
      { 
        pubkey->g = g;
        return 1;
      }
      return 0;
    }
  }
  return 0;
}

int gen_keys(pub_k *pubkey, priv_k *privkey)
{
  if(pubkey!=NULL && privkey!=NULL)
  {
    privkey->d = gen_d_number(pubkey->q);
    pubkey->B = pow_modulo(pubkey->g, privkey->d, pubkey->p);
    return 1;
  }
  return 0;
}

uint64_t gen_ephimeral_key(uint64_t q)
{
  uint64_t K_E;
  do{K_E=gen_random_number()%(q-11+1)+11;}
  while(K_E>q);
  return K_E;
}

uint64_t gen_r_signature(pub_k *pubkey, uint64_t K_E)
{
  uint64_t r;
  do
  {
    r = pow_modulo(pubkey->g, K_E, pubkey->p);
    r%=pubkey->q;
  }
  while(0>=r || r>=pubkey->q);
  return r;
}

uint64_t gen_s_signature(pub_k *pubkey, priv_k *privkey, uint64_t K_E_i, uint64_t r, uint64_t m)
{
  uint64_t s;  
  do
  {
    s = mul_modulo(privkey->d, r, pubkey->q);
    s = add_modulo(m, s, pubkey->q);
    s = mul_modulo(s, K_E_i, pubkey->q);
  }
  while(0>=s || s>=pubkey->q);
  return s;
}
  
int gen_signature(signature *sig, pub_k *pubkey, priv_k *privkey, uint64_t m)
{
  if(sig != NULL && pubkey != NULL && privkey != NULL)
  {
    uint64_t K_E = gen_ephimeral_key(pubkey->q);                 //efimera
    uint64_t K_E_i = mul_inv_modulo(K_E, pubkey->q);               //efimera inversa
    sig->r = gen_r_signature(pubkey, K_E);
    sig->s = gen_s_signature(pubkey, privkey, K_E_i, sig->r, m);
    return 1;
  }
  return 0;
}

int verif_signature(pub_k *pubkey, signature *sig, uint64_t m)
{
  if(pubkey != NULL && sig != NULL)
  {
    uint64_t w = mul_inv_modulo(sig->s, pubkey->q);
    uint64_t u1 = mul_modulo(w, m, pubkey->q);
    uint64_t u2 = mul_modulo(w, sig->r, pubkey->q);
    uint64_t aux1 = pow_modulo(pubkey->g, u1, pubkey->p);
    uint64_t aux2 = pow_modulo(pubkey->B, u2, pubkey->p);
    uint64_t v = mul_modulo(aux1, aux2, pubkey->p);
    v = v%pubkey->q;
    if(v == sig->r)
      return 1;
    return 0;
  }
  return 0;

}

