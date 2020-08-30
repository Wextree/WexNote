#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXSIZE 20
// 定义进程最大时间
#define LEN sizeof(PCB)

// 定义进程控制块结构体
typedef struct node{
    char name[20];      // 进程的名字
    int prio;           // 优先数或者响应比
    int needtime;       // 进程所需要的时间
    char status;        // 进程的状态，“W”表示就绪，“R”表示执行，“F”表示完成
    struct node *next;  // 指向队列下一个结点
}PCB;

// 定义三个队列存放进程
PCB *ready=NULL, *run=NULL, *finish=NULL;
// 初始化总的周转时间和未进入完成队列的进程数
int time = 0;
int pro_num;


// 输出队列情况
void show_quep(){
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

// 按优先级插入就绪队列
void Insert_Wp(PCB *w){
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

// 创建新进程
void Create_prop(int num){
    PCB *newpro;
    int i;
    for(i=1;i<=num;i++){
        // 申请内存空间
        if(NULL==(newpro=(PCB*)malloc(LEN))){
            exit(0);
        }
        char a[4]="s";
        char c[2];
        itoa(i,c,10);
        strcat(a,c);
        strcpy(newpro->name,a);
        newpro->needtime=rand()%20+1;
        // 所需要时间越短，优先级越高
        newpro->prio=MAXSIZE-newpro->needtime;
        newpro->status='W';
        // 按照优先级插入到就绪队列中
        Insert_Wp(newpro);
    }
}

// 插入完成队列
void Insert_Fp(PCB *f){
    PCB *p;
    p=finish;
    if(NULL==finish){
        f->next=finish;
        finish=f;
    }else{
        // 搜寻到完成队列的最后一个位置
        while(NULL!=p->next){
            p=p->next;
        }
        f->next=p->next;
        p->next=f;
    }
}

// 将就绪队列的第一个节点附给执行队列中
void GetFirstp(PCB *w){
    if(w!=NULL)
        Insert_Wp(w);
    run=ready;
    if(NULL!=ready){
        run->status='R';
        ready=ready->next;
        run->next=NULL;
    }
}


// 响应比优先调度
void Priorityp(){
    show_quep();
    GetFirstp(NULL);
    while(run!=NULL){
        // 每次打印各个节点的状态
        show_quep();

        printf("当前%s进程正在执行，还需要%d秒执行完毕...\n",run->name,run->needtime);
        run->needtime--;
        time += pro_num;
        run->prio-=2;

        if(run->needtime==0){
            run->status='F';
            Insert_Fp(run);
            pro_num--;
            GetFirstp(NULL);
        }
        else{
            run->status='W';
            GetFirstp(run);
        }
        }
    }
// 主函数
int main(){
    int num;
    printf("请输入你要插入的进程数量：");
    scanf("%d",&num);
    pro_num = num;
    Create_prop(num);
    Priorityp();
    printf("平均周转时间为：%f\n",float(time)/num);
    return 0;
}



