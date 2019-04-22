#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>

struct User        /*用户*/
{
	int bookcount;
	char username[10];
	char password[10];
	char booklist[10][20];
	struct User *next;
};

struct Manager       /*管理员*/
{
	char managername[10];
	char password[10];
	struct Manager *next;
};

struct Book
{
	int id;
	char bookname[10];
	char author[10];
	int year;
	int month;
	int day;
	int count;
	struct Book *next;
};

struct User *userhead = NULL, *userend = NULL;
struct Manager *managerhead = NULL, *managerend = NULL;
struct Book *bookhead = NULL, *bookend = NULL;
int usercount, managercount, bookcount;
void MainMenu();
void readuserfile();
void readmanagerfile();
void readbookfile();
void userlogin();
void userfunction(struct User *p);
void userregister();
void managerlogin();
void managerregister();
void managerfunction(struct Manager *p);
void borrowbook(struct User *userp);
void findbook(struct User *userp);
void returnbook(struct User *userp);
void addbook();
void deletebook();
void searchbook();
void changebook();
void searchuser();
void writefile(int flag);
void changuserbook(char *userbookname,char *bookname);
void deleteuserbook(char *bookname);

int main()
{
	FILE *fp;
    userhead = (struct User *)malloc(sizeof(struct User));
	managerhead = (struct Manager *)malloc(sizeof(struct Manager));
	bookhead = (struct Book *)malloc(sizeof(struct Book));

    fp = fopen("user", "a+");
	usercount = fread(userhead, sizeof(struct User), 1, fp);
	fclose(fp);
	userend = userhead;
	readuserfile();

    fp = fopen("manager", "a+");
    managercount = fread(managerhead, sizeof(struct Manager), 1, fp);
	fclose(fp);
	managerend = managerhead;
	readmanagerfile();

    fp = fopen("book", "a+");
	bookcount = fread(bookhead, sizeof(struct Book), 1, fp);
	fclose(fp);
	bookend = bookhead;
	readbookfile();

    MainMenu();
	return 0;
}

void MainMenu()
{
    int cmd;
    int end = 0;
    while (1)
    {
        system("cls");
        printf("\n\n\n\n\n\n\n");
        printf("**************************************************************");
        printf("\n欢迎进入图书信息管理系统\n");
        printf("1.用户登陆\n");
        printf("2.管理员登陆\n");
        printf("3.用户注册\n");
        printf("4.管理员注册\n");
        printf("0.退出\n");
        printf("**************************************************************");
        printf("\n\n\n\n\n\n\n");
        printf("输入你需要进行的操作: ");
        scanf("%d", &cmd);
        getchar();
        switch (cmd)
        {
            case 1:
                    userlogin();
            break;

            case 2:
                    managerlogin();
            break;

            case 3:
                    userregister();
            break;

            case 4:
                    managerregister();
            break;

            case 0:
                    end = 1;
            break;

            default:
                    printf("输入错误!\n");
        }

        if (end == 1)
            break;
     }
}

void userlogin()
{
    struct User *p;
    char username[10];
    char password[20];
    int exist = 0;

    FILE *fp;
    p = userhead;
    system("cls");
    printf("\n\n\n\n\n\n\n");
    printf("**************************************************************");
    printf("\n输入您的用户信息: \n");
    printf("username: ");
    gets(username);
    printf("password: ");
    gets(password);
    printf("**************************************************************");
    printf("\n\n\n\n\n\n\n");

    while (p != NULL)
    {
        if (strcmp(p->username, username) == 0)
            {
                exist = 1;
        while(strcmp(p->password, password) != 0)
            {
                printf("密码错误!请重新输入\n");
                printf("password: ");
                gets(password);
            }
            printf("用户%s登陆成功!3秒后自动跳转至用户界面...", p->username);
            Sleep(3000);
            userfunction(p);
            }
            p = p->next;
    }
    if (!exist)
        {
            printf("不存在此用户名,请先前往注册...\n");
            printf("3秒后自动跳转至主界面...");
        Sleep(3000);
        }
}

