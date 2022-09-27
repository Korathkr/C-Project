#pragma comment(lib, "libmysql.lib") // �����ͺ��̽��� ����ϱ� ���� mysql ���̺귯�� ���� ȣ��
#include <stdio.h> // ǥ�� ������Լ� printf(), scanf()�� ����ϱ� ���� ����
#include <stdlib.h> // system()�Լ��� ����ϱ� ���� ����
#include <mysql.h> // �����ͺ��̽��� ����ϱ� ���� ����
#include <time.h> // srand(time)�Լ��� ����ϱ� ���� ����
// ����ӵ� ������ �ϴ� clock()�� ����ϱ� ���� ����
#include <Windows.h> // gotoxy(), Sleep(), removeCursor() �Լ��� ����ϱ� ���� ����
#include <conio.h> // ��ǥ�� ������Լ� _kbhit() �Լ��� ����ϱ� ���� ����
#include <string.h> // strcmp() �Լ��� ����ϱ� ���� ����

// �����ͺ��̽� �ʿ�
#define DB_HOST "127.0.0.1" // ������ �ּ� / �ڱ� ���� �ּ�
#define DB_USER "root"		// ���̵�
#define DB_PASS "1234"	// ��й�ȣ desctop 1234
#define DB_NAME "login"		// �����ͺ��̽� �̸�
#define CHOP(x) x[strlen(x) - 1] = ' '	// ����Ű�� ������ �� �迭�� ������ ���� ���� �Լ�

// �����ʿ�
#define wordmax 103 // �ܾ��� ����
#define wordtime 20000 // �ܾ �ߴ� �ӵ�

