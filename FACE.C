#include<stdio.h>
#include<conio.h>
#include<alloc.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#include<math.h>

struct userinfo
{
	char username[30];
	char pass[20];
	struct userinfo *down;
	struct profile *p_head;      //Pointer to profile
	struct message *m_head;      //Pointer to message head
	struct frnd *f_head;         //Pointer to friend head
};

struct frnd
{
	char username[30];
	char fname[20];
	char lname[20];
	struct frnd *next;
};

struct profile
{
	char username[30];
	char fname[20];
	char lname[20];
	int dob[3];
	char gender[7];
	char city[20];
	char country[20];
	char status[15];
	char edu[30];
	char hobbies[80];
};

struct msg
{
	char to[20];
	char from[20];
	char msg[200];
	struct msg *next;
};

struct message
{
	struct msg *inbox;  //Pointer to a message of the type inbox(I) (Pvt msg)
	struct msg *wall;   //Pointer to a message of the type wall(W) (Public Post)
};

struct userinfo *u_head;
struct userinfo *cur_user;

void edit_profile();
void display_editprofile();
void logout();
void write_file_editp();
void main_menu();
void add_friend();
void login_menu();
void view_friend();
void msg_i();
void msg_self_vw();
void msg_other_vw();
void msg_vi();
void loading();
void home_screen();
void exit_screen();
void acc_create();

/*************Function to check duplicacy of username***************/
int duplicate(struct userinfo*temp)
{
	int flag=0;
	struct userinfo*temp2;

	temp2=u_head;
	do
	{
		if(strcmp(temp2->username,temp->username)==0)
		{
			flag=1;
		}
		temp2=temp2->down;
	}while(temp2!=temp);
	return(flag);
}

/*********************Function To Calculate Age**********************/
int calc_age(int d,int m, int y)
{
	time_t t=time(NULL);
	struct tm *aTime =localtime(&t);
	int age;
	int day = aTime->tm_mday;
	int month = aTime->tm_mon+1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime->tm_year+1900; // Year is # years since 1900

	age=(year-y);
	if(month<m || (month==m && day<d))
	{
		age=age-1;
	}
	return(age);
}

/*************Function to display home screen*************/
void home_screen()
{
	int x, y, color, angle = 0;
	struct arccoordstype a, b;
	cleardevice();

	while(angle<=360)
	{
		setcolor(BLACK);
		arc(getmaxx()/2,getmaxy()/2,angle,angle+2,100);
		setcolor(RED);
		getarccoords(&a);
		circle(a.xstart,a.ystart,35);
		setcolor(BLACK);
		arc(getmaxx()/2,getmaxy()/2,angle,angle+2,150);
		getarccoords(&a);
		setcolor(GREEN);
		circle(a.xstart,a.ystart,35);
		angle = angle+5;
		delay(100);
	}
	setcolor(14);
	settextstyle(3,0,3);
	outtextxy(270,195,"WELCOME");
	outtextxy(290,215," TO");
	outtextxy(260,235,"FRIENDSTER");
	delay(3000);
}

