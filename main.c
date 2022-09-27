#pragma comment(lib, "libmysql.lib") // 데이터베이스를 사용하기 위해 mysql 라이브러리 파일 호출
#include <stdio.h> // 표준 입출력함수 printf(), scanf()를 사용하기 위해 선언
#include <stdlib.h> // system()함수를 사용하기 위해 선언
#include <mysql.h> // 데이터베이스를 사용하기 위해 선언
#include <time.h> // srand(time)함수를 사용하기 위해 선언
// 연산속도 측정을 하는 clock()를 사용하기 위해 선언
#include <Windows.h> // gotoxy(), Sleep(), removeCursor() 함수를 사용하기 위해 선언
#include <conio.h> // 비표준 입출력함수 _kbhit() 함수를 사용하기 위해 선언
#include <string.h> // strcmp() 함수를 사용하기 위해 선언

// 데이터베이스 필요
#define DB_HOST "127.0.0.1" // 루프백 주소 / 자기 로컬 주소
#define DB_USER "root"		// 아이디
#define DB_PASS "1234"	// 비밀번호 desctop 1234
#define DB_NAME "login"		// 데이터베이스 이름
#define CHOP(x) x[strlen(x) - 1] = ' '	// 엔터키를 눌렀을 때 배열에 공백이 들어가기 위한 함수

// 게임필요
#define wordmax 103 // 단어의 개수
#define wordtime 20000 // 단어가 뜨는 속도

