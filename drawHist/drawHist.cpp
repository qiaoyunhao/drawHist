#pragma warning(disable: 4996)
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/*  全局变量的声明及初始化    */
Mat srcImage; //读入的图片矩阵
vector<Mat> rgb_planes; //存放rgb颜色三通道值
Mat dstImage; //读入的图片矩阵
MatND dstHist; //直方图矩阵，对应老版本中的cvCreateHist（）
int g_hdims = 50;     // 划分HIST的初始个数，越高越精确

					  /* 回调函数声明 */
void on_HIST(int t, void *);


/*   主函数   */
int main(int argc, char** argv)
{

	srcImage = imread("E:\\0029.jpg");//"0"表示读入灰度图像
	//namedWindow("原图", 1);
	imshow("原图", srcImage);
	// 分割成3个单通道图像 ( R, G 和 B )
	
	split(srcImage, rgb_planes);

	createTrackbar("hdims", "原图", &g_hdims, 256, on_HIST);
	on_HIST(0, 0);//调用滚动条回调函数
	cvWaitKey(0);
	return 0;
}

/** 滚动条回调函数 **/
void on_HIST(int t, void *)
{
	dstImage = Mat::zeros(512, 800, CV_8UC3);//初始化一个全黑画布画直方图，每次都要初始化
	float hranges[] = { 0,255 }; //灰度范围
	const float *ranges[] = { hranges };//灰度范围的指针

	if (g_hdims == 0)
	{
		printf("直方图条数不能为零！\n");
	}
	else
	{   /*
		srcImage:读入的矩阵
		1:数组的个数为1
		0：因为灰度图像就一个通道，所以选0号通道
		Mat（）：表示不使用掩膜
		dstHist:输出的目标直方图
		1：需要计算的直方图的维度为1
		g_hdims:划分HIST的个数
		ranges:表示每一维度的数值范围
		*/
		//int channels=0;
		//想画那个通道就改这里就行
		calcHist(&rgb_planes[0], 1, 0, Mat(), dstHist, 1, &g_hdims, ranges); // 计算直方图
							
		      /* 获取最大最小值 */
		double max = 0;
		minMaxLoc(dstHist, NULL, &max, 0, 0);// 寻找最大值及其位置
											 
		 /**** 绘出直方图 ****/
		double bin_w = (double)dstImage.cols / g_hdims;  // hdims: 条的个数，则 bin_w 为条的宽度
		double bin_u = (double)dstImage.rows / max;  // max: 最高条的像素个数，则 bin_u 为单个像素的高度

		 /** 画直方图 **/
		for (int i = 0; i<g_hdims; i++)
		{
			Point p0 = Point(i*bin_w, dstImage.rows);//找开始画的点 (即p0为直方图的第0条的左下角点)；

			int val = dstHist.at<float>(i);//计算第i条的像素个数 (即 第i条直方图y方向上的像素个数），注意一点要用float类型；
			Point p1 = Point((i + 1)*bin_w, dstImage.rows - val*bin_u);//(i+1)*bin_w是直方图的第i条的右上角点 x坐标，右上角有坐标为 dstImage.rows - val*bin_u是因为在图像中以左上角点为图像原点
			rectangle(dstImage, p0, p1, cvScalar(0, 255), 1, 8, 0);//画矩形条: p0为矩形条左下角点， p1为右上角点
		}

		 /** 画刻度 **/
		char string[12];//存放转换后十进制数，转化成十进制后的位数不超过12位，这个根据情况自己设定
						//画纵坐标刻度（像素个数）
		int kedu = 0;
		for (int i = 1; kedu<max; i++)
		{
			kedu = i*max / 10;//此处选择10个刻度
			itoa(kedu, string, 10);//把一个整数转换为字符串，这个当中的10指十进制
								   //在图像中显示文本字符串
			putText(dstImage, string, Point(0, dstImage.rows - kedu*bin_u), 1, 1, Scalar(0, 255, 255));//对应旧版中的cvPutText（）
		}
		//画横坐标刻度（像素灰度值）
		kedu = 0;
		for (int i = 1; kedu<256; i++)
		{
			kedu = i * 20;//此处选择间隔为20
			itoa(kedu, string, 10);//把一个整数转换为字符串
								   //在图像中显示文本字符串
			putText(dstImage, string, cvPoint(kedu*(dstImage.cols / 256), dstImage.rows), 1, 1, Scalar(0, 255, 255));
		}
		namedWindow("Histogram", 1);
		imshow("Histogram", dstImage);
	}

}
