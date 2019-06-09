
// LibraryAbsenceDetectionView.cpp: CLibraryAbsenceDetectionView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "LibraryAbsenceDetection.h"
#endif

#include "LibraryAbsenceDetectionDoc.h"
#include "LibraryAbsenceDetectionView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLibraryAbsenceDetectionView

IMPLEMENT_DYNCREATE(CLibraryAbsenceDetectionView, CView)

BEGIN_MESSAGE_MAP(CLibraryAbsenceDetectionView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_LOAD_JPG, &CLibraryAbsenceDetectionView::OnLoadJpg)
	ON_COMMAND(ID_LOAD_AVI, &CLibraryAbsenceDetectionView::OnLoadAvi)
	ON_COMMAND(ID_LOAD_LAD, &CLibraryAbsenceDetectionView::OnLoadLad)
END_MESSAGE_MAP()

// CLibraryAbsenceDetectionView 생성/소멸

CLibraryAbsenceDetectionView::CLibraryAbsenceDetectionView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	rgbBuffer = nullptr;

}

CLibraryAbsenceDetectionView::~CLibraryAbsenceDetectionView()
{
	if (rgbBuffer != nullptr) {
		for (int i = 0; i < imgHeight; i++)
			delete[] rgbBuffer[i];
		delete[] rgbBuffer;
	}
}

BOOL CLibraryAbsenceDetectionView::PreCreateWindow(CREATESTRUCT & cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CLibraryAbsenceDetectionView 그리기

void CLibraryAbsenceDetectionView::OnDraw(CDC * pDC)
{
	CLibraryAbsenceDetectionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (rgbBuffer != NULL) {
		for (int i = 0; i < imgHeight; i++) {
			for (int j = 0; j < imgWidth; j++) {
				POINT p;
				p.x = j;
				p.y = i;
				pDC->SetPixel(p, RGB(rgbBuffer[i][j].rgbRed, rgbBuffer[i][j].rgbGreen, rgbBuffer[i][j].rgbBlue));
				//픽셀 위치 p에 RGB값을 출력

			}
		}
	}


	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CLibraryAbsenceDetectionView 인쇄

BOOL CLibraryAbsenceDetectionView::OnPreparePrinting(CPrintInfo * pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CLibraryAbsenceDetectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CLibraryAbsenceDetectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CLibraryAbsenceDetectionView 진단

#ifdef _DEBUG
void CLibraryAbsenceDetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CLibraryAbsenceDetectionView::Dump(CDumpContext & dc) const
{
	CView::Dump(dc);
}

CLibraryAbsenceDetectionDoc* CLibraryAbsenceDetectionView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLibraryAbsenceDetectionDoc)));
	return (CLibraryAbsenceDetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CLibraryAbsenceDetectionView 메시지 처리기

///JPG 메뉴 호출 함수 전에 아래의 함수 추가
BYTE* LoadJpegFromOpenFile(FILE * fp, BITMAPINFOHEADER * pbh, UINT * pWidth, UINT * pHeight) {
	//파일로부터 JPG 정보 획득
	if (pWidth == NULL || pHeight == NULL) return NULL;
	BYTE * tmp = JpegFile::OpenFileToRGB(fp, pWidth, pHeight);
	if (!tmp) return NULL;
	JpegFile::BGRFromRGB(tmp, *pWidth, *pHeight);
	UINT dw;
	BYTE * pbuf = JpegFile::MakeDwordAlignedBuf(tmp, *pWidth, *pHeight, &dw);
	delete[] tmp;
	if (!pbuf) return NULL;
	JpegFile::VertFlipBuf(pbuf, dw, *pHeight);
	pbh->biSize = sizeof(BITMAPINFOHEADER);
	pbh->biWidth = *pWidth;
	pbh->biHeight = *pHeight;
	pbh->biPlanes = 1;
	pbh->biBitCount = 24;
	pbh->biCompression = BI_RGB;
	pbh->biSizeImage = 0;
	pbh->biXPelsPerMeter = 0;
	pbh->biYPelsPerMeter = 0;
	pbh->biClrUsed = 0;
	pbh->biClrImportant = 0;
	return pbuf;
}


void CLibraryAbsenceDetectionView::OnLoadJpg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, ".jpg", NULL, NULL, "Jpeg File (*.jpg)|*.jpg||");
	if (IDOK != dlg.DoModal()) return;
	CString filename = dlg.GetPathName();
	//BMP 로드때와 마찬가지로, rgbBuffer가 NULL이 아닌 경우 메모리 해제 코드 추가하기
	if (rgbBuffer != NULL) { //이미 할당된 경우, 메모리 해제
		for (int i = 0; i < imgHeight; i++)
			delete[] rgbBuffer[i];
		delete[] rgbBuffer;
	}

	FILE* fp = fopen(filename, "rb");
	BITMAPINFOHEADER pbh;
	UINT w, h;
	BYTE* pbuf = LoadJpegFromOpenFile(fp, &pbh, &w, &h);
	imgWidth = (int)w;
	imgHeight = (int)h;
	//BMP로드때와 마찬가지로, rgbBuffer를 이미지 사이즈 만큼 메모리 할당 코드 추가하기
	rgbBuffer = new RGBQUAD * [imgHeight];
	for (int i = 0; i < imgHeight; i++)
		rgbBuffer[i] = new RGBQUAD[imgWidth];

	int dw = WIDTHBYTES(imgWidth * 24);
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			rgbBuffer[imgHeight - i - 1][j].rgbBlue = pbuf[i * dw + j * 3 + 0];
			rgbBuffer[imgHeight - i - 1][j].rgbGreen = pbuf[i * dw + j * 3 + 1];
			rgbBuffer[imgHeight - i - 1][j].rgbRed = pbuf[i * dw + j * 3 + 2];
		}
	}
	delete[] pbuf;
	fclose(fp);
	Invalidate(TRUE);
}


