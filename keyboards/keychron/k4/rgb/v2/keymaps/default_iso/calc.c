
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "math.h"
#include <stdfix.h>

typedef long long _Accum myfloat;

#define FLOAT_CALC_ENABLE
#define INTEGER_CALC_ENABLE
// const char    *operator_indices = "()*/+-";
// const u_int8_t operator_prios[] = {0, 0, 2, 2, 1, 1};
//# = ** = Pow

// https://en.cppreference.com/w/c/language/operator_precedence

typedef struct {
    char    identifier[3];
    u_int8_t priority;
} Operator;

const Operator operators[] = {
    [0]={.identifier = "(", .priority = 0},
    [1]={.identifier = ")", .priority = 0},
    [2]={.identifier = "**", .priority = 13},
    [3]={.identifier = "*", .priority = 12},
    [4]={.identifier = "/", .priority = 12},
    [5]={.identifier = "+", .priority = 11},
    [6]={.identifier = "-", .priority = 11},
    [7]={.identifier = "<<", .priority = 10},
    [8]={.identifier = ">>", .priority = 10},
    [9]={.identifier = "&", .priority = 7},
    [10]={.identifier = "|", .priority = 5},
    [11]={.identifier = "^", .priority = 6},
    [12]={.identifier = "%", .priority = 12},
};
// const char    *operator_indices = "()<>#*/+-";
// const u_int8_t operator_prios[] = {0, 0, 10, 10, 13, 12, 12, 11, 11};  // ascending precedence
static bool calcMode = false;

typedef struct {
    char *data;
    int   len;
} Data;

enum ExpressionType { VALUE_EXPRESSION, BINARY_EXPRESSION };

typedef struct {
    int expressionType;
} Expression;

enum ValueType { INTEGER, FLOAT };

typedef struct {
    union {
        int        expressionType;
        Expression Expression;
    };
    int valueType;
    union {
#ifdef INTEGER_CALC_ENABLE
        int integer;
#endif
#ifdef FLOAT_CALC_ENABLE
        myfloat floating;
#endif
    };
} ValueExpression;

typedef struct {
    union {
        int        expressionType;
        Expression Expression;
    };
    Expression *a;
    u_int8_t    operator;
    Expression *b;
} BinaryExpression;

enum TokenType {
    INVALID_TOKEN,
#ifdef INTEGER_CALC_ENABLE
    INTEGER_TOKEN,
#endif
#ifdef FLOAT_CALC_ENABLE
    FLOAT_TOKEN,
#endif
    OPERATOR_TOKEN,
    LEFT_PARANTHESIS_TOKEN,
    RIGHT_PARANTHESIS_TOKEN,
    IDENTIFIER_TOKEN,
};

#pragma pack(1)
typedef struct {
    uint8_t index : 8;
    uint8_t priority : 8;
} OperatorTokenData;

typedef struct {
    int tokenType;
    union {
#ifdef INTEGER_CALC_ENABLE
        int integer;
#endif
#ifdef FLOAT_CALC_ENABLE
        myfloat floating;
#endif
        OperatorTokenData operator;
        Data              identifier;
    };
} Token;

typedef struct {
    int   success;
    Data  remainder;
    Token token;
} TokenResult;

typedef struct {
    u_int8_t valueType;
    union {
#ifdef INTEGER_CALC_ENABLE
        int intresult;
#endif
#ifdef FLOAT_CALC_ENABLE
        myfloat floatresult;
#endif
        uint8_t nonresult;
    };
    u_int8_t errorcode;
} EvaluationResult;

myfloat myfloatabs(myfloat val) {
    if (val > 0) return val;
    return -val;
}

#ifdef INTEGER_CALC_ENABLE
ValueExpression *NewValueI(int value) {
    ValueExpression *v = (ValueExpression *)malloc(sizeof(ValueExpression));
    v->valueType       = INTEGER;
    v->integer         = value;
    v->expressionType  = VALUE_EXPRESSION;
    return v;
}
#endif
#ifdef FLOAT_CALC_ENABLE
ValueExpression *NewValueF(myfloat value) {
    ValueExpression *v = (ValueExpression *)malloc(sizeof(ValueExpression));
    v->valueType       = FLOAT;
    v->floating        = value;
    v->expressionType  = VALUE_EXPRESSION;
    return v;
}
#endif

