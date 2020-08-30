#include <stdio.h>
#include <stdlib.h>

// ����һ�������ֵ�ĺ���
#define abs(a,b) a>b?(a-b):(b-a)

// ģ��ʮ�����̣������Ŵ�С����ģ�����Ⱥ󵽴�˳�򣬶�Ӧ��ֵ�Ǵŵ�λ��
int process[10] = {0};
// ��ǰ��ͷ���ڴŵ�
int now_disk = 100;

//Ϊÿ�����������������Ҫ�Ĵŵ���
void create_pro(){
    for(int i=0;i<10;i++){
        // ���ôŵ���СΪ1�����Ϊ150
        process[i] = rand() % 150 +1;
        printf("����%d��%d ",(i+1),process[i]);
    }
    printf("\n");
}

// �����ȷ����㷨
void FCFS(){
    int avg = 0;
    int dis;
    printf("��ʼ��ͷ�ŵ�����%d\n",now_disk);
    printf("| next move | distance |\n");
    for(int i=0;i<10;i++){
        // ��ȡ��ͷ�ƶ�����
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }
    printf("ƽ��Ѱ��ʱ�䣺%.2f\n",float(avg)/10);
}

// ����뵱ǰ��ͷ���ڴŵ�������Ľ��̺�
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

// ���Ѱ��ʱ������
void SSTF(){
    int avg = 0;
    int dis;
    int index;
    printf("��ʼ��ͷ�ŵ�����%d\n",now_disk);
    printf("| next move | distance |\n");
    for(int i=0;i<10;i++){
        index = getShortest();
        dis = abs(process[index],now_disk);
        printf("|   %5d   |   %4d   |\n",process[index],dis);
        now_disk = process[index];
        process[index] = -1;
        avg += dis;
    }
    printf("ƽ��Ѱ��ʱ�䣺%.2f\n",float(avg)/10);
}

// �Խ������鰴��С��������
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

// ɨ���㷨
void SCAN(){
    int index;
    int dis;
    int avg=0;
    // �ȶԽ������鰴��С��������
    pro_sort();
    for(int i=0;i<10;i++){
        if(process[i]>now_disk){
            index = i;
            break;
        }
    }

    printf("��ʼ��ͷ�ŵ�����%d\n",now_disk);
    printf("| next move | distance |\n");

    for(int i=index;i<10;i++){
        // ��ȡ��ͷ�ƶ�����
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }

    for(int i=index-1;i>=0;i--){
        // ��ȡ��ͷ�ƶ�����
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }

    printf("ƽ��Ѱ��ʱ�䣺%.2f\n",float(avg)/10);
}

// ѭ��ɨ���㷨
void CSCAN(){
    int index;
    int dis;
    int avg=0;
    // �ȶԽ������鰴��С��������
    pro_sort();
    for(int i=0;i<10;i++){
        if(process[i]>now_disk){
            index = i;
            break;
        }
    }

    printf("��ʼ��ͷ�ŵ�����%d\n",now_disk);
    printf("| next move | distance |\n");

    for(int i=index;i<10;i++){
        // ��ȡ��ͷ�ƶ�����
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }

    for(int i=0;i<index;i++){
        // ��ȡ��ͷ�ƶ�����
        dis = abs(process[i],now_disk);
        printf("|   %5d   |   %4d   |\n",process[i],dis);
        now_disk = process[i];
        avg += dis;
    }

    printf("ƽ��Ѱ��ʱ�䣺%.2f\n",float(avg)/10);
}

int main(){
    create_pro();
    //FCFS();
    //SSTF();
    //SCAN();
    CSCAN();
    return 0;
}