void gotoxy(int x, int y) { // Ŀ�� ��ġ ���� �Լ�
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void removeCursor() { // �ֿܼ��� Ŀ�� �����ϴ� �Լ�
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void help();// ����
void gamemain();// ���Ӹ���
void word_io();// �ܾ� ����� ���ִ� �Լ�
void design();// �������̽� ȭ��
void rank(); // ��ŷ ȭ��
void menu();// ���� ���� �ʱ� ȭ��
void setting(); // �ð� ���� �޴�
void removeCursor();// printf���� �ܾ �߰��Ҷ� Ŀ�� ����
int database(); // �����ͺ��̽� �Լ�

// DB�� ���� ����
int query_stat; // ������ ����� �Ѿ���� ����üũ
char ans; // �α��� Y/N ���� Ȯ��
char id[20]; // ���̵� �Է� 
char pass[20]; // ��й�ȣ �Է�
char check[255]; // ���̵�, ��й�ȣ üũ�� ������ ������ ���� �ӽù��� ����
char query[255]; // ���̵�, ��й�ȣ�� ������ ������ ���� �ӽù��� ����
char query1[255]; // ȸ�����Խ� ���̵�� ������ ������ ������ ���� �ӽù��� ����
char query2[255]; // �ְ� ������ ����ϱ� ���� ���̵�� ������ ������ ������ ���� �ӽù��� ����
char query3[255]; // �α����� ���� �� ������ �ҷ����� ���� �ӽù��� ���� 
int dbscore; // DB�� ����� ����
int dbtimer; // DB�� ����� �ð�
MYSQL* connection = NULL, conn; // ���� ���� ����
MYSQL_RES* sql_result; // ��� ���� �޸𸮿� ����
MYSQL_ROW sql_row; // �� ���� �ҷ�����
int count = 1; //�α��� ȭ���� �� ���� �߰� �ϱ� ���� ����

// ���� �ʿ� ���� 
int gap; // ���� �ð� ī����
int wordi[100];//�� �� ��µ� �ܾ���� �ٽ� ����� �ȵǰ� �ϱ� ���� ����
int x, y;// gotoxy �� x���� y��
int i;// �ܾ� ��
int x2, y2;// x,y ���� �ٽ� �ҷ��� ����
int score;// ���������� 0���� �ʱ�ȭ
int h;// �ʱ�ȭ�鿡�� �޴������� �Է� ���� ����
int f;// ����
int f2;// ���� ���� �ٽ� �ҷ��� ����
int sword; // scan ������ �ε��� ���� ������Ű�� ���� ����
char scan[50]; // Ű����� ���� �ܾ �Է¹��� ����
char remem[wordmax][10]; // ������ ������ �� �ܾ �ӽ� �����ϰ� ����ϱ� ���� 2���� �迭
int timer = 40;// ������ ���� �ð��� �����ϴ� ���� (�ʱⰪ : 40��)
time_t startTime, endTime;// ���� �ð� ����
// �ܾ� �� �� 103��
// 2���� �迭 word row = Wordmax(103), col = 10
// row�� ���� �� 103���� �ܾ ���� ������ 103���� ����
// �ܾ��� �ִ� ���� ���� 10�������� ���ڿ��� ���� \0�� ���� ������ col���� 10���� ����
char word[wordmax][11] = { "dog", "cat", "bottle", "phone", "robot", "green", "elephant", "include", "sky", "game", "greenday", "kimchanho", "max",

"knife", "glass", "class", "art", "smart", "bell", "carry", "climb", "between", "blow", "album", "ralo", "among", "animal", "any", "box",

"and", "board", "body", "monsrat", "classmate", "city", "boy", "bridge", "clean",

"club", "coat", "bright", "coin", "chopstick", "coffee", "cold", "chance", "chalk", "chair", "cheap", "blue",

"before", "bowl", "aunt", "as", "away", "bicycle", "paka", "card", "hold",

"chose", "come", "drink", "give", "get", "hurt", "lay","had", "feed", "lend", "rand", "wsing", "throw", "wet", "tell",

"set", "wind", "wear", "write", "spend", "stand", "worn", "win", "sweep", "account", "achieve", "across", "accept", "above", "ability", "abuse",

"abnormal", "absurd", "acceptance", "according", "absent", "nation", "past", "value", "though", "person", "machine", "stand", "null" };

int main() {
	//�α��� ȭ���� �� ���� �߰� �ϱ� ����
	if (count == 1) { // ���� count�� 1�̸�
		database(); // �����ͺ��̽� �Լ� ȣ��
	sprintf(query3, "select score, time from rank where id = '%s'", id); // query3�� rank ���̺� �ִ� �α����� id�� �������� ������
	query_stat = mysql_query(connection, query3); // query3�� ���������� ���� �Ѿ���� Ȯ��
	sql_result = mysql_store_result(connection); // select score from rank where id = '%s' �޸𸮿� ����

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {	// sql_row�� NULL���� �ƴ� �� ���� ���
			dbscore = atoi(sql_row[0]); // sql_row[0] ���� dbscore�� ����
			dbtimer = atoi(sql_row[1]); // sql_row[1] ���� dbtimer�� ����
		}
		mysql_free_result(sql_result); // �޸� ����
	}
	
	srand((unsigned)time(NULL));	// ���� �Լ�
	system("mode con cols=120 lines=30"); // ȭ�� ������ ����
	design(); // �������̽� ȭ�� �Լ� ȣ��
	Sleep(1500); // �������̽� ȭ�� ��� �� 1.5�� ������
	gotoxy(60, 9);  // x : 60, y : 9�� Ŀ���̵�
	removeCursor(); // Ŀ���̵�

	while (1) { // �޴�ȭ���� ��� ����ϱ� ���� �ݺ���
		menu(); // �޴�ȭ�� ���
		gotoxy(70, 19); // x : 70 y : 19�� �Է� â �̵�
		scanf("%d", &h); // �޴� ����
		fflush(stdin);  // ǥ�� �Է�(Ű����) ���۸� ��� ������ �Լ�
		switch (h) { // Switch ���ù����� ����ȭ�� ����
		case 1:
			score = 0; // �������� 0 ���� �ʱ�ȭ
			gap = 0;
			gamemain(); // ���ӽ��� �Լ� ȣ��
			break;// ���� �ʱ�ȭ, ���ӽ��� �Լ� ȣ�� �� ���� ����
		case 2:
			help(); // ���� �Լ� ȣ��
			break; // ���� ȭ�� ���
		case 3:
			rank();// ��ŷ ȭ�� �Լ� ȣ��
			break; // switch�� ������
		case 4:
			setting();// �ð����� �Լ� ȣ��
			break; // switch�� ������
		case 5:
			mysql_close(connection); // mysql ���� ����
			gotoxy(45, 25);
			printf("******������ ����˴ϴ�******");
			_getch();
			return 0; // ��������
		default: // �ٸ� Ű�� �Է� ���� ���
			gotoxy(50, 21); // x : 50, y : 21�� ���â ���
			printf("�ùٸ� Ű�� �ƴմϴ�!"); // ���â ���
			_getch(); // Ű����� ���� �Է� ���
			system("cls"); // ȭ�� ����
			break; // switch�� ������
		}
	}
	return 0;
}

void menu() { // �޴�ȭ�� �Լ�
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ���ڻ��� �ٲٴ� �Լ�, ������� ���
	system("cls"); // ȭ�� �����
	design(); // �������̽� �Լ� ȣ��
	gotoxy(44, 3);
	printf("%s�� �ȳ��ϼ���!!", id);
	gotoxy(44, 5); // x = 44, y = 5 ��ǥ�� ȭ�� ���
	printf("*****************************"); // �޴� ȭ�� ������
	gotoxy(44, 6); // x = 44, y = 6 ��ǥ�� ȭ�� ���
	printf("*********TYPING GAME*********"); // �޴� ȭ�� ������
	gotoxy(44, 7); // x = 44, y = 7 ��ǥ�� ȭ�� ���
	printf("*****************************"); // �޴� ȭ�� ������
	gotoxy(50, 9);  // x = 50, y = 9 ��ǥ�� ȭ�� ���
	printf(" 1. ���ӽ���");// 1. ���ӽ���
	gotoxy(50, 11); // x = 50, y = 11 ��ǥ�� ȭ�� ���
	printf(" 2. ����");  // 2. ����
	gotoxy(50, 13); // x = 50, y = 13 ��ǥ�� ȭ�� ���
	printf(" 3. ��  ŷ");    // 3. ��ŷ
	gotoxy(50, 15); // x = 50, y = 15 ��ǥ�� ȭ�� ���
	printf(" 4. �ð�����"); // 4. �ð����� 
	gotoxy(50, 17); // x = 50, y = 17 ��ǥ�� ȭ�� ���
	printf(" 5. ��������"); // 5. ��������
	gotoxy(45, 19); // x = 50, y = 19 ��ǥ�� ȭ�� ���
	printf("  Please enter the key [   ]");// �޴� ����  
	gotoxy(43, 21); // x = 50, y = 21 ��ǥ�� ȭ�� ���
	printf("[ �����̰����б� 1805040 ���ؿ� ]");
}

void design() { // �������̽� ȭ�� �Լ�

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ���ڻ��� �ٲٴ� �Լ�, ������� ���
	// �������̽� ȭ�� 
	printf("|---------------------------------------------------------------------------------------------------------------------|\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|---------------------------------------------------------------------------------------------------------------------|\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|                                                                                                                     |\n");
	printf("|---------------------------------------------------------------------------------------------------------------------|");
}

void help() { // ����
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ���ڻ��� �ٲٴ� �Լ�, ������� ���
	system("cls"); // ȭ�� �����
	design(); // �������̽� ȭ�� �Լ� ȣ��
	gotoxy(35, 7); // x : 35, y : 7�� ȭ�� ���
	printf("- �������� �ߴ� �ܾ���� �����ּ���!!"); // ȭ�� ���
	gotoxy(35, 9); // x : 35, y : 9�� ȭ�� ���
	printf("- ������ ���� ������ �ٸ��ϴ�!!"); // ȭ�� ���
	gotoxy(35, 11); // x : 35, y : 11�� ȭ�� ���

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1); // ���ڻ��� �ٲٴ� �Լ�, �Ķ������� ���
	printf("�Ķ��� : 150�� "); // ȭ�� ���

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // ���ڻ��� �ٲٴ� �Լ�, �ʷϻ����� ���
	printf("�ʷϻ� : 200�� "); // ȭ�� ���

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3); // ���ڻ��� �ٲٴ� �Լ�, �ϴû����� ���
	printf("�ϴû� : 250�� "); // ȭ�� ���

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // ���ڻ��� �ٲٴ� �Լ�, ���������� ���
	printf("������ : 300�� "); // ȭ�� ���

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ���ڻ��� �ٲٴ� �Լ�, ������� ���
	gotoxy(35, 15); // x : 35, y : 15�� ȭ�� ���
	printf("- �ְ������� ����˴ϴ�."); // ȭ�� ���
	gotoxy(35, 17); // x : 35, y : 16�� ȭ�� ���
	printf("- ���θ޴� ��ŷ���� Ȯ�� �����մϴ�."); // ȭ�� ���
	gotoxy(35, 19); // x : 35, y : 17�� ȭ�� ���
	printf("- �ƹ�Ű�� ������ ���� �޴��� ���ư��ϴ�."); // ȭ�� ���
	_getch(); // Ű����� ���� �Է� ���
}

