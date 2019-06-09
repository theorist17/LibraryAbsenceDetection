
// LibraryAbsenceDetectionView.h: CLibraryAbsenceDetectionView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CLibraryAbsenceDetectionView() noexcept;
	DECLARE_DYNCREATE(CLibraryAbsenceDetectionView)

	// Ư���Դϴ�.
public:
	CLibraryAbsenceDetectionDoc* GetDocument() const;

	// �۾��Դϴ�.
public:
	RGBQUAD** rgbBuffer; //�̹����� ������ ����
	int imgHeight; //�̹��� ���� ����
	int imgWidth; //�̹��� �ʺ� ����
	BITMAPINFOHEADER bmpInfo; //BMP ���� ���
	BITMAPFILEHEADER bmpHeader; //BMP ���� ���
	int viewType;

	//Edge Detector

 // �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// �����Դϴ�.
public:
	virtual ~CLibraryAbsenceDetectionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoadJpg();
	afx_msg void OnLoadAvi();
	afx_msg void OnLoadLad();
};

#ifndef _DEBUG  // LibraryAbsenceDetectionView.cpp�� ����� ����
inline CLibraryAbsenceDetectionDoc* CLibraryAbsenceDetectionView::GetDocument() const
{
	return reinterpret_cast<CLibraryAbsenceDetectionDoc*>(m_pDocument);
}
#endif
