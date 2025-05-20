#define _CRT_SECURE_NO_WARNINGS
#include "WindowsProject9.h"
#include "framework.h"
#include <set>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>


using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND htext;
HWND hEditSize;
HWND hEditRelation;
HWND hButton;
HWND hOutput;
HFONT hFontOutput;

using Relation = set<pair<string, string>>;

bool isFunctionMatrix(const vector<wstring>& matrix, int n, int m) {
    for (int i = 0; i < n; ++i) {
        int countOnes = 0;
        if ((int)matrix[i].size() != m) return false; 
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == L'1') {
                countOnes++;
            }
            else if (matrix[i][j] != L'0') {
                return false; 
            }
        }
        if (countOnes != 1) return false; 
    }
    return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT9, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT9));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT9));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT9);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   hFontOutput = CreateFontW(
       26,
       0,
       0,
       0,
       FW_NORMAL,
       FALSE,
       FALSE,
       FALSE,
       DEFAULT_CHARSET,
       OUT_DEFAULT_PRECIS,
       CLIP_DEFAULT_PRECIS,
       DEFAULT_QUALITY,
       DEFAULT_PITCH | FF_DONTCARE,
       L"Segoe UI"
   );

   htext = CreateWindowW(L"STATIC", L"  Является ли отношение функцией", WS_CHILD | WS_VISIBLE | WS_BORDER,
       50, 10, 400, 30, hWnd, NULL, hInstance, NULL);

   hEditSize = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
       130, 50, 700, 30, hWnd, NULL, hInstance, NULL);

   hEditRelation = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
       130, 90, 700, 250, hWnd, NULL, hInstance, NULL);

   hButton = CreateWindowW(L"BUTTON", L"Определить", WS_CHILD | WS_VISIBLE,
       130, 350, 100, 40, hWnd, (HMENU)1, hInstance, NULL);

   hOutput = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
       130, 410, 700, 400, hWnd, NULL, hInstance, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   SendMessage(hOutput, WM_SETFONT, (WPARAM)hFontOutput, TRUE);
   return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (LOWORD(wParam) == 1) {
            WCHAR bufferSize[64];
            GetWindowTextW(hEditSize, bufferSize, 64);

            int n = 0, m = 0;
            if (swscanf_s(bufferSize, L"%d %d", &n, &m) != 2 || n <= 0 || m <= 0) {
                SetWindowTextW(hOutput, L"Ошибка: введите корректные размеры n и m");
                break;
            }

            WCHAR bufferRelation[8192];
            GetWindowTextW(hEditRelation, bufferRelation, 8192);

            wstringstream ss(bufferRelation);
            wstring line;
            vector<wstring> matrix;

            while (getline(ss, line)) {
                line.erase(remove_if(line.begin(), line.end(), iswspace), line.end());

                if (!line.empty()) {
                    matrix.push_back(line);
                }
            }

            wstring debugInfo = L"Матрица:\r\n";
            for (const auto& row : matrix) {
                debugInfo += row + L"\r\n";
            }

            if ((int)matrix.size() != n) {
                debugInfo += L"\r\nОшибка: количество строк матрицы не совпадает с n";
                SetWindowTextW(hOutput, debugInfo.c_str());
                break;
            }

            bool func = isFunctionMatrix(matrix, n, m);
            debugInfo += L"\r\nРезультат: ";
            debugInfo += func ? L"отношение функция" : L"отношение не функция";

            SetWindowTextW(hOutput, debugInfo.c_str());
        }
        break;
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            SelectObject(hdc, hFontOutput); 

            TextOutW(hdc, 10, 50, L"Размер nxm", lstrlenW(L"Размер nxm"));
            TextOutW(hdc, 40, 90, L"nxm", lstrlenW(L"nxm"));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
