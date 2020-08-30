#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLEN 640
// 进程的结构体
typedef struct node3{
    int id;  // 进程的编号
    int status;  // 进程的状态，1表示申请，0表示释放
    int len;    // 进程所需要的内存大小
    struct node3* next;
}PCB;

// 内存中空闲的区块链
typedef struct node1{
    int start;   // 空闲内存的起始位置
    int len;     // 空闲内存的长度
    struct node1* next;
}free_node;

// 内存中被占用的区块链
typedef struct node2{
    int id;      // 占用的进程ID
    int start;   // 空闲内存的起始位置
    int len;     // 空闲内存的长度
    struct node2* next;
}busy_node;

PCB* lst = NULL;
free_node* freen= NULL;
busy_node* busy= NULL;

// 按先后顺序插入到进程序列中
void InsertPCB(PCB* in){
    PCB *p;
    p=lst;
    if(NULL==lst){
        in->next=lst;
        lst=in;
    }else{
        // 搜寻到完成队列的最后一个位置
        while(NULL!=p->next){
            p=p->next;
        }
        in->next=p->next;
        p->next=in;
    }
}

// 初始化生成内存，并且获取申请队列
void Init(){
    // 一开始空闲区为全部内存
    free_node* f = (free_node*)malloc(sizeof(free_node));
    f->start = 0;
    f->len = MAXLEN;
    f->next = NULL;
    freen = f;


    FILE* fp;
    if((fp=fopen("input.txt","r"))==NULL){
        printf("Read Error!!\n");
        exit(1);
    }
    for(int i=0;i<11;i++){
        PCB* p = (PCB*)malloc(sizeof(PCB));
        fscanf(fp,"%d %d %d\n",&(p->id),&(p->status),&(p->len));
        InsertPCB(p);
    }
    fclose(fp);
}

// 插入占用序列
void Insertbusy(busy_node* in){
    busy_node *p;
    p=busy;
    if(NULL==busy){
        in->next=busy;
        busy=in;
    }else{
        // 搜寻到完成队列的最后一个位置
        while(NULL!=p->next){
            p=p->next;
        }
        in->next=p->next;
        p->next=in;
    }
}


// 占用空间并且更新占用内存和空内存序列
void use_free(int len, int id){
    free_node* f=freen;
    while(f!=NULL){
        // 如果空闲区内存足够就插入
        if(f->len >= len){
            busy_node* b = (busy_node*)malloc(sizeof(busy_node));
            b->id = id;
            b->start = f->start;
            b->len = len;
            Insertbusy(b);
            f->start += len;
            f->len -= len;
            break;
        }
        f = f->next;
    }
    // 如果没有足够的内存存放，直接提醒失败
    if(f==NULL)
        printf("没有足够的内存！\n");

}

// 删除freen对应结点
void Delete_F(free_node *f){
    free_node *p,*q;
    p = freen;
    q = p;
    while(p->next!=NULL){
        q = p->next;
        if(q == f){
            p->next = q->next;
            break;
        }
        p = q;
    }
}

// 碎片合并
void free_merge(){
    free_node *f;
    f = freen;
    // 循环这个链表，找到前后可以合并的区块进行合并，然后继续从头开始遍历
    while(f->next){
        // 判断前一个结点的开始位置加上长度是否会等于下一个结点的开始位置，是的话可以合并
        if(f->start+f->len == f->next->start){
                // 更新前一个结点，然后删除后面的结点，重新赋值在链表最前面，重新遍历
                f->len += f->next->len;
                Delete_F(f->next);
                f = freen;
        }else{
            f = f->next;}
    }
}

// 按照开始地址从小到大插入
void Insertfree(int start, int len){
    free_node *p,*q;
    p=freen;
    q=p;
    if(freen->start>start){
        free_node* f = (free_node*)malloc(sizeof(free_node));
        f->start = start;
        f->len  = len;
        f->next = freen;
        freen = f;
        return;
    }else{
        while(p->next!=NULL){
                q = p->next;
                if(q->start>start){
                    free_node* f = (free_node*)malloc(sizeof(free_node));
                    f->start = start;
                    f->len = len;
                    f->next = q;
                    p->next = f;
                    break;
                }
                p=q;
            }
        }
    if(p->next==NULL){
        free_node* f = (free_node*)malloc(sizeof(free_node));
        f->start = start;
        f->len = len;
        f->next=p->next;
        p->next=f;
    }
}

// 释放对应内存
void free_busy(int id){
    busy_node *bl,*br;
    bl = busy;
    br = bl;
    if(busy->id == id){
        Insertfree(busy->start,busy->len);
        busy = busy->next;
        return;
    }
    while(bl->next!=NULL){
        br =  bl->next;
        if(br->id == id){
            Insertfree(br->start, br->len);
            // 进行碎片合并
            free_merge();
            bl->next = br->next;
            return;
        }
        bl = br;
    }
    if(bl->next==NULL)
        printf("找不到对应进程\n");
}

// 对内存分配情况进行打印
void show_free_busy(){
    // 对空闲区块链进行打印
    printf("Free:\n");
    printf("======================================\n");
    free_node* f=freen;
    while(f != NULL){
        printf("%4d    %6d\n",f->start,f->len);
        f = f->next;
    }

    // 对占用区块链进行打印
    printf("Busy:\n");
    printf("======================================\n");
    busy_node* b=busy;
    while(b != NULL){
        printf("%2d    %4d    %6d\n",b->id,b->start,b->len);
        b = b->next;
    }
}

// 运用首次适应的内存分配
void fir_alloc(){
    PCB* p = lst;
    show_free_busy();
    // 开始执行申请序列
    while(p!=NULL){
        // 判断是申请还是释放
        if(p->status==1){
            printf("现在对作业%d进行申请操作，申请内存为：%dKB\n",p->id,p->len);
            use_free(p->len, p->id);
        }else{
            printf("现在对作业%d进行释放操作，释放内存为：%dKB\n",p->id,p->len);
            free_busy(p->id);
        }
        p = p->next;

        show_free_busy();
    }
}

int main(){
    Init();
    fir_alloc();
    return 0;
}
