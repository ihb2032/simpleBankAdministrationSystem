#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
struct Account
{
	char* username;
	char* password;
	float* balance;
	int* securePassword;
};

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
	acc = (struct Account*)malloc(sizeof(struct Account));
	acc->username = (char*)malloc(10 * sizeof(char));
	acc->password = (char*)malloc(20 * sizeof(char));
	acc->balance = (float*)malloc(sizeof(float));
	acc->securePassword = (int*)malloc(6);
	printf("请输入用户名：");
	scanf("%s", acc->username);
	printf("请输入密码：");
	scanf("%s", acc->password);
	printf("请输入初始金额：");
	scanf("%f", acc->balance);
	printf("请设置支付密码：");
	scanf("%d", acc->securePassword);
	FILE* account = fopen("account.txt", "w");
	fprintf(account, "%s %s %f %d\n", acc->username, acc->password, *acc->balance, *acc->securePassword);
	fclose(account);
	free(acc);
	free(acc->username);
	free(acc->password);
	free(acc->balance);
	free(acc->securePassword);
}

void loginAccount(void)
{

}

void viewBalance(void)
{

}

void deposit(void)
{

}

void withdrawal(void)
{

}

void changePassword(void)
{

}

void logoutAccount(void)
{

}

int main(void)
{
	struct  Account account;
	int choice;
	do
	{
		displayMenu();
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			createAccount(&account);
			break;
		case 2:
			loginAccount();
			break;
		case 3:
			viewBalance();
			break;
		case 4:
			deposit();
			break;
		case 5:
			withdrawal();
			break;
		case 6:
			changePassword();
			break;
		case 7:
			logoutAccount();
			break;
		case 8:
			printf("再见！\n");
			break;
		default:
			printf("请重新输入：\n");
		}
	} while (choice != 8);
	return 0;
}