#include "ec.h"
#include <fcntl.h>
#include <unistd.h>

void init_signature(Signature *sig) { mpz_inits(sig->r, sig->s, NULL); }
void del_signature(Signature *sig) { mpz_clears(sig->r, sig->s, NULL); }

static void get_random_k(mpz_t k, mpz_t n) {
    int fd = open(ENTROPY_POOL, O_RDONLY);
    if (fd == -1) { perror("urandom"); exit(1); }
    size_t bytes = (mpz_sizeinbase(n, 2) + 7) / 8;
    unsigned char *buf = malloc(bytes);
    do {
        if (read(fd, buf, bytes) < (ssize_t)bytes) { /* error */ }
        mpz_import(k, bytes, 1, 1, 0, 0, buf);
        mpz_mod(k, k, n);
    } while (mpz_cmp_ui(k, 0) == 0);
    free(buf); close(fd);
}

void generate_keypair(mpz_t priv, Point *pub, Point *G, ECDSA_Params *params, CTX *ctx) {
    get_random_k(priv, params->n);
    EC curve; init_ec(&curve);
    mpz_set(curve.a, params->a); mpz_set(curve.b, params->b); mpz_set(curve.P, params->P);
    
    scalar_multiply(pub, G, priv, &curve, ctx);
    del_ec(&curve);
}

// Se recibe msg_val directamente en lugar del hash
void ecdsa_sign(Signature *sig, mpz_t priv, mpz_t msg_val, Point *G, ECDSA_Params *params, CTX *ctx) {
    mpz_t k, inv_k, tmp;
    mpz_inits(k, inv_k, tmp, NULL);
    Point R_pt; init_point(&R_pt);
    EC curve; init_ec(&curve);
    mpz_set(curve.a, params->a); mpz_set(curve.b, params->b); mpz_set(curve.P, params->P);

    do {
        do {
            get_random_k(k, params->n);
            scalar_multiply(&R_pt, G, k, &curve, ctx);
            mpz_mod(sig->r, R_pt.x, params->n);
        } while (mpz_cmp_ui(sig->r, 0) == 0);

        mpz_invert(inv_k, k, params->n);
        mpz_mul(tmp, sig->r, priv);
        mpz_add(tmp, tmp, msg_val); // Se suma el mensaje en bruto
        mpz_mul(sig->s, inv_k, tmp);
        mpz_mod(sig->s, sig->s, params->n);
    } while (mpz_cmp_ui(sig->s, 0) == 0);

    mpz_clears(k, inv_k, tmp, NULL); 
    del_point(&R_pt); del_ec(&curve);
}

// Se verifica msg_val directamente
int ecdsa_verify(Signature *sig, Point *pub, mpz_t msg_val, Point *G, ECDSA_Params *params, CTX *ctx) {
    if (mpz_cmp_ui(sig->r, 0) <= 0 || mpz_cmp(sig->r, params->n) >= 0) return 0;
    if (mpz_cmp_ui(sig->s, 0) <= 0 || mpz_cmp(sig->s, params->n) >= 0) return 0;

    mpz_t w, u1, u2;
    mpz_inits(w, u1, u2, NULL);
    Point P1, P2, Final;
    init_point(&P1); init_point(&P2); init_point(&Final);
    EC curve; init_ec(&curve);
    mpz_set(curve.a, params->a); mpz_set(curve.b, params->b); mpz_set(curve.P, params->P);

    mpz_invert(w, sig->s, params->n);
    mpz_mul(u1, msg_val, w); mpz_mod(u1, u1, params->n); // Multiplicado por el mensaje
    mpz_mul(u2, sig->r, w); mpz_mod(u2, u2, params->n);

    scalar_multiply(&P1, G, u1, &curve, ctx);
    scalar_multiply(&P2, pub, u2, &curve, ctx);
    add_point(&Final, &P1, &P2, &curve, ctx);

    int valid = (mpz_cmp(Final.x, sig->r) == 0);

    mpz_clears(w, u1, u2, NULL); 
    del_point(&P1); del_point(&P2); del_point(&Final); del_ec(&curve);
    return valid;
}
