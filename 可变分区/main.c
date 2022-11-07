#include <stdio.h>
#include <malloc.h>
#include <string.h>

//内存结构体

typedef struct memory_node

{    int size; //内存大小

    int address; //内存始址

} memoryNode;


//分区结构体

typedef struct link_node

{    int size;//分区大小

    int address; //分区始址

    char flag[20]; //分区状态，空闲或者占用作业名

    struct link_node *next;

} node;

//作业结构体

typedef struct job_node

{    int id;//作业编号

    char name[20];//作业名

    int size;//作业大小

} job;

//打印菜单栏
void printmenu(){
    printf("菜单栏选项\n");
    printf("1.内存分配\n");
    printf("2.内存分配\n");
    printf("0.退出\n");
}

//打印分配算法
void printalgor(){
    printf("分配算法\n");
    printf("1.最先分配法\n");
    printf("2.最优分配法\n");
    printf("3.最坏分配法\n");
}

//打印分区
void printpart(node *finishhead, node *waithead){
    node *p, *q;
    p = finishhead->next;
    q = waithead->next;
    printf("主存分配情况\n");
    printf("已分配分区表:\n");
    printf("分区号\t开始地址(KB)\t长度(KB)\t标志\n");
    while(p){
        printf("%8d%8d%8d \t %s\n", 1, p->address, p->size, p->flag);
        p= p->next;
    }
    printf("未分配分区表:\n");
    printf("分区号\t开始地址(KB)\t长度(KB)\t标志\n");
    while(q){
        printf("%8d%8d%8d \t 空闲\n", 2, q->address, q->size);
        q = q->next;
    }
}

// 分配内存
void assign(node *finishhead, node* waithead, job jobnode, int i){
    node *p, *q, *r, *o;
    o = finishhead->next;
    q = finishhead;
    p = waithead->next;
    if(i == 1){
        while(p){
            if(jobnode.size < p->size){
                while(q->next != NULL){
                    q = q->next;
                }
                r = (node *) malloc(sizeof (node));
                r->size = jobnode.size;
                r->address = p->address;
                strcpy(r->flag, jobnode.name);
                //插入已分配的内存
                while(o && o->address < r->address){
                    q = o;
                    o = o->next;
                }
                q->next = r;
                r->next = o;
                p->address += jobnode.size;
                p->size -= jobnode.size;
                printf("分配成功\n");
                break;
            }
        }
    }
}

//回收内存
void recycle(char name[20],job jobhead[20], node * finishhead, node * waithead){
    node *p, *q, *o, *r, *m;
    //先删除已分配的内存
    p = finishhead->next;
    q = finishhead;
    while(p && strcmp(name, p->flag) != 0){
        q = p;
        p =p->next;
    }
    //分配内存
    m = (node *) malloc(sizeof (node));
    m->size = p->size;
    m->address = p->address;
    q->next = p->next;
    printf("回收成功\n");
    //将回收的内存交还
    o = waithead->next;
    r = waithead;
    while(o && o->address < m->address){
        r = o;
        o = o->next;
    }
    if(r == waithead){
        r->next = m;
        m->next = o;
    }else if(r != waithead && r->address+r->size == m->address && m->address+m->size != o->address){
        r->size += m->size;
    }else if(r != waithead && r->address+r->size == m->address && m->address+m->size == o->address){
        r->size = r->size + o->size + m->size;
        r->next = o->next;
    }else if(r != waithead && r->address+r->size != m->address && m->address+m->size == o->address){
        o->address = m->address;
        o->size += m->size;
    }else if(r != waithead && r->address+r->size != m->address && m->address+m->size != o->address){
        r->next = m;
        m->next = o;
    }
}

int main()
{
    setbuf(stdout, NULL);
    memoryNode *memory;
    node *finishhead, *waithead, *p;
    job jobhead[20];
    finishhead = (node *)malloc(sizeof (node));
    finishhead->next = NULL;
    waithead = (node *)malloc(sizeof (node));
    waithead->next =NULL;
    int a, b, c;
    char name[20];
    //c是作业的大小
    c= 0;
    memory = (memoryNode*)malloc(sizeof (memoryNode));
    printf("请输入内存大小与初始地址");
    scanf("%d %d", &memory->size, &memory->address);
    p = (node*) malloc(sizeof (node));
    p->size = memory->size;
    p->address = memory->address;
    p->next = NULL;
    waithead->next = p;
    printmenu();
    printf("请输入选项");
    scanf("%d", &a);
    while(a){
        if(a == 1){
            printalgor();
            printf("请输入选择");
            scanf("%d", &b);
            printpart(finishhead, waithead);
            printf("请输入作业的信息");
            scanf("%s %d", jobhead[c].name, &jobhead[c].size);
            assign(finishhead, waithead, jobhead[c], a);
            printpart(finishhead, waithead);
            c +=1;
        }else if(a == 2){
            printf("请输入要回收作业的名称");
            scanf("%s", name);
            recycle(name, jobhead, finishhead, waithead);
            printpart(finishhead, waithead);
        }
        printmenu();
        printf("请输入选项");
        scanf("%d", &a);
    }
    return 0;
}
