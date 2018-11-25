#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VERTEX_NUM 200
#define HEADER1 "--------------------------------���ݵ�����·----------------------------------\n"
#define HEADER2 "|NO| ��·���� |  ���վ  |  �յ�վ  |վ����|����ʱ��|�հ�ʱ��| ����ʱ�� |Ʊ��|\n"
#define HEADER3 "|--|----------|----------|----------|------|--------|--------|----------|----|\n"
#define HEADER4 "------------------------------------------------------------------------------\n"
#define HEADER5 "--------------------------------------------\n"
#define HEADER6 "|  վ������  | �Ƿ�ɻ��� |  ������·���  |\n"


#define FORMAT "|%-2d|%-10s|%-10s|%-10s|%-6d|%-8s|%-8s|%-10s|%-4d|\n"
#define DATA p->data.NO,p->data.name,p->data.start,p->data.destination,p->data.number,p->data.starttime,p->data.endtime,p->data.totaltime,p->data.price

int MAX_LINE_NUM=8;
int istrans[MAX_VERTEX_NUM]={0};

typedef struct line
{
    int NO;							//��·���//
    char name[15];					//��·����//
    char start[15];					//���վ//
    char destination[15];			//�յ�վ//
    int number;						//վ����//
    char starttime[10];				//����ʱ��//
    char endtime[10];				//�հ�ʱ��//
    char totaltime[10];				//����ʱ��//
    int price;						//Ʊ��//
}LINE;

typedef struct node					//line�ṹ��ӿ�//
{
    LINE data;
    struct node *next;
}NODE,*NODELINE;


typedef struct enode_type
{
	int adjvex;                     //�ڽӵ��λ��//
	int weight;                     //Ȩֵ������·���)//
	struct enode_type *nextarc;     //ָ����һ���ڽӵ�//
}enode;

typedef struct vnode_type
{
	int id;                         //����ı��//
	char name[20];                  //��������֣���վ������//
	enode *fadj;					//ָ���һ����//
}vnode;

typedef struct Graph_type
{
	vnode vertices[MAX_VERTEX_NUM]; //���㼯��(vertex)//
	int vexnum;                     //ͼ�Ķ�����Ŀ//
	int arcnum;                     //ͼ�ı���Ŀ//

}Graph;

typedef struct pathnode_type
{
	int top;                        //·������Ŀ//
	int path[MAX_VERTEX_NUM];       //·����Ϣ//
	int trans[MAX_VERTEX_NUM];		//���˵���Ϣ,0Ϊ�ǻ��˵㣬1Ϊ���˵�//
	int counter;                    //���˵Ĵ���//
	struct pathnode_type *next;     //ָ����һ���ɵִ��յ��·����Ϣ�ڵ�//
}pathnode;


typedef struct LINK_type
{
	pathnode *head;                 //·���ӿ�//
}LINK;



int initialize();
//�˵�//
void menu();
//����һ����ʾ��·��Ϣ//
void Initline();
//����һ����ȡ��·��Ϣ//
void readfromfile_line(NODELINE L);
//�����ͷ//
void printheader();
//��ӡ����//
void printdata(NODE *q);
//����һ����ʾ��·��Ϣ�ӿ�//
void showline(NODELINE L);

//��ȡվ��ı�ţ���1-130//
int locatevex(Graph *G,char *a);
//��ȡ��ǰ��·��//			
int getline(Graph *G,int i,int j);
//��ȡ���˵�//
void gettrans(Graph *G,pathnode *all);
//��������������//
void create_graph(Graph *G);
//DFS����������������п��ܵ�·���߷�//
void DFS(Graph *G,int stack[],int visited[],int v,int destination,int top,LINK *L);
//������ٻ���·����Ϣ//
void print_lesstransfer(Graph *G,LINK *L);
//���ٻ��˵ĺ����ӿڣ�����1����ʼվ������2���յ�վ//
void lesstransfer(char start[],char destination[]);
//����һ��·������//
void create_path(LINK *L);
//����һ��·����Ϣ//
void addpath(LINK *L,pathnode *p);
//�Ʒѣ�����ÿ��վ��֮��1������׼��㣬��ʵ���кܴ���//
int caculate_price(int station);
//�˳�ʱ��
int caculate_time(int station,int count);


//վ���ѯ��·,��ʵ�ֻ���վ���Զ��ж�
int station_search_line(char station[],int parameter);
void deletedata();
void inputdata();
void save_line_information(NODELINE L);
void xiugaidata();
int main()
{
    int num;
	int label;
	char start[20],destination[20];
	if(initialize()==0)	exit(0);	
    while(1)
    {
        menu();
        printf("������<0-7>:");
        scanf("%d",&label);
        if(label==0) {printf("��л����ʹ�ã�\n"); exit(0);}
        switch(label)
        {
        case 1:
            Initline();
            break;
        case 2:
            inputdata();
            break;
        case 3:

            xiugaidata();
            break;
        case 4:
            printf("4\n");
            deletedata();
            break;
        case 5:
			printf("��������ʼվ��");
			scanf("%s",start);
			printf("�������յ�վ��");
			scanf("%s",destination);

			if(strcmp(start,destination)==0)
				printf("�������վ������ͬ��");
			else
				lesstransfer(start,destination);//��С���˽ӿ�//
            break;
        case 6:
			//��ʵ�ֻ���վ����Զ��ж�
            printf("������վ������");
			scanf("%s",start);
			if(station_search_line(start,1)==-1)
				printf("δ��ѯ��%sվ����Ϣ��\n",start);
            break;
        default:
            fflush(stdin);
            printf("�������ָ������\n");
        }
        printf("\n�����������...\n");
        getch();
    }


    return 0;
}

