#include<windows.h>
#include<stdio.h>
#include<time.h>
#include"resource.h"

//��Ϣ������
LRESULT CALLBACK MyWNDPROC(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

//����ͼƬ
HBITMAP hBitmap_Background;
HBITMAP hBitmap_Apple;
HBITMAP hBitmap_SnakeBody;
HBITMAP hBitmap_SnakeHead_Up;
HBITMAP hBitmap_SnakeHead_Down;
HBITMAP hBitmap_SnakeHead_Left;
HBITMAP hBitmap_SnakeHead_Right;

typedef struct NODE
{
	int x;
	int y;
	struct NODE*pNext;
	struct NODE*pLast;
}Snake,Apple;

enum DIRECTION {UP,DOWN,LEFT,RIGHT};

Snake* pHead = NULL;
Snake* pTail = NULL;
Apple apple = {5,5,NULL,NULL};


enum DIRECTION Direction = RIGHT;

void ShowBackGround(HDC hdc);
void ShowSnake(HDC hdc);
void AddNode(int x, int y);
void Move();
void ShowApple(HDC hdc);
BOOL IfEatApple();
void NewApple();
BOOL IfTouchWall();
BOOL IfEatSelf();



int CALLBACK WinMain(
					HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow
					)
{
	WNDCLASSEX ex; 
	HWND hWnd;				//���ھ��
	MSG msg;

	//����λͼ
	hBitmap_Background = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP1));
	hBitmap_Apple = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP2));
	hBitmap_SnakeBody = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP3));
	hBitmap_SnakeHead_Up = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP5));
	hBitmap_SnakeHead_Down = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP7));
	hBitmap_SnakeHead_Left = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP6));
	hBitmap_SnakeHead_Right = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP4));

	//��ʼ���� 
	AddNode(6,3);
	AddNode(5,3);
	AddNode(4,3);



	//1. ��ƴ���
	ex.style = CS_HREDRAW | CS_VREDRAW;
	ex.cbSize = sizeof(ex);
	ex.cbClsExtra = 0;
	ex.cbWndExtra = 0;
	ex.hInstance = hInstance;
	ex.hIcon = NULL;
	ex.hCursor = NULL;
	ex.hbrBackground = CreateSolidBrush(RGB(15,10,77));
	ex.hIconSm = NULL;
	ex.lpfnWndProc = &MyWNDPROC;
	ex.lpszMenuName = NULL;
	ex.lpszClassName = "Lazarus";


	//2.ע�ᴰ��
	RegisterClassEx(&ex);

	//3.��������
	hWnd = CreateWindow(ex.lpszClassName,"Lazarus0501",WS_OVERLAPPEDWINDOW,50,50,615,638,NULL,NULL,hInstance,NULL);

	//4.��ʾ����
	ShowWindow(hWnd,SW_SHOW);


	//���ö�ʱ��
	SetTimer(hWnd,1,200,NULL);

	//���������
	srand((unsigned int)time(NULL));

	//��Ϣѭ��

	while( GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}



	return 0;
}


