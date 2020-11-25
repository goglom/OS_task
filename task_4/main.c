#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define STRING_BUFFER_SIZE  3

typedef struct list_node
{
    struct list_node* next_node;
	struct list_node* prev_node;
    char* string;
} list_node_t;

typedef struct list 
{
    list_node_t* head;
	list_node_t* tail;
}list_t;

list_node_t* construct_list_node(const char* string)
{
    list_node_t* new_node = calloc(1, sizeof(list_node_t));

    if (new_node == NULL)
    {
        perror("Alloction memmory for new list node error");
        return NULL;
    }
    new_node->string = malloc(strlen(string) + 1);

    if (new_node->string == NULL)
    {
        perror("Alloaction memory for string error");
        free(new_node);
        return NULL;
    }
    strcpy(new_node->string, string);
    return new_node;
}

void list_node_destruct(list_node_t* node) 
{
    free(node->string);
    free(node);
}

void init_list(list_t* list)
{
	list->head = list->tail = NULL;
}

int list_append(list_t* list, const char* string) 
{
    list_node_t* new_node = construct_list_node(string);

    if (new_node == NULL) 
    {
        perror("Creating new node error");
        return -1;
    }
	if (list->tail == NULL) 
	{
		list->tail = list->head  = new_node;
	}
    new_node->next_node = NULL;
	new_node->prev_node = list->head;
	list->head->next_node = new_node;
    list->head = new_node;

	return 0;
}

void pop_front(list_t* list)
{
    list_node_t* poped_node = list->head;

    if (poped_node != list->tail) 
    {
        list->head = poped_node->prev_node;
		list->head->next_node = NULL;
    }
    else
    {
	   list->head = list->tail = NULL;
	}
    list_node_destruct(poped_node);
}

void list_destroy(list_t* list) 
{
    while (list->head != NULL)
    {
	   pop_front(list);
	}
}

void list_print(list_t const* list)
{
	if (list->tail == NULL)
		return;

    list_node_t* current_node = list->tail;

	while (current_node != NULL)
	{
		printf("%s", current_node->string);
		current_node = current_node->next_node;
	}
}

int main()
{
	char string_buffer[STRING_BUFFER_SIZE] = { 0 };	

	if (NULL == fgets(string_buffer, STRING_BUFFER_SIZE, stdin))
    {
		perror("Reading from consloe input error");
		return EXIT_FAILURE;
    }
    list_t console_strings;
	init_list(&console_strings);

    while (string_buffer[0] != '.')
    {
        if (list_append(&console_strings, string_buffer) != 0)
        {
			perror("Error while getting string: ");
            list_destroy(&console_strings);
            return EXIT_FAILURE;
        }
		else if (NULL == fgets(string_buffer, STRING_BUFFER_SIZE, stdin))
			break;
    }
	printf("Inputed strings:\n");
	list_print(&console_strings);
	list_destroy(&console_strings);

    return EXIT_SUCCESS;
}
