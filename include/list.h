#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "boolean.h"

typedef struct node
{
    void *data;
    struct node *next;
    struct node *previous;

} node_t;

typedef struct
{
    node_t *head;
    node_t *tail;
    int size;

} list_t;

node_t *node_new(void *data)
{
    node_t *node;

    node = (node_t *) malloc (sizeof(node_t));
    node->data = data;
    node->next = node->previous = NULL;

    return(node);
}

list_t *list_new()
{
    list_t *list;

    list = (list_t *) malloc(sizeof(list_t));
    list->head = list->tail = NULL;
    list->size = 0;

    return(list);
}

bool_t list_empty(list_t *list)
{
    return(list->size == 0);
}

void list_add_head(list_t *list, void *data)
{
    node_t *node;

    node = node_new(data);
    if(list_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->next = list->head;
        list->head->previous = node;
        list->head = node;
    }
    list->size++;
}

void list_add_tail(list_t *list, void *data)
{
    node_t *node;

    node = node_new(data);
    if(list_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->previous = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

void list_add(list_t *list, node_t *node, void *data)
{
    node_t *new_node;

    list->size++;

    if(list->head == node)
    {
        list_add_head(list, data);
        return;
    }

    if(list->tail == node)
    {
        list_add_tail(list, data);
        return;
    }

    new_node = node_new(data);
    node->next->previous = new_node;
    new_node->next = node->next;
    node->next = new_node;
    new_node->previous = node;
}

void *list_remove_head(list_t *list)
{
    void *data;
    node_t *node;

    if(!list_empty(list))
    {
        node = list->head;
        data = node->data;
        if(node->next == NULL)
        {
            list->head = list->tail = NULL;
        }
        else
        {
            list->head = node->next;
            list->head->previous = NULL;
        }
        list->size--;
        free(node);
        return(data);
    }

    return (NULL);
}

void *list_remove_tail(list_t *list)
{
    void *data;
    node_t *node;

    if(!list_empty(list))
    {
        node = list->tail;
        data = node->data;
        if(node->previous == NULL)
        {
            list->head = list->tail = NULL;
        }
        else
        {
            list->tail = node->previous;
            list->tail->next = NULL;
        }
        list->size--;
        free(node);
        return(data);
    }

    return (NULL);
}

void *list_remove_node(list_t *list, node_t *node)
{
    void *data;
    node_t *node_index;

    if(list->head == node)
    {
        return(list_remove_head(list));
    }

    if(list->tail == node)
    {
        return(list_remove_tail(list));
    }

    node_index = list->head;
    while(node_index != NULL)
    {
        if(node_index == node)
        {
            data = node->data;
            node->previous->next = node->next;
            node->next->previous = node->previous;
            list->size--;
            free(node);
            return(data);
        }
        node_index = node_index->next;
    }

    return(NULL);
}

void *list_remove_data(list_t *list, void *data)
{
    node_t *node;

    if(list->head->data == data)
    {
        return(list_remove_head(list));
    }

    if(list->tail->data == data)
    {
        return(list_remove_tail(list));
    }

    node = list->head;
    while(node != NULL)
    {
        if(node->data == data)
        {
            node->previous->next = node->next;
            node->next->previous = node->previous;
            list->size--;
            free(node);
            return(data);
        }
        node = node->next;
    }

    return(NULL);
}
