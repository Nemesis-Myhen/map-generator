#include <Windows.h>
#include <stdio.h>

/*
* what if we double the map size in both axis and then downscale it to the screen?
*/

double x;

#define width 160
#define height 48

double map[2][height][width];
double MAP[2][height*2][width*2];
void mapgen() {
	for (int i = 0; i < height * 2; i++) {
		for (int j = 0; j < width * 2; j++) {
			x = x * 4 * (1 - x);
			MAP[0][i][j] = x;
		}
	}

	for (int k = 0; k < 20; k++) {

		MAP[1][0][0] = (MAP[0][1][0] + MAP[0][0][1]) / 2;
		MAP[1][(height * 2) - 1][0] = (MAP[0][(height * 2) - 2][0] + MAP[0][(height * 2) - 1][1]) / 2;
		MAP[1][0][(width * 2) - 1] = (MAP[0][1][(width * 2) - 1] + MAP[0][0][(width * 2) - 2]) / 2;
		MAP[1][(height * 2) - 1][(width * 2) - 1] = (MAP[0][(height * 2) - 1][(width * 2) - 2] + MAP[0][(height * 2) - 2][(width * 2) - 1]) / 2;

		for (int i = 1; i < (width * 2) - 1; i++) {
			MAP[1][0][i] = (MAP[0][0][i - 1] + MAP[0][0][i + 1] + MAP[0][1][i]) / 3;
			MAP[1][height * 2 - 1][i] = (MAP[0][height * 2 - 1][i - 1] + MAP[0][height * 2 - 1][i + 1] + MAP[0][height * 2 - 2][i]) / 3;
		}

		for (int i = 1; i < height * 2 - 1; i++) {
			MAP[1][i][0] = (MAP[0][i - 1][0] + MAP[0][i + 1][0] + MAP[0][i][1]) / 3;
			MAP[1][i][width * 2 - 1] = (MAP[0][i - 1][width * 2 - 1] + MAP[0][i + 1][width * 2 - 1] + MAP[0][i][width * 2 - 2]) / 3;
		}

		for (int i = 1; i < height*2-1; i++) {
			for (int j = 1; j < width*2-1; j++) {
				MAP[1][i][j] = (MAP[0][i - 1][j] + MAP[0][i + 1][j] + MAP[0][i][j - 1] + MAP[0][i][j + 1]) / 4;
			}
		}

		for (int i = 0; i < height * 2 - 1; i++) {
			for (int j = 0; j < width * 2 - 1; j++) {
				MAP[0][i][j] = MAP[1][i][j];
			}
		}
	}

	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			map[0][i][j] = (MAP[0][i * 2][j * 2] + MAP[0][i * 2 + 1][j * 2] + MAP[0][i * 2][j * 2 + 1] + MAP[0][i * 2 + 1][j * 2 + 1]) / 4;
		}
	}
}


char scr [48][160];
WORD scrc[48][160];
void scrgen(){
	for (int i = 0; i < 48; i++) {
		for (int j = 0; j < 160; j++) {
			if (map[0][i][j] < .4) {
				scr[i][j] = '~';
				scrc[i][j] = 0x19;
			}
			else if (map[0][i][j] < .48) {
				scr[i][j] = '.';
				scrc[i][j] = 0x6E;
			}
			else if (map[0][i][j] < 2) {
				scr[i][j] = '\"';
				scrc[i][j] = 0x2A;
			}
		}
	}
}

HANDLE consoleout;
DWORD cw;
DWORD aw;
void printscr() {
	for (int i = 0; i < 48; i++){
		for (int j = 0; j < 160; j++){
			WriteConsoleOutputCharacterA(consoleout, &scr[i][0], 160, (COORD) { 0, i }, & cw);
			WriteConsoleOutputAttribute(consoleout, &scrc[i][0], 160, (COORD) { 0, i }, & aw);
		}
	}
}

int main(int argc, char** argv) {
	sscanf_s(argv[1], "%lf", &x);
	//x = 1;
	consoleout = GetStdHandle(STD_OUTPUT_HANDLE);

	mapgen();
	scrgen();
	printscr();
		
	return 0;
}
