#include <stdio.h>
#include <stdlib.h>

// 定义一个求绝对值的函数
#define abs(a,b) a>b?(a-b):(b-a)

// 模拟十个进程，索引号从小到大模拟它先后到达顺序，对应的值是磁道位置
int process[10] = {0};
// 当前磁头所在磁道
int now_disk = 100;

//为每个进程随机产生所需要的磁道数
void create_pro(){
    for(int i=0;i<10;i++){
        // 设置磁道最小为1，最大为150
        process[i] = rand() % 150 +1;
        printf("进程%d：%d ",(i+1),process[i]);
    }
    printf("\n");
}

// 先来先服务算法
void FCFS(){
    int avg = 0;
    int dis;
    printf("初始磁头磁道数：%d\n",now_disk);
    printf("| next move | distance |\n");
    for(int i=0;i<10;i++){
        // 获取磁头移动距离
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }
    printf("平均寻道时间：%.2f\n",float(avg)/10);
}

// 获得与当前磁头所在磁道数最近的进程号
int getShortest(){
    int index;
    int mini = 151;
    for(int i=0;i<10;i++){
        if(process[i] == -1)
            continue;
        if(abs(process[i],now_disk)<mini){
            mini = abs(process[i],now_disk);
            index = i;
        }
    }
    return index;
}

// 最短寻道时间优先
void SSTF(){
    int avg = 0;
    int dis;
    int index;
    printf("初始磁头磁道数：%d\n",now_disk);
    printf("| next move | distance |\n");
    for(int i=0;i<10;i++){
        index = getShortest();
        dis = abs(process[index],now_disk);
        printf("|   %5d   |   %4d   |\n",process[index],dis);
        now_disk = process[index];
        process[index] = -1;
        avg += dis;
    }
    printf("平均寻道时间：%.2f\n",float(avg)/10);
}

// 对进程数组按从小到大排序
void pro_sort(){
    int temp;
    for(int i=0;i<9;i++)
        for(int j = 0;j<10-1-i;j++){
            if(process[j] > process[j+1]){
                temp = process[j];
                process[j] = process[j+1];
                process[j+1] = temp;
            }
        }
    for(int i=0;i<10;i++)
        printf("%d ",process[i]);
}

// 扫描算法
void SCAN(){
    int index;
    int dis;
    int avg=0;
    // 先对进程数组按从小到大排列
    pro_sort();
    for(int i=0;i<10;i++){
        if(process[i]>now_disk){
            index = i;
            break;
        }
    }

    printf("初始磁头磁道数：%d\n",now_disk);
    printf("| next move | distance |\n");

    for(int i=index;i<10;i++){
        // 获取磁头移动距离
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }

    for(int i=index-1;i>=0;i--){
        // 获取磁头移动距离
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }

    printf("平均寻道时间：%.2f\n",float(avg)/10);
}

// 循环扫描算法
void CSCAN(){
    int index;
    int dis;
    int avg=0;
    // 先对进程数组按从小到大排列
    pro_sort();
    for(int i=0;i<10;i++){
        if(process[i]>now_disk){
            index = i;
            break;
        }
    }

    printf("初始磁头磁道数：%d\n",now_disk);
    printf("| next move | distance |\n");

    for(int i=index;i<10;i++){
        // 获取磁头移动距离
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }

    for(int i=0;i<index;i++){
        // 获取磁头移动距离
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }

    printf("平均寻道时间：%.2f\n",float(avg)/10);
}

int main(){
    create_pro();
    //FCFS();
    //SSTF();
    //SCAN();
    CSCAN();
    return 0;
}
