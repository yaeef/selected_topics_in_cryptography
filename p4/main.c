

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dsa.h"

int key_generation()
{
  pub_k publica;
  priv_k privada;
  if(gen_pq(&publica))
    if(gen_g(&publica))
      if(gen_keys(&publica, &privada))
      {
        printf("Publica:(%lu,%lu,%lu,%lu)\n",publica.p, publica.q,publica.g,publica.B);
        printf("Privada:(%lu)\n",privada.d);
      }
  return 0;
}

int signature_generation(pub_k *publica, priv_k *privada, uint64_t m)
{
  if(publica != NULL && privada != NULL)
  {
    signature firma;
    if(gen_signature(&firma, publica, privada, m))
    {
      printf("signature: (%lu,%lu)\n", firma.r, firma.s);
      return 1;
    }
  }
  return 0;
}

int signature_verification(pub_k *publica, signature *sig, uint64_t m)
{
  if(publica != NULL && sig != NULL)
  {
    if(verif_signature(publica, sig, m))
    {
      printf("Firma VALIDA para: m:%lu y firma:(%lu,%lu)\n",m, sig->r, sig->s);
      return 1;
    }
    else 
    {
      printf("Firma NO VALIDA para: m:%lu y firma:(%lu,%lu)\n",m, sig->r, sig->s);
      return 0;
    }
  }
  return 0;
}
int main(int argc, char **argv)
{
  int opt, mode=0;
  //Se van a usar los argumentos como cadenas 
  char *p=NULL, *q=NULL, *g=NULL, *B=NULL, *d=NULL, *r=NULL, *s=NULL, *m=NULL;

  while((opt = getopt(argc, argv, "KSVp:q:g:B:d:r:s:m:")) != -1)
  {
    switch(opt)
    {
      case 'K': mode=1; break;
      case 'S': mode=2; break;
      case 'V': mode=3; break;
      case 'p': p=optarg; break;
      case 'q': q=optarg; break;
      case 'g': g=optarg; break;
      case 'B': B=optarg; break;
      case 'd': d=optarg; break;
      case 'r': r=optarg; break;
      case 's': s=optarg; break;
      case 'm': m=optarg; break;
      default:
        fprintf(stderr, "Uso invalido de banderas :(\n");
        exit(EXIT_FAILURE);
    }
  }
  if(mode == 0)
  {
    fprintf(stderr, "Error: No especificaste un modo\n");
    exit(EXIT_FAILURE);
  }
  if(mode == 1)
  {
    key_generation();
    return 0;
  }
  if(mode == 2)
  {
    if(!p || !q || !g || !B || !d || !m)
    {
      fprintf(stderr, "Error: Faltan argumentos para generar firma\n");
      exit(EXIT_FAILURE);
    }
    pub_k publica;
    priv_k privada;
    publica.p=strtoull(p, NULL, 10);
    publica.q=strtoull(q, NULL, 10);
    publica.g=strtoull(g, NULL, 10);
    publica.B=strtoull(B, NULL, 10);
    privada.d=strtoull(d, NULL, 10);
    uint64_t msj=strtoull(m, NULL, 10);
    signature_generation(&publica, &privada, msj);
  }
  if(mode == 3)
  {
    if(!p || !q || !g || !B || !r ||!s || !m)
    {
      fprintf(stderr, "Error: Faltan argumentos para generar firma\n");
      exit(EXIT_FAILURE);
    }
    pub_k publica;
    signature firma;
    publica.p=strtoull(p, NULL, 10);
    publica.q=strtoull(q, NULL, 10);
    publica.g=strtoull(g, NULL, 10);
    publica.B=strtoull(B, NULL, 10);
    firma.r = strtoull(r, NULL, 10);
    firma.s = strtoull(s, NULL, 10);
    uint64_t msj=strtoull(m, NULL, 10);
    signature_verification(&publica, &firma, msj);
  }

}

/*Modos de Uso
 * ./dsa -K
 * ./dsa -S -p <p> -q <q> -g <g> -B <B> -d <d> -m <m>
 * ./dsa -V -p <p> -q <q> -g <g> -B <B> -r <r> -s <s> -m <m>
 * */
