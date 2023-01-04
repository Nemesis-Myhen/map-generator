#include <Windows.h>
#include <stdio.h>
#include <math.h>

void enableVirtualTerminalSequences(HANDLE* pconsoleOut, DWORD* pmode) {
	*pconsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(*pconsoleOut, &(*pmode));
	*pmode = *pmode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	*pmode = *pmode | ENABLE_PROCESSED_OUTPUT;
	SetConsoleMode(*pconsoleOut, *pmode);
}
HANDLE consoleOut;
DWORD mode;

unsigned long long xorshift(long long n) {
	n ^= n << 13;
	n ^= n >> 17;
	n ^= n << 5;
	return n;
}

struct point {
	int x;
	int y;
};

#define NPOINTS 320
#define WIDTH 1000
#define HEIGHT 1000

int map[HEIGHT][WIDTH];
struct point point[NPOINTS];
double distance[NPOINTS];
double nearestpoint = 0xfffffffff;

char screen[HEIGHT][WIDTH];
WORD screencolor[HEIGHT][WIDTH];
DWORD cw = 0;
DWORD aw = 0;

struct point pos = { 0,0 };

SYSTEMTIME time;

struct keys {
	short up;
	short down;
	short left;
	short righ;
}keys;

unsigned long long x = 0;

const short not1 = 0b1111111111111110;
int main(int argc, char** argv) {
	//enableVirtualTerminalSequences(&consoleOut, &mode);
	sscanf_s(argv[argc - 1], "%llu", &x);
	consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("%llu", x);
	//generate points
	for (int i = 0; i < NPOINTS; i++) {
		x = xorshift(x);
		point[i].x = x%WIDTH;
		x = xorshift(x);
		point[i].y = x%HEIGHT;
	}
	//generate map
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			for (int k = 0; k < NPOINTS; k++) {
				distance[k] = sqrt(abs(point[k].y - i) * abs(point[k].y - i) + abs(point[k].x - j) * abs(point[k].x - j));
			}

			for (int k = 0; k < NPOINTS; k++) {
				if (distance[k] < nearestpoint) nearestpoint = distance[k];
			}

			map[i][j] = nearestpoint;
			nearestpoint = 0xfffffffff;

			if (map[i][j] > 10) {
				screen[i][j] = '~';
				screencolor[i][j] = 0x19;
			}
			else if (map[i][j] > 8) {
				screen[i][j] = '.';
				screencolor[i][j] = 0x6E;
			}
			else if(map[i][j] > 5) {
				screen[i][j] = '\"';
				screencolor[i][j] = 0x2A;
			}
			else if (map[i][j] > 1) {
				screen[i][j] = '^';
				screencolor[i][j] = 0x80;
			}
			else {
				screen[i][j] = '*';
				screencolor[i][j] = 0x7f;
			}
		}
	}

	while (1) {
		ULONGLONG starttime = GetTickCount64();
		ULONGLONG futuretime = starttime + 10;//200000;

		keys.up   = GetAsyncKeyState(VK_UP)    & not1;
		keys.down = GetAsyncKeyState(VK_DOWN)  & not1;
		keys.left = GetAsyncKeyState(VK_DOWN) & not1;
		keys.righ = GetAsyncKeyState(VK_RIGHT) & not1;

		if (GetAsyncKeyState(0x57))pos.y--;
		if (GetAsyncKeyState(0x53))pos.y++;
		if (GetAsyncKeyState(0x41))pos.x--;
		if (GetAsyncKeyState(0x44))pos.x++;
		if (GetAsyncKeyState(VK_SPACE)) pos = (struct point){0,0};

		if (pos.x < 0) pos.x = 0;
		if (pos.y < 0) pos.y = 0;

		if (pos.x > WIDTH) pos.x = WIDTH;
		if (pos.y > HEIGHT) pos.y = HEIGHT;

		//if (GetAsyncKeyState(VK_ESCAPE))goto end;
		//printf("here");
		SetConsoleCursorPosition(consoleOut, (COORD) { 0, 0 });
		for (int i = 0; i < 40; i++) {
			WriteConsoleOutputCharacterA(consoleOut, &screen[i + pos.y][pos.x], 160, (COORD) { 0, i }, & cw);
			WriteConsoleOutputAttribute(consoleOut, &screencolor[i + pos.y][pos.x], 160, (COORD) { 0, i }, & aw);
		}
		SetConsoleCursorPosition(consoleOut, (COORD){ 0,41 });
		printf("                                      \r");
		printf("%i %i", pos.x, pos.y);

		ULONGLONG elapsedtime = GetTickCount64() - starttime;
		while (GetTickCount64() < futuretime) {}// printf("%llu\n", GetTickCount64());

	}
	end:

	return 0;
}
//printf("\033[38;5;12;48;5;4m~");
//printf("\033[38;5;11;48;5;3m.");
//printf("\033[38;5;10;48;5;2m\"");