/********Function to extract already created users from file********/
void create_user_file()
{
	FILE *fp1,*fp2,*fp3,*fp4,*fp5;
	int x=0,i;
	struct userinfo *u_node,*temp,*temp1;
	struct profile *p_node;
	struct frnd *f_node,*temp2;
	struct message *ptr;
	struct msg *m_node,*in,*w;
	char str1[30],str2[30];
	char ch;
	char str_m[200];
	clrscr();
	fp1=fopen("userinfo.txt","r");
	fp2=fopen("profile.txt","r");
	fp3=fopen("friend.txt","r");
	fp4=fopen("inbox.txt","r");
	fp5=fopen("wall.txt","r");

	//For extracting from userinfo.txt,profile.txt
	if(feof(fp1))
	{
		u_head=NULL;
	}
	else
	{
		while(!feof(fp1))
		{
			x++;
			if(x==1)
			{
				u_node=(struct userinfo*)malloc(sizeof(struct userinfo));
				u_node->down=NULL;
				u_node->f_head=NULL;
				u_node->m_head=(struct message*)malloc(sizeof(struct message));
				u_node->m_head->wall=NULL;
				u_node->m_head->inbox=NULL;
				u_head=u_node;
				temp=u_node;
				p_node=(struct profile*)malloc(sizeof(struct profile));
				temp->p_head=p_node;

				fscanf(fp1,"%s %s\n",&temp->username,&temp->pass);
				fscanf(fp2,"%s %s %s %d-%d-%d %s %s %s %s\n%s\n%s\n\n",&p_node->username,&p_node->fname,&p_node->lname,&p_node->dob[0],&p_node->dob[1],&p_node->dob[2],&p_node->gender,&p_node->city,&p_node->country,&p_node->status,&p_node->edu,&p_node->hobbies);
			}
			else
			{
				temp->down=(struct userinfo*)malloc(sizeof(struct userinfo));
				temp=temp->down;
				temp->down=NULL;
				temp->f_head=NULL;
				temp->m_head=(struct message*)malloc(sizeof(struct message));
				temp->m_head->wall=NULL;
				temp->m_head->inbox=NULL;
				p_node=(struct profile*)malloc(sizeof(struct profile));
				temp->p_head=p_node;

				fscanf(fp1,"%s %s\n",&temp->username,&temp->pass);
				fscanf(fp2,"%s %s %s %d-%d-%d %s %s %s %s\n%s\n%s\n\n",&p_node->username,&p_node->fname,&p_node->lname,&p_node->dob[0],&p_node->dob[1],&p_node->dob[2],&p_node->gender,&p_node->city,&p_node->country,&p_node->status,&p_node->edu,&p_node->hobbies);
			}
		}
	}

	//For extracting from Friends.txt
	if(feof(fp3))
	{
	}
	else
	{
		while(!feof(fp3))
		{
			temp=u_head;
			temp1=u_head;
			strcpy(str1,"");
			strcpy(str2,"");
			fscanf(fp3,"%s %s ",str1,str2);
			while(strcmp(temp->username,str1)!=0)
			{
				temp=temp->down;
			}
			while(strcmp(temp1->username,str2)!=0)
			{
				temp1=temp1->down;
			}
			if(temp->f_head==NULL)
			{
				f_node=(struct frnd*)malloc(sizeof(struct frnd));
				f_node->next=NULL;
				temp->f_head=f_node;
				temp2=f_node;
			}
			else
			{
				temp2=temp->f_head;
				while(temp2->next!=NULL)
				{
					temp2=temp2->next;
				}
				temp2->next=(struct frnd*)malloc(sizeof(struct frnd));
				temp2=temp2->next;
				temp2->next=NULL;
			}
			strcpy(temp2->username,temp1->username);
			strcpy(temp2->fname,temp1->p_head->fname);
			strcpy(temp2->lname,temp1->p_head->lname);
		}
	}

	//For extracting from inbox.txt
	if(feof(fp4))
	{
	}
	else
	{
		while(!feof(fp4))
		{
			temp=u_head;
			strcpy(str_m,"");
			m_node=(struct msg*)malloc(sizeof(struct msg));
			m_node->next=NULL;
			fscanf(fp4,"%s ",&m_node->to);
			ch=fgetc(fp4);
			i=0;
			while(ch!='$')
			{
				str_m[i]=ch;
				i++;
				ch=fgetc(fp4);
			}
			str_m[i]='\0';
			strcpy(m_node->msg,str_m);
			fscanf(fp4,"%s\n",&m_node->from);
			while(strcmp(temp->username,m_node->to)!=0 && temp!=NULL)
			{
				temp=temp->down;
			}
			ptr=temp->m_head;
			if(ptr->inbox==NULL)
			{
				ptr->inbox=m_node;
				in=ptr->inbox;
			}
			else
			{
				in=temp->m_head->inbox;
				while(in->next!=NULL)
				{
					in=in->next;
				}
				in->next=m_node;
				in=in->next;
			}
		}
	}

	// For extracting from wall.txt
	if(feof(fp5))
	{
	}
	else
	{
		while(!feof(fp5))
		{
			temp=u_head;
			strcpy(str_m,"");
			m_node=(struct msg*)malloc(sizeof(struct msg));
			m_node->next=NULL;
			fscanf(fp5,"%s ",&m_node->to);
			ch=fgetc(fp5);
			i=0;
			while(ch!='$')
			{
				str_m[i]=ch;
				i++;
				ch=fgetc(fp5);
			}
			str_m[i]='\0';
			strcpy(m_node->msg,str_m);
			fscanf(fp5,"%s\n",&m_node->from);
			while(strcmp(temp->username,m_node->to)!=0 && temp!=NULL)
			{
				temp=temp->down;
			}
			ptr=temp->m_head;
			if(ptr->wall==NULL)
			{
				ptr->wall=m_node;
				w=ptr->wall;
			}
			else
			{
				w=temp->m_head->wall;
				while(w->next!=NULL)
				{
					w=w->next;
				}
				w->next=m_node;
				w=w->next;
			}
		}
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
}

/*****************Function To Display The Main Menu*****************/
void main_menu()
{
	int a;
	clrscr();

	create_user_file();
	printf("\n\n\n\n\n\t\t\tWELCOME TO FRIENDSTER");
	printf("\n1=Login");
	printf("\n2=Create Account");
	printf("\n3=Exit");
	printf("\n\n\n\n\n\nEnter your choice:");
	fflush(stdin);
	scanf("%d",&a);
	if(a==1)
	{
		login();
	}
	else if(a==2)
	{
		create_user();
	}
	else if(a==3)
	{
		exit_screen();
	}
	else
	{
		printf("\nPlease enter a valid option");
		getch();
		main_menu();
	}
}

/********************Function to Create Account*********************/
void create_user()
{
	FILE *fp2;
	FILE *fp1;
	char g,r;
	int i=0;
	int age,flag;
	char ch;
	char str[20]="";
	struct userinfo *u_node,*temp;
	struct profile *p_node;
	clrscr();

	fp1=fopen("userinfo.txt","a");
	fp2=fopen("profile.txt","a");

	if(u_head==NULL)
	{
		u_node=(struct userinfo*)malloc(sizeof(struct userinfo));
		u_node->down=NULL;
		u_node->f_head=NULL;
		u_node->m_head=NULL;
		u_head=u_node;
		temp=u_node;
		p_node=(struct profile*)malloc(sizeof(struct profile));
		u_node->p_head=p_node;
	}
	else
	{	temp=u_head;
		while(temp->down!=NULL)
		{
			temp=temp->down;
		}
		temp->down=(struct userinfo*)malloc(sizeof(struct userinfo));
		temp=temp->down;
		temp->f_head=NULL;
		temp->m_head=NULL;
		p_node=(struct profile*)malloc(sizeof(struct profile));
		temp->p_head=p_node;
	}
	fflush(stdin);
	printf("\nNOTE:The first letter of your first name and last name\nshould be in upper case and subsequent part in lower case\n");
	printf("\nEnter your first name:");
	gets(p_node->fname);
	printf("\nEnter your last name:");
	gets(p_node->lname);

       user_name:	fflush(stdin);
	printf("\nEnter your desired username:");
	gets(temp->username);
	flag=duplicate(temp);    //To check if user with same username exists
	if(flag==0)
	{
		printf("\nUsername available");
	}
	else if(flag==1)
	{
		printf("\nSorry! Username already exists.\nPlease choose a different username.");
		goto user_name;
	}

      pmatch:	fflush(stdin);
	printf("\nEnter password");
	printf("\n(Please enter atleast 8 characters):");
	ch='a';
	i=0;
	while((int)ch!=13)
	{
		ch=getch();
		if(ch!=13&&ch!=8)
		{
			printf("*");
			str[i]=ch;
			i++;
		}
		if(ch==8&&i>0)
		{
			i--;
		}
	}
	str[i]='\0';
	strcpy(temp->pass,str);
	//Checking length of password
	if(strlen(temp->pass)<8)
	{
		printf("\nSorry you have entered less than 8 characters");
		goto pmatch;
	}
	printf("\nEnter password again to confirm:");
	i=0;
	ch='a';
	while((int)ch!=13)
	{
		ch=getch();
		if(ch!=13&&ch!=8)
		{
			printf("*");
			str[i]=ch;
			i++;
		}
		if(ch==8&&i>0)
		{
			i--;
		}
	}
	str[i]='\0';
	//Comparing passwords
	if(strcmp(temp->pass,str)==0)
	{
		printf("\t\t\nPasswords match" );
	}
	else
	{
		printf("\nPasswords dont match, Try again!");
		goto pmatch;
	}
	strcpy(p_node->username,temp->username);

	fflush(stdin);
	printf("\nEnter your Date of Birth:");
	printf("\nEnter Date:");
	scanf("%d",&p_node->dob[0]);
	printf("\nEnter month:");
	scanf("%d",&p_node->dob[1]);
	printf("\nEnter year:");
	scanf("%d",&p_node->dob[2]);

	//Checking eligibility to create account
	age=calc_age(p_node->dob[0],p_node->dob[1],p_node->dob[2]);
	if(age<14)
	{
		clrscr();
		printf("Sorry! you are not eligible to create the account\nMinimum age to create account is 14");
		getch();
		main_menu();
	}
	printf("\nAccount successfully created");
	printf("\nEnter other basic details");

      gender: fflush(stdin);
	printf("\nGender(M/m:MALE  F/f:FEMALE):");
	scanf("%c",&g);
	if(g=='m'||g=='M'||g=='f'||g=='F')
	{
	if(g=='m'||g=='M')
	{
		strcpy(p_node->gender,"Male");
	}
	else if(g=='f'||g=='F')
	{
		strcpy(p_node->gender,"Female");
	}

	}
	else
	{
		printf("\nEnter a valid entry please.");
		goto gender;
	}
	fflush(stdin);
	printf("\nEnter your City:");
	gets(p_node->city);
	printf("\nEnter your Country:");
	gets(p_node->country);

       status: fflush(stdin);
	printf("\nEnter your relationship status(S:Single E:Engaged M:Married):");
	scanf("%c",&r);

	if(r=='s'||r=='S')
	{
		strcpy(p_node->status,"Single");
	}
	else if(r=='e'||r=='E')
	{
		strcpy(p_node->status,"Engaged");
	}
	else if(r=='m'||r=='M')
	{
		strcpy(p_node->status,"Married");
	}
	else
	{
		printf("\nEnter a valid relationship status");
		goto status;
	}
	fflush(stdin);
	printf("\nEnter your Education");
	printf("\n(NOTE:DO NOT ENTER ANY SPACE WHILE INPUT USE UNDERSCORE(_) WHEREVER SPACE IS REQUIRED)\n:");
	gets(p_node->edu);
	if(strcmp(p_node->edu,"")==0)
	{
		strcpy(p_node->edu,"-");
	}
	printf("\nEnter your Hobbies:\nNOTE:USE COMMAS TO SEPERATE TWO HOBBIES(Do not enter spaces)\n:");
	gets(p_node->hobbies);
	if(strcmp(p_node->hobbies,"")==0)
	{
		strcpy(p_node->hobbies,"-");
	}
	acc_create();

	//To store username password
	fprintf(fp1,"%s %s\n",temp->username,temp->pass);
	//To store profile data
	fprintf(fp2,"%s %s %s %d-%d-%d %s %s %s %s\n%s\n%s\n\n",p_node->username,p_node->fname,p_node->lname,p_node->dob[0],p_node->dob[1],p_node->dob[2],p_node->gender,p_node->city,p_node->country,p_node->status,p_node->edu,p_node->hobbies);
	fclose(fp1);
	fclose(fp2);
	main_menu();
}

/*****************Screen after creating account************************/
void acc_create()
{
	int gd=DETECT,gm,i;
	initgraph(&gd,&gm,"../../tc/bgi");

	cleardevice();
	setcolor(12);
	for(i=0;i<460;i++)
	{
		circle(460,i,60);
	}
	setcolor(5);
	delay(500);
	for(i=0;i<180;i++)
	{
		circle(60,i,60);
	}
	setcolor(3);
	delay(500);
	for(i=0;i<120;i++)
	{
	circle(360,i,60);
	}
	setcolor(9);
	delay(500);
	for(i=0;i<440;i++)
	{
		circle(230,i,60);
	}
	setcolor(7);
	delay(500);
	for(i=0;i<20;i++)
	{
		circle(480,i,60);
	}
	setcolor(2);
	delay(500);
	for(i=0;i<20;i++)
	{
		circle(20,i,60);
	}
	setcolor(1);
	delay(500);
	for(i=0;i<320;i++)
	{
		circle(getmaxx()-10,i,60);
	}
	setcolor(15);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
	outtextxy((getmaxx()/2)-270,(getmaxy()/2)," ACCOUNT CREATED SUCCESSFULLY");
	delay(500);
	closegraph();
}

/******************Function To Login Into The Website****************/
void login()
{
	char u_id[30],pwd[20];
	struct userinfo *temp;
	int i;
	char ch;
	clrscr();

	printf("\nEnter your username:");
	scanf("%s",&u_id);
	printf("\nEnter your password:");
	i=0;
	ch='a';
	while((int)ch!=13)
	{
		ch=getch();
		if(ch!=13 && ch!=8)
		{
			printf("*");
			pwd[i]=ch;
			i++;
		}
		if(ch==8&&i>0)
		{
			i--;
		}
	}
	pwd[i]='\0';
	temp=u_head;
	while(temp!=NULL)
	{
		if(strcmp(temp->username,u_id)==0)
		{
			break;
		}
		else
		{
			temp=temp->down;
		}
	}
	if(temp==NULL)
	{
		printf("\nSorry!Account with this username does not exists");
		getch();
		main_menu();
	}
	else
	{
		if(strcmp(temp->pass,pwd)==0)
		{
			printf("\nUser id and password match.");
			delay(500);
			loading();
			cur_user=temp;
			clrscr();
			printf("\n\n\n\n\nWELCOME %s %s !!",temp->p_head->fname,temp->p_head->lname);
		}
		else
		{
			printf("\nSorry!Wrong Password!\nTry Again" );
			getch();
			login();
		}
	}
	getch();
	login_menu();
}

/*********Function to show loading page*****************/
void loading()
{
	int j=0,i;
	int gd=DETECT,gm;
	clrscr();
	initgraph(&gd,&gm,"../../tc/bgi");

	cleardevice();
	while(j<1)
	{
		settextstyle(1,0,1);
		setcolor(RED);
		outtextxy(190,250,"LOADING...PLEASE WAIT!!!");
		setcolor(BROWN);
		for(i=0;i<8;i++)
		{
			rectangle(200-i,190-i,400+i,220+i);
		}
		for(i=205;i<395;)
		{
			setcolor(14);
			bar(i,195,i+10,215);
			i=i+12;
			delay(300);
		}
		cleardevice();
		++j;
	}
	closegraph();
}

/*************Function To Display Menu After Logging In*************/
void login_menu()
{
	int option;
	clrscr();

	printf("\n\nChoose one of the various options available:");
	printf("\n\n1.View Profile\n2.Add friend\n3.Edit Profile\n4.View Friends and their Profile\n5.Send message\n6.View your Inbox\n7.View your Wall\n8.Logout");
	printf("\n\nSelect your option:");
	fflush(stdin);
	scanf("%d",&option);
	switch(option)
	{
		case 1: {
				clrscr();
				display(cur_user->p_head);
				getch();
				login_menu();
			}
			break;
		case 2:add_friend();
			break;
		case 3:display_editprofile();
			break;
		case 4:view_friend();
			break;
		case 5:msg_i();
			break;
		case 6:msg_vi();
			break;
		case 7:msg_self_vw();
			break;
		case 8:logout();
			break;
	}
}

/*********************Function To View Profile***********************/
void display(struct profile *p)
{
	clrscr();
	printf("\nUsername:%s\nFirst Name:%s\nLast Name%s\nDOB:%d-%d-%d",p->username,p->fname,p->lname,p->dob[0],p->dob[1],p->dob[2]);
	printf("\nGender:%s\nCity:%s\nCountry:%s",p->gender,p->city,p->country);
	printf("\nMariatal Status:%s\nEducation:%s\nHobbies:%s",p->status,p->edu,p->hobbies);
}

/*********************Function To Add Friends*********************/
void add_friend()
{
	char f_n_t[20],l_n_t[20];
	char friend_u[30];
	int count=0;
	int x=10;
	int y=30;
	int odd=0;
	FILE *fp3;
	struct userinfo *temp;
	struct frnd *f_node,*temp3;
	struct list
	{
		char username[20];
		struct list*next;
	};
	struct list *head,*temp2;
	int gd=DETECT, gm;

	initgraph(&gd, &gm, "..\\bgi");
	clrscr();
	clearviewport();
	head=(struct list*)malloc(sizeof(struct list));
	strcpy(head->username,"NULL");
	head->next=NULL;
	temp2=head;
	fflush(stdin);
	printf("\nEnter the first name of the friend you want to add:");
	gets(f_n_t);
	printf("\nEnter the last name of the friend you want to add:");
	fflush(stdin);
	gets(l_n_t);
	temp=u_head;
	clearviewport();
	while(temp!=NULL)
	{
		if(strcmp(temp->p_head->fname,f_n_t)==0)
		{
			if(strcmp(temp->p_head->lname,l_n_t)==0)
			{
				count++;
				temp2->next=(struct list*)malloc(sizeof(struct list));
				temp2=temp2->next;
				strcpy(temp2->username,temp->username);
				temp2->next=NULL;
			}
		}
		temp=temp->down;
	}
	temp2=head->next;
	temp=u_head;
	if(count==0)
	{
		printf("\nSorry no user with this name exists" );
	}
	else
	{
		while(temp2!=NULL)
		{
			while(temp!=NULL)
			{
				if(strcmp(temp->username,temp2->username)==0)
				{
					if(odd>0 && odd%2!=0)
					{
						x=x+getmaxx()/2;
					}
					else if(odd>0 && odd%2==0)
					{
						x=10;
						y=y+100;
					}
					display_p_af(temp->p_head, x, y);
					odd++;
					break;
				}
				temp=temp->down;
			}
			temp2=temp2->next;
		}

	fflush(stdin);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPlease select the friend you want to add\n(Enter the username of your friend):");
	gets(friend_u);

	if(cur_user->f_head==NULL)
	{
		f_node=(struct frnd*)malloc(sizeof(struct frnd));
		f_node->next=NULL;
		cur_user->f_head=f_node;
		temp3=f_node;
	}
	else
	{
		temp3=cur_user->f_head;
		while(temp3->next!=NULL)
		{
			temp3=temp3->next;
		}
		temp3->next=(struct frnd*)malloc(sizeof(struct frnd));
		temp3=temp3->next;
		temp3->next=NULL;
	}
	temp=u_head;
	while(strcmp(temp->username,friend_u)!=0)
	{
		temp=temp->down;
	}
	strcpy(temp3->username,friend_u);
	strcpy(temp3->fname,temp->p_head->fname);
	strcpy(temp3->lname,temp->p_head->lname);
	printf("Congratulations!You have successfully added %s %s as your friend",temp3->fname,temp3->lname);
	}
	fp3=fopen("friend.txt","a");
	fprintf(fp3,"%s %s ",cur_user->username,temp3->username);
	getch();
	clearviewport();
	fclose(fp3);
	closegraph();
	login_menu();
	getch();
}

/**********Function to view profile to add friends***************/
void display_p_af(struct profile *m, int x, int y)
{
	char string[20]="";
	char temp[20];

	itoa(m->dob[0], temp, 10);
	strcat(string, temp);
	strcat(string, "/");
	itoa(m->dob[1], temp, 10);
	strcat(string, temp);
	strcat(string, "/");
	itoa(m->dob[2], temp, 10);
	strcat(string, temp);
	settextstyle(2,0,0);
	rectangle(x, y, x+getmaxx()/2-20, y+80);
	outtextxy(x+20, y+10, "Name:");
	outtextxy(x+100, y+10, m->fname);
	outtextxy(x+20, y+20, "Username:");
	outtextxy(x+100, y+20, m->username);
	outtextxy(x+20, y+30, "City:");
	outtextxy(x+100, y+30, m->city);
	outtextxy(x+20, y+40, "Country:");
	outtextxy(x+100, y+40, m->country);
	outtextxy(x+20, y+50, "DOB:");
	outtextxy(x+100, y+50, string);
	outtextxy(x+20, y+60, "Education:");
	outtextxy(x+100, y+60, m->edu);
}

/******Function To Display Various Choices Available In Edit Profile*****/
void display_editprofile()
{
	int option;
	clrscr();
	printf("EDIT PROFILE\n\n");
	printf("1.Change password\n2.Edit your first name\n3.Edit your last name\n");
	printf("4.Change your city\n5.Change your country\n6.Change your relationship status\n");
	printf("7.Edit your education\n8.Edit your hobbies");
	printf("\nSelect the detail you want to edit(Enter the no):");
	scanf("%d",&option);
	edit_profile(option);
}

/********************Function To Edit Profile***********************/
void edit_profile(int option)
{
	int i;
	char ch,str[20],r,ho[80],edu_t[30];
	char cp[20]; 		       //To store current password
	char np[20];                   //To store new password
	struct userinfo*temp;
	struct profile*temp2;
	clrscr();

	temp=cur_user;
	temp2=cur_user->p_head;
	switch(option)
	{
		case 1:	{
				pchange:	fflush(stdin);
				printf("\nEnter your current password:");
				ch='a';
				i=0;
				while((int)ch!=13)
				{
					ch=getch();
					if(ch!=13&&ch!=8)
					{
						printf("*");
						str[i]=ch;
						i++;
					}
					if(ch==8&&i>0)
					{
						i--;
					}
				}
				str[i]='\0';
				strcpy(cp,str);
				if(strcmp(cp,temp->pass)==0)
				{
				}
				else
				{
					printf("\nSorry!You have entered a wrong password!\n");
					stm:	printf("\nPress T to Try again else press P to Go back to the previous menu:");
					scanf("%c",&ch);
					if(ch=='T'||ch=='t')
					{
						goto pchange;
					}
					else if(ch=='P'||ch=='p')
					{
						display_editprofile();
					}
					else
					{
						printf("\nInvalid Option");
						delay(600);
						goto stm;
					}
				}

			       pnew:	printf("\nEnter new password");
				printf("\n(Please enter atleast 8 characters):");
				ch='a';
				i=0;
				while((int)ch!=13)
				{
					ch=getch();
					if(ch!=13&&ch!=8)
					{
						printf("*");
						str[i]=ch;
						i++;
					}
					if(ch==8&&i>0)
					{
						i--;
					}
				}
				str[i]='\0';
				strcpy(np,str);
				//Checking length of password
				if(strlen(np)<8)
				{
					printf("\nSorry you have entered less than 8 characters");
					goto pnew;
				}
				printf("\nEnter the new password again to confirm:");
				i=0;
				ch='a';
				while((int)ch!=13)
				{
					ch=getch();
					if(ch!=13&&ch!=8)
					{
						printf("*");
						str[i]=ch;
						i++;
					}
					if(ch==8&&i>0)
					{
						i--;
					}
				}
				str[i]='\0';
				//Comparing passwords
				if(strcmp(np,str)==0)
				{
					printf("\t\t\nPasswords match" );
					strcpy(temp->pass,"");
					strcpy(temp->pass,np);
					printf("\nYou have successfully changed your password");
				}
				else
				{
					printf("\nPasswords dont match, Try again!");
					goto pchange;
				}
			}
			break;

		case 2: {
				fflush(stdin);
				strcpy(str,"");
				printf("\nNOTE:The first letter of your first name should be\nin upper case and subsequent part in lower case\n");
				printf("\nEnter your first name:");
				scanf("%s",&str);
				strcpy(temp2->fname,"");
				strcpy(temp2->fname,str);
				printf("\nYou have successfully updated your first name");
			}
			break;
		case 3: {
				fflush(stdin);
				strcpy(str,"");
				printf("\nNOTE:The first letter of your last name should be\nin upper case and subsequent part in lower case\n");
				printf("\nEnter your last name:");
				scanf("%s",&str);
				strcpy(temp2->lname,"");
				strcpy(temp2->lname,str);
				printf("\nYou have successfully updated your last name");
			}
			break;
		case 4: {
				fflush(stdin);
				strcpy(str,"");
				printf("\nEnter your currrent city:");
				scanf("%s",&str);
				strcpy(temp2->city,"");
				strcpy(temp2->city,str);
				printf("\nYou have successfully updated your city");
			}
			break;
		case 5:	{
				fflush(stdin);
				strcpy(str,"");
				printf("\nEnter your current country:");
				scanf("%s",&str);
				strcpy(temp2->country,"");
				strcpy(temp2->country,str);
				printf("\nYou have successfully updated your country");
			}
			break;
		case 6:	{
			     new_status:	fflush(stdin);
				printf("\nEnter your relationship status(S:Single E:Engaged M:Married):");
				scanf("%c",&r);
				if(r=='s'||r=='S')
				{
					strcpy(temp2->status,"");
					strcpy(temp2->status,"Single");
					printf("\nYou have successfully updated your relationship status");
				}
				else if(r=='e'||r=='E')
				{
					strcpy(temp2->status,"");
					strcpy(temp2->status,"Engaged");
					printf("\nYou have successfully updated your relationship status");
				}
				else if(r=='m'||r=='M')
				{
					strcpy(temp2->status,"");
					strcpy(temp2->status,"Married");
					printf("\nYou have successfully updated your relationship status");
				}
				else
				{
					printf("\nEnter a valid relationship status");
					goto new_status;
				}
			}
			break;
		case 7:	{
				fflush(stdin);
				printf("\nEnter your current education\n(NOTE:DO NOT ENTER ANY SPACE WHILE INPUT USE UNDERSCORE(_) WHEREVER SPACE IS REQUIRED)\n:");
				gets(edu_t);
				strcpy(temp2->edu,"");
				strcpy(temp2->edu,edu_t);
				printf("\nYou have successfully updated your education");
			}
			break;
		case 8: {
				fflush(stdin);
				printf("\nEnter your Hobbies:\nNOTE:USE COMMAS TO SEPERATE TWO HOBBIES (Do not enter spaces)\n:");
				gets(ho);
				strcpy(temp2->hobbies,"");
				strcpy(temp2->hobbies,ho);
				printf("You have successfully updated your hobbies");
			}
			break;
	}
	write_file_editp();
	getch();
	login_menu();
}

/**************Function To Write Into File After Updation of Profile***********/
void write_file_editp()
{
	FILE *fp1,*fp2;
	struct userinfo*temp;
	struct profile*temp2;

	fp1=fopen("userinfo.txt","w");
	fp2=fopen("profile.txt","w");
	temp=u_head;
	while(temp!=NULL)
	{
		temp2=temp->p_head;
		//To store userinfo
		fprintf(fp1,"%s %s\n",temp->username,temp->pass);
		//To store profile data
		fprintf(fp2,"%s %s %s %d-%d-%d %s %s %s %s\n%s\n%s\n\n",temp2->username,temp2->fname,temp2->lname,temp2->dob[0],temp2->dob[1],temp2->dob[2],temp2->gender,temp2->city,temp2->country,temp2->status,temp2->edu,temp2->hobbies);
		temp=temp->down;
	}
	fclose(fp1);
	fclose(fp2);
}

/********************Function To View Your Friends*******************/
void view_friend()
{
	struct frnd *temp;
	struct userinfo *temp2;
	int option;
	char ch;
	char str[30];
	clrscr();

	temp=cur_user->f_head;
	printf("USERNAME     NAME");
	while(temp!=NULL)
	{
		printf("\n%s %s %s",temp->username,temp->fname,temp->lname);
		temp=temp->next;
	}
	getch();
       stat:	printf("\nDo you want to view profile or wall of your friend?(Y:YES/N:NO):");
	fflush(stdin);
	scanf("%c",&ch);
	if(ch=='y'||ch=='Y')
	{
		printf("\nEnter the userid of your friend whose profile you want to view:");
		fflush(stdin);
		gets(str);
		temp2=u_head;
		while(strcmp(temp2->username,str)!=0)
		{
			temp2=temp2->down;
		}
	      menu:	clrscr();
		printf("\n1.View profile\n2.View wall\n3.Go back to the main menu");
		printf("\nSelect an option:");
		fflush(stdin);
		scanf("%d",&option);
		if(option==1)
		{
			display(temp2->p_head);
			getch();
			printf("\nPress enter to go back to the previous menu");
			getch();
			goto menu;
		}
		else if(option==2)
		{
			msg_other_vw(temp2->m_head->wall);
			printf("\nPress enter to go back to the previous menu");
			getch();
			goto menu;
		}
		else if(option==3)
		{
			login_menu();
		}
		else
		{
			printf("\nPlease enter a valid option");
			getch();
			goto menu;
		}
	}
	else if(ch=='n'|| ch=='N')
	{
		login_menu();
	}
	else
	{
		printf("\nEnter a valid option");
		getch();
		goto stat;
	}
}

/***************Function to view friend's wall***************/
void msg_other_vw(struct msg *temp)
{
	clrscr();
	while(temp!=NULL)
	{
		printf("\n%s\n%s\n",temp->from,temp->msg);
		temp=temp->next;
	}
	getch();
}

/***************Function to Input message from user***************/
void msg_i()
{
	char ch;
	char type;
	int i;
	struct frnd *temp;
	struct message *select;
	struct msg *node,*temp2;
	struct userinfo *temp1;
	FILE *fp4,*fp5;
	clrscr();
	fp4=fopen("inbox.txt","a");
	fp5=fopen("wall.txt","a");

	printf("\nEnter the userid of the person whom you wish to send the message");
	printf("\nIf you dont remember the id then please press 1 to view your friends and their userid else press any key to continue:");
	fflush(stdin);
	scanf("%c",&ch);
	if(ch=='1')
	{
		temp=cur_user->f_head;
		while(temp!=NULL)
		{
			printf("\n%s %s %s",temp->username,temp->fname,temp->lname);
			temp=temp->next;
		}
		getch();
	}
	else
	{
	}
	node=(struct msg*)malloc(sizeof(struct msg));
	node->next=NULL;
	printf("\nEnter the userid:");
	fflush(stdin);
	scanf("%s",node->to);
	printf("\nEnter the type of message i.e. (W) for Wall post and (I) for Inbox:");
	fflush(stdin);
	scanf("%c",&type);
	printf("\nEnter your message:\n");
	fflush(stdin);
	gets(node->msg);
	strcpy(node->from,cur_user->username);

	temp1=u_head;
	while(strcmp(temp1->username,node->to)!=0)
	{
		temp1=temp1->down;
	}

	select=temp1->m_head;
	if(type=='w'||type=='W')
	{
		if(select->wall==NULL)
		{
			select->wall=node;
			temp2=select->wall;
		}
		else
		{
			temp2=select->wall;
			while(temp2->next!=NULL)
			{
				temp2=temp2->next;
			}
			temp2->next=node;
			temp2=temp2->next;
		}
	}
	else if(type=='i'||type=='I')
	{
		if(select->inbox==NULL)
		{
			select->inbox=node;
			temp2=select->inbox;
		}
		else
		{
			temp2=select->inbox;
			while(temp2->next!=NULL)
			{
				temp2=temp2->next;
			}
			temp2->next=node;
			temp2=temp2->next;
		}
	}

	printf("Please wait! Sending your message");
	i=0;
	while(i!=10)
	{
		printf(".");
		delay(100);
		i++;
	}
	printf("\nYour message has been sent.");
	if(type=='i'||type=='I')   //To store inbox message
	{
		fprintf(fp4,"%s %s$%s\n",temp2->to,temp2->msg,temp2->from);
	}
	else if(type=='w'||type=='W')
	{
		fprintf(fp5,"%s %s$%s\n",temp2->to,temp2->msg,temp2->from);
	}
	fclose(fp4);
	fclose(fp5);
	getch();
	login_menu();
}

/**********Function to view private message in inbox**********/
void msg_vi()
{
	struct msg*temp;
	clrscr();

	temp=cur_user->m_head->inbox;
	while(temp!=NULL)
	{
		printf("\n%s\n%s\n",temp->from,temp->msg);
		temp=temp->next;
	}
	getch();
	login_menu();
}

/**********Function to view messages on your own wall**********/
void msg_self_vw()
{
	struct msg*temp;
	clrscr();
	temp=cur_user->m_head->wall;

	while(temp!=NULL)
	{
		printf("\n%s\n%s\n",temp->from,temp->msg);
		temp=temp->next;
	}
	getch();
	login_menu();
}

/**********************Function To Logout*************************/
void logout()
{
	char ch;
	int i=0;
	clrscr();

	printf("\nAre you sure you wish to logout(Y:Yes/N:No)?");
	fflush(stdin);
	scanf("%c",&ch);
	if(ch=='y'||ch=='Y')
	{
		printf("\nYou have successfully logged out from your account!\n\nThank you!");
		getch();
		free(u_head);
		free(cur_user);
		main_menu();
	}
	else if(ch=='n'|| ch=='N')
	{
		printf("\nYou chose to continue!");
		printf("\nDisplaying your profile,Please wait");
		printf("\n");
		while(i!=12)
		{
			printf(".");
			delay(400);
			i++;
		}
		login_menu();
	}
	else
	{
		printf("\nPlease enter a valid option");
		getch();
		logout();
	}
}

/**********Function to display exit screen**************/
void exit_screen()
{
	int i;
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"../../tc/bgi");

	clrscr();
	clearviewport();
	setcolor(24);
	for(i=0;i<460;i++)
	{
		cleardevice();
		circle(460,i,60);
	}
	setcolor(7);
	delay(500);
	for(i=0;i<180;i++)
	{
		cleardevice();
		circle(60,i,60);
	}
	setcolor(68);
	delay(500);
	for(i=0;i<120;i++)
	{
		cleardevice();
		circle(360,i,60);
	}
	setcolor(78);
	delay(500);
	for(i=0;i<440;i++)
	{
		cleardevice();
		circle(230,i,60);
	}
	setcolor(34);
	delay(500);
	for(i=0;i<20;i++)
	{
		cleardevice();
		circle(480,i,60);
	}
	setcolor(6);
	delay(500);
	for(i=0;i<20;i++)
	{
		cleardevice();
		circle(20,i,60);
	}
	setcolor(8);
	delay(500);
	for(i=0;i<320;i++)
	{
		cleardevice();
		circle(getmaxx()-10,i,60);
	}
	setcolor(9);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,6);
	outtextxy(70,(getmaxy()/2)," GOOD BYE....");
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
	outtextxy(70,(getmaxy()/2)+70," Hope To See U Again!!!");
	delay(500);
	setcolor(23);
	circle(460,460,60);
	delay(500);
	setcolor(24);
	circle(60,180,60);
	delay(500);
	setcolor(23);
	circle(360,120,60);
	delay(500);
	setcolor(12);
	circle(230,440,60);
	delay(500);
	setcolor(45);
	circle(480,20,60);
	delay(500);
	setcolor(23);
	circle(20,20,60);
	delay(500);
	setcolor(12);
	circle(getmaxx()-10,320,60);
	clrscr();
	clearviewport();
	closegraph();
	getch();
	exit(0);
}

/***************************Main**************************/
void main()
{
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"../../tc/bgi");

	home_screen();
	fflush(stdin);
	closegraph();

	main_menu();
	getch();
}
