#include "queue.h"

int main()
{
    system("color f0");
    linkQueue *stuqueue;
    stuqueue = (linkQueue*)malloc(sizeof(linkQueue));
    init(stuqueue);
    int x, choice;
    printf("初始化学生信息:1继续 0结束:");
    scanf("%d",&x);
    while(x == 1){
        addStu(stuqueue);
        printf("\n1继续 0结束:");
        scanf("%d",&x);
    }
    printStu(stuqueue);
    x = 1;
    while(x == 1){
        printf("\n学生信息队列基本功能菜单\n");
        printf("========================================\n");
        printf("1.添加学生信息(入队)\n");
        printf("2.删除学生信息(出队)\n");
        printf("3.显示学生信息(遍历)\n");
        printf("4.修改学生信息(修改)\n");
        printf("5/查找学生信息(检索)\n");
        printf("6.退出学生信息队列操作\n");
        printf("========================================\n");
        printf("请选择功能:");
        scanf("%d", &choice);
        if(choice == 1)
            addStu(stuqueue);
        else if(choice == 2)
            delStu(stuqueue);
        else if(choice == 3)
            printStu(stuqueue);
        else if(choice == 4)
            changeStu(stuqueue);
        else if(choice == 5)
            findStu(stuqueue);
        else if(choice == 6){
            break;
        }else{
            printf("操作不正确:");
        }
    }
    return 0;
}
