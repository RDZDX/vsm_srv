#ifndef _VRE_APP_Sm_
#define	_VRE_APP_Sm_

#define bool int
#define true 1
#define false 0
#define TOKEN_BAD 'b'
#define TOKEN_END 'e'
#define TOKEN_NUMBER 'n'

#include "vmsys.h"
#include "vmsm.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"

#include <stdlib.h>
#include "stdio.h"
#include <string.h>
#include "vmpromng.h"
#include "stdint.h"

typedef uint8_t u8;
typedef double f64;

enum {
    id_get_hello_string = 1,
    id_get_hello_string2 = 2,
    id_get_hello_string3 = 3
};

//enum my_func {
//    ID_GET_HELLO_STRING1,
//    ID_GET_HELLO_STRING2,
//    ID_GET_HELLO_STRING3,
//};

VMINT handle_smevt(VMINT message, VMINT param);
void get_hello_string(VMSTR str, VMINT len);
void get_hello_string2(VMSTR str, VMINT len);
void get_hello_string3(VMSTR str, VMINT len);

bool next_token();
bool token_is(int kind);
bool match_token(int kind);
f64 parse_level_three();
f64 parse_level_two();
f64 parse_level_one();
f64 parse_level_zero();
f64 parse_string(char * string);

#endif
