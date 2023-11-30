#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
struct Account
{
	char username[11];
	char password[21];
	float balance;
	int securePassword;
};

struct Account* acc_t = NULL;

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

bool isValidUsername(char* username)
{
	bool hasDigit = false;
	bool hasAlpha = false;
	size_t len = strlen(username);
	if (len > 10)
	{
		printf("�û������10λ������������\n");
		return false;
	}
	for (int i = 0; i < len; i++)
	{
		if (isdigit(username[i]))
		{
			hasDigit = true;
		}
		if (isalpha(username[i]))
		{
			hasAlpha = true;
		}
	}
	return hasAlpha && hasDigit;
}

bool isValidPassword(char* password)
{
	bool hasDigit = false;
	bool hasAlpha = false;
	bool hasPunct = false;
	size_t len = strlen(password);
	if (len < 8)
	{
		printf("���벻������8λ������������\n");
		return false;
	}
	else if ( len > 20)
	{
		printf("���벻�ܴ���20λ������������\n");
		return false;
	}
	for (int i = 0; i < len; i++)
	{
		if (isdigit(password[i]))
		{
			hasDigit = true;
		}
		if (isalpha(password[i]))
		{
			hasAlpha = true;
		}
		if (ispunct(password[i]))
		{
			hasPunct = true;
		}
	}
	return hasDigit && hasAlpha && hasPunct;
}

void updateAccountInFile(struct Account* acc)
{
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

bool isLogin(struct Account* acc)
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

void createAccount()
{
	struct Account* acc;
	acc = (struct Account*)malloc(sizeof(struct Account));
	if (acc == NULL)
	{
		printf("�ڴ����ʧ��\n");
		return;
	}
	struct Account* acc_temp;
	acc_temp = (struct Account*)malloc(sizeof(struct Account));
	if (acc_temp == NULL)
	{
		printf("�ڴ����ʧ��\n");
		free(acc);
		return;
	}
	char line[1024];
	FILE* account = fopen("account.txt", "a+");
	if (account == NULL)
	{
		printf("�ļ���ʧ��\n");
		free(acc);
		free(acc_temp);
		return;
	}
	if (isLogin(acc_t))
	{
		printf("�˻��Ѿ���¼�����ܼ�������\n");
		free(acc);
		free(acc_temp);
		return;
	}
	bool isUsernameValid = true;
	int c;
	printf("�������û��������10λ��ֻ�ܰ�����ĸ�����֣�");
	do
	{
		fgets(acc->username, sizeof(acc->username), stdin);
		acc->username[strcspn(acc->username, "\n")] = 0;
		rewind(account);
		bool userExists = false;
		while (fgets(line, sizeof(line), account) != NULL)
		{
			if (sscanf(line, "%s %s %f %d", acc_temp->username, acc_temp->password, &acc_temp->balance, &acc_temp->securePassword) == 4)
			{
				if (strcmp(acc->username, acc_temp->username) == 0)
				{
					printf("�˻��Ѿ����ڣ�����������\n");
					userExists = true;
					break;
				}
			}
		}
		if (userExists)
		{
			continue;
		}
		if (!isValidUsername(acc->username))
		{
			printf("��������Ƿ��ַ������������룺");
			continue;
		}
		isUsernameValid = false;
	} while (isUsernameValid);
	bool isPasswordValid = true;
	printf("���������룬���20λ����Ҫ�������֡���ĸ�������ַ���");
	do
	{
		fgets(acc->password, sizeof(acc->password), stdin);
		acc->password[strcspn(acc->password, "\n")] = 0;
		if (!isValidPassword(acc->password))
		{
			printf("��������Ƿ��ַ������������룺");
			continue;
		}
		isPasswordValid = false;
	} while (isPasswordValid);
	printf("�������ʼ��");
	scanf("%f", &acc->balance);
	while ((c = getchar()) != '\n' && c != EOF);
	bool isSecurePasswordValid = true;
	printf("������֧�����룬Ӧ������λ��");
	do
	{
		scanf("%d", &acc->securePassword);
		while ((c = getchar()) != '\n' && c != EOF);
		bool isAllDigits = true;
		int temp = acc->securePassword;
		while (temp != 0)
		{
			if (temp % 10 < 0 || temp % 10 > 9)
			{
				isAllDigits = false;
				break;
			}
			temp = temp / 10;
		}
		if (!isAllDigits)
		{
			printf("֧������Ӧ��ֻ�������֣����������룺");
			continue;
		}
		if (countDigits(acc->securePassword) != 6)
		{
			printf("���Ϸ���֧������Ӧ������λ�����������룺");
		}
		isSecurePasswordValid = false;
	} while (isSecurePasswordValid);
	printf("�˻������ɹ�\n");
	fprintf(account, "%s %s %.2f %d\n", acc->username, acc->password, acc->balance, acc->securePassword);
	free(acc);
	free(acc_temp);
	fclose(account);
}

void loginAccount(struct Account* acc)
{
	char username[11];
	char password[21];
	bool found = false;
	acc = (struct Account*)malloc(sizeof(struct Account));
	if (acc == NULL)
	{
		printf("�ڴ����ʧ��\n");
		return;
	}
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("�ļ���ʧ��\n");
		free(acc);
		return;
	}
	if (isLogin(acc_t))
	{
		printf("�Ѿ���¼�����ܵ�¼�ڶ���\n");
		free(acc);
		return;
	}
	printf("�������û�����");
	fgets(username, sizeof(username), stdin);
	username[strcspn(username, "\n")] = 0;
	while (fscanf(account, "%s %s %f %d", acc->username, acc->password, &acc->balance, &acc->securePassword) != EOF)
	{
		if (strcmp(acc->username, username) == 0)
		{
			found = true;
			printf("���������룺");
			scanf("%20s", password);
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			if (strcmp(acc->password, password) == 0)
			{
				printf("��¼�ɹ�\n");
				acc_t = acc;
				free(acc);
				return;
			}
			else
			{
				printf("�������\n");
				free(acc);
				break;
			}
		}
	}
	if (!found)
	{
		printf("�˻������ڣ�����ע��\n");
		free(acc);
		return;
	}
	fclose(account);
}

