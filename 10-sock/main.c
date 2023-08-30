/*
 * Copyright (C) 2016-17 Freie Universität Berlin
 * Copyright (C) 2023 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 * @author  Martine Lenders <martine.lenders@tu-dresden.de>
 */

#include <stdio.h>

#include "msg.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/sock/async/event.h"
#include "net/ipv6/addr.h"
#include "shell.h"
#include "thread.h"

#define MSG_QUEUE_SIZE  8

#define SERVER_MSG_QUEUE_SIZE   (8)
#define SERVER_BUFFER_SIZE      (64)

static sock_udp_t sock;
static bool server_running = false;
static char server_buffer[SERVER_BUFFER_SIZE];
static char server_stack[THREAD_STACKSIZE_DEFAULT];
static event_queue_t queue;

/* [TASK 3: Add sock_udp event handler here] */

/* [TASK 3: Add further event handlers here] */

/* [TASK 2: add thread handler here] */

/* [TASK 1 & 2: add shell handlers] */

int main(void)
{
    puts("sock example.\n");

    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* [TASK 3: You might want to create the thread now here] */

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
    
    /* ignore pre-initialized variables for now */
    (void)sock;
    (void)server_running;
    (void)server_buffer;
    (void)server_stack;
    (void)queue;

    return 0;
}

/** @} */