void rank() { // ��ŷ ȭ�� �Լ� 
	int y = 7; // gotoxy�� y�� 7�� �ʱ�ȭ
	int x = 1; // ������ ��Ÿ���� ���� ���� 1�� �ʱ�ȭ

	system("cls"); // ȭ�� �����
	design(); // �������̽� ȭ�� ���

	query_stat = mysql_query(connection, "select * from rank order by score desc");
	// ���������� ������ DB�� �Ѿ�� "select * from rank order by score dsec"�� ���
	// ������ ������������ ���
	sql_result = mysql_store_result(connection); // select * from rank order by score desc �޸𸮿� ����

	gotoxy(40, 3); // x : 40, y : 3�� ȭ�� ���
	printf("---------------��     ŷ---------------"); // ��ŷ
	gotoxy(48, 5); // x : 48, y : 5�� ȭ�� ���
	printf("���̵�"); // ���̵� ���
	gotoxy(65, 5); // x : 65, y : 5�� ȭ�� ���
	printf("����"); // ���� ���
	gotoxy(72, 5); // x : 65, y : 5�� ȭ�� ���
	printf("�ð�"); // ���� ���
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {	// sql_row�� NULL���� �ƴ� �� ���� ���
		gotoxy(45, y); // x : 45 y : 7�� ȭ�� ���
		printf("%d", x); // ���� ǥ�� 1���� ~
		gotoxy(48, y); // x : 48 y : 8�� ȭ�� ���
		printf("%s", sql_row[0]); // ID ���
		gotoxy(65, y); // x : 65 y : 9�� ȭ�� ���
		printf("%s", sql_row[1]); // ���� ���
		gotoxy(72, y); // x : 70 y : 10�� ȭ�� ���
		printf("%s", sql_row[2]); // ���� ���
		x++; // ���� o ����
		y++; // ���� q ����
	}

	gotoxy(40, 16); // x : 35, y : 16�� ȭ�� ���
	printf("�ƹ�Ű�� ������ ���� �޴��� ���ư��ϴ�."); // ȭ�� ���
	_getch();
	mysql_free_result(sql_result); // �޸� ����
}