void userfunction(struct User *p)
{
    int cmd;
    int end = 0;
    while (1)
        {
            system("cls");
            printf("\n\n\n\n\n\n\n");
            printf("**************************************************************");
            printf("\n图书馆用户功能系统\n");
            printf("1.借阅书籍\n");
            printf("2.归还书籍\n");
            printf("3.已借阅书籍信息\n");
            printf("0.退出\n");
            printf("**************************************************************");
            printf("\n\n\n\n\n\n\n");
            printf("输入您需要进行的操作: ");
            scanf("%d", &cmd);
            switch (cmd)
            {
                case 1:
                    borrowbook(p);
                break;
                case 2:
                    returnbook(p);
                break;
                case 3:
                    findbook(p);
                break;
                case 0:
                    end = 1;
                break;
                default:
                    printf("错误的输入!\n");
            }
            if (end == 1)
                break;
        }
}

int isexist(char *name, int flag)
{
    if (flag == 0)
    {
        struct User *p;
        p = userhead;
        while (p != NULL && usercount != 0)
        {
            if (strcmp(name, p->username) == 0)
                return 1;
            p = p->next;
        }
    }
    else if (flag == 1)
    {
        struct Manager *p;
        p = managerhead;
        while (p != NULL)
        {
            if (strcmp(name, p->managername) == 0)
                return 1;
            p = p->next;
        }
    }
    else if (flag == 2)
    {

		struct Book *p;
		p = bookhead;
		while (p != NULL)
        {
            if (strcmp(name, p->bookname) == 0)
                return 1;
            p = p->next;
        }
    }
    return 0;
}

void userregister()
{

	struct User *p;
	char username[10];
	char password[20];

	FILE *fp;
    system("cls");
    if (usercount == 0)            //第一次添加
        {
            printf("username: ");
            while (gets(username))
            {
                if (strlen(username) < 10 && !isexist(username, 0))
                break;
                else if (isexist(username, 0))
                    {
                        printf("该用户名已经存在!\n");
                        printf("username: ");
                    }
                else
                {
                printf("This username is too long!\n");

				printf("username: ");
				}
            }
            strcpy(userhead->username, username);
            printf("password: ");
            while (gets(password))
            {
                if (strlen(password) < 20)
                    break;
                else
                    printf("This password is too long!");
            }
            strcpy(userhead->password, password);
            userhead->bookcount = 0;
            userend = userhead;
            userend->next = NULL;
        }
    else
    {
        p = (struct User *)malloc(sizeof(struct User));
        printf("username: ");
        while (gets(username))
        {
            if (strlen(username) < 10 && !isexist(username, 0))
                break;
            else if (isexist(username, 0))
            {
                printf("该用户名已经存在,请直接前往登陆······\n");
                printf("username: ");
            }
            else
            {
                printf("This username is too long!\n");
                printf("username: ");
            }
        }
        strcpy(p->username, username);
        printf("password: ");
        while (gets(password))
        {
            if (strlen(password) < 20)
                break;
            else
                printf("This password is too long!");
        }
        strcpy(p->password, password);
        p->bookcount = 0;
        userend->next = p;
        userend = p;
        userend->next = NULL;
    }
    usercount++;
    fp = fopen("user", "w");
    p = userhead;
    while (p != NULL)
    {
        fwrite(p, sizeof(struct User), 1, fp);
        p = p->next;
    }
    fclose(fp);
    system("cls");
    printf("用户%s创建成功\n", userend->username);
    printf("3秒后自动返回主界面....\n");
    Sleep(3000);
}

void managerlogin()
{
    struct Manager *p;
    char managername[100];
    char password[100];

    p = managerhead;
    system("cls");
    printf("\n\n\n\n\n\n\n");
    printf("**************************************************************");
    printf("\n输入您的管理员信息: \n");
    printf("managername: ");
    gets(managername);
    printf("password: ");
    gets(password);
    printf("**************************************************************");
    printf("\n\n\n\n\n\n\n");
    while (p != NULL)
    {
        if (strcmp(p->managername, managername) == 0)
        {
            while (strcmp(p->password, password) != 0)
            {
                printf("密码错误!请重新输入\n");
                printf("password: ");
                gets(password);
            }
            printf("管理员%s登陆成功!3秒后自动跳转至管理员界面...", p->managername);
            Sleep(3000);
            managerfunction(p);
        }
        p = p->next;
    }
}

