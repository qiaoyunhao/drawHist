#pragma warning(disable: 4996)
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/*  ȫ�ֱ�������������ʼ��    */
Mat srcImage; //�����ͼƬ����
Mat dstImage; //�����ͼƬ����
MatND dstHist; //ֱ��ͼ���󣬶�Ӧ�ϰ汾�е�cvCreateHist����
int g_hdims = 50;     // ����HIST�ĳ�ʼ������Խ��Խ��ȷ

					  /* �ص��������� */
void on_HIST(int t, void *);


/*   ������   */
int main(int argc, char** argv)
{

	srcImage = imread("E:\\dataset\\football.jpg", 0);//"0"��ʾ����Ҷ�ͼ��
	namedWindow("ԭͼ", 1);//��Ӧ�ϰ汾�е�cvNamedWindow( )
	imshow("ԭͼ", srcImage);//��Ӧ�ϰ汾�е� cvShowImage����

	createTrackbar("hdims", "ԭͼ", &g_hdims, 256, on_HIST);//��Ӧ�ɰ汾�е�cvCreateTrackbar( );
	on_HIST(0, 0);//���ù������ص�����
	cvWaitKey(0);
	return 0;
}


/*      �������ص�����       */
void on_HIST(int t, void *)
{
	dstImage = Mat::zeros(512, 800, CV_8UC3);//ÿ�ζ�Ҫ��ʼ��
	float hranges[] = { 0,255 }; //�Ҷȷ�Χ
	const float *ranges[] = { hranges };//�Ҷȷ�Χ��ָ��

	if (g_hdims == 0)
	{
		printf("ֱ��ͼ��������Ϊ�㣡\n");
	}
	else
	{
		/*
		srcImage:����ľ���
		1:����ĸ���Ϊ1
		0����Ϊ�Ҷ�ͼ���һ��ͨ��������ѡ0��ͨ��
		Mat��������ʾ��ʹ����Ĥ
		dstHist:�����Ŀ��ֱ��ͼ
		1����Ҫ�����ֱ��ͼ��ά��Ϊ1
		g_hdims:����HIST�ĸ���
		ranges:��ʾÿһά�ȵ���ֵ��Χ
		*/
		//int channels=0;
		calcHist(&srcImage, 1, 0, Mat(), dstHist, 1, &g_hdims, ranges); // ����ֱ��ͼ��Ӧ�ϰ汾��cvCalcHist

																		/* ��ȡ�����Сֵ */
		double max = 0;
		minMaxLoc(dstHist, NULL, &max, 0, 0);// Ѱ�����ֵ����λ�ã���Ӧ�ɰ汾��cvGetMinMaxHistValue();

											 /*  ���ֱ��ͼ    */

		double bin_w = (double)dstImage.cols / g_hdims;  // hdims: ���ĸ������� bin_w Ϊ���Ŀ��
		double bin_u = (double)dstImage.rows / max;  //// max: ����������ظ������� bin_u Ϊ�������صĸ߶�

													 // ��ֱ��ͼ
		for (int i = 0; i<g_hdims; i++)
		{
			Point p0 = Point(i*bin_w, dstImage.rows);//��Ӧ�ɰ汾�е�cvPoint����

			int val = dstHist.at<float>(i);//ע��һ��Ҫ��float���ͣ���Ӧ�ɰ汾�е� cvGetReal1D(hist->bins,i);
			Point p1 = Point((i + 1)*bin_w, dstImage.rows - val*bin_u);
			rectangle(dstImage, p0, p1, cvScalar(0, 255), 1, 8, 0);//��Ӧ�ɰ��е�cvRectangle();
		}

		/*   ���̶�   */
		char string[12];//���ת����ʮ��������ת����ʮ���ƺ��λ��������12λ�������������Լ��趨
						//��������̶ȣ����ظ�����
		int kedu = 0;
		for (int i = 1; kedu<max; i++)
		{
			kedu = i*max / 10;//�˴�ѡ��10���̶�
			itoa(kedu, string, 10);//��һ������ת��Ϊ�ַ�����������е�10ָʮ����
								   //��ͼ������ʾ�ı��ַ���
			putText(dstImage, string, Point(0, dstImage.rows - kedu*bin_u), 1, 1, Scalar(0, 255, 255));//��Ӧ�ɰ��е�cvPutText����
		}
		//��������̶ȣ����ػҶ�ֵ��
		kedu = 0;
		for (int i = 1; kedu<256; i++)
		{
			kedu = i * 20;//�˴�ѡ����Ϊ20
			itoa(kedu, string, 10);//��һ������ת��Ϊ�ַ���
								   //��ͼ������ʾ�ı��ַ���
			putText(dstImage, string, cvPoint(kedu*(dstImage.cols / 256), dstImage.rows), 1, 1, Scalar(0, 255, 255));
		}
		namedWindow("Histogram", 1);
		imshow("Histogram", dstImage);
	}

}
