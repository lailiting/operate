#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//分区结构体

typedef struct node5{

    int id;//分区号

    int size;//分区长度

    int address;//分区始址

    char station[10];//分区状态

    struct node5 *next;

}node5;

//作业结构体

typedef struct job{

    int id;//作业编号

    char name[10];//作业名

    int size;//作业大小

}job;

//输入区块信息
void addnode5(node5 * node5head, int a){
    node5 *p, *q;
    p = node5head;
    int i = 0;
    printf("请输入区块信息\n");
    for(i; i < a; i++){
        q = (node5*)malloc(sizeof(node5));
        scanf("%d %d %d", &q->id, &q->size, &q->address);
        q->next = NULL;
        p->next = q;
        p = p->next;
    }
}

//打印区块信息
void printnode5(node5 *node5head){
    node5 *p;
    p = node5head->next;
    printf("打印区块信息\n");
    printf("分区号\t 大小(KB) \t 始址(KB) \t 状态\n");
    while(p){
        if(strlen(p->station) == 0)
        {
            printf("%5d %10d %8d \t\t 0\t\n", p->id, p->size, p->address);
        }else{
            printf("%5d %10d %8d \t\t %s\t\n", p->id, p->size, p->address, p->station);
        }
        p = p->next;
    }
}
//打印作业信息
void printjob(job jobhead[100], int n){
    int i;
    printf("打印各作业的信息\n");
    printf("作业名\t作业大小\n");
    for(i = 0; i < n;i++){
        printf("%s%d %10dKB\n", jobhead[i].name,i+1, jobhead[i].size);
    }
}

//输入作业信息
void addjob(job jobhead[100], int n){
    int i;
    printf("请输入这些作业的信息\n");
    for(i = 0; i < n; i++){
        jobhead[i].id = i+1;
        itoa(i+1,jobhead[i].name,10);
        strcat(jobhead[i].name, "JOB");
        printf("请输入作业%d的大小", i+1);
        scanf("%d", &jobhead[i].size);
    }
}

//作业放入区块中
void working(job jobhead[100], int n, node5 * node5head){
    node5 *p;
    int i;
    p = node5head->next;
    for(i=0; i<n; i++){
        p = node5head->next;
        while(p)
        {
            if (jobhead[i].size < p->size && strlen(p->station) == 0)
            {
                strcpy(p->station, jobhead[i].name);
                break;
            }
            p = p->next;
        }
    }
}
void deljob(node5 *node5head, char name[10]){
    node5 *p;
    int i;
    p = node5head->next;
    while(p){
        if(strcmp(p->station, name) == 0){
            for(i = 0; i < strlen(p->station); i++){
                p->station[i] = '\0';
            }
            printf("回收成功\n");
            printnode5(node5head);
            break;
        }
        p = p->next;
    }
}

int main()
{
    setbuf(stdout, NULL);
    system("color f0");
    //区块信息的头节点
    node5 *node5head;
    job jobhead[100];
    char name[10];
    int a, b, i;
    node5head = (node5 *)malloc(sizeof(node5));
    node5head->next = NULL;
    printf("请输入区块个数");
    scanf("%d", &a);
    addnode5(node5head, a);
    printnode5(node5head);
    printf("请输入作业个数");
    scanf("%d", &b);
    addjob(jobhead, b);
    printjob(jobhead, b);
    working(jobhead, b, node5head);
    printnode5(node5head);
    printf("是否要回收(1/0):");
    scanf("%d", &i);
    while(i){
        printnode5(node5head);
        printf("请输入回收的作业名:");
        scanf("%s", name);
        deljob(node5head, name);
        printf("是否要回收(1/0):");
        scanf("%d", &i);
    }
    return 0;
}
