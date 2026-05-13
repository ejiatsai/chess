// chess.cpp : 定義應用程式的進入點。
//

#include "framework.h"
#include "chess.h"
#include "chessmen.h"
#include "resource.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100
// 全域變數:
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱
static int clickCol = -1;
static int clickRow = -1;
static int currentCol = -1;
static int currentRow = -1;
static int turn = 1;
static int pawnTwo = -1;
static int BoardSize = 80;
/* record the user click position
*/
bool Move[8][8] = { FALSE };
bool Attack[8][8] = { FALSE };

Chess ChessBoard[8][8] = {
    { {1, -1, 0}, {2, -1, 0}, {3, -1, 0}, {4, -1, 0}, {5, -1, 0}, {3, -1, 0}, {2, -1, 0}, {1, -1, 0} },
    { {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0} },
    { {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0} },
    { {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0} },
    { {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0} },
    { {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0}, {-1,  0, 0} },
    { {0,  1, 0}, {0,  1, 0}, {0,  1, 0}, {0,  1, 0}, {0,  1, 0}, {0,  1, 0}, {0,  1, 0}, {0,  1, 0} },
    { {1,  1, 0}, {2,  1, 0}, {3,  1, 0}, {4,  1, 0}, {5,  1, 0}, {3,  1, 0}, {2,  1, 0}, {1,  1, 0} }
};
/*  Pawn = 0, Rook = 1, Knight = 2, Bishop = 3, Queen =  4, King =  5
    White = 1,Black = -1
    space = -1;
    not moved = 0,not moved = 1;
*/

chessmen chessmove;

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