void setting() { // �ð� �����ϴ� �Լ�
	system("cls"); // ȭ�� �����
	design(); // �������̽� ȭ�� ȣ��
	gotoxy(48, 10); // x = 48, y = 10 ��ǥ�� ȭ�� ���
	printf("���� ���� �ð� : %d", timer); // ���� �����Ǿ� �ִ� �ð� ���
	gotoxy(48, 12); // x = 48, y = 12 ��ǥ�� ȭ�� ���
	printf("�ð��� �����ϼ��� : "); // �ð� ���� ȭ�� ���
	scanf("%d", &timer); // �ð� �Է� ex) 10, 30, 60, 90�� ������ ���� ����

	system("cls"); // ������ �Ϸ�Ǹ� ȭ�� �����
	design(); // �������̽� ȭ�� ���
	gotoxy(48, 10); // x = 48, y = 10 ��ǥ�� ȭ�� ���
	printf("�ð��� %d �ʷ� �����Ǿ����ϴ�!", timer); // �ð��� �Է��� ������ ���� �Ǿ��ٴ� ȭ�� ���

	gotoxy(48, 12); // x = 48, y = 12 ��ǥ�� ȭ�� ���
	printf("����Ͻ÷��� �ƹ� Ű�� ��������."); // �ƹ� Ű�� ��������
	_getch(); // Ű����� �Է¹�����
}

