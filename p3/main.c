#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ec.h"

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
