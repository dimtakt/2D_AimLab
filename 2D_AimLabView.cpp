
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
	ON_WM_TIMER()
	ON_COMMAND(gotoMain, &CMy2DAimLabView::Ongotomain)
	ON_COMMAND(gotoGame, &CMy2DAimLabView::Ongotogame)
	ON_COMMAND(gotoRank, &CMy2DAimLabView::Ongotorank)
	ON_COMMAND(dataSave, &CMy2DAimLabView::Ondatasave)
	ON_COMMAND(dataLoad, &CMy2DAimLabView::Ondataload)
	ON_COMMAND(dataClear, &CMy2DAimLabView::Ondataclear)
END_MESSAGE_MAP()

// CMy2DAimLabView 생성/소멸

CMy2DAimLabView::CMy2DAimLabView() noexcept
{
	// *********************************************************
	// TODO: 여기에 생성 코드를 추가합니다.
	// *********************************************************

	isOpened = false;				// 실행하면 true로 바뀜. OnDraw 최초 호출시에만 작동되게 하기위함.

	// 코드 작성 중 확인용으로 변경함. 0을 기본값으로 둘 것
	windowStatus = 0;				// 현재 표시할 화면이 무엇인지 제어 (int)
									// 예시)  0:메인 1:게임화면 2:랭킹..

	circRad = 40;					// 원의 반지름
	score = 0;						// 현재 점수
	combo = 0;						// 콤보시스템, 점수증가량 (연속하여 맞출 시 1씩 증가, 틀릴 시 0으로 초기화)

	genAmount = 3;					// 존재할 원 갯수


	circMatrix;						// 랜덤 원이 생성될 위치 배열 정보 (vector<CPoint>)
									// 실제 원의 정보를 담은 게 아님, 단순 위치 참고용
	circles;						// 랜덤 원의 정보가 인덱스를 통해 저장되는 곳 (vector<CPoint>)

	isExist.resize(25);				// 랜덤 원이 해당 circMatrix 인덱스에 존재하는지 여부 (vector<bool>)
	
	playHistory_elapsedTime;		// 플레이 기록(경과 시간)이 저장될 변수 (vector<int>)
	playHistory_score;				// 플레이 기록(획득 점수)이 저장될 변수 (vector<int>)
	

	srand((unsigned)time(NULL));	// 랜덤 초기화



	elapsed_time = 0;				// 게임 중 지난 시간 [나중에 수정]	0.1초단위
	left_time = 600;				// 게임 중 남은 시간 [나중에 수정]	0.1초단위 : 60초
	left_time_Sub = 50;				// 오답 시 깎일 시간				0.1초단위 : 1.0초

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




	// OnDraw 최초 호출시에만 실행
	if (!isOpened) {
		isOpened = true;

		//	circMatrix 위치 정의, isExist 기본값 정의
		for (int i = 0; i < 25; i++) {

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


		// windowStatus == 0 (현재 화면이 메인이라면)
		if (windowStatus == 0) {
			window_Main();
		}
	
		// windowStatus == 1 (현재 화면이 게임화면이라면)
		else if (windowStatus == 1) {
			window_Game();
		}

		// windowStatus == 2 (현재 화면이 랭킹화면이라면)
		else if (windowStatus == 2) {
			window_Rank();
		}
	}



	// windowStatus == 0 (현재 화면이 메인이라면)
	if (windowStatus == 0) {

	}

	// windowStatus == 1 (현재 화면이 게임화면이라면)
	else if (windowStatus == 1) {
		refreshScore();
		refreshTime();
		refreshCirc();

		
		// 상단 타이머 역할 바
		CClientDC dc(this);
		CRect rc;
		GetClientRect(&rc);

		CBrush timerBar(RGB(200, 255, 255));
		dc.SelectObject(&timerBar);

		dc.Rectangle(	rc.right / 2.0 - (rc.right / 2.0) * (left_time / 600.0), 0,
						rc.right / 2.0 + (rc.right / 2.0) * (left_time / 600.0), 20);
		
		
		// 게임이 종료되었다면
		if (left_time <= 0) {
			KillTimer(1);
			
			CString msg;
			msg.Format(_T("게임이 종료되었습니다. 당신의 점수는 %d점입니다."), score);
			MessageBox(msg, _T("알림"), MB_ICONINFORMATION);

			// 내부 변수에 플레이기록 담기
			playHistory_elapsedTime.push_back(elapsed_time);
			playHistory_score.push_back(score);

			// 값들 초기화
			gameDataClear();

			// 랭킹 화면으로 넘어감
			isOpened = false;
			windowStatus = 2;
			Invalidate();
		}
	}

	// windowStatus == 2 (현재 화면이 랭킹화면이라면)
	else if (windowStatus == 2) {
		
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


void CMy2DAimLabView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// *********************************************************
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// *********************************************************

	CClientDC dc(this);

	CRect rc;
	GetClientRect(&rc);



	// windowStatus == 0 (현재 화면이 메인이라면)
	if (windowStatus == 0) {

		CRect rect1(rc.right / 2 - 500, rc.bottom / 2 + rc.bottom * 0.0 + 140,
					rc.right / 2 + 500, rc.bottom / 2 + rc.bottom * 0.0 + 180);
		bool isClickStart =	rc.right / 2 - 500 <= point.x &&
							point.x <= rc.right / 2 + 500 &&
							rc.bottom / 2 + rc.bottom * 0.0 + 140 <= point.y &&
							point.y <= rc.bottom / 2 + rc.bottom * 0.0 + 180;
		// 만약 게임시작 버튼을 눌렀다면
		if (isClickStart) {
			// 게임 시작 화면으로 넘어감
			isOpened = false;
			windowStatus = 1;
			Invalidate();
		}


		CRect rect2(rc.right / 2 - 500, rc.bottom / 2 + rc.bottom * 0.0 + 190,
					rc.right / 2 + 500, rc.bottom / 2 + rc.bottom * 0.0 + 230);
		bool isClickRank =	rc.right / 2 - 500 <= point.x &&
							point.x <= rc.right / 2 + 500 &&
							rc.bottom / 2 + rc.bottom * 0.0 + 190 <= point.y &&
							point.y <= rc.bottom / 2 + rc.bottom * 0.0 + 230;
		// 만약 플레이 기록 버튼을 눌렀다면
		if (isClickRank) {
			// 플레이 기록 화면으로 넘어감
			isOpened = false;
			windowStatus = 2;
			Invalidate();
		}
	}

	// windowStatus == 1 (현재 화면이 게임화면이라면)
	else if (windowStatus == 1) {

		int tryCount = 0;		// 값이 circles.size() 에 도달한 경우 = 원을 클릭하지 않음 
	
	
		for (int i = 0; i < circles.size(); i++) {
		
			// 원을 클릭했는지의 조건 확인
			double dist = sqrt(	pow(point.x - circles[i].x, 2) +
								pow(point.y - circles[i].y, 2)	);
			bool isCorrect = dist <= circRad;

		
			// 만약에 원을 클릭했다면
			if (isCorrect) {

				// 클릭한 부분에 대응되는 isExist 내 원 존재여부 정보 반영하기 (지우기)
				// 이거 안해서 아까 문제 생겼음
				for (int j = 0; j < isExist.size(); j++) {
					if (circles[i].x == circMatrix[j].x && circles[i].y == circMatrix[j].y) {
						isExist[j] = false;
						combo++;					// 연속하여 맞출 시 점수 증가량 증가
						score += combo;

						// 맞출 시에 시간 추가 기능
						if			(elapsed_time <= 100)	{ left_time += 3; }
						else if		(elapsed_time <= 300)	{ left_time += 2; }
						else if		(elapsed_time <= 500)	{ left_time += 1; }
					
						// 남은 시간은 최초 시간을 넘을 수 없음
						if			(left_time > 600)		{ left_time = 600; }
					}
				}
				// 클릭한 원 지움
				circles.erase(i + circles.begin());



			
				// 새로운 원 생성
				for (int i = 0; i < 1; i++) {
					int randNum = rand() % 25;
					// 중복 자리에 원이 있으면 재생성 시도
					if (isExist[randNum]) {
						i--;
					}
					// 원 생성
					else {
						isExist[randNum] = true;
						CPoint temp = circMatrix[randNum];
						circles.push_back(temp);

						tryCount = 0; // 원을 클릭했음을 확인했으므로 0으로 초기화
						break;
					}
				}
			}
			else {
				tryCount++; // 원을 클릭한게 아닌것으로 추정되므로 +1, 3 이상이면 아래 if문으로 넘어감
			}

			// 만약에 원을 클릭하지 않았다면
			if (tryCount >= circles.size()) {
				left_time -= left_time_Sub;		// 페널티로 남은 시간 감소
				combo = 0;		// 점수 증가량 초기화
				tryCount = 0;	// 원을 클릭한게 아닌것임을 확인했으므로 0으로 초기화
			}

		}

		Invalidate();

	}

	// windowStatus == 2 (현재 화면이 랭킹화면이라면)
	else if (windowStatus == 2) {
		
		CRect rect1(rc.right / 2 - 500, rc.bottom / 2 + rc.bottom * 0.0 + 140,
					rc.right / 2 + 500, rc.bottom / 2 + rc.bottom * 0.0 + 180);
		bool isClickStart =	rc.right / 2 - 500 <= point.x &&
							point.x <= rc.right / 2 + 500 &&
							rc.bottom / 2 + rc.bottom * 0.0 + 140 <= point.y &&
							point.y <= rc.bottom / 2 + rc.bottom * 0.0 + 180;
		// 만약 게임시작 버튼을 눌렀다면
		if (isClickStart) {
			// 게임 시작 화면으로 넘어감
			isOpened = false;
			windowStatus = 1;
			Invalidate();
		}

		CRect rect2(rc.right / 2 - 500, rc.bottom / 2 + rc.bottom * 0.0 + 190,
					rc.right / 2 + 500, rc.bottom / 2 + rc.bottom * 0.0 + 230);
		bool isClickBack =	rc.right / 2 - 500 <= point.x &&
							point.x <= rc.right / 2 + 500 &&
							rc.bottom / 2 + rc.bottom * 0.0 + 190 <= point.y &&
							point.y <= rc.bottom / 2 + rc.bottom * 0.0 + 230;
		// 만약 뒤로가기 버튼을 눌렀다면
		if (isClickBack) {
			// 메인 화면으로 넘어감
			isOpened = false;
			windowStatus = 0;
			Invalidate();
		}

	}

	

	// *********************************************************
	CView::OnLButtonDown(nFlags, point);
}




// 원을 그려오는 함수
void CMy2DAimLabView::refreshCirc()
{
	// *********************************************************
	// TODO: 여기에 구현 코드 추가.
	// 참고 :	주석처리한 부분은 원의 숫자를 표시하는 부분
	//			주석 해제 시 숫자가 보임
	// *********************************************************

	CClientDC dc(this);

	for (int i = 0; i < circles.size(); i++) {
		//CString index;
		//index.Format(L"%d", i + 1);
		CRect circPos(	circles[i].x - circRad,
						circles[i].y - circRad,
						circles[i].x + circRad,
						circles[i].y + circRad	);

		dc.Ellipse(circPos);
		//dc.DrawText(index, circPos, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	// *********************************************************
}

void CMy2DAimLabView::refreshScore()
{
	// *********************************************************
	// TODO: 여기에 구현 코드 추가.
	// *********************************************************

	CClientDC dc(this);


	CString scoreDisplay;
	CString comboDisplay;
	scoreDisplay.Format(L"score : %d", score);
	comboDisplay.Format(L"%d combo!!", combo);

	CRect rect1(50, 50, 250, 70);
	CRect rect2(50, 70, 250, 90);

	dc.DrawText(scoreDisplay, rect1, DT_SINGLELINE);
	if (combo >= 5) {
		dc.DrawText(comboDisplay, rect2, DT_SINGLELINE);
	}

	// *********************************************************
}


void CMy2DAimLabView::refreshTime()
{
	// *********************************************************
	// TODO: 여기에 구현 코드 추가.
	// *********************************************************

	CClientDC dc(this);
	CRect rc;
	GetClientRect(&rc);


	CString leftTimeDisplay;
	CString elapsedTimeDisplay;
	leftTimeDisplay.Format(L"Left Time : %d", left_time);
	elapsedTimeDisplay.Format(L"Elapsed Time : %d", elapsed_time);

	CRect rect1(rc.right - 250, 50, rc.right - 50, 70);
	CRect rect2(rc.right - 250, 70, rc.right - 50, 90);

	dc.DrawText(leftTimeDisplay, rect1, DT_SINGLELINE);
	dc.DrawText(elapsedTimeDisplay, rect2, DT_SINGLELINE);

	// *********************************************************
}




void CMy2DAimLabView::window_Main()
{
	// *********************************************************
	// TODO: 여기에 구현 코드 추가.
	// *********************************************************

	CClientDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	CString mainLogo;
	CString mainMessage1;
	CString mainMessage2;
	CString mainHowMessage1;
	CString mainHowMessage2;
	CString mainStartBtn;
	CString mainRankBtn;

	// 로고 출력
	mainLogo.Format(L"2D AimLab");
	mainMessage1.Format(L"게임을 시작하시려면 아래의 버튼을 누르거나");
	mainMessage2.Format(L"상단 메뉴의 창 변경 - 게임 시작을 통해 시작해주세요.");
	mainHowMessage1.Format(L"게임 방법은 AimLab과 같이, 화면에 보이는 원을 연속해서 클릭하면 됩니다.");
	mainHowMessage2.Format(L"원을 누르지 않으면 콤보끊김, 제한시간 감소 등의 페널티가 있습니다.");
	mainStartBtn.Format(L"게임 시작");
	mainRankBtn.Format(L"플레이 기록");
	// 게임 설명도 추가

	CRect rect(		rc.right / 2 - 500,	rc.bottom / 2 - rc.bottom * 0.3,
					rc.right / 2 + 500,	rc.bottom / 2 - rc.bottom * 0.1);
	CRect rect2(	rc.right / 2 - 500,	rc.bottom / 2 - rc.bottom * 0.0,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 20);
	CRect rect3(	rc.right / 2 - 500,	rc.bottom / 2 + rc.bottom * 0.0 + 20,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 40);
	CRect rectHow1(	rc.right / 2 - 500,	rc.bottom / 2 + rc.bottom * 0.0 + 60,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 80);
	CRect rectHow2(	rc.right / 2 - 500,	rc.bottom / 2 + rc.bottom * 0.0 + 80,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 100);


	CRect rect4(	rc.right / 2 - 500,	rc.bottom / 2 + rc.bottom * 0.0 + 140,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 180);
	CRect rect5(	rc.right / 2 - 500,	rc.bottom / 2 + rc.bottom * 0.0 + 190,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 230);

	dc.DrawText(mainLogo, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc.DrawText(mainMessage1, rect2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc.DrawText(mainMessage2, rect3, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc.DrawText(mainHowMessage1, rectHow1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc.DrawText(mainHowMessage2, rectHow2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc.Rectangle(rect4);
	dc.DrawText(mainStartBtn, rect4, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc.Rectangle(rect5);
	dc.DrawText(mainRankBtn, rect5, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// *********************************************************
}


void CMy2DAimLabView::window_Game()
{
	// *********************************************************
	// TODO: 여기에 구현 코드 추가.
	// *********************************************************

	MessageBox(_T("게임을 시작합니다."), _T("알림"), MB_ICONINFORMATION);

	int numOrder = 0;		// 클릭해야 하는 숫자

	// 최초 원 생성
	for (int i = 0; i < genAmount; i++) {
		int randNum = rand() % 25;
		// 중복 자리에 원이 있으면 재생성 시도
		if (isExist[randNum]) {
			i--;
		}
		// 원 생성
		else if (!isExist[randNum]) {
			isExist[randNum] = true;
			CPoint temp = circMatrix[randNum];
			circles.push_back(temp);
		}
	}

	Invalidate();
	SetTimer(1, 100, NULL);  // ms단위. 즉 1000 당 1초

	

	// *********************************************************
}


void CMy2DAimLabView::window_Rank()
{
	// *********************************************************
	// TODO: 여기에 구현 코드 추가.
	// *********************************************************

	CClientDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	// 파일 불러오기
	std::fstream file("ranking.txt", std::ios::in);



	CRect header(	rc.right / 2 - 500,	rc.bottom / 2 - rc.bottom * 0.0 - 240,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 - 220);
	dc.DrawText(_T("===== 최근 플레이 기록 ====="), header, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


		
	for (int i = 0; i < playHistory_elapsedTime.size(); i++) {
			
		CRect rect(	rc.right / 2 - 500, rc.bottom / 2 - rc.bottom * 0.0 - 200 + 20 * i,
					rc.right / 2 + 500, rc.bottom / 2 + rc.bottom * 0.0 - 180 + 20 * i);

		CString text;
		text.Format(L"플레이 시간 : %d, 점수 : %d", playHistory_elapsedTime[i], playHistory_score[i]);

		dc.DrawText(text, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	}



	CRect info(	rc.right / 2 - 500,	rc.bottom / 2 - rc.bottom * 0.0 + 110,
				rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 130);
	dc.DrawText(_T("플레이 기록은 메뉴의 기록 데이터 - 저장하기 불러오기를 통해 외부 파일로의 저장 및 읽기가 가능합니다."), info, DT_SINGLELINE | DT_CENTER | DT_VCENTER);



	CString mainStartBtn;
	CString mainBackBtn;

	mainStartBtn.Format(L"게임 시작");
	mainBackBtn.Format(L"돌아가기");

	CRect rect1(	rc.right / 2 - 500,	rc.bottom / 2 + rc.bottom * 0.0 + 140,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 180);
	CRect rect2(	rc.right / 2 - 500,	rc.bottom / 2 + rc.bottom * 0.0 + 190,
					rc.right / 2 + 500,	rc.bottom / 2 + rc.bottom * 0.0 + 230);

	dc.Rectangle(rect1);
	dc.Rectangle(rect2);

	dc.DrawText(mainStartBtn, rect1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	dc.DrawText(mainBackBtn, rect2, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// *********************************************************
}


void CMy2DAimLabView::OnTimer(UINT_PTR nIDEvent)
{
	// *********************************************************
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// *********************************************************

	
	left_time -= 1;		// 0.1초마다 남은 시간 1씩 감소
	elapsed_time += 1;	// 0.1초마다 경과 시간 1씩 증가

	Invalidate();

	// *********************************************************
	CView::OnTimer(nIDEvent);
}


// 게임 도중 나올 시, 변수들을 초기화해주는 함수
void CMy2DAimLabView::gameDataClear()
{
	// TODO: 여기에 구현 코드 추가.
	score = 0;
	combo = 0;
	std::fill(isExist.begin(), isExist.end(), false);
	left_time = 600;
	elapsed_time = 0;
	circles.clear();
}







// *********************************************************
// *********************************************************
// 이하는 메뉴용
// *********************************************************
// *********************************************************

void CMy2DAimLabView::Ongotomain()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	gameDataClear();
	KillTimer(1);

	isOpened = false;
	windowStatus = 0;
	Invalidate();
}


void CMy2DAimLabView::Ongotogame()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	gameDataClear();
	KillTimer(1);

	isOpened = false;
	windowStatus = 1;
	Invalidate();
}


void CMy2DAimLabView::Ongotorank()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	gameDataClear();
	KillTimer(1);

	isOpened = false;
	windowStatus = 2;
	Invalidate();
}



// 플레이 기록을 외부 데이터로 저장
void CMy2DAimLabView::Ondatasave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	std::fstream file("ranking.txt", std::ios::out);

	if (file.is_open()) {
		
		for (int i = 0; i < playHistory_elapsedTime.size(); i++) {
			file << playHistory_score[i] << " " << playHistory_elapsedTime[i] << std::endl;
		}
		file.close(); // 파일 닫기
		MessageBox(_T("파일 저장에 성공했습니다."), _T("알림"), MB_ICONINFORMATION);
	}
	else {
		MessageBox(_T("파일 저장에 실패했습니다."), _T("오류"), MB_ICONERROR);
	}
}

// 플레이 기록을 외부 데이터로부터 불러오기
void CMy2DAimLabView::Ondataload()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	std::fstream file("ranking.txt", std::ios::in);

	if (file.is_open()) {
		int loadScore, loadElapsedTime;
		playHistory_score.clear();
		playHistory_elapsedTime.clear();

		while (file >> loadScore >> loadElapsedTime) {
			playHistory_score.push_back(loadScore);
			playHistory_elapsedTime.push_back(loadElapsedTime);
		}
		file.close();
		MessageBox(_T("파일 불러오기에 성공했습니다."), _T("알림"), MB_ICONINFORMATION);
	}
	else {
		MessageBox(_T("파일 불러오기에 실패했습니다."), _T("오류"), MB_ICONERROR);
	}

	// 만약 현재 창이 플레이기록 창이라면, 불러온 데이터를 볼 수 있게 새로고침함
	if (windowStatus == 2) {
		Ongotorank();
	}
}


void CMy2DAimLabView::Ondataclear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	playHistory_score.clear();
	playHistory_elapsedTime.clear();

	// 만약 현재 창이 플레이기록 창이라면, 불러온 데이터를 볼 수 있게 새로고침함
	if (windowStatus == 2) {
		Ongotorank();
	}

	MessageBox(_T("로컬 플레이기록이 초기화되었습니다."), _T("알림"), MB_ICONINFORMATION);
}
