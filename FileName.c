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
	acc = (struct Account*)malloc(sizeof(struct Account));
	acc->username = (char*)malloc(10 * sizeof(char));
	acc->password = (char*)malloc(20 * sizeof(char));
	acc->balance = (float*)malloc(sizeof(float));
	acc->securePassword = (int*)malloc(6);
	printf("�������û�����");
	scanf("%s", acc->username);
	printf("���������룺");
	scanf("%s", acc->password);
	printf("�������ʼ��");
	scanf("%f", acc->balance);
	printf("������֧�����룺");
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
			printf("�ټ���\n");
			break;
		default:
			printf("���������룺\n");
		}
	} while (choice != 8);
	return 0;
}