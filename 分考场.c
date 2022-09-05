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

	//读取学校信息

	printf("请输入 学校 列(样例表格E列，不包含表头)：\n");
	for (i = 1; i <= N_school; i++) {
		printf("第%d所学校\n", i);
		gets(temp_school);
		p = (struct school *)malloc(sizeof(struct school));
		strcpy(p->school, temp_school);
		if (head == NULL)
			head = tail = p;
		else {
			tail->next = p;
			tail = p;
		}
	}

	//读取各学校队伍信息

	//读取各学校队伍总数
	p = head;

	printf("请输入 各校队伍总数 列(样例表格F列，不包含表头)：\n");

	for (i = 1; i <= N_school; i++) {
		printf("第%d所学校队伍总数\n", i);
		scanf("%d", &temp_n_team);
		getchar();
		p->n_unassigned = p->n_team = temp_n_team;
		p = p->next;
	}

	//读取各学校队名
	p = head;

	printf("请输入 队名 列(样例表格B列，不包含表头)：\n");
	for (i = 1; i <= N_school; i++) {//遍历每个学校
		for (j = 0; j < p->n_team; j++) {//遍历每个学校的每个队伍
			gets(p->name_team[j].team);//读入队名
			p->name_team[j].num = -1;//初始化该队伍的考场编号为-1
		}
		p = p->next;
	}

	//给每个队伍分考场

	num_room = 0;//考场编号
	p = head;
	int room_members[200]={0};//假定最多200考场，记录每个考场队伍数

	//假如存在某个学校的队伍数大于考场容量，先给这个学校分配考场，分配的队伍数量为考场容量的整数倍
	for (i = 0; i < N_school; i++) {//逐一查看每个学校
		j = 0;
		while (p->n_unassigned >= Room_maxnum_team) {
			//看当前学校未分配的队伍数是否大于等于考场容量，如果大于考场容量，则先填满一个考场

			for ( room_members[num_room] =0; room_members[num_room]< Room_maxnum_team; room_members[num_room]++) {
				p->name_team[j].num = num_room;
				j++;
			}
			printf("第%d个考场%d队。\n",num_room,room_members[num_room]);
			num_room++;//下一个考场
			p->n_unassigned -= Room_maxnum_team;
		}
		p = p->next;//下一个学校
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

	for (i = 0; i < N_school; i++) {//遍历所有学校
		if (p->n_unassigned != 0) {//当前学校有未分配的队伍
			//给当前学校未分配的队伍分配考点
			for (j = p->n_team - 1; j > p->n_team - 1 - p->n_unassigned; j--) {
				p->name_team[j].num = num_room;
			}

			room_members[num_room] = p->n_unassigned;//当前考点已分配的队伍数增加
			p->n_unassigned = 0;//当前学校未分配的队伍数归零

			p_temp = p;//p_temp表示当前学校后面的学校

			for (k = i + 1; k < N_school; k++) {
				p_temp = p_temp->next;
				if (p_temp->n_unassigned != 0) {
					if ((room_members[num_room] + p_temp->n_unassigned )<= (Room_maxnum_team + 1)) {
						for (j = p_temp->n_team - 1; j > p_temp->n_team - 1 - p_temp->n_unassigned; j--)
							p_temp->name_team[j].num = num_room;
						room_members[num_room]+= p_temp->n_unassigned;
						p_temp->n_unassigned = 0;
					}
				}
				if (room_members[num_room] >= Room_maxnum_team)
					break;
			}
			printf("第%d个考场%d队。\n",num_room,room_members[num_room]);
			num_room++;

		}

		p = p->next;

	}

	getchar();
	printf("回车输出给每个队伍的考点\n");
	//输出给每个队伍的考点

	p = head;

	for (i = 0; i < N_school; i++) {
		for (j = 0; j < p->n_team; j++) {
			printf("%s   %s   线上考点 %03d\n", p->school, p->name_team[j].team, p->name_team[j].num);
		}
		p_temp = p;
		p = p->next;
		free(p_temp);
	}

	getchar();

	printf("输出完毕！！！！");


	return 0;
}