void menu()
{
    printf("\t|-----------------------------------------------|\n");
    printf("\t|              ���ݵ�����ѯϵͳ                 |\n");
    printf("\t|-----------------------------------------------|\n");
    printf("\t|       0:�˳�ϵͳ                              |\n");
    printf("\t|       1:��ʾ������·                          |\n");
    printf("\t|       2:¼�������·                          |\n");
    printf("\t|       3:�޸ĵ�����·                          |\n");
    printf("\t|       4:ɾ��������·                          |\n");
    printf("\t|       5:���˷��ò�ѯ                          |\n");
    printf("\t|       6:վ��������·                          |\n");
    printf("\t|-----------------------------------------------|\n");
}

int initialize()
{
	int flag=1;
	FILE *fp;
	fp=fopen("stationinfo.txt","r");
	if(fp==NULL) {printf("ȱʧstationinfo.txt\n");flag=0;}
	fclose(fp);
	fp=fopen("station.txt","r");
	if(fp==NULL) {printf("ȱʧstation.txt\n");flag=0;}
	fclose(fp);
	fp=fopen("stationdata.txt","r");
	if(fp==NULL) {printf("ȱʧstationdata.txt\n");flag=0;}
	fclose(fp);
	fp=fopen("line.txt","r");
	if(fp==NULL) {printf("ȱʧline.txt\n");flag=0;}
	fclose(fp);
	fp=fopen("line.txt","r");
	fscanf(fp,"%d%d",&MAX_LINE_NUM);
	fclose(fp);
	if(flag==0)	exit(0);
	else	return flag;
}


void Initline()
{
	
    int i,j,n,temp,k;
	int a[MAX_VERTEX_NUM]={0};
	int b[MAX_VERTEX_NUM]={0};
	int c[MAX_VERTEX_NUM]={0};
    char ch;
    NODELINE L,p;
	Graph G;
	FILE *fp;
    L=(NODE *)malloc(sizeof(NODE));
    L->next=NULL;
    readfromfile_line(L);
    showline(L);
	printf("�Ƿ���ʾ���е���վ�㣿<Y or N>");
	getchar();
	scanf("%c",&ch);
	if(ch=='Y' || ch=='y')
	{
		create_graph(&G);

		fp=fopen("stationdata.txt","r");
		for(i=1;i<=MAX_VERTEX_NUM;i++)
		{
			fscanf(fp,"%d %d %d",&a[i],&b[i],&c[i]);
		}
		temp=1;
		L=L->next;
		for(i=1;i<=MAX_LINE_NUM;i++)
		{
		
			printf("-----------------------------------����%d����-----------------------------------\n",L->data.NO);
			for(j=temp;c[j]==i;j++)
			{
				printf("%s->",G.vertices[a[j]].name);
			}
			printf("%s\n",G.vertices[b[--j]].name);
			temp=++j;
			L=L->next;
			
			
		}
		fclose(fp);
	}
	else
		return;

}

void readfromfile_line(NODELINE L)
{
    int NO;                 /*��·���*/
    char name[15];          /*��·����*/
    char start[15];         /*���վ*/
    char destination[15];   /*�յ�վ*/
    int number;             /*վ����*/
    char starttime[10];     /*����ʱ��*/
    char endtime[10];       /*�հ�ʱ��*/
    char totaltime[10];     /*����ʱ��*/
    int price;              /*Ʊ��*/
    int i,j;
    int counter;

    FILE *fp;
    NODELINE r,p;
    //NODESTATION S;
    r=L;
    fp=fopen("line.txt","r");
    fscanf(fp,"%d\n",&counter);
    for(i=1;i<=counter;i++)
    {
        p=(NODE *)malloc(sizeof(NODE));
        fscanf(fp,"%d\n",&NO);
        fscanf(fp,"%s\n",name);
        fscanf(fp,"%s\n",start);
        fscanf(fp,"%s\n",destination);
        fscanf(fp,"%d\n",&number);
        fscanf(fp,"%s\n",starttime);
        fscanf(fp,"%s\n",endtime);
        fscanf(fp,"%s\n",totaltime);
        fscanf(fp,"%d\n",&price);

        p->data.NO=NO;
        strcpy(p->data.name,name);
        strcpy(p->data.start,start);
        strcpy(p->data.destination,destination);
        p->data.number=number;
        strcpy(p->data.starttime,starttime);
        strcpy(p->data.endtime,endtime);
        strcpy(p->data.totaltime,totaltime);
        p->data.price=price;
        p->next=NULL;
        r->next=p;
        r=p;
    }
    fclose(fp);
}
void printheader()
{
    printf(HEADER1);
    printf(HEADER2);
    printf(HEADER3);
}
void printdata(NODE *q)
{
    NODE *p;
    p=q;
    printf(FORMAT,DATA);
    //printf("%s",p->data.destination);
}
void showline(NODELINE L)
{
    NODELINE r;
    r=L;
    printheader();
    if(r->next==NULL) printf("�޵�����·��\n");
    while(r->next!=NULL)
    {
        r=r->next;
        printdata(r);
    }
    printf(HEADER4);
}







