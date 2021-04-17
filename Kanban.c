/*
* File: p1.c
* Author: Nuno Martins 99292
* Description: A program to manage tasks using the method Kanban in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DIM_LINHA 100000      /*max line length*/
#define MAX_TAREFAS 10000     /*max number of tasks*/
#define MAX_DESCRICAO 51      /*max description length*/
#define MAX_USERS 50          /*max number of users*/
#define MAX_USER_NAME 21      /*max user length*/
#define MAX_ATIVIDADES 10     /*max number of activities*/
#define MAX_ATIVIDADE_NAME 21 /*max activities length*/

/*function prototypes*/
int lelinha(char s[], int lim);
void check_command(char line[]);

void divide_args_t(char line[]);
void divide_args_l(char line[]);
void divide_args_n(char line[]);
void divide_args_u(char line[]);
void divide_args_m(char line[]);
void divide_args_d(char line[]);
void divide_args_a(char line[]);

void command_q();
void command_t(int duracao, char descricao[]);
void command_l(int ids[], int num_ids);
void command_l_0args();
void command_n(int duracao);
void command_u(char user[]);
void command_u_0args();
void command_m(int id, char user[], char atividade[]);
void command_d(char atividade[]);
void command_a_0args();
void command_a(char s[]);

int command_m_errors(int id, char user[], char atividade[]);

/*global variables*/
int time = 0;                                                                           /*current time*/
int num_tarefas = 0;                                                                    /*number of current tasks*/
int num_atividades = 3;                                                                 /*number of current activities*/
int num_users = 0;                                                                      /*number of current users*/
char atividades[MAX_ATIVIDADES][MAX_ATIVIDADE_NAME] = {"TO DO", "IN PROGRESS", "DONE"}; /*vector of activities*/
char users[MAX_USERS][MAX_USER_NAME] = {'\0'};                                          /*vector of users*/

struct tarefa
{
    int id;
    char descricao[MAX_DESCRICAO];
    char user[MAX_USER_NAME];
    char atividade[MAX_ATIVIDADE_NAME];
    int duracao;
    int inicio;
};

struct tarefa lista_tarefas[MAX_TAREFAS] = {0};      /*vector of tasks*/
struct tarefa lista_tarefas_copy[MAX_TAREFAS] = {0}; /*vector of tasks, which will be used as a copy*/

/* Reads the input line with 'lelinha' until the input line is empty and runs 'check_command' on every iteration */
int main()
{
    char line[DIM_LINHA] = {'\0'};

    while (lelinha(line, DIM_LINHA) != 0)
    {
        /* transform input into arguments */
        check_command(line);
    }
    return 0;
}

/* Reads the input line and stores it in a string vector that has been passed as an argument */
int lelinha(char s[], int lim)
{
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
    {
        s[i] = c;
    }
    s[i] = '\0';
    return i;
}

/* Reads the command (first character of the line) and calls other functions based on the command. */
void check_command(char line[])
{
    int len_line = strlen(line);

    switch (line[0])
    {
    case 'q':
        command_q();
        break;

    case 't':
        divide_args_t(line);
        break;

    case 'l':
        if (len_line == 1)
        {
            command_l_0args();
        }
        else
        {
            divide_args_l(line);
        }
        break;

    case 'n':
        divide_args_n(line);
        break;

    case 'u':
        if (len_line == 1)
        {
            command_u_0args();
        }
        else
        {
            divide_args_u(line);
        }
        break;

    case 'm':
        divide_args_m(line);
        break;

    case 'd':
        divide_args_d(line);
        break;

    case 'a':
        if (len_line == 1)
        {
            command_a_0args();
        }
        else
        {
            divide_args_a(line);
        }
        break;
    }
}

/* Iterates through the input line to find 2 variables - duracao(int) and
 passes those 2 variables as arguments to another function */
void divide_args_t(char line[])
{
    char descricao[MAX_DESCRICAO] = {'\0'}, command[2] = {'\0'};
    int duracao;

    sscanf(line, "%s %d %[^\n]", command, &duracao, descricao);

    command_t(duracao, descricao);
}

/* Iterates through the input line to find all the ids(int) and put them all in a vector of int 
values and pass the vector as an argument to another function as well as the 
variable num_ids (number of ids that are in the vector) */
void divide_args_l(char line[])
{
    int count = 0, idx = 0, ids[100000] = {0};

    char *token = strtok(line, " ");

    while (token != NULL)
    {
        ids[idx] = atoi(token);
        idx++;
        token = strtok(NULL, " ");
        count++;
    }
    command_l(ids, count);
}

