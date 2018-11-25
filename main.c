#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VERTEX_NUM 200
#define HEADER1 "--------------------------------广州地铁线路----------------------------------\n"
#define HEADER2 "|NO| 线路名称 |  起点站  |  终点站  |站点数|起运时间|收班时间| 运行时长 |票价|\n"
#define HEADER3 "|--|----------|----------|----------|------|--------|--------|----------|----|\n"
#define HEADER4 "------------------------------------------------------------------------------\n"
#define HEADER5 "--------------------------------------------\n"
#define HEADER6 "|  站点名称  | 是否可换乘 |  所属线路编号  |\n"


#define FORMAT "|%-2d|%-10s|%-10s|%-10s|%-6d|%-8s|%-8s|%-10s|%-4d|\n"
#define DATA p->data.NO,p->data.name,p->data.start,p->data.destination,p->data.number,p->data.starttime,p->data.endtime,p->data.totaltime,p->data.price

int MAX_LINE_NUM=8;
int istrans[MAX_VERTEX_NUM]={0};

typedef struct line
{
    int NO;							//线路编号//
    char name[15];					//线路名称//
    char start[15];					//起点站//
    char destination[15];			//终点站//
    int number;						//站点数//
    char starttime[10];				//起运时间//
    char endtime[10];				//收班时间//
    char totaltime[10];				//运行时长//
    int price;						//票价//
}LINE;

typedef struct node					//line结构体接口//
{
    LINE data;
    struct node *next;
}NODE,*NODELINE;


typedef struct enode_type
{
	int adjvex;                     //邻接点的位置//
	int weight;                     //权值（即线路编号)//
	struct enode_type *nextarc;     //指向下一个邻接点//
}enode;

typedef struct vnode_type
{
	int id;                         //顶点的编号//
	char name[20];                  //顶点的名字，即站的名字//
	enode *fadj;					//指向第一个点//
}vnode;

typedef struct Graph_type
{
	vnode vertices[MAX_VERTEX_NUM]; //顶点集合(vertex)//
	int vexnum;                     //图的顶点数目//
	int arcnum;                     //图的边数目//

}Graph;

typedef struct pathnode_type
{
	int top;                        //路径的数目//
	int path[MAX_VERTEX_NUM];       //路径信息//
	int trans[MAX_VERTEX_NUM];		//换乘点信息,0为非换乘点，1为换乘点//
	int counter;                    //换乘的次数//
	struct pathnode_type *next;     //指向下一个可抵达终点的路径信息节点//
}pathnode;


typedef struct LINK_type
{
	pathnode *head;                 //路径接口//
}LINK;



int initialize();
//菜单//
void menu();
//功能一：显示线路信息//
void Initline();
//功能一：读取线路信息//
void readfromfile_line(NODELINE L);
//输出表头//
void printheader();
//打印数据//
void printdata(NODE *q);
//功能一：显示线路信息接口//
void showline(NODELINE L);

//获取站点的编号，从1-130//
int locatevex(Graph *G,char *a);
//获取当前线路号//			
int getline(Graph *G,int i,int j);
//获取换乘点//
void gettrans(Graph *G,pathnode *all);
//建立带无向网络//
void create_graph(Graph *G);
//DFS深度优先搜索出所有可能的路线走法//
void DFS(Graph *G,int stack[],int visited[],int v,int destination,int top,LINK *L);
//输出最少换乘路径信息//
void print_lesstransfer(Graph *G,LINK *L);
//最少换乘的函数接口，参数1：起始站，参数2：终点站//
void lesstransfer(char start[],char destination[]);
//创建一个路径链表//
void create_path(LINK *L);
//插入一个路径信息//
void addpath(LINK *L,pathnode *p);
//计费，（按每个站点之间1公里简易计算，与实际有很大差别）//
int caculate_price(int station);
//乘车时间
int caculate_time(int station,int count);