//���ٻ��˵ĺ����ӿڣ�����1����ʼվ������2���յ�վ//
void lesstransfer(char start[],char destination[])
{
	Graph G;
	LINK L;

	int v1,v2;
	int stack[MAX_VERTEX_NUM]={0};     //��ʼ��
	int  visited[MAX_VERTEX_NUM]={0};
	create_graph(&G);	 //����һ����������//
	create_path(&L);	//��������·��������//
	v1=locatevex(&G,start);
	v2=locatevex(&G,destination);
 
	if(v1==-1 || v2==-1)
	{
		printf("�������վ������\n");
		return;
	}

	DFS(&G,stack,visited,v1,v2,0,&L);
	print_lesstransfer(&G,&L);

}

//��ȡվ��ı�ţ���1-130//
int locatevex(Graph *G,char a[])
{
	int i=1;
	while(strcmp(G->vertices[i].name,a)&&i<=G->vexnum)
		i++;
	if(i<=G->vexnum)
		return i;
	else
        return -1;
}
//����һ����������
void create_graph(Graph *G)
{
	enode * p;
	int i,j,k,weight;
	FILE *fp;
	fp=fopen("stationinfo.txt","r");
	fscanf(fp,"%d%d",&G->vexnum,&G->arcnum);
	fclose(fp);
	fp=fopen("station.txt","r");
	for(i=1;i<=G->vexnum;i++)
	{
		fscanf(fp,"%s",G->vertices[i].name);
		G->vertices[i].id=i;
		G->vertices[i].fadj=NULL;
		//����ͷ�巨������
	}
	fclose(fp);
	fp=fopen("stationdata.txt","r");
	for (k=1;k<=G->arcnum;k++)
	{
		fscanf(fp,"%d %d %d",&i,&j,&weight);
		p=(enode *)malloc(sizeof(enode));
		p->adjvex=j;
		p->weight=weight;
		p->nextarc=G->vertices[i].fadj;
		G->vertices[i].fadj=p;
		//�������磬˫���ڽ�
		p=(enode *)malloc(sizeof(enode));
		p->adjvex=i;
		p->weight=weight;
		p->nextarc=G->vertices[j].fadj;
		G->vertices[j].fadj=p;
	}
	fclose(fp);
}
/*������������ĵݹ��ҳ����е�·��
����һ��ͼ  ����2����ʱ���·��˳������  ����3���Ƿ����  ����4����ǰλ��  ����5�������λ��  ����6��ջ��  ����7������·��������*/
void DFS(Graph *G,int stack[],int visited[],int v,int destination,int top,LINK *L)
{
	int j,i;
	enode *p;
	pathnode *all;
	stack[top]=v;
	if(v==destination)
	{
		all=(pathnode *)malloc(sizeof(pathnode));
		all->counter=0;
		all->top=top;
		for(i=0;i<=top;i++)
		{
			all->trans[i]=0;
			all->path[i]=stack[i];
		}
		gettrans(G,all);
		addpath(L,all);
		return ;//�ݹ�߽�����
	}
	else
		visited[v]=1;
	p=G->vertices[v].fadj;
	while(p)
	{
		if(!visited[p->adjvex])
			DFS(G,stack,visited,p->adjvex,destination,top+1,L);
		p=p->nextarc;
	}
	stack[top]=0;
	visited[v]=0;
}

//���·��//
void print_lesstransfer(Graph *G,LINK *L)
{
	int maximum=999;
	int line,k;
	int count=1;
	int i=0,j;
	pathnode *p,*r;
	p=L->head;
	while(p->next!=NULL)
	{
		p=p->next;
		if(p->counter<=maximum)
			maximum=p->counter;
	}
	r=L->head;
	while(r->next!=NULL)
	{
		r=r->next;
		if(r->counter==maximum)
		{
			i=0;
			printf("\n���ٻ�����·��\n");
			printf("------------------------->-------------------------->------------------------->\n");
			for(k=0;k<(r->top);k++)
			{
				if(r->trans[k])
				{
					printf("%s(�ڴ˻���%d����)---->",G->vertices[r->path[k]].name,G->vertices[r->path[k]].fadj->weight);
					i++;
				}
				else
				{
					line=G->vertices[r->path[k]].fadj->weight;
					printf("%s(%d|%d����)-->",G->vertices[r->path[k]].name,G->vertices[r->path[k]].id,line);
					i++;
				}
			}
			printf("%s(�յ�)\n",G->vertices[r->path[(r->top)]].name);
			printf("-------------------------------------------------------------------------------\n");
			if(maximum==0)
				printf("����Ҫ����\n");
			else
				printf("��Ҫ����:%d��\n",maximum);
			printf("����վ��:%d��\n",i);
			printf("�˳�ʱ��:%d����\n",caculate_time(i,maximum));
			printf("Ʊ��Ϊ:%dԪ\n",caculate_price(i));
			break;//Ϊ�˱����ظ���ֱ������
		}
	}
}

