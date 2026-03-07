#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ec.h"



void ecdlp_brute_force(mpz_t k_result, Point *Q, Point *G, EC *E)
{
    if (is_infinite_point(Q)) 
    {
        mpz_set_ui(k_result, 0);
        return;
    }

    Point Current, Temp;
    init_point(&Current);
    init_point(&Temp);
    
    set_point(G->x, G->y, G->z, &Current); //EMpezamos en 1*G

    mpz_t k;
    mpz_init_set_ui(k, 1);

    while(1) //Hazta encontrar K :( 
    {
        if (is_equal_point(&Current, Q)) 
        {
            mpz_set(k_result, k);
            break;
        }

        if (is_infinite_point(&Current) && mpz_cmp_ui(k, 1) > 0) 
        {
            mpz_set_si(k_result, -1); 
            break;
        }

        if (is_equal_point(&Current, G))
            double_point(&Temp, &Current, E);
        else
            add_point(&Temp, &Current, G, E);
      
        set_point(Temp.x, Temp.y, Temp.z, &Current);
        mpz_add_ui(k, k, 1);
    }

    mpz_clears(k, NULL);
    del_point(&Current);
    del_point(&Temp);
}


void exercise_one(char* a_str, char* b_str, char* p_str, char* x1_str, char* y1_str, char* z1_str, char* k_str)
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

    if(is_singular_ec(E))
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

    if(!is_on_curve(&Q, &E))
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
    scalar_multiply(&R, &Q, k, &E);
    
    gmp_printf("\nResultado de %Zd * P:\n", k);
    print_point(&R);

    // Limpieza
    mpz_clears(a, b, P, x1, y1, z1, k, NULL);
    del_point(&Q); del_point(&R);
    del_ec(&E);
}

void exercise_two(char* a_str, char* b_str, char* p_str, char* x1_str, char* y1_str, char* z1_str, char* x2_str, char* y2_str, char* z2_str)
{
    EC E; 
    init_ec(&E);
    mpz_t a, b, P;
    mpz_inits(a, b, P, NULL);

    if (mpz_set_str(a, a_str, 10) != 0 || mpz_set_str(b, b_str, 10) != 0 || mpz_set_str(P, p_str, 10) != 0) {
        fprintf(stderr, "Error: Parámetros de curva inválidos.\n");
        exit(EXIT_FAILURE);
    }
    set_ec(a, b, P, &E);

    if(is_singular_ec(E))
    {
        fprintf(stderr, "Error: La curva es singular.\n");
        exit(EXIT_FAILURE);
    }

    Point Q, G; 
    init_point(&Q); 
    init_point(&G); 

    mpz_t tx, ty, tz;
    mpz_inits(tx, ty, tz, NULL);
    
    // Punto Q (Objetivo)
    mpz_set_str(tx, x1_str, 10); mpz_set_str(ty, y1_str, 10); mpz_set_str(tz, z1_str, 10);
    set_point(tx, ty, tz, &Q);
    
    // Punto G (Generador)
    mpz_set_str(tx, x2_str, 10); mpz_set_str(ty, y2_str, 10); mpz_set_str(tz, z2_str, 10);
    set_point(tx, ty, tz, &G);

    if(!(is_on_curve(&Q, &E) && is_on_curve(&G, &E)))
    {
        fprintf(stderr, "Error: Q o G no pertenecen a la curva.\n");
        exit(EXIT_FAILURE);
    }

    mpz_t k_result; 
    mpz_init(k_result);

    // Ejecución
    ecdlp_brute_force(k_result, &Q, &G, &E);

    if (mpz_cmp_si(k_result, -1) == 0) {
        fprintf(stderr, "\n[Ejercicio 2] Fallo: Q no pertenece al subgrupo generado por G.\n");
    } else {
        gmp_printf("\n[Ejercicio 2] Éxito. Q = %Zd * G\n", k_result);
    }

    // Limpieza
    mpz_clears(a, b, P, tx, ty, tz, k_result, NULL);
    del_point(&Q); del_point(&G);
    del_ec(&E);
}

int main(int argc, char **argv)
{
    int opt;
    int mode = 0;
    
    //Los argumentos de CLI se manejaran como cadenas porque si se usan como int con atoi entonces 
    //no va a servir de nadota que use gmp porque estría limitado a los 32 bits de un int
    char *a = NULL, *b = NULL, *P = NULL, *k = NULL;
    char *x1 = NULL, *y1 = NULL, *z1 = NULL;
    char *x2 = NULL, *y2 = NULL, *z2 = NULL;

    while((opt = getopt(argc, argv, "12a:b:p:k:x:y:z:X:Y:Z:")) != -1)
    {
        switch(opt)
        {
            case '1': mode = 1; break;
            case '2': mode = 2; break;
            case 'a': a = optarg; break;
            case 'b': b = optarg; break;
            case 'p': P = optarg; break;
            case 'k': k = optarg; break;
            case 'x': x1 = optarg; break;
            case 'y': y1 = optarg; break;
            case 'z': z1 = optarg; break;
            case 'X': x2 = optarg; break;
            case 'Y': y2 = optarg; break;
            case 'Z': z2 = optarg; break;
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
        exercise_one(a, b, P, x1, y1, z1, k);
    }
    else if (mode == 2)
    {
        if (!a || !b || !P || !x1 || !y1 || !z1 || !x2 || !y2 || !z2) 
        {
            fprintf(stderr, "Error: Faltan argumentos para el Ejercicio 2.\n");
            exit(EXIT_FAILURE);
        }
        exercise_two(a, b, P, x1, y1, z1, x2, y2, z2);
    }

    return 0;
    
}/*Modos de uso*/
//./practica -1 -a <a> -b <b> -p <p> -x <x> -y <y> -z <z> -k <k>
//./practica -2 -a <a> -b <b> -p <p> -x <x1> -y <y1> -z <z1> -X <x2> -Y <y2> -Z <z2>