/* Iterates through the input line to find 1 variable - duracao(int), but before that, 
it checks for errors and if there's no errors, pass duracao(int) as 
an argument to another function */
void divide_args_n(char line[])
{
    int duracao;
    char command[2] = {'\0'};
    sscanf(line, "%s %d", command, &duracao);

    /*check for errors*/
    if (duracao < 0)
    {
        printf("invalid time\n");
        return;
    }

    /*if no errors continue:*/
    ;
    command_n(duracao);
}

/* Iterates through the input line to find 1 variable - user(char). After that it checks for errors 
and if there's no errors, pass user(char) as an argument to another function */
void divide_args_u(char line[])
{
    char user[MAX_USER_NAME] = {'\0'}, command[2] = {'\0'};
    int i;
    sscanf(line, "%s %s", command, user);

    /*check for errors*/
    for (i = 0; i < num_users; i++)
    {
        if (strcmp(users[i], user) == 0)
        {
            printf("user already exists\n");
            return;
        }
    }
    if (num_users > 49)
    {
        printf("too many users\n");
        return;
    }

    /*if no errors continue:*/

    command_u(user);
}

/* Iterates through the input line to find 3 variables - id(int), user(char) and atividade(char), 
checks for errors (by calling a helper function 'command_m_errors') and if there's no errors, 
pass those 3 variables as arguments to another function */
void divide_args_m(char line[])
{
    char command[2] = {'\0'}, user[MAX_USER_NAME] = {'\0'}, atividade[MAX_ATIVIDADE_NAME] = {'\0'};
    int id, error;

    sscanf(line, "%s %d %s %[^\n]", command, &id, user, atividade);

    /*if the activity is in TODO and we want to change it to TODO, return and do nothing and we do 
    the same thing if the activity is in DONE and we want to change it to DONE*/
    if ((strcmp(lista_tarefas[id - 1].atividade, "TO DO") == 0 && strcmp(atividade, "TO DO") == 0) ||
        (strcmp(lista_tarefas[id - 1].atividade, "DONE") == 0 && strcmp(atividade, "DONE") == 0))
    {
        return;
    }

    /*check for errors*/
    error = command_m_errors(id, user, atividade);

    /*if no errors continue:*/
    if (error == 0)
    {
        command_m(id, user, atividade);
    }
}

/* Iterates through the input line to find 1 variable - atividade(char), checks for errors 
and if there's no errors, pass atividade(char) as an argument to another function */
void divide_args_d(char line[])
{
    char atividade[MAX_ATIVIDADE_NAME] = {'\0'}, command[2] = {'\0'};
    int i, count = 0;
    sscanf(line, "%s %[^\n]", command, atividade);

    /*check for errors*/
    for (i = 0; i < num_atividades; i++)
    {
        if (strcmp(atividades[i], atividade) == 0)
        {
            count++;
        }
    }
    if (count == 0)
    {
        printf("no such activity\n");
        return;
    }
    /*if no errors continue:*/
    command_d(atividade);
}

/* Iterates through the input line to find 1 variable - atividade(char), checks for errors 
and if there's no errors, pass atividade(char) as an argument to another function */
void divide_args_a(char line[])
{
    char atividade[MAX_ATIVIDADE_NAME] = {'\0'}, command[2] = {'\0'};
    int i, len_atividade;
    sscanf(line, "%s %[^\n]", command, atividade);

    /*check for errors*/
    for (i = 0; i < num_atividades; i++)
    {
        if (strcmp(atividades[i], atividade) == 0)
        {
            printf("duplicate activity\n");
            return;
        }
    }

    len_atividade = strlen(atividade);

    for (i = 0; i < len_atividade; i++)
    {
        if (islower(atividade[i]))
        {
            printf("invalid description\n");
            return;
        }
    }

    if (num_atividades > 9)
    {
        printf("too many activities\n");
        return;
    }

    /*if no errors continue:*/

    command_a(atividade);
}

/*-------------------------------------------------------*/

/* Terminates the program */
void command_q()
{
    exit(0);
}

