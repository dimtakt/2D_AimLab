
// 2D_AimLabView.h: CMy2DAimLabView 클래스의 인터페이스
//

#pragma once

 #include <iostream>
 #include <vector>
 #include <algorithm>
 #include <random>
 #include <cstdlib>
 #include <ctime>

 #include <fstream>		// 파일 입출력




class CMy2DAimLabView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMy2DAimLabView() noexcept;
	DECLARE_DYNCREATE(CMy2DAimLabView)

// 특성입니다.
public:
	CMy2DAimLabDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMy2DAimLabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	// *********************************************************

	bool isOpened;
	
	int windowStatus;
	int circRad;
	int score;
	int combo;
	

	int genAmount;

	std::vector<CPoint> circMatrix;
	std::vector<CPoint> circles;
	std::vector<bool> isExist;
	//std::vector<int> circ;


	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	void refreshCirc();

	void window_Main();
	void window_Game();
	void window_Rank();	

	int elapsed_time;
	int left_time;
	int left_time_Sub;

	// *********************************************************
	void refreshScore();
};

#ifndef _DEBUG  // 2D_AimLabView.cpp의 디버그 버전
inline CMy2DAimLabDoc* CMy2DAimLabView::GetDocument() const
   { return reinterpret_cast<CMy2DAimLabDoc*>(m_pDocument); }
#endif

