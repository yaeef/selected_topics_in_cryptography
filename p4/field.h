/*Definición de operaciones modulares*/

#ifndef FIELD_H
#define FIELD_H
#include <stdint.h>
#define GF_ERROR UINT64_MAX //Definida para errores :p


uint64_t add_modulo(uint64_t, uint64_t, uint64_t);    
uint64_t add_inv_modulo(uint64_t, uint64_t);
uint64_t sub_modulo(uint64_t, uint64_t, uint64_t);
uint64_t mul_modulo(uint64_t, uint64_t, uint64_t);
uint64_t mul_inv_modulo(uint64_t, uint64_t);
uint64_t div_modulo(uint64_t, uint64_t, uint64_t);
uint64_t pow_modulo(uint64_t, uint64_t, uint64_t);
#endif // FIELD_H
