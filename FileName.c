#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
// 定义一个存储账户信息的结构体
struct Account
{
	char username[11];
	char password[21];
	float balance;
	int securePassword;
};

// 定义一个判断用户名正确性的函数
int isValidUsername(char* username)
{
	bool hasDigit = false;
	bool hasAlpha = false;
	size_t len = strlen(username);
	if (len > 10)
	{
		return 1;
	}
	for (int i = 0; i < len; i++)
	{
		if (isdigit(username[i]))
		{
			hasDigit = true;
		}
		else if (isalpha(username[i]))
		{
			hasAlpha = true;
		}
		else
		{
			return 2;
		}
	}
	if (hasAlpha && hasDigit)
	{
		return 0;
	}
	else
	{
		return 3;
	}
}

// 定义一个判断密码正确性的函数
int isValidPassword(char* password)
{
	bool hasDigit = false;
	bool hasAlpha = false;
	bool hasPunct = false;
	size_t len = strlen(password);
	if (len < 8)
	{
		return 1;
	}
	else if (len > 20)
	{
		return 2;
	}
	for (int i = 0; i < len; i++)
	{
		if (isdigit(password[i]))
		{
			hasDigit = true;
		}
		else if (isalpha(password[i]))
		{
			hasAlpha = true;
		}
		else if (ispunct(password[i]))
		{
			hasPunct = true;
		}
		else
		{
			return 3;
		}
	}
	if (hasAlpha && hasDigit && hasPunct)
	{
		return 0;
	}
	else
	{
		return 4;
	}
}

// 定义一个判断支付密码正确性的函数
bool isValidSecurePassword(int num)
{
	int count = 0;
	while (num != 0)
	{
		num = num / 10;
		count++;
	}
	if (count != 6)
	{
		return false;
	}
	return true;
}

// 定义一个接收用户名输入的函数
void getUsername(char username[11])
{
	fgets(username, 11, stdin);
	username[strcspn(username, "\n")] = 0;
}

// 定义一个接收用户输入密码的函数
void getPassword(char password[21])
{
	fgets(password, 21, stdin);
	password[strcspn(password, "\n")] = 0;
}

// 定义一个接收用户输入金额的函数
void getBalance(float* balance)
{
	int c;
	scanf("%f", balance);
	while ((c = getchar()) != '\n' && c != EOF);
}

// 定义一个接收用户输入支付密码的函数
void getSecurePassword(int* securePassword)
{
	int c;
	scanf("%d", securePassword);
	while ((c = getchar()) != '\n' && c != EOF);
}

// 定义一个更新文件信息的函数
void updateAccountInFile(struct Account* acc)
{
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("无法打开账户文件。\n");
		return;
	}
	FILE* temp = fopen("temp.txt", "w");
	if (temp == NULL)
	{
		printf("无法创建临时文件。\n");
		fclose(account);
		return;
	}
	struct Account* acc_temp = malloc(sizeof(struct Account));
	char line[1024];
	while (fgets(line, sizeof(line), account) != NULL)
	{
		sscanf(line, "%s %s %f %d", acc_temp->username, acc_temp->password, &acc_temp->balance, &acc_temp->securePassword);
		if (strcmp(acc_temp->username, acc->username) == 0)
		{
			fprintf(temp, "%s %s %.2f %d\n", acc->username, acc->password, acc->balance, acc->securePassword);
		}
		else
		{
			fputs(line, temp);
		}
	}
	fclose(account);
	fclose(temp);
	remove("account.txt");
	rename("temp.txt", "account.txt");
	free(acc_temp);
}

