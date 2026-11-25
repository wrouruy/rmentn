#pragma once

#include <stddef.h>
#include <pthread.h>

#define WORKERS 3

typedef struct Node {
    char *path;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} Queue;

typedef struct {
    Queue tasks;
    Queue results;

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    // queued + in-progress tasks
    size_t pending;

    char *target;
} Context;

// appends an item to the queue. call with mutex held
void enqueue(Queue *queue, char *path);

// removes an item from the front. call with mutex held
char *dequeue(Queue *queue);

// adds a found target to the results
void add_result(Context *ctx, char *path);

// adds a targets to scan
void add_task(Context *ctx, char *path);

// // builds the full path of a child entry
// char *join_path(const char *parent, const char *name);

// scans a single directory
void scan_directory(Context *ctx, char *path);

// worker thread entry point
void *worker(void *arg);