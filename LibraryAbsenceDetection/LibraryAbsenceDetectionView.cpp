
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
	ON_COMMAND(ID_LOAD_FACEDETECTION, &CLibraryAbsenceDetectionView::OnLoadFacedetection)
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
	
}

bool CLibraryAbsenceDetectionView::block_maching(Point input, Point& output, Mat next, Mat prev, int BLOCK_SIZE, int WINDOW_SIZE)
{
	int MAXROWS = prev.rows / BLOCK_SIZE;
	int MAXCOLS = prev.cols / BLOCK_SIZE;
	int trackingRow = input.y / BLOCK_SIZE;
	int trackingCol = input.x / BLOCK_SIZE;

	// Find best matching dx, dy (in relative measure)
	int dx = 0, dy = 0;

	Mat diff;
	Rect reference(trackingCol * BLOCK_SIZE, trackingRow * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
	cv::absdiff(prev(reference), next(reference), diff);
	double MAE, min = cv::sum(diff)[0];// / ((double)curr.channels() * BLOCK_SIZE * BLOCK_SIZE);

	// Find best matching block in (r, c as absolute values of x, y in image matrix)
	int nextRow, nextCol;
	// Loop searching 8 points each step
	for (int step = 1, w = WINDOW_SIZE / 2; step <= 3; step++, w = w >> 1) {
		int dxcenter = dx, dycenter = dy;

		for (int dycandidate = -w; dycandidate <= w; dycandidate += w) {
			for (int dxcandidate = -w; dxcandidate <= w; dxcandidate += w) {
				if (dxcandidate == 0 && dycandidate == 0) // no need to consider center point again
					continue;

				// Bound check block coordinates
				nextRow = trackingRow + dycenter + dycandidate;
				nextCol = trackingCol + dxcenter + dxcandidate;
				if (nextRow < 0 || nextRow >= MAXROWS || nextCol < 0 || nextCol >= MAXCOLS)
					continue;

				//// Get block coordinate in search window 
				Rect candidate(nextCol * BLOCK_SIZE, nextRow * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
				cv::absdiff(prev(candidate), next(reference), diff);
				MAE = cv::sum(diff)[0];// / ((double)prev.channels() * BLOCK_SIZE * BLOCK_SIZE);

				if (MAE < min) {
					dx = dxcenter + dxcandidate;
					dy = dycenter + dycandidate;
					min = MAE;
				}
			}
		}
	}

	// ��ġ������ ŭ 
	if (abs(dx)>5|| abs(dy)>5)
		return false;
	// �����ڸ� ����
	else if (nextRow == 0 || nextRow == MAXROWS - 1 || nextCol == 0 || nextCol == MAXCOLS - 1)
		return false;
	// �������� ��ȭ�̸� �����ڸ��� �ƴϾ��
	else if (dx != 0|| dy!= 0) {
		output += Point(dx * 16 + 8, dy * 16 + 8);
		return true;
	}
}



void CLibraryAbsenceDetectionView::OnLoadFacedetection()
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
	if (!face_cascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml"))
		AfxMessageBox("Error Classifier");

	//���Ͽ� ������ �ִ� ���, ���� �޼����� ǥ�� �� ����
	if (!Capture.isOpened())
		AfxMessageBox("Error Video");

	int seat_num = 3;
	string seat_name[3] = { "��ȣ","����", "����" };

	vector<Rect>* faces = new vector<Rect>;
	vector<Rect>* seats = new vector<Rect>;
	vector<bool>* absence = new vector<bool>;

	Rect rect(230, 150, 150, 170);   //seat1
	(*seats).push_back(rect);
	(*absence).push_back(true);
	Rect rect2(10, 310, 150, 170);   //seat2
	(*seats).push_back(rect2);
	(*absence).push_back(true);
	Rect rect3(240, 290, 150, 170);   //seat3
	(*seats).push_back(rect3);
	(*absence).push_back(true);

	//Time
	vector<clock_t>* start_time = new vector<clock_t>;
	vector<clock_t>* end_time = new vector<clock_t>;
	vector<bool>* checkingtime = new vector<bool>;
	//���� �ʱ�ȭ
	for (int i = 0; i < seat_num; i++) {
		(*start_time).push_back(0);
		(*end_time).push_back(0);
		(*checkingtime).push_back(false);
	}

	Mat frame, hsvframe, prame;
	vector<Mat>* range = new vector<Mat>;
	vector<Rect>* hello = new vector<Rect>[seat_num];

	Capture >> frame;
	cvtColor(frame, prame, CV_BGR2GRAY);
	for (;;) {
		Capture >> frame;

		if (frame.empty()) //���� �������� ���� ��� �ݺ��� ����
			break;

		/// Convert to grayscale
		cvtColor(frame, hsvframe, CV_BGR2GRAY);

		/// Apply Histogram Equalization
		equalizeHist(hsvframe, hsvframe);


		for (int j = 0; j < seat_num; j++)
			(*absence).at(j) = true;

		(*range).clear();
		for (int i = 0; i < seat_num; i++) {
			(*range).push_back(hsvframe((*seats).at(i)));
		}

		for (int r = 0; r < seat_num; r++) {
			rectangle(frame, (*seats).at(r), Scalar(0, 255, 0), 4, 5, 0); // �簢�� ���

			(*faces).clear();
			face_cascade.detectMultiScale((*range).at(r), *faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

			// ���ν� ���� : ������ �߽��� �����Ѵ� (����Ī)
			if ((*faces).size() == 0) {
				for (int i = 0; i < hello[r].size(); i++) {
					Point center((hello[r])[i].x + (hello[r])[i].width * 0.5, (hello[r])[i].y + (hello[r])[i].height * 0.5);
					Point fcenter((*seats)[r].x + center.x, (*seats)[r].y + center.y);
					//Point center : ������ ��ǥ, Point delta : ����� ��ǥ
					//Mat range[r] : ����Ī ��ü ����, Mat prame((*seats).at(i)) : ���� ������ ��ü����
					if (block_maching(center, fcenter, (*range)[r], prame((*seats).at(r)), 16, 8)) {
						ellipse(frame, fcenter, Size(5, 5), 0, 0, 360, Scalar(0, 255, 255), 2, 8, 0);
					}
				}
			}
			// ���ν� ���� : ���ο� �󱼷� �����Ѵ�
			else
				hello[r] = *faces;

			for (int i = 0; i < (*faces).size(); i++)
			{
				Point center((*faces)[i].x + (*faces)[i].width * 0.5, (*faces)[i].y + (*faces)[i].height * 0.5);
				Point fcenter((*seats)[r].x + center.x, (*seats)[r].y + center.y);

				// Occupied
				if ((*seats)[r].x <= fcenter.x &&
					fcenter.x <= (*seats)[r].x + (*seats)[r].width &&
					(*seats)[r].y <= fcenter.y &&
					fcenter.y <= (*seats)[r].y + (*seats)[r].height)
				{
					(*absence).at(r) = false;
					if ((*checkingtime).at(r) == true) {   //�ð� ����ִ� �� �϶�
						(*end_time).at(r) = clock();
						//���
						clock_t absenceTime = (*end_time).at(r) - (*start_time).at(r);
						if (absenceTime / 1000.0 >= 10) {   //20�� �̻��� ���� ���
							if (absenceTime / 1000.0 >= 15)	cout << "***";
							cout << seat_name[r] << "��� �ð�: " << absenceTime / 1000.0 << " ��" << endl;
						}
						(*checkingtime).at(r) = false;
						//break;
					}
					ellipse(frame, fcenter, Size((*faces)[i].width * 0.5, (*faces)[i].height * 0.5), 0, 0, 360, Scalar(0, 255, 0), 4, 8, 0);
				}
			}
		}

		for (int j = 0; j < seat_num; j++) {
			if ((*absence).at(j) == true) {   //��� ������

				if ((*checkingtime).at(j) == false) {   //�ð� ����� ������
					(*start_time).at(j) = clock();
					cout << "start_time:" << (*start_time).at(j) << endl;
					(*checkingtime).at(j) = true;
				}
				else if ((*checkingtime).at(j) == true) {   //�ð� ����ִ� �� �϶�
					(*end_time).at(j) = clock();
					clock_t absenceTime = (*end_time).at(j) - (*start_time).at(j);
					if (absenceTime / 1000.0 >= 10) { //25�� �̻��� ���� ���
						rectangle(frame, (*seats).at(j), Scalar(0, 0, 255), 4, 5, 0);
						hello[j].clear();
					}
					else
						rectangle(frame, (*seats).at(j), Scalar(0, 255, 255), 4, 5, 0);
				}
			}
		}

		imshow("video", frame); //"video"��� â�� �����Ͽ�, �� �������� ���

		if (waitKey(30) >= 0)   //30ms ���
			break;

		prame = hsvframe.clone();
	}

	Capture.release();
	destroyAllWindows();
	AfxMessageBox("Completed");   //�ε� �Ϸ� �޼��� ���

}
