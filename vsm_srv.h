#ifndef _VRE_APP_Sm_
#define	_VRE_APP_Sm_

#include "vmsys.h"
#include "vmsm.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"

#define HELLO_SM		"Hello_SM"

enum {
    id_get_hello_string = 1
};

VMINT handle_smevt(VMINT message, VMINT param);
void get_hello_string(VMSTR str, VMINT len);

typedef void (*get_hello_string_t)(VMSTR str, VMINT len);


#endif