BinaryExpression *NewBinaryExpression(Expression *a, OperatorTokenData operator, Expression * b) {
    BinaryExpression *binExpr = (BinaryExpression *)malloc(sizeof(BinaryExpression));
    binExpr->expressionType   = BINARY_EXPRESSION;
    binExpr->a                = a;
    binExpr->operator         = operator.index;
    binExpr->b                = b;
    return binExpr;
}

#ifdef INTEGER_CALC_ENABLE
#    define TokenizeCharConditional(_input, _condition)  \
        ({                                               \
            Data        _d_a_t_a = (_input);             \
            TokenResult res      = {0, _d_a_t_a, {}};    \
            if (_d_a_t_a.len > 0) {                      \
                char currentChar = _d_a_t_a.data[0];     \
                if (_condition) {                        \
                    res.token.tokenType = INTEGER_TOKEN; \
                    res.token.integer   = currentChar;   \
                    res.remainder.data++;                \
                    res.remainder.len--;                 \
                    res.success = 1;                     \
                }                                        \
            }                                            \
            res;                                         \
        })
#else
#    define TokenizeCharConditional(_input, _condition) \
        ({                                              \
            Data        _d_a_t_a = (_input);            \
            TokenResult res      = {0, _d_a_t_a, {}};   \
            if (_d_a_t_a.len > 0) {                     \
                char currentChar = _d_a_t_a.data[0];    \
                if (_condition) {                       \
                    res.token.tokenType = FLOAT_TOKEN;  \
                    res.token.floating  = currentChar;  \
                    res.remainder.data++;               \
                    res.remainder.len--;                \
                    res.success = 1;                    \
                }                                       \
            }                                           \
            res;                                        \
        })
#endif

#define TOKEN_DIGIT_CONDITION (currentChar >= '0' && currentChar <= '9')
#define TOKEN_LETTER_CONDITION ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z'))
#define TOKEN_IDENTIFIER_CHAR_CONDITION (TOKEN_DIGIT_CONDITION || TOKEN_LETTER_CONDITION)

TokenResult TokenizeChar(Data data, char tokenChar) { return TokenizeCharConditional(data, currentChar == tokenChar); }

TokenResult TokenizeLetter(Data data) { return TokenizeCharConditional(data, TOKEN_LETTER_CONDITION); }

TokenResult TokenizeDigit(Data data) {
    TokenResult res = TokenizeCharConditional(data, TOKEN_DIGIT_CONDITION);
    if (res.success) {
#ifdef INTEGER_CALC_ENABLE
        res.token.integer -= '0';
#else
        res.token.floating -= '0';
#endif
    }
    return res;
}

TokenResult TokenizeIdentifier(Data data) {
    TokenResult res = TokenizeLetter(data);
    if (!res.success) return res;
    do {
        res = TokenizeCharConditional(res.remainder, TOKEN_IDENTIFIER_CHAR_CONDITION);
    } while (res.success);

    res.success               = 1;
    res.token.tokenType       = IDENTIFIER_TOKEN;
    res.token.identifier.data = data.data;
    res.token.identifier.len  = data.len - res.remainder.len;
    return res;
}

TokenResult TokenizeNatural(Data data) {
    TokenResult current = {0, {data.data, data.len}, {}};
    int         offset = 0, value = 0;
    do {
        TokenResult digit = TokenizeDigit(data);
        if (!digit.success) break;
        value *= 10;
#ifdef INTEGER_CALC_ENABLE
        value += digit.token.integer;
#else
        value += digit.token.floating;
#endif
        data = digit.remainder;
        offset++;
    } while (true);

    if (offset > 0) {
        current.success = 1;
#ifdef INTEGER_CALC_ENABLE
        current.token.tokenType = INTEGER_TOKEN;
        current.token.integer   = value;
#else
        current.token.tokenType = FLOAT_TOKEN;
        current.token.floating  = value;
#endif
        current.remainder = data;
    }
    return current;
}

