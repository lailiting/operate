#include <stdio.h>
#include <stdlib.h>

#define M 3 //资源数量
#define N 5 //序列
int Resource[M]; //初始资源数
int Max[N][M]; //最大需求量
int Allocation[N][M]; //已申请的数量
int Need[N][M]; //还需的数量
int Available[M]; //总的剩余空间
int Work[M];
int Finish[N];
int s[N];
int lists[N];
int init = 0;
int List[N];    //存放安全序列的下标序列

void reqresourse(int i,int Request[M]);
//初始化 resourse max allocation
//根据输入计算available need
void initival()
{
    int i, j;
    printf("请输入3类资源初始化的资源数:\n");
    for (i = 0; i < M; i++)
    {
        scanf("%d", &Resource[i]);
        Available[i] = Resource[i];
    }
    printf("请输入5个进程对每种资源的最大需求量:\n");
    for (j = 0; j < N; j++)
    {
        for (i = 0; i < M; i++)
        {
            scanf("%d", &Max[j][i]);
        }
    }
    //初始化
    for (j = 0; j < N; j++)
    {
        for (i = 0; i < M; i++)
        {
            Allocation[j][i] = 0;
            Need[j][i] = Max[j][i] - Allocation[j][i];
        }
    }
    printf("请输入5个进程已经对每种资源申请的数目:\n");
    for (j = 0; j < N; j++)
    {
        printf("p[%d]对三类资源的申请", j+1);
        for (i = 0; i < M; i++)
        {
            scanf("%d", &Allocation[j][i]);
        }
        reqresourse(j, Allocation[j]);
    }


    //计算Available[数组
    for (j = 0; j < M; j++)
    {
        for (i = 0; i < N; i++)
        {
            Available[j] -= Allocation[i][j];
        }
    }
}

//打印初始化的序列
void printList()
{
    int i;
    printf("初始化序列为:\n");
    printf("进程名称| 最大申请资源 | 已经申请资源 | 需要的资源 | 库存\n");
    for (i = 0; i < N; i++)
    {
        if (i == 0)
        {
            printf("进程p%d \t %2d %2d %2d \t %2d %2d %2d \t %2d %2d %2d \t %2d %2d %2d\n", i + 1, Max[i][0], Max[i][1],
                   Max[i][2], Allocation[i][0], Allocation[i][1], Allocation[i][2], Need[i][0], Need[i][1], Need[i][2],
                   Available[0], Available[1], Available[2]);
        } else
        {
            printf("进程p%d \t %2d %2d %2d \t %2d %2d %2d \t %2d %2d %2d\n", i + 1, Max[i][0], Max[i][1], Max[i][2],
                   Allocation[i][0], Allocation[i][1], Allocation[i][2], Need[i][0], Need[i][1], Need[i][2]);
        }
    }
}

void printWork()
{
    int i;
    printf("完成申请后10序列为:\n");
    printf("进程名称| 最大申请资源 | 已经申请资源 | 需要的资源 | 进程完成是否\n");
    for (i = 0; i < N; i++)
    {
        if (s[i] == 0)
        {
            printf("进程p%d \t %2d %2d %2d \t %2d %2d %2d \t %2d %2d %2d \t working\n", i + 1, Max[i][0], Max[i][1],
                   Max[i][2], Allocation[i][0], Allocation[i][1], Allocation[i][2], Need[i][0], Need[i][1], Need[i][2]);
        } else
        {
            printf("进程p%d \t %2d %2d %2d \t %2d %2d %2d \t %2d %2d %2d\t Finished \n", i + 1, Max[i][0], Max[i][1], Max[i][2],
                   Allocation[i][0], Allocation[i][1], Allocation[i][2], Need[i][0], Need[i][1], Need[i][2]);
        }
    }
}


int isFinish()
{
    int i, j, count = 0;
    for (i = 0; i < N; i++)
    {
        count = 0;
        for (j = 0; j < M; j++)
        {
            //满足两个条件 未完成 并且需要的数目小于总资源数
            if (Finish[i] == 0 && Need[i][j] <= Work[j])
            {
                count++;
            }
        }
        if (count == 3)
        {
            Finish[i] = 1;
            for (j = 0; j < M; j++)
            {
                if (Need[i][0] != 0 || Need[i][1] != 0 || Need[i][2] != 0)
                {
                    Work[j] += Allocation[i][j];
                }
            }
            return i;
        }
    }
    return -1;
}


