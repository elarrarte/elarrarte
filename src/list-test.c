#include <stdio.h>
#include "../include/posix_exitcodes.h"
#include "../include/list.h"

void print_list(list_t *list)
{
    node_t *node;
    node = list->head;
    while(node != NULL)
    {
        printf("%d\n", *((int *) node->data));
        node = node->next;
    }
}

void print_node_debug(node_t *node)
{
    printf("node : %p\n", node);
    printf("data : %p\n", node->data);
    printf("next : %p\n", node->next);
    printf("prev : %p\n", node->previous);
}

void print_list_debug(list_t *list)
{
    node_t *node;
    printf("head : %p\n", list->head);
    printf("tail : %p\n", list->tail);
    printf("size : %d\n", list->size);
    node = list->head;
    while(node != NULL)
    {
        print_node_debug(node);
        node = node->next;
    }
}

int main()
{
    list_t *list;
    int data[] = {0, 1, 2, 3};

    /*
    list = list_new();
    list_add_head(list, &data[1]);
    list_add_head(list, &data[0]);
    list_add_tail(list, &data[2]);
    list_add_tail(list, &data[3]);
    print_list(list);
    printf("-----------------------------------------------\n");
    list_remove_head(list);
    list_remove_tail(list);
    print_list(list);
    printf("-----------------------------------------------\n");
    list_add_tail(list, &data[3]);
    list_add(list, list->head->next, &data[0]);
    list_add(list, list->head, &data[0]);
    list_add(list, list->tail, &data[0]);
    print_list(list);
    printf("-----------------------------------------------\n");
    list_remove_node(list, list->head);
    list_remove_node(list, list->tail);
    list_remove_node(list, list->head->next);
    print_list(list);
    printf("-----------------------------------------------\n");
    list_remove_data(list, &data[0]);
    print_list(list);
    */
   
    printf("data1: %p\n", &data[0]);
    printf("data2: %p\n", &data[1]);
    printf("data3: %p\n", &data[2]);
    printf("data4: %p\n", &data[3]);

    list = list_new();
    print_list_debug(list);
    
    list_add_head(list, &data[0]);
    print_list_debug(list);
    list_add_tail(list, &data[1]);
    print_list_debug(list);
    //list_remove_head(list);
    //print_list_debug(list);
    list_remove_data(list, &data[1]);
    print_list_debug(list);

    /*
    list_add_tail(list, &data[0]);
    list_add_tail(list, &data[1]);
    print_list(list);
    //list_remove_head(list);
    //print_list(list);
    free(list);
    */

    exit(EXIT_SUCCESS);
}