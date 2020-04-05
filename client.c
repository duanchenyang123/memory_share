/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月04日 星期六 20时06分02秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<signal.h>
#include <string.h>

struct Msg {
    int pid;
    char share_memory[512];
};

struct Msg *msg = NULL;

void print(int sig) {
    printf("<parent> : %s\n", msg->share_memory);
}

int main(){
    pid_t pid;
    int shmid;
    key_t key = ftok(".",198);

    if((shmid = shmget(key,sizeof(struct Msg),IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return 1;
    }
    if((msg = (struct Msg *)shmat(shmid, NULL, 0)) < 0){
        perror("shmat");
        return 1;
    }
    memset(msg->share_memory, 0, sizeof(msg->share_memory));
    
    while (1) {
        printf("<client> : ");
        scanf("%[^\n]s", msg->share_memory);
        getchar();
        if(msg->pid == 0) {
            printf("sever isn't online!\n");
        } else
            kill(msg->pid, SIGUSR2);
    }
    return 0;
}