//站点查询线路,可实现换乘站点自动判断
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
        printf("请输入<0-7>:");
        scanf("%d",&label);
        if(label==0) {printf("感谢您的使用！\n"); exit(0);}
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
			printf("请输入起始站：");
			scanf("%s",start);
			printf("请输入终点站：");
			scanf("%s",destination);

			if(strcmp(start,destination)==0)
				printf("您输入的站点名相同！");
			else
				lesstransfer(start,destination);//最小换乘接口//
            break;
        case 6:
			//可实现换乘站点的自动判断
            printf("请输入站点名：");
			scanf("%s",start);
			if(station_search_line(start,1)==-1)
				printf("未查询到%s站的信息！\n",start);
            break;
        default:
            fflush(stdin);
            printf("您输入的指令有误！\n");
        }
        printf("\n按任意键继续...\n");
        getch();
    }


    return 0;
}

void menu()
{
    printf("\t|-----------------------------------------------|\n");
    printf("\t|              广州地铁查询系统                 |\n");
    printf("\t|-----------------------------------------------|\n");
    printf("\t|       0:退出系统                              |\n");
    printf("\t|       1:显示地铁线路                          |\n");
    printf("\t|       2:录入地铁线路                          |\n");
    printf("\t|       3:修改地铁线路                          |\n");
    printf("\t|       4:删除地铁线路                          |\n");
    printf("\t|       5:换乘费用查询                          |\n");
    printf("\t|       6:站点名查线路                          |\n");
    printf("\t|-----------------------------------------------|\n");
}

int initialize()
{
	int flag=1;
	FILE *fp;
	fp=fopen("stationinfo.txt","r");
	if(fp==NULL) {printf("缺失stationinfo.txt\n");flag=0;}
	fclose(fp);
	fp=fopen("station.txt","r");
	if(fp==NULL) {printf("缺失station.txt\n");flag=0;}
	fclose(fp);
	fp=fopen("stationdata.txt","r");
	if(fp==NULL) {printf("缺失stationdata.txt\n");flag=0;}
	fclose(fp);
	fp=fopen("line.txt","r");
	if(fp==NULL) {printf("缺失line.txt\n");flag=0;}
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
	printf("是否显示所有地铁站点？<Y or N>");
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
		
			printf("-----------------------------------地铁%d号线-----------------------------------\n",L->data.NO);
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
    int NO;                 /*线路编号*/
    char name[15];          /*线路名称*/
    char start[15];         /*起点站*/
    char destination[15];   /*终点站*/
    int number;             /*站点数*/
    char starttime[10];     /*起运时间*/
    char endtime[10];       /*收班时间*/
    char totaltime[10];     /*运行时长*/
    int price;              /*票价*/
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
    if(r->next==NULL) printf("无地铁线路！\n");
    while(r->next!=NULL)
    {
        r=r->next;
        printdata(r);
    }
    printf(HEADER4);
}







//最少换乘的函数接口，参数1：起始站，参数2：终点站//
void lesstransfer(char start[],char destination[])
{
	Graph G;
	LINK L;

	int v1,v2;
	int stack[MAX_VERTEX_NUM]={0};     //初始化
	int  visited[MAX_VERTEX_NUM]={0};
	create_graph(&G);	 //创建一个无向网络//
	create_path(&L);	//创建保存路径的链表//
	v1=locatevex(&G,start);
	v2=locatevex(&G,destination);
 
	if(v1==-1 || v2==-1)
	{
		printf("您输入的站名有误！\n");
		return;
	}

	DFS(&G,stack,visited,v1,v2,0,&L);
	print_lesstransfer(&G,&L);

}

//获取站点的编号，从1-130//
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
//创建一个无向网络
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
		//利用头插法插入结点
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
		//无向网络，双向邻接
		p=(enode *)malloc(sizeof(enode));
		p->adjvex=i;
		p->weight=weight;
		p->nextarc=G->vertices[j].fadj;
		G->vertices[j].fadj=p;
	}
	fclose(fp);
}
/*利用深度搜索的递归找出所有的路径
参数一：图  参数2：临时存放路径顺序数组  参数3：是否访问  参数4：当前位置  参数5：到达的位置  参数6：栈顶  参数7：所有路径的链表*/
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
		return ;//递归边界条件
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

