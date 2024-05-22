
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
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMy2DAimLabView 생성/소멸

CMy2DAimLabView::CMy2DAimLabView() noexcept
{
	// *********************************************************
	// TODO: 여기에 생성 코드를 추가합니다.
	// *********************************************************

	// 코드 작성 중 확인용으로 변경함. 0을 기본값으로 둘 것
	windowStatus = 1;				// 현재 표시할 화면이 무엇인지 제어 (int)
									// 예시)  0:메인 1:게임화면 2:랭킹..

	circMatrix;						// 랜덤 원이 생성될 위치 배열 정보 (vector<CPoint>)
									// 실제 원의 정보를 담은 게 아님
	circles;						// 랜덤 원의 정보가 인덱스를 통해 저장되는 곳 (vector<CPoint>)

	isExist;						// 랜덤 원이 해당 circMatrix 인덱스에 존재하는지 여부 (vector<bool>)

	srand((unsigned)time(NULL));	// 랜덤 초기화

	circRad = 40;					// 원의 반지름
	score = 0;						// 현재 점수
	//scoreAdd = 0;					// 현재 원의 번호이자 추가 점수
	genAmount = 3;					// 존재할 원 갯수


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

	// *********************************************************
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// *********************************************************

	CClientDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	//	circMatrix 위치 정의, isExist 기본값 정의
	for (int i = 0; i < 25; i++) {

		isExist.push_back(false);
		
		// 임시로 위치 잘 나오는지 확인용, 확인 후 삭제
		//if (rand() % 2 == 0)		isExist.push_back(true);
		//else						isExist.push_back(false);
		
		circMatrix.push_back(CPoint(0, 0));
		
		if		(i / 5 == 0) { circMatrix[i].x = rc.right / 2 - 200; }
		else if (i / 5 == 1) { circMatrix[i].x = rc.right / 2 - 100; }
		else if (i / 5 == 2) { circMatrix[i].x = rc.right / 2 - 0; }
		else if (i / 5 == 3) { circMatrix[i].x = rc.right / 2 + 100; }
		else if (i / 5 == 4) { circMatrix[i].x = rc.right / 2 + 200; }

		if		(i % 5 == 0) { circMatrix[i].y = rc.bottom / 2 - 200; }
		else if (i % 5 == 1) { circMatrix[i].y = rc.bottom / 2 - 100; }
		else if (i % 5 == 2) { circMatrix[i].y = rc.bottom / 2 - 0; }
		else if (i % 5 == 3) { circMatrix[i].y = rc.bottom / 2 + 100; }
		else if (i % 5 == 4) { circMatrix[i].y = rc.bottom / 2 + 200; }
	}

	//for (int i = 0; i < 25; i++) {
	//	if (isExist[i]) {
	//		dc.Ellipse(	circMatrix[i].x - circRad,
	//					circMatrix[i].y - circRad,
	//					circMatrix[i].x + circRad,
	//					circMatrix[i].y + circRad);
	//	}
	//}
	
		
	/**
	*	windowStatus == 0 (현재 화면이 메인이라면)
	*/
	if (windowStatus == 0) {
		
		CString mainLogo;
		CString mainMessage1;
		CString mainMessage2;

		// 로고 출력
		mainLogo.Format(L"2D AimLab");
		mainMessage1.Format(L"게임을 시작하시려면 (단축키명) 버튼을 누르거나");
		mainMessage2.Format(L"상단의(메뉴명) 메뉴를 통해 시작해주세요.");
		// 게임 설명도 추가

		CRect rect(		rc.right / 2 - 500,	rc.bottom / 2 - rc.bottom * 0.3,
						rc.right / 2 + 500,	rc.bottom / 2 - rc.bottom * 0.1);
		CRect rect2(	rc.right / 2 - 500,	rc.bottom / 2 - rc.bottom * 0.0,
						rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 20);
		CRect rect3(	rc.right / 2 - 500,	rc.bottom / 2 + rc.bottom * 0.0 + 20,
						rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 40);

		dc.DrawText(mainLogo, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dc.DrawText(mainMessage1, rect2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		dc.DrawText(mainMessage2, rect3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}


	/**
	*	windowStatus == 1 (현재 화면이 게임화면이라면)
	*/
	else if (windowStatus == 1) {

		int numOrder = 0;		// 클릭해야 하는 숫자

		// 최초 원 생성
		for (int i = 0; i < genAmount; i++) {
			int randNum = rand() % 25;
			if (isExist[randNum]) {
				i--;
			}
			else if (!isExist[randNum]) {
				isExist[randNum] = true;
				CPoint temp = circMatrix[randNum];
				circles.push_back(temp);
			}
		}
		refreshCirc();




	}

	/**
	*	windowStatus == 2 (현재 화면이 랭킹화면이라면)
	*/
	else if (windowStatus == 2) {

	}
	
	dc.Rectangle(10, 100, 20, 20); // x1, y1, x2, y2

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


void CMy2DAimLabView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// *********************************************************
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// *********************************************************
	
	

	// *********************************************************
	CView::OnLButtonDown(nFlags, point);
}





void CMy2DAimLabView::refreshCirc()
{
	// *********************************************************
	// TODO: 여기에 구현 코드 추가.
	// *********************************************************
	CClientDC dc(this);

	for (int i = 0; i < circles.size(); i++) {
		CString index;
		index.Format(L"%d", i + 1);
		CRect circPos(	circles[i].x - circRad,
						circles[i].y - circRad,
						circles[i].x + circRad,
						circles[i].y + circRad	);

		dc.Ellipse(circPos);
		dc.DrawText(index, circPos, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	// *********************************************************
}
