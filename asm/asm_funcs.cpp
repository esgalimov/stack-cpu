#include "asm_header.h"

void run_comp(FILE * stream)
{
    s_asm asem = {};
    asm_ctor(&asem, stream);

    size_t i = 0, i_code = 0;

    while (i < asem.commands.len)
    {
        char cmd[20] = "";
        asem.len_cmd = 0;
        asem.len_cmd_gap = 0;
        sscanf(asem.commands.strings[i], "%s%n", cmd, &asem.len_cmd);

        if (is_without_text(cmd))
        {
            i++;
            continue;
        }

        while (asem.len_cmd > 0)
        {
            asem.toks[i_code].name = (char *) calloc(20, sizeof(char));

            if (strcmp(cmd, "push") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = PUSH;

            }
            else if (strcmp(cmd, "add") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = ADD;
            }
            else if (strcmp(cmd, "sub") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = SUB;
            }
            else if (strcmp(cmd, "div") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = DIV;
            }
            else if (strcmp(cmd, "mul") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = MUL;
            }
            else if (strcmp(cmd, "pop") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = POP;
            }
            else if (strcmp(cmd, "out") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = OUT;
            }
            else if (strcmp(cmd, "hlt") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = HLT;
            }
            else if (strcmp(cmd, "jmp") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JMP;
            }
            else if (strcmp(cmd, "jb") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JB;
            }
            else if (strcmp(cmd, "jbe") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JBE;
            }
            else if (strcmp(cmd, "ja") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JA;
            }
            else if (strcmp(cmd, "jae") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JAE;
            }
            else if (strcmp(cmd, "je") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JE;
            }
            else if (strcmp(cmd, "jne") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = JNE;
            }
            else if (strcmp(cmd, "call") == 0)
            {
                asem.toks[i_code].type = CMD1;
                asem.toks[i_code].value = CALL;
            }
            else if (strcmp(cmd, "ret") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = RET;
            }
            else if (strcmp(cmd, "in") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = IN;
            }
            else if (str_of_digits(cmd))
            {
                double value = 0;
                sscanf(cmd, "%lf", &value);

                asem.toks[i_code].type = NUM;
                asem.toks[i_code].value = int (value * ACCURACY);
            }
            else if (is_label(cmd))
            {
                int label = 0;
                sscanf(cmd + 1, "%d", &label);

                asem.toks[i_code].type = LABEL;
                asem.toks[i_code].value = label;
            }
            else if (strcmp(cmd, "ax") == 0)
            {
                asem.toks[i_code].type = REG;
                asem.toks[i_code].value = AX;
            }
            else if (strcmp(cmd, "bx") == 0)
            {
                asem.toks[i_code].type = REG;
                asem.toks[i_code].value = BX;
            }
            else if (strcmp(cmd, "cx") == 0)
            {
                asem.toks[i_code].type = REG;
                asem.toks[i_code].value = CX;
            }
            else if (strcmp(cmd, "dx") == 0)
            {
                asem.toks[i_code].type = REG;
                asem.toks[i_code].value = DX;
            }
            else if (strcmp(cmd, "sqrt") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = SQRT;
            }
            else if (strcmp(cmd, "noroots") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = NOROOTS;
            }
            else if (strcmp(cmd, "allnum") == 0)
            {
                asem.toks[i_code].type = CMD0;
                asem.toks[i_code].value = ALLNUM;
            }
            else
            {
                asem.toks[i_code].type = UNDEFIND;
            }

            strcpy(asem.toks[i_code].name, cmd);
            asem.toks[i_code].line = (int) (i + 1);
            i_code++;

            asem.len_cmd_gap += asem.len_cmd;
            asem.len_cmd = 0;

            sscanf(asem.commands.strings[i] + asem.len_cmd_gap, "%s%n", cmd, &asem.len_cmd);

            check_toks_for_size(&asem, i_code);
        }
        i++;
    }

    labels_init(&asem, i_code);

    if (check_code(&asem, i_code))
    {
        make_label_jmp_push_reg(&asem, i_code);

        write_code_to_file(asem.toks, i_code);
        printf("Compiled OK\n");
    }

    asm_dtor(&asem);
}

void write_code_to_file(token * toks, size_t n_cmd)
{
    assert(toks != NULL);

    FILE * fp = fopen("../code/test.code", "w");
    FILE * fp_bin = fopen("../code/test.bin", "wb");

    if (fp == NULL || fp_bin == NULL)
    {
        printf("Can't open file");
        abort();
    }

    elem * code = (elem *) calloc(n_cmd, sizeof(elem));
    size_t label_found = 0;

    for (size_t i = 0; i < n_cmd; i++)
    {
        if (toks[i].type == LABEL)
        {
            label_found++;
        }
        else
        {
            code[i - label_found] = toks[i].value;
            fprintf(fp, "%d ", toks[i].value);
        }

    }

    fwrite(code, sizeof(elem), n_cmd, fp_bin);

    fclose(fp);
    fclose(fp_bin);

    free(code);
}