//输出路径//
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
			printf("\n最少换乘线路：\n");
			printf("------------------------->-------------------------->------------------------->\n");
			for(k=0;k<(r->top);k++)
			{
				if(r->trans[k])
				{
					printf("%s(在此换乘%d号线)---->",G->vertices[r->path[k]].name,G->vertices[r->path[k]].fadj->weight);
					i++;
				}
				else
				{
					line=G->vertices[r->path[k]].fadj->weight;
					printf("%s(%d|%d号线)-->",G->vertices[r->path[k]].name,G->vertices[r->path[k]].id,line);
					i++;
				}
			}
			printf("%s(终点)\n",G->vertices[r->path[(r->top)]].name);
			printf("-------------------------------------------------------------------------------\n");
			if(maximum==0)
				printf("不需要换乘\n");
			else
				printf("需要换乘:%d次\n",maximum);
			printf("经过站点:%d个\n",i);
			printf("乘车时间:%d分钟\n",caculate_time(i,maximum));
			printf("票价为:%d元\n",caculate_price(i));
			break;//为了避免重复，直接跳出
		}
	}
}

void create_path(LINK *L)
{
	//新建链表操作//
	L->head=(pathnode *)malloc(sizeof(pathnode));
	L->head->next=NULL;
}

void addpath(LINK *L,pathnode *p)
{
	//链表的插入操作//
	p->next=L->head->next;
	L->head->next=p;
}
//求出同一条边上的两点间的线路号//
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

//求出每条路线上的转乘次数//
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
//简易价格计算//
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
//乘车时间计算//
int caculate_time(int station,int count)
{
	return (3*station+count*6);
}
//站点名查找线路信息  自动判断换乘点 //
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
			printf("|%12s|%12s|%12d号线|\n",station,"否",line[0]);
			printf(HEADER5);
			
		}
			//printf("%s站位于地铁%d号线\n",station,line[0]);
		else
		{
			printf(HEADER5);
			printf(HEADER6);
			printf(HEADER5);
			printf("|%-12s|%-12s|  %d号线和%d号线  |\n",station,"是",line[0],line[1]);
			printf(HEADER5);
		}
			//printf("%s站是换乘站，位于地铁%d号线和地铁%d线\n",station,line[0],line[1]);
	}
	else if(parameter==0)
	{
		if(line[1]==0)
			return 0;
		else
			return 1;
	}
	else
		printf("参数传递错误！\n");

}
//添加数据  
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
	
	int NO;							//线路编号//
    char name[15];					//线路名称//
    char start[15];					//起点站//
    char destination[15];			//终点站//
    int number;						//站点数//
    char starttime[10];				//起运时间//
    char endtime[10];				//收班时间//
    char totaltime[10];				//运行时长//
    int price;						//票价//

	char str[20];
	int ret;
	fp=fopen("stationinfo.txt","r");
	fscanf(fp,"%d%d",&vexnum,&arcnum);
	fclose(fp);
	fp=fopen("line.txt","r");
	fscanf(fp,"%d",&MAX_LINE_NUM);
	fclose(fp);
	vexnum++;//新的站点的序号
	L=(NODE *)malloc(sizeof(NODE));
	L->next=NULL;
	readfromfile_line(L);
	create_graph(&G);
	G1=G;
	/* *********************************以下为录入线路信息***************************** */
	r=L;
	while(r->next!=NULL)
		r=r->next;
