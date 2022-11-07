#include <cstdio>
//#include <malloc>
#include <string>
#include <cstdlib>
#include <string>
#include <iostream>
//#include <sys/malloc.h>
int block;
using namespace std;

//打印菜单栏
void printmenu() {
    printf("**********请求分页式存储管理*8************\n");
    printf("1.FIFO分配\n");
    printf("2.LRU(LFU)分配\n");
    printf("0.退出\n");
    cout << "请输入选项";
}

//FIFO分配
void funFIFO(){
    string jobname;
    int joblen, joblist[20], ans[10][30] , i, j, num = 0, pos=0, stopc = 0;
    cout << "请输入作业名:";
    cin >> jobname;
    cout << "请输入作业页面的长度:";
    cin >> joblen;
    cout << "请输入页面顺序";
    for(i =0;i<joblen; i++){
        cin >> joblist[i];
    }

    for(i = 0; i<block+2; i++){
        for(j = 0; j<joblen; j++){
            ans[i][j] = -1;
        }
    }

    //进程调度
        for(j = 0; j<joblen; j++){
            //记录是否在内存中
            num = 0;
            for(i = 0; i<block; i++){
                if(j != 0){
                    if(ans[i][j-1] == joblist[j]){
                        break;
                    }else{
                        num++;
                    }
                }else{
                    num = block;
                    break;
                }
            }

            //如果在内存中不处理，不在内存中
            if(num == block){
                //如果页面已经满了置换，如果没满放
                pos = 0;
                for(i = 0; i <block; i++){
                    if(j != 0){
                        if(ans[i][j-1] != -1){
                            pos++;
                        }else{
                            break;
                        }
                    }else{
                        break;
                    }
                }
                if(pos == block){
                    //先把前一列的拷贝一下
                   if(j != 0){
                       for(i = 0; i < block; i++){
                           ans[i][j] = ans[i][j-1];
                       }
                   }
                    //保存被置换出去的页面
                    ans[block][j] = 0;
                    ans[block+1][j] = ans[0][j];
                    for(i = 0; i < block-1; i++){
                        ans[i][j] = ans[i+1][j];
                    }
                    ans[block-1][j] = joblist[j];
                }else{
                    if(j != 0){
                        for(i = 0; i < block; i++){
                            ans[i][j] = ans[i][j-1];
                        }
                    }
                    ans[block][j] = 0;
                    ans[pos][j] = joblist[j];
                }
                stopc++;
            }else{
                if(j != 0){
                    for(i = 0; i < block; i++){
                        ans[i][j] = ans[i][j-1];
                    }
                }
            }
        }

    //打印进程调度
    for(j = 0; j < joblen; j++){
        printf("%4d", j);
    }
    printf("\n");
    for(j = 0; j < joblen; j++){
        printf("%4d", joblist[j]);
    }
    printf("\n");
    for(i = 0; i < block +2; i++){
        for(j = 0; j < joblen; j++){
            if( i == block){
                if(ans[i][j] != 0){
                    printf("%4c", ' ');
                }else{
                    printf("%4c", '+');
                }
            }else{
                if(ans[i][j] != -1){
                    printf("%4d", ans[i][j]);
                }else{
                    printf("\t");
                }
            }
        }
        cout << endl;
    }
    printf("缺页中断率为%3d.00", stopc*100/joblen);
}

void funLRU(){
    //再利用一个二维数组，记录页面，以及页面最近被访问的次数
    int recodelist[block][2], i, j, maxp, posp, stopc=0;
    //maxp找最近最少被使用的 posp为位置
    for(i = 0; i < block; i++){
        recodelist[i][0] = -1;
        recodelist[i][1] = 0;
    }

    //输入进程调度
    string jobname;
    int joblen, joblist[20], ans[10][30] , num = 0, pos=0;
    cout << "请输入作业名:";
    cin >> jobname;
    cout << "请输入作业页面的长度:";
    cin >> joblen;
    cout << "请输入页面顺序";
    for(i =0;i<joblen; i++){
        cin >> joblist[i];
    }

    for(i = 0; i<block+2; i++){
        for(j = 0; j<joblen; j++){
            ans[i][j] = -1;
        }
    }
    //循环查找，如果在或者添加就置为0，其他则加一
    for(j = 0; j<joblen; j++){
        //记录是否在内存中
        num = 0;
        for(i = 0; i<block; i++){
            if(j != 0){
                if(ans[i][j-1] == joblist[j]){
                    break;
                }else{
                    num++;
                }
            }else{
                num = block;
                break;
            }
        }

        //如果在内存中不处理，不在内存中
        if(num == block){
            //如果页面已经满了置换，如果没满放
            pos = 0;
            for(i = 0; i <block; i++){
                if(j != 0){
                    if(ans[i][j-1] != -1){
                        pos++;
                    }else{
                        break;
                    }
                }else{
                    break;
                }
            }
            if(pos == block){
                //先把前一列的拷贝一下
                maxp = 0;
                posp = 0;
                if(j != 0){
                    for(i = 0; i < block; i++){
                        recodelist[i][1]++;
                    }
                    for(i = 0; i < block; i++){
                        if(recodelist[i][1] > maxp){
                            maxp = recodelist[i][1];
                            posp= i;
                        }
                    }
                }
                //保存被置换出去的页面
                ans[block][j] = 0;
                ans[block+1][j] = recodelist[posp][0];
                recodelist[posp][0] = joblist[j];
                recodelist[posp][1] = 0;
                for(i = 0; i < block; i++){
                    ans[i][j] =recodelist[i][0];
                }

            }else{
                posp = 0;
                for(i = 0; i < block; i++){
                    if(j != 0){
                        if(ans[i][j-1] == -1){
                            posp = i;
                            break;
                        }else{
                            recodelist[i][1]++;
                        }
                    }else{
                        break;
                    }
                }

                recodelist[posp][0] = joblist[j];
                recodelist[posp][1] = 0;
                ans[block][j] = 0;
                for(i = 0; i < block; i++){
                    ans[i][j] = recodelist[i][0];
                }
            }
            stopc++;
        }else{
            if(j != 0){
                for(i = 0; i < block; i++){
                    ans[i][j] = ans[i][j-1];
                }
                for(i = 0; i < block; i++){
                    recodelist[i][1]++;
                }
                recodelist[num][1] = 0;
            }
        }
    }



    //打印进程调度
    for(j = 0; j < joblen; j++){
        printf("%4d", j);
    }
    printf("\n");
    for(j = 0; j < joblen; j++){
        printf("%4d", joblist[j]);
    }
    printf("\n");
    for(i = 0; i < block +2; i++){
        for(j = 0; j < joblen; j++){
            if( i == block){
                if(ans[i][j] != 0){
                    printf("%4c", ' ');
                }else{
                    printf("%4c", '+');
                }
            }else{
                if(ans[i][j] != -1){
                    printf("%4d", ans[i][j]);
                }else{
                    printf("\t");
                }
            }
        }
        cout << endl;
    }
    printf("缺页中断率为%3d.00\n", stopc*100/joblen);
}

int main() {
    setbuf(stdout, NULL);
    int choose;
    cout << "请输入物理快的块数:";
    cin >> block;
    printmenu();
    cin >> choose;
    while(choose != 0){
        if(choose == 1){
            funFIFO();
        }else{
            funLRU();
        }
        printmenu();
        cin >> choose;
    }
    return 0;
}