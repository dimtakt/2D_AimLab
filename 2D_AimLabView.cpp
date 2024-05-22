
// 2D_AimLabView.cpp: CMy2DAimLabView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "2D_AimLab.h"
#endif

#include "2D_AimLabDoc.h"
#include "2D_AimLabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2DAimLabView

IMPLEMENT_DYNCREATE(CMy2DAimLabView, CView)

BEGIN_MESSAGE_MAP(CMy2DAimLabView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMy2DAimLabView 생성/소멸

CMy2DAimLabView::CMy2DAimLabView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// *********************************************************

	// 현재 표시할 화면이 무엇인지 제어 (int)
	// 예시)  0:메인 1:게임화면 2:랭킹..
	windowStatus = 0;

	// 랜덤 원이 생성될 위치 배열 (vector<CPoint>)
	aimMatrix;

	// 랜덤 초기화
	srand((unsigned)time(NULL));		


	// *********************************************************
}

CMy2DAimLabView::~CMy2DAimLabView()
{
}

BOOL CMy2DAimLabView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMy2DAimLabView 그리기

void CMy2DAimLabView::OnDraw(CDC* /*pDC*/)
{
	CMy2DAimLabDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// *********************************************************

	CClientDC dc(this);
	
	/**
	*	windowStatus == 0 (현재 화면이 메인이라면)
	*/
	if (windowStatus == 0) {
		
		// 위치 나중에 조절하기
		CString mainLogo;
		CString mainMessage;
		CString mainMessage2;

		// 로고 출력
		mainLogo.Format(L"2D AimLab");
		mainMessage.Format(L"게임을 시작하시려면 (미정) 버튼을 누르거나");
		mainMessage2.Format(L"상단의(미정2) 메뉴를 통해 시작해주세요.");
		// 게임 설명도 추가

		CRect rect(	50 - 20,
					50 - 20,
					50 + 100,
					50 + 100	);
		CRect rect2(	100 - 20,
						100 - 20,
						100 + 500,
						100 + 200	);
		CRect rect3(	200 - 20,
						200 - 20,
						200 + 500,
						200 + 200	);

		dc.DrawText(mainLogo, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dc.DrawText(mainMessage, rect2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dc.DrawText(mainMessage2, rect3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}


	// *********************************************************
}


// CMy2DAimLabView 인쇄

BOOL CMy2DAimLabView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMy2DAimLabView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMy2DAimLabView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMy2DAimLabView 진단

#ifdef _DEBUG
void CMy2DAimLabView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2DAimLabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2DAimLabDoc* CMy2DAimLabView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2DAimLabDoc)));
	return (CMy2DAimLabDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy2DAimLabView 메시지 처리기
