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

// ����һ���洢�˻���Ϣ������
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
	// �û�����֤״̬
	UsernameValid = 0,
	UsernameEmpty = 1,
	UsernameInvalidLength = 2,
	UsernameInvalidCharacter = 3,
	UsernameRequiresAlphaAndDigit = 4,
	// ������֤״̬
	PasswordValid = 10,
	PasswordEmpty = 11,
	PasswordInvalidLength = 12,
	PasswordInvalidCharacter = 13,
	PasswordRequiresAlphaDigitPunct = 14
};

// ���˻��ļ�
FILE* openAccountFile(char* mode)
{
	FILE* file = fopen("account.txt", mode);
	if (file == NULL)
	{
		perror("�ļ���ʧ��");
	}
	return file;
}

// ���ļ��ж�ȡ�˻���Ϣ
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

// ���ļ���д���˻���Ϣ
void writeAccountToFile(FILE* file, const struct account* acc)
{
	fprintf(file, "%s %s %.2f %d\n", acc->username, acc->password, acc->balance, acc->paymentPin);
}

// �����ļ��е��˻���Ϣ
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

// ���ļ���ɾ���˻���Ϣ
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

// ��������������ַ��Ƿ�Ϸ�
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

// ��������������ַ��������Ƿ���ָ���ķ�Χ��
bool isLengthValid(char* str, size_t minLen, size_t maxLen)
{
	size_t len = strlen(str);
	return len >= minLen && len <= maxLen;
}

// ����û����Ƿ�Ϸ�
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
			return UsernameInvalidCharacter; // �����Ƿ��ַ�
		}
	}
	if (hasAlpha && hasDigit)
	{
		return UsernameValid; // �û�����Ч
	}
	else
	{
		return UsernameRequiresAlphaAndDigit; // �û�����Ҫ������ĸ������
	}
}

// ��֤�����Ƿ�Ϸ�
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
			return PasswordInvalidCharacter; // �����Ƿ��ַ�
		}
	}
	if (hasAlpha && hasDigit && hasPunct)
	{
		return PasswordValid; // ������Ч
	}
	else
	{
		return PasswordRequiresAlphaDigitPunct; // ������Ҫ������ĸ�����ֺ������ַ�
	}
}

// ����һ���ж�֧��������ȷ�Եĺ���
bool isValidSecurePassword(const int* password)
{
	// ������볤���Ƿ�Ϊ6λ
	if (strlen(password) != 6)
	{
		return false;
	}
	// ���������е�ÿ���ַ�
	for (int i = 0; i < 6; i++)
	{
		// �����ǰ�ַ��������֣��򷵻�false
		if (!isdigit((unsigned char)password[i]))
		{
			return false;
		}
	}
	// ���м�鶼ͨ���ˣ�����true
	return true;
}

// ��ȡ�û����벢ȥ�����з�
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

// ����˻��Ƿ����
bool accountExists(const char input[], FILE* accountFile)
{
	struct account tempAccount;
	rewind(accountFile); // ȷ�����ļ���ͷ��ʼ
	while (readAccountFromFile(accountFile, &tempAccount))
	{
		if (strcmp(input, tempAccount.username) == 0)
		{
			return true; // �ҵ�ƥ����˻���
		}
	}
	return false; // û���ҵ�ƥ����˻���
}

// �����û�������������벢������֤
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

// ����һ�����ղ���֤�û����ĺ���
void checkAndReadUsername(char username[MaxUsernameLength + 1], FILE* accountFile)
{
	char* usernameErrors[] = { "", "�û�������Ϊ�գ����������룺", "�û������10λ������������", "����Ƿ��ַ�������������", "�û�����Ҫ������ĸ�����֣�����������" };
	checkAndReadInput(username, sizeof(username), accountFile, (int (*)(const char*))isValidUsername, "�������û��������10λ��ֻ�ܰ�����ĸ�����֣�", usernameErrors);
}

// ����һ�����ղ���֤����ĺ���
void checkAndReadPassword(char password[MaxPasswordLength + 1])
{
	char* passwordErrors[] = { "", "���벻��Ϊ�գ����������룺", "���벻�ܴ���20λ�����������룺", "����Ƿ��ַ������������룺", "������������ĸ�����ֺ������ַ������������룺" };
	checkAndReadInput(password, sizeof(password), NULL, (int (*)(const char*))isValidPassword, "���������룬���20λ����Ҫ�������֡���ĸ�������ַ���", passwordErrors);
}

void inputNumber(void* number, const char* prompt, const char* format)
{
	int scanResult;
	int extraChar;
	do
	{
		printf("%s", prompt); // ��ʾ��ʾ��Ϣ
		scanResult = scanf(format, number); // ���ݸ�ʽ�ַ�����ȡ����
		if (scanResult != 1)
		{
			printf("��Ч���룬������һ�����֣�"); // ���������Ч����ʾ������Ϣ
			while ((extraChar = getchar()) != '\n' && extraChar != EOF); // ������뻺����
			continue;
		}
		while ((extraChar = getchar()) != '\n' && extraChar != EOF); // ������뻺����
	} while (scanResult != 1);
}

// �ж��û��Ƿ��Ѿ���¼
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

