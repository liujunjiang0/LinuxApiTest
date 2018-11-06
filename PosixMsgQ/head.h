// head.h

// system configuration files paths.
const char* MSG_MAX = "/proc/sys/fs/mqueue/msg_max";
const char* MSGSIZE_MAX = "/proc/sys/fs/mqueue/msgsize_max";
const char* QUEUES_MAX = "/proc/sys/fs/mqueue/queues_max";

// queue name should begin with slash, the following characters excludes slash.
const char* MQ = "/my_mquque";

typedef struct my_msg
{
    char c;
    char* s;
    int len;
}my_msg_t, * my_msg_ptr;


