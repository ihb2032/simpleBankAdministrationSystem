#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
// 定义一个存储账户信息的结构体
struct account
{
	char username[11];
	char password[21];
	float balance;
	int paymentPin;
};

// 定义一个判断用户名正确性的函数
int isValidUsername(const char* username)
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
int isValidPassword(const char* password)
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
bool isValidSecurePassword(int paymentPin)
{
	int count = 0;
	while (paymentPin != 0)
	{
		paymentPin = paymentPin / 10;
		count++;
	}
	if (count != 6)
	{
		return false;
	}
	return true;
}

// 接收用户名和密码的输入并进行验证
void checkAndReadInput(char input[], int inputSize, FILE* accountFile, int (*validateInput)(const char*), const char* inputPrompt, const char* errorMessages[])
{
	bool inputValid;
	char buffer[1024];
	do
	{
		inputValid = true;
		printf("%s", inputPrompt);
		fgets(buffer, sizeof(buffer), stdin);
		if (strchr(buffer, '\n') == NULL)
		{
			int extraChar;
			while ((extraChar = getchar()) != '\n' && extraChar != EOF);
		}
		size_t len = strlen(buffer);
		if (buffer[len - 1] == '\n')
		{
			buffer[len - 1] = '\0';
		}
		strncpy(input, buffer, inputSize - 1);
		input[inputSize - 1] = '\0';
		if (accountFile != NULL)
		{
			struct account tempAccount;
			rewind(accountFile);
			while (fgets(buffer, sizeof(buffer), accountFile) != NULL)
			{
				sscanf(buffer, "%s %s %f %d", tempAccount.username, tempAccount.password, &tempAccount.balance, &tempAccount.paymentPin);
				if (strcmp(input, tempAccount.username) == 0)
				{
					printf("账户已经存在，请重新输入\n");
					inputValid = false;
					break;
				}
			}
		}
		int validationResult = validateInput(input);
		if (validationResult != 0)
		{
			printf("%s", errorMessages[validationResult]);
			inputValid = false;
		}
	} while (!inputValid);
}

// 定义一个接收并验证用户名的函数
void checkAndReadUsername(char username[11], FILE* accountFile)
{
	char* usernameErrors[] = { "", "用户名最多10位，请重新输入\n", "输入非法字符，请重新输入\n", "用户名需要包括字母和数字，请重新输入\n" };
	checkAndReadInput(username, sizeof(username), accountFile, isValidUsername, "请输入用户名，最多10位，只能包括字母和数字：", usernameErrors);
}

// 定义一个接收并验证密码的函数
void checkAndReadPassword(char password[21])
{
	char* passwordErrors[] = { "", "密码不能少于八位，请重新输入\n", "密码不能大于20位，请重新输入\n", "输入非法字符，请重新输入\n", "密码必须包括字母、数字和特殊字符，请重新输入\n" };
	checkAndReadInput(password, sizeof(password), NULL, isValidPassword, "请输入密码，最多20位，需要包括数字、字母和特殊字符：", passwordErrors);
}

// 定义一个接收用户输入金额的函数
void inputBalance(float* balance)
{
	int scanResult;
	int extraChar;
	do
	{
		scanResult = scanf("%f", balance);
		if (scanResult != 1)
		{
			printf("无效输入，请输入一个数字：");
		}
		while ((extraChar = getchar()) != '\n' && extraChar != EOF);
	} while (scanResult != 1);
}

// 定义一个接收用户输入支付密码的函数
void inputSecurePassword(int* paymentPin)
{
	int scanResult;
	int extraChar;
	do
	{
		scanResult = scanf("%d", paymentPin);
		if (scanResult != 1)
		{
			printf("无效输入，请输入一个数字：\n");
		}
		while ((extraChar = getchar()) != '\n' && extraChar != EOF);
	} while (scanResult != 1);
}

