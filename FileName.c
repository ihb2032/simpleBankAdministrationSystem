#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MinLength 1
#define MaxUsernameLength 10
#define MaxPasswordLength 20
#define MaxPinDigits 6
#define MaxLineLength 1024

// 定义一个存储账户信息的链表
struct account
{
	char username[MaxUsernameLength + 1];
	char password[MaxPasswordLength + 1];
	float balance;
	int paymentPin;
	struct account* next;
};

enum ValidationStatus
{
	// 用户名验证状态
	UsernameValid = 0,
	UsernameEmpty = 1,
	UsernameInvalidLength = 2,
	UsernameInvalidCharacter = 3,
	UsernameRequiresAlphaAndDigit = 4,
	// 密码验证状态
	PasswordValid = 10,
	PasswordEmpty = 11,
	PasswordInvalidLength = 12,
	PasswordInvalidCharacter = 13,
	PasswordRequiresAlphaDigitPunct = 14
};

// 打开账户文件
FILE* openAccountFile(char* mode)
{
	FILE* file = fopen("account.txt", mode);
	if (file == NULL)
	{
		perror("文件打开失败");
	}
	return file;
}

// 从文件中读取账户信息
bool readAccountFromFile(FILE* file, struct account* acc)
{
	char line[MaxLineLength];
	if (fgets(line, sizeof(line), file) != NULL)
	{
		if (sscanf(line, "%s %s %f %d", acc->username, acc->password, &acc->balance, &acc->paymentPin) == 4)
		{
			return true;
		}
	}
	return false;
}

// 向文件中写入账户信息
void writeAccountToFile(FILE* file, const struct account* acc)
{
	fprintf(file, "%s %s %.2f %d\n", acc->username, acc->password, acc->balance, acc->paymentPin);
}

// 更新文件中的账户信息
void updateAccountInFile(struct account* accountPtr)
{
	FILE* accountFile = openAccountFile("r");
	FILE* tempFile = openAccountFile("w");
	struct account tempAccount;
	while (readAccountFromFile(accountFile, &tempAccount))
	{
		if (strcmp(tempAccount.username, accountPtr->username) == 0)
		{
			writeAccountToFile(tempFile, accountPtr);
		}
		else
		{
			writeAccountToFile(tempFile, &tempAccount);
		}
	}
	fclose(accountFile);
	fclose(tempFile);
	remove("account.txt");
	rename("temp.txt", "account.txt");
}

// 从文件中删除账户信息
void deleteAccountFromFile(struct account** accountPtr)
{
	FILE* accountFile = openAccountFile("r");
	FILE* tempFile = openAccountFile("w");
	struct account tempAccount;
	bool accountRemoved = false;
	while (readAccountFromFile(accountFile, &tempAccount))
	{
		if (strcmp((*accountPtr)->username, tempAccount.username) != 0)
		{
			writeAccountToFile(tempFile, &tempAccount);
		}
		else
		{
			accountRemoved = true;
		}
	}
	fclose(accountFile);
	fclose(tempFile);
	if (accountRemoved)
	{
		remove("account.txt");
		rename("temp.txt", "account.txt");
		free(*accountPtr);
		*accountPtr = NULL;
	}
	else
	{
		remove("temp.txt");
	}
}

// 辅助函数：检查字符是否合法
bool isCharValid(char ch, bool isForUsername)
{
	if (isForUsername)
	{
		return isdigit(ch) || isalpha(ch);
	}
	else
	{
		return isdigit(ch) || isalpha(ch) || ispunct(ch);
	}
}

// 辅助函数：检查字符串长度是否在指定的范围内
bool isLengthValid(char* str, size_t minLen, size_t maxLen)
{
	size_t len = strlen(str);
	return len >= minLen && len <= maxLen;
}

// 检查用户名是否合法
enum ValidationStatus isValidUsername(char* username)
{
	if (!isLengthValid(username, MinLength, MaxUsernameLength))
	{
		return username[0] == '\0' ? UsernameEmpty : UsernameInvalidLength;
	}
	bool hasDigit = false;
	bool hasAlpha = false;
	for (int i = 0; username[i] != '\0'; i++)
	{
		if (isCharValid(username[i], true))
		{
			hasDigit |= isdigit(username[i]);
			hasAlpha |= isalpha(username[i]);
		}
		else
		{
			return UsernameInvalidCharacter; // 包含非法字符
		}
	}
	if (hasAlpha && hasDigit)
	{
		return UsernameValid; // 用户名有效
	}
	else
	{
		return UsernameRequiresAlphaAndDigit; // 用户名需要包含字母和数字
	}
}

// 验证密码是否合法
enum ValidationStatus isValidPassword(char* password)
{
	if (!isLengthValid(password, MinLength, MaxPasswordLength))
	{
		return password[0] == '\0' ? PasswordEmpty : PasswordInvalidLength;
	}
	bool hasDigit = false;
	bool hasAlpha = false;
	bool hasPunct = false;
	for (int i = 0; password[i] != '\0'; i++)
	{
		if (isCharValid(password[i], false))
		{
			hasDigit |= isdigit(password[i]);
			hasAlpha |= isalpha(password[i]);
			hasPunct |= ispunct(password[i]);
		}
		else
		{
			return PasswordInvalidCharacter; // 包含非法字符
		}
	}
	if (hasAlpha && hasDigit && hasPunct)
	{
		return PasswordValid; // 密码有效
	}
	else
	{
		return PasswordRequiresAlphaDigitPunct; // 密码需要包含字母、数字和特殊字符
	}
}

