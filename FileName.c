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

void createAccount(struct Account* acc)
{
	printf("�������û�����ֻ�ܰ�����ĸ�����֣�");
	bool boolUsername = true;
	do
	{
		scanf("%s", acc->username);
		if (strlen(acc->username) > 10)
		{
			printf("�û������10λ�����������룺");
		}
		else
		{
			if (!isalpha(*acc->username) && !isdigit(*acc->username))
			{
				printf("��������Ƿ��ַ������������룺");
			}
			else
			{
				boolUsername = false;
			}
		}
	} while (boolUsername);
	bool boolPassword = true;
	printf("���������룬������20λ����Ҫ�������֡���ĸ�������ַ���");
	do
	{
		scanf("%s", acc->password);
		if (strlen(acc->password) > 20)
		{
			printf("�������20λ�����������룺");
		}
		else
		{
			if (!isalpha(*acc->password) && !isdigit(*acc->password) && !ispunct(*acc->password))
			{
				printf("��������Ƿ��ַ������������룺");
			}
			else
			{
				boolPassword = false;
			}
		}
	} while (boolPassword);
	printf("�������ʼ��");
	scanf("%f", acc->balance);
	bool boolSecurePassword = true;
	printf("������֧�����룺");
	do
	{
		scanf("%d", acc->securePassword);
		if (countDigits(*acc->securePassword) != 6)
		{
			printf("���Ϸ���֧������Ӧ������λ�����������룺");
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
		printf("�������û�����");
		scanf("%s", username);
		while (fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword) != EOF)
		{
			if (strcmp(acc->username, username) == 0)
			{
				printf("���������룺");
				scanf("%s", password);
				if (strcmp(acc->password, password) == 0)
				{
					printf("��¼�ɹ�\n");
					acc_t = *acc;
					return 1;
				}
			}
			else
			{
				printf("�˻������ڣ�����ע��\n");
				return 0;
			}
		}
	}
	fclose(account);
}

void viewBalance()
{
	printf("��Ľ��Ϊ��%.2f\n", *acc_t.balance);
}

void deposit()
{
	float balance;
	printf("���������");
	scanf("%f", &balance);
	*acc_t.balance = *acc_t.balance + balance;
}

void withdrawal()
{
	float balance;
	int securePassword;
	bool boolSecurePassword = true;
	printf("������֧�����룺");
	do
	{
		scanf("%d", &securePassword);
		if (securePassword != *acc_t.securePassword)
		{
			printf("�����������������\n");
		}
		else
		{
			boolSecurePassword = false;
		}
	} while (boolSecurePassword);
	bool boolBalance = true;
	printf("������ȡ���");
	do
	{
		scanf("%f", &balance);
		if (*acc_t.balance - balance < 0)
		{
			printf("���㣬���������룺");
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
	printf("�������޸ĵ����룺");
	do
	{
		scanf("%s", password);
		if (strcmp(acc_t.password, password) == 0)
		{
			printf("�����Ѿ����ڣ����������룺");
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
	printf("�������޸ĵ�֧�����룺");
	do
	{
		scanf("%d", securePassword);
		if (acc_t.securePassword == securePassword)
		{
			printf("�����Ѿ����ڣ�����������\n");
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
				printf("û�е�¼�����ȵ�¼\n");
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
			printf("�ټ���\n");
			break;
		default:
			printf("���������룺\n");
		}
	} while (choice != 10);
	memoryFree(account);
	return 0;
}