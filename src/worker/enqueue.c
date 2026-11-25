#include <stdlib.h>
#include "worker.h"

void enqueue(Queue *queue, char *path) {
    Node *node = malloc(sizeof(Node));

    node->path = path;
    node->next = NULL;

    if (queue->tail)
        queue->tail->next = node;
    else
        queue->head = node;

    queue->tail = node;
}

char *dequeue(Queue *queue) {
    if (!queue->head)
        return NULL;

    Node *node = queue->head;
    char *path = node->path;

    queue->head = node->next;

    if (!queue->head)
        queue->tail = NULL;

    free(node);
    return path;
}