void gotoxy(int x, int y) { // 커서 위치 조정 함수
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void removeCursor() { // 콘솔에서 커서 삭제하는 함수
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void help();// 도움말
void gamemain();// 게임메인
void word_io();// 단어 입출력 해주는 함수
void design();// 인터페이스 화면
void rank(); // 랭킹 화면
void menu();// 게임 시작 초기 화면
void setting(); // 시간 설정 메뉴
void removeCursor();// printf에서 단어를 뜨게할때 커서 제거
int database(); // 데이터베이스 함수

// DB에 쓰일 변수
int query_stat; // 쿼리가 제대로 넘어갔는지 오류체크
char ans; // 로그인 Y/N 유무 확인
char id[20]; // 아이디 입력 
char pass[20]; // 비밀번호 입력
char check[255]; // 아이디, 비밀번호 체크를 쿼리로 날리기 위해 임시버퍼 선언
char query[255]; // 아이디, 비밀번호를 쿼리로 날리기 위해 임시버퍼 선언
char query1[255]; // 회원가입시 아이디와 점수를 쿼리로 날리기 위해 임시버퍼 선언
char query2[255]; // 최고 점수를 기록하기 위해 아이디와 점수를 쿼리로 날리기 위해 임시버퍼 선언
char query3[255]; // 로그인을 했을 때 점수를 불러오기 위해 임시버퍼 선언 
int dbscore; // DB에 저장될 점수
int dbtimer; // DB에 저장될 시간
MYSQL* connection = NULL, conn; // 연결 정보 관리
MYSQL_RES* sql_result; // 결과 값을 메모리에 저장
MYSQL_ROW sql_row; // 행 단위 불러오기
int count = 1; //로그인 화면을 한 번만 뜨게 하기 위해 선언

// 게임 필요 변수 
int gap; // 게임 시간 카운터
int wordi[100];//한 번 출력된 단어들은 다시 출력이 안되게 하기 위해 선언
int x, y;// gotoxy 의 x값과 y값
int i;// 단어 수
int x2, y2;// x,y 값을 다시 불러올 변수
int score;// 게임점수를 0으로 초기화
int h;// 초기화면에서 메뉴선택을 입력 받을 변수
int f;// 색깔
int f2;// 색깔 값을 다시 불러올 변수
int sword; // scan 변수의 인덱스 값을 증가시키기 위한 변수
char scan[50]; // 키보드로 부터 단어를 입력받을 변수
char remem[wordmax][10]; // 게임을 시작할 때 단어를 임시 저장하고 출력하기 위한 2차원 배열
int timer = 40;// 게임의 남은 시간을 설정하는 변수 (초기값 : 40초)
time_t startTime, endTime;// 게임 시간 제한
// 단어 수 총 103개
// 2차원 배열 word row = Wordmax(103), col = 10
// row의 값은 총 103개의 단어가 들어가기 때문에 103으로 설정
// 단어의 최대 글자 수는 10개이지만 문자열은 끝에 \0이 들어가기 때문에 col값은 10으로 설정
char word[wordmax][11] = { "dog", "cat", "bottle", "phone", "robot", "green", "elephant", "include", "sky", "game", "greenday", "kimchanho", "max",

"knife", "glass", "class", "art", "smart", "bell", "carry", "climb", "between", "blow", "album", "ralo", "among", "animal", "any", "box",

"and", "board", "body", "monsrat", "classmate", "city", "boy", "bridge", "clean",

"club", "coat", "bright", "coin", "chopstick", "coffee", "cold", "chance", "chalk", "chair", "cheap", "blue",

"before", "bowl", "aunt", "as", "away", "bicycle", "paka", "card", "hold",

"chose", "come", "drink", "give", "get", "hurt", "lay","had", "feed", "lend", "rand", "wsing", "throw", "wet", "tell",

"set", "wind", "wear", "write", "spend", "stand", "worn", "win", "sweep", "account", "achieve", "across", "accept", "above", "ability", "abuse",

"abnormal", "absurd", "acceptance", "according", "absent", "nation", "past", "value", "though", "person", "machine", "stand", "null" };

int main() {
	//로그인 화면을 한 번만 뜨게 하기 위해
	if (count == 1) { // 만약 count가 1이면
		database(); // 데이터베이스 함수 호출
	sprintf(query3, "select score, time from rank where id = '%s'", id); // query3에 rank 테이블에 있는 로그인한 id의 점수값을 가져옴
	query_stat = mysql_query(connection, query3); // query3이 정상적으로 값이 넘어갔는지 확인
	sql_result = mysql_store_result(connection); // select score from rank where id = '%s' 메모리에 저장

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {	// sql_row가 NULL값이 아닐 때 까지 출력
			dbscore = atoi(sql_row[0]); // sql_row[0] 값을 dbscore에 저장
			dbtimer = atoi(sql_row[1]); // sql_row[1] 값을 dbtimer에 저장
		}
		mysql_free_result(sql_result); // 메모리 해제
	}
	
	srand((unsigned)time(NULL));	// 랜덤 함수
	system("mode con cols=120 lines=30"); // 화면 사이즈 조절
	design(); // 인터페이스 화면 함수 호출
	Sleep(1500); // 인터페이스 화면 출력 후 1.5초 딜레이
	gotoxy(60, 9);  // x : 60, y : 9로 커서이동
	removeCursor(); // 커서이동

	while (1) { // 메뉴화면을 계속 출력하기 위한 반복문
		menu(); // 메뉴화면 출력
		gotoxy(70, 19); // x : 70 y : 19로 입력 창 이동
		scanf("%d", &h); // 메뉴 선택
		fflush(stdin);  // 표준 입력(키보드) 버퍼를 비워 버리는 함수
		switch (h) { // Switch 선택문으로 설정화면 선택
		case 1:
			score = 0; // 현재점수 0 으로 초기화
			gap = 0;
			gamemain(); // 게임시작 함수 호출
			break;// 점수 초기화, 게임시작 함수 호출 후 게임 시작
		case 2:
			help(); // 도움말 함수 호출
			break; // 도움말 화면 출력
		case 3:
			rank();// 랭킹 화면 함수 호출
			break; // switch문 나가기
		case 4:
			setting();// 시간설정 함수 호출
			break; // switch문 나가기
		case 5:
			mysql_close(connection); // mysql 연결 종료
			gotoxy(45, 25);
			printf("******게임이 종료됩니다******");
			_getch();
			return 0; // 게임종료
		default: // 다른 키를 입력 했을 경우
			gotoxy(50, 21); // x : 50, y : 21로 경고창 출력
			printf("올바른 키가 아닙니다!"); // 경고창 출력
			_getch(); // 키보드로 부터 입력 대기
			system("cls"); // 화면 삭제
			break; // switch문 나가기
		}
	}
	return 0;
}

void menu() { // 메뉴화면 함수
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 글자색을 바꾸는 함수, 흰색으로 출력
	system("cls"); // 화면 지우기
	design(); // 인터페이스 함수 호출
	gotoxy(44, 3);
	printf("%s님 안녕하세요!!", id);
	gotoxy(44, 5); // x = 44, y = 5 좌표로 화면 출력
	printf("*****************************"); // 메뉴 화면 디자인
	gotoxy(44, 6); // x = 44, y = 6 좌표로 화면 출력
	printf("*********TYPING GAME*********"); // 메뉴 화면 디자인
	gotoxy(44, 7); // x = 44, y = 7 좌표로 화면 출력
	printf("*****************************"); // 메뉴 화면 디자인
	gotoxy(50, 9);  // x = 50, y = 9 좌표로 화면 출력
	printf(" 1. 게임시작");// 1. 게임시작
	gotoxy(50, 11); // x = 50, y = 11 좌표로 화면 출력
	printf(" 2. 도움말");  // 2. 도움말
	gotoxy(50, 13); // x = 50, y = 13 좌표로 화면 출력
	printf(" 3. 랭  킹");    // 3. 랭킹
	gotoxy(50, 15); // x = 50, y = 15 좌표로 화면 출력
	printf(" 4. 시간설정"); // 4. 시간설정 
	gotoxy(50, 17); // x = 50, y = 17 좌표로 화면 출력
	printf(" 5. 게임종료"); // 5. 게임종료
	gotoxy(45, 19); // x = 50, y = 19 좌표로 화면 출력
	printf("  Please enter the key [   ]");// 메뉴 선택  
	gotoxy(43, 21); // x = 50, y = 21 좌표로 화면 출력
	printf("[ 영남이공대학교 1805040 김준영 ]");
}

void design() { // 인터페이스 화면 함수

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 글자색을 바꾸는 함수, 흰색으로 출력
	// 인터페이스 화면 
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

void help() { // 도움말
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 글자색을 바꾸는 함수, 흰색으로 출력
	system("cls"); // 화면 지우기
	design(); // 인터페이스 화면 함수 호출
	gotoxy(35, 7); // x : 35, y : 7로 화면 출력
	printf("- 무작위로 뜨는 단어들을 없애주세요!!"); // 화면 출력
	gotoxy(35, 9); // x : 35, y : 9로 화면 출력
	printf("- 색마다 점수 배점이 다릅니다!!"); // 화면 출력
	gotoxy(35, 11); // x : 35, y : 11로 화면 출력

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1); // 글자색을 바꾸는 함수, 파랑색으로 출력
	printf("파랑색 : 150점 "); // 화면 출력

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // 글자색을 바꾸는 함수, 초록색으로 출력
	printf("초록색 : 200점 "); // 화면 출력

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3); // 글자색을 바꾸는 함수, 하늘색으로 출력
	printf("하늘색 : 250점 "); // 화면 출력

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // 글자색을 바꾸는 함수, 빨강색으로 출력
	printf("빨강색 : 300점 "); // 화면 출력

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 글자색을 바꾸는 함수, 흰색으로 출력
	gotoxy(35, 15); // x : 35, y : 15로 화면 출력
	printf("- 최고점수가 저장됩니다."); // 화면 출력
	gotoxy(35, 17); // x : 35, y : 16로 화면 출력
	printf("- 메인메뉴 랭킹에서 확인 가능합니다."); // 화면 출력
	gotoxy(35, 19); // x : 35, y : 17로 화면 출력
	printf("- 아무키나 누르면 메인 메뉴로 돌아갑니다."); // 화면 출력
	_getch(); // 키보드로 부터 입력 대기
}

void rank() { // 랭킹 화면 함수 
	int y = 7; // gotoxy의 y값 7로 초기화
	int x = 1; // 순위를 나타내기 위한 변수 1로 초기화

	system("cls"); // 화면 지우기
	design(); // 인터페이스 화면 출력

	query_stat = mysql_query(connection, "select * from rank order by score desc");
	// 정상적으로 쿼리가 DB로 넘어가면 "select * from rank order by score dsec"문 출력
	// 점수를 내림차순으로 출력
	sql_result = mysql_store_result(connection); // select * from rank order by score desc 메모리에 저장

	gotoxy(40, 3); // x : 40, y : 3로 화면 출력
	printf("---------------랭     킹---------------"); // 랭킹
	gotoxy(48, 5); // x : 48, y : 5로 화면 출력
	printf("아이디"); // 아이디 출력
	gotoxy(65, 5); // x : 65, y : 5로 화면 출력
	printf("점수"); // 점수 출력
	gotoxy(72, 5); // x : 65, y : 5로 화면 출력
	printf("시간"); // 점수 출력
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {	// sql_row가 NULL값이 아닐 때 까지 출력
		gotoxy(45, y); // x : 45 y : 7로 화면 출력
		printf("%d", x); // 순위 표시 1부터 ~
		gotoxy(48, y); // x : 48 y : 8로 화면 출력
		printf("%s", sql_row[0]); // ID 출력
		gotoxy(65, y); // x : 65 y : 9로 화면 출력
		printf("%s", sql_row[1]); // 점수 출력
		gotoxy(72, y); // x : 70 y : 10로 화면 출력
		printf("%s", sql_row[2]); // 점수 출력
		x++; // 변수 o 증감
		y++; // 변수 q 증감
	}

	gotoxy(40, 16); // x : 35, y : 16로 화면 출력
	printf("아무키나 누르면 메인 메뉴로 돌아갑니다."); // 화면 출력
	_getch();
	mysql_free_result(sql_result); // 메모리 해제
}

void setting() { // 시간 설정하는 함수
	system("cls"); // 화면 지우기
	design(); // 인터페이스 화면 호출
	gotoxy(48, 10); // x = 48, y = 10 좌표로 화면 출력
	printf("현재 설정 시간 : %d", timer); // 현재 설정되어 있는 시간 출력
	gotoxy(48, 12); // x = 48, y = 12 좌표로 화면 출력
	printf("시간을 설정하세요 : "); // 시간 설정 화면 출력
	scanf("%d", &timer); // 시간 입력 ex) 10, 30, 60, 90초 단위로 설정 가능

	system("cls"); // 설정이 완료되면 화면 지우기
	design(); // 인터페이스 화면 출력
	gotoxy(48, 10); // x = 48, y = 10 좌표로 화면 출력
	printf("시간이 %d 초로 설정되었습니다!", timer); // 시간이 입력한 값으로 설정 되었다는 화면 출력

	gotoxy(48, 12); // x = 48, y = 12 좌표로 화면 출력
	printf("계속하시려면 아무 키나 누르세요."); // 아무 키나 누르세요
	_getch(); // 키보드로 입력받으면
}

void gamemain() { // 게임시작 함수
	gotoxy(50, 9); // x : 50, y : 9로 화면 출력
	system("cls"); // 화면 지우기
	design(); // 인터페이스 화면 함수 호출

	gotoxy(50, 9); // x : 50, y : 9로 화면 출력
	printf(" 3초 후 시작 !!"); // 화면 출력
	Sleep(1000); // 1초 기다립니다.

	system("cls"); // 화면 지우기
	design(); // 인터페이스 화면 함수 호출

	gotoxy(50, 9); // x : 50, y : 9로 화면 출력
	printf(" 2초 후 시작 !!"); // 화면 출력
	Sleep(1000); // 1초 기다립니다.

	system("cls"); // 화면 지우기
	design(); // 인터페이스 화면 함수 호출

	gotoxy(50, 9); // x : 50, y : 9로 화면 출력
	printf(" 1초 후 시작 !!"); // 화면 출력
	Sleep(1000); // 1초 기다립니다.

	system("cls"); // 화면 지우기
	startTime = clock(); // 시간 설정 초기화 clock 함수 호출
	design(); // 인터페이스 화면 함수 호출

	while (timer >= gap) { // 게임이 끝날 때 까지 반복
		word_io(); // 단어를 입력하는 함수 호출
	}
	if (score > dbscore && timer != dbtimer) { // 현재점수가 dbscore 보다 클 경우
		dbscore = score; // 현재점수를 dbscore에 저장
		dbtimer = timer; // 설정시간을 dbtimer에 저장
		sprintf(query2, "update rank set score = '%d', time = '%d' where id = '%s'", dbscore, dbtimer, id); 
		// query2에 rank 테이블에 아이디, 점수, 시간 값을 저장
		query_stat = mysql_query(connection, query2); // query2이 정상적으로 값이 넘어갔는지 확인
	}
	system("cls"); // 화면 지우기
	design(); // 인터페이스 화면 함수 호출
	gotoxy(50, 11); // x : 50, y : 11 좌표로 화면 출력
	printf("---게 임 종 료---"); // 게임이 종료되었다는 화면 출력
	Sleep(2000); // 2초간 기다립니다.
}

void word_io() { // 단어를 입출력하는 함수

	x = rand() % 105 + 5; // 단어를 x축 5에서 105까지 랜덤으로 출력
	y = rand() % 19 + 3; // 단어를 y축 3에서 19까지 랜덤으로 출력
	i = rand() % wordmax; // 0에서 103(wordmax = 103) 번째 단어를 랜덤으로 출력
	f = rand() % 4 + 1; // 단어의 색을 파랑색, 초록색, 하늘색, 빨강색으로 랜덤 부여

	if (wordi[i] != 1) { // 단어를 한 번만 뜨게 하는 배열 wordi 인덱스 값이 1이 아닌 경우
		wordi[i] = 1; // 배열 wordi에 1을 저장

		gotoxy(x, y); // 랜덤으로 설정한 x, y축으로 단어 출력
		for (int j = 0; j < 3; j++) {  // 반복문을 사용하기 위해 j를 선언하고 0으로 초기화, j가 3보다 클때까지 반복, 반복할 때마다 j 1증감
			if (j == 0) { // 만약 j가 0일때
				remem[i][j] = x; // 단어를 출력하기 위한 임시 2차원배열 remem에 단어의 x축 값 저장
			}
			else if (j == 1) { // 만약 j가 1일때
				remem[i][j] = y; // 단어를 출력하기 위한 임시 2차원배열 remem에 단어의 y축 값 저장
			}
			else if (j == 2) { // 만약 j가 3일때
				remem[i][j] = f; // 단어를 출력하기 위한 임시 2차원 배열 remem에 단어의 색깔 값을 저장
			}
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), f); // 글자색을 바꾸는 함수, 랜덤 색깔로 출력
		printf("%s ", word[i]); //배열 word에 저장된 0 ~ 103열에 있는 단어를 출력
	}

	int p = 0; // 단어를 출력하기 위한 변수 p 선언 후 0으로 초기화
	while (p < wordtime) { // p가 wordtime(20000)보다 클 때까지 반복
		p++; // p 증감 
		i = 0; // 단어 수를 입력 받기 위해 0으로 초기화
		int scanc = 0; // 키보드 입력 값을 받을 변수
		endTime = clock();  // 코드가 끝난 시간 저장
		gap = (double)(endTime - startTime) / (CLOCKS_PER_SEC);// 초 단위로 시간을 출력하기 위함

		gotoxy(18, 2); // x : 18, y, 2 좌표로 화면 출력
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 글자색을 바꾸는 함수, 랜덤 색깔로 출력
		printf("[ 남은 시간 : %d / %d ]", gap, timer); // [ 남은시간 : 0 / 40 ] 남은 시간을 알려줌 gqp은 1초마다 증가 / timer 메뉴 4. 시간설정에서 설정 값 출력

		if (_kbhit()) { // 만약 키보드가 입력 되었을 경우
			scanc = _getch(); // 키보드 입력 값을 변수 scanc에 저장
			if (scanc != 8 && scanc != 13) { // 만약 키보드로부터 scanc에 저장된 키 값이 아스키 코드 값 8(backspace(<-)) 그리고 13(carriage rerturn(\r))이 아닐 경우
				scan[sword++] = scanc; // 키보드 입력으로 부터 저장된 scanc 값을 문자를 입력받는 scan[sword == 0] 변수에 저장 (반복을 진행할 때 마다 증감)
									   // ex) Hello 입력 scan[0] = 'H', scan[1] = 'e', scan[2] = 'l', scan[3] = 'l', scan[4] = 'o'
				gotoxy(5, 25); // x = 5, y = 25 좌표로 화면 출력
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 글자색을 바꾸는 함수, 흰색으로 출력
				printf("%s", scan); // scan변수에 저장된 키보드로 입력된 값을 출력해 줌
			}
			else if (scanc == 8) { // backspace 입력 됨 (만약 scanc에 저장된 키 값이 아스키 코드 8(backspace(<-))일 경우)
				gotoxy(1, 25); // x = 1, y = 25 좌표로 화면 출력
				printf("|                                                                                                                     |\n");
				// backspace가 입력 될 경우 단어가 정상적으로 지워지도록 하기 위해 출력
				gotoxy(5, 25); // x = 5, y = 25 좌표로 화면 출력
				scan[--sword] = scanc; // 키보드 입력으로 부터 저장된 scanc 값을 문자를 입력받는 scan[sword - 1] 변수에 저장된 값을 삭제
									   // ex) Heloo(오타)를 입력 scan[4] = 'o' 삭제 scan[3] = 'o' 삭제
				if (sword == -1) // 만약 sword가 -1일 경우
					sword = 0; // scan 변수의 인덱스 값을 나타내주는 변수이기 때문에 0으로 초기화 하면 다시 scan[0]부터 입력을 받음
				printf("%s", scan); // scan변수에 저장된 키보드로 입력된 값을 출력해줌
			}
			else if (scanc == 13) { //만약 키보드로부터 scanc에 저장된 키 값이 아스키코드 값 13(\r)일 경우
				sword = 0; // scan 변수의 인덱스 값을 나타내주는 변수이기 때문에 0으로 초기화 하면 다시 scan[0]부터 입력을 받음
				gotoxy(1, 25); // x = 1, y = 25 좌표 값으로 화면 출력
				printf("|                                                                                                                     |\n");
				// \r(줄바꿈)이 입력 될 경우 단어를 다시 정상적으로 입력 받기 위해서 출력
				for (i = 0; i <= wordmax; i++) { // 단어 수(i)를 0으로 초기화, 단어 수(i)가 최대 단어수 wordmax(103)보다 크거나 같을 때까지 단어 수(i++)를 증감하면서 반복
					if (!strcmp(scan, word[i])) { // word[i]에 있는 단어와 키보드로 부터 입력 된 scan에 저장된 값이 같을 경우
						x2 = remem[i][0]; // x 좌표를 다시 불러올 변수 x2에 임시 배열 remen[i][0] 값을 저장
						y2 = remem[i][1]; // y 좌표를 다시 불러올 변수 x2에 임시 배열 remen[i][1] 값을 저장
						f2 = remem[i][2]; // 색을 다시 불러올 변수 f2에 임시 배열 remen[i][2] 값을 저장

						gotoxy(x2, y2); // 임시 배열 remem에 저장 받은 복사된 랜덤 x2, y2축 값을 출력
						printf("          "); // 복사된 x, y 축 값에 가서 출력된 단어를 공백 처리로 출력하여 지움
						gotoxy(1, 25); // x = 1, y = 25 좌표로 화면 출력
						printf("|                                                                                                                     |\n");
						// \r(줄바꿈)이 입력 될 단어를 정상적으로 입력받기 위해서 출력
						switch (f2) { // 임시 배열 remem에 저장 되고 복사된 랜덤 색깔 지정 변수 f2에 Switch문 생성
						// 기존의 f (색을 1 ~ 4로 랜덤 부여 됨)
						case 1: score += 150; break; // 파랑색 글자에 현재점수 150점 부여
						case 2: score += 200; break; // 초록색 글자에 현재점수 200점 부여
						case 3: score += 250; break; // 하늘색 글자에 현재점수 250점 부여
						case 4: score += 300; break; // 빨강색 글자에 현재점수 300점 부여
						default: break; //Swith문 빠져나옴
						}
						gotoxy(3, 2); // x = 3, y = 2 좌표로 화면 출력
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //글자 색을 바꿔주는 함수, 흰색으로 설정
						printf("현재점수 : %d", score); // 현재점수를 나타내줌
					}
				}
				for (i = 0; i < 50; i++) { // i를 0으로 초기화, i가 50보다 클때까지 i를 증감하면서 반복 
					scan[i] = NULL; // scan[i]에 저장된 값을 키보드에 입력된 값을 NULL처리
				}
			}
		}
	}
}