LRESULT CALLBACK MyWNDPROC(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch(message)
	{
	case WM_CLOSE:					//��⵽�رյ���Ϣ
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:				//��⵽���̰��µ���Ϣ
		switch(wParam)				//wparam ����水���Ĳ���
		{
		case VK_UP:
			Direction = UP;
			break;
		case VK_DOWN:
			Direction = DOWN;
			break;
		case VK_LEFT:
			Direction = LEFT;
			break;
		case VK_RIGHT:
			Direction = RIGHT;
			break;
		}
		hdc = GetDC(hWnd);

		//��ʾ����
		ShowBackGround(hdc);
		//��ʾ��
		ShowSnake(hdc);
		//��ʾƻ��
		ShowApple(hdc);

		ReleaseDC(hWnd,hdc);
		break;
	case WM_PAINT:							//�ػ���Ϣ
		hdc = BeginPaint(hWnd,&ps);

		//��ʾ����
		ShowBackGround(hdc);
		//��ʾ��
		ShowSnake(hdc);
		//��ʾƻ��
		ShowApple(hdc);

		EndPaint(hWnd,&ps);

		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		//צ��
		Move();
		
		//�ж���Ϸ�Ƿ����
		//ҧ�Լ� ײǽ

		if(IfEatSelf() || IfTouchWall())
		{
			KillTimer(hWnd,1);
			MessageBox(hWnd,"��Ϸ����","warning",MB_OK);
		}

		//��û�Ե���ƻ��
		if(IfEatApple())
		{
			//�ڵ��1
			//�ڿ������ĵط���һ��
			//���ƶ����Զ��������������
			AddNode(-11,-11);
			//�µ�Apple
			NewApple();

		}

		//��ʾ����
		ShowBackGround(hdc);
		//��ʾ��
		ShowSnake(hdc);
		//��ʾƻ��
		ShowApple(hdc);

		ReleaseDC(hWnd,hdc);
		break;

	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

void ShowBackGround(HDC hdc)
{
	//����������DC
	HDC hdcmem = CreateCompatibleDC(hdc);
	//ѡ��һ��λͼ
	SelectObject(hdcmem,hBitmap_Background);
	//����
	BitBlt(hdc,0,0,600,600,hdcmem,0,0,SRCCOPY);
	//ɾ��������DC
	DeleteDC(hdcmem);

	return;
}


void AddNode(int x,int y)
{
	Snake* pTemp = (Snake*)malloc(sizeof ( Snake ) );
	pTemp->x = x;
	pTemp->y = y;
	pTemp->pNext = NULL;
	pTemp->pLast = NULL;

	if(pHead == NULL)
	{
		pHead = pTemp;
	}
	else
	{
		pTail->pNext = pTemp;
		pTemp->pLast = pTail;
	}

	pTail = pTemp;
	return;

}


void ShowSnake(HDC hdc)
{
	Snake *pMark = pHead->pNext;
	HDC hdcmem = CreateCompatibleDC(hdc);

	//1.����ͷ
	switch (Direction)
	{
	case UP:
		SelectObject(hdcmem,hBitmap_SnakeHead_Up);
		break;
	case DOWN:
		SelectObject(hdcmem,hBitmap_SnakeHead_Down);
		break;
	case LEFT:
		SelectObject(hdcmem,hBitmap_SnakeHead_Left);
		break;
	case RIGHT:
		SelectObject(hdcmem,hBitmap_SnakeHead_Right);
		break;
	default:
		break;
	}
	
	BitBlt(hdc,pHead->x*30,pHead->y*30,30,30,hdcmem,0,0,SRCCOPY);

	//2.������
	SelectObject(hdcmem,hBitmap_SnakeBody);
	while(pMark != NULL)
	{
		BitBlt(hdc,pMark->x*30,pMark->y*30,30,30,hdcmem,0,0,SRCCOPY);
		pMark = pMark->pNext;
	}

	DeleteDC(hdcmem);

	return;
}

void Move()
{
	Snake* pMark = pTail;
	//1.�ȶ�����
	while(pMark!= pHead)
	{
		pMark->x = pMark->pLast->x;
		pMark->y = pMark->pLast->y;

		pMark = pMark->pLast;
	}
	//2.�ƶ���ͷ
	switch (Direction)
	{
	case UP:
		pHead->y--;
		break;
	case DOWN:
		pHead->y++;
		break;
	case LEFT:
		pHead->x--;
		break;
	case RIGHT:
		pHead->x++;
		break;
	default:
		break;
	}

	return;
}

void ShowApple(HDC hdc)
{
	HDC hdcmem = CreateCompatibleDC(hdc);
	SelectObject(hdcmem,hBitmap_Apple);
	BitBlt(hdc,apple.x*30,apple.y*30,30,30,hdcmem,0,0,SRCCOPY);
	DeleteDC(hdcmem);

	return;
}

BOOL IfEatApple()
{
	if( pHead->x == apple.x && pHead->y == apple.y)
		return TRUE;

	return FALSE;
}


void NewApple()
{
	Snake* pMark = pHead;

	//����ͼ0-19
	//ȥ��ǽ ���ɵ�������1-18
	//�������APPle������
	int x;
	int y;

	do
	{
		x = rand() % 18 + 1;
		y = rand() % 18 + 1;

		//�ж��Ƿ�������
		pMark = pHead;
		while(pMark != NULL)
		{
			if(pMark->x == x && pMark->y == y)
				break;			//������������

			pMark = pMark->pNext;
		}

	}while(pMark != NULL);

	apple.x = x;
	apple.y = y;

	return;
}

BOOL IfTouchWall()
{
	if( pHead->x == 0 || pHead->x == 19 || pHead->y == 0 || pHead->y == 19)
		return TRUE;

	return FALSE;
}

BOOL IfEatSelf()
{
	Snake*pMark = pHead->pNext;

	while(pMark->pNext != NULL)
	{
		if(pMark->x == pHead->x && pMark->y == pHead->y)
			return TRUE;

		pMark = pMark->pNext;
	}

	return FALSE;
}
