#include "ec.h"
#include <unistd.h>
#include <string.h>

void print_usage() {
    fprintf(stderr, "Uso:\n");
    fprintf(stderr, "  Generar:   ./ecdsa -G -a <a> -b <b> -p <p> -n <q> -x <Gx> -y <Gy> -z <Gz>\n");
    fprintf(stderr, "  Firmar:    ./ecdsa -S -a <a> -b <b> -p <p> -n <q> -x <Gx> -y <Gy> -z <Gz> -k <d> -M <mensaje_numerico>\n");
    fprintf(stderr, "  Verificar: ./ecdsa -v -a <a> -b <b> -p <p> -n <q> -x <Gx> -y <Gy> -z <Gz> -M <mensaje_numerico> -r <r> -s <s> -U <Bx> -W <By> -J <Bz>\n");
}

int main(int argc, char **argv) {
    int opt;
    char mode = 0;
    
    char *a = NULL, *b = NULL, *p = NULL, *n = NULL;
    char *gx = NULL, *gy = NULL, *gz = NULL;
    char *k_priv = NULL, *msg_str = NULL;
    char *r_str = NULL, *s_str = NULL;
    char *ux = NULL, *wy = NULL, *jz = NULL; 

    while ((opt = getopt(argc, argv, "GSva:b:p:n:x:y:z:k:M:r:s:U:W:J:")) != -1) {
        switch (opt) {
            case 'G': mode = 'G'; break;
            case 'S': mode = 'S'; break;
            case 'v': mode = 'v'; break;
            case 'a': a = optarg; break;
            case 'b': b = optarg; break;
            case 'p': p = optarg; break;
            case 'n': n = optarg; break; // Aquí se recibe el orden q
            case 'x': gx = optarg; break;
            case 'y': gy = optarg; break;
            case 'z': gz = optarg; break;
            case 'k': k_priv = optarg; break; // Llave privada d
            case 'M': msg_str = optarg; break;
            case 'r': r_str = optarg; break;
            case 's': s_str = optarg; break;
            case 'U': ux = optarg; break; // Llave publica B.x
            case 'W': wy = optarg; break; // Llave publica B.y
            case 'J': jz = optarg; break; // Llave publica B.z
            default: 
                print_usage();
                return 1;
        }
    }

    if (!mode || !a || !b || !p || !n || !gx || !gy || !gz) {
        print_usage();
        return 1;
    }

    CTX ctx; init_ctx(&ctx);
    ECDSA_Params params; mpz_inits(params.a, params.b, params.P, params.n, NULL);
    mpz_set_str(params.a, a, 10); mpz_set_str(params.b, b, 10);
    mpz_set_str(params.P, p, 10); mpz_set_str(params.n, n, 10);

    Point G; init_point(&G);
    mpz_t tx, ty, tz; mpz_inits(tx, ty, tz, NULL);
    mpz_set_str(tx, gx, 10); mpz_set_str(ty, gy, 10); mpz_set_str(tz, gz, 10);
    set_point(tx, ty, tz, &G);

    if (mode == 'G') {
        mpz_t priv; mpz_init(priv); 
        Point pub; init_point(&pub);
        generate_keypair(priv, &pub, &G, &params, &ctx);
        
        // kpriv=(d)
        gmp_printf("kpriv=(%Zd)\n", priv);
        
        // kpuv=(p,a,b,q,G,B) -> donde G=(x,y,z) y B=(x,y,z)
        gmp_printf("kpuv=(%Zd, %Zd, %Zd, %Zd, (%Zd, %Zd, %Zd), (%Zd, %Zd, %Zd))\n",
                   params.P, params.a, params.b, params.n,
                   G.x, G.y, G.z,
                   pub.x, pub.y, pub.z);
                   
        mpz_clear(priv); del_point(&pub);
    } 
    else if (mode == 'S') {
        if (!k_priv || !msg_str) { 
            fprintf(stderr, "Error: Faltan banderas -k o -M\n"); 
            return 1; 
        }
        Signature sig; init_signature(&sig);
        mpz_t priv, msg_val; mpz_inits(priv, msg_val, NULL);
        mpz_set_str(priv, k_priv, 10); mpz_set_str(msg_val, msg_str, 10);
        
        ecdsa_sign(&sig, priv, msg_val, &G, &params, &ctx);
        
        // signature=(r,s)
        gmp_printf("signature=(%Zd, %Zd)\n", sig.r, sig.s);
        
        del_signature(&sig); mpz_clears(priv, msg_val, NULL);
    }
    else if (mode == 'v') {
        if (!r_str || !s_str || !msg_str || !ux || !wy || !jz) { 
            fprintf(stderr, "Error: Faltan banderas para verificar\n"); 
            return 1; 
        }
        Signature sig; init_signature(&sig); 
        Point pub; init_point(&pub);
        mpz_t msg_val; mpz_init(msg_val);
        
        mpz_set_str(sig.r, r_str, 10); mpz_set_str(sig.s, s_str, 10);
        mpz_set_str(msg_val, msg_str, 10);
        
        mpz_set_str(tx, ux, 10); mpz_set_str(ty, wy, 10); mpz_set_str(tz, jz, 10);
        set_point(tx, ty, tz, &pub);
        
        if (ecdsa_verify(&sig, &pub, msg_val, &G, &params, &ctx)) {
            printf("Firma VALIDA\n");
        } else {
            printf("Firma INVALIDA\n");
        }
        
        del_signature(&sig); del_point(&pub); mpz_clear(msg_val);
    }

    mpz_clears(params.a, params.b, params.P, params.n, tx, ty, tz, NULL);
    del_point(&G); del_ctx(&ctx);
    return 0;
}
