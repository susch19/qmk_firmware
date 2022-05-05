#pragma once

#include <stddef.h>

#define StackType(StackTypeName, TypeName) \
struct\
{\
    TypeName* data;\
    size_t size, index;\
} StackTypeName;\
\
StackTypeName New ## StackTypeName(int initialSize)\
{\
    StackTypeName res = {\
            .data = (TypeName*)malloc(sizeof(TypeName) * initialSize),\
            .size = initialSize,\
            .index = 0,\
        };\
    return res;\
}\
\
void StackTypeName ## ResizeStack(StackTypeName* stack, int newSize)\
{\
    StackTypeName newStack = New ## StackTypeName(newSize);\
    for (int i = 0;i < stack->index;i++)\
        newStack.data[i] = stack->data[i];\
    free(stack->data);\
    stack->data = newStack.data;\
    stack->size = newStack.size;\
}\
\
bool StackTypeName ## Push(StackTypeName* stack, TypeName operator)\
{\
    if (stack->index == stack->size)\
    {\
        StackTypeName ## ResizeStack(stack, stack->size * 2);\
    }\
    stack->data[stack->index++] = operator;\
    return true;\
}\
bool StackTypeName ## Pop(StackTypeName* stack, TypeName* operator)\
{\
    if (stack->index == 0)\
        return false;\
    if (stack->index == stack->size / 4 && stack->index > 2)\
    {\
        StackTypeName ## ResizeStack(stack, stack->size / 2);\
    }\
    *operator = stack->data[--stack->index];\
    return true;\
}