void CLibraryAbsenceDetectionView::OnLoadAvi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//AVI 파일 선택하는 다이얼로그
	CFileDialog dlg(TRUE, ".avi", NULL, NULL, "AVI File (*.avi)|*.avi||");
	if (IDOK != dlg.DoModal())
		return;

	//파일 경로 입력
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	const cv::String filename(strAtl);

	//AVI 파일 로드
	cv::VideoCapture Capture;
	Capture.open(filename);

	//파일에 문제가 있는 경우, 에러 메세지로 표시 후 종료
	if (!Capture.isOpened())
		AfxMessageBox("Error Video");

	Mat Kx(3, 3, CV_32S);
	Kx.at<int>(0, 0) = -1;
	Kx.at<int>(1, 0) = -2;
	Kx.at<int>(2, 0) = -1;
	Kx.at<int>(0, 1) = 0;
	Kx.at<int>(1, 1) = 0;
	Kx.at<int>(2, 1) = 0;
	Kx.at<int>(0, 2) = 1;
	Kx.at<int>(1, 2) = 2;
	Kx.at<int>(2, 2) = 1;

	Mat Ky(3, 3, CV_32S);
	Ky.at<int>(0, 0) = 1;
	Ky.at<int>(0, 1) = 2;
	Ky.at<int>(0, 2) = 1;
	Ky.at<int>(1, 0) = 0;
	Ky.at<int>(1, 1) = 0;
	Ky.at<int>(1, 2) = 0;
	Ky.at<int>(2, 0) = -1;
	Ky.at<int>(2, 1) = -2;
	Ky.at<int>(2, 2) = -1;




	bool is_firstframe = true;

	for (;;) {
		Mat frame;   //한 프레임씩 변수 삽입
		Capture >> frame;

		if (frame.data == nullptr) //남은 프레임이 없는 경우 반복문 종료
			break;

		Mat hsvframe;
		cvtColor(frame, hsvframe, CV_BGR2HSV);
		int f_width = hsvframe.cols;      //x
		int f_height = hsvframe.rows;   //y

		Mat BinarySobel;
		BinarySobel = Mat::zeros(f_height, f_width, CV_8UC1);


		int sum_x, sum_y, M;
		for (int i = 1; i < f_height - 1; i++) {
			for (int j = 1; j < f_width - 1; j++) {
				Mat Sx, Sy, temp;
				Rect rect(j - 1, i - 1, 3, 3);
				hsvframe(rect).convertTo(temp, CV_32S);
				//cv::multiply(temp, Kx, Sx);
				Sx = temp * Kx;
				sum_x = cv::sum(Sx)[0];
				//cv::multiply(temp, Ky, Sy);
				Sy = temp * Ky;
				sum_y = cv::sum(Sy)[0];
				M = pow(sum_x * sum_x + sum_y * sum_y, 0.5);
				BinarySobel.at<uchar>(i, j) = M > 128 ? 255 : 0;

			}
		}

		imshow("video", BinarySobel); //"video"라는 창을 생성하여, 각 프레임을 출력

		if (waitKey(30) >= 0)   //30ms 대기
			break;
	}
	//AfxMessageBox("Completed");   //로드 완료 메세지 출력
}

void CLibraryAbsenceDetectionView::OnLoadLad()
{

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//AVI 파일 선택하는 다이얼로그
	CFileDialog dlg(TRUE, ".avi", NULL, NULL, "AVI File (*.avi)|*.avi||");
	if (IDOK != dlg.DoModal())
		return;

	//파일 경로 입력
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	const cv::String filename(strAtl);

	//AVI 파일 로드
	VideoCapture Capture;
	Capture.open(filename);
	
	CascadeClassifier face_cascade;
	if (!face_cascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml"))
		AfxMessageBox("Error Classifier");

	//파일에 문제가 있는 경우, 에러 메세지로 표시 후 종료
	if (!Capture.isOpened())
		AfxMessageBox("Error Video");


	vector<Rect>* faces = new vector<Rect>; //never call delete whatever you do
	//In theory it's a memory leak, however calling delete on a vector of cv::Rect (emptied or not) causes problems.
	//Care must be taken not to continuously allocate cv::Rect vectors. 
	//Anything left over at the program end will be cleaned up by the OS.
	//The problem lies somewhere within OpenCV's code.

	Mat frame;
	for (;;) {
		Capture >> frame;

		if (frame.empty()) //남은 프레임이 없는 경우 반복문 종료
			break;

		(*faces).clear();

		face_cascade.detectMultiScale(frame, *faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));

		for (int i = 0; i < (*faces).size(); i++)
		{
			Point center((*faces)[i].x + (*faces)[i].width * 0.5, (*faces)[i].y + (*faces)[i].height * 0.5);
			ellipse(frame, center, Size((*faces)[i].width * 0.5, (*faces)[i].height * 0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		}

		imshow("video", frame); //"video"라는 창을 생성하여, 각 프레임을 출력

		if (waitKey(30) >= 0)   //30ms 대기
			break;
	}

	Capture.release();
	destroyAllWindows();
	AfxMessageBox("Completed");   //로드 완료 메세지 출력
}
