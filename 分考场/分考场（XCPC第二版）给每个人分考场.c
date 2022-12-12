//第一版的缺陷是EXCEL原始表需要做大量处理才能将数据读入程序，第二版能够直接复制粘贴原始数据读入将其读入程序

/*
  算法：
  1.读入数据
  2.计算每所学校的队伍数
  3.学校按队伍数降序排序
  4.从队伍数最多的学校开始分考场
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_team_member 3//假设每队最多3人
#define MAX_room 200//假设最多200个考场
#define MAX_school_team 30//假设每校最多30队

struct persons{
	char name[50];
	int room_num;//考场编号
	char school[50];
	int team_member;
	char team[150];

	//int team_id;
	struct persons *pre;
	struct persons *next;
};

struct school{
	char school[50];
	int unassigned_team;//该校未分配的队伍数
	char team[MAX_school_team][150];//假设每所学校最多30队
//	int cnt_team;//该校队伍数

	struct school *pre;
	struct school *next;
};

int main(){
	int room_maxnum_teams;//每个考场最多队伍数
	int n_person;//总人数
	int cnt_school=0,cnt_team=0,i,j,flag1,flag2;
	//int room_maxnum_teams=20;
	int room_num = 0;//考场编号
	int room_team[MAX_room]={0};//记录每个考场的队伍数,假设最多200考场
	
	struct school *head = NULL;
	struct school *tail = NULL;
	struct school *p,*p1,*p2;
	struct persons *person,*person1;
	struct persons *person_head=NULL;
	struct persons *person_tail=NULL;
	
	printf("每个考场最多队伍数：");
	scanf("%d",&room_maxnum_teams);
	printf("输入总人数：");
	scanf("%d",&n_person);
	getchar();
	printf("\n");
	
	//读取姓名列
	printf("输入姓名列，不包含表头\n");
	for(i=0;i<n_person;i++){
		person=(struct persons *)malloc(sizeof(struct persons));
		gets(person->name);
//		printf("姓名验证：%s\n",person->name);
		//person->team_id=i;
		person->room_num=-1;//考场编号初始化
		if(person_head==NULL){
			person_head=person_tail=person;
			person_head->pre=NULL;
			person_tail->next=NULL;
		}else{
			person_tail->next=person;
			person->pre=person_tail;
			person_tail=person;
			person_tail->next=NULL;
		}
	}
	person=person_head;
	
	//读取学校列，同时计数各学校队伍数和人数
	head=tail=(struct school *)malloc(sizeof(struct school));
	head->next=NULL;
	head->pre=NULL;
	head->school[0]='\0';
	
	printf("输入学校列，不包含表头\n");
	for(i=0;i<n_person;i++){
//		printf("%s\n",person->name );
		gets(person->school);
		if(head->school[0]=='\0'){
			strcpy(head->school,person->school);
			cnt_school++;
		}else{
			flag1=1;//假设输入的学校不存在
			for(p=head;p!=NULL;p=p->next){
				if(!strcmp(person->school,p->school)){
					flag1=0;
					break;
				}
			}
			if(flag1){//循环完了，输入的学校确实不存在
				p=(struct school *)malloc(sizeof(struct school));
				strcpy(p->school,person->school);
				p->pre=tail;
				p->next=NULL;
				tail->next=p;
				tail=p;
				cnt_school++;
			}
		}
//		printf("学校验证：%s  %s\n",person->name,person->school);
		person=person->next;
	}
	person=person_head;
	
	//初始化所有学校的队伍数为0
	for(p=head;p!=NULL;p=p->next)
		p->unassigned_team=0;

	//读取队名,同时计算各校队伍数

	printf("输入队名列，不包含表头\n");
	for(i=0;i<n_person;i++){
//		printf("%s   %s\n",person->school,person->name);
		gets(person->team);
//		printf("%s   %s    %s\n",person->school,person->name,person->team);
		//看这个人所在的学校是否已有这个队
		flag1=0;//还没找到这个人的学校
		flag2=1;//假设这个人所在的学校没有这个队
		for(p=head;p!=NULL;p=p->next){
//			printf("当前学校：%s\n",p->school);
			if(!strcmp(person->school,p->school)){
//				printf("找到了这个人所在的学校：%s\n",p->school);
				flag1=1;//找到了这个人所在的学校
				for(j=0;j<p->unassigned_team;j++){
					if(!strcmp(person->team,p->team[j])){
						flag2=0;//这个学校已经有这个队伍了
						break;
					}
				}
				if(flag2){//这个学校还没有这个队伍
					cnt_team++;
					//p->cnt_team++;
					p->unassigned_team++;
					strcpy(p->team[j],person->team);
				}
			}
			if(flag1)break;
		}
		person=person->next;
	}
printf("cnt_team=%d",cnt_team);
	//计算每个队伍的人数
	person=person_head;
	for(i=0;i<n_person;i++){
		if((!i)||(strcmp(person->school,person->pre->school))||(strcmp(person->team,person->pre->team))){
			person->team_member=1;
			if(i!=n_person-1)person1=person->next;
			else break;
			for(j=0;j<9;j++){
				if(!strcmp(person->school,person1->school)){
//					printf("%s  %s  %s队伍人数计数：%d\n",person->school,person->team,person->name,person->team_member);
					if(!strcmp(person->team,person1->team)){
						person->team_member++;
						person1=person1->next;
						if(person1==NULL)break;
					}else break;
				}else break;
			}
		}else{
			if(!strcmp(person->school,person->pre->school)&&!strcmp(person->team,person->pre->team))
				person->team_member=person->pre->team_member;
			
		}
		printf("%s  %s  %s  队伍人数：%d\n",person->school,person->team,person->name,person->team_member);
		person=person->next;
	}
printf("分考场\n");
	//分考场
	//假如存在某个学校的队伍数大于考场容量，先给这个学校分配考场，分配的队伍数量为考场容量的整数倍
	for(p=head;p!=NULL;p=p->next) {//逐一查看每个学校
		person=person_head;
		printf("%s 未分配队伍数：%d\n",p->school,p->unassigned_team);
		while(p->unassigned_team>=room_maxnum_teams){
			//当前学校未分配的队伍数如果大于考场容量，则先填满一个考场
			for(room_team[room_num]=0;room_team[room_num]<room_maxnum_teams||p->unassigned_team==1;person=person->next){
				if(!strcmp(p->school,person->school)){
					person1=person;
					for(i=0;i<person1->team_member;i++){
						person->room_num=room_num;
						person=person->next;
					}
					p->unassigned_team--;
					room_team[room_num]++;
					if(person!=NULL)person=person->pre;
					else break;
				}
			}
			printf("第%d个考场%d队。\n", room_num,room_team[room_num]);
			room_num++;//下一个考场
		}
		
	}
	
printf("将学校按照未分配队伍数降序排序\n");
	//将学校按照未分配队伍数降序排序
	//先找出队伍数最多的学校作为头结点
	
	for(p=head,p1=head;p!=NULL;p=p->next)
		if(p->unassigned_team>p1->unassigned_team)p1=p;
	
	if(p1!=head){
		if(p1!=tail)
			p1->next->pre=p1->pre;
		else tail=p1->pre;
		p1->pre->next=p1->next;
		head->pre=p1;
		p1->pre=NULL;
		p1->next=head;
		head=p1;
	}
	
	p2=p1;
	p1=p1->next;
	
	//链表剩余部分排序,p2是上一轮找到的最大项，p1是本轮找到的最大项
	for(i=1;i<cnt_school-1;i++){
//		printf("第 %d 所学校：%s  %d队未分配\n",i,p2->school,p2->unassigned_team);
		for(p=p1;p!=NULL;p=p->next)
			if(p->unassigned_team>p1->unassigned_team)p1=p;
		if(p1!=p2->next){
			p1->pre->next=p1->next;
			if(p1!=tail)
				p1->next->pre=p1->pre;
			else tail=p1->pre;
			p1->pre=p2;
			p1->next=p2->next;
			p2->next->pre=p1;
			p2->next=p1;
		}
		p2=p1;
		p1=p1->next;
	}

//	printf("给各学校未分配考场的队伍分配考场\n");
	//给各学校未分配考场的队伍分配考场，此时各学校未分配的队伍数量都小于考场容量
	/*
	  从第0个学校开始，遍历所有队伍，看是否有未分配的队伍，
	  如果有未分配的队伍，则将这一个学校的所有未分配队伍分配进同一个考场。
	  看是否存在unassigned_team!=0的学校未分配队伍数量相加小于考场容量，如果相加小于考场容量也分配进同一考场，
	  如果遍历完了没有找到可以分配进同一考场的学校，那么遇到队伍数量相同的学校不再遍历，
	 */
	
	
	
	for(p=head;p!=NULL;p=p->next) {
		person=person_head;
		if(p->unassigned_team!=0){//当前学校有未分配的队伍
			//给当前学校未分配的队伍分配考点
//			printf("给 %s 未分配的队伍分配考点\n",p->school);
			while(p->unassigned_team>0){
				if(!strcmp(p->school,person->school)){
					if(person->room_num==-1){
						person1=person;
						for(i=0;i<person1->team_member;i++){
							person->room_num=room_num;
							person=person->next;
						}
						p->unassigned_team--;
						room_team[room_num]++;
						if(person!=NULL)person=person->pre;
						else break;
					}
				}
				if(person!=NULL)person=person->next;
				else break;
			}
//			printf("寻找与 %s 组合学校\n",p->school);
			
			for(p1=p->next;p1!=NULL;p1=p1->next){
				if(p1->unassigned_team!=0){
					if(p1->unassigned_team+room_team[room_num]<=room_maxnum_teams){
//						printf("找到 %s 与 %s 组合,%s %d\n",p1->school,p->school,p1->school,p1->unassigned_team);
						person=person_head;
						while(p1->unassigned_team>0){
							if(!strcmp(p1->school,person->school)){
								if(person->room_num==-1){
									person1=person;
									for(i=0;i<person1->team_member;i++){
										person->room_num=room_num;
										person=person->next;
									}
									p1->unassigned_team--;
									room_team[room_num]++;
									if(person!=NULL)person=person->pre;
									else break;
								}
							}
							if(person!=NULL)person=person->next;
							else break;
						}
					}
				}
				if(room_team[room_num]==room_maxnum_teams)break;
				
			}
			person=person_head;
			printf("第%d个考场%d队。\n", room_num,room_team[room_num]);
			room_num++;
			
		}
	}
	printf("检查最后一个考场是否填满。\n");
	room_num--;//最后一个考场的考场编号
	//如果最后一个考场的队伍数小于考场容量，并且小于总的考场数，并且每个队伍来自不同的学校，则把最后一个考场的队伍均分到其他考场
	if(room_team[room_num]<room_maxnum_teams&&room_team[room_num]<room_num){
		printf("需要手动修改最后一个考场队伍的考场编号。\n");
	}
	
	printf("回车输出每个人的考点\n");
	getchar();
	
	//文件输出
	FILE *fout = NULL;
	FILE *fout1 = NULL;

	
	//输出每个人的考点
	person=person_head;
	for(i=0;i<n_person;i++){
		printf("%s   %s   %s   线上考点 %03d\n",person->school,person->team,person->name,person->room_num);
		fout = fopen("分考场.txt", "a");
		fout1 = fopen("分考场.csv", "a");
		fprintf(fout, "%s   %s   %s   线上考点 %03d\n",person->school,person->team,person->name,person->room_num);
		fprintf(fout1, "%s   %s   %s   线上考点 %03d\n",person->school,person->team,person->name,person->room_num);
		fclose(fout);
		fclose(fout1);
		person=person->next;
	}

	printf("输出完毕！！！\n");


	p=head->next;
	while(p!=NULL){
		free(p->pre);
		p=p->next;
	}
	free(tail);
	
	person=person_head->next;
	while(person!=person_tail){
		free(person->pre);
		person=person->next;
	}
	free(person_tail);
	getchar();
	getchar();
	return 0;
}