INT_PTR CALLBACK Promotion(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG:
        return true;
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BTN_ROOK:
            EndDialog(hwndDlg, 1);
            return TRUE;
        case IDC_BTN_KNIGHT:
            EndDialog(hwndDlg, 2);
            return TRUE;
        case IDC_BTN_BISHOP:
            EndDialog(hwndDlg, 3);
            return TRUE;
        case IDC_BTN_QUEEN:
            EndDialog(hwndDlg, 4);
            return TRUE;
        case IDCANCEL:
            EndDialog(hwndDlg, 4);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

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
    case WM_CREATE: 
        {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        BoardSize = screenHeight / 8;
        SetMenu(hWnd, NULL);
        SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(hWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED);
        return 0;
        }
        break;
    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hWnd);
        }
    }
    break;
    case WM_RBUTTONDOWN:
    {
        for (int r = 0;r < 8;r++) {
            for (int c = 0;c < 8;c++) {
                Move[r][c] = FALSE;
                Attack[r][c] = FALSE;
            }
        }
        currentCol = -1;
        currentRow = -1;
        clickCol = -1;
        clickRow = -1;
        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);

    }
    break;
    case WM_LBUTTONDOWN: 
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        int col = xPos / BoardSize;
        int row = yPos / BoardSize;
        if (col >= 0 && col < 8 && row >= 0 && row < 8) {
            if (Move[row][col] == TRUE || Attack[row][col] == TRUE) {
                bool Moved = FALSE;
                bool promotion = FALSE;
                int p = 0;
                if (ChessBoard[currentRow][currentCol].type == 0) {
                    if (currentCol != col && currentRow != row && ChessBoard[row][col].type == -1 && col == pawnTwo) {
                        if (row == 2 && Attack[3][col] == TRUE) {
                            chessmove.move(ChessBoard, currentCol, currentRow, col, row);
                            ChessBoard[3][col] = { -1,0,0 };
                            Moved = TRUE;
                        }
                        else if (row == 5 && Attack[4][col] == TRUE) {
                            chessmove.move(ChessBoard, currentCol, currentRow, col, row);
                            ChessBoard[4][col] = { -1,0,0 };
                            Moved = TRUE;
                        }
                    }
                    if (!Moved && currentRow != row) {
                        chessmove.move(ChessBoard, currentCol, currentRow, col, row);
                        if (row == 0) {
                            promotion = TRUE;
                            p = 1;
                        }
                        else if (row == 7) {
                            promotion = TRUE;
                            p = -1;
                        }
                        if (abs(currentRow - row) == 2) {
                            pawnTwo = col;
                        }
                        else {
                            pawnTwo = -1;
                        }
                        Moved = TRUE;
                    }
                    else {
                        pawnTwo = -1;
                    }
                }
                else {
                    pawnTwo = -1;
                    chessmove.move(ChessBoard, currentCol, currentRow, col, row);
                    Moved = TRUE;
                }
                if (Moved) {
                    ChessBoard[row][col].ismoved++;
                    currentCol = -1;
                    currentRow = -1;
                    clickCol = -1;
                    clickRow = -1;
                    for (int r = 0;r < 8;r++) {
                        for (int c = 0;c < 8;c++) {
                            Move[r][c] = FALSE;
                            Attack[r][c] = FALSE;
                        }
                    }
                    turn *= -1;
                }
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
                if (promotion) {
                    int promotionPiece = DialogBox(hInst, MAKEINTRESOURCE(IDD_PROMOTION), hWnd, Promotion);
                    ChessBoard[row][col] = { promotionPiece,p,0 };
                }
            }
            if (turn == 1 && ChessBoard[row][col].color == 1) {
                for (int r = 0;r < 8;r++) {
                    for (int c = 0;c < 8;c++) {
                        Move[r][c] = FALSE;
                        Attack[r][c] = FALSE;
                    }
                }
                switch (ChessBoard[row][col].type) {
                case 0: // pawn
                {
                    if (ChessBoard[row][col].ismoved == 0) {
                        if (row - 2 >= 0 && row - 2 < 8 && col >= 0 && col < 8 && ChessBoard[row - 2][col].type == -1 && ChessBoard[row - 1][col].type == -1) {
                            Move[row - 2][col] = TRUE;
                            Move[row - 1][col] = TRUE;
                        }
                        else if (row - 1 >= 0 && row - 1 < 8 && col >= 0 && col < 8 && ChessBoard[row - 1][col].type == -1) {
                            Move[row - 1][col] = TRUE;
                        }
                        if (row - 1 >= 0 && row - 1 < 8 && col - 1 >= 0 && col - 1 < 8 && ChessBoard[row - 1][col - 1].type != -1 && ChessBoard[row - 1][col - 1].color != 1) {
                            Attack[row - 1][col - 1] = TRUE;
                        }
                        if (row - 1 >= 0 && row - 1 < 8 && col + 1 >= 0 && col + 1 < 8 && ChessBoard[row - 1][col + 1].type != -1 && ChessBoard[row - 1][col + 1].color != 1) {
                            Attack[row - 1][col + 1] = TRUE;
                        }
                    }
                    else {
                        if (row - 1 >= 0 && row - 1 < 8 && col >= 0 && col < 8 && ChessBoard[row - 1][col].type == -1) {
                            Move[row - 1][col] = TRUE;
                        }
                        if (row - 1 >= 0 && row - 1 < 8 && col - 1 >= 0 && col - 1 < 8 && ChessBoard[row - 1][col - 1].type != -1 && ChessBoard[row - 1][col - 1].color != 1) {
                            Attack[row - 1][col - 1] = TRUE;
                        }
                        if (row - 1 >= 0 && row - 1 < 8 && col + 1 >= 0 && col + 1 < 8 && ChessBoard[row - 1][col + 1].type != -1 && ChessBoard[row - 1][col + 1].color != 1) {
                            Attack[row - 1][col + 1] = TRUE;
                        }
                    }
                    if (row == 3) {
                        if (col - 1 >= 0 && col - 1 == pawnTwo) {
                            if (ChessBoard[3][col - 1].type == 0 && ChessBoard[3][col - 1].color == -1 && ChessBoard[2][col - 1].type == -1) {
                                Move[2][col - 1] = TRUE;
                                Attack[3][col - 1] = TRUE;
                            }
                        }
                        if (col + 1 < 8 && col + 1 == pawnTwo) {
                            if (ChessBoard[3][col + 1].type == 0 && ChessBoard[3][col + 1].color == -1 && ChessBoard[2][col + 1].type == -1) {
                                Move[2][col + 1] = TRUE;
                                Attack[3][col + 1] = TRUE;
                            }
                        }
                    }
                }
                    break;
                case 1: //rook
                {
                    int dr[4] = { 1,-1,0,0 };
                    int dc[4] = { 0,0,1,-1 };
                    for (int i = 0;i < 4;i++) {
                        for (int r = row + dr[i], c = col + dc[i];r >= 0 && r < 8 && c >= 0 && c < 8;r += dr[i], c += dc[i]) {
                            if (ChessBoard[r][c].type == -1) {
                                Move[r][c] = TRUE;
                            }
                            else {
                                if (ChessBoard[r][c].color != 1) {
                                    Attack[r][c] = TRUE;
                                    break;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                    }
                }
                    break;
                case 2: // kinght
                {
                    int dr[8] = { -2,-2,-1,1,2,2,1,-1 };
                    int dc[8] = { 1,-1,2,2,1,-1,-2,-2 };
                    for (int i = 0;i < 8;i++) {
                        int moveR = row + dr[i];
                        int moveC = col + dc[i];
                        if (moveR >= 0 && moveR < 8 && moveC >= 0 && moveC < 8) {
                            if (ChessBoard[moveR][moveC].type == -1) {
                                Move[moveR][moveC] = TRUE;
                            }
                            else if (ChessBoard[moveR][moveC].color != 1) {
                                Attack[moveR][moveC] = TRUE;
                            }
                        }
                    }
                }
                    break;
                case 3: // bishop
                {
                    int dr[4] = { 1,1,-1,-1 };
                    int dc[4] = { 1,-1,1,-1 };
                    for (int i = 0;i < 4;i++) {
                        for (int r = row + dr[i], c = col + dc[i];r >= 0 && r < 8 && c >= 0 && c < 8;r += dr[i], c += dc[i]) {
                            if (ChessBoard[r][c].type == -1) {
                                Move[r][c] = TRUE;
                            }
                            else {
                                if (ChessBoard[r][c].color != 1) {
                                    Attack[r][c] = TRUE;
                                    break;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                    }
                }
                    break;
                case 4: // queen
                {
                    int dr[8] = { 1,1,-1,-1,1,-1,0,0 };
                    int dc[8] = { 1,-1,1,-1,0,0,1,-1 };
                    for (int i = 0;i < 8;i++) {
                        for (int r = row + dr[i], c = col + dc[i];r >= 0 && r < 8 && c >= 0 && c < 8;r += dr[i], c += dc[i]) {
                            if (ChessBoard[r][c].type == -1) {
                                Move[r][c] = TRUE;
                            }
                            else {
                                if (ChessBoard[r][c].color != 1) {
                                    Attack[r][c] = TRUE;
                                    break;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                    }
                }
                    break;
                case 5: // king
                {
                    int dr[8] = { 1,1,-1,-1,1,-1,0,0 };
                    int dc[8] = { 1,-1,1,-1,0,0,1,-1 };
                    for (int i = 0;i < 8;i++) {
                        int moveR = row + dr[i];
                        int moveC = col + dc[i];
                        bool isKing = FALSE;
                        if (moveR >= 0 && moveR < 8 && moveC >= 0 && moveC < 8) {
                            for (int j = 0;j < 8;j++) {
                                int kR = moveR + dr[j];
                                int kC = moveC + dc[j];
                                if (kR >= 0 && kR < 8 && kC >= 0 && kC < 8) {
                                    if (ChessBoard[kR][kC].type == 5 && ChessBoard[kR][kC].color == -1) {
                                        isKing = TRUE;
                                        break;
                                    }
                                }
                            }
                            if (ChessBoard[moveR][moveC].type == -1 && isKing == FALSE) {
                                Move[moveR][moveC] = TRUE;
                            }
                            else if (ChessBoard[moveR][moveC].color != 1 && isKing == FALSE) {
                                Attack[moveR][moveC] = TRUE;
                            }
                        }
                    }
                }
                    break;
                }
                clickCol = col;
                clickRow = row;
                currentRow = row;
                currentCol = col;
            }
            else if (turn == -1 && ChessBoard[row][col].color == -1) {
                for (int r = 0;r < 8;r++) {
                    for (int c = 0;c < 8;c++) {
                        Move[r][c] = FALSE;
                        Attack[r][c] = FALSE;
                    }
                }
                switch (ChessBoard[row][col].type) {
                case 0: // pawn
                {
                    if (ChessBoard[row][col].ismoved == 0) {
                        if (row + 2 >= 0 && row + 2 < 8 && col >= 0 && col < 8 && ChessBoard[row + 2][col].type == -1 && ChessBoard[row + 1][col].type == -1) {
                            Move[row + 2][col] = TRUE;
                            Move[row + 1][col] = TRUE;
                        }
                        else if (row + 1 >= 0 && row + 1 < 8 && col >= 0 && col < 8 && ChessBoard[row + 1][col].type == -1) {
                            Move[row + 1][col] = TRUE;
                        }
                        if (row + 1 >= 0 && row + 1 < 8 && col - 1 >= 0 && col - 1 < 8 && ChessBoard[row + 1][col - 1].type != -1 && ChessBoard[row + 1][col - 1].color != -1) {
                            Attack[row + 1][col - 1] = TRUE;
                        }
                        if (row + 1 >= 0 && row + 1 < 8 && col + 1 >= 0 && col + 1 < 8 && ChessBoard[row + 1][col + 1].type != -1 && ChessBoard[row + 1][col + 1].color != -1) {
                            Attack[row + 1][col + 1] = TRUE;
                        }
                    }
                    else {
                        if (row + 1 >= 0 && row + 1 < 8 && col >= 0 && col < 8 && ChessBoard[row + 1][col].type == -1) {
                            Move[row + 1][col] = TRUE;
                        }
                        if (row + 1 >= 0 && row + 1 < 8 && col - 1 >= 0 && col - 1 < 8 && ChessBoard[row + 1][col - 1].type != -1 && ChessBoard[row + 1][col - 1].color != -1) {
                            Attack[row + 1][col - 1] = TRUE;
                        }
                        if (row + 1 >= 0 && row + 1 < 8 && col + 1 >= 0 && col + 1 < 8 && ChessBoard[row + 1][col + 1].type != -1 && ChessBoard[row + 1][col + 1].color != -1) {
                            Attack[row + 1][col + 1] = TRUE;
                        }
                    }
                    if (row == 4) {
                        if (col - 1 >= 0 && col - 1 == pawnTwo) {
                            if (ChessBoard[4][col - 1].type == 0 && ChessBoard[4][col - 1].color == -1) {
                                Move[5][col - 1] = TRUE;
                                Attack[4][col - 1] = TRUE;
                            }
                        }
                        if (col + 1 < 8 && col + 1 == pawnTwo) {
                            if (ChessBoard[4][col + 1].type == 0 && ChessBoard[4][col + 1].color == -1) {
                                Move[5][col + 1] = TRUE;
                                Attack[4][col + 1] = TRUE;
                            }
                        }
                    }
                }
                    break;
                case 1: // rook
                {
                    int dr[4] = { 1,-1,0,0 };
                    int dc[4] = { 0,0,1,-1 };
                    for (int i = 0;i < 4;i++) {
                        for (int r = row + dr[i], c = col + dc[i];r >= 0 && r < 8 && c >= 0 && c < 8;r += dr[i], c += dc[i]) {
                            if (ChessBoard[r][c].type == -1) {
                                Move[r][c] = TRUE;
                            }
                            else {
                                if (ChessBoard[r][c].color != -1) {
                                    Attack[r][c] = TRUE;
                                    break;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                    }
                }
                    break;
                case 2: // kinght
                {
                    int dr[8] = { -2,-2,-1,1,2,2,1,-1 };
                    int dc[8] = { 1,-1,2,2,1,-1,-2,-2 };
                    for (int i = 0;i < 8;i++) {
                        int moveR = row + dr[i];
                        int moveC = col + dc[i];
                        if (moveR >= 0 && moveR < 8 && moveC >= 0 && moveC < 8) {
                            if (ChessBoard[moveR][moveC].type == -1) {
                                Move[moveR][moveC] = TRUE;
                            }
                            else if (ChessBoard[moveR][moveC].color != -1) {
                                Attack[moveR][moveC] = TRUE;
                            }
                        }
                    }
                }
                    break;
                case 3: // bishop
                {
                    int dr[4] = { 1,1,-1,-1 };
                    int dc[4] = { 1,-1,1,-1 };
                    for (int i = 0;i < 4;i++) {
                        for (int r = row + dr[i], c = col + dc[i];r >= 0 && r < 8 && c >= 0 && c < 8;r += dr[i], c += dc[i]) {
                            if (ChessBoard[r][c].type == -1) {
                                Move[r][c] = TRUE;
                            }
                            else {
                                if (ChessBoard[r][c].color != -1) {
                                    Attack[r][c] = TRUE;
                                    break;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                    }
                }
                    break;
                case 4: // queen
                {
                    int dr[8] = { 1,1,-1,-1,1,-1,0,0 };
                    int dc[8] = { 1,-1,1,-1,0,0,1,-1 };
                    for (int i = 0;i < 8;i++) {
                        for (int r = row + dr[i], c = col + dc[i];r >= 0 && r < 8 && c >= 0 && c < 8;r += dr[i], c += dc[i]) {
                            if (ChessBoard[r][c].type == -1) {
                                Move[r][c] = TRUE;
                            }
                            else {
                                if (ChessBoard[r][c].color != -1) {
                                    Attack[r][c] = TRUE;
                                    break;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                    }
                }
                    break;
                case 5: // king
                {
                    int dr[8] = { 1,1,-1,-1,1,-1,0,0 };
                    int dc[8] = { 1,-1,1,-1,0,0,1,-1 };
                    for (int i = 0;i < 8;i++) {
                        int moveR = row + dr[i];
                        int moveC = col + dc[i];
                        bool isKing = FALSE;
                        if (moveR >= 0 && moveR < 8 && moveC >= 0 && moveC < 8) {
                            for (int j = 0;j < 8;j++) {
                                int kR = moveR + dr[j];
                                int kC = moveC + dc[j];
                                if (kR >= 0 && kR < 8 && kC >= 0 && kC < 8) {
                                    if (ChessBoard[kR][kC].type == 5 && ChessBoard[kR][kC].color == 1) {
                                        isKing = TRUE;
                                        break;
                                    }
                                }
                            }
                            if (ChessBoard[moveR][moveC].type == -1 && isKing == FALSE) {
                                Move[moveR][moveC] = TRUE;
                            }
                            else if (ChessBoard[moveR][moveC].color != -1 && isKing == FALSE) {
                                Attack[moveR][moveC] = TRUE;
                            }
                        }
                    }
                }
                    break;
                }
                clickCol = col;
                clickRow = row;
                currentRow = row;
                currentCol = col;
            }
        }
        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此新增任何使用 hdc 的繪圖程式碼...
            HBRUSH Light = CreateSolidBrush(RGB(238, 238, 210)); // white board
            HBRUSH Dark = CreateSolidBrush(RGB(118, 150, 86)); // green board
            HPEN click = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
            HPEN move = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HPEN attack = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
            HBRUSH hHollowBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
            HBRUSH Black = CreateSolidBrush(RGB(0, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            LOGFONT lf = { 0 };// initialization
            lf.lfHeight = BoardSize; // set font size
            LOGFONT ro = { 0 };
            ro.lfHeight = BoardSize / 2;
            LOGFONT cl = { 0 };
            cl.lfHeight = BoardSize / 5;
            HFONT close = CreateFontIndirect(&cl);
            HFONT round = CreateFontIndirect(&ro);
            HFONT chess = CreateFontIndirect(&lf); // create font
            RECT R;
            R.left = 9 * BoardSize;
            R.top = 0 * BoardSize;
            R.right = 12 * BoardSize;
            R.bottom = 1 * BoardSize;
            SelectObject(hdc, round);
            FillRect(hdc, &R, (HBRUSH)(COLOR_WINDOW + 1));
            if (turn == 1) {
                DrawText(hdc, L"Round:White", -1, &R, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            else {
                DrawText(hdc, L"Round:Black", -1, &R, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            SelectObject(hdc, close);
            RECT C;
            C.left = 11 * BoardSize;
            C.top = 7 * BoardSize;
            C.right = 13 * BoardSize;
            C.bottom = 8 * BoardSize;
            DrawText(hdc, L"Press ESC to close", -1, &C, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
            SelectObject(hdc, chess);
            for (int r = 0;r < 8;r++) {
                for (int c = 0;c < 8;c++) {
                    RECT rect;
                    rect.left = c * BoardSize;
                    rect.top = r * BoardSize;
                    rect.right = c * BoardSize + BoardSize;
                    rect.bottom = r * BoardSize + BoardSize;
                    if (clickRow == r && clickCol == c) {
                        if (ChessBoard[clickRow][clickCol].type != -1) {
                            if ((r + c) % 2 == 0) {
                                SelectObject(hdc, Light);
                            }
                            else {
                                SelectObject(hdc, Dark);
                            }
                            SelectObject(hdc, click);
                            Rectangle(hdc, rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1);
                        }
                        else {
                            if ((r + c) % 2 == 0) {
                                FillRect(hdc, &rect, Light);
                            }
                            else {
                                FillRect(hdc, &rect, Dark);
                            }
                        }
                    }
                    else {
                        if ((r + c) % 2 == 0) {
                            FillRect(hdc, &rect, Light);
                        }
                        else {
                            FillRect(hdc, &rect, Dark);
                        }
                    }
                    switch (ChessBoard[r][c].type) {
                        case 0:
                            if (ChessBoard[r][c].color == -1) {
                                DrawText(hdc, L"♟", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            else {
                                DrawText(hdc, L"♙", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            break;
                        case 1:
                            if (ChessBoard[r][c].color == -1) {
                                DrawText(hdc, L"♜", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            else {
                                DrawText(hdc, L"♖", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            break;
                        case 2:
                            if (ChessBoard[r][c].color == -1) {
                                DrawText(hdc, L"♞", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            else {
                                DrawText(hdc, L"♘", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            break;
                        case 3:
                            if (ChessBoard[r][c].color == -1) {
                                DrawText(hdc, L"♝", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            else {
                                DrawText(hdc, L"♗", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            break;
                        case 4:
                            if (ChessBoard[r][c].color == -1) {
                                DrawText(hdc, L"♛", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            else {
                                DrawText(hdc, L"♕", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            break;
                        case 5:
                            if (ChessBoard[r][c].color == -1) {
                                DrawText(hdc, L"♚", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            else {
                                DrawText(hdc, L"♔", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            }
                            break;
                        case -1:
                            DrawText(hdc, L"", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                            break;
                    }
                }
            } // create chess board
            for (int r = 0;r < 8;r++) {
                for (int c = 0;c < 8;c++) {
                    int left = (c * BoardSize) + (BoardSize / 2);
                    int top = (r * BoardSize) + (BoardSize / 2);
                    int right = (c * BoardSize) + (BoardSize / 2);
                    int bottom = (r * BoardSize) + (BoardSize / 2);
                    if (Move[r][c] == TRUE) {
                        SelectObject(hdc, Black);
                        SelectObject(hdc, move);
                        Ellipse(hdc, left + 10, top + 10, right - 10, bottom - 10);
                    }
                    if (Attack[r][c] == TRUE) {
                        SelectObject(hdc, attack);
                        SelectObject(hdc, hHollowBrush);
                        Ellipse(hdc, left + ((BoardSize / 2) + 1), top + ((BoardSize / 2) + 1), right - ((BoardSize / 2) + 1), bottom - ((BoardSize / 2) + 1));
                    }
                }
            }
            DeleteObject(Light); // release memory
            DeleteObject(Dark); 
            DeleteObject(click);
            DeleteObject(Black);
            DeleteObject(chess);
            DeleteObject(move);
            DeleteObject(attack);
            DeleteObject(round);
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
