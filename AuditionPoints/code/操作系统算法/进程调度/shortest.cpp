#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXSIZE 20
// ����������ʱ��
#define LEN sizeof(PCB)

// ������̿��ƿ�ṹ��
typedef struct node{
    char name[4];      // ���̵�����
    int prio;           // ������������Ӧ��
    int needtime;       // ��������Ҫ��ʱ��
    char status;        // ���̵�״̬����W����ʾ��������R����ʾִ�У���F����ʾ���
    struct node *next;  // ָ�������һ�����
}PCB;

// �����������д�Ž���
PCB *ready=NULL, *run=NULL, *finish=NULL;
// ��ʼ���ܵ���תʱ���δ������ɶ��еĽ�����
int time = 0;
int pro_num;

// ����������
void show_que(){
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

// �����ȼ������������
void Insert_W(PCB *w){
    PCB *p,*q;
    p=ready;
    q=p;
    if(NULL==ready||ready->prio<w->prio){
        w->next=ready;
        ready=w;
    }else{
        while(p->next!=NULL){
            q=p->next;
            if(q->prio<w->prio){
                w->next=q;
                p->next=w;
                break;
            }
            p=q;
        }
        if(p->next==NULL){
            w->next=p->next;
            p->next=w;
        }
    }
}

// �����½���
void Create_pro(int num){
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
        // ����Ҫʱ��Խ�̣����ȼ�Խ��
        newpro->prio=MAXSIZE-newpro->needtime;
        newpro->status='W';
        // �������ȼ����뵽����������
        Insert_W(newpro);
    }
}

// ������ɶ���
void Insert_F(PCB *f){
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

// ���������еĵ�һ���ڵ㸽��ִ�ж�����
void GetFirst(){
    run=ready;
    if(NULL!=ready){
        run->status='R';
        ready=ready->next;
        run->next=NULL;
    }
}

// �̽������ȵ���
void Priority(){
    show_que();
    GetFirst();
    while(run!=NULL){
        // ÿ�δ�ӡ�����ڵ��״̬
        show_que();
        while(run->needtime){
            printf("��ǰ%s��������ִ�У�����Ҫ%d��ִ�����...\n",run->name,run->needtime);
            run->needtime--;
            time += pro_num;
        }
        Insert_F(run);
        pro_num--;
        GetFirst();
        }
    }

// ������
int main(){
    int num;
    printf("��������Ҫ����Ľ���������");
    scanf("%d",&num);
    pro_num = num;
    Create_pro(num);
    Priority();
    printf("ƽ����תʱ��Ϊ��%f\n",float(time)/num);
    return 0;
}