label: printf("请输入新的线路编号(0-返回)：",MAX_LINE_NUM+1);//考虑不为负数
	   ret=scanf("%d",&NO);
	if(NO==0)	return;
   while(ret!=1)
		{
			lab:fflush(stdin);
			printf("请正确输入线路编号：");
			ret=scanf("%d",&NO); 
		
		}
        	if(NO!=MAX_LINE_NUM+1)
			{
				printf("输入的线路编号应为现有线路编号的下一位！(%d号)\n",MAX_LINE_NUM+1); goto lab;
			}
		s=L;
		while(s)
		{
			if(s->data.NO==NO)
			{
				printf("该地铁线路编号已存在！\n");
				getchar();
				goto label;
			}
			s=s->next;
		}
		p=(NODE *)malloc(sizeof(NODE));
		p->data.NO=NO;
		printf("请输入地铁线路名称：");
		scanf("%s",p->data.name);
		printf("请输入起始站：");
		scanf("%s",str);
		for(i=0;str[i]!='\0';i++)
			if(isdigit(str[i]))
			{
				fflush(stdin);
				printf("站点名不能带有数字！\n请重新输入起始站：");
				scanf("%s",str);
				break;
			}
		strcpy(p->data.start,str);
		//scanf("%s",p->data.start);
		memset(str,0,sizeof(str));
		printf("请输入终点站：");
		scanf("%s",str);
		for(i=0;str[i]!='\0';i++)
		{
			if(isdigit(str[i]))//判断是否数字
			{
				fflush(stdin);//清空文件缓冲
				printf("站点名不能带有数字！\n请重新输入终点站：");
				scanf("%s",str);
				break;
			}
		}
		strcpy(p->data.destination,str);
		//scanf("%s",p->data.destination);
		memset(str,0,sizeof(str));
		printf("请输入站点数：");
		ret=scanf("%d",&p->data.number);
		while(ret!=1)
		{
			label1:fflush(stdin);
			printf("请正确输入站点数：");
			ret=scanf("%d",&p->data.number);
		}
         if(p->data.number<0) { printf("请输入一个大于0的数字！\n");  goto label1; }
		printf("请输入起运时间：");
		scanf("%s",p->data.starttime);
		printf("请输入收班时间：");
		scanf("%s",p->data.endtime);
		printf("请输入运行时长：");
		scanf("%s",p->data.totaltime);
		printf("请输入票价：");
		ret=scanf("%d",&p->data.price);
		while(ret!=1)
		{
			label2:fflush(stdin);
			printf("请正确输入票价：");
			ret=scanf("%d",&p->data.price);
		}
         if((p->data.price<0)||(p->data.price>30)) { printf("请输入一个大于0且小于30的数字！\n");  goto label2; }
		p->next=NULL;
		r->next=p;
		r=p;
		MAX_LINE_NUM++;
		//arcnum+=number;
		
		/* *********************************以上为录入线路信息***************************** */
		
		/* *********************************以下为录入站点信息***************************** */
		printf("请输入地铁%d号线的站点信息：\n",r->data.NO);
		stationnum=r->data.number;
		for(i=1;i<=stationnum;i++)
		{
			printf("请输入第%d个站点的名称：",i);
			scanf("%s",station);
		

			for(j=1;j<=MAX_VERTEX_NUM;j++)
			{
				if(strcmp(G.vertices[j].name,station)==0)
				{
					printf("该站点是否为换乘点<0-1>?");//站点名重复，要么是换乘点，要么是站点名重复输错
					scanf("%d",&istrans);
					if(istrans)
					{
						newroute[i]=locatevex(&G,station);//寻找第一次出现的地方
						strcpy(newroutename[i],"0");//如果有把它置为0
						vexnum--;
						break;
					}
					else
					{
						while(strcmp(G.vertices[j].name,station)==0)
						{
							printf("站点名重复！请重新输入第%d个站点名称:",i);
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


		fp=fopen("stationdata.txt","a+");//添加新增站点信息
		for(i=1;i<=stationnum;i++)
		{
			if(i+1<=stationnum)//另接表要+1
			{fprintf(fp,"%d %d %d\n",newroute[i],newroute[i+1],r->data.NO);arcnum++;}

		}
		fclose(fp);

		fp=fopen("station.txt","a+");//换乘点不重复则添加
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

		/* *********************************以上为录入站点信息***************************** */

		save_line_information(L);
		printf("成功更新一条地铁线路信息，总线路为%d条！\n",MAX_LINE_NUM);

	
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
	int logic=0; //判断有无站点
	int NO;							//线路编号//
    char name[15];					//线路名称//
    char start[15];					//起点站//
    char destination[15];			//终点站//
    int number;						//站点数//
    char starttime[10];				//起运时间//
    char endtime[10];				//收班时间//
    char totaltime[10];				//运行时长//
    int price;						//票价//
	int a1[MAX_VERTEX_NUM],b1[MAX_VERTEX_NUM],c1[MAX_VERTEX_NUM];
	char name1[MAX_VERTEX_NUM][20];//记录删除站点的名字
	int del=0,del1=0,del2=0;//记录删除站点的数目
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
	/* *********************************以下为录入线路信息***************************** */
	r=L;
     printf("请问您是否要删除最后一条线路？(A-是,else-否):");
	 getchar();
     scanf("%s",&c);
     if((strcmp("a",c)==0)||(strcmp("A",c)==0))
     {
		 NO=MAX_LINE_NUM;
    /*printf("请输入要删除的的线路编号(0-返回)：");
        if( scanf("%d",&NO) !=1)
		{
			fflush(stdin);
			printf("请正确线路编号：");
			scanf("%d",&NO); 
		}*/
		s=L;
		while(s->next!=NULL)
		{
			s=s->next;
			if(s->data.NO==NO)
			{logic=1; break;}
		}//找到S节点
		
		if(logic==0) {printf("该线路不存在!\n");return ;}
		//del1=s->data.number;
		stationnum=s->data.number;
		while(r->next!=s)
          r=r->next;//找到S节点前驱
         r->next=s->next;
         free(s);
		MAX_LINE_NUM--;


        save_line_information(L);
		fp=fopen("stationdata.txt","r");
		//删除该线路的所有站点邻接信息
		for(i=1;i<=arcnum;i++)
			fscanf(fp,"%d %d %d",&a1[i],&b1[i],&c1[i]);//a[]，b[]站点，c[]线路编号
		fclose(fp);
		fp=fopen("stationdata.txt","w");
		for(i=1;c1[i]!=NO;i++)
		{fprintf(fp,"%d %d %d\n",a1[i],b1[i],c1[i]);del++;}
		fclose(fp);
	
		//读入邻接表数据，
		fp=fopen("stationdata.txt","r");
		for(i=1;i<=arcnum;i++)
			fscanf(fp,"%d %d %d",&a1[i],&b1[i],&c1[i]);
		fclose(fp);
		//从邻接表数据中获取需要修改站点的第一个序号
		for(i=1;i<=arcnum;i++)
		{
			if(c1[i]==NO)
			{templabel=a1[i];break;}
		}

		fp=fopen("station.txt","r");
	
		for(i=templabel-stationnum;i<=templabel-1;i++)
		{
			fscanf(fp,"%s",tempname[i]);
			if( station_search_line(tempname[i],0)==0 )//找所有站点，看是否出现过
			{istrans[i]=0;del1++;}//del1++;} //非换乘点置为0，换乘点置为1
			else
			{istrans[i]=1;del2++;}
			//printf("%d",istrans[i]);
		}
	
		fclose(fp);
		//更新顶点数和边数信息
		fp=fopen("stationinfo.txt","w");
		fprintf(fp,"%d\n%d",vexnum-del1-del2,del);
		fclose(fp);

		//删除该线路所有站点名
		fp=fopen("station.txt","r");
		for(i=1;i<=vexnum;i++)
			fscanf(fp,"%s",name1[i]);
		fclose(fp);
	

		fp=fopen("station.txt","w");
		for(i=1;i<=vexnum-del1-del2;i++)
			fprintf(fp,"%s\n",name1[i]);
		fclose(fp);
        printf("成功更新一条地铁线路信息，总线路为%d条！\n",MAX_LINE_NUM);

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
	int NO;							//线路编号//
    char name[15];					//线路名称//
    char start[15];					//起点站//
    char destination[15];			//终点站//
    int number;						//站点数//
    char starttime[10];				//起运时间//
    char endtime[10];				//收班时间//
    char totaltime[10];				//运行时长//
    int price;						//票价//
	int a1[MAX_VERTEX_NUM],b1[MAX_VERTEX_NUM],c1[MAX_VERTEX_NUM]; //临时
	int templabel;//欲修改线路的第一个站点的编号
	char tempname[MAX_VERTEX_NUM][20];//欲修改线路的站点集合
	char tempname1[20];//获取临时站点名
	int label;//临时

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
	/* *********************************以下为修改线路信息***************************** */
	r=L;
	while(r->next!=NULL)
		r=r->next;
        printf("请输入您要修改的信息：<x-线路信息/z-站点信息/else-返回>");
	getchar();
	scanf("%c",&ch);
	if(ch=='X' || ch=='x')  
	{
		 printf("请输入您要修改的线路编号(0-return)：");
         ret=scanf("%d",&NO);
		while(ret!=1)
		{
			fflush(stdin);
			lab:printf("请正确输入线路编号：");
			ret=scanf("%d",&NO);
		}
		if(NO==0)	return;
        if(NO<0)	{ printf("请输入一个大于0的数字！\n");goto lab;}
		s=L;
		while(s->next!=NULL)
		{
			s=s->next;
			if(s->data.NO==NO)
			{ logic=1; break; }
		}//找到S节点
        if(logic==0) { printf("没有该线路编号！\n"); return; }
          s->data.NO=NO;


		printf("请输入地铁线路名称：");
		scanf("%s",s->data.name);
		printf("请输入起始站：");
		scanf("%s",s->data.start);
		printf("请输入终点站：");
		scanf("%s",s->data.destination);
		//站点数保持不变
		//printf("请输入站点数：");
		//scanf("%d",&s->data.number);
		printf("请输入起运时间：");
		scanf("%s",s->data.starttime);
		printf("请输入收班时间：");
		scanf("%s",s->data.endtime);
		printf("请输入运行时长：");
		scanf("%s",s->data.totaltime);
		printf("请输入票价：");
        ret=scanf("%d",&s->data.price);
        while(ret!=1)
		{
			lab1:fflush(stdin);
			printf("请正确输入票价：");
			ret=scanf("%d",&NO);
		}
        if((s->data.price<0)||(s->data.price>30))	{printf("请输入一个大于0且小于30的数字！");goto lab1;}
        

		
		/* *********************************以下为修改站点信息（保留换乘信息，不允许更改）***************************** */
		//读入邻接表数据，
		fp=fopen("stationdata.txt","r");
		for(i=1;i<=arcnum;i++)
			fscanf(fp,"%d %d %d",&a1[i],&b1[i],&c1[i]);
		fclose(fp);
		//从邻接表数据中获取需要修改站点的第一个序号
		for(i=1;i<=arcnum;i++)
		{
			if(c1[i]==NO)
			{templabel=a1[i];break;}
		}
		//读入所有站点信息，并且判断所有站点是否为换乘点
		fp=fopen("station.txt","r");
		for(i=1;i<=vexnum;i++)
		{
			fscanf(fp,"%s",tempname[i]);
			if( station_search_line(tempname[i],0)==0 )
				istrans[i]=0; //非换乘点置为0，换乘点置为1
			else
				istrans[i]=1;

		}
		fclose(fp);
		label=templabel;

		//修改站点名，保留原有换乘信息
		//该方法仅修改站点名字，保留原有邻接信息和站点序号
		//请谨慎使用，注意备份！
		printf("请输入地铁%d号线的站点信息：\n",s->data.NO);
		stationnum=s->data.number;
		for(i=1;i<=stationnum;i++)
		{
			if( istrans[templabel] ==1) //原来该站点为换乘点，现在需要重新输入新的换乘点
			{	printf("第%d站点为换乘点--%s\n",i,tempname[label]);
				strcpy(newroutename[i],tempname[label]);
				templabel++;
				label++;
				continue;
			}
			label++;
			printf("请输入第%d个站点的名称：",i);//原来该站点为普通站点，现在需要更改名字
			scanf("%s",station);
			templabel++;
			for(j=1;j<=MAX_VERTEX_NUM;j++)
			{
				if(strcmp(G.vertices[j].name,station)==0)
				{
						while(strcmp(G.vertices[j].name,station)==0)
						{
							printf("站点名重复！请重新输入第%d个站点名称:",i);
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
		//更新站点名列表，保存到station.txt
		for(i=templabel-stationnum;i<=templabel-1;i++) //这里修正过了
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
		printf("成功修改线路号为：%d的线路信息，总线路为%d条！\n",NO,MAX_LINE_NUM);
	}
	else if(ch=='Z' || ch=='z')
	{
		printf("请输入您要修改的站点：");
		scanf("%s",station);
		fp=fopen("station.txt","r");
		for(i=1;i<=vexnum;i++)
		{
			
			fscanf(fp,"%s",tempname[i]);
			if( station_search_line(tempname[i],0)==0 )
				istrans[i]=0; //非换乘点置为0，换乘点置为1
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
		if(istrans[j]==1)	{printf("该站点为换乘点！不允许修改\n");  ;return ;}
		
		if(k==0)
		{
			printf("不存在该站点！");
			return ;
		}
		printf("请输入新的站点名：");
		scanf("%s",station);
		for(i=1;i<=vexnum;i++)
		{
			if(strcmp(tempname[i],station)==0)
			{
				printf("请勿输入已存在的换乘点！");
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
