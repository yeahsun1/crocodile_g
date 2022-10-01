// 202007014이예선_Project.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "202007014이예선_Project.h"


#include <stdlib.h>     // 랜덤 값 획득 함수 호출
#include <time.h>       // SEED 값 조절을 위해 현재 시간 값 이용
#include <random>       //난수 생성기를 사용하기 위해서 선언하는 헤더파일


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY202007014PROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY202007014PROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY202007014PROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY202007014PROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// 악어의 입술, 이빨 좌표 변수
RECT g_eye[2], g_mouth[2], g_teeth[10];
int g_x, g_y;

// 상대의 속도를 가질 변수
int g_speed;

// 점수
int g_score;

// 타이머
int g_timer;

// 마우스가 클릭 되었는지 확인
int g_flag;  

// 사각형 색상 지정
int g_color;           


// 타이머 아이디를 선언
#define TIMER_ID_1          1       // 상대 이동을 위한 타이머 아이디
#define TIMER_ID_2          2

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
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
        //악어 눈 사각형 설정
        g_eye[0].left = 390;
        g_eye[0].top = 110;
        g_eye[0].right = 410;
        g_eye[0].bottom = 190;

        g_eye[1].left = 590;
        g_eye[1].top = 110;
        g_eye[1].right = 610;
        g_eye[1].bottom = 190;


        // 악어의 입술 사각형 설정
        g_mouth[0].left = 300;
        g_mouth[0].top = 150;
        g_mouth[0].right = 700;
        g_mouth[0].bottom = 270;

        g_mouth[1].left = 300;
        g_mouth[1].top = 350;
        g_mouth[1].right = 700;
        g_mouth[1].bottom = 400;

        // 악어의 이빨 좌표 구성
        //윗 이빨
        g_teeth[0].left = 300;
        g_teeth[0].top = 220;
        g_teeth[0].right = g_teeth[0].left + 40;
        g_teeth[0].bottom = g_teeth[0].top + 50;

        //// SEED 값 초기화
        //srand(time(NULL));

        g_teeth[1].left = 390;
        g_teeth[1].top = 220;
        g_teeth[1].right = g_teeth[1].left + 40;
        g_teeth[1].bottom = g_teeth[1].top + 50;

        g_teeth[2].left = 480;
        g_teeth[2].top = 220;
        g_teeth[2].right = g_teeth[2].left + 40;
        g_teeth[2].bottom = g_teeth[2].top + 50;

        g_teeth[3].left = 570;
        g_teeth[3].top = 220;
        g_teeth[3].right = g_teeth[3].left + 40;
        g_teeth[3].bottom = g_teeth[3].top + 50;

        g_teeth[4].left = 660;
        g_teeth[4].top = 220;
        g_teeth[4].right = g_teeth[4].left + 40;
        g_teeth[4].bottom = g_teeth[4].top + 50;


        //아랫 이빨
        g_teeth[5].left = 300;
        g_teeth[5].top = 350;
        g_teeth[5].right = g_teeth[5].left + 40;
        g_teeth[5].bottom = g_teeth[5].top + 50;

        g_teeth[6].left = 390;
        g_teeth[6].top = 350;
        g_teeth[6].right = g_teeth[6].left + 40;
        g_teeth[6].bottom = g_teeth[6].top + 50;

        g_teeth[7].left = 480;
        g_teeth[7].top = 350;
        g_teeth[7].right = g_teeth[7].left + 40;
        g_teeth[7].bottom = g_teeth[7].top + 50;

        g_teeth[8].left = 570;
        g_teeth[8].top = 350;
        g_teeth[8].right = g_teeth[8].left + 40;
        g_teeth[8].bottom = g_teeth[8].top + 50;

        g_teeth[9].left = 660;
        g_teeth[9].top = 350;
        g_teeth[9].right = g_teeth[9].left + 40;
        g_teeth[9].bottom = g_teeth[9].top + 50;


        // 이빨이 움직이는 속도 설정. 2초
        g_speed = 2000;
        // 타이머를 기동 --> 주기적으로 WM_TIMER를 OS가 호출해준다.
        SetTimer(hWnd, TIMER_ID_1, g_speed, NULL);

        // 게임 타이머 시간 설정. 무조건 1초에 한번씩만 불려야 함
        g_timer = 20;       // 제한 시간 20초
        SetTimer(hWnd, TIMER_ID_2, 1000, NULL);
        break;
    }
    case WM_TIMER:
    {
        //RECT dst;       // 겹침 영역의 좌표를 획득

        // WM_TIMER 메시지는 wParam에 SetTimer에서 설정한 ID 값이 넘겨져 온다!
        switch (wParam)
        {
            // 악어의 이빨이 주기적으로 움직이는 타이머가 도착했다!
        case TIMER_ID_1:
            if (g_speed != 1000)
            {
                g_speed += 1000;     // 0.1초씩 감소

                // 먼저 현재 동작 중인 타이머를 제거
                KillTimer(hWnd, TIMER_ID_1);
                // 다시 새롭게 타이머를 설정
                SetTimer(hWnd, TIMER_ID_1, g_speed, NULL);
            }

            //윗 이빨의 움직임
            //for (int i = 0; i < 5; i++)
            //{
            //    if (g_teeth[i].top <= 250) //이빨이 나올 준비 완료
            //        // 이빨이 나오기
            //        if (g_teeth[i].bottom - g_teeth[i].top <= 30)// bottom - top 거리가 30보다 작거나 같으면
            //        {
            //            g_teeth[i].top += 30;
            //            g_teeth[i].bottom += 30;
            //        }
            //        else // 다 나옴
            //        {
            //            g_teeth[i].top -= 30;
            //            g_teeth[i].bottom -= 30;
            //        }
            //    else // 이빨이 밖에 나와있으면 다 들어가기..
            //    {
            //        if (g_teeth[i].top > g_mouth[i].bottom)
            //        {
            //            g_teeth[i].top -= 30;
            //            g_teeth[i].bottom -= 30;
            //        }
            //        
            //    }
            //}

            int i, j;
            int ran;
            for (i = 0; i < 10; i++)
            {
                for (j = 0; j < 2; j++) {
                    if (g_teeth[i].top <= g_mouth[j].bottom)
                    {
                        ran = rand() % 10;
                        g_teeth[ran].top = g_teeth[i].top - 30;

                        if (g_teeth[i].top = 320) {
                            ran = rand() % 5;
                            g_teeth[ran].top = g_teeth[i].top + 30;
                            break;
                        }
                    }





                }
                break;

                // 게임 타이머 감소

        case TIMER_ID_2:
            g_timer--;
            if (0 == g_timer)
            {
                KillTimer(hWnd, TIMER_ID_1);
                KillTimer(hWnd, TIMER_ID_2);
                MessageBox(hWnd, L"시간으로 종료", L"게임 오버", MB_OK);
            }
            break;
            }
            // 화면 무효화
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

        // 왼쪽 마우스 클릭
    case WM_LBUTTONDOWN:
    {
        /*RECT dst;*/
        // 좌표 획득
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);

        // 프로그래머에 의한 화면 무효화 요청: 전체 Client 영역
        // --> 내부적으로 WM_PAINT가 호출 된다.
        InvalidateRect(hWnd, NULL, TRUE);
        // 왼쪽 마우스 클릭에 대한 처리 종료

         // 초기 상대 속도 설정. 1초
        g_speed = 1000;
        // 타이머를 기동 --> 주기적으로 WM_TIMER를 OS가 호출해준다.
        SetTimer(hWnd, TIMER_ID_1, g_speed, NULL);

        // 게임 타이머 시간 설정. 무조건 1초에 한번씩만 불려야 함
        g_timer = 10;       // 제한 시간 10초
        SetTimer(hWnd, TIMER_ID_2, 1000, NULL);
    }
    break;

    case WM_PAINT:
    {
        int i = 0;
        WCHAR score[64] = { 0, };
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        // 점수 출력
        wsprintfW(score, L"Score : %d\t Timer: %d", g_score, g_timer);
        TextOut(hdc, 440, 490, score, lstrlenW(score));

        // 악어 이빨 그리기
        for (i = 0; i < 10; i++)
            Rectangle(hdc, g_teeth[i].left, g_teeth[i].top, g_teeth[i].right, g_teeth[i].bottom);

        // 악어 입술 그리기
        for (i = 0; i < 2; i++)
        {
            Rectangle(hdc, g_mouth[i].left, g_mouth[i].top, g_mouth[i].right, g_mouth[i].bottom);
            HBRUSH brush = CreateSolidBrush(RGB(47, 157, 39));
            FillRect(hdc, &g_mouth[i], brush);

        }

        // 악어 눈 원 그리기
        Ellipse(hdc, 350, 100, 450, 200);
        Ellipse(hdc, 550, 100, 650, 200);

        // 눈 사각형 그리기
        for (i = 0; i < 2; i++)
        {
            Rectangle(hdc, g_eye[i].left, g_eye[i].top, g_eye[i].right, g_eye[i].bottom);
            HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &g_eye[i], brush);
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
    }
}

// 정보 대화 상자의 메시지 처리기입니다.
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
