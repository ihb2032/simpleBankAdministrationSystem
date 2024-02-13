#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MaxUsernameLength 11
#define MaxPasswordLength 21
#define MinPasswordLength 8

// 定义一个存储账户信息的链表
typedef struct Card
{
	char* username;
	char* password;
	float balance;
	int paymentPin;
	struct Card* next;
} Card;

// 用户名验证状态
enum usernameValidationStatus
{
	UsernameValid = 0,
	UsernameEmpty = 1,
	UsernameInvalidLength = 2,
	UsernameInvalidCharacter = 3,
	UsernameRequiresAlphaAndDigit = 4
};

// 密码验证状态
enum  passwordValidationStatus
{
	PasswordValid = 0,
	PasswordEmpty = 1,
	PasswordInvalidLengthLower = 2,
	PasswordInvalidLengthUpper = 3,
	PasswordRequiresAlphaDigitPunct = 4
};

// 比较函数
int isMatch(Card* card, Card* newCard)
{
	return strcmp(card->username, newCard->username) == 0 && strcmp(card->password, newCard->password) == 0;
}

// 清除输入缓冲区直到遇到换行或EOF
void clearInputBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// 创建银行卡链表
Card* createCard(Card* newCard)
{
	Card* card = (Card*)malloc(sizeof(Card));
	card->username = (char*)malloc(MaxUsernameLength * sizeof(char));
	strcpy(card->username, newCard->username);
	card->password = (char*)malloc(MaxPasswordLength * sizeof(char));
	strcpy(card->password, newCard->password);
	card->balance = newCard->balance;
	card->paymentPin = newCard->paymentPin;
	card->next = NULL;
	return card;
}

// 在链表尾部插入账号
void insertEnd(Card** head, Card* card)
{
	Card* newCard = createCard(card);
	if (*head == NULL)
	{
		*head = newCard;
	}
	else
	{
		Card* current = *head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newCard;
	}
}