void create_path(LINK *L)
{
	//�½��������//
	L->head=(pathnode *)malloc(sizeof(pathnode));
	L->head->next=NULL;
}

void addpath(LINK *L,pathnode *p)
{
	//����Ĳ������//
	p->next=L->head->next;
	L->head->next=p;
}
//���ͬһ�����ϵ���������·��//
int getline(Graph *G,int i,int j)
{
	enode *p=G->vertices[i].fadj;
	while(p)
	{
		if(p->adjvex==j)
			return p->weight;
		p=p->nextarc;
	}
	return 0;
}

//���ÿ��·���ϵ�ת�˴���//
void gettrans(Graph *G,pathnode *all)
{
	int i;
	for(i=0;i<all->top-1;i++)
		if(getline(G,all->path[i],all->path[i+1])!=getline(G,all->path[i+1],all->path[i+2]))
		{
			all->trans[i+1]=1;
			all->counter++;
		}
}
//���׼۸����//
int caculate_price(int station)
{
	if(station<=4)
		return 2;
	else if (station>4 && station<=12)
		return (2+(station%4-1));
	else if (station>12 && station<=24)
		return (4+(station%4-1));
	else
		return 7;
}
//�˳�ʱ�����//
int caculate_time(int station,int count)
{
	return (3*station+count*6);
}
//վ����������·��Ϣ  �Զ��жϻ��˵� //
int station_search_line(char station[],int parameter)
{
	Graph G;
	enode *p;
	FILE *fp;
	int line[2]={0};
	int num,i,count,clear,l=0;
	int a[MAX_VERTEX_NUM]={0};
	int b[MAX_VERTEX_NUM]={0};
	int c[MAX_VERTEX_NUM]={0};

	create_graph(&G);
	fp=fopen("stationinfo.txt","r");
	fscanf(fp,"%d%d",&clear,&count);
	fclose(fp);

	num=locatevex(&G,station);
	fp=fopen("stationdata.txt","r");

	for(i=1;i<=count;i++)
		fscanf(fp,"%d %d %d",&a[i],&b[i],&c[i]);
	fclose(fp);
	for(i=1;i<=count;i++)
	{
		if(num==b[i])
		{
			line[l++]=c[i];
		}
	}

//	p=G.vertices[num].fadj;
	if(line[1]==0 && line[0]==0)
		 return -1;
	if(parameter==1)
	{
		if(line[1]==0)
		{
			printf(HEADER5);
			printf(HEADER6);
			printf(HEADER5);
			printf("|%12s|%12s|%12d����|\n",station,"��",line[0]);
			printf(HEADER5);
			
		}
			//printf("%sվλ�ڵ���%d����\n",station,line[0]);
		else
		{
			printf(HEADER5);
			printf(HEADER6);
			printf(HEADER5);
			printf("|%-12s|%-12s|  %d���ߺ�%d����  |\n",station,"��",line[0],line[1]);
			printf(HEADER5);
		}
			//printf("%sվ�ǻ���վ��λ�ڵ���%d���ߺ͵���%d��\n",station,line[0],line[1]);
	}
	else if(parameter==0)
	{
		if(line[1]==0)
			return 0;
		else
			return 1;
	}
	else
		printf("�������ݴ���\n");

}
//�������  
void inputdata()
{
	Graph G,G1;
	FILE *fp;
	NODELINE L,p,r,s;
	int vexnum,arcnum;
	char station[20];
	int istrans;
	int i,j,k,n;
	char ch;
	int stationnum;
	int newroute[100];
	char newroutename[100][20];
	
	int NO;							//��·���//
    char name[15];					//��·����//
    char start[15];					//���վ//
    char destination[15];			//�յ�վ//
    int number;						//վ����//
    char starttime[10];				//����ʱ��//
    char endtime[10];				//�հ�ʱ��//
    char totaltime[10];				//����ʱ��//
    int price;						//Ʊ��//

	char str[20];
	int ret;
	fp=fopen("stationinfo.txt","r");
	fscanf(fp,"%d%d",&vexnum,&arcnum);
	fclose(fp);
	fp=fopen("line.txt","r");
	fscanf(fp,"%d",&MAX_LINE_NUM);
	fclose(fp);
	vexnum++;//�µ�վ������
	L=(NODE *)malloc(sizeof(NODE));
	L->next=NULL;
	readfromfile_line(L);
	create_graph(&G);
	G1=G;
	/* *********************************����Ϊ¼����·��Ϣ***************************** */
	r=L;
	while(r->next!=NULL)
		r=r->next;
label: printf("�������µ���·���(0-����)��",MAX_LINE_NUM+1);//���ǲ�Ϊ����
	   ret=scanf("%d",&NO);
	if(NO==0)	return;
   while(ret!=1)
		{
			lab:fflush(stdin);
			printf("����ȷ������·��ţ�");
			ret=scanf("%d",&NO); 
		
		}
        	if(NO!=MAX_LINE_NUM+1)
			{
				printf("�������·���ӦΪ������·��ŵ���һλ��(%d��)\n",MAX_LINE_NUM+1); goto lab;
			}
		s=L;
		while(s)
		{
			if(s->data.NO==NO)
			{
				printf("�õ�����·����Ѵ��ڣ�\n");
				getchar();
				goto label;
			}
			s=s->next;
		}
		p=(NODE *)malloc(sizeof(NODE));
		p->data.NO=NO;
		printf("�����������·���ƣ�");
		scanf("%s",p->data.name);
		printf("��������ʼվ��");
		scanf("%s",str);
		for(i=0;str[i]!='\0';i++)
			if(isdigit(str[i]))
			{
				fflush(stdin);
				printf("վ�������ܴ������֣�\n������������ʼվ��");
				scanf("%s",str);
				break;
			}
		strcpy(p->data.start,str);
		//scanf("%s",p->data.start);
		memset(str,0,sizeof(str));
		printf("�������յ�վ��");
		scanf("%s",str);
		for(i=0;str[i]!='\0';i++)
		{
			if(isdigit(str[i]))//�ж��Ƿ�����
			{
				fflush(stdin);//����ļ�����
				printf("վ�������ܴ������֣�\n�����������յ�վ��");
				scanf("%s",str);
				break;
			}
		}
		strcpy(p->data.destination,str);
		//scanf("%s",p->data.destination);
		memset(str,0,sizeof(str));
		printf("������վ������");
		ret=scanf("%d",&p->data.number);
		while(ret!=1)
		{
			label1:fflush(stdin);
			printf("����ȷ����վ������");
			ret=scanf("%d",&p->data.number);
		}
         if(p->data.number<0) { printf("������һ������0�����֣�\n");  goto label1; }
		printf("����������ʱ�䣺");
		scanf("%s",p->data.starttime);
		printf("�������հ�ʱ�䣺");
		scanf("%s",p->data.endtime);
		printf("����������ʱ����");
		scanf("%s",p->data.totaltime);
		printf("������Ʊ�ۣ�");
		ret=scanf("%d",&p->data.price);
		while(ret!=1)
		{
			label2:fflush(stdin);
			printf("����ȷ����Ʊ�ۣ�");
			ret=scanf("%d",&p->data.price);
		}
         if((p->data.price<0)||(p->data.price>30)) { printf("������һ������0��С��30�����֣�\n");  goto label2; }
		p->next=NULL;
		r->next=p;
		r=p;
		MAX_LINE_NUM++;
		//arcnum+=number;
		
		/* *********************************����Ϊ¼����·��Ϣ***************************** */
		
		/* *********************************����Ϊ¼��վ����Ϣ***************************** */
		printf("���������%d���ߵ�վ����Ϣ��\n",r->data.NO);
		stationnum=r->data.number;
		for(i=1;i<=stationnum;i++)
		{
			printf("�������%d��վ������ƣ�",i);
			scanf("%s",station);
		

			for(j=1;j<=MAX_VERTEX_NUM;j++)
			{
				if(strcmp(G.vertices[j].name,station)==0)
				{
					printf("��վ���Ƿ�Ϊ���˵�<0-1>?");//վ�����ظ���Ҫô�ǻ��˵㣬Ҫô��վ�����ظ����
					scanf("%d",&istrans);
					if(istrans)
					{
						newroute[i]=locatevex(&G,station);//Ѱ�ҵ�һ�γ��ֵĵط�
						strcpy(newroutename[i],"0");//����а�����Ϊ0
						vexnum--;
						break;
					}
					else
					{
						while(strcmp(G.vertices[j].name,station)==0)
						{
							printf("վ�����ظ��������������%d��վ������:",i);
							scanf("%s",station);
						}
						newroute[i]=vexnum;
						strcpy(newroutename[i],station);
					}
					
				}
				else
				{
					newroute[i]=vexnum;
					strcpy(newroutename[i],station);
				}

			}
			vexnum++;
		}
		
		/*for(i=1;i<=stationnum;i++)
		{
			if(i+1<=stationnum)
				printf("%d %d %d\n",newroute[i],newroute[i+1],r->data.NO);
			printf("%s\n",newroutename[i]);
		}*/


		fp=fopen("stationdata.txt","a+");//�������վ����Ϣ
		for(i=1;i<=stationnum;i++)
		{
			if(i+1<=stationnum)//��ӱ�Ҫ+1
			{fprintf(fp,"%d %d %d\n",newroute[i],newroute[i+1],r->data.NO);arcnum++;}

		}
		fclose(fp);

		fp=fopen("station.txt","a+");//���˵㲻�ظ������
		for(i=1;i<=stationnum;i++)
		{
			if(strcmp(newroutename[i],"0")!=0)
				fprintf(fp,"%s\n",newroutename[i]);
		}
		fclose(fp);

		fp=fopen("stationinfo.txt","w");
		fprintf(fp,"%d\n%d",vexnum-1,arcnum);
		fclose(fp);

		fp=fopen("line.txt","a+");
		fprintf(fp,"%d\n",r->data.NO);
		fprintf(fp,"%s\n",r->data.name);
		fprintf(fp,"%s\n",r->data.start);
		fprintf(fp,"%s\n",r->data.destination); 
		fprintf(fp,"%d\n",r->data.number);
		fprintf(fp,"%s\n",r->data.starttime);
		fprintf(fp,"%s\n",r->data.endtime);
		fprintf(fp,"%s\n",r->data.totaltime);
		fprintf(fp,"%d\n",r->data.price);
		fclose(fp);

		/* *********************************����Ϊ¼��վ����Ϣ***************************** */

		save_line_information(L);
		printf("�ɹ�����һ��������·��Ϣ������·Ϊ%d����\n",MAX_LINE_NUM);

	
}
void deletedata()
{
	Graph G,G1;
	FILE *fp;
	NODELINE L,p,r,s;
	int vexnum,arcnum;
    char c[10];
	char station[20];
	int istrans[MAX_VERTEX_NUM];
	int i,j,k,n;
	char ch;
	int stationnum;
	int logic=0; //�ж�����վ��
	int NO;							//��·���//
    char name[15];					//��·����//
    char start[15];					//���վ//
    char destination[15];			//�յ�վ//
    int number;						//վ����//
    char starttime[10];				//����ʱ��//
    char endtime[10];				//�հ�ʱ��//
    char totaltime[10];				//����ʱ��//
    int price;						//Ʊ��//
	int a1[MAX_VERTEX_NUM],b1[MAX_VERTEX_NUM],c1[MAX_VERTEX_NUM];
	char name1[MAX_VERTEX_NUM][20];//��¼ɾ��վ�������
	int del=0,del1=0,del2=0;//��¼ɾ��վ�����Ŀ
	int templabel=0;
	char tempname[MAX_VERTEX_NUM][20];
	

	fp=fopen("stationinfo.txt","r");
	fscanf(fp,"%d%d",&vexnum,&arcnum);
	fclose(fp);
	fp=fopen("line.txt","r");
	fscanf(fp,"%d",&MAX_LINE_NUM);
	fclose(fp);
	L=(NODE *)malloc(sizeof(NODE));
	L->next=NULL;
	readfromfile_line(L);
	create_graph(&G);
	G1=G;
	/* *********************************����Ϊ¼����·��Ϣ***************************** */
	r=L;
     printf("�������Ƿ�Ҫɾ�����һ����·��(A-��,else-��):");
	 getchar();
     scanf("%s",&c);
     if((strcmp("a",c)==0)||(strcmp("A",c)==0))
     {
		 NO=MAX_LINE_NUM;
    /*printf("������Ҫɾ���ĵ���·���(0-����)��");
        if( scanf("%d",&NO) !=1)
		{
			fflush(stdin);
			printf("����ȷ��·��ţ�");
			scanf("%d",&NO); 
		}*/
		s=L;
		while(s->next!=NULL)
		{
			s=s->next;
			if(s->data.NO==NO)
			{logic=1; break;}
		}//�ҵ�S�ڵ�
		
		if(logic==0) {printf("����·������!\n");return ;}
		//del1=s->data.number;
		stationnum=s->data.number;
		while(r->next!=s)
          r=r->next;//�ҵ�S�ڵ�ǰ��
         r->next=s->next;
         free(s);
		MAX_LINE_NUM--;


        save_line_information(L);
		fp=fopen("stationdata.txt","r");
		//ɾ������·������վ���ڽ���Ϣ
		for(i=1;i<=arcnum;i++)
			fscanf(fp,"%d %d %d",&a1[i],&b1[i],&c1[i]);//a[]��b[]վ�㣬c[]��·���
		fclose(fp);
		fp=fopen("stationdata.txt","w");
		for(i=1;c1[i]!=NO;i++)
		{fprintf(fp,"%d %d %d\n",a1[i],b1[i],c1[i]);del++;}
		fclose(fp);
	
		//�����ڽӱ����ݣ�
		fp=fopen("stationdata.txt","r");
		for(i=1;i<=arcnum;i++)
			fscanf(fp,"%d %d %d",&a1[i],&b1[i],&c1[i]);
		fclose(fp);
		//���ڽӱ������л�ȡ��Ҫ�޸�վ��ĵ�һ�����
		for(i=1;i<=arcnum;i++)
		{
			if(c1[i]==NO)
			{templabel=a1[i];break;}
		}

		fp=fopen("station.txt","r");
	
		for(i=templabel-stationnum;i<=templabel-1;i++)
		{
			fscanf(fp,"%s",tempname[i]);
			if( station_search_line(tempname[i],0)==0 )//������վ�㣬���Ƿ���ֹ�
			{istrans[i]=0;del1++;}//del1++;} //�ǻ��˵���Ϊ0�����˵���Ϊ1
			else
			{istrans[i]=1;del2++;}
			//printf("%d",istrans[i]);
		}
	
		fclose(fp);
		//���¶������ͱ�����Ϣ
		fp=fopen("stationinfo.txt","w");
		fprintf(fp,"%d\n%d",vexnum-del1-del2,del);
		fclose(fp);

		//ɾ������·����վ����
		fp=fopen("station.txt","r");
		for(i=1;i<=vexnum;i++)
			fscanf(fp,"%s",name1[i]);
		fclose(fp);
	

		fp=fopen("station.txt","w");
		for(i=1;i<=vexnum-del1-del2;i++)
			fprintf(fp,"%s\n",name1[i]);
		fclose(fp);
        printf("�ɹ�����һ��������·��Ϣ������·Ϊ%d����\n",MAX_LINE_NUM);

	 }
	 else   return;

		
}
void xiugaidata()
{
	Graph G,G1;
	FILE *fp;
	NODELINE L,p,r,s;
	int vexnum,arcnum;
	char station[20];
	char ch;
	int i,j,k,n,ret;

	int stationnum;
	int newroute[100];
	char newroutename[100][20];
	int logic;
	int NO;							//��·���//
    char name[15];					//��·����//
    char start[15];					//���վ//
    char destination[15];			//�յ�վ//
    int number;						//վ����//
    char starttime[10];				//����ʱ��//
    char endtime[10];				//�հ�ʱ��//
    char totaltime[10];				//����ʱ��//
    int price;						//Ʊ��//
	int a1[MAX_VERTEX_NUM],b1[MAX_VERTEX_NUM],c1[MAX_VERTEX_NUM]; //��ʱ
	int templabel;//���޸���·�ĵ�һ��վ��ı��
	char tempname[MAX_VERTEX_NUM][20];//���޸���·��վ�㼯��
	char tempname1[20];//��ȡ��ʱվ����
	int label;//��ʱ

	fp=fopen("stationinfo.txt","r");
	fscanf(fp,"%d%d",&vexnum,&arcnum);
	fclose(fp);
	fp=fopen("line.txt","r");
	fscanf(fp,"%d",&MAX_LINE_NUM);
	fclose(fp);
	L=(NODE *)malloc(sizeof(NODE));
	L->next=NULL;
	readfromfile_line(L);
	create_graph(&G);
	G1=G;
	/* *********************************����Ϊ�޸���·��Ϣ***************************** */
	r=L;
	while(r->next!=NULL)
		r=r->next;
        printf("��������Ҫ�޸ĵ���Ϣ��<x-��·��Ϣ/z-վ����Ϣ/else-����>");
	getchar();
	scanf("%c",&ch);
	if(ch=='X' || ch=='x')  
	{
		 printf("��������Ҫ�޸ĵ���·���(0-return)��");
         ret=scanf("%d",&NO);
		while(ret!=1)
		{
			fflush(stdin);
			lab:printf("����ȷ������·��ţ�");
			ret=scanf("%d",&NO);
		}
		if(NO==0)	return;
        if(NO<0)	{ printf("������һ������0�����֣�\n");goto lab;}
		s=L;
		while(s->next!=NULL)
		{
			s=s->next;
			if(s->data.NO==NO)
			{ logic=1; break; }
		}//�ҵ�S�ڵ�
        if(logic==0) { printf("û�и���·��ţ�\n"); return; }
          s->data.NO=NO;


		printf("�����������·���ƣ�");
		scanf("%s",s->data.name);
		printf("��������ʼվ��");
		scanf("%s",s->data.start);
		printf("�������յ�վ��");
		scanf("%s",s->data.destination);
		//վ�������ֲ���
		//printf("������վ������");
		//scanf("%d",&s->data.number);
		printf("����������ʱ�䣺");
		scanf("%s",s->data.starttime);
		printf("�������հ�ʱ�䣺");
		scanf("%s",s->data.endtime);
		printf("����������ʱ����");
		scanf("%s",s->data.totaltime);
		printf("������Ʊ�ۣ�");
        ret=scanf("%d",&s->data.price);
        while(ret!=1)
		{
			lab1:fflush(stdin);
			printf("����ȷ����Ʊ�ۣ�");
			ret=scanf("%d",&NO);
		}
        if((s->data.price<0)||(s->data.price>30))	{printf("������һ������0��С��30�����֣�");goto lab1;}
        

		
		/* *********************************����Ϊ�޸�վ����Ϣ������������Ϣ����������ģ�***************************** */
		//�����ڽӱ����ݣ�
		fp=fopen("stationdata.txt","r");
		for(i=1;i<=arcnum;i++)
			fscanf(fp,"%d %d %d",&a1[i],&b1[i],&c1[i]);
		fclose(fp);
		//���ڽӱ������л�ȡ��Ҫ�޸�վ��ĵ�һ�����
		for(i=1;i<=arcnum;i++)
		{
			if(c1[i]==NO)
			{templabel=a1[i];break;}
		}
		//��������վ����Ϣ�������ж�����վ���Ƿ�Ϊ���˵�
		fp=fopen("station.txt","r");
		for(i=1;i<=vexnum;i++)
		{
			fscanf(fp,"%s",tempname[i]);
			if( station_search_line(tempname[i],0)==0 )
				istrans[i]=0; //�ǻ��˵���Ϊ0�����˵���Ϊ1
			else
				istrans[i]=1;

		}
		fclose(fp);
		label=templabel;

		//�޸�վ����������ԭ�л�����Ϣ
		//�÷������޸�վ�����֣�����ԭ���ڽ���Ϣ��վ�����
		//�����ʹ�ã�ע�ⱸ�ݣ�
		printf("���������%d���ߵ�վ����Ϣ��\n",s->data.NO);
		stationnum=s->data.number;
		for(i=1;i<=stationnum;i++)
		{
			if( istrans[templabel] ==1) //ԭ����վ��Ϊ���˵㣬������Ҫ���������µĻ��˵�
			{	printf("��%dվ��Ϊ���˵�--%s\n",i,tempname[label]);
				strcpy(newroutename[i],tempname[label]);
				templabel++;
				label++;
				continue;
			}
			label++;
			printf("�������%d��վ������ƣ�",i);//ԭ����վ��Ϊ��ͨվ�㣬������Ҫ��������
			scanf("%s",station);
			templabel++;
			for(j=1;j<=MAX_VERTEX_NUM;j++)
			{
				if(strcmp(G.vertices[j].name,station)==0)
				{
						while(strcmp(G.vertices[j].name,station)==0)
						{
							printf("վ�����ظ��������������%d��վ������:",i);
							scanf("%s",station);
						}
						newroute[i]=vexnum;
						strcpy(newroutename[i],station);
				}
				else
				{
					newroute[i]=vexnum;
					strcpy(newroutename[i],station);

				}

			}
		}
		j=0;
		//����վ�����б����浽station.txt
		for(i=templabel-stationnum;i<=templabel-1;i++) //������������
		{
		
			j++;
			if( strcmp(newroutename[j],"0")!=0 )
			{strcpy(tempname[i],newroutename[j]);}
			else
				strcpy(tempname[i],"0");
		}
		fp=fopen("station.txt","w");
		
		for(i=1;i<=vexnum;i++){
			if(strcmp(tempname[i],"0")!=0)
			{fprintf(fp,"%s\n",tempname[i]);}//printf("%s",tempname[i]);}
		}
		fclose(fp);
		save_line_information(L);
		printf("�ɹ��޸���·��Ϊ��%d����·��Ϣ������·Ϊ%d����\n",NO,MAX_LINE_NUM);
	}
	else if(ch=='Z' || ch=='z')
	{
		printf("��������Ҫ�޸ĵ�վ�㣺");
		scanf("%s",station);
		fp=fopen("station.txt","r");
		for(i=1;i<=vexnum;i++)
		{
			
			fscanf(fp,"%s",tempname[i]);
			if( station_search_line(tempname[i],0)==0 )
				istrans[i]=0; //�ǻ��˵���Ϊ0�����˵���Ϊ1
			else
				istrans[i]=1;


		}

		fclose(fp);
		j=0,k=0;
		for(i=1;i<=vexnum;i++)
		{
			if(strcmp(tempname[i],station)==0)
			{
				j=i;
				k++;
				break;
			}
			j++;
		}
		if(istrans[j]==1)	{printf("��վ��Ϊ���˵㣡�������޸�\n");  ;return ;}
		
		if(k==0)
		{
			printf("�����ڸ�վ�㣡");
			return ;
		}
		printf("�������µ�վ������");
		scanf("%s",station);
		for(i=1;i<=vexnum;i++)
		{
			if(strcmp(tempname[i],station)==0)
			{
				printf("���������Ѵ��ڵĻ��˵㣡");
				return ;
			}
		}

		if(istrans[j]==0)
		
		{
			//printf("%s",tempname[j]);
			strcpy( tempname[j],station );
			fp=fopen("station.txt","w");
			for(i=1;i<=vexnum;i++)
			{
				if(strcmp(tempname[i],"0")!=0)
				{fprintf(fp,"%s\n",tempname[i]);}//printf("%s",tempname[i]);}
			}
			fclose(fp);
		}
		
	}
	else
	{
		return ;
	}

	
}

void save_line_information(NODELINE L)
{
	FILE *fp;
	NODELINE r;
	r=L;
	fp=fopen("line.txt","w");
	fprintf(fp,"%d\n",MAX_LINE_NUM);
	L=L->next->next;
	while(r->next!=NULL)
	{
		r=r->next;
		fprintf(fp,"%d\n%s\n%s\n%s\n%d\n%s\n%s\n%s\n%d\n",r->data.NO,r->data.name,r->data.start,r->data.destination,r->data.number,r->data.starttime,r->data.endtime,r->data.totaltime,r->data.price);
	}
	fclose(fp);
}
