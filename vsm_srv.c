#include "vsm_srv.h"

void vm_main(void) {
    vm_reg_sm_callback(handle_smevt);
}

VMINT handle_smevt(VMINT message, VMINT param) {

    switch (message)
    {
    case VM_SM_MSG_LOAD:
        break;
    case VM_SM_MSG_QUERY:
        switch (param)
        {
        case id_get_hello_string:
            return (VMINT)get_hello_string;
        default:
            return NULL;
        }
        break;
        case VM_SM_MSG_UNLOAD:
            break;
    }
    return 0;
}

void get_hello_string(VMSTR str, VMINT len) {

    SM_PROCESS_BEGIN()
        
        strncpy(str, "Hello SM !!!", len);
    
    SM_PROCESS_END()
}