// 定义一个判断支付密码正确性的函数
bool isValidSecurePassword(const int* password)
{
	// 检查密码长度是否为6位
	if (strlen(password) != 6)
	{
		return false;
	}
	// 遍历密码中的每个字符
	for (int i = 0; i < 6; i++)
	{
		// 如果当前字符不是数字，则返回false
		if (!isdigit((unsigned char)password[i]))
		{
			return false;
		}
	}
	// 所有检查都通过了，返回true
	return true;
}

// 读取用户输入并去除换行符
void readInput(char input[], int inputSize)
{
	char buffer[MaxLineLength];
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
}

// 检查账户是否存在
bool accountExists(const char input[], FILE* accountFile)
{
	struct account tempAccount;
	rewind(accountFile); // 确保从文件开头开始
	while (readAccountFromFile(accountFile, &tempAccount))
	{
		if (strcmp(input, tempAccount.username) == 0)
		{
			return true; // 找到匹配的账户名
		}
	}
	return false; // 没有找到匹配的账户名
}

// 接收用户名和密码的输入并进行验证
void checkAndReadInput(char input[], int inputSize, FILE* accountFile, int (*validateInput)(const char*), const char* inputPrompt, const char* errorMessages[])
{
	printf("%s", inputPrompt);
	while (true)
	{
		readInput(input, inputSize);
		int validationResult = validateInput(input);
		if (validationResult != UsernameValid && validationResult != PasswordValid)
		{
			printf("%s", errorMessages[validationResult]);
			continue;
		}
		else
		{
			break;
		}
	}
}

// 定义一个接收并验证用户名的函数
void checkAndReadUsername(char username[MaxUsernameLength + 1], FILE* accountFile)
{
	char* usernameErrors[] = { "", "用户名不能为空，请重新输入：", "用户名最多10位，请重新输入", "输入非法字符，请重新输入", "用户名需要包括字母和数字，请重新输入" };
	checkAndReadInput(username, sizeof(username), accountFile, (int (*)(const char*))isValidUsername, "请输入用户名，最多10位，只能包括字母和数字：", usernameErrors);
}

// 定义一个接收并验证密码的函数
void checkAndReadPassword(char password[MaxPasswordLength + 1])
{
	char* passwordErrors[] = { "", "密码不能为空，请重新输入：", "密码不能大于20位，请重新输入：", "输入非法字符，请重新输入：", "密码必须包括字母、数字和特殊字符，请重新输入：" };
	checkAndReadInput(password, sizeof(password), NULL, (int (*)(const char*))isValidPassword, "请输入密码，最多20位，需要包括数字、字母和特殊字符：", passwordErrors);
}

void inputNumber(void* number, const char* prompt, const char* format)
{
	int scanResult;
	int extraChar;
	do
	{
		printf("%s", prompt); // 显示提示信息
		scanResult = scanf(format, number); // 根据格式字符串读取输入
		if (scanResult != 1)
		{
			printf("无效输入，请输入一个数字："); // 如果输入无效，显示错误信息
			while ((extraChar = getchar()) != '\n' && extraChar != EOF); // 清除输入缓冲区
			continue;
		}
		while ((extraChar = getchar()) != '\n' && extraChar != EOF); // 清除输入缓冲区
	} while (scanResult != 1);
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
	while ((discardChar = getchar()) != '\n' && discardChar != EOF);
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
	FILE* account = openAccountFile("a+");
	char username[MaxUsernameLength + 1];
	char password[MaxPasswordLength + 1];
	float balance;
	int paymentPin;
	checkAndReadUsername(username, account);
	checkAndReadPassword(password);
	inputNumber(&balance, "请输入初始金额：", "%f");
	while (true)
	{
		inputNumber(&paymentPin, "请设置支付密码，应该有六位：", "%d");
		if (isValidSecurePassword(&paymentPin))
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
	char username[MaxUsernameLength + 1];
	char password[MaxPasswordLength + 1];
	struct account* localAccount = malloc(sizeof(struct account));
	FILE* account = openAccountFile("r");
	checkAndReadUsername(username, account);
	checkAndReadPassword(password);
	bool found = false;
	while (readAccountFromFile(account, localAccount))
	{
		if (strcmp(username, localAccount->username) == 0 && strcmp(password, localAccount->password) == 0)
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		printf("登录成功\n");
		*accountPtr = localAccount;
	}
	else
	{
		printf("用户名或密码错误\n");
		free(localAccount);
	}
	fclose(account);
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
	int paymentPin;
	if (!isLogin(accountPtr))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	inputNumber(&paymentPin, "请输入支付密码：", "d");
	inputNumber(&balance, "请输入存款金额：", "f");
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
	int paymentPin;
	inputNumber(&paymentPin, "请输入支付密码：", "d");
	while (true)
	{
		inputNumber(&balance, "请输入取款金额", "f");
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
	char password[MaxPasswordLength + 1];
	if (!isLogin(accountPtr))
	{
		printf("没有登录，请先登录\n");
		return;
	}
	printf("请输入新密码：");
		checkAndReadPassword(password);
		strcpy((*accountPtr)->password, password);
		updateAccountInFile(*accountPtr);
		printf("密码修改成功\n");
		return;
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
	inputNumber(&paymentPin, "请输入新密码：", "d");
			(*accountPtr)->paymentPin = paymentPin;
			updateAccountInFile(*accountPtr);
			printf("支付密码修改成功\n");
			return;
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
	deleteAccountFromFile(accountPtr);
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
	return 0;
}