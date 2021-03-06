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

// Rationale: I use in x86 emulator testing.

#include "stuff.h"
#include "datatypes.h"

#ifdef  __cplusplus
extern "C" {
#endif

#if __WORDSIZE==32

void intrin_SHL (IN tetra value, IN uint8_t shift_value, OUT tetra* result, IN OUT tetra* flags);
void intrin_SHR (IN tetra value, IN uint8_t shift_value, OUT tetra* result, IN OUT tetra* flags);
void intrin_SAR (IN tetra value, IN uint8_t shift_value, OUT tetra* result, IN OUT tetra* flags);
void intrin_ADD (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags);
void intrin_ADC (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags);
void intrin_SUB (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags);
void intrin_SBB (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags);
void intrin_XOR (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags);
void intrin_XOR_addr (IN tetra *address_of_op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags);
void intrin_OR (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags);
void intrin_AND (IN tetra op1, IN tetra op2, OUT tetra* result, IN OUT tetra* flags);
void intrin_NOT (IN tetra op1, OUT tetra* result, IN OUT tetra* flags);
void intrin_NEG (IN tetra op1, OUT tetra* result, IN OUT tetra* flags);

#elif __WORDSIZE==64

byte rotr8(byte x, byte r);
byte rotl8(byte x, byte r);
wyde rotr16(wyde x, byte r);
wyde rotl16(wyde x, byte r);
tetra rotr32(tetra x, byte r);
tetra rotl32(tetra x, byte r);
octa rotr64(octa x, byte r);
octa rotl64(octa x, byte r);

#else
#error "__WORDSIZE is not defined"
#endif

#ifdef  __cplusplus
}
#endif

