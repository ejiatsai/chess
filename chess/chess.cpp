// chess.cpp : 定義應用程式的進入點。
//

#include "framework.h"
#include "chess.h"

#define MAX_LOADSTRING 100

// 全域變數:
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱

// 這個程式碼模組所包含之函式的向前宣告:
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

    // TODO: 在此放置程式碼。

    // 將全域字串初始化
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CHESS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 執行應用程式初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHESS));

    MSG msg;

    // 主訊息迴圈:
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
//  函式: MyRegisterClass()
//
//  用途: 註冊視窗類別。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHESS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHESS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函式: InitInstance(HINSTANCE, int)
//
//   用途: 儲存執行個體控制代碼並且建立主視窗
//
//   註解:
//
//        在這個函式中，我們將執行個體控制代碼儲存在全域變數中，
//        並建立及顯示主程式視窗。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函式: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  用途: 處理主視窗的訊息。
//
//  WM_COMMAND  - 處理應用程式功能表
//  WM_PAINT    - 繪製主視窗
//  WM_DESTROY  - 張貼結束訊息然後傳回
//
//
int ChessBoard[8][8] = {
    {-2,-3,-4,-5,-6,-4,-3,-2},
    {-1,-1,-1,-1,-1,-1,-1,-1},
    { 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0},
    { 0, 0, 0, 0, 0, 0, 0, 0},
    { 1, 1, 1, 1, 1, 1, 1, 1},
    { 2, 3, 4, 5, 6, 4, 3, 2},
};
/*  White:Pawn =  1, Rook =  2,Knight =  3,Bishop =  4,Queen =  5,King =  6
    Black:Pawn = -1, Rook = -2,Knight = -3,Bishop = -4,Queen = -5,King = -6
*/ 

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 剖析功能表選取項目:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此新增任何使用 hdc 的繪圖程式碼...
            int BoardSize = 80; // chess board size
            HBRUSH Light = CreateSolidBrush(RGB(238, 238, 210)); // white board
            HBRUSH Dark = CreateSolidBrush(RGB(118, 150, 86)); // green board
            SetBkMode(hdc, TRANSPARENT); 
            LOGFONT lf = { 0 };// initialization
            lf.lfHeight = 80; // set font size
            HFONT chess = CreateFontIndirect(&lf); // create font
            SelectObject(hdc, chess);
            for (int r = 0;r < 8;r++) {
                for (int c = 0;c < 8;c++) {
                    RECT rect;
                    rect.left = c * BoardSize;
                    rect.top = r * BoardSize;
                    rect.right = c * BoardSize + BoardSize;
                    rect.bottom = r * BoardSize + BoardSize;
                    if ((r + c) % 2 == 0) {
                        FillRect(hdc, &rect, Light);
                    }
                    else {
                        FillRect(hdc, &rect, Dark);
                    }
                    switch (ChessBoard[r][c]) {
                        case -2:
                            DrawText(hdc, L"♜", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case -3:
                            DrawText(hdc, L"♞", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case -4:
                            DrawText(hdc, L"♝", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case -5:
                            DrawText(hdc, L"♛", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case -6:
                            DrawText(hdc, L"♚", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case -1:
                            DrawText(hdc, L"♟", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case 1:
                            DrawText(hdc, L"♙", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case 2:
                            DrawText(hdc, L"♖", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case 3:
                            DrawText(hdc, L"♘", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case 4:
                            DrawText(hdc, L"♗", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case 5:
                            DrawText(hdc, L"♕", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                        case 6:
                            DrawText(hdc, L"♔", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                    }
                }
            } // create chess board
            DeleteObject(Light); // release memory
            DeleteObject(Dark); // release memory
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

// [關於] 方塊的訊息處理常式。
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
