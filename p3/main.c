#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ec.h"


void ecdh(char* a_str, char* b_str, char* p_str, char* x1_str, char* y1_str, char* z1_str, char* k_str, CTX* ctx)
{
    EC E; 
    init_ec(&E);
    mpz_t a, b, P;
    mpz_inits(a, b, P, NULL);

    // Validación estricta de entrada
    if (mpz_set_str(a, a_str, 10) != 0 || mpz_set_str(b, b_str, 10) != 0 || mpz_set_str(P, p_str, 10) != 0) 
    {
        fprintf(stderr, "Error: Parámetros de curva inválidos.\n");
        exit(EXIT_FAILURE);
    }
    
    set_ec(a, b, P, &E);

    if(is_singular_ec(E, ctx))
    {
        fprintf(stderr, "Error: La curva es singular.\n");
        exit(EXIT_FAILURE);
    }

    Point Q, R; 
    init_point(&Q); 
    init_point(&R); 

    mpz_t x1, y1, z1;
    mpz_inits(x1, y1, z1, NULL);
    mpz_set_str(x1, x1_str, 10);
    mpz_set_str(y1, y1_str, 10);
    mpz_set_str(z1, z1_str, 10);
    set_point(x1, y1, z1, &Q);

    if(!is_on_curve(&Q, &E, ctx))
    {
        fprintf(stderr, "Error: El punto Q no pertenece a la curva.\n");
        exit(EXIT_FAILURE);
    }

    mpz_t k; 
    mpz_init(k);
    if(mpz_set_str(k, k_str, 10) != 0) {
        fprintf(stderr, "Error: Escalar k inválido.\n");
        exit(EXIT_FAILURE);
    }

    // Ejecución
    scalar_multiply_ltor(&R, &Q, k, &E, ctx);
    
    gmp_printf("\nResultado de %Zd * P:\n", k);
    print_point(&R);

    // Limpieza
    mpz_clears(a, b, P, x1, y1, z1, k, NULL);
    del_point(&Q); del_point(&R);
    del_ec(&E);
}

int main(int argc, char **argv)
{
    int opt;
    int mode = 0;

    CTX ctx;
    init_ctx(&ctx);
    
    //Los argumentos de CLI se manejaran como cadenas porque si se usan como int con atoi entonces 
    //no va a servir de nadota que use gmp porque estría limitado a los 32 bits de un int
    char *a = NULL, *b = NULL, *P = NULL, *k = NULL;
    char *x1 = NULL, *y1 = NULL, *z1 = NULL;

    while((opt = getopt(argc, argv, "1a:b:p:k:x:y:z:")) != -1)
    {
        switch(opt)
        {
            case '1': mode = 1; break;
            case 'a': a = optarg; break;
            case 'b': b = optarg; break;
            case 'p': P = optarg; break;
            case 'k': k = optarg; break;
            case 'x': x1 = optarg; break;
            case 'y': y1 = optarg; break;
            case 'z': z1 = optarg; break;
            default:
                fprintf(stderr, "Uso inválido de banderas.\n");
                exit(EXIT_FAILURE);
        }
    }

    if (mode == 0) 
    {
        fprintf(stderr, "Error: Debes especificar el modo -1 o -2.\n");
        exit(EXIT_FAILURE);
    }

    if (mode == 1)
    {
        if (!a || !b || !P || !x1 || !y1 || !z1 || !k) 
        {
            fprintf(stderr, "Error: Faltan argumentos para el Ejercicio 1.\n");
            exit(EXIT_FAILURE);
        }
        ecdh(a, b, P, x1, y1, z1, k, &ctx);
    }
    del_ctx(&ctx);
    return 0;
    
}/*Modos de uso*/
//./practica -1 -a <a> -b <b> -p <p> -x <x> -y <y> -z <z> -k <k>
