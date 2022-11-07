#include <iostream>
#include <cstdio>
#include <list>
#include <string>
//list容器可以将数据进行链式存储,有很多方便的方法
//list<node>将node形式进行链式存储
//list<node>&是一种引用传递

using namespace std;

//时间结构体

typedef struct data{

    int hour;

    int min;

}Time;

typedef struct link_node{
    string name;			//作业名称
    Time arrive;			//作业入井时间
    int zx;					//作业执行时间
    Time JobTime;			//作业调度时间
    int JobWait;			//作业调度等待时间
    Time ProcessTime;		//进程调度时间
    int ProcessWait;		//进程调度等待时间
    Time finish;			//作业完成时间
    int zz;					//作业周转时间=作业完成时间-作业入井时间
    float zzxs;				//作业带权周转时间=作业周转时间/作业执行时间
    int finished;			//标记作业完成
    float response;			//作业响应化
}node;

/*
输入示例
1
4
JOB1 8:00 120
JOB2 8:50 50
JOB3 9:00 10
JOB4 9:50 20
1
1
4
JOB1 8:00 120
JOB2 8:50 50
JOB3 9:00 10
JOB4 9:50 20
2
1
4
JOB1 8:00 120
JOB2 8:50 50
JOB3 9:00 10
JOB4 9:50 20
3
0
*/



void printfoption(){
    std::cout << "**************作业管理***************\n";
    std::cout << "\t\t*\t\t1.FCFS\t\t*\n";
    std::cout << "\t\t*\t\t2.SJF\t\t*\n";
    std::cout << "\t\t*\t\t3.HRRF\t\t*\n";
    std::cout << "\t\t*\t\t0.退出\t\t*\n";
    std::cout << "请输入要进行的操作号:";
}


//全局变量
Time nowTime;
float avgZZ;
float avgZZXS;

//比较函数

//将时间转换惟分钟
int toMinute(Time t){
    return t.hour*60 + t.min;
}

//将分钟转换为时间
Time toTime(int t){
    Time s;
    s.hour = t/60;
    s.min = t % 60;
    return s;
}

//根据到达时间比较
bool timeCompare(node a, node b){
    return toMinute(a.arrive) < toMinute(b.arrive);
}

//根据运行时间进行比较
bool runtimeCompare(node a, node b){
    return a.zx < b.zx;
}

//根据响应比进行比较
bool respsonseCompare(node a, node b){
    return a.response > b.response;
}

void printFinished(list<node>& finishedJobs, int choose) {
    if (choose == 1) {
        cout << "模拟作业FCFS调度过程的输出结果：" << endl;
    }
    else if (choose == 2) {
        cout << "模拟作业SJF调度过程的输出结果：" << endl;
    }
    else if (choose == 3) {
        cout << "模拟作业HRRF调度过程的输出结果：" << endl;
    }

    printf("名字\t入井时间\t运行时间\t作业调度时间\t作业调度等待时间\t进程调度时间\t进程调度等待时间\t完成时间\t周转时间\t带权周转系数\n");
    for (auto it = finishedJobs.begin(); it != finishedJobs.end(); it++) {
        printf("%s\t%d:%02d\t\t%d(分钟)\t%d:%02d\t\t%d(分钟)  \t\t%d:%02d\t\t%d(分钟)\t\t\t%d:%02d\t\t%d(分钟)\t%.4f\n",
               it->name.c_str(), it->arrive.hour, it->arrive.min, it->zx, it->JobTime.hour, it->JobTime.min, it->JobWait,
               it->ProcessTime.hour, it->ProcessTime.min, it->ProcessWait, it->finish.hour, it->finish.min, it->zz, it->zzxs);
    }
    printf("系统平均周转时间为：%.2f\n", avgZZ);
    printf("系统平均带权周转时间为：%.4f\n", avgZZXS);
}

void FCFS(int n, list<node>& readyJobs, list<node>& backupJobs, list<node>& finishedJobs){
    while(!readyJobs.empty()){
        //弹出就绪队列第一个作业到终止队列中
        //计算相关时间
        //更新进程信息和当前时间
        node t = readyJobs.front();
        readyJobs.pop_front();
        t.JobTime = t.ProcessTime = nowTime;
        t.JobWait = toMinute(t.JobTime) - toMinute(t.arrive);
        t.ProcessWait = 0;
        nowTime = t.finish = toTime(toMinute(t.JobTime) + t.zx);
        t.zz = toMinute(t.finish) - toMinute(t.arrive);
        t.zzxs = t.zz * 1.0 / t.zx;
        //加入到完成队列
        finishedJobs.push_back(t);
        //更新平均周转时间和系数
        avgZZ += t.zz * 1.0 / n;
        avgZZXS += t.zzxs / n;


        //更新就绪队列
        while (!backupJobs.empty() && toMinute(backupJobs.front().arrive) <= toMinute(nowTime)) {
            readyJobs.push_back(backupJobs.front());
            backupJobs.pop_front();
        }
    }
}

