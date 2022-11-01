#ifndef MY_HEADER_FILE_H
#define MY_HEADER_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define stack_ctor(stk, size) stack_ctor_((stk), (size), #stk, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define stack_dump(stk, error_number) stack_dump_((stk), (error_number), __PRETTY_FUNCTION__, __FILE__, __LINE__)

typedef double elem;

struct stack
{
    elem * data;
    size_t size;
    size_t capacity;
    const char * name;
    const char * func;
    const char * file;
    int line;
};

enum Errors
{
    NULL_DATA = 1,
    SIZE_ERROR = 2,
    CAP_ERROR = 4,
    SIZE_CAP_ERROR = 8,
};

extern FILE * log_file;

int stack_ctor_(stack * stk, size_t capacity, const char * name,
                const char * func, const char * file, int line);
int stack_verify(stack * stk);
void stack_push(stack * stk, elem value);
void stack_pop(stack * stk, elem * value);
void stack_resize(stack * stk, size_t new_size);
void error_num_translate(int error_number);
int power_two(int p);
void write_error_to_log(char * error_string);
void stack_dump_(stack * stk, int error_number, const char * func, const char * file, int line);
void write_stack_info(stack * stack);

#endif