void managerfunction(struct Manager *p)
{
    int cmd;
    int end = 0;
    while (1)
	{
		system("cls");
		printf("\n\n\n\n\n\n\n");
		printf("**************************************************************");
		printf("\n图书馆管理员功能系统\n");
		printf("1.添加书籍信息\n");
		printf("2.删除书籍信息\n");
		printf("3.修改书籍信息\n");
		printf("4.查询书籍信息\n");
		printf("5.查询用户信息\n");
		printf("0.退出\n");
		printf("**************************************************************");
		printf("\n\n\n\n\n\n\n");
		printf("输入您需要进行的操作: ");
		scanf("%d", &cmd);
		getchar();
		switch (cmd)
		{
		    case 1:
		        addbook();
			break;
			case 2:
			    deletebook();
			break;
		    case 3:
			    changebook();
			break;
		    case 4:
			    searchbook();
			break;
		    case 5:
			    searchuser();
			break;
		    case 0:
			    end = 1;
			break;
		    default:
			printf("错误的输入!\n");
		}
		if (end == 1)
			break;
	}
}

void managerregister()
{
	struct Manager *p;
	char managername[100];
	char password[100];

	FILE *fp;
	system("cls");
	if (managercount == 0)            //第一次添加
	{
	    managerhead = (struct Manager *)malloc(sizeof(struct Manager));
		printf("managername: ");
		while (gets(managername))
		{
			if (strlen(managername) < 10)
				break;
			else
				printf("This managername is too long!");
		}
		strcpy(managerhead->managername, managername);
		printf("password: ");
		while (gets(password))
		{
			if (strlen(password) < 20)
				break;
			else
				printf("This password is too long!");
		}
		strcpy(managerhead->password, password);
		managerend = managerhead;
		managerend->next = NULL;
	}
	else
	{
		p = (struct Manager *)malloc(sizeof(struct Manager));
		printf("managername: ");
		while (gets(managername))
		{
			if (strlen(managername) < 10)
				break;
			else
				printf("This managername is too long!");
		}
		strcpy(p->managername, managername);
		printf("password: ");
		while (gets(password))
		{
			if (strlen(password) < 20)
				break;
			else
				printf("This password is too long!");
		}

		strcpy(p->password, password);
		managerend->next = p;
		managerend = p;
		managerend->next = NULL;
    }
    managercount++;
    fp = fopen("manager", "w");
    p = managerhead;
    while (p != NULL)
    {
        fwrite(p, sizeof(struct Manager), 1, fp);
        p = p->next;
    }
    fclose(fp);
    system("cls");
    printf("管理员%s创建成功\n", managerend->managername);
    printf("3秒后自动返回主界面....\n");
    Sleep(3000);
}

void borrowbook(struct User *userp)
{
    struct Book *p;
    char bookname[100];
    int exist = 0;

    system("cls");
    p = bookhead;
    while (p != NULL)
    {
        printf("bookname: %10s\tbookcount: %3d\n", p->bookname, p->count);
        p = p->next;
    }
    printf("输入您需要借阅的书籍名称: ");
    getchar();
    gets(bookname);
    p = bookhead;
    while (p != NULL)
    {
        if (strcmp(bookname, p->bookname) == 0)
        {
            if (p->count == 0)
            printf("此书无余量...");
            else
            {
                p->count -= 1;
                userp->bookcount++;
                strcpy(userp->booklist[userp->bookcount - 1], p->bookname);
                writefile(0);
                writefile(2);
                exist = 1;
                break;
            }
        }
        p = p->next;
    }
    if (exist == 0)
        printf("此书不存在!\n");
    else
        printf("%s借阅成功", p->bookname);
        printf("3秒后返回用户界面...");
    Sleep(3000);
}

void returnbook(struct User *userp)

