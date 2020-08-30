
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXSIZE 20
// ����������ʱ��
#define LEN sizeof(PCB)
// ����ʱ��Ƭ����
#define TLEN 4

// ������̿��ƿ�ṹ��
typedef struct node{
    char name[20];      // ���̵�����
    int prio;           // ������������Ӧ��
    int needtime;       // ��������Ҫ��ʱ��
    char status;        // ���̵�״̬����W����ʾ��������R����ʾִ�У���F����ʾ���
    struct node *next;  // ָ�������һ�����
}PCB;

// �����������д�Ž���
PCB *ready=NULL, *run=NULL, *finish=NULL;
// ��ʼ���ܵ���תʱ���δ������ɶ��еĽ�����
int time_z = 0;
int pro_num;


// ����������
void show_quet(){
    PCB *p;
    p=ready;
    printf("|-------------------------------------------|\n");
    printf("|Waiting prosess:                           |\n");
    printf("|-------------------------------------------|\n");
    printf("|    name    priority    needtime    status |\n");
    printf("|-------------------------------------------|\n");
    while(p!=NULL){
        printf("|    %s    %8d    %8d        %c    |\n",p->name,p->prio,p->needtime,p->status);
        p=p->next;
    }
    printf("|-------------------------------------------|\n");

    printf("\n\n");
    p=run;
    printf("|-------------------------------------------|\n");
    printf("|Runing prosess:                           |\n");
    printf("|-------------------------------------------|\n");
    printf("|    name    priority    needtime    status |\n");
    printf("|-------------------------------------------|\n");
    while(p!=NULL){
        printf("|    %s    %8d    %8d        %c    |\n",p->name,p->prio,p->needtime,p->status);
        p=p->next;
    }
    printf("|-------------------------------------------|\n");

    printf("\n\n");
    p=finish;
    printf("|-------------------------------------------|\n");
    printf("|Finished prosess:                           |\n");
    printf("|-------------------------------------------|\n");
    printf("|    name    priority    needtime    status |\n");
    printf("|-------------------------------------------|\n");
    while(p!=NULL){
        printf("|    %s    %8d    %8d        %c    |\n",p->name,p->prio,p->needtime,p->status);
        p=p->next;
    }
    printf("|-------------------------------------------|\n");
}

// ���Ⱥ�˳������������
void Insert_Wt(PCB *w){
    PCB *p;
    p=ready;

    if(NULL==ready){
        w->next=ready;
        ready=w;
    }else{
        while(p->next!=NULL)
            p=p->next;
        w->next=p->next;
        p->next=w;
    }
}

// ������ɶ���
void Insert_Ft(PCB *f){
    PCB *p;
    p=finish;
    if(NULL==finish){
        f->next=finish;
        finish=f;
    }else{
        // ��Ѱ����ɶ��е����һ��λ��
        while(NULL!=p->next){
            p=p->next;
        }
        f->next=p->next;
        p->next=f;
    }
}

// �����½���
void Create_prot(int num){
    PCB *newpro;
    int i;
    for(i=1;i<=num;i++){
        // �����ڴ�ռ�
        if(NULL==(newpro=(PCB*)malloc(LEN))){
            exit(0);
        }
        char a[4]="s";
        char c[2];
        itoa(i,c,10);
        strcat(a,c);
        strcpy(newpro->name,a);
        newpro->needtime=rand()%20+1;
        newpro->prio=0;
        newpro->status='W';
        // �������ȼ����뵽����������
        Insert_Wt(newpro);
    }
}

// ���������еĵ�һ���ڵ㸽��ִ�ж�����
void GetFirstt(PCB *w){
    if(w!=NULL)
        Insert_Wt(w);
    run=ready;
    if(NULL!=ready){
        run->status='R';
        ready=ready->next;
        run->next=NULL;
    }
}

// ʱ��Ƭ��ת����
void Priorityt(){
    int time;
    show_quet();
    GetFirstt(NULL);
    while(run!=NULL){
        // ÿ�δ�ӡ�����ڵ��״̬
        show_quet();
        time = 0;
        while(run->needtime&&time<TLEN){
            printf("��ǰ%s��������ִ�У�����Ҫ%d��ִ�����...\n",run->name,run->needtime);
            run->needtime--;
            time_z += pro_num;
            time++;
        }
        if(run->needtime==0){
            run->status='F';
            Insert_Ft(run);
            pro_num--;
            GetFirstt(NULL);
        }
        else{
            run->status='W';
            GetFirstt(run);
        }
        }
    }

// ������
int main(){
    int num;
    printf("��������Ҫ����Ľ���������");
    scanf("%d",&num);
    pro_num = num;
    Create_prot(num);
    Priorityt();
    printf("ƽ����תʱ��Ϊ��%f\n",float(time_z)/num);
    return 0;
}


