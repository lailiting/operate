#include <stdio.h>
#include <stdlib.h>

typedef struct  data

{

    int hour;

    int min;

}time;

typedef struct node

{

    int id;//进程编号

    char name[20];//进程名

    int good;          //优先级

    time arrive;//到达就绪队列时间

    int zx;//执行时间

    time start;//开始执行时间

    time finish;//执行完成时间

    int zz;//周转时间=执行完成时间-到达就绪队列时间

    float zzxs;//带权周转时间=周转时间/执行时间

    struct node* next;

}linklist;


//初始化
void init(linklist *head){
    head = (linklist *) malloc(sizeof(linklist));
    head->next = NULL;
}

//输入
void input(linklist *head, int n){
    linklist *p, *q;
    int i = 0;
    q = head;
    printf("\nid 名字  优先级 开始时间 执行时间\n");
    while(i < n){
        p = (linklist *)malloc(sizeof(linklist));
        scanf("%d %s %d %d:%d %d", &p->id, p->name, &p->good, &p->arrive.hour, &p->arrive.min, &p->zx);
        p->start.hour = 0;
        p->start.min = 0;
        p->finish.hour = 0;
        p->finish.min = 0;
        p->zz = 0;
        p->zzxs = 0;
        p->next = NULL;
        q->next = p;
        q = p;
        i= i+1;
    }
}



void print(linklist *head, int n){
    linklist *p;
    int i = 0;
    p = head->next;
    printf("\nid\t名字 优先级\t到达时间 \t执行时间 开始时间 \t 结束时间 \t 周转时间 带权周转时间 :\n");
    while(i < n && p!=NULL){
        printf("%d\t%s\t%d \t %d:%d\t\t   %d  \t  %d:%d \t\t %d:%d  \t        %d\t   %.2f\n", p->id, p->name, p->good, p->arrive.hour, p->arrive.min, p->zx, p->start.hour, p->start.min, p->finish.hour, p->finish.min, p->zz, p->zzxs);
        p = p->next;
        i = i+1;
    }
}

//根据进入时间排序
void sort(linklist *head){
    linklist *pre, *p, *q;
    //pre有序列表的第一个结点 p记录位置 q为排序的第一个结点
    q = head->next->next;
    head->next->next = NULL;
    p = q;
    while(p){
        q = p->next;//先记录要排序的后一个结点
        pre = head;
        //找到插入的位置
        while(pre->next && pre->next->arrive.hour*60 + pre->next->arrive.min < p->arrive.hour*60 + p->arrive.min){
            pre = pre->next;
        }

        //插入已排序的结点
        p->next = pre->next;
        pre->next = p;
        p = q;
    }
}

void good(linklist *head, linklist *p){
    //结点移动
    linklist *q, *pre;
    pre = head;
    q = pre->next;
    //从已结束进程的下一个结点开始找
    while(q != p){
        if(q->good < p->good){
            if(p->next == NULL){
                pre->next = p;
                p->next = q;
                q->next->next = NULL;
            }else{
                pre->next = p;
            q->next = p->next;
            p->next = q;
            }
            break;
        }
        pre = q;
        q = q->next;
    }
}



//根据优先级进行排序
void goodsort(linklist *q){
    linklist *p, *pre;
    p = q->next;
    while(p){
        if(p->arrive.hour*60 + p->arrive.min < q->finish.hour*60 + q->finish.min){
            pre = p->next;
            good(q, p);
            p = pre;
        }else{
            p = p->next;
        }
    }
}

//优先级调度算法
void run(linklist *head, int n){
    int i = 0, j = 0, allzz = 0;
    float allzzxs = 0;
    linklist *p, *pre;
    p = head->next;
    while(i < n && p!= NULL){
        if(i == 0){
            p->start.hour = p->arrive.hour;
            p->start.min = p->arrive.min;
            //计算开始时间 结束时间
            if(p->start.min + p->zx >= 60){
                    p->finish.hour = p->start.hour + 1;
                    p->finish.min = p->start.min + p->zx - 60;
            }else{
                    p->finish.hour = p->start.hour;
                    p->finish.min = p->start.min + p->zx;
            }
        }else{
            if(p->arrive.hour*60 + p->arrive.min < pre->finish.hour*60 + pre->finish.min){
                p->start.hour = pre->finish.hour;
                p->start.min = pre->finish.min;
                //计算开始时间 结束时间
                if(p->start.min + p->zx >= 60){
                    p->finish.hour = p->start.hour + 1;
                    p->finish.min = p->start.min + p->zx - 60;
                }else{
                    p->finish.hour = p->start.hour;
                    p->finish.min = p->start.min + p->zx;
                }
            }else{
                p->start.hour = p->arrive.hour;
                p->start.min = p->arrive.min;
                //计算开始时间 结束时间
                if(p->start.min + p->zx >= 60){
                    p->finish.hour = p->start.hour + 1;
                    p->finish.min = p->start.min + p->zx -60;
                }else{
                    p->finish.hour = p->start.hour;
                    p->finish.min = p->start.min + p->zx;
                }
            }
        }
        pre = p;

        //下一个结点需要根据优先级重新进行排序
        goodsort(pre);
        p = p->next;
        i = i+1;
    }

    //计算带权
    i = 0;
    p = head->next;
    while(i < n){
        p->zz = p->finish.hour*60 + p->finish.min - p->arrive.hour*60 - p->arrive.min;
        p->zzxs = (float) p->zz/ (float) p->zx;
        p = p->next;
        i++;
        j++;
    }
    i = 0;
    p = head->next;
    while(i < n){
        allzz = allzz + p->zz;
        allzzxs = allzzxs + p->zzxs;
        p = p->next;
        i++;
    }
    print(head, n);
    printf("\n系统周转时间:%d  \n系统带权周转时间%.2f\n", allzz/j, allzzxs/j);
}



int main()
{
    system("color f0");
    linklist head;
    int n, i;
    printf("请输入操作 1(进程调度) 0(结束进程):");
    scanf("%d", & i);
    while(i == 1){
        printf("请输入进程的数量:");
        scanf("%d", &n);
        init(&head);
        input(&head, n);
        sort(&head);
        printf("\n模拟进程调度FCFS算法的执行结果:\n");
        run(&head, n);
        printf("请输入操作 1(进程调度) 0(结束进程):");
        scanf("%d", & i);
    }
    printf("\n操作结束");
    return 0;
}
