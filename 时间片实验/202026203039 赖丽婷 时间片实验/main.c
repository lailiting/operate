#include <stdio.h>
#include <stdlib.h>

//定义结构体
typedef struct data {
    int hour;
    int min;
} time;
typedef struct node {
    int id;//进程编号
    char name[20];//进程名
    time arrive;//到达就绪队列时间
    int zx;//执行时间
    time start;//开始执行时间
    time finish;//执行完成时间
    int finishtime; //已经执行完的时间片
    int restime; //剩余要执行的时间
    int zz;//周转时间=执行完成时间-到达就绪队列时间
    float zzxs;//带权周转时间=周转时间/执行时间
    struct node *next;
} linklist;

linklist* readyqueueinsert(linklist *finishhead,linklist *front, linklist *rear, linklist *p, int timepiece, time *runtime);
void readyqueueprint(linklist *front, time *runtime);
void readyqueuedelete(linklist *finishhead, linklist *front, linklist *rear, int timepiece, time *runtime);

//初始化
linklist init(linklist *head) {
    head = (linklist *) malloc(sizeof(linklist));
    head->next = NULL;
    return (*head);
}

//输入
void input(linklist *head, int n) {
    linklist *p, *q;
    int i = 0;
    q = head;
    printf("\nid 名字 开始时间 执行时间\n");
    while (i < n) {
        p = (linklist *) malloc(sizeof(linklist));
        scanf("%d %s %d:%d %d", &p->id, p->name, &p->arrive.hour, &p->arrive.min, &p->zx);
        p->start.hour = 0;
        p->start.min = 0;
        p->finish.hour = 0;
        p->finish.min = 0;
        p->zz = 0;
        p->zzxs = 0;
        p->finishtime = 0;
        p->restime = p->zx;
        p->next = NULL;
        q->next = p;
        q = p;
        i = i + 1;
    }
}

//根据入队时间进行排序
void sort(linklist *head) {
    linklist *pre, *p, *q;
    //pre有序列表的第一个结点 p记录位置 q为排序的第一个结点
    q = head->next->next;
    head->next->next = NULL;
    p = q;
    while (p) {
        q = p->next;//先记录要排序的后一个结点
        pre = head;
        //找到插入的位置
        while (pre->next && pre->next->arrive.hour * 60 + pre->next->arrive.min < p->arrive.hour * 60 + p->arrive.min) {
            pre = pre->next;
        }

        //插入已排序的结点
        p->next = pre->next;
        pre->next = p;
        p = q;
    }
}

//就绪队列执行入口
void readyqueue(linklist *head, linklist *finishhead, int timepiece) {
    linklist front, rear, *p;
    //p用来记录把等待运行的队列加入到就绪队列中
    //用time来记录正在运行进程的开始时间
    time *runtime;

    //申请空间
    runtime = (time *) malloc(sizeof(time));

    //初始化
    p = head->next;
    runtime->hour = p->arrive.hour;
    runtime->min = p->arrive.min;
    printf("time is %d:%d\n", runtime->hour, runtime->min);
    front = init(&front);
    rear = init(&rear);
    while (p != NULL) {
        //就绪队列的插入
        p = readyqueueinsert(finishhead,&front, &rear, p, timepiece, runtime);
        //队列执行打印
        if(p != NULL){
            readyqueueprint(&front, runtime);
            //时间片轮转
            readyqueuedelete(finishhead, &front, &rear, timepiece, runtime);

            readyqueueprint(&front, runtime);
        }
        if (p == NULL) {
            while (front.next != NULL) {
                //执行循环
                readyqueuedelete(finishhead, &front, &rear, timepiece, runtime);
                if(front.next != NULL){
                    readyqueueprint(&front, runtime);
                }
            }
        }
    }
    //执行结束
}

//对执行进程的时间进行处理
void runqueuetime(linklist *p, int timepiece, time *runtime) {
    if(p->restime < timepiece){
        runtime->min += p->restime;
    }else{
        runtime->min += timepiece;
    }
    if(runtime->min >= 60){
        runtime->hour+=1;
        runtime->min -= 60;
    }
    if(p->restime < timepiece){
        p->finishtime += p->restime;
    }else{
        p->finishtime += timepiece;
    }
    p->restime -= timepiece;
    if (p->restime < 0) {
        p->restime = 0;
    }
}


