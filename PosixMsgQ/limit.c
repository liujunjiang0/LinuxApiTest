// limit.c
/*
    Test for verifying that rlimit limits the number of bytes allocated for all mqueues created by current process,
    NOT for each one.
 The number of bytes allocated for each mqueue equeals to(Linux 3.4 and earlier):
    attr.mq_maxmsg * sizeof(struct msg_msg) + min(attr.mq_maxmsg, MQ_PRIO_MAX) * sizeof(struct posix_msg_tree_node)
    + attr.mq_maxmsg * attr.mq_msgsize
    msg_msg and posix_msg_tree_node are some kernel structs. 
 So the bytes > attr.mq_maxmsg * attr.mq_msgsize.
**/

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/stat.h>

// For RLIMIT_MSGQUEUE, limit the number of bytes allocated for mqueues calling process created.
#define SOFT    1024
#define HARD    2048    // unpriviliged process is limited to SOFT by kernel.

#define MQ  "/myqueue"
#define OTHER "/other"

int main(int argc, char* argv[])
{
    // Set the mqueue limits for current process.
    struct rlimit lmt;
    getrlimit(RLIMIT_MSGQUEUE, &lmt);
    printf("soft=%d, hard=%d\n", lmt.rlim_cur, lmt.rlim_max);
    lmt.rlim_cur = SOFT;
    lmt.rlim_max = HARD;
    setrlimit(RLIMIT_MSGQUEUE, &lmt);
    // Another way is using config files:/etc/security/limits.d/msgqueue.conf
    // ulimit cmd can do the same thing, but should consider how to work on currut process.

    struct mq_attr mattr;
    
    // Failed because of 'Too many open files', 1*1024 + kernel things > SOFT
    //mattr.mq_maxmsg = 1;
    //mattr.mq_msgsize = 1024;

    // Sucess for one mqueue, but failed for two mqueues.
    mattr.mq_maxmsg = 1;
    mattr.mq_msgsize = 512;

    
    mqd_t mqd;
    mqd = mq_open(MQ, O_RDWR | O_CREAT, 0666, &mattr);
    if (mqd == -1)
    {
        printf("Create myqueue error : %s.\n", strerror(errno));
        exit(1);
    }
    else
    {
        printf("Create myqueue success.\n");
    }

    mqd_t mqd1;
    mqd1 = mq_open(OTHER, O_RDWR | O_CREAT, 0666, &mattr);
    if (mqd1 == -1)
    {
        printf("Create myqueue error : %s.\n", strerror(errno));
        exit(1);
    }
    else
    {
        printf("Create myqueue success.\n");
    }
    mq_unlink(MQ);
    mq_unlink(OTHER);

    return 0;
}