int database() { // 데이터베이스 함수

	mysql_init(&conn);

	//한글문자셋 설정(오류방지)
	mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
	mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

	//연결 mysql_real_connect
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

	system("mode con: cols=34 lines=20"); // 화면 사이즈 조절

	if (connection == NULL) { // 연결 정보가 없을 때 
		fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn)); // 실패 원인 출력
		return 1; // 비정상 종료
	}


	query_stat = mysql_query(connection, "select * from login"); // 정상적으로 쿼리가 DB로 넘어가면 "select * from login" 문 출력
	if (query_stat != 0) {	// 만약 query_stat에 0값이 넘어가지 않으면 ( 0값이 넘어가면 정상 )
		fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn)); // 실패 원인 출력
		return 1; // 비정상 종료
	}
	printf("\t    회원 목록\n"); // 회원 목록 출력
	printf("\t  [  아이디  ]\n"); // 아이디 출력
	sql_result = mysql_store_result(connection); // select * from login 메모리에 저장
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {	// sql_row가 NULL값이 아닐 때 까지 출력
		printf("\t  %10s\n", sql_row[0]); // 행 단위로 출력
	}
	mysql_free_result(sql_result); // 메모리 해제
	
	while (1) {
		printf("\n\n [ 회원가입(Y) / 로그인(N) ] : "); // 회원가입 여부 출력
		scanf("%c", &ans); // 키보드로 부터 입력
		scanf("%*c"); // 입력된 값을 변수에 저장하지 않고 무시

		if (ans == 'y' || ans == 'Y') { // 만약 ans가 'y' 이거나 'Y' 일 경우
			// 회원가입
			system("cls"); // 화면 지우기

			gotoxy(12, 3); // x : 12, y : 3로 화면 출력 
			printf("회 원 가 입"); // 회원가입 화면 출력

			gotoxy(3, 5); // x : 3, y : 5로 화면 출력
			printf("아이디 : "); // 아이디 입력창 출력
			fgets(id, 20, stdin); // 아이디 입력을 20 크기까지 입력을 받음
			CHOP(id); // 엔터키가 들어오면 공백으로 처리

			gotoxy(3, 8); // x : 3, y : 8로 화면 출력
			printf("패스워드 : "); // 패스워드 입력창 출력
			fgets(pass, 20, stdin); // 패스워드 입력을 20 크기까지 입력을 받음
			CHOP(pass); // 엔터키가 들어오면 공백으로 처리

			sprintf(query, "insert into login values" "('%s', '%s')", id, pass); //query를 login 테이블에 아이디 패스워드 값을 저장
			query_stat = mysql_query(connection, query); // query가 정상적으로 값이 넘어갔는지 확인

			sprintf(query1, "insert into rank values" "('%s', '%d', '%d')", id, 0, 0); // query1을 rank 테이블에 아이디, 점수, 시간 값을 저장
			query_stat = mysql_query(connection, query1); // query1이 정상적으로 값이 넘어갔는지 확인

			gotoxy(4, 11); // x : 4, y : 11로 화면 출력
			printf("회원가입이 완료되었습니다!"); // 회원가입이 완료되었다는 화면 출력
			_getch(); // 키보드로 부터 입력대기
			system("cls"); // 화면 지우기
		}
		else if (ans == 'N' || ans == 'n') { // 만약 ans가 'n' 이거나 'N' 일 경우
			//로그인
			system("cls"); // 화면 지우기

			gotoxy(12, 3); // x : 12, y : 3로 화면 출력
			printf("로 그 인"); // 로 그 인 화면 출력

			gotoxy(3, 5); // x : 3, y : 5로 화면 출력
			printf("아이디 : "); // 아이디 입력창 출력
			fgets(id, 20, stdin); // 아이디 입력을 20 크기까지 입력을 받음
			CHOP(id); // 엔터키가 들어오면 공백으로 처리

			gotoxy(3, 8); // x : 3, y : 8로 화면 출력
			printf("패스워드 : "); // 비밀번호 입력창 출력
			fgets(pass, 20, stdin); // 패스워드 입력을 20 크기까지 입력을 받음
			CHOP(pass); // 엔터키가 들어오면 공백으로 처리

			sprintf(check, "select * from login where id = '%s' and passwd = '%s'", id, pass); // query에 저장된 아이디와 패스워드가 맞는지 확인
			query_stat = mysql_query(connection, check); // query에 정상적으로 값이 넘어갔는지 확인
			sql_result = mysql_store_result(connection); //select * from login 메모리에 저장

			if ((sql_row = mysql_fetch_row(sql_result)) == NULL) { // 만약 sql_row값이 NULL일 경우
				system("cls"); // 화면 지우기
				gotoxy(3, 6); // x : 3, y : 6으로 화면 출력
				printf("회원정보가 일치하지 않습니다."); // 경고문 출력
				gotoxy(6, 8); // x : 6 y : 8으로 화면 출력
				printf("프로그램이 종료됩니다..."); // 프로그램 종료창
				_getch(); // 키보드로부터 입력 대기
				exit(1); // 비정상 종료
			}

			system("cls"); // 화면 지우기
			gotoxy(11, 5); // x : 11, y : 5로 화면 출력
			printf("로그인 성공!!"); // 로그인 성공 출력

			gotoxy(9, 7);  // x : 9, y : 7로 화면 출력
			printf("%s님 환영합니다!!\n", id); // 로그인한 아이디를 출력

			gotoxy(2, 9);  // x : 2, y : 9로 화면 출력
			count++; // 로그인 화면을 한 번만 뜨게 하기 위해 count증감
			printf("시작하시려면 아무 키나 누르세요."); // 아무키나 누르세요 화면 출력
			_getch(); // 키 입력 대기
			mysql_free_result(sql_result); // 메모리 해제
			break; // While문 빠져나가기
		}
	}
	return 0; // 함수 종료
}