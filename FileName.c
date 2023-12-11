#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
// ����һ���洢�˻���Ϣ�Ľṹ��
struct account
{
	char username[11];
	char password[21];
	float balance;
	int paymentPin;
};

// ����һ���ж��û�����ȷ�Եĺ���
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

// ����һ���ж�������ȷ�Եĺ���
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

// ����һ���ж�֧��������ȷ�Եĺ���
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

// �����û�������������벢������֤
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
					printf("�˻��Ѿ����ڣ�����������\n");
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

// ����һ�����ղ���֤�û����ĺ���
void checkAndReadUsername(char username[11], FILE* accountFile)
{
	char* usernameErrors[] = { "", "�û������10λ������������\n", "����Ƿ��ַ�������������\n", "�û�����Ҫ������ĸ�����֣�����������\n" };
	checkAndReadInput(username, sizeof(username), accountFile, isValidUsername, "�������û��������10λ��ֻ�ܰ�����ĸ�����֣�", usernameErrors);
}

// ����һ�����ղ���֤����ĺ���
void checkAndReadPassword(char password[21])
{
	char* passwordErrors[] = { "", "���벻�����ڰ�λ������������\n", "���벻�ܴ���20λ������������\n", "����Ƿ��ַ�������������\n", "������������ĸ�����ֺ������ַ�������������\n" };
	checkAndReadInput(password, sizeof(password), NULL, isValidPassword, "���������룬���20λ����Ҫ�������֡���ĸ�������ַ���", passwordErrors);
}

// ����һ�������û�������ĺ���
void inputBalance(float* balance)
{
	int scanResult;
	int extraChar;
	do
	{
		scanResult = scanf("%f", balance);
		if (scanResult != 1)
		{
			printf("��Ч���룬������һ�����֣�");
		}
		while ((extraChar = getchar()) != '\n' && extraChar != EOF);
	} while (scanResult != 1);
}

// ����һ�������û�����֧������ĺ���
void inputSecurePassword(int* paymentPin)
{
	int scanResult;
	int extraChar;
	do
	{
		scanResult = scanf("%d", paymentPin);
		if (scanResult != 1)
		{
			printf("��Ч���룬������һ�����֣�\n");
		}
		while ((extraChar = getchar()) != '\n' && extraChar != EOF);
	} while (scanResult != 1);
}

// ����һ�������ļ���Ϣ�ĺ���
void updateAccountInFile(struct account* accountPtr)
{
	struct account* tempAccount = malloc(sizeof(struct account));
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("�޷����˻��ļ���\n");
		free(tempAccount);
		return;
	}
	FILE* temp = fopen("temp.txt", "w");
	if (temp == NULL)
	{
		printf("�޷�������ʱ�ļ���\n");
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

bool verifyPaymentPin(const struct account* accountPtr)
{
	int paymentPin;
	printf("������֧�����룺");
	inputSecurePassword(&paymentPin);
	if (!isValidSecurePassword(paymentPin))
	{
		printf("֧�������������λ������������\n");
		return false;
	}
	if (paymentPin != accountPtr->paymentPin)
	{
		printf("֧�������������������\n");
		return false;
	}
	return true;
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
	FILE* account = fopen("account.txt", "a+");
	if (account == NULL)
	{
		printf("�ļ���ʧ��\n");
		return;
	}
	char username[11];
	char password[21];
	float balance;
	int paymentPin;
	checkAndReadUsername(username, account);
	checkAndReadPassword(password);
	printf("�������ʼ��");
	inputBalance(&balance);
	printf("������֧�����룬Ӧ������λ��");
	while (true)
	{
		inputSecurePassword(&paymentPin);
		if (!isValidSecurePassword(paymentPin))
		{
			printf("֧�������������λ������������\n");
			continue;
		}
		else
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
	char username[11];
	char password[21];
	struct account* localAccount = malloc(sizeof(struct account));
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("�ļ���ʧ��\n");
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
			printf("�û�������\n");
			fclose(account);
			continue;
		}
	}
	while (true)
	{
		checkAndReadPassword(password);
		if (strcmp(password, localAccount->password) != 0)
		{
			printf("�����������������\n");
			continue;
		}
		else
		{
			printf("��¼�ɹ�\n");
			*accountPtr = localAccount;
			updateAccountInFile(localAccount);
			fclose(account);
			return;
		}
	}
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
	if (!isLogin(accountPtr))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	if (!verifyPaymentPin(*accountPtr))
	{
		return;
	}
	printf("���������");
	inputBalance(&balance);
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
	if (!verifyPaymentPin(*accountPtr))
	{
		return;
	}
	printf("������ȡ���");
	while (true)
	{
		inputBalance(&balance);
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
	char password[21];
	if (!isLogin(accountPtr))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	printf("�����������룺");
	while (true)
	{
		checkAndReadPassword(password);
		strcpy((*accountPtr)->password, password);
		updateAccountInFile(*accountPtr);
		printf("�����޸ĳɹ�\n");
		return;
	}
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
	printf("�������޸ĵ�֧�����룺");
	while (true)
	{
		inputSecurePassword(&paymentPin);
		if (!isValidSecurePassword(paymentPin))
		{
			printf("֧�������������λ������������\n");
			continue;
		}
		else
		{
			(*accountPtr)->paymentPin = paymentPin;
			updateAccountInFile(*accountPtr);
			printf("֧�������޸ĳɹ�\n");
			return;
		}
	}
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
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("�޷����˻��ļ���\n");
		return;
	}
	FILE* temp = fopen("temp.txt", "w");
	if (temp == NULL)
	{
		printf("�޷�������ʱ�ļ���\n");
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
		printf("�˻��ѳɹ�ɾ��\n");
		free(*accountPtr);
		*accountPtr = NULL;
		fclose(account);
		fclose(temp);
		return;
	}
	else
	{
		remove("temp.txt");
		printf("û���ҵ�Ҫɾ�����˻�\n");
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
	free(currentAccount);
	return 0;
}