void gamemain() { // ���ӽ��� �Լ�
	gotoxy(50, 9); // x : 50, y : 9�� ȭ�� ���
	system("cls"); // ȭ�� �����
	design(); // �������̽� ȭ�� �Լ� ȣ��

	gotoxy(50, 9); // x : 50, y : 9�� ȭ�� ���
	printf(" 3�� �� ���� !!"); // ȭ�� ���
	Sleep(1000); // 1�� ��ٸ��ϴ�.

	system("cls"); // ȭ�� �����
	design(); // �������̽� ȭ�� �Լ� ȣ��

	gotoxy(50, 9); // x : 50, y : 9�� ȭ�� ���
	printf(" 2�� �� ���� !!"); // ȭ�� ���
	Sleep(1000); // 1�� ��ٸ��ϴ�.

	system("cls"); // ȭ�� �����
	design(); // �������̽� ȭ�� �Լ� ȣ��

	gotoxy(50, 9); // x : 50, y : 9�� ȭ�� ���
	printf(" 1�� �� ���� !!"); // ȭ�� ���
	Sleep(1000); // 1�� ��ٸ��ϴ�.

	system("cls"); // ȭ�� �����
	startTime = clock(); // �ð� ���� �ʱ�ȭ clock �Լ� ȣ��
	design(); // �������̽� ȭ�� �Լ� ȣ��

	while (timer >= gap) { // ������ ���� �� ���� �ݺ�
		word_io(); // �ܾ �Է��ϴ� �Լ� ȣ��
	}
	if (score > dbscore && timer != dbtimer) { // ���������� dbscore ���� Ŭ ���
		dbscore = score; // ���������� dbscore�� ����
		dbtimer = timer; // �����ð��� dbtimer�� ����
		sprintf(query2, "update rank set score = '%d', time = '%d' where id = '%s'", dbscore, dbtimer, id); 
		// query2�� rank ���̺� ���̵�, ����, �ð� ���� ����
		query_stat = mysql_query(connection, query2); // query2�� ���������� ���� �Ѿ���� Ȯ��
	}
	system("cls"); // ȭ�� �����
	design(); // �������̽� ȭ�� �Լ� ȣ��
	gotoxy(50, 11); // x : 50, y : 11 ��ǥ�� ȭ�� ���
	printf("---�� �� �� ��---"); // ������ ����Ǿ��ٴ� ȭ�� ���
	Sleep(2000); // 2�ʰ� ��ٸ��ϴ�.
}

