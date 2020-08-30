#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLEN 640
// ���̵Ľṹ��
typedef struct node3{
    int id;  // ���̵ı��
    int status;  // ���̵�״̬��1��ʾ���룬0��ʾ�ͷ�
    int len;    // ��������Ҫ���ڴ��С
    struct node3* next;
}PCB;

// �ڴ��п��е�������
typedef struct node1{
    int start;   // �����ڴ����ʼλ��
    int len;     // �����ڴ�ĳ���
    struct node1* next;
}free_node;

// �ڴ��б�ռ�õ�������
typedef struct node2{
    int id;      // ռ�õĽ���ID
    int start;   // �����ڴ����ʼλ��
    int len;     // �����ڴ�ĳ���
    struct node2* next;
}busy_node;

PCB* lst = NULL;
free_node* freen= NULL;
busy_node* busy= NULL;

// ���Ⱥ�˳����뵽����������
void InsertPCB(PCB* in){
    PCB *p;
    p=lst;
    if(NULL==lst){
        in->next=lst;
        lst=in;
    }else{
        // ��Ѱ����ɶ��е����һ��λ��
        while(NULL!=p->next){
            p=p->next;
        }
        in->next=p->next;
        p->next=in;
    }
}

// ��ʼ�������ڴ棬���һ�ȡ�������
void Init(){
    // һ��ʼ������Ϊȫ���ڴ�
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

// ����ռ������
void Insertbusy(busy_node* in){
    busy_node *p;
    p=busy;
    if(NULL==busy){
        in->next=busy;
        busy=in;
    }else{
        // ��Ѱ����ɶ��е����һ��λ��
        while(NULL!=p->next){
            p=p->next;
        }
        in->next=p->next;
        p->next=in;
    }
}


// ռ�ÿռ䲢�Ҹ���ռ���ڴ�Ϳ��ڴ�����
void use_free(int len, int id){
    free_node* f=freen;
    while(f!=NULL){
        // ����������ڴ��㹻�Ͳ���
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
    // ���û���㹻���ڴ��ţ�ֱ������ʧ��
    if(f==NULL)
        printf("û���㹻���ڴ棡\n");

}

// ɾ��freen��Ӧ���
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

// ��Ƭ�ϲ�
void free_merge(){
    free_node *f;
    f = freen;
    // ѭ����������ҵ�ǰ����Ժϲ���������кϲ���Ȼ�������ͷ��ʼ����
    while(f->next){
        // �ж�ǰһ�����Ŀ�ʼλ�ü��ϳ����Ƿ�������һ�����Ŀ�ʼλ�ã��ǵĻ����Ժϲ�
        if(f->start+f->len == f->next->start){
                // ����ǰһ����㣬Ȼ��ɾ������Ľ�㣬���¸�ֵ��������ǰ�棬���±���
                f->len += f->next->len;
                Delete_F(f->next);
                f = freen;
        }else{
            f = f->next;}
    }
}

// ���տ�ʼ��ַ��С�������
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

// �ͷŶ�Ӧ�ڴ�
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
            // ������Ƭ�ϲ�
            free_merge();
            bl->next = br->next;
            return;
        }
        bl = br;
    }
    if(bl->next==NULL)
        printf("�Ҳ�����Ӧ����\n");
}

// ���ڴ����������д�ӡ
void show_free_busy(){
    // �Կ������������д�ӡ
    printf("Free:\n");
    printf("======================================\n");
    free_node* f=freen;
    while(f != NULL){
        printf("%4d    %6d\n",f->start,f->len);
        f = f->next;
    }

    // ��ռ�����������д�ӡ
    printf("Busy:\n");
    printf("======================================\n");
    busy_node* b=busy;
    while(b != NULL){
        printf("%2d    %4d    %6d\n",b->id,b->start,b->len);
        b = b->next;
    }
}

// �����״���Ӧ���ڴ����
void fir_alloc(){
    PCB* p = lst;
    show_free_busy();
    // ��ʼִ����������
    while(p!=NULL){
        // �ж������뻹���ͷ�
        if(p->status==1){
            printf("���ڶ���ҵ%d������������������ڴ�Ϊ��%dKB\n",p->id,p->len);
            use_free(p->len, p->id);
        }else{
            printf("���ڶ���ҵ%d�����ͷŲ������ͷ��ڴ�Ϊ��%dKB\n",p->id,p->len);
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
