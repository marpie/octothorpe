/*
 *             _        _   _                           
 *            | |      | | | |                          
 *   ___   ___| |_ ___ | |_| |__   ___  _ __ _ __   ___ 
 *  / _ \ / __| __/ _ \| __| '_ \ / _ \| '__| '_ \ / _ \
 * | (_) | (__| || (_) | |_| | | | (_) | |  | |_) |  __/
 *  \___/ \___|\__\___/ \__|_| |_|\___/|_|  | .__/ \___|
 *                                          | |         
 *                                          |_|
 *
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#pragma once

#include <stdbool.h>
#include "strbuf.h"
#include "datatypes.h"

// "Any sufficiently complicated C or Fortran program contains an ad hoc, 
// informally-specified, bug-ridden, slow implementation of half of Common Lisp." 
// (Greenspun's tenth rule of programming)

// there shouldn't be int, use tetra
enum obj_type
{
    OBJ_NONE=0, // not handled normally yet
    OBJ_BYTE,
    OBJ_WYDE,
    OBJ_TETRA,
    OBJ_OCTA,
    OBJ_DOUBLE,
    OBJ_XMM, // 16 bytes
    OBJ_CSTRING,
    OBJ_CONS,
    OBJ_OPAQUE
};

struct _cons_cell;

typedef struct _obj_opaque
{
    void* ptr;
    void (*dumper_fn) (strbuf*, void*); // may be NULL
    void (*free_fn) (void*); // may be NULL
} obj_opaque;

typedef struct _obj
{
    enum obj_type t;
    union
    {
        byte b; // OBJ_BYTE
        wyde w; // OBJ_WYDE
        tetra tb; // OBJ_TETRA
        octa ob; // OBJ_OCTA
        double d; // OBJ_DOUBLE
        byte *xmm; // OBJ_XMM
        char *s; // OBJ_CSTRING
        struct _cons_cell *c; // OBJ_CONS
        struct _obj_opaque *o; // OBJ_OPAQUE
    } u;
} obj;

typedef struct _cons_cell
{
    obj *head; // AKA car
    obj *tail; // AKA cdr
} cons_cell;

#ifdef  __cplusplus
extern "C" {
#endif

obj* LAST(obj *l);
void obj_byte2 (byte i, obj* o);
void obj_wyde2 (wyde i, obj* o);
void obj_tetra2 (tetra i, obj* o);
void obj_octa2 (octa i, obj* o);
void obj_REG2 (REG i, obj* o);
void obj_double2 (double d, obj* o);
void obj_xmm2 (byte *ptr, obj *o);
obj* obj_byte (byte i);
obj* obj_wyde (wyde i);
obj* obj_tetra (tetra i);
obj* obj_octa (octa i);
obj* obj_xmm (byte *ptr);
obj* obj_REG (REG i);
octa zero_extend_to_octa(obj*);
REG zero_extend_to_REG(obj* o);
obj* obj_double (double i);
obj* obj_wyde_n_times (wyde i, int t);
obj* obj_tetra_n_times (tetra i, int t);
obj* obj_cstring (const char *s);
obj* cons (obj* head, obj* tail);
obj* setcdr (obj* cell, obj* new_tail);
obj* create_obj_opaque(void* ptr, void (*dumper_fn) (strbuf*, void *), void (*free_fn) (void*));
bool LISTP(obj *o);
//void obj_dump_as_list(obj *o);
void obj_dump(obj *o);
void obj_to_strbuf(strbuf* sb, obj *o);
obj* obj_dup (obj *src);
bool EQL(obj *o1, obj* o2);
unsigned LENGTH (obj *l);
int get_lowest_byte(obj *i);
bool obj_get_4th_bit(obj *i);
// l1 may be NULL, it's OK. l1 and l2 should be lists!
obj *NCONC (obj *l1, obj *l2);
void obj_free_structures(obj* o);
void obj_free(obj* o);
void obj_free_conses_of_list(obj* o);
bool CONSP(obj* o);
obj* car(obj* o);
obj* cdr(obj* o);
bool obj_is_opaque(obj* o);
void* obj_unpack_opaque(obj* o);
tetra obj_get_as_tetra(obj* o);
octa obj_get_as_octa(obj* o);
byte obj_get_as_byte(obj* o);
wyde obj_get_as_wyde(obj* o);
REG obj_get_as_REG(obj* o);
char* obj_get_as_cstring(obj* o);
byte* obj_get_as_xmm(obj* o);
obj* create_list_1_element (obj *e);
obj* create_list(obj* o, ...); // terminated with NULL
obj* add_to_list(obj* l, obj* o);
void obj_REG2_and_set_type(enum obj_type t, REG v, double f, obj* out);
double obj_get_as_double(obj* o);
void list_of_bytes_to_array (byte** array, unsigned *array_len, obj* o);
void list_of_wydes_to_array (wyde** array, unsigned *array_len, obj* o);
void obj_copy2 (obj *dst, obj *src);
int get_2nd_most_significant_bit(obj *i);
int get_most_significant_bit(obj *i);
void obj_AND_with(obj* op1, byte op2);
bool obj_is_zero(obj* o);
void obj2_sign_extended_shift_right (obj *op1, byte op2, obj *out);
void obj_zero_extend (obj *in, enum obj_type out_type, obj* out);
void obj_sign_extend (obj *in, enum obj_type out_type, obj* out);
void obj_increment(obj *i);
void obj_decrement(obj *i);
int obj_compare(obj *op1, obj *op2);
void obj_add(obj *op1, obj *op2, obj *result);
void obj_subtract(obj *op1, obj *op2, obj *result);
void obj_OR(obj *op1, obj *op2, obj *result);
void obj_AND(obj *op1, obj *op2, obj *result);
void obj_XOR(obj *op1, obj *op2, obj *result);
void obj_NOT(obj *op1, obj *result);
void obj_NEG(obj *op1, obj *result);
unsigned obj_width_in_bits(obj *o);
obj* text_file_to_list (char *fname, bool trim_newlines);
// destructive
obj* split_list_into_sublists(obj* input, bool (*pred) (obj*));
// destructive
// may return NULL is the resulting list is empty
obj* delete_if(obj* lst, bool (*predicate) (obj*));
obj* nth (obj* lst, unsigned n); // starting at 0
obj* list_pick_random (obj* lst);
void print_list_of_strings (obj* input);

#ifdef  __cplusplus
}
#endif