void SJF(int n, list<node>& readyJobs, list<node>& backupJobs, list<node>& finishedJobs){
    while(!readyJobs.empty()){
        node t = readyJobs.front();
        readyJobs.pop_front();
        t.JobTime = t.ProcessTime = nowTime;
        t.JobWait = toMinute(t.JobTime) - toMinute(t.arrive);
        nowTime = t.finish = toTime(toMinute(t.JobTime) + t.zx);
        t.zz = toMinute(t.finish) - toMinute(t.arrive);
        t.zzxs = t.zz*1.0/t.zx;
        finishedJobs.push_back(t);
        avgZZ += t.zz*1.0/n;
        avgZZXS += t.zzxs*1.0/n;
        list<node> s;
        while(!backupJobs.empty() && toMinute(backupJobs.front().arrive) <= toMinute(nowTime)){
            s.push_back(backupJobs.front());
            backupJobs.pop_front();
        }
        s.sort(runtimeCompare);
        readyJobs.insert(readyJobs.end(), s.begin(), s.end());
    }

}
void HRRF(int n, list<node>& readyJobs, list<node>& backupJobs, list<node>& finishedJobs){
    while (!readyJobs.empty()) {	//判断就绪队列是否为空
        //更新进程信息和当前时间
        node t = readyJobs.front();
        readyJobs.pop_front();
        t.JobTime = t.ProcessTime = nowTime;
        t.JobWait = toMinute(t.JobTime) - toMinute(t.arrive);
        t.ProcessWait = 0;
        nowTime = t.finish = toTime(toMinute(t.JobTime) + t.zx);
        t.zz = toMinute(t.finish) - toMinute(t.arrive);
        t.zzxs = t.zz * 1.0 / t.zx;
        //加入到完成队列
        finishedJobs.push_back(t);
        //更新平均周转时间和系数
        avgZZ += t.zz * 1.0 / n;
        avgZZXS += t.zzxs / n;

        //对后备队列中到达时间比当前时间小的作业，再按响应比排序
        list<node> s;
        //找出到达时间比当前时间小的作业
        while (!backupJobs.empty() && toMinute(backupJobs.front().arrive) <= toMinute(nowTime)) {
            s.push_back(backupJobs.front());
            backupJobs.pop_front();
        }
        //计算出加入就绪队列的响应比
        //将他们按响应比从大到小排序
        //将他们加入到就绪队列
        readyJobs.insert(readyJobs.end(), s.begin(), s.end());
        for (auto it = readyJobs.begin(); it != readyJobs.end(); it++) {
            //响应比 = 1 + 作业等待时间 / 作业处理时间
            it->response = 1 + (toMinute(nowTime) - toMinute(it->arrive)) * 1.0 / it->zx;
        }
        readyJobs.sort(respsonseCompare);
    }

}

int main()
{
    int operate,num,option;
    char c;		//读取字符
    cout << "请输入操作(1:开始作业调度; 0:结束作业):";
    cin >> option;
    while(option!=0){
        avgZZ = avgZZXS = 0;
        cout << "请输入你需要创建的作业数:";
        cin >> num;
        list<node> readyJobs;	//就绪队列
        list<node> backupJobs;	//后备队列
        list<node> finishedJobs;//已完成队列
        cout << "请依次输入" << endl;
        cout << "作业名\t入井时间\t运行时间(分钟)" << endl;
        for(int i = 0; i < num; i++){
            node job;
            cin >> job.name >> job.arrive.hour >> c >> job.arrive.min >> job.zx;
            backupJobs.push_back(job);
        }
        //先根据到达时间进行排序
        backupJobs.sort(timeCompare);
        nowTime = backupJobs.front().arrive;
        //初始化就绪队列
        readyJobs.push_back(backupJobs.front());
        backupJobs.pop_front();
        printfoption();
        cin >> operate;
        if(operate == 1){
            FCFS(num, readyJobs, backupJobs, finishedJobs);
            printFinished(finishedJobs, operate);
        }else if(operate == 2){
            SJF(num, readyJobs, backupJobs, finishedJobs);
            printFinished(finishedJobs, operate);
        }else if(operate == 3){
            HRRF(num, readyJobs, backupJobs, finishedJobs);
            printFinished(finishedJobs, operate);
        }else if(operate == 0){
            return 0;
        }
        cout << "请输入操作(1:开始作业调度; 0:结束作业):";
        cin >> option;
    }
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