void word_io() { // �ܾ ������ϴ� �Լ�

	x = rand() % 105 + 5; // �ܾ x�� 5���� 105���� �������� ���
	y = rand() % 19 + 3; // �ܾ y�� 3���� 19���� �������� ���
	i = rand() % wordmax; // 0���� 103(wordmax = 103) ��° �ܾ �������� ���
	f = rand() % 4 + 1; // �ܾ��� ���� �Ķ���, �ʷϻ�, �ϴû�, ���������� ���� �ο�

	if (wordi[i] != 1) { // �ܾ �� ���� �߰� �ϴ� �迭 wordi �ε��� ���� 1�� �ƴ� ���
		wordi[i] = 1; // �迭 wordi�� 1�� ����

		gotoxy(x, y); // �������� ������ x, y������ �ܾ� ���
		for (int j = 0; j < 3; j++) {  // �ݺ����� ����ϱ� ���� j�� �����ϰ� 0���� �ʱ�ȭ, j�� 3���� Ŭ������ �ݺ�, �ݺ��� ������ j 1����
			if (j == 0) { // ���� j�� 0�϶�
				remem[i][j] = x; // �ܾ ����ϱ� ���� �ӽ� 2�����迭 remem�� �ܾ��� x�� �� ����
			}
			else if (j == 1) { // ���� j�� 1�϶�
				remem[i][j] = y; // �ܾ ����ϱ� ���� �ӽ� 2�����迭 remem�� �ܾ��� y�� �� ����
			}
			else if (j == 2) { // ���� j�� 3�϶�
				remem[i][j] = f; // �ܾ ����ϱ� ���� �ӽ� 2���� �迭 remem�� �ܾ��� ���� ���� ����
			}
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), f); // ���ڻ��� �ٲٴ� �Լ�, ���� ����� ���
		printf("%s ", word[i]); //�迭 word�� ����� 0 ~ 103���� �ִ� �ܾ ���
	}

	int p = 0; // �ܾ ����ϱ� ���� ���� p ���� �� 0���� �ʱ�ȭ
	while (p < wordtime) { // p�� wordtime(20000)���� Ŭ ������ �ݺ�
		p++; // p ���� 
		i = 0; // �ܾ� ���� �Է� �ޱ� ���� 0���� �ʱ�ȭ
		int scanc = 0; // Ű���� �Է� ���� ���� ����
		endTime = clock();  // �ڵ尡 ���� �ð� ����
		gap = (double)(endTime - startTime) / (CLOCKS_PER_SEC);// �� ������ �ð��� ����ϱ� ����

		gotoxy(18, 2); // x : 18, y, 2 ��ǥ�� ȭ�� ���
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ���ڻ��� �ٲٴ� �Լ�, ���� ����� ���
		printf("[ ���� �ð� : %d / %d ]", gap, timer); // [ �����ð� : 0 / 40 ] ���� �ð��� �˷��� gqp�� 1�ʸ��� ���� / timer �޴� 4. �ð��������� ���� �� ���

		if (_kbhit()) { // ���� Ű���尡 �Է� �Ǿ��� ���
			scanc = _getch(); // Ű���� �Է� ���� ���� scanc�� ����
			if (scanc != 8 && scanc != 13) { // ���� Ű����κ��� scanc�� ����� Ű ���� �ƽ�Ű �ڵ� �� 8(backspace(<-)) �׸��� 13(carriage rerturn(\r))�� �ƴ� ���
				scan[sword++] = scanc; // Ű���� �Է����� ���� ����� scanc ���� ���ڸ� �Է¹޴� scan[sword == 0] ������ ���� (�ݺ��� ������ �� ���� ����)
									   // ex) Hello �Է� scan[0] = 'H', scan[1] = 'e', scan[2] = 'l', scan[3] = 'l', scan[4] = 'o'
				gotoxy(5, 25); // x = 5, y = 25 ��ǥ�� ȭ�� ���
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // ���ڻ��� �ٲٴ� �Լ�, ������� ���
				printf("%s", scan); // scan������ ����� Ű����� �Էµ� ���� ����� ��
			}
			else if (scanc == 8) { // backspace �Է� �� (���� scanc�� ����� Ű ���� �ƽ�Ű �ڵ� 8(backspace(<-))�� ���)
				gotoxy(1, 25); // x = 1, y = 25 ��ǥ�� ȭ�� ���
				printf("|                                                                                                                     |\n");
				// backspace�� �Է� �� ��� �ܾ ���������� ���������� �ϱ� ���� ���
				gotoxy(5, 25); // x = 5, y = 25 ��ǥ�� ȭ�� ���
				scan[--sword] = scanc; // Ű���� �Է����� ���� ����� scanc ���� ���ڸ� �Է¹޴� scan[sword - 1] ������ ����� ���� ����
									   // ex) Heloo(��Ÿ)�� �Է� scan[4] = 'o' ���� scan[3] = 'o' ����
				if (sword == -1) // ���� sword�� -1�� ���
					sword = 0; // scan ������ �ε��� ���� ��Ÿ���ִ� �����̱� ������ 0���� �ʱ�ȭ �ϸ� �ٽ� scan[0]���� �Է��� ����
				printf("%s", scan); // scan������ ����� Ű����� �Էµ� ���� �������
			}
			else if (scanc == 13) { //���� Ű����κ��� scanc�� ����� Ű ���� �ƽ�Ű�ڵ� �� 13(\r)�� ���
				sword = 0; // scan ������ �ε��� ���� ��Ÿ���ִ� �����̱� ������ 0���� �ʱ�ȭ �ϸ� �ٽ� scan[0]���� �Է��� ����
				gotoxy(1, 25); // x = 1, y = 25 ��ǥ ������ ȭ�� ���
				printf("|                                                                                                                     |\n");
				// \r(�ٹٲ�)�� �Է� �� ��� �ܾ �ٽ� ���������� �Է� �ޱ� ���ؼ� ���
				for (i = 0; i <= wordmax; i++) { // �ܾ� ��(i)�� 0���� �ʱ�ȭ, �ܾ� ��(i)�� �ִ� �ܾ�� wordmax(103)���� ũ�ų� ���� ������ �ܾ� ��(i++)�� �����ϸ鼭 �ݺ�
					if (!strcmp(scan, word[i])) { // word[i]�� �ִ� �ܾ�� Ű����� ���� �Է� �� scan�� ����� ���� ���� ���
						x2 = remem[i][0]; // x ��ǥ�� �ٽ� �ҷ��� ���� x2�� �ӽ� �迭 remen[i][0] ���� ����
						y2 = remem[i][1]; // y ��ǥ�� �ٽ� �ҷ��� ���� x2�� �ӽ� �迭 remen[i][1] ���� ����
						f2 = remem[i][2]; // ���� �ٽ� �ҷ��� ���� f2�� �ӽ� �迭 remen[i][2] ���� ����

						gotoxy(x2, y2); // �ӽ� �迭 remem�� ���� ���� ����� ���� x2, y2�� ���� ���
						printf("          "); // ����� x, y �� ���� ���� ��µ� �ܾ ���� ó���� ����Ͽ� ����
						gotoxy(1, 25); // x = 1, y = 25 ��ǥ�� ȭ�� ���
						printf("|                                                                                                                     |\n");
						// \r(�ٹٲ�)�� �Է� �� �ܾ ���������� �Է¹ޱ� ���ؼ� ���
						switch (f2) { // �ӽ� �迭 remem�� ���� �ǰ� ����� ���� ���� ���� ���� f2�� Switch�� ����
						// ������ f (���� 1 ~ 4�� ���� �ο� ��)
						case 1: score += 150; break; // �Ķ��� ���ڿ� �������� 150�� �ο�
						case 2: score += 200; break; // �ʷϻ� ���ڿ� �������� 200�� �ο�
						case 3: score += 250; break; // �ϴû� ���ڿ� �������� 250�� �ο�
						case 4: score += 300; break; // ������ ���ڿ� �������� 300�� �ο�
						default: break; //Swith�� ��������
						}
						gotoxy(3, 2); // x = 3, y = 2 ��ǥ�� ȭ�� ���
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //���� ���� �ٲ��ִ� �Լ�, ������� ����
						printf("�������� : %d", score); // ���������� ��Ÿ����
					}
				}
				for (i = 0; i < 50; i++) { // i�� 0���� �ʱ�ȭ, i�� 50���� Ŭ������ i�� �����ϸ鼭 �ݺ� 
					scan[i] = NULL; // scan[i]�� ����� ���� Ű���忡 �Էµ� ���� NULLó��
				}
			}
		}
	}
}