int check_code(s_asm * asem, size_t n_cmd)
{
    assert(asem != NULL);

    int is_hlt = 0, is_ok = 1;

    for (size_t i = 0; i < n_cmd; i++)
    {
        if (asem->toks[i].type == UNDEFIND)
        {
            printf("Error: command %s not found at line %d\n", asem->toks[i].name, asem->toks[i].line);
            is_ok = 0;
        }

        else if (asem->toks[i].type == CMD1 && asem->toks[i].value == PUSH)
        {
            if ((i + 1 < n_cmd && (asem->toks[i + 1].type != NUM && asem->toks[i + 1].type != REG)) || (i + 1 == n_cmd))
            {
                printf("Error: invalid syntax at line %d: %s has not given an argument, but it must have 1 argument\n",
                        asem->toks[i].line, asem->toks[i].name);
                is_ok = 0;
            }

            else if (i + 2 < n_cmd && ((asem->toks[i + 1].type == NUM && asem->toks[i + 2].type == NUM) || (asem->toks[i + 1].type == REG && asem->toks[i + 2].type == REG)))
            {
                printf("Error: invalid syntax at line %d: %s has given more than 1 argument, but it must have 1 argument\n",
                        asem->toks[i].line, asem->toks[i].name);
                is_ok = 0;
            }
        }

        else if (asem->toks[i].type == CMD1 && check_for_jump(asem->toks[i].value))
        {
            if ((i + 1 < n_cmd && asem->toks[i + 1].type != LABEL) || (i + 1) == n_cmd)
            {
                printf("Error: invalid syntax at line %d: %s has not given an argument, but it must have 1 argument\n",
                        asem->toks[i].line, asem->toks[i].name);
                is_ok = 0;
            }

            else
            {
                if (asem->labels[asem->toks[i + 1].value].value == -1)
                {
                    printf("Error: label doesn't exist at line %d\n", asem->toks[i].line);
                    is_ok = 0;
                }
            }
        }

        else if (asem->toks[i].type == LABEL)
        {
            if (asem->labels[asem->toks[i].value].cnt > 1)
            {
                printf("Error: there are more than one label to jump at line %d\n", asem->toks[i].line);
                is_ok = 0;
            }
        }

        else if (asem->toks[i].type == CMD0)
        {
            if (i + 1 < n_cmd && asem->toks[i + 1].type == NUM)
            {
                printf("Error: invalid syntax at line %d: %s must not have arguments \n",
                        asem->toks[i].line, asem->toks[i].name);
                is_ok = 0;
            }

            else if (i + 2 < n_cmd && asem->toks[i].value == POP && asem->toks[i + 1].type == REG && asem->toks[i + 2].type == REG)
            {
                printf("Error: invalid syntax at line %d: %s can not have more than one register as argument \n",
                    asem->toks[i].line, asem->toks[i].name);
            }

            else if (asem->toks[i].value == HLT)
            {
                is_hlt = 1;
            }
        }
    }

    if (is_hlt == 0)
    {
        printf("Error: there is no hlt command in programm\n");
        is_ok = 0;
    }

    return is_ok;
}


int str_of_digits(const char * cmd)
{
    size_t len_of_cmd = strlen(cmd);
    size_t i = 0;

    if (cmd[0] == '-')
        i = 1;

    for (; i < len_of_cmd; i++)
    {
        if (isdigit(cmd[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}

int is_label(const char * cmd)
{
    if (cmd[0] != ':')
    {
        return 0;
    }
    else
    {
        if (str_of_digits(cmd + 1))
        {
            return 1;
        }
        return 0;
    }
}

void check_toks_for_size(s_asm * asem, size_t i_code)
{
    if (asem->size_toks <= i_code)
    {
        asem->size_toks *= 2;
        asem->toks = (token *) realloc(asem->toks, asem->size_toks * sizeof(elem));
    }
}

void asm_ctor(s_asm * asem, FILE * stream)
{
    asem->commands = {};

    construct(&asem->commands, stream);

    asem->toks = (token *) calloc(asem->commands.len * 2, sizeof(token));

    asem->labels = (label *) calloc(N_LABELS, sizeof(label));

    for (int i = 0; i < N_LABELS; i++)
        asem->labels[i].value = -1;

    asem->size_toks = asem->commands.len * 2;

    asem->len_cmd = 0;
    asem->len_cmd_gap = 0;
}

void asm_dtor(s_asm * asem)
{
    assert(asem != NULL);

    destruct(&asem->commands);

    asem->len_cmd = 0;
    asem->len_cmd_gap = 0;
    asem->size_toks = 0;

    free(asem->labels);
    free(asem->toks);
    asem->labels = NULL;
    asem->toks = NULL;

}

void labels_init(s_asm * asem, size_t n_toks)
{
    size_t label_found = 0;

    if (asem->toks[0].type == LABEL)
    {
        asem->labels[asem->toks[0].value].value = 0;
        asem->labels[asem->toks[0].value].cnt++;
        label_found++;
    }

    size_t i = 0;

    for (i = 1; i < n_toks; i++)
    {
        if (asem->toks[i].type == LABEL)
        {
            if (asem->toks[i - 1].type == CMD1 && check_for_jump(asem->toks[i - 1].value))
                continue;

            asem->labels[asem->toks[i].value].value = (int) (i - label_found);
            asem->labels[asem->toks[i].value].cnt++;
            label_found++;
        }
    }
}

void make_label_jmp_push_reg(s_asm * asem, size_t n_toks)
{
    for (size_t i = 0; i < n_toks; i++)
    {
        if (asem->toks[i].type == CMD1 && check_for_jump(asem->toks[i].value))
        {
            asem->toks[i + 1].type = LABEL_JMP;
            asem->toks[i + 1].value = asem->labels[asem->toks[i + 1].value].value;
        }

        if (asem->toks[i].type == CMD1 && asem->toks[i].value == PUSH && asem->toks[i + 1].type == REG)
        {
            asem->toks[i].value = PUSH_REG;
        }

        if (asem->toks[i].type == CMD0 && asem->toks[i].value == POP && asem->toks[i + 1].type == REG)
        {
            asem->toks[i].value = POP_REG;
            asem->toks[i].type = CMD1;
        }
    }
}

int check_for_jump(int value)
{
    return (value >= JMP && value <= CALL);
}
