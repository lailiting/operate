#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXID 20
#define MAXNAME 30


//定义学生结构体
typedef struct{
    char stuId[MAXID];
    char stuName[MAXNAME];
    struct stuMessage *next;
}stuMessage;

//队列结构体
typedef struct{
    stuMessage *front;
    stuMessage *rear;
}linkQueue;

//初始化队列
void init(linkQueue *lq){
    lq->front = lq->rear = NULL;
}


//入队操作
void addStu(linkQueue *lq){
    stuMessage *stu;
    stu = (stuMessage *)malloc(sizeof(stuMessage));
    printf("请输入学生学号:");
    scanf("%s",stu->stuId);
    printf("\n请输入学生姓名:");
    scanf("%s",stu->stuName);
    stu->next = NULL;
    if(lq->front == NULL){
        lq->front = lq->rear = stu;
    }else{
        lq->rear->next = stu;
        lq->rear = stu;

    }
    printf("\n添加成功");
}

//遍历学生队列元素
void printStu(linkQueue *lq){
    stuMessage *p;
    if(lq->front==NULL && lq->rear==NULL){
        printf("队列为空");
        exit(1);
    }
    p = lq->front;
    printf("学生信息表:\n");
    while(p != NULL){
        printf("学生的学号为:%s，学生的姓名为:%s\n",p->stuId, p->stuName);
        p = p->next;
    }
    printf("遍历完成");
}

//删除队列
void delStu(linkQueue *lq){
    stuMessage *p;
    if(lq->front==NULL && lq->rear == NULL){
        printf("队列为空不能删除");
        exit;
    }
    p = lq->front;
    lq->front = lq->front->next;
    free(p);
    printf("删除学生信息成功\n");
}

//查找学生信息
void findStu(linkQueue *lq){
    stuMessage *p;
    char name[MAXNAME];
    printf("\n请输入学生姓名:");
    scanf("%s", name);
    if(lq->front == NULL && lq->rear == NULL){
        printf("\n队列为空未找到");
        exit(1);
    }
    p = lq->front;
    while(p != NULL && strcmp(p->stuName, name) != 0){
        p = p->next;
    }
    if(p == NULL){
        printf("未找到");
    }else{
        printf("\n查找成功,%s的学号为%s\n",p->stuName, p->stuId);
    }
}

//修改学生信息
void changeStu(linkQueue *lq){
    stuMessage *p;
    char name[MAXNAME];
    char id[MAXID];
    int x;
    printf("你要修改学生的姓名还是学号 0姓名 1学号 ");
    scanf("%d", &x);
    if(x == 0){
        printf("\n请要修改的学生姓名");
        scanf("%s", name);
    }else{
        printf("\n请要修改的学生学号");
        scanf("%s", id);
    }
    p = lq->front;
    if(lq->front == NULL && lq->rear == NULL){
        printf("\n学生队列为空，无法修改\n");
        exit(1);
    }
    if(x == 0){
        while(p != NULL && strcmp(p->stuName, name) != 0){
            p = p->next;
        }
        if(p != NULL){
            printf("\n请输入要改的姓名:");
            scanf("%s",p->stuName);
            printf("\n修改成功\n");
        }
    }else{
        while(p != NULL && strcmp(p->stuId, id) != 0){
            p = p->next;
        }
        if(p != NULL){
            printf("\n请输入要改的学号:");
            scanf("%s",p->stuId);
            printf("\n修改成功\n");
        }
    }
}



