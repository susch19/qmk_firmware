
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "stack.h"

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
        int   integer;
        float floating;
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
    INTEGER_TOKEN,
    FLOAT_TOKEN,
    OPERATOR_TOKEN,
    IDENTIFIER_TOKEN,
};

typedef struct {
    int tokenType;
    union {
        int      integer;
        float    floating;
        u_int8_t operator;
        Data     identifier;
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
        int   intresult;
        double floatresult;
    };
    u_int8_t errorcode;
} EvaluationResult;

ValueExpression *NewValueI(int value) {
    ValueExpression *v = (ValueExpression *)malloc(sizeof(ValueExpression));
    v->valueType       = INTEGER;
    v->integer         = value;
    v->expressionType  = VALUE_EXPRESSION;
    return v;
}
ValueExpression *NewValueF(double value) {
    ValueExpression *v = (ValueExpression *)malloc(sizeof(ValueExpression));
    v->valueType       = FLOAT;
    v->floating        = value;
    v->expressionType  = VALUE_EXPRESSION;
    return v;
}

BinaryExpression *NewBinaryExpression(Expression *a, u_int8_t operator, Expression * b) {
    BinaryExpression *binExpr = (BinaryExpression *)malloc(sizeof(BinaryExpression));
    binExpr->expressionType   = BINARY_EXPRESSION;
    binExpr->a                = a;
    binExpr->operator         = operator;
    binExpr->b                = b;
    return binExpr;
}

#define TokenizeCharConditional(_input, _condition)  \
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

#define TOKEN_DIGIT_CONDITION (currentChar >= '0' && currentChar <= '9')
#define TOKEN_LETTER_CONDITION ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z'))
#define TOKEN_IDENTIFIER_CHAR_CONDITION (TOKEN_DIGIT_CONDITION || TOKEN_LETTER_CONDITION)

TokenResult TokenizeChar(Data data, char tokenChar) { return TokenizeCharConditional(data, currentChar == tokenChar); }

TokenResult TokenizeLetter(Data data) { return TokenizeCharConditional(data, TOKEN_LETTER_CONDITION); }

