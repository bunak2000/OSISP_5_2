#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const wchar_t* const ClassName = L"PerfectLab";
RECT Rect, TextBox;
HDC hdc, hCompatibleDC;
HANDLE hOldBitmap;

int N = 1, M = 1;
int ColumnSize;
int LineSize;
int Line;

void InitializeGlobalVariables(HWND hWnd)
{
	srand(time(NULL));

	N = 5;
	M = 5;

	GetClientRect(hWnd,&Rect);
	LineSize = Rect.right / M;
	
	ColumnSize = 0;
	Line = 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &Rect);

		int X = 0, Y = 0, Temp = 0;
		ColumnSize = 0;
		
		SetBkMode(hdc, TRANSPARENT);

		for (int i = 0 + Line; i < N; i++)
		{

			X = 0;
			for (int j = 0; j < M; j++)
			{

				TextBox.left = X + 1;
				TextBox.right = X + LineSize - 1;
				TextBox.top = Y + 1;
				TextBox.bottom = Y + Rect.bottom - 1;
				
				switch (rand() % 5 + 1)
				{
				case 1:
					Temp = DrawTextW(hdc, L"If the function succeeds, the return value is the text height in logical units.", -1, &TextBox, DT_EDITCONTROL | DT_WORDBREAK);
					break;
				case 2:
					Temp = DrawTextW(hdc, L"A handle to the device context in which to draw.", -1, &TextBox, DT_EDITCONTROL | DT_WORDBREAK);
					break;
				case 3:
					Temp = DrawTextW(hdc, L"The formatting options. This parameter can be one or more of the following values.", -1, &TextBox, DT_EDITCONTROL | DT_WORDBREAK);
					break;
				case 4:
					Temp = DrawTextW(hdc, L"If the cchText parameter is -1, the string must be null-terminated.", -1, &TextBox, DT_EDITCONTROL | DT_WORDBREAK);
					break;
				case 5:
					Temp = DrawTextW(hdc, L"A pointer to the string that contains the text to draw.", -1, &TextBox, DT_EDITCONTROL | DT_WORDBREAK);
					break;
				default:
					Temp = DrawTextW(hdc, L"Default Text", -1, &TextBox, DT_EDITCONTROL | DT_WORDBREAK);
					break;
				}

				if (Temp > ColumnSize) 
				{
					ColumnSize = Temp;
				}

				X = X + LineSize;
			}

			MoveToEx(hdc, 0, Y, NULL);
			LineTo(hdc, X, Y);

			Temp = 0;

			Y = Y + ColumnSize;
		}
		MoveToEx(hdc, 0, Y, NULL);
		LineTo(hdc, X, Y);

		X = 0;
		for (int j = 0; j < M; j++)
		{
			MoveToEx(hdc, X, 0, NULL);
			LineTo(hdc, X, Y);

			X = X + LineSize;
		}
		MoveToEx(hdc, X, 0, NULL);
		LineTo(hdc, X, Y);

		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
		{
			if (Line > 0)
			{
				Line--;
				GetClientRect(hWnd, &Rect);
				InvalidateRect(hWnd, &Rect, true);
			}
			break;
		}
		case VK_DOWN:
		{
			if (Line < N-1)
			{
				Line++;
				GetClientRect(hWnd, &Rect);
				InvalidateRect(hWnd, &Rect, true);
			}
			break;
		}
		case VK_LEFT:
		{
			GetClientRect(hWnd, &Rect);
			if (M > 1)
			{
				M--;
				LineSize = Rect.right / M;
			}

			InvalidateRect(hWnd, &Rect, true);

			break;
		}
		case VK_RIGHT:
		{
			GetClientRect(hWnd, &Rect);
			if ((Rect.right / M + 1) > 13) 
			{
				M++;
				LineSize = Rect.right / M;
			}

			InvalidateRect(hWnd, &Rect, true);
		}
		default:
			break;
		}
		return 0;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 13*M;
		lpMMI->ptMinTrackSize.y = 100;
	}
	return 0;

	case WM_SIZE:
		GetClientRect(hWnd, &Rect);
		LineSize = Rect.right / M;
		InvalidateRect(hWnd, &Rect, true);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	HWND hMainWindow;
	WNDCLASSEX wcex;
	MSG msg;

	srand(time(NULL));



	memset(&wcex, 0, sizeof wcex);
	wcex.cbSize = sizeof wcex;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_INACTIVECAPTION + 1);//(HBRUSH)(COLOR_HIGHLIGHT + 1);
	wcex.lpszClassName = ClassName;
	RegisterClassEx(&wcex);

	hMainWindow = CreateWindowEx(0, ClassName, L"Lab_1", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 600, 400, 0, 0, hInstance, NULL);

	InitializeGlobalVariables(hMainWindow);

	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);



	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}