int database() { // �����ͺ��̽� �Լ�

	mysql_init(&conn);

	//�ѱ۹��ڼ� ����(��������)
	mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
	mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

	//���� mysql_real_connect
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

	system("mode con: cols=34 lines=20"); // ȭ�� ������ ����

	if (connection == NULL) { // ���� ������ ���� �� 
		fprintf(stderr, "���п��� : %s\n", mysql_error(&conn)); // ���� ���� ���
		return 1; // ������ ����
	}


	query_stat = mysql_query(connection, "select * from login"); // ���������� ������ DB�� �Ѿ�� "select * from login" �� ���
	if (query_stat != 0) {	// ���� query_stat�� 0���� �Ѿ�� ������ ( 0���� �Ѿ�� ���� )
		fprintf(stderr, "���п��� : %s\n", mysql_error(&conn)); // ���� ���� ���
		return 1; // ������ ����
	}
	printf("\t    ȸ�� ���\n"); // ȸ�� ��� ���
	printf("\t  [  ���̵�  ]\n"); // ���̵� ���
	sql_result = mysql_store_result(connection); // select * from login �޸𸮿� ����
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {	// sql_row�� NULL���� �ƴ� �� ���� ���
		printf("\t  %10s\n", sql_row[0]); // �� ������ ���
	}
	mysql_free_result(sql_result); // �޸� ����
	
	while (1) {
		printf("\n\n [ ȸ������(Y) / �α���(N) ] : "); // ȸ������ ���� ���
		scanf("%c", &ans); // Ű����� ���� �Է�
		scanf("%*c"); // �Էµ� ���� ������ �������� �ʰ� ����

		if (ans == 'y' || ans == 'Y') { // ���� ans�� 'y' �̰ų� 'Y' �� ���
			// ȸ������
			system("cls"); // ȭ�� �����

			gotoxy(12, 3); // x : 12, y : 3�� ȭ�� ��� 
			printf("ȸ �� �� ��"); // ȸ������ ȭ�� ���

			gotoxy(3, 5); // x : 3, y : 5�� ȭ�� ���
			printf("���̵� : "); // ���̵� �Է�â ���
			fgets(id, 20, stdin); // ���̵� �Է��� 20 ũ����� �Է��� ����
			CHOP(id); // ����Ű�� ������ �������� ó��

			gotoxy(3, 8); // x : 3, y : 8�� ȭ�� ���
			printf("�н����� : "); // �н����� �Է�â ���
			fgets(pass, 20, stdin); // �н����� �Է��� 20 ũ����� �Է��� ����
			CHOP(pass); // ����Ű�� ������ �������� ó��

			sprintf(query, "insert into login values" "('%s', '%s')", id, pass); //query�� login ���̺� ���̵� �н����� ���� ����
			query_stat = mysql_query(connection, query); // query�� ���������� ���� �Ѿ���� Ȯ��

			sprintf(query1, "insert into rank values" "('%s', '%d', '%d')", id, 0, 0); // query1�� rank ���̺� ���̵�, ����, �ð� ���� ����
			query_stat = mysql_query(connection, query1); // query1�� ���������� ���� �Ѿ���� Ȯ��

			gotoxy(4, 11); // x : 4, y : 11�� ȭ�� ���
			printf("ȸ�������� �Ϸ�Ǿ����ϴ�!"); // ȸ�������� �Ϸ�Ǿ��ٴ� ȭ�� ���
			_getch(); // Ű����� ���� �Է´��
			system("cls"); // ȭ�� �����
		}
		else if (ans == 'N' || ans == 'n') { // ���� ans�� 'n' �̰ų� 'N' �� ���
			//�α���
			system("cls"); // ȭ�� �����

			gotoxy(12, 3); // x : 12, y : 3�� ȭ�� ���
			printf("�� �� ��"); // �� �� �� ȭ�� ���

			gotoxy(3, 5); // x : 3, y : 5�� ȭ�� ���
			printf("���̵� : "); // ���̵� �Է�â ���
			fgets(id, 20, stdin); // ���̵� �Է��� 20 ũ����� �Է��� ����
			CHOP(id); // ����Ű�� ������ �������� ó��

			gotoxy(3, 8); // x : 3, y : 8�� ȭ�� ���
			printf("�н����� : "); // ��й�ȣ �Է�â ���
			fgets(pass, 20, stdin); // �н����� �Է��� 20 ũ����� �Է��� ����
			CHOP(pass); // ����Ű�� ������ �������� ó��

			sprintf(check, "select * from login where id = '%s' and passwd = '%s'", id, pass); // query�� ����� ���̵�� �н����尡 �´��� Ȯ��
			query_stat = mysql_query(connection, check); // query�� ���������� ���� �Ѿ���� Ȯ��
			sql_result = mysql_store_result(connection); //select * from login �޸𸮿� ����

			if ((sql_row = mysql_fetch_row(sql_result)) == NULL) { // ���� sql_row���� NULL�� ���
				system("cls"); // ȭ�� �����
				gotoxy(3, 6); // x : 3, y : 6���� ȭ�� ���
				printf("ȸ�������� ��ġ���� �ʽ��ϴ�."); // ��� ���
				gotoxy(6, 8); // x : 6 y : 8���� ȭ�� ���
				printf("���α׷��� ����˴ϴ�..."); // ���α׷� ����â
				_getch(); // Ű����κ��� �Է� ���
				exit(1); // ������ ����
			}

			system("cls"); // ȭ�� �����
			gotoxy(11, 5); // x : 11, y : 5�� ȭ�� ���
			printf("�α��� ����!!"); // �α��� ���� ���

			gotoxy(9, 7);  // x : 9, y : 7�� ȭ�� ���
			printf("%s�� ȯ���մϴ�!!\n", id); // �α����� ���̵� ���

			gotoxy(2, 9);  // x : 2, y : 9�� ȭ�� ���
			count++; // �α��� ȭ���� �� ���� �߰� �ϱ� ���� count����
			printf("�����Ͻ÷��� �ƹ� Ű�� ��������."); // �ƹ�Ű�� �������� ȭ�� ���
			_getch(); // Ű �Է� ���
			mysql_free_result(sql_result); // �޸� ����
			break; // While�� ����������
		}
	}
	return 0; // �Լ� ����
}