/* Adds a new task to our lista_tarefas(vector of structures). This function receives 
duracao(int) and descricao(char) as inputs, checks for errors and if there's no errors,
it outputs 'task <id>' where id = num_tarefas (our global variable to keep track 
of the number of tasks)*/
void command_t(int duracao, char descricao[])
{
    int i;
    /*check for errors*/
    if (num_tarefas > 9999)
    {
        printf("too many tasks\n");
        return;
    }

    for (i = 0; i < num_tarefas; i++)
    {
        if (strcmp(lista_tarefas[i].descricao, descricao) == 0)
        {
            printf("duplicate description\n");
            return;
        }
    }

    if (duracao <= 0)
    {
        printf("invalid duration\n");
        return;
    }

    /*if no errors continue:*/
    lista_tarefas[num_tarefas].id = num_tarefas + 1;
    strcpy(lista_tarefas[num_tarefas].descricao, descricao);
    strcpy(lista_tarefas[num_tarefas].atividade, "TO DO");
    lista_tarefas[num_tarefas].duracao = duracao;
    lista_tarefas[num_tarefas].inicio = 0;

    num_tarefas++;

    printf("task %d\n", num_tarefas);
}

/* Prints all the tasks by the order of input of the IDS with the command 'l'. Since all the 
tasks are in ascending order of their index in lista_tarefas(vector of structures) and their 
index = ID-1, we just iterate through 'ids' (vector of IDs) and first we check if the id is 
of a task not initiated (all the IDs of initiated tasks go from 1 to 'num_tarefas'), and then 
if no errors occur, we print the task (ID, atividade, duracao and descricao of the task)*/
void command_l(int ids[], int num_ids)
{
    int i;
    /*start at i=1, because ids[0]=atoi(command) and that's not an ID*/
    for (i = 1; i < num_ids; i++)
    {
        if (ids[i] < 1 || ids[i] > num_tarefas)
        {
            printf("%d: no such task\n", ids[i]);
        }
        else
        {
            printf("%d %s #%d %s\n", lista_tarefas[ids[i] - 1].id, lista_tarefas[ids[i] - 1].atividade,
                   lista_tarefas[ids[i] - 1].duracao, lista_tarefas[ids[i] - 1].descricao);
        }
    }
}

/* Prints all the tasks by alphabetical order of their 'descricao'. For this I use 
INSERTION SORT which runs in a time complexity of O(n^2). This function will use a copy of 
lista_tarefas(vector of structures), so that insertion sort doesn't modify the original vector */
void command_l_0args()
{
    struct tarefa temp;
    int i, j;

    /* creating a copy of lista_tarefas */
    for (i = 0; i < num_tarefas; i++)
    {
        lista_tarefas_copy[i] = lista_tarefas[i];
    }

    /* INSERTION SORT by alphabetical order of the task's 'descricao' */
    for (i = 1; i < num_tarefas; i++)
    {
        j = i - 1;
        while (j >= 0 && strcmp(lista_tarefas_copy[j + 1].descricao, lista_tarefas_copy[j].descricao) < 0)
        {
            temp = lista_tarefas_copy[j + 1];
            lista_tarefas_copy[j + 1] = lista_tarefas_copy[j];
            lista_tarefas_copy[j] = temp;
            j--;
        }
    }

    /* Print all the tasks after being sorted */
    for (i = 0; i < num_tarefas; i++)
    {
        printf("%d %s #%d %s\n", lista_tarefas_copy[i].id, lista_tarefas_copy[i].atividade,
               lista_tarefas_copy[i].duracao, lista_tarefas_copy[i].descricao);
    }
}

/* Advances the global variable 'time' by the value in 
the parameter 'duracao' and prints the new time */
void command_n(int duracao)
{
    time += duracao;
    printf("%d\n", time);
}

/* Adds the user(char), passed as a parameter, to the 'users'(vector of strings) 
and increments the global variable of the user count 'num_users' */
void command_u(char user[])
{
    strcpy(users[num_users], user);
    num_users++;
}

/* Prints all the users by creation date by iterating through 
the global variable 'users'(vector of strings) */
void command_u_0args()
{
    int i;
    for (i = 0; i < num_users; i++)
    {
        printf("%s\n", users[i]);
    }
}

