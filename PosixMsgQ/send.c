// send.c

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "head.h"

int msg_max = 0;
int msgsize_max = 0;
int queues_max = 0;

void read_sys_configs()
{
    FILE* msg_max_fd = NULL;
    msg_max_fd = fopen(MSG_MAX, "r");
    if (msg_max_fd == NULL)
    {
        printf("Open %s failed!\n", MSG_MAX);
        return;
    }
    fscanf(msg_max_fd, "%d", &msg_max);
    fclose(msg_max_fd);

    FILE* msgsize_max_fd = NULL;
    msgsize_max_fd = fopen(MSGSIZE_MAX, "r");
    if (msgsize_max_fd == NULL)
    {
        printf("Open %s failed!\n", MSGSIZE_MAX);
        return;
    }
    fscanf(msgsize_max_fd, "%d", &msgsize_max);
    fclose(msgsize_max_fd);
    
    FILE* queues_max_fd = NULL;
    queues_max_fd = fopen(QUEUES_MAX, "r");
    if (queues_max_fd == NULL)
    {
        printf("Open %s failed!\n", QUEUES_MAX);
        return;
    }
    fscanf(queues_max_fd, "%d", &queues_max);
    fclose(queues_max_fd);
}

mqd_t mqd;
int ret;

int main(int argc, char* argv[])
{
    // Create a new mq, @mode and @attr MUST be used.
    // @mode same as 'ls -l' cmd shows
    // @attr - NULL use default attributes that in /proc/sys/fs/mqueue/*
    mqd = mq_open(MQ, O_WRONLY | O_CREAT, 0666, NULL);
    if (mqd == -1)
    {
        // failed
        printf("Creating mq failed.\n");
        exit(1);
    }

    // Check the default attributes.
    struct mq_attr mq_attrs;
    mq_getattr(mqd, &mq_attrs);
    read_sys_configs();
    printf("sys configs: msg_max=%d, msgsize_max=%d, queues_max=%d\n", msg_max, msgsize_max, queues_max);
    printf("get configs: msg_max=%d, msgsize_max=%d, curmsgs=%d, flags=%d\n", mq_attrs.mq_maxmsg, mq_attrs.mq_msgsize, mq_attrs.mq_curmsgs, mq_attrs.mq_flags);

    mq_attrs.mq_maxmsg = 2;
    mq_attrs.mq_msgsize = 128;    // minimum value
    //mq_attrs.mq_flags = O_NONBLOCK;
    mq_setattr(mqd, &mq_attrs, NULL);   // Only mq_flags is changed, others are ignored.
    mq_getattr(mqd, &mq_attrs);
    printf("sys configs: msg_max=%d, msgsize_max=%d, queues_max=%d\n", msg_max, msgsize_max, queues_max);
    printf("get configs: msg_max=%d, msgsize_max=%d, curmsgs=%d, flags=%d\n", mq_attrs.mq_maxmsg, mq_attrs.mq_msgsize, mq_attrs.mq_curmsgs, mq_attrs.mq_flags);

    my_msg_t msg;
    msg.c = 'A';
    msg.s = "Hello World";
    msg.len = 11;

    // Send
    int flag = 1;
    int ret = 0;
    while (flag)
    {
        ret = mq_send(mqd, (const char*)&msg, sizeof(msg), 1);
        if (ret == -1)
        {
            printf("Send Error\n");
            switch (errno)
            {
            case EAGAIN:
                printf("EAGAIN\n");
                break;
            case EBADF:
                printf("EBADF\n");
                break;
            case EINTR:
                printf("EINTR\n");
                break;
            case EINVAL:
                printf("EINVAL\n");
                break;
            case EMSGSIZE:
                printf("EMSGSIZE\n");
                break;
            case ETIMEDOUT:
                printf("ETIMEDOUT\n");
                break;
            default:
                printf("Unknown Error\n");
            }
        }
        else
        {
            printf("Has send 1 msg\n");
        }
        fscanf(stdin, "%d", &flag);
    }

    // Destroy the MQ.
    mq_unlink(MQ);

    return 0;
}