{
    struct Book *p;
    char bookname[100];
    int count = userp->bookcount;
    int exist;
    int flag;

    system("cls");
    p = bookhead;
    while (count--)
    {
        printf("booklist:\n");
        printf("%s\n", userp->booklist[count]);
    }
    printf("\n\n\n\n\n");
    printf("**************************************************************");
    printf("\n输入你需要归还的书籍名称: ");
    getchar();
    while (gets(bookname))
    {
        exist = 0;
        for (int i = 0; i < userp->bookcount; i++)
        {
            if (strcmp(bookname, userp->booklist[i]) == 0)
            {
                flag = i;
                exist = 1;
                break;
            }
        }
        if (!exist)
        {
            printf("你借阅的书籍中不存在此书籍...\n");
            printf("请重新输入你需要归还的书籍名称: ");
        }
        else
            break;
    }
    printf("**************************************************************");
    while (p != NULL)
    {
        if (strcmp(p->bookname, bookname) == 0)
        {
            p->count++;
            break;
        }
        p = p->next;
    }
    for (int i = flag; i < userp->bookcount - 1; i++)
    {
        strcpy(userp->booklist[i], userp->booklist[i + 1]);
    }
    userp->bookcount--;
    writefile(0);
    writefile(2);
    printf("\n%s归还成功...", bookname);
    printf("3秒后自动跳转回用户界面...");
    Sleep(3000);
}

void findbook(struct User *userp)
{
    int count = userp->bookcount;
    int n;

    system("cls");
    printf("\n\n\n\n\n");
    printf("**************************************************************");
    printf("\nbooklist:\n");
    while (count--)
    {
        printf("%s\n", userp->booklist[count]);
    }
    printf("**************************************************************");
    printf("\n输入0返回用户界面: ");
    while (1)
    {
        scanf("%d", &n);
        if (n == 0)
            break;
        else
            printf("无效输入，请重新输入: ");
    }
}

void addbook()
{
    struct Book *p;
    char bookname[100];

    FILE *fp;
    system("cls");
    if (bookcount == 0)
    {
        printf("输入新增书籍的书名: ");
        strcpy(bookhead->bookname, bookname);
        printf("id: ");
        scanf("%d", &bookhead->id);
        printf("author: ");
        scanf("%s", bookhead->author);
        printf("year: ");
        scanf("%d", &bookhead->year);
        printf("month: ");
        scanf("%d", &bookhead->month);
        printf("day: ");
        scanf("%d", &bookhead->day);
        printf("bookcount: ");
        scanf("%d", &bookhead->count);
        bookend = bookhead;
        bookend->next = NULL;
    }
    else
    {
        printf("输入新增书籍的书名: ");
        int isexist = 0;
        p = bookhead;
        while (p != NULL)
        {
            if (strcmp(bookname, p->bookname) == 0)
            {
                isexist = 1;
                break;
            }
            p = p->next;
        }
        if (isexist == 1)
        {
            int count;
            printf("输入添加书籍数量: ");
            scanf("%d", &count);
            p->count += count;
        }
        else
        {
            p = (struct Book *)malloc(sizeof(struct Book));
            strcpy(p->bookname, bookname);
            printf("id: ");
            scanf("%d", &p->id);
            printf("author: ");
            scanf("%s", p->author);
            printf("year: ");
            scanf("%d", &p->year);
            printf("month: ");
            scanf("%d", &p->month);
            printf("day: ");
            scanf("%d", &p->day);
            printf("bookcount: ");
            scanf("%d", &p->count);
            bookend->next = p;
            bookend = p;
            bookend->next = NULL;
        }
    }
    bookcount++;
    p = bookhead;
    fp = fopen("book", "w");
    while (p != NULL)
    {
        fwrite(p, sizeof(struct Book), 1, fp);
        p = p->next;
    }
    fclose(fp);
    system("cls");
    printf("书籍添加成功\n");
    printf("3秒后自动返回管理员界面....\n");
    Sleep(3000);
}

void deletebook()
{
    struct Book *bookp;
    char bookname[100];
    int exist = 0;

    bookp = bookhead;
    printf("输入你需要删除的书籍名称: ");
    while (gets(bookname))
    {
        while (bookp != NULL)
        {
            if (strcmp(bookname, bookp->bookname) == 0)
            {
                exist = 1;
                break;
            }
            bookp = bookp->next;
        }
        if (!exist)
        {
            printf("不存在此书籍...\n");
            printf("请重新输入: ");
        }
        else
            break;
    }
    deleteuserbook(bookname);
    bookp = bookhead;
    if (strcmp(bookhead->bookname,bookname) == 0)
        bookhead = bookhead->next;
    while (bookp->next != NULL)
    {
        if (strcmp(bookp->next->bookname, bookname) == 0)
        {
            bookp->next = bookp->next->next;
            break;
        }
        bookp = bookp->next;
    }
    bookp = NULL;
    writefile(2);
}

