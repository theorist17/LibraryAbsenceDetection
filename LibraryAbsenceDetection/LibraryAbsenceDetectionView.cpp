
// LibraryAbsenceDetectionView.cpp: CLibraryAbsenceDetectionView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_LOAD_JPG, &CLibraryAbsenceDetectionView::OnLoadJpg)
	ON_COMMAND(ID_LOAD_AVI, &CLibraryAbsenceDetectionView::OnLoadAvi)
	ON_COMMAND(ID_LOAD_LAD, &CLibraryAbsenceDetectionView::OnLoadLad)
END_MESSAGE_MAP()

// CLibraryAbsenceDetectionView ����/�Ҹ�

CLibraryAbsenceDetectionView::CLibraryAbsenceDetectionView() noexcept
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CLibraryAbsenceDetectionView �׸���

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
				//�ȼ� ��ġ p�� RGB���� ���

			}
		}
	}


	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CLibraryAbsenceDetectionView �μ�

BOOL CLibraryAbsenceDetectionView::OnPreparePrinting(CPrintInfo * pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CLibraryAbsenceDetectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CLibraryAbsenceDetectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CLibraryAbsenceDetectionView ����

#ifdef _DEBUG
void CLibraryAbsenceDetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CLibraryAbsenceDetectionView::Dump(CDumpContext & dc) const
{
	CView::Dump(dc);
}

CLibraryAbsenceDetectionDoc* CLibraryAbsenceDetectionView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLibraryAbsenceDetectionDoc)));
	return (CLibraryAbsenceDetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CLibraryAbsenceDetectionView �޽��� ó����

///JPG �޴� ȣ�� �Լ� ���� �Ʒ��� �Լ� �߰�
BYTE* LoadJpegFromOpenFile(FILE * fp, BITMAPINFOHEADER * pbh, UINT * pWidth, UINT * pHeight) {
	//���Ϸκ��� JPG ���� ȹ��
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(TRUE, ".jpg", NULL, NULL, "Jpeg File (*.jpg)|*.jpg||");
	if (IDOK != dlg.DoModal()) return;
	CString filename = dlg.GetPathName();
	//BMP �ε嶧�� ����������, rgbBuffer�� NULL�� �ƴ� ��� �޸� ���� �ڵ� �߰��ϱ�
	if (rgbBuffer != NULL) { //�̹� �Ҵ�� ���, �޸� ����
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
	//BMP�ε嶧�� ����������, rgbBuffer�� �̹��� ������ ��ŭ �޸� �Ҵ� �ڵ� �߰��ϱ�
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//AVI ���� �����ϴ� ���̾�α�
	CFileDialog dlg(TRUE, ".avi", NULL, NULL, "AVI File (*.avi)|*.avi||");
	if (IDOK != dlg.DoModal())
		return;

	//���� ��� �Է�
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	const cv::String filename(strAtl);

	//AVI ���� �ε�
	cv::VideoCapture Capture;
	Capture.open(filename);

	//���Ͽ� ������ �ִ� ���, ���� �޼����� ǥ�� �� ����
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
		Mat frame;   //�� �����Ӿ� ���� ����
		Capture >> frame;

		if (frame.data == nullptr) //���� �������� ���� ��� �ݺ��� ����
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

		imshow("video", BinarySobel); //"video"��� â�� �����Ͽ�, �� �������� ���

		if (waitKey(30) >= 0)   //30ms ���
			break;
	}
	//AfxMessageBox("Completed");   //�ε� �Ϸ� �޼��� ���
}

void CLibraryAbsenceDetectionView::OnLoadLad()
{

	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//AVI ���� �����ϴ� ���̾�α�
	CFileDialog dlg(TRUE, ".avi", NULL, NULL, "AVI File (*.avi)|*.avi||");
	if (IDOK != dlg.DoModal())
		return;

	//���� ��� �Է�
	CString cfilename = dlg.GetPathName();
	CT2CA strAtl(cfilename);
	const cv::String filename(strAtl);

	//AVI ���� �ε�
	VideoCapture Capture;
	Capture.open(filename);
	
	CascadeClassifier face_cascade;
	if (!face_cascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml"))
		AfxMessageBox("Error Classifier");

	//���Ͽ� ������ �ִ� ���, ���� �޼����� ǥ�� �� ����
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

		if (frame.empty()) //���� �������� ���� ��� �ݺ��� ����
			break;

		(*faces).clear();

		face_cascade.detectMultiScale(frame, *faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 100));

		for (int i = 0; i < (*faces).size(); i++)
		{
			Point center((*faces)[i].x + (*faces)[i].width * 0.5, (*faces)[i].y + (*faces)[i].height * 0.5);
			ellipse(frame, center, Size((*faces)[i].width * 0.5, (*faces)[i].height * 0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		}

		imshow("video", frame); //"video"��� â�� �����Ͽ�, �� �������� ���

		if (waitKey(30) >= 0)   //30ms ���
			break;
	}

	Capture.release();
	destroyAllWindows();
	AfxMessageBox("Completed");   //�ε� �Ϸ� �޼��� ���
}
