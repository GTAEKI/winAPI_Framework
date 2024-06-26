﻿#include "Core.h"
#include "Scene\SceneManager.h"
#include "Timer\Timer.h"
#include "PathManager.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Texture.h"
#include "Camera.h"
#include "Input.h"
#include "Collider/CollisionManager.h"

CCore* CCore::m_pInst = NULL;
bool CCore::m_bLoop = true;

CCore::CCore()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // 메모리 누수를 체크
	//_CrtSetBreakAlloc(191); //메모리 누수 있을때만 그 위치로 이동해주는것
}

CCore::~CCore() 
{
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CCamera);
	DESTROY_SINGLE(CTimer);
	DESTROY_SINGLE(CPathManager); 
	DESTROY_SINGLE(CResourcesManager);
	DESTROY_SINGLE(CInput);

	ReleaseDC(m_hWnd, m_hDC);
}

ATOM CCore::MyRegisterClass()
{
	windowClass.lpszClassName = TEXT("The Binding of Isaac");
	windowClass.lpszMenuName = NULL;

	windowClass.lpfnWndProc = CCore::WndProc;
	windowClass.hInstance = m_hInst;

	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;

	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;

	return RegisterClass(&windowClass);
}

BOOL CCore::Create()
{
	m_hWnd = CreateWindow(windowClass.lpszClassName, windowClass.lpszClassName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, m_hInst, NULL);
	if (!m_hWnd) 
	{
		return FALSE;
	}

	RECT rc = { 0,0,m_tRS.iW, m_tRS.iH};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

void CCore::Logic()
{
	// 타이머 갱신
	GET_SINGLE(CTimer)->Update();

	float fDeltaTime = GET_SINGLE(CTimer)->GetDeltaTime();

	Input(fDeltaTime);
	Update(fDeltaTime);
	LateUpdate(fDeltaTime);
	Collision(fDeltaTime);
	Render(fDeltaTime);
}

void CCore::Input(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->Input(fDeltaTime);
	GET_SINGLE(CCamera)->Input(fDeltaTime);
}	

int CCore::Update(float fDeltaTime)
{
	GET_SINGLE(CInput)->Update(fDeltaTime);
	GET_SINGLE(CSceneManager)->Update(fDeltaTime);
	GET_SINGLE(CCamera)->Update(fDeltaTime);
	return 0;
}

int CCore::LateUpdate(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->LateUpdate(fDeltaTime);
	return 0;
}

void CCore::Collision(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->Collision(fDeltaTime); // 충돌체가 다 모여있을것이다.

	GET_SINGLE(CCollisionManager)->Collision(fDeltaTime); // 콜리전 매니저의 콜리전이 호출된다.
}

void CCore::Render(float fDeltaTime)
{
	// 더블버퍼링
	CTexture* pBackBuffer = GET_SINGLE(CResourcesManager) -> GetBackBuffer();

	/*RECT clientRect;
	clientRect.left = 0;
	clientRect.right = 1280;
	clientRect.bottom = 720;
	clientRect.top = 0;
	FillRect(pBackBuffer->GetDC(), &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));*/

	Rectangle(pBackBuffer->GetDC(), 0, 0, 1280, 720);
	//GET_SINGLE(CSceneManager)->Render(m_hDC,fDeltaTime);
	GET_SINGLE(CSceneManager)->Render(pBackBuffer->GetDC(), fDeltaTime);

	BitBlt(m_hDC, 0, 0, m_tRS.iW, m_tRS.iH, pBackBuffer->GetDC(), 0, 0, SRCCOPY);

	SAFE_RELEASE(pBackBuffer);
}

bool CCore::Init(HINSTANCE hInst) 
{
	m_hInst = hInst;

	MyRegisterClass();

	// 해상도 설정
	m_tRS.iW = 1280;
	m_tRS.iH = 720;

	Create();

	// 화면 DC를 만들어준다.
	m_hDC = GetDC(m_hWnd);

	// 타이머 초기화
	if (!GET_SINGLE(CTimer)->init()) 
	{
		return false;
	}

	// 경로 관리자 초기화
	if (!GET_SINGLE(CPathManager)->Init())
	{
		return false;
	}

	if (!GET_SINGLE(CInput)->Init(m_hWnd)) 
	{
		return false;
	}

	// 리소스 관리자 초기화
	if (!GET_SINGLE(CResourcesManager)->Init(hInst,m_hDC)) 
	{
		return false;
	}

	// 카메라 관리자 초기화
	if (!GET_SINGLE(CCamera)->Init(POSITION(0.f, 0.f), m_tRS, RESOLUTION(2952, 1860)))
	{
		return false;
	}

	// 장면관리자 초기화
	if (!GET_SINGLE(CSceneManager)->Init())
	{
		return false;
	}

	return true;
}

int CCore::Run() 
{
	MSG msg;

	// 기본 메세지 루프
	while (m_bLoop) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우 데드타임일 경우
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

LRESULT CCore::WndProc(HWND hWnd, UINT messageType, WPARAM wParam, LPARAM lParam)
{
	switch (messageType)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// 여기에 hdc를 사용하는 그리기 코드 추가
		EndPaint(hWnd, &ps);
	}
	break;

	// 윈도우 종료시 들어오는 메세지
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, messageType, wParam, lParam);
	}
	return 0;
}
