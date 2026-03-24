#include "ec.h"

void init_ctx(CTX *ctx) {
    mpz_inits(ctx->temp0, ctx->temp1, ctx->temp2, ctx->temp3, ctx->temp4, 
              ctx->temp5, ctx->temp6, ctx->temp7, ctx->temp8, ctx->temp9, NULL);
}

void del_ctx(CTX *ctx) {
    mpz_clears(ctx->temp0, ctx->temp1, ctx->temp2, ctx->temp3, ctx->temp4, 
               ctx->temp5, ctx->temp6, ctx->temp7, ctx->temp8, ctx->temp9, NULL);
}

void init_point(Point *p) { mpz_inits(p->x, p->y, p->z, NULL); }

void set_point(mpz_t x, mpz_t y, mpz_t z, Point *p) {
    mpz_set(p->x, x); mpz_set(p->y, y); mpz_set(p->z, z);
}

void set_point_ui(unsigned int x, unsigned int y, unsigned int z, Point *p) {
    mpz_set_ui(p->x, x); mpz_set_ui(p->y, y); mpz_set_ui(p->z, z);
}

void del_point(Point *p) { mpz_clears(p->x, p->y, p->z, NULL); }

int is_infinite_point(Point *P) {
    return (mpz_cmp_ui(P->z, 0) == 0);
}

void print_point(Point *P) {
    if(is_infinite_point(P)) printf("\tPoint at Infinity\n");
    else gmp_printf("\t(X: %Zd, Y: %Zd)\n", P->x, P->y);
}

/* --- Funciones de Curva faltantes --- */
void init_ec(EC *ec) {
    mpz_inits(ec->a, ec->b, ec->P, NULL);
}

void del_ec(EC *ec) {
    mpz_clears(ec->a, ec->b, ec->P, NULL);
}

int is_singular_ec(EC ec, CTX *ctx) {
    mpz_powm_ui(ctx->temp8, ec.a, 3, ec.P);
    mpz_mul_ui(ctx->temp8, ctx->temp8, 4);
    mpz_powm_ui(ctx->temp9, ec.b, 2, ec.P);
    mpz_mul_ui(ctx->temp9, ctx->temp9, 27);
    mpz_add(ctx->temp7, ctx->temp8, ctx->temp9);
    mpz_mod(ctx->temp7, ctx->temp7, ec.P);
    return (mpz_cmp_ui(ctx->temp7, 0) == 0);
}

int is_on_curve(Point *p, EC *ec, CTX *ctx) {
    if (is_infinite_point(p)) return 1;
    mpz_powm_ui(ctx->temp0, p->x, 3, ec->P);
    mpz_mul(ctx->temp1, ec->a, p->x);
    mpz_add(ctx->temp0, ctx->temp0, ctx->temp1);
    mpz_add(ctx->temp0, ctx->temp0, ec->b);
    mpz_mod(ctx->temp0, ctx->temp0, ec->P); 

    mpz_powm_ui(ctx->temp1, p->y, 2, ec->P); 
    return (mpz_cmp(ctx->temp0, ctx->temp1) == 0);
}
/* ------------------------------------ */

int add_point(Point *R, Point *P, Point *Q, EC *E, CTX *ctx) {
    if (is_infinite_point(P)) { set_point(Q->x, Q->y, Q->z, R); return 1; }
    if (is_infinite_point(Q)) { set_point(P->x, P->y, P->z, R); return 1; }
    if (mpz_cmp(P->x, Q->x) == 0) {
        if (mpz_cmp(P->y, Q->y) != 0) { set_point_ui(0, 1, 0, R); return 1; }
        return double_point(R, P, E, ctx);
    }

    mpz_sub(ctx->temp0, Q->y, P->y); 
    mpz_sub(ctx->temp1, Q->x, P->x);
    if (mpz_invert(ctx->temp2, ctx->temp1, E->P) == 0) { set_point_ui(0, 1, 0, R); return 1; }
    
    mpz_mul(ctx->temp3, ctx->temp0, ctx->temp2);
    mpz_mod(ctx->temp3, ctx->temp3, E->P); 

    mpz_powm_ui(ctx->temp4, ctx->temp3, 2, E->P);
    mpz_sub(ctx->temp4, ctx->temp4, P->x);
    mpz_sub(ctx->temp4, ctx->temp4, Q->x);
    mpz_mod(ctx->temp4, ctx->temp4, E->P); 

    mpz_sub(ctx->temp5, P->x, ctx->temp4);
    mpz_mul(ctx->temp5, ctx->temp5, ctx->temp3);
    mpz_sub(ctx->temp5, ctx->temp5, P->y);
    mpz_mod(ctx->temp5, ctx->temp5, E->P); 

    set_point(ctx->temp4, ctx->temp5, ctx->temp1, R);
    mpz_set_ui(R->z, 1);
    return 1;
}

int double_point(Point *R, Point *P, EC *E, CTX *ctx) {
    if (is_infinite_point(P) || mpz_cmp_ui(P->y, 0) == 0) { set_point_ui(0, 1, 0, R); return 1; }

    mpz_powm_ui(ctx->temp0, P->x, 2, E->P);
    mpz_mul_ui(ctx->temp0, ctx->temp0, 3);
    mpz_add(ctx->temp0, ctx->temp0, E->a); 
    
    mpz_mul_ui(ctx->temp1, P->y, 2);
    if (mpz_invert(ctx->temp2, ctx->temp1, E->P) == 0) { set_point_ui(0, 1, 0, R); return 1; }

    mpz_mul(ctx->temp3, ctx->temp0, ctx->temp2);
    mpz_mod(ctx->temp3, ctx->temp3, E->P); 

    mpz_powm_ui(ctx->temp4, ctx->temp3, 2, E->P);
    mpz_sub(ctx->temp4, ctx->temp4, P->x);
    mpz_sub(ctx->temp4, ctx->temp4, P->x);
    mpz_mod(ctx->temp4, ctx->temp4, E->P); 

    mpz_sub(ctx->temp5, P->x, ctx->temp4);
    mpz_mul(ctx->temp5, ctx->temp5, ctx->temp3);
    mpz_sub(ctx->temp5, ctx->temp5, P->y);
    mpz_mod(ctx->temp5, ctx->temp5, E->P); 

    set_point(ctx->temp4, ctx->temp5, P->z, R);
    mpz_set_ui(R->z, 1);
    return 1;
}

void scalar_multiply(Point *R, Point *P, mpz_t k, EC *ec, CTX *ctx) {
    if (mpz_cmp_ui(k, 0) == 0 || is_infinite_point(P)) { set_point_ui(0, 1, 0, R); return; }

    Point Q; init_point(&Q);
    set_point_ui(0, 1, 0, R);
    size_t bits = mpz_sizeinbase(k, 2);

    for (long i = bits - 1; i >= 0; i--) {
        double_point(R, R, ec, ctx);
        if (mpz_tstbit(k, i)) {
            add_point(R, R, P, ec, ctx);
        }
    }
    del_point(&Q);
}
