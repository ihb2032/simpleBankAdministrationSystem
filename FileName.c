#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
// ����һ���洢�˻���Ϣ�Ľṹ��
struct Account
{
	char username[11];
	char password[21];
	float balance;
	int securePassword;
};

// ����һ���ж��û�����ȷ�Եĺ���
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

// ����һ���ж�������ȷ�Եĺ���
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

// ����һ���ж�֧��������ȷ�Եĺ���
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

// ����һ�������û�������ĺ���
void getUsername(char username[11])
{
	fgets(username, 11, stdin);
	username[strcspn(username, "\n")] = 0;
}

// ����һ�������û���������ĺ���
void getPassword(char password[21])
{
	fgets(password, 21, stdin);
	password[strcspn(password, "\n")] = 0;
}

// ����һ�������û�������ĺ���
void getBalance(float* balance)
{
	int c;
	scanf("%f", balance);
	while ((c = getchar()) != '\n' && c != EOF);
}

// ����һ�������û�����֧������ĺ���
void getSecurePassword(int* securePassword)
{
	int c;
	scanf("%d", securePassword);
	while ((c = getchar()) != '\n' && c != EOF);
}

// ����һ�������ļ���Ϣ�ĺ���
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

// �ж��û��Ƿ��Ѿ���¼
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
void createAccount(struct Account** acc)
{
	if (isLogin(acc))
	{
		printf("�˻��Ѿ���¼�����ܼ�������\n");
		return;
	}
	struct Account* acc_temp;
	acc_temp = (struct Account*)malloc(sizeof(struct Account));
	if (acc_temp == NULL)
	{
		printf("�ڴ����ʧ��\n");
		return;
	}
	char line[1024];
	FILE* account = fopen("account.txt", "a+");
	if (account == NULL)
	{
		printf("�ļ���ʧ��\n");
		free(acc_temp);
		return;
	}
	char username[11];
	char password[21];
	float balance;
	int securePassword;
	printf("�������û��������10λ��ֻ�ܰ�����ĸ�����֣�");
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
					printf("�˻��Ѿ����ڣ�����������\n");
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
			printf("�û������10λ������������\n");
			break;
		case 2:
			printf("����Ƿ��ַ�������������\n");
			break;
		case 3:
			printf("�û�����Ҫ������ĸ�����֣�����������\n");
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
	printf("���������룬���20λ����Ҫ�������֡���ĸ�������ַ���");
	while (true)
	{
		getPassword(password);
		int passwordValidationResult = isValidPassword(password);
		switch (passwordValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("���벻�����ڰ�λ������������\n");
			break;
		case 2:
			printf("���벻�ܴ���20λ������������\n");
			break;
		case 3:
			printf("����Ƿ��ַ�������������\n");
			break;
		case 4:
			printf("������������ĸ�����ֺ������ַ�������������\n");
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
	printf("�������ʼ��");
	getBalance(&balance);
	printf("������֧�����룬Ӧ������λ��");
	while (true)
	{
		getSecurePassword(&securePassword);
		if (!isValidSecurePassword(securePassword))
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
	fprintf(account, "%s %s %.2f %d\n", username, password, balance, securePassword);
	free(acc_temp);
	fclose(account);
}

// ��¼
void loginAccount(struct Account** acc)
{
	if (isLogin(acc))
	{
		printf("�Ѿ���¼�����ܵ�¼�ڶ���\n");
		return;
	}
	char username[11];
	char password[21];
	struct Account* acc_local = malloc(sizeof(struct Account));
	FILE* account = fopen("account.txt", "r");
	if (account == NULL)
	{
		printf("�ļ���ʧ��\n");
		return;
	}
	printf("�������û�����");
	while (true)
	{
		getUsername(username);
		int usernameValidationResult = isValidUsername(username);
		switch (usernameValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("�û������10λ������������\n");
			break;
		case 2:
			printf("����Ƿ��ַ�������������\n");
			break;
		case 3:
			printf("�û�����Ҫ������ĸ�����֣�����������\n");
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
		printf("�˻�������\n");
		break;
	}
	printf("���������룺");
	while (true)
	{
		getPassword(password);
		int passwordValidationResult = isValidPassword(password);
		switch (passwordValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("���벻�����ڰ�λ������������\n");
			break;
		case 2:
			printf("���벻�ܴ���20λ������������\n");
			break;
		case 3:
			printf("����Ƿ��ַ�������������\n");
			break;
		case 4:
			printf("������������ĸ�����ֺ������ַ�������������\n");
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
			printf("�����������������\n");
			continue;
		}
		else
		{
			break;
		}
	}
	printf("��¼�ɹ�\n");
	*acc = acc_local;
	updateAccountInFile(acc_local);
	fclose(account);
}

// �鿴���
void viewBalance(struct Account** acc)
{
	if (isLogin(acc))
	{
		printf("��Ľ��Ϊ��%.2f\n", (*acc)->balance);
	}
	else
	{
		printf("��û�е�¼�����ȵ�¼\n");
	}
	return;
}

// ���
void deposit(struct Account** acc)
{
	float balance;
	int securePassword;
	if (!isLogin(acc))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	printf("������֧�����룺");
	while (true)
	{
		getSecurePassword(&securePassword);
		if (!isValidSecurePassword(securePassword))
		{
			printf("֧�������������λ������������\n");
			continue;
		}
		if (securePassword != (*acc)->securePassword)
		{
			printf("�����������������\n");
			continue;
		}
		else
		{
			break;
		}
	}
	printf("���������");
	getBalance(&balance);
	(*acc)->balance = (*acc)->balance + balance;
	updateAccountInFile(*acc);
	printf("���ɹ�\n");
}

// ȡ��
void withdrawal(struct Account** acc)
{
	if (!isLogin(acc))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	float balance;
	int securePassword;
	printf("������֧�����룺");
	while (true)
	{
		getSecurePassword(&securePassword);
		if (!isValidSecurePassword(securePassword))
		{
			printf("֧�������������λ������������\n");
			continue;
		}
		if (securePassword != (*acc)->securePassword)
		{
			printf("�����������������\n");
			continue;
		}
		else
		{
			break;
		}
	}
	printf("������ȡ���");
	while (true)
	{
		getBalance(&balance);
		if ((*acc)->balance - balance < 0)
		{
			printf("���㣬�������%f�����������룺", balance);
			continue;
		}
		else
		{
			(*acc)->balance = (*acc)->balance - balance;
			updateAccountInFile(*acc);
			printf("ȡ��ɹ�\n");
			return;
		}
	}
}

// �޸�����
void changePassword(struct Account** acc)
{
	char password[21];
	if (!isLogin(acc))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	printf("�����������룺");
	while (true)
	{
		getPassword(password);
		int passwordValidationResult = isValidPassword(password);
		switch (passwordValidationResult)
		{
		case 0:
			break;
		case 1:
			printf("���벻�����ڰ�λ������������\n");
			break;
		case 2:
			printf("���벻�ܴ���20λ������������\n");
			break;
		case 3:
			printf("����Ƿ��ַ�������������\n");
			break;
		case 4:
			printf("������������ĸ�����ֺ������ַ�������������\n");
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
			printf("�����޸ĳɹ�\n");
			return;
		}
	}
}

// �޸�֧������
void changeSecurePassword(struct Account** acc)
{
	int securePassword;
	if (!isLogin(acc))
	{
		printf("û�е�¼�����ȵ�¼\n");
		return;
	}
	printf("�������޸ĵ�֧�����룺");
	while (true)
	{
		getSecurePassword(&securePassword);
		if (!isValidSecurePassword(securePassword))
		{
			printf("֧�������������λ������������\n");
			continue;
		}
		else
		{
			(*acc)->securePassword = securePassword;
			updateAccountInFile(*acc);
			printf("֧�������޸ĳɹ�\n");
			return;
		}
	}
}

// �˳�ϵͳ
void logoutAccount(struct Account** acc)
{
	if (!isLogin(acc))
	{
		printf("�˻�û�е�¼\n");
		return;
	}
	free(*acc);
	acc = NULL;
	printf("���ѳɹ�ע��\n");
}

// ɾ���˻�
void removeAccount(struct Account** acc)
{
	if (!isLogin(acc))
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
		printf("�˻��ѳɹ�ɾ����\n");
		free(*acc);
		return;
	}
	else
	{
		remove("temp.txt");
		printf("û���ҵ�Ҫɾ�����˻���\n");
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
		printf("�ڴ����ʧ��\n");
		return 1;
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