/* Receives 3 parameters - id(int), user(char), atividade(char), prints the 
'gasto' which is the time the task spent since it left the activity 'TO DO' and 
the 'slack' which is the difference between the 'gasto' and the planned time of 
completion ('inicio' of the task). It also updates the task with the 'id' passed 
as a parameter with the new 'user' and the new 'atividade' also passed as parameters */
void command_m(int id, char user[], char atividade[])
{
    int gasto, slack;
    /* if the new 'atividade' is not 'DONE' and 
    the current activity of the task is 'TO DO' we need to start it */
    if (strcmp(atividade, "DONE") != 0)
    {
        if (strcmp(lista_tarefas[id - 1].atividade, "TO DO") == 0)
        {
            lista_tarefas[id - 1].inicio = time;
        }
    }
    else
    {
        /* If we want to update our task from 'TO DO' to 'DONE' then the time 
        spent ('gasto') is 0 */
        if (strcmp(lista_tarefas[id - 1].atividade, "TO DO") == 0)
        {
            lista_tarefas[id - 1].inicio = time;
        }

        gasto = time - lista_tarefas[id - 1].inicio;

        slack = gasto - lista_tarefas[id - 1].duracao;

        printf("duration=%d slack=%d\n", gasto, slack);
    }
    /* update our task with the new 'user' and 'atividade' passed as parameters */
    strcpy(lista_tarefas[id - 1].user, user);
    strcpy(lista_tarefas[id - 1].atividade, atividade);
}

/* Lists all the tasks that have the activity equal to 'atividade' passed as a parameter 
(they get printed by ascending order of their 'inicio' and by alphabetical order of their 
'descricao' is two or more tasks have the same 'inicio') */
void command_d(char atividade[])
{
    struct tarefa temp;
    int i, j;

    /* creating a copy of lista_tarefas */
    for (i = 0; i < num_tarefas; i++)
    {
        lista_tarefas_copy[i] = lista_tarefas[i];
    }

    /* INSERTION SORT by alphabetical order of the task's 'descricao' */
    for (i = 1; i < num_tarefas; i++)
    {
        j = i - 1;
        while (lista_tarefas_copy[j].inicio > lista_tarefas_copy[j + 1].inicio ||
               (lista_tarefas_copy[j].inicio ==
                    lista_tarefas_copy[j + 1].inicio &&
                strcmp(lista_tarefas_copy[j + 1].descricao, lista_tarefas_copy[j].descricao) < 0))
        {
            temp = lista_tarefas_copy[j + 1];
            lista_tarefas_copy[j + 1] = lista_tarefas_copy[j];
            lista_tarefas_copy[j] = temp;
            j--;
        }
    }

    /* In the end we print all the now sorted tasks in lista_tarefas_same */
    for (i = 0; i < num_tarefas; i++)
    {
        if (strcmp(lista_tarefas_copy[i].atividade, atividade) == 0)
        {
            printf("%d %d %s\n", lista_tarefas_copy[i].id,
                   lista_tarefas_copy[i].inicio, lista_tarefas_copy[i].descricao);
        }
    }
}

/* Prints all the activities by creation date by iterating through the 
global variable 'atividades'(vector of strings) */
void command_a_0args()
{
    int i;
    for (i = 0; i < num_atividades; i++)
    {
        printf("%s\n", atividades[i]);
    }
}

/* Adds the atividade(char), passed as a parameter, to the 'atividades'
(vector of strings) and increments the global variable of the atividade count 'num_atividades' */
void command_a(char atividade[])
{
    strcpy(atividades[num_atividades], atividade);
    num_atividades++;
}

/* Checks if there are any errors regarding the parameters, id(int), 
user(char) and atividade(char). If there are, return 1, else return 0 */
int command_m_errors(int id, char user[], char atividade[])
{
    int i, same_id = 0, same_user = 0, same_atividade = 0;

    for (i = 0; i < num_tarefas; i++)
    {
        if (lista_tarefas[i].id == id)
        {
            same_id++;
        }
    }
    for (i = 0; i < num_atividades; i++)
    {
        if (strcmp(atividades[i], atividade) == 0)
        {
            same_atividade++;
        }
    }

    for (i = 0; i < num_users; i++)
    {
        if (strcmp(users[i], user) == 0)
        {
            same_user++;
        }
    }

    if (same_id == 0)
    {
        printf("no such task\n");
        return 1;
    }
    if (strcmp(atividade, "TO DO") == 0)
    {
        printf("task already started\n");
        return 1;
    }
    if (same_user == 0)
    {
        printf("no such user\n");
        return 1;
    }
    if (same_atividade == 0)
    {
        printf("no such activity\n");
        return 1;
    }

    /* if no errors return 0:*/
    return 0;
}