TokenResult TokenizeDigit(Data data) {
    TokenResult res = TokenizeCharConditional(data, TOKEN_DIGIT_CONDITION);
    if (res.success) {
        res.token.integer -= '0';
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
        value += digit.token.integer;
        data = digit.remainder;
        offset++;
    } while (true);

    if (offset > 0) {
        current.success         = 1;
        current.token.tokenType = INTEGER_TOKEN;
        current.token.integer   = value;
        current.remainder       = data;
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
        number.token.integer *= (negChar.success ? -1 : 1);
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
    if (integer.success) {
        TokenResult decimalSepChar = TokenizeCharConditional(integer.remainder, currentChar == '.' || currentChar == ',');
        if (!decimalSepChar.success) return integer;

        TokenResult decimalRest = TokenizeNatural(decimalSepChar.remainder);
        int         restVal     = decimalRest.success ? decimalRest.token.integer : 0;
        TokenResult res         = {1, decimalRest.remainder, {FLOAT_TOKEN, .floating = (integer.token.integer + restVal / (double)digitDivisor(restVal))}};

        return res;
    }
    return integer;
}



EvaluationResult int_add(int a, int b)
{
    return (EvaluationResult) { INTEGER, .intresult = a + b, 0 };
}
EvaluationResult int_sub(int a, int b)
{
    return (EvaluationResult) { INTEGER, .intresult = a - b, 0 };
}
EvaluationResult int_mul(int a, int b)
{
    return (EvaluationResult) { INTEGER, .intresult = a * b, 0 };
}
EvaluationResult int_div(int a, int b)
{
    if (b == 0)
        return (EvaluationResult) { INTEGER, .intresult = 0, 1 };
    return (EvaluationResult) { INTEGER, .intresult = a / b, 0 };
}

EvaluationResult float_add(double a, double b)
{
    return (EvaluationResult) { FLOAT, .floatresult = a + b, 0 };
}
EvaluationResult float_sub(double a, double b)
{
    return (EvaluationResult) { FLOAT, .floatresult = a - b, 0 };
}
EvaluationResult float_mul(double a, double b)
{
    return (EvaluationResult) { FLOAT, .floatresult = a * b, 0 };
}
EvaluationResult float_div(double a, double b)
{
    if (b == 0)
        return (EvaluationResult) { FLOAT, .floatresult = 0, 1 };
    return (EvaluationResult) { FLOAT, .floatresult = a / b, 0 };
}

const char    *operator_indices = "*/+-";
const u_int8_t operator_prios[] = {1, 1, 0, 0};
typedef EvaluationResult (*BinaryOperatorFunctionFloat)(double, double);
typedef EvaluationResult (*BinaryOperatorFunctionInt)(int, int);
BinaryOperatorFunctionInt int_operator_functions[] = {int_mul, int_div, int_add, int_sub};
BinaryOperatorFunctionFloat float_operator_functions[] = {float_mul, float_div, float_add, float_sub};


TokenResult TokenizeOperator(Data data) {
    TokenResult errorResult = {0, data};

    if (data.len == 0) {
        return errorResult;
    }

    for (int i = 0; i < strlen(operator_indices); i++) {
        if (data.data[0] == operator_indices[i]) {
            TokenResult res = {1, {data.data + 1, data.len - 1}, {OPERATOR_TOKEN, .operator= i } };
            return res;
        }
    }

    return errorResult;
}

TokenResult Tokenize(Data data) {
    TokenResult t = TokenizeOperator(data);
    if (!t.success) {
        t = TokenizeNumeric(data);
        if (!t.success) t = TokenizeIdentifier(data);
    }
    return t;
}

typedef StackType(OperandStack, Expression *);
typedef StackType(OperatorStack, u_int8_t);

bool Po(OperatorStack *operatorStack, OperandStack *operandStack) {
    if (operandStack->index <= 1)  // TODO special case unary
    {
        printf("Not enough parameters for operator\n");
        return 0;
    }
    u_int8_t operator= 0;
    OperatorStackPop(operatorStack, &operator);
    Expression *b;
    OperandStackPop(operandStack, &b);
    Expression *a;
    OperandStackPop(operandStack, &a);
    BinaryExpression *res = NewBinaryExpression(a, operator, b);
    OperandStackPush(operandStack, &res->Expression);
    return 1;
}


#define ValueFromExpr(_input) (_input.valueType == INTEGER ? _input.intresult : _input.floatresult)

#define EVAL_RESULT(type, value)                          \
    ({                                                    \
        EvaluationResult res = {type, .intresult = 0, 0}; \
        if ((type) == INTEGER) {                          \
            res.intresult = value;                        \
        } else {                                          \
            res.floatresult = value;                      \
        }                                                 \
        res;                                              \
    })

EvaluationResult EvaluateExpression(Expression *expression) {
    switch (expression->expressionType) {
        case VALUE_EXPRESSION: {
            ValueExpression *expr = (ValueExpression *)expression;
            if (expr->valueType == INTEGER)
                return (EvaluationResult){INTEGER, .intresult = expr->integer, 0};
            else if (expr->valueType == FLOAT) {
                return (EvaluationResult){FLOAT, .floatresult = expr->floating, 0};
            } else
                return (EvaluationResult){INTEGER, .intresult = expr->integer, 2};
        }
        case BINARY_EXPRESSION: {
            BinaryExpression *binExpr = (BinaryExpression *)expression;
            EvaluationResult  resa    = EvaluateExpression(binExpr->a);
            if (resa.errorcode) return (EvaluationResult){0, .intresult = 0, resa.errorcode};
            EvaluationResult resb = EvaluateExpression(binExpr->b);
            if (resb.errorcode) return (EvaluationResult){0, .intresult = 0, resb.errorcode};

            if (resa.valueType | resb.valueType)
            {
                return float_operator_functions[binExpr->operator]( ValueFromExpr(resa),  ValueFromExpr(resb));
            }
            else
            {
                return int_operator_functions[binExpr->operator]( resa.intresult,  resb.intresult);
            }
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
            break;
    }
    return (EvaluationResult){0, .intresult = 0, 2};
}
