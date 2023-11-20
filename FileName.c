#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>
#include<ctype.h>
bool _bool = true;
struct Account
{
	char* username;
	char* password;
	float* balance;
	int* securePassword;
};

void memoryAlloc(struct Account** acc)
{
	*acc = (struct Account*)malloc(sizeof(struct Account));
	(*acc)->username = (char*)malloc(10 * sizeof(char));
	(*acc)->password = (char*)malloc(20 * sizeof(char));
	(*acc)->balance = (float*)malloc(sizeof(float));
	(*acc)->securePassword = (int*)malloc(6);
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
				_bool = false;
			}
		}
	} while (_bool);
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
				_bool = false;
			}
		}
	} while (_bool);
	printf("请输入初始金额：");
	scanf("%f", acc->balance);
	printf("请设置支付密码：");
	do
	{
		scanf("%d", acc->securePassword);
		if (sizeof(acc->securePassword) != 6)
		{
			printf("不合法，支付密码应该有六位，请重新输入：");
		}
		else
		{
			_bool = false;
		}
	} while (_bool);
	FILE* account = fopen("account.txt", "w");
	if (account != NULL)
	{
		fprintf(account, "%s %s %.2f %d\n", acc->username, acc->password, *acc->balance, *acc->securePassword);
	}
	fclose(account);
}

void loginAccount(struct Account* acc)
{
	FILE* account = fopen("account.txt", "r");
	if (account != NULL)
	{
		fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword);
	}
	fclose(account);
	char username[10];
	char password[20];
	do
	{
		printf("请输入用户名：");
		scanf("%s", username);
		printf("请输入密码：");
		scanf("%s", password);
		if (strcmp(acc->username, username) != 0 || strcmp(acc->password, password) != 0)
		{
			printf("用户名或密码错误，请重新输入\n");
		}
		else
		{
			printf("成功登录\n");
			_bool = false;
		}
	} while (_bool);
}

void viewBalance(struct Account* acc)
{
	FILE* account = fopen("account.txt", "r");
	if (account != NULL)
	{
		fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword);
	}
	fclose(account);
	printf("你的金额为：%f\n", *acc->balance);
}

void deposit(struct Account* acc)
{
	float balance;
	FILE* account = fopen("account.txt", "w");
	if (account != NULL)
	{
		fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword);
	}
	printf("请输入存款金额：");
	scanf("%f", &balance);
	*acc->balance = *acc->balance + balance;
	fprintf(account, "%s %s %f %d\n", acc->username, acc->password, *acc->balance, *acc->securePassword);
	fclose(account);
}

void withdrawal(struct Account* acc)
{
	FILE* account = fopen("account.txt", "w");
	if (account != NULL)
	{
		fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword);
	}
	float balance;
	float t;
	int securePassword;
	printf("请输入支付密码：");
	do
	{
		scanf("%d", &securePassword);
		if (securePassword != *acc->securePassword)
		{
			printf("密码错误，请重新输入\n");
		}
		else
		{
			_bool = false;
		}
	} while (_bool);
	printf("请输入取款金额：");
	do
	{
		scanf("%f", &balance);
		if (*acc->balance - balance < 0)
		{
			printf("余额不足，请重新输入：");
		}
		else
		{
			t = *acc->balance - balance;
			_bool = false;
		}
	} while (_bool);
	fprintf(account, "%s %s %f %d", acc->username, acc->password, t, *acc->securePassword);
	fclose(account);
}

void changePassword(struct Account* acc)
{
	FILE* account = fopen("account.txt", "w");
	if (account != NULL)
	{
		fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword);
	}
	char password;
	printf("请输入修改的密码：");
	do
	{
		scanf("%s", &password);
		if (strcmp(acc->password, &password) == 0)
		{
			printf("密码已经存在，请重新输入\n");
		}
		else
		{
			*acc->password = password;
			_bool = false;
		}
	} while (_bool);
	fprintf(account, "%s %s %f %d\n", acc->username, acc->password, *acc->balance, *acc->securePassword);
	fclose(account);
}

void changeSecurePassword(struct Account* acc)
{
	int securePassword;
	FILE* account = fopen("account.txt", "w");
	if (account != NULL)
	{
		fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword);
	}
	printf("请输入修改的支付密码：");
	do
	{
		scanf("%d", &securePassword);
		if (*acc->securePassword == securePassword)
		{
			printf("密码已经存在，请重新输入\n");
		}
		else
		{
			*acc->securePassword = securePassword;
			_bool = false;
		}
	} while (_bool);
	fprintf(account, "%s %s %f %d\n", acc->username, acc->password, *acc->balance, *acc->securePassword);
	fclose(account);
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
			createAccount(account);
			break;
		case 2:
			loginAccount(account);
			break;
		case 3:
			viewBalance(account);
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