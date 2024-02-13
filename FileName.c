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

// ����һ���洢�˻���Ϣ������
typedef struct Card
{
	char* username;
	char* password;
	float balance;
	int paymentPin;
	struct Card* next;
} Card;

// �û�����֤״̬
enum usernameValidationStatus
{
	UsernameValid = 0,
	UsernameEmpty = 1,
	UsernameInvalidLength = 2,
	UsernameInvalidCharacter = 3,
	UsernameRequiresAlphaAndDigit = 4
};

// ������֤״̬
enum  passwordValidationStatus
{
	PasswordValid = 0,
	PasswordEmpty = 1,
	PasswordInvalidLengthLower = 2,
	PasswordInvalidLengthUpper = 3,
	PasswordRequiresAlphaDigitPunct = 4
};

// �ȽϺ���
int isMatch(Card* card, Card* newCard)
{
	return strcmp(card->username, newCard->username) == 0 && strcmp(card->password, newCard->password) == 0;
}

// ������뻺����ֱ���������л�EOF
void clearInputBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// �������п�����
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

// ������β�������˺�
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

// ɾ�������е��˺�
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

// �ͷ��˻�����
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

// ���ļ��е��˺Ŷ�������
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
			free(newCard); // ���������ͷ�newCard�������ǵ���freeCard(&newCard)
			break;
		}
	}
	fclose(file);
}

// ��������Ϣ�洢���ļ�
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

// �ж��û����ĺϷ���
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

// �ж�����ĺϷ���
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

// �ж�֧������ĺϷ���
bool isPaymentPinValid(const int paymentPin)
{
	if (paymentPin < 100000 || paymentPin > 999999)
	{
		return false;
	}
	return true;
}

// �û������뺯��
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
			printf("�û�������Ϊ�գ����������룺");
			continue;
		}
		if (usernameValid == UsernameInvalidLength)
		{
			printf("�û������10λ�����������룺");
			continue;
		}
		if (usernameValid == UsernameInvalidCharacter)
		{
			printf("����Ƿ��ַ������������룺");
			continue;
		}
		if (usernameValid == UsernameRequiresAlphaAndDigit)
		{
			printf("�û������������ĸ�����֣����������룺");
			continue;
		}
	}
}

// �������뺯��
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
			printf("���벻��Ϊ�գ����������룺");
			continue;
		}
		if (passwordValid == PasswordInvalidLengthLower)
		{
			printf("���벻�ܵ��ڰ�λ�����������룺");
			continue;
		}
		if (passwordValid == PasswordInvalidLengthUpper)
		{
			printf("���벻�ܳ���20λ�����������룺");
			continue;
		}
		if (passwordValid == PasswordRequiresAlphaDigitPunct)
		{
			printf("�������������֡���ĸ�������ַ������������룺");
			continue;
		}
	}
}

// ֧���������뺯��
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
			printf("֧��������Ҫ��λ�����������룺");
			continue;
		}
	}
}

// ��ʾһ���˵�
void mainMenu(void)
{
	printf("1. �����˻�\n");
	printf("2. ��¼\n");
	printf("3. �˳�\n");
	printf("��ѡ��һ�����֣�");
}

// ��ʾ�����˵�
void secondMenu(void)
{
	printf("1. �鿴���\n");
	printf("2.���\n");
	printf("3. ȡ��\n");
	printf("4. �޸�����\n");
	printf("5. �޸�֧������\n");
	printf("6. �˳���¼\n");
	printf("7. ע���˻�\n");
	printf("��ѡ��һ�����֣�");
}