void searchbook()
{
    struct Book *p;
    char bookname[100];
    int exist = 0;

    system("cls");
    printf("\n\n\n\n\n\n\n");
    printf("\n请输入您需要查询的书籍名称或书籍编号: ");
    gets(bookname);
    printf("\n\n\n\n\n\n\n");
    p = bookhead;
    while (p != NULL)
    {
        if (strcmp(p->bookname, bookname) == 0)
        {
            printf("**************************************************************");
            printf("\nbookname: ");
            puts(p->bookname);
            printf("bookid: %d\n", p->id);
            printf("author: %s\n", p->author);
            printf("year/month/day: %d/%d/%d\n", p->year, p->month, p->day);
            printf("bookcount: %d\n", p->count);
            printf("**************************************************************\n");
            exist = 1;
            break;
        }
        p = p->next;
    }
    if (exist == 0)
        printf("未找到相关书籍...\n");
        printf("输入0返回管理员界面: ");
    int n;
    while (1)
    {
        scanf("%d", &n);
    if (n == 0)
        break;
    }
}

void changebook()
{
    struct Book *bookp;
    char bookname[100];
    int exist = 0;
    int end = 0;
    int year, month, day, bookid, count;
    char author[100];

    bookp = bookhead;
    system("cls");
    printf("booklist:\n");
    while (bookp != NULL)
    {
        printf("bookname: %10s\n", bookp->bookname);
        bookp = bookp->next;
    }
    bookp = bookhead;
    printf("输入你需要修改的书籍名称(按@退出): ");
    while (gets(bookname))
    {
        if (strcmp(bookname, "@") == 0)
            break;
        while (bookp != NULL)
        {
            if (strcmp(bookp->bookname, bookname) == 0)
            {
                exist = 1;
                break;
            }
            bookp = bookp->next;
        }
        if (!exist)
            printf("不存在该书籍，请重新输入: ");
        else
            break;
    }
    if (!exist)
        return;
    else
    {
        int cmd;
        system("cls");
        printf("\n\n\n\n\n\n\n");
        printf("**************************************************************\n");
        printf("1.修改书籍名称\n");
        printf("2.修改书籍id号\n");
        printf("3.修改书籍日期\n");
        printf("4.修改书籍数量\n");
        printf("5.修改书籍作者\n");
        printf("0.退出\n");
        printf("**************************************************************");
        printf("\n\n\n\n\n\n\n");
        while (1)
        {
            printf("输入您需要进行的操作: ");
            scanf("%d", &cmd);
            switch (cmd)
            {
                case 1:
                {
                    printf("输入你修改后的书籍名称: ");
                    scanf("%s", bookname);
                    changuserbook(bookp->bookname,bookname);
                    strcpy(bookp->bookname, bookname);
                    break;
                }
                case 2:
                {
                    printf("输入你修改后的书籍ID: ");
                    scanf("%d", &bookid);
                    bookp->id = bookid;
                    break;
                }
                case 3:
                {
                    printf("输入你修改后的书籍日期: ");
                    scanf("%d%d%d", &year, &month, &day);
                    bookp->year = year;
                    bookp->month = month;
                    bookp->day = day;
                    break;
                }
                case 4:
                {
                    printf("输入你修改后的书籍数量: ");
                    scanf("%d", &count);
                    bookp->count = count;
                    break;
                }
                case 5:
                {
                    printf("输入你修改后的书籍作者: ");
                    scanf("%s", author);
                    strcpy(bookp->author, author);
                    break;
                }
                case 0:
                {
                    end = 1;
                    break;
                }
            }
            if (end == 1)
                break;
        }
    }
    writefile(2);
}