TokenResult TokenizeInt(Data data) {
    TokenResult errorResult = {0, data, {}};

    if (data.len == 0) {
        return errorResult;
    }

    TokenResult negChar = TokenizeChar(data, '-');

    TokenResult number = TokenizeNatural(negChar.remainder);
    if (number.success) {
#ifdef INTEGER_CALC_ENABLE
        number.token.integer *= (negChar.success ? -1 : 1);
#else
        number.token.floating *= (negChar.success ? -1 : 1);
#endif
        return number;
    }
    return errorResult;
}

int digitDivisor(int val) {
    int amount = 1;
    while (val != 0) {
        amount *= 10;
        val /= 10;
    }
    return amount;
}

TokenResult TokenizeNumeric(Data data) {
    TokenResult integer = TokenizeInt(data);
#ifdef FLOAT_CALC_ENABLE
    if (integer.success) {
        TokenResult decimalSepChar = TokenizeCharConditional(integer.remainder, currentChar == '.' || currentChar == ',');
        if (!decimalSepChar.success) return integer;

        TokenResult decimalRest = TokenizeNatural(decimalSepChar.remainder);
#    ifdef INTEGER_CALC_ENABLE
        int         restVal = decimalRest.success ? decimalRest.token.integer : 0;
        TokenResult res     = {1, decimalRest.remainder, {FLOAT_TOKEN, .floating = ((myfloat)integer.token.integer + (myfloat)restVal / (myfloat)digitDivisor(restVal))}};
#    else
        myfloat     restVal = decimalRest.success ? (myfloat)decimalRest.token.integer : 0.0llk;
        TokenResult res     = {1, decimalRest.remainder, {FLOAT_TOKEN, .floating = (integer.token.floating + (restVal - (int)restVal))}};
#    endif
        return res;
    }
#endif
    return integer;
}
#ifdef INTEGER_CALC_ENABLE
EvaluationResult int_add(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a + b, 0}; }
EvaluationResult int_sub(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a - b, 0}; }
EvaluationResult int_mul(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a * b, 0}; }
EvaluationResult int_shift_left(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a << b, 0}; }
EvaluationResult int_shift_right(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a >> b, 0}; }

EvaluationResult int_and(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a & b, 0}; }
EvaluationResult int_or(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a | b, 0}; }
EvaluationResult int_xor(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a ^ b, 0}; }
EvaluationResult int_mod(int a, int b) { return (EvaluationResult){INTEGER, .intresult = a % b, 0}; }
EvaluationResult int_div(int a, int b) {
    if (b == 0) return (EvaluationResult){INTEGER, .intresult = 0, 1};
    return (EvaluationResult){INTEGER, .intresult = a / b, 0};
}
int int_pow_calc(int x, int y) {
    if (y == 0) return 1;
    int temp = int_pow_calc(x, y / 2);
    if (y % 2 == 0)
        return temp * temp;
    else {
        if (y > 0)
            return x * temp * temp;
        else
            return (temp * temp) / x;
    }
}
EvaluationResult int_pow(int a, int b) { return (EvaluationResult){INTEGER, .intresult = int_pow_calc(a, b), 0}; }
typedef EvaluationResult (*BinaryOperatorFunctionInt)(int, int);
BinaryOperatorFunctionInt int_operator_functions[] = {NULL, NULL, int_pow, int_mul, int_div, int_add, int_sub, int_shift_left, int_shift_right, int_and, int_or, int_xor, int_mod};
#endif
#ifdef FLOAT_CALC_ENABLE
EvaluationResult float_add(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a + b, 0}; }
EvaluationResult float_sub(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a - b, 0}; }
EvaluationResult float_mul(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a * b, 0}; }
EvaluationResult float_shift_left(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a << (int)b, 0}; }
EvaluationResult float_shift_right(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a >> (int)b, 0}; }
// EvaluationResult float_and(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a & b, 0}; }
// EvaluationResult float_or(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a | b, 0}; }
// EvaluationResult float_xor(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a ^ b, 0}; }
// EvaluationResult float_mod(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = a % b, 0}; }
EvaluationResult float_div(myfloat a, myfloat b) {
    if (b == 0) return (EvaluationResult){FLOAT, .floatresult = 0, 1};
    return (EvaluationResult){FLOAT, .floatresult = a / b, 0};
}
// myfloat __attribute__ ((noinline)) myfloat_pow(myfloat a, int b){
//     if(b <  0)
//         return 1llk/myfloat_pow(a, -b);
//     myfloat res = 1llk;
//     for (size_t i = 0; i < b; i++) res *= a;
//     return res;
// }
myfloat square_myfloat(myfloat n) {
    /*We are using n itself as initial approximation
   This can definitely be improved */
    myfloat x = n;
    myfloat y = 1;
    myfloat e = 0.000001llk; /* e decides the accuracy level*/
    while (x - y > e) {
        x = (x + y) / 2;
        y = n / x;
    }
    return x;
}