// �����˻�
void createAccount(Card** card)
{
	Card* user = (Card*)malloc(sizeof(Card));
	user->username = (char*)malloc(MaxUsernameLength * sizeof(char));
	user->password = (char*)malloc(MaxPasswordLength * sizeof(char));
	Card* tCard = *card;
	printf("�������û�������Ҫ������ĸ�����֣�");
	inputUsername(user->username);
	printf("���������룬��Ҫ������ĸ�����ֺ������ַ���");
	inputPassword(user->password);
	printf("�����ó�ʼ��");
	scanf("%f", &user->balance);
	clearInputBuffer();
	printf("������֧�����룺");
	inputPaymentPin(&user->paymentPin);
	while (tCard != NULL)
	{
		if (isMatch(tCard, user))
		{
			printf("�˻��Ѿ�����\n");
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
	printf("ע��ɹ�\n");
	insertEnd(card, user);
}

// ��¼
Card* loginAccount(Card** card)
{
	Card* user = (Card*)malloc(sizeof(Card));
	user->username = (char*)malloc(MaxUsernameLength * sizeof(char));
	user->password = (char*)malloc(MaxPasswordLength * sizeof(char));
	Card* tCard = *card;
	printf("�������û�����");
	inputUsername(user->username);
	printf("���������룺");
	inputPassword(user->password);
	while (tCard != NULL)
	{
		if (isMatch(tCard, user))
		{
			printf("��½�ɹ�\n");
			user->balance = tCard->balance;
			user->paymentPin = tCard->paymentPin;
			user->next = NULL;
			free(user->username);
			free(user->password);
			return user;
		}
		tCard = tCard->next;
	}
	printf("�˻�������\n");
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

// �鿴���
void viewBalance(Card* user)
{
	printf("������Ϊ�� %.2f\n", user->balance);
}

// ���
void deposit(Card** card, Card* user)
{
	float balance;
	int paymentPin;
	printf("������֧�����룺");
	inputPaymentPin(&paymentPin);
	printf("����������");
	scanf("%f", &balance);
	clearInputBuffer();
	printf("���ɹ�\n");
	deleteAccount(card, user);
	user->balance = user->balance + balance;
	insertEnd(card, user);
}

// ȡ��
void withdrawal(Card** card, Card* user)
{
	float balance;
	int paymentPin;
	printf("������֧�����룺");
	inputPaymentPin(&paymentPin);
	printf("������ȡ���");
	while (true)
	{
		scanf("%f", &balance);
		clearInputBuffer();
		if (user->balance - balance < 0)
		{
			printf("��Ľ��㣬�㻹ʣ %fԪ�� ���������룺", user->balance);
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

// �޸�����
void changePassword(Card** card, Card* user)
{
	char* password = (char*)malloc(MaxPasswordLength * sizeof(char));
	char* oldPassword = (char*)malloc(MaxPasswordLength * sizeof(char));
	printf("������ԭʼ���룺");
	while (true)
	{
		inputPassword(oldPassword);
		if (strcmp(oldPassword, user->password) == 0)
		{
			break;
		}
		else
		{
			printf("�������ԭʼ���벻��ȷ�����������룺");
			continue;
		}
	}
	printf("�����������룺");
	inputPassword(password);
	deleteAccount(card, user);
	free(oldPassword);
	strcpy(user->password, password);
	insertEnd(card, user);
	free(password);
}

// �޸�֧������
void changeSecurePassword(Card** card, Card* user)
{
	int paymentPin;
	printf("������ԭʼ֧�����룺");
	inputPaymentPin(&user->paymentPin);
	printf("�������µ�֧�����룺");
	inputPaymentPin(&paymentPin);
	deleteAccount(card, user);
	user->paymentPin = paymentPin;
	insertEnd(card, user);
}

// �˳�ϵͳ
void logoutAccount(Card** card, Card** user)
{
	saveCardToFile(*card);
	free(*user);
	*user = NULL;
	printf("�˳�ϵͳ�ɹ�����ӭ�´μ���ʹ��\n");
}

// ɾ���˻�
void removeAccount(Card** card, Card** user)
{
	deleteAccount(card, *user);
	printf("�˺�ɾ���ɹ�\n");
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
						printf("��Ч��ѡ�������ѡ��\n");
						break;
					}
				}
			}
			break;
		case 3:
			isRunning = false;
			saveCardToFile(cardList);
			printf("ллʹ�ã��ټ���\n");
			break;
		default:
			printf("��Ч��ѡ�������ѡ��\n");
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