// 判断用户是否已经登录
bool isLogin(struct Account** acc)
{
	if (acc != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 显示主菜单
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

// 创建账户
void createAccount(struct Account** acc)
{
	if (isLogin(acc))
	{
		printf("账户已经登录，不能继续创建\n");
		return;
	}
	struct Account* acc_temp;
	acc_temp = (struct Account*)malloc(sizeof(struct Account));
	if (acc_temp == NULL)
	{
		printf("内存分配失败\n");
		return;
	}
	char line[1024];
	FILE* account = fopen("account.txt", "a+");
	if (account == NULL)
	{
		printf("文件打开失败\n");
		free(acc_temp);
		return;
	}
	char username[11];
	char password[21];
	float balance;
	int securePassword;
	printf("请输入用户名，最多10位，只能包括字母和数字：");
	while (true)
	{
		getUsername(username);
		rewind(account);
		while (fgets(line, sizeof(line), account) != NULL)
		{
			if (sscanf(line, "%s %s %f %d", acc_temp->username, acc_temp->password, &acc_temp->balance, &acc_temp->securePassword) == 4)
			{
				if (strcmp(username, acc_temp->username) == 0)
				{
					printf("账户已经存在，请重新输入\n");
					continue;
				}
			}
		}
		int usernameValidationResult = isValidUsername(username);
		switch (usernameValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("用户名最多10位，请重新输入\n");
			break;
		case 2:
			printf("输入非法字符，请重新输入\n");
			break;
		case 3:
			printf("用户名需要包括字母和数字，请重新输入\n");
			break;
		default:
			break;
		}
		if (usernameValidationResult != 0)
		{
			continue;
		}
		break;
	}
	printf("请输入密码，最多20位，需要包括数字、字母和特殊字符：");
	while (true)
	{
		getPassword(password);
		int passwordValidationResult = isValidPassword(password);
		switch (passwordValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("密码不能少于八位，请重新输入\n");
			break;
		case 2:
			printf("密码不能大于20位，请重新输入\n");
			break;
		case 3:
			printf("输入非法字符，请重新输入\n");
			break;
		case 4:
			printf("密码必须包括字母、数字和特殊字符，请重新输入\n");
			break;
		default:
			break;
		}
		if (passwordValidationResult != 0)
		{
			continue;
		}
		break;
	}
	printf("请输入初始金额：");
	getBalance(&balance);
	printf("请设置支付密码，应该有六位：");
	while (true)
	{
		getSecurePassword(&securePassword);
		if (!isValidSecurePassword(securePassword))
		{
			printf("支付密码必须有六位，请重新输入\n");
			continue;
		}
		else
		{
			break;
		}
	}
	printf("账户创建成功\n");
	fprintf(account, "%s %s %.2f %d\n", username, password, balance, securePassword);
	free(acc_temp);
	fclose(account);
}

// 登录
void loginAccount(struct Account** acc)
{
	if (isLogin(acc))
	{
		printf("已经登录，不能登录第二个\n");
		return;
	}
	char username[11];
	char password[21];
	struct Account* acc_local = malloc(sizeof(struct Account));
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("文件打开失败\n");
		return;
	}
	printf("请输入用户名：");
	while (true)
	{
		getUsername(username);
		int usernameValidationResult = isValidUsername(username);
		switch (usernameValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("用户名最多10位，请重新输入\n");
			break;
		case 2:
			printf("输入非法字符，请重新输入\n");
			break;
		case 3:
			printf("用户名需要包括字母和数字，请重新输入\n");
			break;
		default:
			break;
		}
		if (usernameValidationResult != 0)
		{
			continue;
		}
		break;
	}
	while (fscanf(account, "%s %s %f %d", acc_local->username, acc_local->password, &acc_local->balance, &acc_local->securePassword) != EOF)
	{
		if (strcmp(username, acc_local->username) == 0)
		{
			break;
		}
		printf("账户不存在\n");
		break;
	}
	printf("请输入密码：");
	while (true)
	{
		getPassword(password);
		int passwordValidationResult = isValidPassword(password);
		switch (passwordValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("密码不能少于八位，请重新输入\n");
			break;
		case 2:
			printf("密码不能大于20位，请重新输入\n");
			break;
		case 3:
			printf("输入非法字符，请重新输入\n");
			break;
		case 4:
			printf("密码必须包括字母、数字和特殊字符，请重新输入\n");
			break;
		default:
			break;
		}
		if (passwordValidationResult != 0)
		{
			continue;
		}
		if (strcmp(password, acc_local->password) != 0)
		{
			printf("密码错误，请重新输入\n");
			continue;
		}
		else
		{
			break;
		}
	}
	printf("登录成功\n");
	*acc = acc_local;
	updateAccountInFile(acc_local);
	fclose(account);
}

// 查看余额
void viewBalance(struct Account** acc)
{
	if (isLogin(acc))
	{
		printf("你的金额为：%.2f\n", (*acc)->balance);
	}
	else
	{
		printf("还没有登录，请先登录\n");
	}
	return;
}

// 存款
void deposit(struct Account** acc)
{
	float balance;
	int securePassword;
	if (!isLogin(acc))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	printf("请输入支付密码：");
	while (true)
	{
		getSecurePassword(&securePassword);
		if (!isValidSecurePassword(securePassword))
		{
			printf("支付密码必须有六位，请重新输入\n");
			continue;
		}
		if (securePassword != (*acc)->securePassword)
		{
			printf("密码错误，请重新输入\n");
			continue;
		}
		else
		{
			break;
		}
	}
	printf("请输入存款金额：");
	getBalance(&balance);
	(*acc)->balance = (*acc)->balance + balance;
	updateAccountInFile(*acc);
	printf("存款成功\n");
}

// 取款
void withdrawal(struct Account** acc)
{
	if (!isLogin(acc))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	float balance;
	int securePassword;
	printf("请输入支付密码：");
	while (true)
	{
		getSecurePassword(&securePassword);
		if (!isValidSecurePassword(securePassword))
		{
			printf("支付密码必须有六位，请重新输入\n");
			continue;
		}
		if (securePassword != (*acc)->securePassword)
		{
			printf("密码错误，请重新输入\n");
			continue;
		}
		else
		{
			break;
		}
	}
	printf("请输入取款金额：");
	while (true)
	{
		getBalance(&balance);
		if ((*acc)->balance - balance < 0)
		{
			printf("余额不足，你的余额还有%f，请重新输入：", balance);
			continue;
		}
		else
		{
			(*acc)->balance = (*acc)->balance - balance;
			updateAccountInFile(*acc);
			printf("取款成功\n");
			return;
		}
	}
}

// 修改密码
void changePassword(struct Account** acc)
{
	char password[21];
	if (!isLogin(acc))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	printf("请输入新密码：");
	while (true)
	{
		getPassword(password);
		int passwordValidationResult = isValidPassword(password);
		switch (passwordValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("密码不能少于八位，请重新输入\n");
			break;
		case 2:
			printf("密码不能大于20位，请重新输入\n");
			break;
		case 3:
			printf("输入非法字符，请重新输入\n");
			break;
		case 4:
			printf("密码必须包括字母、数字和特殊字符，请重新输入\n");
			break;
		default:
			break;
		}
		if (passwordValidationResult != 0)
		{
			continue;
		}
		else
		{
			strcpy((*acc)->password, password);
			updateAccountInFile(*acc);
			printf("密码修改成功\n");
			return;
		}
	}
}

// 修改支付密码
void changeSecurePassword(struct Account** acc)
{
	int securePassword;
	if (!isLogin(acc))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	printf("请输入修改的支付密码：");
	while (true)
	{
		getSecurePassword(&securePassword);
		if (!isValidSecurePassword(securePassword))
		{
			printf("支付密码必须有六位，请重新输入\n");
			continue;
		}
		else
		{
			(*acc)->securePassword = securePassword;
			updateAccountInFile(*acc);
			printf("支付密码修改成功\n");
			return;
		}
	}
}

// 退出系统
void logoutAccount(struct Account** acc)
{
	if (!isLogin(acc))
	{
		printf("账户没有登录\n");
		return;
	}
	free(*acc);
	acc = NULL;
	printf("你已成功注销\n");
}

// 删除账户
void removeAccount(struct Account** acc)
{
	if (!isLogin(acc))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("无法打开账户文件。\n");
		return;
	}
	FILE* temp = fopen("temp.txt", "w");
	if (temp == NULL)
	{
		printf("无法创建临时文件。\n");
		fclose(account);
		return;
	}
	char line[1024];
	bool accountRemoved = false;
	struct Account* acc_local;
	acc_local = (struct Account*)malloc(sizeof(struct Account));
	while (fgets(line, sizeof(line), account) != NULL)
	{
		if (sscanf(line, "%s %s %f %d", acc_local->username, acc_local->password, &acc_local->balance, &acc_local->securePassword) == 4)
		{
			if (strcmp((*acc)->username, acc_local->username) != 0)
			{
				fputs(line, temp);
			}
			else
			{
				accountRemoved = true;
			}
		}
	}
	fclose(account);
	fclose(temp);
	if (accountRemoved)
	{
		remove("account.txt");
		rename("temp.txt", "account.txt");
		printf("账户已成功删除。\n");
		free(*acc);
		return;
	}
	else
	{
		remove("temp.txt");
		printf("没有找到要删除的账户。\n");
		free(*acc);
		return;
	}
}