myfloat power_int(myfloat x, int y) {
    if (y == 0) return 1;
    myfloat temp = power_int(x, y / 2);
    if (y % 2 == 0)
        return temp * temp;
    else {
        if (y > 0)
            return x * temp * temp;
        else
            return (temp * temp) / x;
    }
}
myfloat power_fract(myfloat x, myfloat y) {
    // x^0,5 = square_myfloat
    return 1.f;
}
myfloat myfloat_power(myfloat x, myfloat y) {
    int i = (int)y;

    return power_int(x, i) * power_fract(x, y - i);
}

// myfloat __attribute__ ((noinline)) myfloat_pow(myfloat a, int b){
//     if(b <  0)
//         return 1llk/myfloat_pow(a, -b);
//     myfloat res = 1llk;
//     for (size_t i = 0; i < b; i++) res = res * a;
//     return res;
// }
// EvaluationResult float_pow(myfloat a, myfloat b) {
//     int exp = (int)b;
//     myfloat res = 1llk;
//     bool calcResult = exp < 0;
//     exp = abs(exp);
//     for (size_t i = 0; i < exp; i++) res *= a;
//     if(calcResult)
//         res = 1llk/res;
//     return (EvaluationResult){FLOAT, .floatresult = res, 0};
// }
EvaluationResult float_pow(myfloat a, myfloat b) { return (EvaluationResult){FLOAT, .floatresult = myfloat_power(a, b), 0}; }
typedef EvaluationResult (*BinaryOperatorFunctionFloat)(myfloat, myfloat);
BinaryOperatorFunctionFloat float_operator_functions[] = {NULL, NULL, float_pow, float_mul, float_div, float_add, float_sub, float_shift_left, float_shift_right, NULL, NULL, NULL, NULL};
#endif

TokenResult TokenizeOperator(Data data) {
    TokenResult errorResult = {0, data};

    if (data.len == 0) {
        return errorResult;
    }

    // for (int i = 0; i < strlen(operator_indices); i++) {
    //     if (data.data[0] == operator_indices[i]) {
    //         TokenResult res = {1, {data.data + 1, data.len - 1}, {OPERATOR_TOKEN, .operator= i } };
    //         return res;
    //     }
    // }
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        Operator currOperator = operators[i];
        uint8_t operatorSize = strlen(currOperator.identifier);
        if (data.len >= operatorSize
            && strncmp(data.data, currOperator.identifier, operatorSize) == 0) {
            TokenResult res = {1, {data.data + operatorSize, data.len - operatorSize}, {OPERATOR_TOKEN, .operator= {.index = i, .priority = currOperator.priority} } };
            return res;
        }
    }

    return errorResult;
}

TokenResult TokenizeParanthesis(Data data) {
    TokenResult errorResult = {0, data};

    if (data.len == 0) {
        return errorResult;
    }

    if (data.data[0] == '(') {
        TokenResult res = {1, {data.data + 1, data.len - 1}, {LEFT_PARANTHESIS_TOKEN, .operator= {.index=0, .priority=0} } };
        return res;
    } else if (data.data[0] == ')') {
        TokenResult res = {1, {data.data + 1, data.len - 1}, {RIGHT_PARANTHESIS_TOKEN, .operator= {.index=1, .priority=0} } };
        return res;
    }

    return errorResult;
}

