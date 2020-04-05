/*************************************************************************
	> File Name: b.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月04日 星期六 20时07分54秒
 ************************************************************************/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<signal.h>
#include<string.h>

struct Msg {
    int pid;
    char share_memory[512];
};

struct Msg *msg = NULL;


void print(int sig) {
    printf("<sever> : %s\n", msg->share_memory);
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
    
    memset(msg, 0, sizeof(struct Msg));
    msg->pid = getpid();
    while(1){
        signal(SIGUSR2, print);
        pause();
    }
    return 0;
}

