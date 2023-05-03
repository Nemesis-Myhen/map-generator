#include <stdio.h>
#include <Windows.h>

#pragma warning (push, 4)

WNDCLASSEXW mapwindow;
HWND mapwindowhandle;
MSG mapwindowmessages;
BOOL endmainloop;
PAINTSTRUCT mapwindowpaintstruct;
BITMAPINFO mapimageinfo;
void* mapimage;
double x;

LRESULT mapwindowprocedures(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void mapgen();
void scrgen();

INT wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT nCmdShow)
{
	mapwindow.cbSize = sizeof(WNDCLASSEXW);
	mapwindow.style = CS_SAVEBITS | CS_VREDRAW | CS_HREDRAW;
	mapwindow.lpfnWndProc = mapwindowprocedures;
	mapwindow.cbClsExtra = 0;
	mapwindow.cbWndExtra = 0;
	mapwindow.hInstance = hInstance;
	mapwindow.hIcon = NULL;
	mapwindow.hCursor = NULL;
	mapwindow.hbrBackground = NULL;
	mapwindow.lpszMenuName = NULL;
	mapwindow.lpszClassName = L"map window";
	mapwindow.hIconSm = NULL;

	RegisterClassExW(&mapwindow);
	mapwindowhandle = CreateWindowExW(
		WS_EX_OVERLAPPEDWINDOW,
		L"map window",
		L"World generation: now inside a window!",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		320, 96,
		NULL, NULL, NULL, NULL
	);
	ShowWindow(mapwindowhandle, 5);

	mapimageinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	mapimageinfo.bmiHeader.biWidth = 160;
	mapimageinfo.bmiHeader.biHeight = 48;
	mapimageinfo.bmiHeader.biPlanes = 1;
	mapimageinfo.bmiHeader.biBitCount = 8;
	mapimageinfo.bmiHeader.biCompression = BI_RGB;
	mapimageinfo.bmiHeader.biSizeImage = 0;
	mapimageinfo.bmiHeader.biXPelsPerMeter = 1;
	mapimageinfo.bmiHeader.biYPelsPerMeter = 1;
	mapimageinfo.bmiHeader.biClrUsed = 3;
	mapimageinfo.bmiHeader.biClrImportant = 0;
	mapimageinfo.bmiColors[0] = (RGBQUAD){ 0, 1, 3, 0 };
	mapimageinfo.bmiColors[1] = (RGBQUAD){ 2, 0, 0, 0 };
	mapimageinfo.bmiColors[2] = (RGBQUAD){ 0, 0, 0, 0 };

	mapimage = malloc((size_t)8 * 160 * 48);

	x = 0.1;
	swscanf_s(pCmdLine, L"%lf", &x);


	mapgen();
	scrgen();

	while (!endmainloop)
	{
		GetMessageW(&mapwindowmessages, mapwindowhandle, 0, 0);
		DispatchMessageW(&mapwindowmessages);
	}

	return 0;
}

LRESULT mapwindowprocedures(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
		BeginPaint(mapwindowhandle, &mapwindowpaintstruct);
		struct {
			WORD        palVersion;
			WORD        palNumEntries;
			PALETTEENTRY   palPalEntry[3];
		} pal;
		//LOGPALETTE pal;
		pal.palVersion = 0;
		pal.palNumEntries = 3;
		pal.palPalEntry[0] = (PALETTEENTRY){   0, 55,218,PC_EXPLICIT };
		pal.palPalEntry[1] = (PALETTEENTRY){ 193,156,  0,PC_EXPLICIT };
		pal.palPalEntry[2] = (PALETTEENTRY){ 166,202,240,PC_EXPLICIT };

		HPALETTE pall = CreatePalette(&pal);
		SelectPalette(mapwindowpaintstruct.hdc, pall, FALSE);
		RealizePalette(mapwindowpaintstruct.hdc);

		StretchDIBits(mapwindowpaintstruct.hdc,
			mapwindowpaintstruct.rcPaint.left, mapwindowpaintstruct.rcPaint.top, mapwindowpaintstruct.rcPaint.right, mapwindowpaintstruct.rcPaint.bottom,
			0, 0, 160, 48, mapimage, &mapimageinfo, DIB_PAL_COLORS, SRCCOPY);

		EndPaint(mapwindowhandle, &mapwindowpaintstruct);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		free(mapimage);
		endmainloop = 1;
		break;
	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}


#define width 160
#define height 48

double map[2][height][width];
double MAP[2][height * 2][width * 2];
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

		for (int i = 1; i < height * 2 - 1; i++) {
			for (int j = 1; j < width * 2 - 1; j++) {
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


void scrgen() {
	for (int i = 0; i < 48; i++) {
		for (int j = 0; j < 160; j++) {
			if (map[0][i][j] < .4) {
				*((char*)mapimage + (i * 160 + j)) = 0;
			}
			else if (map[0][i][j] < .48) {
				*((char*)mapimage + (i * 160 + j)) = 1;
			}
			else if (map[0][i][j] < 2) {
				*((char*)mapimage + (i * 160 + j)) = 2;
			}
		}
	}
}

#pragma warning (pop)