void searchuser()
{
    struct User *p;
    char username[100];
    int flag = 0;

    p = userhead;
    system("cls");
    printf("\n\n\n\n\n\n\n");
	printf("**************************************************************");
	printf("\n输入您需要查询的用户名!\n");
	printf("username: ");
	gets(username);
	while (p != NULL)
    {
        if (strcmp(p->username, username) == 0)
        {
            flag = 1;
            break;
        }
        p = p->next;
    }
    if (flag == 0)
    {
        system("cls");
        printf("不存在该用户信息\n");
        printf("3秒后自动跳转至管理员页面...\n");
        Sleep(3000);
        return;
    }
    else
    {
        int n;
        while (1)
        {
            system("cls");
            printf("\n\n\n\n\n\n\n");
            printf("**************************************************************");
            printf("\n\t用户%s的个人信息\n", p->username);
            printf("**************************************************************");
            printf("\nusername: ");
            puts(p->username);
            printf("password: ");
            puts(p->password);
            printf("bookcount: %d\n", p->bookcount);
            if (p->bookcount != 0)
            {
                printf("booklist: ");
                for (int i = 0; i < p->bookcount; i++)
                    puts(p->booklist[i]);
            }
            printf("输入0返回管理员页面!\n");
            scanf("%d", &n);
            if (n == 0)
                break;
        }
    }
}

void readuserfile()
{
    struct User *p;

    FILE *fp;
    p = userhead;
    fp = fopen("user", "ab+");
    while (fread(p,sizeof(struct User),1,fp))
    {
        if (p->next != NULL)
        {
            p = (struct User *)malloc(sizeof(struct User));
            userend->next = p;
            userend = p;
            userend->next = NULL;
        }
    }
}

void readmanagerfile()
{
    struct Manager *p;

    FILE *fp;
    p = managerhead;
    fp = fopen("manager", "ab+");
    while (fread(p, sizeof(struct Manager), 1, fp))
    {
        if (p->next != NULL)
        {
            p = (struct Manager *)malloc(sizeof(struct Manager));
            managerend->next = p;
            managerend = p;
            managerend->next = NULL;
        }
    }
}

void readbookfile()
{
    struct Book *p;

    FILE *fp;
    p = bookhead;
    fp = fopen("book", "ab+");
    while (fread(p, sizeof(struct Book), 1, fp))
    {
        if (p->next != NULL)
        {
            p = (struct Book *)malloc(sizeof(struct Book));
            bookend->next = p;
            bookend = p;
            bookend->next = NULL;
        }
    }
}

void writefile(int flag)
{
    FILE *fp;

    struct User *userp;
    struct Manager *managerp;
    struct Book *bookp;

    userp = userhead;
    managerp = managerhead;
    bookp = bookhead;

    if (flag == 0)
    {
        fp = fopen("user", "w");
        while (userp != NULL)
        {
            fwrite(userp, sizeof(struct User), 1, fp);
            userp = userp->next;
        }
    }
    else if (flag == 1)
    {
        fp = fopen("manager", "w");
        while (managerp != NULL)
        {
            fwrite(managerp, sizeof(struct Manager), 1, fp);
            managerp = managerp->next;
		}
	}
	else
	{
	    fp = fopen("book", "w");
		while (bookp != NULL)
		{
			fwrite(bookp, sizeof(struct Book), 1, fp);
			bookp = bookp->next;
		}
	}
	fclose(fp);
}

void changuserbook(char *userbookname,char *bookname)
{
	struct User *userp;

	userp = userhead;
	while (userp != NULL)
	{
		for (int i = 0; i < userp->bookcount; i++)
		{
			if (strcmp(userp->booklist[i], userbookname) == 0)
			{
				strcpy(userp->booklist[i], bookname);
			}
		}
		userp = userp->next;
	}
	writefile(0);
}

void deleteuserbook(char *bookname)
{
	struct User *userp;
	int flag = -1;

	userp = userhead;
	while (userp != NULL)
	{
		flag = -1;
		for (int i = 0; i < userp->bookcount; i++)
		{
			if (strcmp(userp->booklist[i], bookname) == 0)
			{
                flag = i;
				break;
			}
		}
		if (flag != -1)
		{
			for (int i = flag; i < userp->bookcount - 1; i++)
			{
				strcpy(userp->booklist[i], userp->booklist[i + 1]);
			}
			userp->bookcount--;
		}
		userp = userp->next;
	}
	writefile(0);
}




















