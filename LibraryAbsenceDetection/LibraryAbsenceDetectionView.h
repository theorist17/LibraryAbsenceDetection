
// LibraryAbsenceDetectionView.h: CLibraryAbsenceDetectionView 클래스의 인터페이스
//

#pragma once
#include <iostream>
#include "Jpegfile.h"
#include "opencv2/opencv.hpp"

// headers for face detection
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

class CLibraryAbsenceDetectionView : public CView
{
protected: // serialization에서만 만들어집니다.
	CLibraryAbsenceDetectionView() noexcept;
	DECLARE_DYNCREATE(CLibraryAbsenceDetectionView)

	// 특성입니다.
public:
	CLibraryAbsenceDetectionDoc* GetDocument() const;

	// 작업입니다.
public:
	RGBQUAD** rgbBuffer; //이미지를 저장할 변수
	int imgHeight; //이미지 높이 정보
	int imgWidth; //이미지 너비 정보
	BITMAPINFOHEADER bmpInfo; //BMP 정보 헤더
	BITMAPFILEHEADER bmpHeader; //BMP 파일 헤더
	int viewType;

	//Edge Detector

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
	virtual ~CLibraryAbsenceDetectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoadJpg();
	afx_msg void OnLoadAvi();
	afx_msg void OnLoadLad();
};

#ifndef _DEBUG  // LibraryAbsenceDetectionView.cpp의 디버그 버전
inline CLibraryAbsenceDetectionDoc* CLibraryAbsenceDetectionView::GetDocument() const
{
	return reinterpret_cast<CLibraryAbsenceDetectionDoc*>(m_pDocument);
}
#endif