void viewBalance(struct Account* acc)
{
	if (isLogin(acc_t))
	{
		printf("��Ľ��Ϊ��%.2f\n", acc->balance);
	}
	else
	{
		printf("��û�е�¼�����ȵ�¼\n");
		return;
	}
}

void deposit(struct Account* acc)
{
	float balance;
	int securePassword;
	if (!isLogin(acc_t))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	bool isSecurePasswordValid = true;
	printf("������֧�����룺");
	do
	{
		scanf("%d", &securePassword);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		if (securePassword != acc_t->securePassword)
		{
			printf("�����������������\n");
		}
		else
		{
			isSecurePasswordValid = false;
		}
	} while (isSecurePasswordValid);
	printf("���������");
	scanf("%f", &balance);
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	acc_t->balance = acc_t->balance + balance;
	updateAccountInFile(acc_t);
	printf("���ɹ�\n");
}

void withdrawal(struct Account* acc)
{
	float balance;
	int securePassword;
	if (!isLogin(acc_t))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	bool isSecurePasswordValid = true;
	printf("������֧�����룺");
	do
	{
		scanf("%d", &securePassword);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		if (securePassword != acc_t->securePassword)
		{
			printf("�����������������\n");
		}
		else
		{
			isSecurePasswordValid = false;
		}
	} while (isSecurePasswordValid);
	bool isBalanceValid = true;
	printf("������ȡ���");
	do
	{
		scanf("%f", &balance);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		if (acc_t->balance - balance < 0)
		{
			printf("���㣬���������룺");
		}
		else
		{
			acc_t->balance = acc_t->balance - balance;
			updateAccountInFile(acc_t);
			printf("ȡ��ɹ�\n");
			isBalanceValid = false;
		}
	} while (isBalanceValid);
}

void changePassword(struct Account* acc)
{
	char password[21];
	bool isPasswordValid = true;
	if (!isLogin(acc_t))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	printf("�������޸ĵ����룺");
	do
	{
		scanf("%20s", password);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		if (strcmp(acc_t->password, password) == 0)
		{
			printf("�����Ѿ����ڣ����������룺");
		}
		else
		{
			strcpy(acc_t->password, password);
			updateAccountInFile(acc_t);
			printf("�����޸ĳɹ�\n");
			isPasswordValid = false;
		}
	} while (isPasswordValid);
}

void changeSecurePassword(struct Account* acc)
{
	int securePassword;
	bool isSecurePasswordValid = true;
	if (!isLogin(acc_t))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	printf("�������޸ĵ�֧�����룺");
	do
	{
		scanf("%d", &securePassword);
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
		if (countDigits(securePassword) != 6)
		{
			printf("���벻�Ϸ���Ӧ������λ\n");
		}
		else
		{
			acc_t->securePassword = securePassword;
			updateAccountInFile(acc_t);
			printf("֧�������޸ĳɹ�\n");
			isSecurePasswordValid = false;
		}
	} while (isSecurePasswordValid);
}

void logoutAccount(void)
{
	if (!isLogin(acc_t))
	{
		printf("�˻�û�е�¼\n");
		return;
	}
	acc_t = NULL;
	printf("���ѳɹ�ע��\n");
}

void removeAccount(void)
{
	if (!isLogin(acc_t))
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
	struct Account* acc;
	acc = (struct Account*)malloc(sizeof(struct Account));
	while (fgets(line, sizeof(line), account) != NULL)
	{
		if (sscanf(line, "%s %s %f %d", acc->username, acc->password, &acc->balance, &acc->securePassword) == 4)
		{
			if (strcmp(acc->username, acc_t->username) != 0)
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
		printf("�˻��ѳɹ�ɾ����\n");
	}
	else
	{
		remove("temp.txt");
		printf("û���ҵ�Ҫɾ�����˻���\n");
	}
	free(acc);
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
		printf("�ڴ����ʧ��\n");
		return;
	}
	do
	{
		displayMenu();
		if (scanf("%d", &choice) != 1)
		{
			printf("��Ч���룬������һ������\n");
			while ((c = getchar()) != '\n' && c != EOF);
			continue;
		}
		if (choice < 1 || choice > 10)
		{
			printf("��Χ����ȷ����ȷ�ķ�ΧӦ�ý���1-10֮�䣬����������\n");
			continue;
		}
		while ((c = getchar()) != '\n' && c != EOF);
		switch (choice)
		{
		case 1:
			createAccount();
			break;
		case 2:
			loginAccount(acc);
			break;
		case 3:
			viewBalance(acc_t);
			break;
		case 4:
			deposit(acc_t);
			break;
		case 5:
			withdrawal(acc_t);
			break;
		case 6:
			changePassword(acc_t);
			break;
		case 7:
			changeSecurePassword(acc_t);
			break;
		case 8:
			logoutAccount();
			break;
		case 9:
			removeAccount();
			break;
		case 10:
			if (acc_t != NULL)
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
	free(acc);
	return 0;
}