// 删除链表中的账号
void deleteAccount(Card** head, Card* card)
{
	Card* current = *head;
	Card* prev = NULL;
	while (current != NULL)
	{
		if (isMatch(current, card))
		{
			if (prev == NULL)
			{
				*head = current->next;
			}
			else
			{
				prev->next = current->next;
			}
			free(current->username);
			free(current->password);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

// 释放账户链表
void freeCard(Card** card)
{
	Card* current = *card;
	Card* next = NULL;
	while (current != NULL)
	{
		next = current->next;
		free(current->username);
		free(current->password);
		free(current);
		current = next;
	}
	*card = NULL;
}

// 将文件中的账号读入链表
void fileToListNode(Card** card)
{
	FILE* file = fopen("account.txt", "r");
	if (file == NULL)
	{
		perror("Error opening file");
		return;
	}
	while (true)
	{
		Card* newCard = (Card*)malloc(sizeof(Card));
		newCard->username = (char*)malloc(MaxUsernameLength * sizeof(char));
		newCard->password = (char*)malloc(MaxPasswordLength * sizeof(char));
		if (fscanf(file, "%s %s %f %d", newCard->username, newCard->password, &newCard->balance, &newCard->paymentPin) != EOF)
		{
			insertEnd(card, newCard);
		}
		else
		{
			free(newCard->username);
			free(newCard->password);
			free(newCard); // 仅在这里释放newCard，而不是调用freeCard(&newCard)
			break;
		}
	}
	fclose(file);
}

// 将链表信息存储到文件
void saveCardToFile(Card* card)
{
	FILE* file = fopen("account.txt", "w");
	if (file == NULL)
	{
		perror("Error opening file");
		return;
	}
	Card* current = card;
	while (current != NULL)
	{
		fprintf(file, "%s %s %.2f %d", current->username, current->password, current->balance, current->paymentPin);
		fprintf(file, "\n");
		current = current->next;
	}
	fclose(file);
}

// 判断用户名的合法性
enum usernameValidationStatus isUsernameValid(const char* username)
{
	if (username[0] == '\0')
	{
		return UsernameEmpty;
	}
	size_t len = strlen(username);
	if (len > MaxUsernameLength)
	{
		return UsernameInvalidLength;
	}
	bool alpha = false;
	bool digit = false;
	for (int i = 0; i < len; i++)
	{
		if (isalpha(username[i]))
		{
			alpha = true;
		}
		if (isdigit(username[i]))
		{
			digit = true;
		}
		if (!isalnum(username[i]) && username[i] != ' ')
		{
			return UsernameInvalidCharacter;
		}
	}
	if (alpha && digit)
	{
		return UsernameValid;
	}
	else
	{
		return UsernameRequiresAlphaAndDigit;
	}
}

// 判断密码的合法性
enum passwordValidationStatus isPasswordValid(const char* password)
{
	if (password[0] == '\0')
	{
		return PasswordEmpty;
	}
	size_t len = strlen(password);
	if (len < MinPasswordLength)
	{
		return PasswordInvalidLengthLower;
	}
	if (len > MaxPasswordLength)
	{
		return PasswordInvalidLengthUpper;
	}
	bool alpha = false;
	bool digit = false;
	bool specialCharacter = false;
	for (int i = 0; i < len; i++)
	{
		if (isalpha(password[i]))
		{
			alpha = true;
		}
		if (isdigit(password[i]))
		{
			digit = true;
		}
		if (!isalnum(password[i]) && password[i] != ' ')
		{
			specialCharacter = true;
		}
	}
	if (alpha && digit && specialCharacter)
	{
		return PasswordValid;
	}
	else
	{
		return PasswordRequiresAlphaDigitPunct;
	}
}

// 判断支付密码的合法性
bool isPaymentPinValid(const int paymentPin)
{
	if (paymentPin < 100000 || paymentPin > 999999)
	{
		return false;
	}
	return true;
}

// 用户名输入函数
void inputUsername(char* username)
{
	while (true)
	{
		scanf("%10s", username);
		clearInputBuffer();
		int usernameValid = isUsernameValid(username);
		if (usernameValid == UsernameValid)
		{
			break;
		}
		if (usernameValid == UsernameEmpty)
		{
			printf("用户名不能为空，请重新输入：");
			continue;
		}
		if (usernameValid == UsernameInvalidLength)
		{
			printf("用户名最多10位，请重新输入：");
			continue;
		}
		if (usernameValid == UsernameInvalidCharacter)
		{
			printf("输入非法字符，请重新输入：");
			continue;
		}
		if (usernameValid == UsernameRequiresAlphaAndDigit)
		{
			printf("用户名必须包括字母和数字，请重新输入：");
			continue;
		}
	}
}

// 密码输入函数
void inputPassword(char* password)
{
	while (true)
	{
		scanf("%20s", password);
		clearInputBuffer();
		int passwordValid = isPasswordValid(password);
		if (passwordValid == PasswordValid)
		{
			break;
		}
		if (passwordValid == PasswordEmpty)
		{
			printf("密码不能为空，请重新输入：");
			continue;
		}
		if (passwordValid == PasswordInvalidLengthLower)
		{
			printf("密码不能低于八位，请重新输入：");
			continue;
		}
		if (passwordValid == PasswordInvalidLengthUpper)
		{
			printf("密码不能超过20位，请重新输入：");
			continue;
		}
		if (passwordValid == PasswordRequiresAlphaDigitPunct)
		{
			printf("密码必须包括数字、字母和特殊字符，请重新输入：");
			continue;
		}
	}
}

// 支付密码输入函数
void inputPaymentPin(int* paymentPin)
{
	while (true)
	{
		scanf("%d", paymentPin);
		clearInputBuffer();
		if (isPaymentPinValid(*paymentPin))
		{
			break;
		}
		else
		{
			printf("支付密码需要六位，请重新输入：");
			continue;
		}
	}
}

// 显示一级菜单
void mainMenu(void)
{
	printf("1. 创建账户\n");
	printf("2. 登录\n");
	printf("3. 退出\n");
	printf("请选择一个数字：");
}

// 显示二级菜单
void secondMenu(void)
{
	printf("1. 查看余额\n");
	printf("2.存款\n");
	printf("3. 取款\n");
	printf("4. 修改密码\n");
	printf("5. 修改支付密码\n");
	printf("6. 退出登录\n");
	printf("7. 注销账户\n");
	printf("请选择一个数字：");
}

// 创建账户
void createAccount(Card** card)
{
	Card* user = (Card*)malloc(sizeof(Card));
	user->username = (char*)malloc(MaxUsernameLength * sizeof(char));
	user->password = (char*)malloc(MaxPasswordLength * sizeof(char));
	Card* tCard = *card;
	printf("请输入用户名，需要包括字母和数字：");
	inputUsername(user->username);
	printf("请输入密码，需要包括字母、数字和特殊字符：");
	inputPassword(user->password);
	printf("请设置初始金额：");
	scanf("%f", &user->balance);
	clearInputBuffer();
	printf("请设置支付密码：");
	inputPaymentPin(&user->paymentPin);
	while (tCard != NULL)
	{
		if (isMatch(tCard, user))
		{
			printf("账户已经存在\n");
			if (user->username != NULL)
			{
				free(user->username);
			}
			if (user->password != NULL)
			{
				free(user->password);
			}
			free(user);
			return;
		}
		tCard = tCard->next;
	}
	printf("注册成功\n");
	insertEnd(card, user);
}

// 登录
Card* loginAccount(Card** card)
{
	Card* user = (Card*)malloc(sizeof(Card));
	user->username = (char*)malloc(MaxUsernameLength * sizeof(char));
	user->password = (char*)malloc(MaxPasswordLength * sizeof(char));
	Card* tCard = *card;
	printf("请输入用户名：");
	inputUsername(user->username);
	printf("请输入密码：");
	inputPassword(user->password);
	while (tCard != NULL)
	{
		if (isMatch(tCard, user))
		{
			printf("登陆成功\n");
			user->balance = tCard->balance;
			user->paymentPin = tCard->paymentPin;
			user->next = NULL;
			free(user->username);
			free(user->password);
			return user;
		}
		tCard = tCard->next;
	}
	printf("账户不存在\n");
	if (user->username != NULL)
	{
		free(user->username);
	}
	if (user->password != NULL)
	{
		free(user->password);
	}
	free(user);
	return NULL;
}

// 查看余额
void viewBalance(Card* user)
{
	printf("你的余额为： %.2f\n", user->balance);
}

// 存款
void deposit(Card** card, Card* user)
{
	float balance;
	int paymentPin;
	printf("请输入支付密码：");
	inputPaymentPin(&paymentPin);
	printf("请输入存入金额：");
	scanf("%f", &balance);
	clearInputBuffer();
	printf("存款成功\n");
	deleteAccount(card, user);
	user->balance = user->balance + balance;
	insertEnd(card, user);
}

// 取款
void withdrawal(Card** card, Card* user)
{
	float balance;
	int paymentPin;
	printf("请输入支付密码：");
	inputPaymentPin(&paymentPin);
	printf("请输入取款金额：");
	while (true)
	{
		scanf("%f", &balance);
		clearInputBuffer();
		if (user->balance - balance < 0)
		{
			printf("你的金额不足，你还剩 %f元， 请重新输入：", user->balance);
			continue;
		}
		else
		{
			break;
		}
	}
	deleteAccount(card, user);
	user->balance = user->balance - balance;
	insertEnd(card, user);
}

// 修改密码
void changePassword(Card** card, Card* user)
{
	char* password = (char*)malloc(MaxPasswordLength * sizeof(char));
	char* oldPassword = (char*)malloc(MaxPasswordLength * sizeof(char));
	printf("请输入原始密码：");
	while (true)
	{
		inputPassword(oldPassword);
		if (strcmp(oldPassword, user->password) == 0)
		{
			break;
		}
		else
		{
			printf("你输入的原始密码不正确，请重新输入：");
			continue;
		}
	}
	printf("请输入新密码：");
	inputPassword(password);
	deleteAccount(card, user);
	free(oldPassword);
	strcpy(user->password, password);
	insertEnd(card, user);
	free(password);
}

// 修改支付密码
void changeSecurePassword(Card** card, Card* user)
{
	int paymentPin;
	printf("请输入原始支付密码：");
	inputPaymentPin(&user->paymentPin);
	printf("请输入新的支付密码：");
	inputPaymentPin(&paymentPin);
	deleteAccount(card, user);
	user->paymentPin = paymentPin;
	insertEnd(card, user);
}

// 退出系统
void logoutAccount(Card** card, Card** user)
{
	saveCardToFile(*card);
	free(*user);
	*user = NULL;
	printf("退出系统成功，欢迎下次继续使用\n");
}

// 删除账户
void removeAccount(Card** card, Card** user)
{
	deleteAccount(card, *user);
	printf("账号删除成功\n");
	free(*user);
	*user = NULL;
	saveCardToFile(*card);
}

int main(void)
{
	Card* cardList = NULL;
	Card* currentUser = NULL;
	bool isRunning = true;
	int choice;
	while (isRunning)
	{
		mainMenu();
		scanf("%d", &choice);
		clearInputBuffer();
		switch (choice)
		{
		case 1:
			createAccount(&cardList);
			break;
		case 2:
			currentUser = loginAccount(&cardList);
			if (currentUser != NULL)
			{
				bool isLoggedIn = true;
				while (isLoggedIn)
				{
					secondMenu();
					scanf("%d", &choice);
					clearInputBuffer();
					switch (choice)
					{
					case 1:
						viewBalance(currentUser);
						break;
					case 2:
						deposit(&cardList, currentUser);
						break;
					case 3:
						withdrawal(&cardList, currentUser);
						break;
					case 4:
						changePassword(&cardList, currentUser);
						break;
					case 5:
						changeSecurePassword(&cardList, currentUser);
						break;
					case 6:
						logoutAccount(&cardList, &currentUser);
						isLoggedIn = false;
						break;
					case 7:
						removeAccount(&cardList, &currentUser);
						isLoggedIn = false;
						break;
					default:
						printf("无效的选项，请重新选择。\n");
						break;
					}
				}
			}
			break;
		case 3:
			isRunning = false;
			saveCardToFile(cardList);
			printf("谢谢使用，再见！\n");
			break;
		default:
			printf("无效的选项，请重新选择。\n");
			break;
		}
	}
	if (currentUser != NULL)
	{
		free(currentUser->username);
		free(currentUser->password);
		free(currentUser);
	}
	if (cardList != NULL)
	{
		freeCard(&cardList);
	}
	return 0;
}