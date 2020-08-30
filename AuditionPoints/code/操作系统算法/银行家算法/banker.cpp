#include <stdio.h>
#include <stdlib.h>

#define PROCESSNUM 5    // 定义进程数量
#define RESOURSE_A 10  // 定义各种资源数量
#define RESOURSE_B 15  // 定义各种资源数量
#define RESOURSE_C 12  // 定义各种资源数量


// 定义进程结构体
typedef struct node{
    char name;     // 进程名
    int MaxNeed[3];     // 各类资源最大需要资源数
    int Allocated[3];    // 各类资源已经分配的数量
    int WantRes[3] ={0};    // 下一次申请的资源数
    char status;        // 表示它的状态：W,F,B
    struct node* next;
}PCB;



// 创建三个队列，分别是就绪，完成和阻塞
PCB *Wait = NULL;
PCB *Finish = NULL;
PCB *Block = NULL;
int res[3] = {RESOURSE_A, RESOURSE_B, RESOURSE_C};

// 产生随机数1至最高high
int random(int high){
    if(high==0)
    	return 0;
    return rand()%high+1;
}

// 向就绪队列末尾插入结点
void Insert_W(PCB *w){
    PCB *p;
    p=Wait;

    if(NULL==Wait){
        w->next=Wait;
        Wait=w;
    }else{
        while(p->next!=NULL)
            p=p->next;
        w->next=p->next;
        p->next=w;
    }
}

// 向完成队列末尾插入结点
void Insert_F(PCB *f){
    PCB *p;
    p=Finish;

    if(NULL==Finish){
        f->next=Finish;
        Finish=f;
    }else{
        while(p->next!=NULL)
            p=p->next;
        f->next=p->next;
        p->next=f;
    }
}

// 向阻塞队列末尾插入结点
void Insert_B(PCB *b){
    PCB *p;
    p=Block;

    if(NULL==Block){
        b->next=Block;
        Block=b;
    }else{
        while(p->next!=NULL)
            p=p->next;
        b->next=p->next;
        p->next=b;
    }
}

// 初始化就绪队列
void InitPro(int num){
    for(int i=0;i<num;i++){
        PCB *w = (PCB*)malloc(sizeof(PCB));
        w->name = i +'0';

        w->MaxNeed[0] = random(RESOURSE_A);
        w->MaxNeed[1] = random(RESOURSE_B);
        w->MaxNeed[2] = random(RESOURSE_C);

        w->Allocated[0] = 0;
        w->Allocated[1] = 0;
        w->Allocated[2] = 0;

        w->status = 'W';
        Insert_W(w);
    }
}

// 显示队列情况
void show_status(){
    printf("现在各个资源数有：A：%d   B：%d   C：%d\n",res[0],res[1],res[2]);
    printf("===================================================\n");
    PCB *w = Wait;
    printf("就绪队列（Wait）：\n");
    printf("\tname\tMax_A\tMax_B\tMax_C\tAlloc_A\tAlloc_B\tAlloc_C\tstatus\n");
    while(w!=NULL){
        printf("\t%c   \t%5d\t%5d\t%5d\t%7d\t%7d\t%7d\t %c\n",
               w->name,w->MaxNeed[0],w->MaxNeed[1],w->MaxNeed[2],
               w->Allocated[0],w->Allocated[1],w->Allocated[2],w->status);
        w = w->next;
    }

    printf("\n");
    printf("===================================================\n");
    PCB *f = Finish;
    printf("完成队列（Finish）：\n");
    printf("\tname\tMax_A\tMax_B\tMax_C\tAlloc_A\tAlloc_B\tAlloc_C\tstatus\n");
    while(f!=NULL){
        printf("\t%c   \t%5d\t%5d\t%5d\t%7d\t%7d\t%7d\t %c\n",
               f->name,f->MaxNeed[0],f->MaxNeed[1],f->MaxNeed[2],
               f->Allocated[0],f->Allocated[1],f->Allocated[2],f->status);
        f = f->next;
    }

    printf("\n");
    printf("===================================================\n");
    PCB *b = Block;
    printf("阻塞队列（Block）：\n");
    printf("\tname\tMax_A\tMax_B\tMax_C\tAlloc_A\tAlloc_B\tAlloc_C\tstatus\n");
    while(b!=NULL){
        printf("\t%c   \t%5d\t%5d\t%5d\t%7d\t%7d\t%7d\t %c\n",
               b->name,b->MaxNeed[0],b->MaxNeed[1],b->MaxNeed[2],
               b->Allocated[0],b->Allocated[1],b->Allocated[2],b->status);
        b = b->next;
    }
}

// 获取就绪队列的长度
int getNum(){
    PCB *w = Wait;
    int count_w = 0;
    while(w){
        count_w++;
        w = w->next;
    }
    return count_w;
}

// 设置就绪队列标志位
void setLabel(){
    PCB *w = Wait;
    while(w){
        w->status = 'W';
        w = w->next;
    }
}