// �����û�����
void userChoice(int* choice)
{
	char discardChar;
	if (scanf("%d", choice) != 1)
	{
		printf("��Ч���룬������һ������\n");
		while ((discardChar = getchar()) != '\n' && discardChar != EOF);
		return;
	}
	if (*choice < 1 || *choice > 10)
	{
		printf("��Χ����ȷ����ȷ�ķ�ΧӦ�ý���1-10֮�䣬����������\n");
		while ((discardChar = getchar()) != '\n' && discardChar != EOF);
		return;
	}
	while ((discardChar = getchar()) != '\n' && discardChar != EOF);
}

// ��ʾ���˵�
void displayMenu(void)
{
	printf("1. �����˻�\n");
	printf("2. ��¼\n");
	printf("3. �鿴���\n");
	printf("4. ���\n");
	printf("5. ȡ��\n");
	printf("6. �޸�����\n");
	printf("7. �޸�֧������\n");
	printf("8. �˳���¼\n");
	printf("9. ע���˻�\n");
	printf("10. �˳�\n");
	printf("��ѡ��һ�����֣�");
}

// �����˻�
void createAccount(struct account** accountPtr)
{
	if (isLogin(accountPtr))
	{
		printf("�˻��Ѿ���¼�����ܼ�������\n");
		return;
	}
	FILE* account = openAccountFile("a+");
	char username[MaxUsernameLength + 1];
	char password[MaxPasswordLength + 1];
	float balance;
	int paymentPin;
	checkAndReadUsername(username, account);
	checkAndReadPassword(password);
	inputNumber(&balance, "�������ʼ��", "%f");
	while (true)
	{
		inputNumber(&paymentPin, "������֧�����룬Ӧ������λ��", "%d");
		if (isValidSecurePassword(&paymentPin))
		{
			break;
		}
	}
	printf("�˻������ɹ�\n");
	fprintf(account, "%s %s %.2f %d\n", username, password, balance, paymentPin);
	fclose(account);
}

// ��¼
void loginAccount(struct account** accountPtr)
{
	if (isLogin(accountPtr))
	{
		printf("�Ѿ���¼�����ܵ�¼�ڶ���\n");
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
		printf("��¼�ɹ�\n");
		*accountPtr = localAccount;
	}
	else
	{
		printf("�û������������\n");
		free(localAccount);
	}
	fclose(account);
}

// �鿴���
void viewBalance(struct account** accountPtr)
{
	if (isLogin(accountPtr))
	{
		printf("��Ľ��Ϊ��%.2f\n", (*accountPtr)->balance);
	}
	else
	{
		printf("��û�е�¼�����ȵ�¼\n");
		return;
	}
}

// ���
void deposit(struct account** accountPtr)
{
	float balance;
	int paymentPin;
	if (!isLogin(accountPtr))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	inputNumber(&paymentPin, "������֧�����룺", "d");
	inputNumber(&balance, "���������", "f");
	(*accountPtr)->balance = (*accountPtr)->balance + balance;
	updateAccountInFile(*accountPtr);
	printf("���ɹ�\n");
}

// ȡ��
void withdrawal(struct account** accountPtr)
{
	if (!isLogin(accountPtr))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	float balance;
	int paymentPin;
	inputNumber(&paymentPin, "������֧�����룺", "d");
	while (true)
	{
		inputNumber(&balance, "������ȡ����", "f");
		if ((*accountPtr)->balance - balance < 0)
		{
			printf("���㣬�������%f�����������룺", (*accountPtr)->balance);
			continue;
		}
		else
		{
			(*accountPtr)->balance = (*accountPtr)->balance - balance;
			updateAccountInFile(*accountPtr);
			printf("ȡ��ɹ�\n");
			return;
		}
	}
}

// �޸�����
void changePassword(struct account** accountPtr)
{
	char password[MaxPasswordLength + 1];
	if (!isLogin(accountPtr))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	printf("�����������룺");
		checkAndReadPassword(password);
		strcpy((*accountPtr)->password, password);
		updateAccountInFile(*accountPtr);
		printf("�����޸ĳɹ�\n");
		return;
}

// �޸�֧������
void changeSecurePassword(struct account** accountPtr)
{
	int paymentPin;
	if (!isLogin(accountPtr))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	inputNumber(&paymentPin, "�����������룺", "d");
			(*accountPtr)->paymentPin = paymentPin;
			updateAccountInFile(*accountPtr);
			printf("֧�������޸ĳɹ�\n");
			return;
}

// �˳�ϵͳ
void logoutAccount(struct account** accountPtr)
{
	if (!isLogin(accountPtr))
	{
		printf("�˻�û�е�¼\n");
		return;
	}
	free(*accountPtr);
	*accountPtr = NULL;
	printf("���ѳɹ�ע��\n");
}

// ɾ���˻�
void removeAccount(struct account** accountPtr)
{
	if (!isLogin(accountPtr))
	{
		printf("û�е�¼�����ȵ�¼\n");
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
				printf("�㻹û��ע��������ע��\n");
				break;
			}
			else
			{
				printf("�ټ���\n");
				running = false;
			}
			break;
		default:
			printf("���������룺\n");
		}
	} while (running);
	return 0;
}