int main(void)
{
	int choice;
	int c;
	bool running = true;
	struct Account* acc = NULL;
	acc = (struct Account*)malloc(sizeof(struct Account));
	if (acc == NULL)
	{
		printf("内存分配失败\n");
		return 1;
	}
	do
	{
		displayMenu();
		if (scanf("%d", &choice) != 1)
		{
			printf("无效输入，请输入一个数字\n");
			while ((c = getchar()) != '\n' && c != EOF);
			continue;
		}
		if (choice < 1 || choice > 10)
		{
			printf("范围不正确，正确的范围应该介于1-10之间，请重新输入\n");
			continue;
		}
		while ((c = getchar()) != '\n' && c != EOF);
		switch (choice)
		{
		case 1:
			createAccount(&acc);
			break;
		case 2:
			loginAccount(&acc);
			break;
		case 3:
			viewBalance(&acc);
			break;
		case 4:
			deposit(&acc);
			break;
		case 5:
			withdrawal(&acc);
			break;
		case 6:
			changePassword(&acc);
			break;
		case 7:
			changeSecurePassword(&acc);
			break;
		case 8:
			logoutAccount(&acc);
			break;
		case 9:
			removeAccount(&acc);
			break;
		case 10:
			if (acc != NULL)
			{
				printf("你还没有注销，请先注销\n");
				break;
			}
			else
			{
				printf("再见！\n");
				running = false;
			}
			break;
		default:
			printf("请重新输入：\n");
		}
	} while (running);
	free(acc);
	return 0;
}