// 安全性检测
int SafeCheck(){
    // 创建一个辅助数组去复制现有的资源数
    int res_t[3];
    for(int i=0;i<3;i++){
    	res_t[i] = res[i];
	}

    PCB *w;
    int num = getNum();
    int i = 0;

    while(i<num){
        w = Wait;
        while(w){
            // 标记位为F表示已经被释放过了，继续遍历其他
            if(w->status == 'F'){
                w = w->next;
                continue;
            }
            // 计算现存资源能否满足就绪队列中某个进程的剩下所需资源数
            if(res_t[0]>=(w->MaxNeed[0]-w->Allocated[0]) &&
               res_t[1]>=(w->MaxNeed[1]-w->Allocated[1]) &&
               res_t[2]>=(w->MaxNeed[2]-w->Allocated[2])){
                    // 可以保证完成即释放内存
                    for(int  ii=0;ii<3;ii++){
                        res_t[ii] += w->Allocated[ii];
                    }
                    w->status = 'F';
                    break;
            }
            w = w->next;
        }
        // 如果有一次循环走到最后都没有找到可以释放内存的进程，表示安全检测失败
        // 恢复标志位然后返回失败
        if(NULL == w){
            // 因为利用了进程的状态作为标志位，所以要重新设置回来
            setLabel();
            return 0;
        }
        i++;
    }
    setLabel();
    return 1;

}

// 删除阻塞队列结点
void Delete_B(PCB *b){
    if(b==Block){
        Block = Block->next;
    }else{
        PCB *p,*q;
        p = Block;
        q = p;
        while(p->next!=NULL){
            q = p->next;
            if(q == b){
                p->next = q->next;
                break;
            }
            p = q;
        }
    }
}

// 银行家算法
void Banker(){
    PCB *p;
    show_status();
    // 检查就绪队列和阻塞队列中还有没有进程
    while(Wait!=NULL||Block!=NULL){
        // 取出就绪队列的头结点进行判断
        p = Wait;
//        Wait = Wait->next;

        // 随机申请资源数
        for(int i=0;i<3;i++){
            p->WantRes[i] = random(p->MaxNeed[i]-p->Allocated[i]);
        }
        printf("取出进程%c，它所申请的资源：A：%d   B：%d   C：%d\n",p->name,p->WantRes[0],p->WantRes[1],p->WantRes[2]);

        // 判断现在系统资源数足不足够，足够进入安全性检测，不足进去阻塞队列
        for(int i=0;i<3;i++){
            if(p->WantRes[i] > res[i]){
            	Wait = Wait->next;
                p->status = 'B';
                Insert_B(p);
                printf("资源不足，进程%c插入阻塞队列，重新调度...\n",p->name);
                show_status();
                break;
            }
        }
        // 如果进程进入阻塞队列，那么重新调度就绪队列里面的资源
        if(p->status == 'B')
            continue;

        // 试探性分配资源
        for(int i=0;i<3;i++){
            p->Allocated[i] += p->WantRes[i];
            res[i] -= p->WantRes[i];
            }
        // 进行安全性检测
        printf("开始进行安全性检测...\n");
        // 如果检测成功，判断是否进程全体资源分配完成，若完成就加入完成队列
        if(SafeCheck()){
        	Wait = Wait->next;
            printf("检测成功...\n");
            // 判断是不是已经把全体资源分配完成
            if(p->Allocated[0]==p->MaxNeed[0] &&
               p->Allocated[1]==p->MaxNeed[1] &&
               p->Allocated[2]==p->MaxNeed[2]){
                    p->status = 'F';
                    // 释放内存
                    for(int i=0;i<3;i++)
                        res[i] += p->Allocated[i];
                    Insert_F(p);
                    printf("进程%c完成，进入完成队列...\n",p->name);
                    show_status();


                    // 查看阻碍队列中有没有进程可以回到就绪队列进行等待
                    PCB *q = Block;
                    while(q){
                        if((q->MaxNeed[0]-q->Allocated[0])<=res[0] &&
                           (q->MaxNeed[1]-q->Allocated[1])<=res[1] &&
                           (q->MaxNeed[2]-q->Allocated[2])<=res[2]){
                            printf("进程%c回到就绪队列...\n",q->name);
                            q->status = 'W';
                            Delete_B(q);
                            Insert_W(q);
                            show_status();
                           }
                           q=q->next;
                        }
                }else{
                    // 若没有完成则继续插入就绪队列进行重新分配资源
                    printf("进程%c还未完成，重进就绪队列进行等待...\n",p->name);
                    Insert_W(p);
                    show_status();
            }
        }else{
        	Wait = Wait->next;
            // 安全检测不成功就把进程重新插入队列中重新排队
            printf("安全检测不成功，插入就绪队列重新排队...\n");
            for(int i=0;i<3;i++){
                // 把之前提前分配给它的资源拿回来
                p->Allocated[i] -= p->WantRes[i];
                res[i] += p->WantRes[i];
            }
                Insert_W(p);
                show_status();
        }

    }
}

int main(){
    InitPro(PROCESSNUM);
    Banker();
    return 0;
}