// 定义一个更新文件信息的函数
void updateAccountInFile(struct account* accountPtr)
{
	struct account* tempAccount = malloc(sizeof(struct account));
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("无法打开账户文件。\n");
		free(tempAccount);
		return;
	}
	FILE* temp = fopen("temp.txt", "w");
	if (temp == NULL)
	{
		printf("无法创建临时文件。\n");
		fclose(account);
		free(tempAccount);
		return;
	}
	char line[1024];
	while (fgets(line, sizeof(line), account) != NULL)
	{
		sscanf(line, "%s %s %f %d", tempAccount->username, tempAccount->password, &tempAccount->balance, &tempAccount->paymentPin);
		if (strcmp(tempAccount->username, accountPtr->username) == 0)
		{
			fprintf(temp, "%s %s %.2f %d\n", accountPtr->username, accountPtr->password, accountPtr->balance, accountPtr->paymentPin);
		}
		else
		{
			fputs(line, temp);
		}
	}
	fclose(account);
	fclose(temp);
	free(tempAccount);
	remove("account.txt");
	rename("temp.txt", "account.txt");
}

// 判断用户是否已经登录
bool isLogin(struct account** accountPtr)
{
	if (*accountPtr != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool verifyPaymentPin(const struct account* accountPtr)
{
	int paymentPin;
	printf("请输入支付密码：");
	inputSecurePassword(&paymentPin);
	if (!isValidSecurePassword(paymentPin))
	{
		printf("支付密码必须有六位，请重新输入\n");
		return false;
	}
	if (paymentPin != accountPtr->paymentPin)
	{
		printf("支付密码错误，请重新输入\n");
		return false;
	}
	return true;
}

// 接受用户输入
void userChoice(int* choice)
{
	char discardChar;
	if (scanf("%d", choice) != 1)
	{
		printf("无效输入，请输入一个数字\n");
		while ((discardChar = getchar()) != '\n' && discardChar != EOF);
		return;
	}
	if (*choice < 1 || *choice > 10)
	{
		printf("范围不正确，正确的范围应该介于1-10之间，请重新输入\n");
		while ((discardChar = getchar()) != '\n' && discardChar != EOF);
		return;
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
void createAccount(struct account** accountPtr)
{
	if (isLogin(accountPtr))
	{
		printf("账户已经登录，不能继续创建\n");
		return;
	}
	FILE* account = fopen("account.txt", "a+");
	if (account == NULL)
	{
		printf("文件打开失败\n");
		return;
	}
	char username[11];
	char password[21];
	float balance;
	int paymentPin;
	checkAndReadUsername(username, account);
	checkAndReadPassword(password);
	printf("请输入初始金额：");
	inputBalance(&balance);
	printf("请设置支付密码，应该有六位：");
	while (true)
	{
		inputSecurePassword(&paymentPin);
		if (!isValidSecurePassword(paymentPin))
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
	fprintf(account, "%s %s %.2f %d\n", username, password, balance, paymentPin);
	fclose(account);
}

// 登录
void loginAccount(struct account** accountPtr)
{
	if (isLogin(accountPtr))
	{
		printf("已经登录，不能登录第二个\n");
		return;
	}
	char username[11];
	char password[21];
	struct account* localAccount = malloc(sizeof(struct account));
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("文件打开失败\n");
		free(localAccount);
		return;
	}
	while (fscanf(account, "%s %s %f %d", localAccount->username, localAccount->password, &localAccount->balance, &localAccount->paymentPin) != EOF)
	{
		checkAndReadUsername(username, account);
		if (strcmp(username, localAccount->username) == 0)
		{
			break;
		}
		else
		{
			printf("用户名错误\n");
			fclose(account);
			continue;
		}
	}
	while (true)
	{
		checkAndReadPassword(password);
		if (strcmp(password, localAccount->password) != 0)
		{
			printf("密码错误，请重新输入\n");
			continue;
		}
		else
		{
			printf("登录成功\n");
			*accountPtr = localAccount;
			updateAccountInFile(localAccount);
			fclose(account);
			return;
		}
	}
}

// 查看余额
void viewBalance(struct account** accountPtr)
{
	if (isLogin(accountPtr))
	{
		printf("你的金额为：%.2f\n", (*accountPtr)->balance);
	}
	else
	{
		printf("还没有登录，请先登录\n");
		return;
	}
}

// 存款
void deposit(struct account** accountPtr)
{
	float balance;
	if (!isLogin(accountPtr))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	if (!verifyPaymentPin(*accountPtr))
	{
		return;
	}
	printf("请输入存款金额：");
	inputBalance(&balance);
	(*accountPtr)->balance = (*accountPtr)->balance + balance;
	updateAccountInFile(*accountPtr);
	printf("存款成功\n");
}

// 取款
void withdrawal(struct account** accountPtr)
{
	if (!isLogin(accountPtr))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	float balance;
	if (!verifyPaymentPin(*accountPtr))
	{
		return;
	}
	printf("请输入取款金额：");
	while (true)
	{
		inputBalance(&balance);
		if ((*accountPtr)->balance - balance < 0)
		{
			printf("余额不足，你的余额还有%f，请重新输入：", (*accountPtr)->balance);
			continue;
		}
		else
		{
			(*accountPtr)->balance = (*accountPtr)->balance - balance;
			updateAccountInFile(*accountPtr);
			printf("取款成功\n");
			return;
		}
	}
}

// 修改密码
void changePassword(struct account** accountPtr)
{
	char password[21];
	if (!isLogin(accountPtr))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	printf("请输入新密码：");
	while (true)
	{
		checkAndReadPassword(password);
		strcpy((*accountPtr)->password, password);
		updateAccountInFile(*accountPtr);
		printf("密码修改成功\n");
		return;
	}
}

// 修改支付密码
void changeSecurePassword(struct account** accountPtr)
{
	int paymentPin;
	if (!isLogin(accountPtr))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	printf("请输入修改的支付密码：");
	while (true)
	{
		inputSecurePassword(&paymentPin);
		if (!isValidSecurePassword(paymentPin))
		{
			printf("支付密码必须有六位，请重新输入\n");
			continue;
		}
		else
		{
			(*accountPtr)->paymentPin = paymentPin;
			updateAccountInFile(*accountPtr);
			printf("支付密码修改成功\n");
			return;
		}
	}
}

// 退出系统
void logoutAccount(struct account** accountPtr)
{
	if (!isLogin(accountPtr))
	{
		printf("账户没有登录\n");
		return;
	}
	free(*accountPtr);
	*accountPtr = NULL;
	printf("你已成功注销\n");
}

// 删除账户
void removeAccount(struct account** accountPtr)
{
	if (!isLogin(accountPtr))
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
	struct account* localAccount;
	localAccount = (struct account*)malloc(sizeof(struct account));
	while (fgets(line, sizeof(line), account) != NULL)
	{
		if (sscanf(line, "%s %s %f %d", localAccount->username, localAccount->password, &localAccount->balance, &localAccount->paymentPin) == 4)
		{
			if (strcmp((*accountPtr)->username, localAccount->username) != 0)
			{
				fputs(line, temp);
			}
			else
			{
				accountRemoved = true;
			}
		}
	}
	free(localAccount);
	if (accountRemoved)
	{
		remove("account.txt");
		rename("temp.txt", "account.txt");
		printf("账户已成功删除\n");
		free(*accountPtr);
		*accountPtr = NULL;
		fclose(account);
		fclose(temp);
		return;
	}
	else
	{
		remove("temp.txt");
		printf("没有找到要删除的账户\n");
		fclose(account);
		fclose(temp);
		return;
	}
}

int main(void)
{
	int choice;
	bool running = true;
	struct account* currentAccount = NULL;
	do
	{
		displayMenu();
		userChoice(&choice);
		switch (choice)
		{
		case 1:
			createAccount(&currentAccount);
			break;
		case 2:
			loginAccount(&currentAccount);
			break;
		case 3:
			viewBalance(&currentAccount);
			break;
		case 4:
			deposit(&currentAccount);
			break;
		case 5:
			withdrawal(&currentAccount);
			break;
		case 6:
			changePassword(&currentAccount);
			break;
		case 7:
			changeSecurePassword(&currentAccount);
			break;
		case 8:
			logoutAccount(&currentAccount);
			break;
		case 9:
			removeAccount(&currentAccount);
			break;
		case 10:
			if (currentAccount != NULL)
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
	free(currentAccount);
	return 0;
}