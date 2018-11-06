#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "head.h"
char buf[10240];
my_msg_t msg;

int main(int argc, char* argv[])
{
    mqd_t mqd;
    mqd = mq_open(MQ, O_RDONLY);
    if (mqd == -1)
    {
        printf("Open MQ error\n");
        exit(1);
    }

    while(1)
    {
        // len - MUST be greater than msgsize.
        int ret = mq_receive(mqd, buf, 10240, NULL);
        if (ret == -1)
        {
            printf("Receive Error\n");
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
        printf("%s\n", buf);
        memcpy(&msg, buf, sizeof(msg));
        printf("c=%c\nmsg=%s\nlen=%d\n", msg.c, "msg", msg.len);
        sleep(2);
    }

    return 0;
}
