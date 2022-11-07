#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

typedef struct node
{
    int size;//作业大小
    int row[MAX];  //记录行数
    int col[MAX];  //记录列数
    int block[MAX];  //记录对应的块号
    char flag[20]; //作业名
    struct node *next;
} linklist;

int page[MAX][MAX] = {0}; //存放物理块标志位0/1
int surplus;  //物理块剩余量
int Msize;  //内存空间容量
int Wlength;  //字长
int Blength;  //块长
int Hlength; //行数-1
int Rlength; //最后一行的列数

//打印位示图表
void printPos(){
    int i, j;
    for(i = 0; i < Hlength; i++){
        for(j = 0;j < Wlength; j++){
            printf("%2d", page[i][j]);
        }
        printf("\n");
    }
    for(j = 0; j < Rlength; j++){
        printf("%2d", page[i][j]);
    }
    printf("\n");
}


//初始化位示图函数
void initPos(){
    int a = 465;
    int i, j, m, n;
    //物理块总数
    int ABlength = Msize/Blength;
    //列数
    Hlength = ABlength/Wlength;
    Rlength = ABlength%Wlength;
    //随机取值为1
    for(i = 0; i < a; i++){
        m = rand() % 16;
        if(m == Hlength){
            n = rand() % Rlength;
        }else{
            n = rand() % Wlength;
        }
        page[m][n] = 1;
    }
    printPos();
}


//打印分配的行与列
void printRC(int size, linklist *p){
    int i;
    for(i = 0; i < size; i++){
        printf("行%d 列%d", p->row[i], p->col[i]);
    }
}
//打印页表
void printpage(linklist *p){
    int i;
    printf("\n******打印%s作业的页表******", p->flag);
    for(i = 0; i < p->size; i++){
        p->block[i] = p->row[i]*64 + p->col[i];
        printf("%5d %5d\n", i, p->block[i]);
    }
}


//内存分配函数
void assignment(linklist *joblist){
    linklist *p, *q;
    // r c为行数 列数
    int m = 0, i = 0, j = 0, r = 0, c = 0;
    //物理块总数
    int ABlength = Msize/Blength;
    //行数
    int Hlength = ABlength/Wlength;
    //最后一行的列数
    int Rlength = ABlength%Wlength;
    p = joblist;
    printPos();
    printf("剩余物理块数%d\n", surplus);
    while (p->next){
        p = p->next;
    }
    q = (linklist *) malloc(sizeof(linklist));
    q ->next = NULL;
    printf("请输入作业的名字:");
    scanf("%s", q->flag);
    printf("请输入作业需要分配的大小:");
    scanf("%d", &q->size);
    if(q->size < surplus){
        printf("分配成功\n");
        surplus -= q->size;
        while (m < q->size){
            //找到行列不为1的，存入数据
            if(page[i][j] == 0){
                page[i][j] = 1;
                q->row[r] = i;
                q->col[c] = j;
                r +=1;
                c += 1;
                m += 1;
            }
            j += 1;
            if(j >= Wlength){
                i += 1;
                j = 0;
            }
        }
        printPos();
        printpage(q);
        p->next = q;
    }else{
        printf("内存不够\n");
    }
    printf("%d剩余物理块数\n", surplus);
}

//内存去配
void recycle(linklist *jobhead){
    char name[20];
    int i;
    linklist *p, *q;
    p = jobhead->next;
    q = jobhead;
    printf("请输入你当前要回收的作业名:");
    scanf("%s", name);
    while(p){
        if(strcmp(name,p->flag) == 0){
            printf("回收成功\n");
            for(i = 0; i < p->size; i++){
                page[p->row[i]][p->col[i]]=0;
            }
            surplus += p->size;
            q->next = p->next;
            printPos();
            printf("剩余块数%d\n", surplus);
            return;
        }
        q = p;
        p = p->next;
    }
    printf("无此作业，回收失败\n");
}

void printchoose(){
    printf("**************分页式管理************\n");
    printf("*\t\t1.内存分配\t\t*\n");
    printf("*\t\t2.内存去配\t\t*\n");
    printf("*\t\t0.退出\t\t*\n");
}

int main()
{
    setbuf(stdout, NULL);
    //定义作业列表
    linklist *joblist;
    joblist = (linklist *) malloc(sizeof (linklist));
    //定义选择
    int c;
    surplus = 535;
    joblist->next = NULL;
    printf("请输入系统内存空间的大小:");
    scanf("%d", &Msize);
    printf("请输入字长(16/32/64):");
    scanf("%d", &Wlength);
    printf("请输入物理块大小:");
    scanf("%d", &Blength);
    initPos();
    printf("剩余物理块数%d\n", surplus);
    printchoose();
    printf("请输入选项");
    scanf("%d", &c);
    while(c != 0){
        if(c == 1){
            assignment(joblist);
        }else if(c == 2){
            recycle(joblist);
        }
        printchoose();
        printf("请输入选项");
        scanf("%d", &c);
    }
    return 0;
}
