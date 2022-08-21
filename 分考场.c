#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct team {/*队伍信息*/
	char team[100];//队伍名称
	int num;//该队考场编号
};


struct school {
	char school[80];//学校名称
	int n_team; //各校队伍总数
	int n_unassigned;//该校未分配队伍数
	struct team name_team[100];//假定每个学校不超过100个队伍
	struct school *next;
};



int main() {

	int N_school;//学校总数
	int Room_maxnum_team;//每个考场最多队伍数

	printf("请输入每个考场最多队伍数：");
	scanf("%d", &Room_maxnum_team);
	getchar();
	printf("\n请输入学校总数：");
	scanf("%d", &N_school);
	getchar();

	struct school *head = NULL;
	struct school *tail = NULL;
	struct school *p;
	int i, j, num_room;
	char temp_school[80];//%s学校名称
	int temp_n_team;//%d各校队伍总数

	//读取队伍信息

	printf("请输入 学校 列(样例表格E列，不包含表头)：\n");
	for (i = 1; i <= N_school; i++) {
		printf("第%d所学校\n", i);
		gets(temp_school);
		p = (struct school *)malloc(sizeof(struct school));
		strcpy(p->school,temp_school);
		if (head == NULL)
			head = tail = p;
		else {
			tail->next = p;
			tail = p;
		}
	}

	p = head;

	printf("请输入 各校队伍总数 列(样例表格F列，不包含表头)：\n");
	for (i = 1; i <= N_school; i++) {
		printf("第%d所学校队伍总数\n", i);
		scanf("%d", &temp_n_team);
		getchar();
		p->n_unassigned = p->n_team = temp_n_team;
		p = p->next;
	}

	p = head;

	printf("请输入 队名 列(样例表格B列，不包含表头)：\n");
	for (i = 1; i <= N_school; i++) {
		for (j = 0; j < p->n_team; j++) {
			gets(p->name_team[j].team);//读入队名
			p->name_team[j].num = -1;//初始化考场编号为-1
		}
		p = p->next;
	}

	//给每个队伍分考场

	num_room = 0;//考场编号
	p = head;
	int cnt_team;

	//假如存在某个学校的队伍数大于考场容量，先给这个学校分配考场，分配的队伍数量为考场容量的整数倍
	for (i = 0; i < N_school; i++) {
		j = 0;
		while (p->n_unassigned >= Room_maxnum_team) {

			for ( cnt_team = 0; cnt_team < Room_maxnum_team; cnt_team++) {
				p->name_team[j].num = num_room;
				j++;
			}
			num_room++;
			p->n_unassigned -= Room_maxnum_team;
		}
		p = p->next;
	}

	//给各学校未分配考场的队伍分配考场，此时各学校未分配的队伍数量都小于考场容量

	/*
	从第0个学校开始，遍历所有队伍，看是否有未分配的队伍（p->n_unassigned！=0），
	如果有未分配的队伍，则将这一个学校的所有未分配队伍分配进同一个考场。
	看是否存在n_unassigned!=0的学校未分配队伍数量相加小于考场容量，如果相加小于考场容量也分配进同一考场，
	如果遍历完了没有找到可以分配进同一考场的学校，那么遇到队伍数量相同的学校不再遍历，
	*/

	p = head;
	int k;
	struct school *p_temp;
	int n_assigned;//n_assigned当前考点分配的队伍数

	for (i = 0; i < N_school; i++) {
		n_assigned=0;
		if (p->n_unassigned != 0) {
			for (j = p->n_team - 1; j > p->n_team - 1 - p->n_unassigned; j--) {
				p->name_team[j].num = num_room;
			}
			
			n_assigned=p->n_unassigned;
			p->n_unassigned = 0;
			
			p_temp = p;//p_temp表示当前学校后面的学校

			for (k = i + 1; k < N_school; k++) {
				p_temp = p_temp->next;
				if (p_temp->n_unassigned != 0) {
					if ((p->n_unassigned + p_temp->n_unassigned <= Room_maxnum_team)
					        || ((p->n_unassigned + p_temp->n_unassigned <= Room_maxnum_team + 2) && Room_maxnum_team > 19)) {
						for (j = p_temp->n_team - 1; j > p_temp->n_team - 1 - p_temp->n_unassigned; j--) 
							p_temp->name_team[j].num = num_room;
						n_assigned+=p_temp->n_unassigned;
						p_temp->n_unassigned = 0;
					}
				}
				if (n_assigned >= Room_maxnum_team)break;
			}
			
			num_room++;
			
		}
		p = p->next;
	}



	//输出给每个队伍的考点

	p = head;

	for (i = 0; i < N_school; i++) {
		for (j = 0; j < p->n_team; j++) {
			printf("%s   %s   线上考点 %03d\n", p->school, p->name_team[j].team, p->name_team[j].num);
		}
		p = p->next;
	}
	
	printf("输出完毕！！！！");
	return 0;
}


