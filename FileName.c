#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
struct Account
{
	char* username;
	char* password;
	float* balance;
	int* securePassword;
};

struct Account acc_t;

void memoryAlloc(struct Account** acc)
{
	*acc = (struct Account*)malloc(sizeof(struct Account));
	(*acc)->username = (char*)malloc(10 * sizeof(char));
	(*acc)->password = (char*)malloc(20 * sizeof(char));
	(*acc)->balance = (float*)malloc(sizeof(float));
	(*acc)->securePassword = (int*)malloc(sizeof(int));
}

void memoryFree(struct Account* acc)
{
	if (acc != NULL)
	{
		free(acc->username);
		free(acc->password);
		free(acc->balance);
		free(acc->securePassword);
		free(acc);
	}
}

int countDigits(int num)
{
	int count = 0;
	while (num != 0)
	{
		num = num / 10;
		count++;
	}
	return count;
}

void displayMenu(void)
{
	printf("1. 创建账户\n");
	printf("2. 登录\n");
	printf("3. 查看余额\n");
	printf("4. 存款\n");
	printf("5. 取款\n");
	printf("6. 修改密码\n");
	printf("7. 修改支付密码\n");
	printf("8. 退出登录\n");
	printf("9. 注销账户\n");
	printf("10. 退出\n");
	printf("请选择一个数字：");
}

void createAccount(struct Account* acc)
{
	printf("请输入用户名，只能包括字母和数字：");
	bool boolUsername = true;
	do
	{
		scanf("%s", acc->username);
		if (strlen(acc->username) > 10)
		{
			printf("用户名最多10位，请重新输入：");
		}
		else
		{
			if (!isalpha(*acc->username) && !isdigit(*acc->username))
			{
				printf("输入包含非法字符，请重新输入：");
			}
			else
			{
				boolUsername = false;
			}
		}
	} while (boolUsername);
	bool boolPassword = true;
	printf("请输入密码，不超过20位，需要包括数字、字母和特殊字符：");
	do
	{
		scanf("%s", acc->password);
		if (strlen(acc->password) > 20)
		{
			printf("密码最多20位，请重新输入：");
		}
		else
		{
			if (!isalpha(*acc->password) && !isdigit(*acc->password) && !ispunct(*acc->password))
			{
				printf("输入包含非法字符，请重新输入：");
			}
			else
			{
				boolPassword = false;
			}
		}
	} while (boolPassword);
	printf("请输入初始金额：");
	scanf("%f", acc->balance);
	bool boolSecurePassword = true;
	printf("请设置支付密码：");
	do
	{
		scanf("%d", acc->securePassword);
		if (countDigits(*acc->securePassword) != 6)
		{
			printf("不合法，支付密码应该有六位，请重新输入：");
		}
		else
		{
			boolSecurePassword = false;
		}
	} while (boolSecurePassword);
	FILE* account = fopen("account.txt", "r+");
	if (account != NULL)
	{
		fprintf(account, "%s %s %.2f %d\n", acc->username, acc->password, *acc->balance, *acc->securePassword);
	}
	fclose(account);
	system("cls");
}

int loginAccount(struct Account* acc)
{
	char username[10];
	char password[20];
	FILE* account = fopen("account.txt", "r");
	if (account != NULL)
	{
		;
	}
	while (true)
	{
		printf("请输入用户名：");
		scanf("%s", username);
		while (fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword) != EOF)
		{
			if (strcmp(acc->username, username) == 0)
			{
				printf("请输入密码：");
				scanf("%s", password);
				if (strcmp(acc->password, password) == 0)
				{
					printf("登录成功\n");
					acc_t = *acc;
					return 1;
				}
			}
			else
			{
				printf("账户不存在，请先注册\n");
				return 0;
			}
		}
	}
	fclose(account);
}

void viewBalance()
{
	printf("你的金额为：%.2f\n", *acc_t.balance);
}

void deposit()
{
	float balance;
	printf("请输入存款金额：");
	scanf("%f", &balance);
	*acc_t.balance = *acc_t.balance + balance;
}

void withdrawal()
{
	float balance;
	int securePassword;
	bool boolSecurePassword = true;
	printf("请输入支付密码：");
	do
	{
		scanf("%d", &securePassword);
		if (securePassword != *acc_t.securePassword)
		{
			printf("密码错误，请重新输入\n");
		}
		else
		{
			boolSecurePassword = false;
		}
	} while (boolSecurePassword);
	bool boolBalance = true;
	printf("请输入取款金额：");
	do
	{
		scanf("%f", &balance);
		if (*acc_t.balance - balance < 0)
		{
			printf("余额不足，请重新输入：");
		}
		else
		{
			*acc_t.balance = *acc_t.balance - balance;
			boolBalance = false;
		}
	} while (boolBalance);
}

void changePassword()
{
	char password[20];
	bool boolPassword = true;
	printf("请输入修改的密码：");
	do
	{
		scanf("%s", password);
		if (strcmp(acc_t.password, password) == 0)
		{
			printf("密码已经存在，请重新输入：");
		}
		else
		{
			*acc_t.password = *password;
			boolPassword = false;
		}
	} while (boolPassword);
}

void changeSecurePassword()
{
	int securePassword[6];
	bool boolSecurePassword = true;
	printf("请输入修改的支付密码：");
	do
	{
		scanf("%d", securePassword);
		if (acc_t.securePassword == securePassword)
		{
			printf("密码已经存在，请重新输入\n");
		}
		else
		{
			*acc_t.securePassword = *securePassword;
			boolSecurePassword = false;
		}
	} while (boolSecurePassword);
}

void logoutAccount(void)
{

}

void removeAccount(void)
{

}

int main(void)
{
	struct  Account* account;
	int choice;
	memoryAlloc(&account);
	do
	{
		displayMenu();
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			system("cls");
			createAccount(account);
			break;
		case 2:
			system("cls");
			loginAccount(account);
			break;
		case 3:
			if (loginAccount(account) == 1)
			{
				viewBalance(account);
			}
			else
			{
				printf("没有登录，请先登录\n");
			}
			break;
		case 4:
			deposit(account);
			break;
		case 5:
			withdrawal(account);
			break;
		case 6:
			changePassword(account);
			break;
		case 7:
			changeSecurePassword(account);
			break;
		case 8:
			logoutAccount();
			break;
		case 9:
			removeAccount();
			break;
		case 10:
			printf("再见！\n");
			break;
		default:
			printf("请重新输入：\n");
		}
	} while (choice != 10);
	memoryFree(account);
	return 0;
}