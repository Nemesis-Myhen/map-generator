/*
* water: connects with sand | value 1 | char '~'
* sand: connects with water | value 2 | char '.'
*/

#include <Windows.h>
#include <stdio.h>

HANDLE consoleOut = NULL;
DWORD mode = 0;
unsigned long long x = 0;
unsigned long long n = 0;
char map[40][160];

char* rules[] = { 
	"\1\1\2", 
	"\1\2\2",
	"\1\2",
	"\1\1\2",
	"\1\2\2"
};

unsigned long long xorshift(long long n){
	n ^= n << 13;
	n ^= n >> 17;
	n ^= n << 5;
	return n;
}
void enableVirtualTerminalSequences(HANDLE* pconsoleOut, DWORD* pmode){
	*pconsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(*pconsoleOut, &(*pmode));
	*pmode = *pmode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	*pmode = *pmode | ENABLE_PROCESSED_OUTPUT;
	SetConsoleMode(*pconsoleOut, *pmode);
}

int main(int argc, char** argv) {
	enableVirtualTerminalSequences(&consoleOut, &mode);
	sscanf_s(argv[1], "%llu", &x);
	x = xorshift(x);

	map[0][0] = (x % 2 == 0) ? (1) : (2);

	for (int i = 1; i < 160; i = i + 1) {
		x = xorshift(x);
		if (map[0][i - 1] == 1) {
			map[0][i] = rules[0][x % 3];
		}
		else if (map[0][i - 1] == 2) {
			map[0][i] = rules[1][x % 3];
		}
	}
	a:
	for (int i = 1; i < 40; i++) {
		for (int j = 0; j < 160; j++) {
			x = xorshift(x);
			if (j == 0) {
				if (map[i - 1][j] == 1 && map[i - 1][j + 1] == 1) {
					map[i][j] = 1;
				}
				else if (map[i - 1][j] == 1 && map[i - 1][j + 1] == 2) {
					map[i][j] = rules[2][x % 2];
				}
				else if (map[i - 1][j] == 2 && map[i - 1][j + 1] == 1) {
					map[i][j] = rules[2][x % 2];
				}
				else if (map[i - 1][j] == 2 && map[i - 1][j + 1] == 2) {
					map[i][j] = 1;
				}
			}
			else if (j == 159) {
				if (map[i - 1][j] == 1 && map[i - 1][j - 1] == 1) {
					map[i][j] = 1;
				}
				else if (map[i - 1][j] == 1 && map[i - 1][j - 1] == 2) {
					map[i][j] = rules[2][x % 2];
				}
				else if (map[i - 1][j] == 2 && map[i - 1][j - 1] == 1) {
					map[i][j] = rules[2][x % 2];
				}
				else if (map[i - 1][j] == 2 && map[i - 1][j - 1] == 2) {
					map[i][j] = 1;
				}
			}
			else {
				if (map[i - 1][j - 1] == 1 && map[i - 1][j] == 1 && map[i - 1][j + 1] == 1) {
					map[i][j] = 1;
				}
				else if (map[i - 1][j - 1] == 1 && map[i - 1][j] == 1 && map[i - 1][j + 1] == 2) {
					map[i][j] = rules[3][x % 3];
				}
				else if (map[i - 1][j - 1] == 1 && map[i - 1][j] == 2 && map[i - 1][j + 1] == 1) {
					map[i][j] = rules[3][x % 3];
				}
				else if (map[i - 1][j - 1] == 1 && map[i - 1][j] == 2 && map[i - 1][j + 1] == 2) {
					map[i][j] = rules[4][x % 3];
				}
				else if (map[i - 1][j - 1] == 2 && map[i - 1][j] == 1 && map[i - 1][j + 1] == 1) {
					map[i][j] = rules[3][x % 3];
				}
				else if (map[i - 1][j - 1] == 2 && map[i - 1][j] == 1 && map[i - 1][j + 1] == 2) {
					map[i][j] = rules[4][x % 3];
				}
				else if (map[i - 1][j - 1] == 2 && map[i - 1][j] == 2 && map[i - 1][j + 1] == 1) {
					map[i][j] = rules[4][x % 3];
				}
				else if (map[i - 1][j - 1] == 2 && map[i - 1][j] == 2 && map[i - 1][j + 1] == 2) {
					map[i][j] = 2;
				}
			}
		}
	}

	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 160; j++) {
			if (map[i][j] == 1) printf("\033[38;5;4;48;5;12m~");
			else if (map[i][j] == 2) printf("\033[38;5;11;48;5;3m.");
		}
		putchar('\n');
	}
	for (int i = 0; i < 160; i = i + 1) {
		map[0][i] = map[39][i];
	}
	goto a;
	return 0;
}