//就绪队列的插入操作

linklist* readyqueueinsert(linklist *finishhead,linklist *front, linklist *rear, linklist *p, int timepiece, time *runtime) {
    linklist *q =p;
    while (q != NULL) {
        if (q->arrive.hour * 60 + q->arrive.min <= runtime->hour * 60 + runtime->min + timepiece) {
            if (front->next == NULL) {
                front->next = rear->next = q;
                front->next->start.hour = runtime->hour;
                front->next->start.min = runtime->min;
            } else {
                readyqueuedelete(finishhead, front,rear, timepiece, runtime);
                readyqueueprint(front, runtime);
                rear->next->next = q;
                rear->next = rear->next->next;
            }
            q = q->next;
            rear->next->next = NULL;
        } else {
            if(q->next == NULL){
                return NULL;
            }else{
                return q;
            }
        }
    }
    return NULL;
}


//进程执行完成，插入完成进程队列
void finishinsert(linklist *head, linklist *p) {
    linklist *q;
    q = head;
    while (q->next != NULL) {
        q = q->next;
    }
    q->next = p;
    q->next->next = NULL;
}

//就绪队列的删除操作
void readyqueuedelete(linklist *finishhead, linklist *front, linklist *rear, int timepiece, time *runtime) {
    linklist *p, *q;
    if(front->next != NULL && front->next->start.hour == 0 && front->next->start.min == 0){
        front->next->start.hour = runtime->hour;
        front->next->start.min = runtime->min;
    }
    if (front->next != NULL) {
        runqueuetime(front->next, timepiece, runtime);
    }
    if (front->next->restime == 0) {
        //执行完成插入
        q = front->next->next;
        front->next->finish.hour = runtime->hour;
        front->next->finish.min = runtime->min;
        finishinsert(finishhead, front->next);
        front->next = q;
    } else {
        if (front->next->next != NULL) {
            p = front->next->next;
            rear->next->next = front->next;
            front->next = p;
            rear->next = rear->next->next;
            rear->next->next = NULL;
        }
    }
    //删除完成
}


//就绪队列的打印操作
void readyqueueprint(linklist *front, time *runtime) {
    linklist *p;
    p = front->next;
    printf("id  名字 当前时间   完成时间   剩余时间\n");
    while (p != NULL) {
        if (p == front->next) {
            printf("%d\t %s \t %d:%d \t %d %d\n", p->id, p->name, runtime->hour, runtime->min, p->finishtime,
                   p->restime);
        } else {
            printf("%d\t %s \t 00:00 \t %d %d\n", p->id, p->name,  p->finishtime,
                   p->restime);
        }
        p = p->next;
    }
}




//最终结果的打印

void print(linklist *head, int n) {
    linklist *p;
    int i = 0, j = 0, allzz = 0;
    float allzzxs = 0;
    p = head->next;
    //计算带权
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
    i = 0;
    p = head->next;
    printf("\n序列为id  名字   到达时间  执行时间  首次开始时间   结束时间    周转时间  带权周转时间 :\n");
    while (i < n && p != NULL) {
        printf("%d          %s      %d:%d        %d      %d:%d      %d:%d       %d        %.2f\n", p->id, p->name,
               p->arrive.hour, p->arrive.min, p->zx, p->start.hour, p->start.min, p->finish.hour, p->finish.min, p->zz,
               p->zzxs);
        p = p->next;
        i = i + 1;
    }

    printf("\n系统周转时间:%d  系统带权周转时间%.2f", allzz/j, allzzxs/j);
}


//需要三个队列，就绪队列与运行队列，最终执行完成的结果序列
int main() {
    setbuf(stdout, NULL);
    system("color f0");
    linklist head, finishhead;
    int n, timepiece;
    head = init(&head);
    finishhead = init(&finishhead);
    printf("请输入进程个数:");
    scanf("%d", &n);
    printf("\n请输入时间片的时间:");
    scanf("%d", &timepiece);
    input(&head, n);
    sort(&head);
    readyqueue(&head, &finishhead, timepiece);
    print(&finishhead, n);
    return 0;
}
