#include "header.h"

int stack_ctor_(stack * stk, size_t capacity, var_info info)
{
    if (stk == NULL || capacity < 1)
        return 1;


    if (capacity < MIN_CAPACITY)
        capacity = MIN_CAPACITY;

    stk->data = (elem *) calloc(capacity, sizeof(elem));
    stk->capacity = capacity;
    stk->size = 0;

    write_zeros_to_data(stk, 0, stk->capacity);

    stk->info = info;

    int error_number = stack_verify(stk);
    stack_dump(stk, error_number);

    return 0;
}

int stack_verify(stack * stk)
{
    assert(stk != NULL);
    int error_number = 0;
    if (stk->data == NULL)
        error_number += NULL_DATA;
    if (stk->size < 0)
        error_number += SIZE_ERROR;
    if (stk->capacity < 0)
        error_number += CAP_ERROR;
    if (stk->capacity < stk->size)
        error_number += SIZE_CAP_ERROR;
    return error_number;
}

void error_number_translate(int error_number)
{
    int current = 0;
    int current_error = 0;
    while (error_number >= 1)
    {
        if (error_number & 1)
        {
            current_error = power_two(current);

            switch (current_error)
            {
                case NULL_DATA:
                    write_error_to_log("Data have NULL pointer");
                    break;
                case SIZE_ERROR:
                    write_error_to_log("Size is lower than 0");
                    break;
                case CAP_ERROR:
                    write_error_to_log("Capacity is lower than 0");
                    break;
                case SIZE_CAP_ERROR:
                    write_error_to_log("Size bigger than capacity");
                    break;
                case POP_ERROR:
                    write_error_to_log("Try to pop empty stack");
                    break;
                default:
                    write_error_to_log("Unknown error");
                    break;
            }
        }
        current++;
        error_number >>= 1;
    }
}

void write_error_to_log(const char * error_string)
{
    fputs(error_string, log_file);
    fputs("\n", log_file);
}

void stack_push(stack * stk, elem value)
{
    assert(stk != NULL);
    int error_number = stack_verify(stk);
    stack_dump(stk, error_number);
    if (!error_number)
    {
        if (stk->size >= stk->capacity)
            stack_resize(stk, stk->capacity * 2);
        stk->data[stk->size] = value;
        stk->size++;
    }
    stack_dump(stk, stack_verify(stk));
}

void stack_pop(stack * stk, elem * value)
{
    assert(stk != NULL);
    int error_number = stack_verify(stk);
    stack_dump(stk, error_number);
    if (!error_number)
    {
        if (stk->size > 0)
        {
            *value = stk->data[stk->size - 1];
            stk->data[stk->size - 1] = 0;
            stk->size--;
            if (stk->size < stk->capacity / 4 && stk->size > 0)
                stack_resize(stk, stk->capacity / 2);
        }
        else
        {
            error_number += POP_ERROR;
        }
    }
    stack_dump(stk, stack_verify(stk) + error_number);
}

void stack_resize(stack * stk, size_t new_size)
{
    assert(stk != NULL);

    int error_number = stack_verify(stk);

    stack_dump(stk, error_number);

    if (!error_number)
    {
        stk->data = (elem *) realloc(stk->data, new_size * sizeof(elem));
        stk->capacity = new_size;

        write_zeros_to_data(stk, stk->size, stk->capacity);
    }
    stack_dump(stk, stack_verify(stk));
}

int power_two(int p)
{
    int ans = 1;
    for (int i = 0; i < p; i++)
        ans *= 2;
    return ans;
}

void write_zeros_to_data(stack * stk, size_t i_start, size_t i_end)
{
    for (size_t i = i_start; i < i_end; i++)
    {
        stk->data[i] = 0;
    }
}

void stack_dump_(stack * stk, int error_number, const char * func, const char * file, int line)
{
    fprintf(log_file, "%s at %s(%d):\n", func, file, line);
    if (!error_number)
    {
        fprintf(log_file, "Stack %p (OK) \"%s\" at %s at %s(%d):\n",
                stk, stk->info.name, stk->info.func, stk->info.file, stk->info.line);

        fprintf(log_file, "{\n    size     = %lu\n    capacity = %lu\n",
            stk->size, stk->capacity);
        fprintf(log_file, "    data [%p]\n      {\n", stk->data);
        write_stack_elems(stk);
        fprintf(log_file, "      }");
        fprintf(log_file, "\n}\n\n\n");
    }
    else
    {
        fprintf(log_file, "Stack %p (ERROR) \"%s\" at %s at %s(%d):\n",
                stk, stk->info.name, stk->info.func, stk->info.file, stk->info.line);
        error_number_translate(error_number);

        fprintf(log_file, "{\n    size     = %lu\n    capacity = %lu\n",
            stk->size, stk->capacity);
        fprintf(log_file, "    data [%p]", stk->data);
        fprintf(log_file, "\n}\n\n\n");

        fclose(log_file);
        abort();
    }
}

void write_stack_elems(stack * stk)
{
    assert(stk != NULL);
    for (size_t i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size)
            fprintf(log_file, "      *[i] = %d\n", stk->data[i]);
        else
            fprintf(log_file, "       [i] = %d (POISON)\n", stk->data[i]);
    }

}

void stack_dtor(stack * stk)
{
    write_zeros_to_data(stk, 0, stk->capacity);

    free(stk->data);
    stk->data = NULL;
    stk->size = 0;
    stk->capacity = 0;

    fprintf(log_file, "Stack %p \"%s\" at %s at %s(%d): DESTRUCTED\n",
                stk, stk->info.name, stk->info.func, stk->info.file, stk->info.line);
}

