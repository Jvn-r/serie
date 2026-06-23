#include "ser_proc_conn.h"

int sock_crea(){
    int sock = socket(PF_NETLINK, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, NETLINK_CONNECTOR);

    struct sockaddr_nl addr = {0};
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = gettid();
    addr.nl_groups = CN_IDX_PROC;
    
    int err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if(err){
        close(sock);
        printf("couldnt bind socket\n");
        printf("errno msg = %s\n", strerror(errno));
        return -errno;
    }
    return sock;
}

void sock_regi(int sock){
    enum proc_cn_mcast_op op = PROC_CN_MCAST_LISTEN;

    int err = sock_send(sock, op);
    if(err){
        printf("couldnt register socket\n");
        printf("errno msg = %s\n", strerror(errno));
    }
}

void sock_unre(int sock){
    enum proc_cn_mcast_op op = PROC_CN_MCAST_IGNORE;

    int err = sock_send(sock, op);
    if(err){
        printf("couldnt unregister socket\n");
        printf("errno msg = %s\n", strerror(errno));
    }
}

int sock_send(int sock, enum proc_cn_mcast_op op){
    size_t size = sizeof(op);
    uint8_t buff[1024] = {0};

    struct nlmsghdr *nl_hdr = (struct nlmsghdr *)buff;
    struct cn_msg *cn_msg = (struct cn_msg *)(NLMSG_DATA(nl_hdr));

    nl_hdr->nlmsg_len = NLMSG_LENGTH(sizeof(struct cn_msg) + size);
    nl_hdr->nlmsg_type = NLMSG_DONE;
    nl_hdr->nlmsg_pid = gettid();
    nl_hdr->nlmsg_flags = 0;

    cn_msg->id.idx = CN_IDX_PROC;
    cn_msg->id.val = CN_VAL_PROC;
    cn_msg->seq = 0;
    cn_msg->ack = 0;
    cn_msg->len = size;

    memcpy(cn_msg->data, &op, size);

    struct iovec iov = {};
    iov.iov_base = (void*)nl_hdr;
    iov.iov_len = nl_hdr->nlmsg_len;

    struct sockaddr_nl kern_addr = {0};
    kern_addr.nl_family = AF_NETLINK;
    kern_addr.nl_groups = CN_IDX_PROC;
    kern_addr.nl_pid = 0;
    
    struct msghdr msg = {};
    msg.msg_name = (void*)&kern_addr;
    msg.msg_namelen = sizeof(kern_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;

    ssize_t n = sendmsg(sock, &msg, 0);
    if(n < 0 || n != (ssize_t)nl_hdr->nlmsg_len){
        printf("sendmsg return size is 0, or did not match nlmsg_len\n");
        printf("errno msg = %s\n", strerror(errno));
        return -errno;
    }
    return 0;
}

int sock_reci(int sock){
    uint8_t buff[4096] = {0};

    struct iovec iov = {0};
    iov.iov_base = buff;
    iov.iov_len = sizeof(buff);
    
    struct sockaddr_nl addr = {0};
    
    struct msghdr msg = {0};
    msg.msg_name = &addr;
    msg.msg_namelen = sizeof(addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;

    ssize_t n = recvmsg(sock, &msg, 0);
    
    if(n <= 0){
        printf("recvmsg returned nothing\n");
        printf("errno msg = %s\n", strerror(errno));
        return -errno;
    }
    if(addr.nl_pid != 0){
        printf("addr nlpid != 0, so its not from the core kernel\n");
        return -1;
    }

    int len = n;
    for(struct nlmsghdr *nlmsghdr = (struct nlmsghdr *)buff; NLMSG_OK(nlmsghdr, len); nlmsghdr = NLMSG_NEXT(nlmsghdr, len)){
        if((nlmsghdr->nlmsg_type == NLMSG_ERROR) || (nlmsghdr->nlmsg_type == NLMSG_NOOP))
            continue;
        struct cn_msg *cn_msg = (struct cn_msg *)NLMSG_DATA(nlmsghdr);
        if((cn_msg->id.idx != CN_IDX_PROC) || (cn_msg->id.val != CN_VAL_PROC))
            continue;

        read_even((struct proc_event *)cn_msg->data);

        if(nlmsghdr->nlmsg_type == NLMSG_DONE)
            break;
    }
    return 0;
}

void read_even(struct proc_event *even){
    printf("EVENT: %u\n", even->what);
    switch (even->what){
        case PROC_EVENT_FORK:
            even_fork(even);
            break;
        case PROC_EVENT_EXEC:
            even_exec(even);
            break;
        case PROC_EVENT_EXIT:
            even_exit(even);
            break;
        case PROC_EVENT_COMM:
            even_comm(even);
            break;
        case PROC_EVENT_UID:
            even_uid(even);
            break;
        case PROC_EVENT_GID:
            even_gid(even);
            break;
        default:
            break;
    }
}

void even_fork(struct proc_event *even){
    printf("[FORK] Parent PID: %d -> Child PID: %d\n", 
                   even->event_data.fork.parent_tgid, 
                   even->event_data.fork.child_tgid);
}

void even_exec(struct proc_event *even){
    printf("[EXEC] Process PID: %d changed binaries\n", 
                   even->event_data.exec.process_tgid);
}

void even_exit(struct proc_event *even){
    printf("[EXIT] Process PID: %d exited with code: %d\n", 
                   even->event_data.exit.process_tgid, 
                   even->event_data.exit.exit_code);
}

void even_comm(struct proc_event *even){
    printf("[COMM] Process PID: %d changed name to: %s\n",
           even->event_data.comm.process_tgid,
           even->event_data.comm.comm);
}

void even_uid(struct proc_event *even){
    printf("[UID] Process PID: %d changed UID: %u -> %u\n",
           even->event_data.id.process_tgid,
           even->event_data.id.r.ruid,
           even->event_data.id.e.euid);
}

void even_gid(struct proc_event *even){
    printf("[GID] Process PID: %d changed GID: %u -> %u\n",
           even->event_data.id.process_tgid,
           even->event_data.id.r.rgid,
           even->event_data.id.e.egid);
}
