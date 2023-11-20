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
				_bool = false;
			}
		}
	} while (_bool);
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
				_bool = false;
			}
		}
	} while (_bool);
	printf("�������ʼ��");
	scanf("%f", acc->balance);
	printf("������֧�����룺");
	do
	{
		scanf("%d", acc->securePassword);
		if (sizeof(acc->securePassword) != 6)
		{
			printf("���Ϸ���֧������Ӧ������λ�����������룺");
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
		printf("�������û�����");
		scanf("%s", username);
		printf("���������룺");
		scanf("%s", password);
		if (strcmp(acc->username, username) != 0 || strcmp(acc->password, password) != 0)
		{
			printf("�û����������������������\n");
		}
		else
		{
			printf("�ɹ���¼\n");
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
	printf("��Ľ��Ϊ��%f\n", *acc->balance);
}

void deposit(struct Account* acc)
{
	float balance;
	FILE* account = fopen("account.txt", "w");
	if (account != NULL)
	{
		fscanf(account, "%s %s %f %d", acc->username, acc->password, acc->balance, acc->securePassword);
	}
	printf("���������");
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
	printf("������֧�����룺");
	do
	{
		scanf("%d", &securePassword);
		if (securePassword != *acc->securePassword)
		{
			printf("�����������������\n");
		}
		else
		{
			_bool = false;
		}
	} while (_bool);
	printf("������ȡ���");
	do
	{
		scanf("%f", &balance);
		if (*acc->balance - balance < 0)
		{
			printf("���㣬���������룺");
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
	printf("�������޸ĵ����룺");
	do
	{
		scanf("%s", &password);
		if (strcmp(acc->password, &password) == 0)
		{
			printf("�����Ѿ����ڣ�����������\n");
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
	printf("�������޸ĵ�֧�����룺");
	do
	{
		scanf("%d", &securePassword);
		if (*acc->securePassword == securePassword)
		{
			printf("�����Ѿ����ڣ�����������\n");
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
			printf("�ټ���\n");
			break;
		default:
			printf("���������룺\n");
		}
	} while (choice != 10);
	memoryFree(account);
	return 0;
}