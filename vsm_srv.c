#include "vsm_srv.h"

VMCHAR cmyresult[21] = "Invalid Expression !";
VMFLOAT myresult;
VMINT myresult1 = 0;

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
        case id_get_hello_string2:
            return (VMINT)get_hello_string2;
        case id_get_hello_string3:
            return (VMINT)get_hello_string3;
        case id_get_hello_string4:
            return (VMINT)get_hello_string4;
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

void get_hello_string2(VMSTR str, VMINT len) {

    SM_PROCESS_BEGIN()
        
        strncpy(str, "Hello SM2 !!!", len);
    
    SM_PROCESS_END()
}

void get_hello_string3(VMSTR str, VMINT len) {

    SM_PROCESS_BEGIN()
        
        strncpy(str, "Hello SM3 !!!", len);
    
    SM_PROCESS_END()
}

void get_hello_string4(VMSTR str, VMINT len, VMSTR input_data) {

    SM_PROCESS_BEGIN()
        
        myresult = parse_string(input_data);
        if (myresult1 != 1) {sprintf(cmyresult, "%01.2f", myresult);}
        strncpy(str, cmyresult, len);

    SM_PROCESS_END()
}

//-----------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    char * start;
    int length;
    f64 value;
    u8 kind;
}
Token;

char * stream;
Token token;

bool next_token() {

    bool fractional;
    f64 whole_portion;
    f64 fractional_portion;
    f64 fractional_multiplier;
    if (*stream >= '0' && *stream <= '9')
    {
        // The next token is a number.
        token.start = stream;
        whole_portion = 0.0;
        fractional_portion = 0.0;
        fractional_multiplier = 0.1;
        fractional = false;
        while ((*stream >= '0' && *stream <= '9') || *stream == '.')
        {
            if (*stream == '.')
            {
                fractional = true;
                ++stream;
            }
            else if (fractional)
            {
                fractional_portion += (*stream++ - '0') * fractional_multiplier;
                fractional_multiplier *= 0.1;
            }
            else
            {
                whole_portion *= 10.0;
                whole_portion += *stream++ - '0';
            }
        }
        token.length = stream - token.start;
        token.kind = TOKEN_NUMBER;
        token.value = whole_portion + fractional_portion;
    }
    else if (*stream == ' ' || *stream == '\t')
    {
        // The next token is whitespace, so skip it.
        while (*stream == ' ' || *stream == '\t') ++stream;
        next_token();
    }
    else if (*stream == '+' || *stream == '-' ||
             *stream == '*' || *stream == '/' ||
             *stream == '(' || *stream == ')')
    {
        // The next token is an operator.
        token.start = stream;
        token.length = 1;
        token.kind = *stream++;
    }
    else if (*stream == '\0')
    {
        // The next token is the end of the character stream.
        token.kind = TOKEN_END;
        token.start = stream;
        token.length = 1;
        return false;
    }
    else
    {
        // The next token is an unknown character.
        if (token.kind == TOKEN_BAD)
        {
            ++token.length;
        }
        else
        {
            token.kind = TOKEN_BAD;
            token.start = stream;
            token.length = 1;
        }
        return false;
    }
    return true;
}

bool token_is(int kind) {

    return token.kind == kind;
}

bool match_token(int kind) {

    if (token_is(kind))
    {
        next_token();
        return true;
    }
    else
    {
        return false;
    }
}

// Numerical values and brackets.
f64 parse_level_three() {

    if (token_is(TOKEN_NUMBER))
    {
        f64 value = token.value;
        next_token();
        return value;
    }
    else if (match_token('('))
    {
        f64 parse_level_zero();
        f64 value = parse_level_zero();
        if (match_token(')'))
        {
            return value;
        }
        else
        {
            //printf("Invalid Expression: Expected ')'.\n");
            //sprintf(cmyresult, "%s", "Invalid Expression: Expected ')'.\n");
            sprintf(cmyresult, "%s", "Invalid Expression !");
            myresult1 = 1;
            return 0;
            exit(1);
        }
    }
    else
    {
        //printf("Invalid Expression: Expected number or '('.\n");
        //sprintf(cmyresult, "%s", "Invalid Expression: Expected number or '('.\n");
        sprintf(cmyresult, "%s", "Invalid Expression !");
        myresult1 = 1;
        return 0;
        exit(1);
    }
}

// Unary plus and minus.
f64 parse_level_two() {

    if (match_token('-'))
    {
        return -parse_level_two();
    }
    else if (match_token('+'))
    {
        return parse_level_two();
    }
    else
    {
        return parse_level_three();
    }
}

// Multiplication and division.
f64 parse_level_one() {

    f64 r_value;
    f64 value = parse_level_two();
    while (token_is('*') || token_is('/'))
    {
        int operator = token.kind;
        next_token();
        r_value = parse_level_one();
        if (operator == '*')
        {
            value *= r_value;
        }
        else if (operator == '/')
        {
            value /= r_value;
        }
        else
        {
            //printf("Invalid Expression: Expected '*' or '/'.\n");
            //sprintf(cmyresult, "%s", "Invalid Expression: Expected '*' or '/'.\n");
            sprintf(cmyresult, "%s", "Invalid Expression !");
            myresult1 = 1;
            return 0;
            exit(1);
        }
    }
    return value;
}

// Addition and subtraction.
f64 parse_level_zero() {

    f64 r_value;
    f64 value = parse_level_one();
    while (token_is('+') || token_is('-'))
    {
        int operator = token.kind;
        next_token();
        r_value = parse_level_one();
        if (operator == '+')
        {
            value += r_value;
        }
        else if (operator == '-')
        {
            value -= r_value;
        }
        else
        {
            //printf("Invalid Expression: Expected '+' or '-'.\n");
            //sprintf(cmyresult, "%s", "Invalid Expression: Expected '+' or '-'.\n");
            sprintf(cmyresult, "%s", "Invalid Expression !");
            myresult1 = 1;
            return 0;
            exit(1);
        }
    }
    return value;
}

f64 parse_string(char * string) {

    f64 result = 0;
    stream = string;
    next_token();
    result = parse_level_zero();
    return result;
}