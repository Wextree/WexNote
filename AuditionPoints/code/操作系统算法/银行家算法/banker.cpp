#include <stdio.h>
#include <stdlib.h>

#define PROCESSNUM 5    // �����������
#define RESOURSE_A 10  // ���������Դ����
#define RESOURSE_B 15  // ���������Դ����
#define RESOURSE_C 12  // ���������Դ����


// ������̽ṹ��
typedef struct node{
    char name;     // ������
    int MaxNeed[3];     // ������Դ�����Ҫ��Դ��
    int Allocated[3];    // ������Դ�Ѿ����������
    int WantRes[3] ={0};    // ��һ���������Դ��
    char status;        // ��ʾ����״̬��W,F,B
    struct node* next;
}PCB;



// �����������У��ֱ��Ǿ�������ɺ�����
PCB *Wait = NULL;
PCB *Finish = NULL;
PCB *Block = NULL;
int res[3] = {RESOURSE_A, RESOURSE_B, RESOURSE_C};

// ���������1�����high
int random(int high){
    if(high==0)
    	return 0;
    return rand()%high+1;
}

// ���������ĩβ������
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

// ����ɶ���ĩβ������
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

// ����������ĩβ������
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

// ��ʼ����������
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

// ��ʾ�������
void show_status(){
    printf("���ڸ�����Դ���У�A��%d   B��%d   C��%d\n",res[0],res[1],res[2]);
    printf("===================================================\n");
    PCB *w = Wait;
    printf("�������У�Wait����\n");
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
    printf("��ɶ��У�Finish����\n");
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
    printf("�������У�Block����\n");
    printf("\tname\tMax_A\tMax_B\tMax_C\tAlloc_A\tAlloc_B\tAlloc_C\tstatus\n");
    while(b!=NULL){
        printf("\t%c   \t%5d\t%5d\t%5d\t%7d\t%7d\t%7d\t %c\n",
               b->name,b->MaxNeed[0],b->MaxNeed[1],b->MaxNeed[2],
               b->Allocated[0],b->Allocated[1],b->Allocated[2],b->status);
        b = b->next;
    }
}

// ��ȡ�������еĳ���
int getNum(){
    PCB *w = Wait;
    int count_w = 0;
    while(w){
        count_w++;
        w = w->next;
    }
    return count_w;
}

// ���þ������б�־λ
void setLabel(){
    PCB *w = Wait;
    while(w){
        w->status = 'W';
        w = w->next;
    }
}

// ��ȫ�Լ��
int SafeCheck(){
    // ����һ����������ȥ�������е���Դ��
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
            // ���λΪF��ʾ�Ѿ����ͷŹ��ˣ�������������
            if(w->status == 'F'){
                w = w->next;
                continue;
            }
            // �����ִ���Դ�ܷ��������������ĳ�����̵�ʣ��������Դ��
            if(res_t[0]>=(w->MaxNeed[0]-w->Allocated[0]) &&
               res_t[1]>=(w->MaxNeed[1]-w->Allocated[1]) &&
               res_t[2]>=(w->MaxNeed[2]-w->Allocated[2])){
                    // ���Ա�֤��ɼ��ͷ��ڴ�
                    for(int  ii=0;ii<3;ii++){
                        res_t[ii] += w->Allocated[ii];
                    }
                    w->status = 'F';
                    break;
            }
            w = w->next;
        }
        // �����һ��ѭ���ߵ����û���ҵ������ͷ��ڴ�Ľ��̣���ʾ��ȫ���ʧ��
        // �ָ���־λȻ�󷵻�ʧ��
        if(NULL == w){
            // ��Ϊ�����˽��̵�״̬��Ϊ��־λ������Ҫ�������û���
            setLabel();
            return 0;
        }
        i++;
    }
    setLabel();
    return 1;

}

// ɾ���������н��
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

// ���м��㷨
void Banker(){
    PCB *p;
    show_status();
    // ���������к����������л���û�н���
    while(Wait!=NULL||Block!=NULL){
        // ȡ���������е�ͷ�������ж�
        p = Wait;
//        Wait = Wait->next;

        // ���������Դ��
        for(int i=0;i<3;i++){
            p->WantRes[i] = random(p->MaxNeed[i]-p->Allocated[i]);
        }
        printf("ȡ������%c�������������Դ��A��%d   B��%d   C��%d\n",p->name,p->WantRes[0],p->WantRes[1],p->WantRes[2]);

        // �ж�����ϵͳ��Դ���㲻�㹻���㹻���밲ȫ�Լ�⣬�����ȥ��������
        for(int i=0;i<3;i++){
            if(p->WantRes[i] > res[i]){
            	Wait = Wait->next;
                p->status = 'B';
                Insert_B(p);
                printf("��Դ���㣬����%c�����������У����µ���...\n",p->name);
                show_status();
                break;
            }
        }
        // ������̽����������У���ô���µ��Ⱦ��������������Դ
        if(p->status == 'B')
            continue;

        // ��̽�Է�����Դ
        for(int i=0;i<3;i++){
            p->Allocated[i] += p->WantRes[i];
            res[i] -= p->WantRes[i];
            }
        // ���а�ȫ�Լ��
        printf("��ʼ���а�ȫ�Լ��...\n");
        // ������ɹ����ж��Ƿ����ȫ����Դ������ɣ�����ɾͼ�����ɶ���
        if(SafeCheck()){
        	Wait = Wait->next;
            printf("���ɹ�...\n");
            // �ж��ǲ����Ѿ���ȫ����Դ�������
            if(p->Allocated[0]==p->MaxNeed[0] &&
               p->Allocated[1]==p->MaxNeed[1] &&
               p->Allocated[2]==p->MaxNeed[2]){
                    p->status = 'F';
                    // �ͷ��ڴ�
                    for(int i=0;i<3;i++)
                        res[i] += p->Allocated[i];
                    Insert_F(p);
                    printf("����%c��ɣ�������ɶ���...\n",p->name);
                    show_status();


                    // �鿴�谭��������û�н��̿��Իص��������н��еȴ�
                    PCB *q = Block;
                    while(q){
                        if((q->MaxNeed[0]-q->Allocated[0])<=res[0] &&
                           (q->MaxNeed[1]-q->Allocated[1])<=res[1] &&
                           (q->MaxNeed[2]-q->Allocated[2])<=res[2]){
                            printf("����%c�ص���������...\n",q->name);
                            q->status = 'W';
                            Delete_B(q);
                            Insert_W(q);
                            show_status();
                           }
                           q=q->next;
                        }
                }else{
                    // ��û��������������������н������·�����Դ
                    printf("����%c��δ��ɣ��ؽ��������н��еȴ�...\n",p->name);
                    Insert_W(p);
                    show_status();
            }
        }else{
        	Wait = Wait->next;
            // ��ȫ��ⲻ�ɹ��Ͱѽ������²�������������Ŷ�
            printf("��ȫ��ⲻ�ɹ�������������������Ŷ�...\n");
            for(int i=0;i<3;i++){
                // ��֮ǰ��ǰ�����������Դ�û���
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