TokenResult Tokenize(Data data) {
    TokenResult t = TokenizeParanthesis(data);
    if (!t.success) {
        t = TokenizeOperator(data);
        if (!t.success) {
            t = TokenizeNumeric(data);
            if (!t.success) t = TokenizeIdentifier(data);
        }
    }
    return t;
}

typedef StackType(OperandStack, Expression *);
// typedef StackType(OperatorStack, u_int8_t);
typedef StackType(OperatorStack, OperatorTokenData);

bool Po(OperatorStack *operatorStack, OperandStack *operandStack) {
    if (operandStack->index <= 1)  // TODO special case unary
    {
        printf("Not enough parameters for operator\n");
        return 0;
    }
    OperatorTokenData operator= {};
    OperatorStackPop(operatorStack, &operator);
    Expression *b;
    OperandStackPop(operandStack, &b);
    Expression *a;
    OperandStackPop(operandStack, &a);
    BinaryExpression *res = NewBinaryExpression(a, operator, b);
    OperandStackPush(operandStack, &res->Expression);
    return 1;
}

#if defined INTEGER_CALC_ENABLE && defined FLOAT_CALC_ENABLE
#    define ValueFromExpr(_input) (_input.valueType == INTEGER ? (myfloat)_input.intresult : _input.floatresult)
#    define EVAL_RESULT(type, value)                          \
        ({                                                    \
            EvaluationResult res = {type, .intresult = 0, 0}; \
            ((type) == INTEGER) { res.intresult = value; }    \
            else {                                            \
                res.floatresult = value;                      \
            }                                                 \
            res;                                              \
        })
#elif defined INTEGER_CALC_ENABLE
#    define ValueFromExpr(_input) (_input.intresult)
#    define EVAL_RESULT(type, value)                              \
        ({                                                        \
            EvaluationResult res = {type, .intresult = value, 0}; \
            res;                                                  \
        })
#elif defined FLOAT_CALC_ENABLE
#    define ValueFromExpr(_input) (_input.floatresult)
#    define EVAL_RESULT(type, value)                                \
        ({                                                          \
            EvaluationResult res = {type, .floatresult = value, 0}; \
            res;                                                    \
        })
#endif

EvaluationResult EvaluateExpression(Expression *expression) {
    switch (expression->expressionType) {
        case VALUE_EXPRESSION: {
            ValueExpression *expr = (ValueExpression *)expression;
#if defined INTEGER_CALC_ENABLE && defined FLOAT_CALC_ENABLE
            if (expr->valueType == INTEGER)
                return (EvaluationResult){INTEGER, .intresult = expr->integer, 0};
            else

                if (expr->valueType == FLOAT) {
                return (EvaluationResult){FLOAT, .floatresult = expr->floating, 0};
            }
#elif defined INTEGER_CALC_ENABLE
            if (expr->valueType == INTEGER) return (EvaluationResult){INTEGER, .intresult = expr->integer, 0};
#elif defined FLOAT_CALC_ENABLE
            if (expr->valueType == FLOAT) {
                return (EvaluationResult){FLOAT, .floatresult = expr->floating, 0};
            }
#endif
            else
                return (EvaluationResult){FLOAT, .nonresult = 0, 2};
        }
        case BINARY_EXPRESSION: {
            BinaryExpression *binExpr = (BinaryExpression *)expression;
            EvaluationResult  resa    = EvaluateExpression(binExpr->a);
            if (resa.errorcode) return (EvaluationResult){0, .nonresult = 0, resa.errorcode};
            EvaluationResult resb = EvaluateExpression(binExpr->b);
            if (resb.errorcode) return (EvaluationResult){0, .nonresult = 0, resb.errorcode};

#if defined INTEGER_CALC_ENABLE && defined FLOAT_CALC_ENABLE
            if (resa.valueType | resb.valueType) {
                return float_operator_functions[binExpr->operator](ValueFromExpr(resa), ValueFromExpr(resb));
            } else {
                return int_operator_functions[binExpr->operator]( resa.intresult,  resb.intresult);
            }
#elif defined INTEGER_CALC_ENABLE
                return int_operator_functions[binExpr->operator]( resa.intresult,  resb.intresult);
#elif defined FLOAT_CALC_ENABLE
                return float_operator_functions[binExpr->operator]( ValueFromExpr(resa),  ValueFromExpr(resb));
#endif
            /*switch (operator_indices[binExpr->operator])
            {
                case '+':
                    return EVAL_RESULT(resa.valueType | resb.valueType, ValueFromExpr(resa) + ValueFromExpr(resb));
                case '-':
                    return EVAL_RESULT(resa.valueType | resb.valueType, ValueFromExpr(resa) - ValueFromExpr(resb));
                case '*':
                    return EVAL_RESULT(resa.valueType | resb.valueType, ValueFromExpr(resa) * ValueFromExpr(resb));
                case '/':
                    if ((resb.valueType == INTEGER && resb.intresult == 0) || (resb.valueType == FLOAT && resb.floatresult == 0.0)) return (EvaluationResult){0, .intresult = 1, 2};
                    return EVAL_RESULT(resa.valueType | resb.valueType, ValueFromExpr(resa) / ValueFromExpr(resb));
            }*/

        } break;
        default:
            // send_string_with_delay("DEFExpr\n", 1);(15+5)*5=100

            break;
    }

    send_string_with_delay("ERRExpr\n", 1);
    return (EvaluationResult){0, .nonresult = 0, 2};
}