int isSafe()
{
    int i, a, count = 0;
    for (i = 0; i < N; i++)
    {
        Finish[i] = 0;
    }
    //判断是否已经工作完
//    for(i = 0; i < N; i++){
//        if(Need[i][0] != 0 || Need[i][1] != 0 || Need[i][2]!=0){
//            Work[j] += Allocation[i][j];
//        }
//    }
    for (i = 0; i < N; i++)
    {
        a = isFinish();
        if (a != -1)
        {
            List[i] = a;
            if(s[i] != 1){
                lists[i] = a+1;
            }else{
                lists[i] = 0;
            }
            count++;
        }
    }
    if (count == 5)
    {
        return 1;
    } else
        return 0;
}

void printSafe()
{
    int i, j, a;
    printf("序列为:\n");
    printf("进程名称| work资源数目 | work+Allocation数目 | 已申请资源数目 | 需要的资源 | 是否安全\n");
    for (i = 0; i < M; i++)
    {
        Work[i] = Available[i];
    }
    for (i = 0; i < N; i++)
    {
        a = List[i];
        printf("进程p%d \t %2d %2d %2d \t\t %2d %2d %2d \t\t %2d %2d %2d \t %2d %2d %2d \t %2d\n", a + 1, Work[0], Work[1],
               Work[2], Allocation[a][0] + Work[0], Allocation[a][1] + Work[1], Allocation[a][2] + Work[2],
               Allocation[a][0], Allocation[a][1], Allocation[a][2], Need[a][0], Need[a][1], Need[a][2], Finish[a]);
        for (j = 0; j < M; j++)
        {
            if (Need[i][0] != 0 || Need[i][1] != 0 || Need[i][2] != 0)
            {
                Work[j] += Allocation[a][j];
            }
        }
    }
}

//先假设当前申请资源满足
void reqresourse(int i, int Request[M])
{
    //申请了对All Nees Avia数组进行处理
    int count1, count2, j, countN = 0;
    //判断当前借出的进程是否已经借完全部资源，如果借出且为安全序列，最后应将资源归还
    count1 = count2 = 0;
    for (j = 0; j < M; j++)
    {
        if (Request[j] <= Need[i][j])
        {
            count1++;
        }
    }
    for (j = 0; j < M; j++)
    {
        if (Request[j] <= Available[j])
        {
            count2++;
        }
    }
    if (count1 != 3 || count2 != 3)
    {
        if(count1 !=3){
            printf("你输入的资源数超出需要范围\n申请失败\n");
        }
        if(count2 != 3){
            printf("你输入的资源数超出可支配范围\n申请失败\n");
        }
        return;
    }
    if (count1 == 3 && count2 == 3)
    {
        for (j = 0; j < M; j++)
        {
            if(init == 1){
                Allocation[i][j] += Request[j];
            }
            Need[i][j] -= Request[j];
            Available[j] -= Request[j];
        }
        for (j = 0; j < M; j++)
        {
            Work[j] = Available[j];
        }
        for (j = 0; j < M; j++)
        {
            if (Need[i][j] == 0)
            {
                countN++;
            }
        }
        if (countN == 3)
        {
            for (j = 0; j < M; j++)
            {
                Work[j] += Allocation[i][j];
            }
        }
        if (isSafe() == 0)
        {
            printf("\n无安全序列,申请不成功\n");
            //不是安全序列要将资源返回
            for (j = 0; j < M; j++)
            {

                   Allocation[i][j] -= Request[j];
                   Need[i][j] += Request[j];
                Available[j] += Request[j];
            }
            printf("当前资源总数:%2d %2d %2d\n", Available[0], Available[1], Available[2]);
        } else
        {
            printf("申请成功\n");

            countN = 0;
            for (j = 0; j < M; j++)
            {
                if (Need[i][j] == 0)
                {
                    countN++;
                }
            }
            if (countN == 3)
            {
                for (j = 0; j < M; j++)
                {
                    Available[j] += Allocation[i][j];
                    s[i] = 1;
                }
            }
            printf("安全序列为:");
            for(j = 0; j < N; j++){
                if(lists[j] != 0){
                    printf("p%d->", lists[j]);
                }
            }
            printWork();
            printf("当前资源总数:%2d %2d %2d\n", Available[0], Available[1], Available[2]);
        }
    }
}


int main()
{
    setbuf(stdout, NULL);
    system("color f0");
    //i为当前需要申请资源的id
    int i, Request[M], j;
    for(j = 0; j < N; j++){
        s[j] = 0;
    }
    initival();
    init = 1;
    Available[0] = 3;
    Available[1] = 3;
    Available[2] = 1;
    printf("请输入当前需要申请资源进程编号[1-5]:\n");
    scanf("%d", &i);
    while (i >= 0)
    {
        printf("请输入进程p%d对三类资源的申请:", i);
        for (j = 0; j < M; j++)
        {
            scanf("%d", &Request[j]);
        }
        reqresourse(i - 1, Request);
        printf("请输入当前需要申请申请资源进程编号[1-5]:\n");
        scanf("%d", &i);
    }
    return 0;
}
