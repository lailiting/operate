#include <cstdio>
//#include <malloc>
#include <string>
#include <cstdlib>
#include <string>
#include <iostream>
//#include <sys/malloc.h>

using namespace std;


/*
40 256
 1
jobA

 */

//内存结构体

typedef struct memory_node {
    int size; //内存大小
    int address; //内存始址
} memoryNode;


//分区结构体
typedef struct link_node {
    int size;//分区大小
    int address; //分区始址
    string flag; //分区状态，空闲或者占用作业名
    struct link_node *next;
} node;

//作业结构体

typedef struct job_node {
    int section;//作业段数
    int a[10][10];
    string name;//作业名
    int size;//作业大小
    struct job_node *next;
} job;

//打印菜单栏
void printmenu() {
    printf("菜单栏选项\n");
    printf("1.内存分配\n");
    printf("2.内存去配\n");
    printf("0.退出\n");
}


//打印分区
void printpart(node *finishhead, node *waithead) {
    node *p, *q;
    int i = 1;
    p = finishhead->next;
    q = waithead->next;
    printf("主存分配情况\n");
    printf("已分配分区表:\n");
    printf("分区号\t开始地址(KB)\t长度(KB)\t标志\n");
    while (p) {
        printf("%8d%8d%8d \t %s\n", i, p->address, p->size, p->flag.c_str());
        p = p->next;
        i += 1;
    }
    printf("未分配分区表:\n");
    printf("分区号\t开始地址(KB)\t长度(KB)\t标志\n");
    while (q) {
        printf("%8d%8d%8d \t 空闲\n", i, q->address, q->size);
        q = q->next;
        i += 1;
    }
}

//内存分配函数
void assignment(node *finishhead, node *waithead, job *jobhead) {
    //q保存分段的数据
    //p记录已分配内存
    //r记录未分配内存地址
    node *p, *q, *r;
    char numstr[5];
    job *jobnode;
    p = finishhead;
    r = waithead->next;
    int i;
//    jobnode = (job *) malloc(sizeof(job));
    jobnode = new job;
    jobnode->next = NULL;
    printf("请输入作业名:");
    cin >> jobnode->name;
//    scanf("%s", &jobnode->name[0]);
    printf("请输入%s需要分配的主存大小", jobnode->name.c_str());
    scanf("%d", &jobnode->size);
    printf("请输入要分成几段:");
    scanf("%d", &jobnode->section);
    //找到已经分配的内存可插入位置
    while (p->next) {
        p = p->next;
    }
    for (i = 0; i < jobnode->section; i++) {
        printf("请输入第%d段的大小", i + 1);
        q = new node;
        q->next = NULL;
        scanf("%d", &q->size);

        q->address = r->address;

//        q->flag += jobnode->name;
        printf("%s", (jobnode->name + to_string(i)).c_str());
        q->flag = jobnode->name + to_string(i);

//        strcat(q->flag, jobnode->name);
//        itoa(i, numstr, 10);
//        strcat(q->flag, numstr);

        r->address += q->size;
        r->size -= q->size;
        //记录段数据
        jobnode->a[i][0] = i;
        jobnode->a[i][1] = q->size;
        jobnode->a[i][2] = q->address;
        p->next = q;
        p = p->next;
        printpart(finishhead, waithead);
        printf("分配成功\n");
    }
    //打印段表
    printf("********打印%s段表\n", jobnode->name.c_str());
    printf("段号\t段长\t基址\n");
    for (i = 0; i < jobnode->section; i++) {
        printf("%d%5d%5d\n", jobnode->a[i][0], jobnode->a[i][1], jobnode->a[i][2]);
    }
}

//内存去配
void recycle(node *finishhead, node *waithead, job *jobhead) {
    //找到作业的位置进行删除
    //根据段表数据对分配表进行修改
    //修改完成将作业删除
    //p, q记录已分配内存的前后节点
    //o,r记录为分配的前后节点
    node *p, *q, *o, *r, *mid;
    int i, num = 0;
    p = finishhead->next;
    q = finishhead;
    //记录作业
    job *j;
    j = jobhead;
    string jobname;
    printf("请输入要回收的作业名");
    cin >> jobname;
//    scanf("%s", &jobname[0]);
    while (j->next) {
        j = j->next;
        if (j->name == jobname) {
            break;
        }
    }
    //找到作业，对已经分配的内存进行回收
    while (p) {
       for(i = 0; i < 4; i++){
           if(jobname[i] == p->flag[i]){
               num++;
           }
       }
        //num用来记录字符串匹配数，前4位符合属于同一段
        if (num == 4) {
            //将回收的内存交还
            //mid用来保存p的下一个节点
            mid = p->next;
            printf("回收%s的段%s成功\n", jobname.c_str(), p->flag.c_str());
            o = waithead->next;
            r = waithead;
            while (o && o->address < p->address) {
                r = o;
                o = o->next;
            }
            if (r == waithead) {
                r->next = p;
                p->next = o;
            } else if (r != waithead && r->address + r->size == p->address && p->address + p->size != o->address) {
                r->size += p->size;
            } else if (r != waithead && r->address + r->size == p->address && p->address + p->size == o->address) {
                r->size = r->size + o->size + p->size;
                r->next = o->next;
            } else if (r != waithead && r->address + r->size != p->address && p->address + p->size == o->address) {
                o->address = p->address;
                o->size += p->size;
            } else if (r != waithead && r->address + r->size != p->address && p->address + p->size != o->address) {
                r->next = p;
                p->next = o;
            }
            q->next = mid;
            p = mid;
        }else{
            q = p;
            p = p->next;
        }
        num = 0;
    }
    printpart(finishhead, waithead);
}


int main() {
    setbuf(stdout, NULL);
    //finishhead是已分配内存头节点 waithead是未分配内存的头节点
    node *finishhead, *waithead, *p;
    //作业头节点
    job *jobhead;
    //记录选择的tag
    int f;
    //数据初始化
    finishhead = new node;
    finishhead->next = NULL;
    waithead = new node;
    waithead->next = NULL;
    jobhead = new job;
    jobhead->next = NULL;
    printf("请输入初始内存起址和大小");
    //输入初始化内存
    p = new node;
    p->next = NULL;
    scanf("%d %d", &p->address, &p->size);
    waithead->next = p;
    printmenu();
    printf("请输入选项:");
    scanf("%d", &f);
    while (f != 0) {
        if (f == 1) {
            printpart(finishhead, waithead);
            assignment(finishhead, waithead, jobhead);
        } else if (f == 2) {
            recycle(finishhead, waithead, jobhead);
        }
        printmenu();
        printf("请输入选项:");
        scanf("%d", &f);
    }
    return 0;
}