typedef StackType(InputCharsStack, char);
InputCharsStack  input          = {};
EvaluationResult lastCalcResult = {};
bool             shiftPressed   = false;

bool process_calc_input(uint16_t keycode, keyrecord_t *record) {
    bool printResult = false;
    if (keycode == KC_LSPO || keycode == KC_RSPC) {
        shiftPressed = record->event.pressed;
    }
    if (calcMode && record->event.pressed) {
        if (keycode >= KC_KP_1 && keycode <= KC_KP_0) {
            InputCharsStackPush(&input, ((keycode + 2) % 10) + '0');
        } else {
            switch (keycode) {
                case KC_PSLS:
                    InputCharsStackPush(&input, '/');
                    break;
                case KC_PAST:
                    // if (input.index > 1 && input.data[input.index - 1] == '*') {
                    //     char _g;
                    //     InputCharsStackPop(&input, &_g);
                    //     InputCharsStackPush(&input, '#');
                    // } else
                    InputCharsStackPush(&input, '*');

                    break;
                case KC_PMNS:
                    InputCharsStackPush(&input, '-');
                    break;
                case KC_PPLS:
                    InputCharsStackPush(&input, '+');
                    break;
                case KC_PENT:
                    tap_code16(S(KC_0));
                    printResult = true;
                    break;
                case KC_PDOT:
                case KC_BTNM:
                    InputCharsStackPush(&input, '.');
                    break;
                case KC_8:
                    if (shiftPressed) {
                        InputCharsStackPush(&input, '(');
                    }
                    return false;
                    break;
                case KC_9:
                    if (shiftPressed) {
                        InputCharsStackPush(&input, ')');
                    }
                    return false;
                    break;
                case KC_BACKSPACE: {
                    char garbage;
                    InputCharsStackPop(&input, &garbage);
                    // if (garbage == '#') InputCharsStackPush(&input, '*');
                    break;
                }
                case KC_NUBS:{
                    if(shiftPressed){
                        InputCharsStackPush(&input, '>');
                    }
                    else{
                        InputCharsStackPush(&input, '<');
                    }
                }
            }
        }
    }
    if (record->event.pressed && keycode == MYCALC) {
        calcMode = !calcMode;
        if (calcMode) {
            if (input.data) {
                free(input.data);
                input.data = NULL;
            }
            input = NewInputCharsStack(8);
        } else {
            printResult = true;
        }
    }

    if (printResult) {
        printResult = !printResult;
        char *s     = input.data;
        Data  d     = {s, input.index};

        OperandStack  operandStack  = NewOperandStack(4);
        OperatorStack operatorStack = NewOperatorStack(4);
        TokenResult   t             = {0, d, {}};
        // char         *temp_buffer_input = (char *)malloc(input.index + 2);
        // memset(temp_buffer_input, '\0', input.index + 2);
        // memcpy(temp_buffer_input, s, input.index);
        // temp_buffer_input[input.index] = '\n';
        // send_string_with_delay(temp_buffer_input, 1);
        // free(temp_buffer_input);
        bool res = true;
        do {
            t = Tokenize(t.remainder);
            switch (t.token.tokenType) {
                case INVALID_TOKEN:
                    res = false;
                    break;
#ifdef INTEGER_CALC_ENABLE
                case INTEGER_TOKEN:
                    OperandStackPush(&operandStack, &(NewValueI(t.token.integer)->Expression));
                    break;
#endif
#ifdef FLOAT_CALC_ENABLE
                case FLOAT_TOKEN:
                    OperandStackPush(&operandStack, &(NewValueF(t.token.floating)->Expression));
                    break;
#endif
                case OPERATOR_TOKEN:
                    // while(operatorStack.index > 0
                    //     && operator_prios[operatorStack.data[operatorStack.index-1]] >= operator_prios[t.token.operator]
                    //     && res)
                     while(operatorStack.index > 0
                        && operatorStack.data[operatorStack.index-1].priority >= t.token.operator.priority
                        && res)
                    {
                        // Pop operator and push to operand via expression
                        res &= Po(&operatorStack, &operandStack);
                    }
                    if (res) OperatorStackPush(&operatorStack, t.token.operator);
                    break;
                case LEFT_PARANTHESIS_TOKEN:
                    OperatorStackPush(&operatorStack, t.token.operator);
                    break;
                case RIGHT_PARANTHESIS_TOKEN:
                    // while (operatorStack.index > 0 && operatorStack.data[operatorStack.index - 1] != 0 && res) {
                    while (operatorStack.index > 0 && operatorStack.data[operatorStack.index - 1].index != 0 && res) {
                        // Pop operator and push to operand via expression
                        res &= Po(&operatorStack, &operandStack);
                    }
                    if (res) {
                        OperatorTokenData _discard;
                        OperatorStackPop(&operatorStack, &_discard);
                    }

                    break;
                case IDENTIFIER_TOKEN:
                    break;
            }
        } while (t.success && t.remainder.len > 0 && res);
        while (operatorStack.index > 0 && res) {
            // assert the operator on top of the stack is not a (left) parenthesis
            res &= Po(&operatorStack, &operandStack);
        }
        if (res) {
            lastCalcResult = EvaluateExpression(operandStack.data[0]);
            char buffer[12];
            memset(buffer, 0, 12);
            if (lastCalcResult.errorcode) {
                send_string_with_delay("Invalid Expression", 1);
            } else {
#ifdef INTEGER_CALC_ENABLE
                if (lastCalcResult.valueType == INTEGER) {
                    itoa(lastCalcResult.intresult, buffer, 10);
                }
#endif
#ifdef FLOAT_CALC_ENABLE
                else if (lastCalcResult.valueType == FLOAT) {
                    itoa((int)lastCalcResult.floatresult, buffer, 10);
                }
#endif
                if (buffer[0] == '-') {
                    tap_code(KC_KP_MINUS);
                    send_string_with_delay(&buffer[1], 1);
                } else
                    send_string_with_delay(buffer, 1);
#ifdef FLOAT_CALC_ENABLE
                if (lastCalcResult.valueType == FLOAT) {
                    memset(buffer, 0, 12);
                    int decimalValues = (myfloatabs(lastCalcResult.floatresult - (int)lastCalcResult.floatresult) * 1000000000);
                    if (decimalValues != 0) {
                        tap_code(KC_KP_DOT);
                        itoa((myfloatabs(lastCalcResult.floatresult - (int)lastCalcResult.floatresult) * 1000000000) + 1000000000, buffer, 10);
                        send_string_with_delay(&buffer[1], 1);
                    }
                }
#endif
            }
        }

        if (calcMode) {
            if (input.data) {
                free(input.data);
                input.data = NULL;
            }
            input = NewInputCharsStack(8);
        }
        free(operandStack.data);
        free(operatorStack.data);
    }
    return true;
}
