/*=================================================================================
 *                      Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 自己实现的高斯滤波器与OpenCV滤波器的对比
 * Reference               : https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#gaabe8c836e97159a9193fb0b11ac52cf1
 *                           https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#gac05a120c1ae92a6060dd0db190a61afa
 * 				             https://docs.opencv.org/3.3.1/de/db2/laplace_8cpp-example.html#a17
 *				             http://blog.csdn.net/weixin_37720172/article/details/72843238 该博文代码有错误,已与作者联系
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2018.03.22, William Yu
                             ver1.1, 2018.03.23, William Yu, add notes
                             ver1.2, 2018.03.31, William Yu, add Q1
                			      Q1: 卷积核越大，图像整体越暗(solved)
			                      A1: double temp 
			                 ver1.3, 2018.04.01, William Yu, Q1 solved
=================================================================================*/
/// Include Files
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;



/// Function Definitions

//--------------------------------【 myconvolution()函数 】----------------------------------------------
//	void myconvolution(const Mat& myImage, Mat& Result, double ** kernel, int ksize);
//		参数
//			& myImage  :输入图像
//			& Result   :输出图像
//			& kernel   :卷积核
//			ksize      :卷积核大小
//------------------------------------------------------------------------------------------------- 
void myconvolution(const Mat& myImage, Mat& Result, double ** kernel, int ksize)
{
    const int nChannels = myImage.channels();
    int center =(int) ((ksize)/2);
    
    for(int j = center; j < myImage.rows - center; ++j)
    {
	///创建行指针首地址序列
	vector<const uchar*> col_ptrs;
	for(int k= 0; k< ksize; ++k)
	{
	  const uchar* col_ptr = myImage.ptr<uchar>(j- center + k );  
	  col_ptrs.push_back(col_ptr);
	}
	
	uchar* output = Result.ptr<uchar>(j); //结果图片行首地址
	output += nChannels * center; //将行首地址加偏移量
	
        for(int i = nChannels * center; i < nChannels*myImage.cols - nChannels * center; ++i)
        {
	  ///卷积操作
	  double temp = 0;
	  for(int aaa=0; aaa<ksize; ++aaa) 
	  {
	     for(int bbb=0; bbb<ksize; ++bbb)
	     {
		temp += kernel[aaa][bbb] * col_ptrs[aaa][i - nChannels*center + nChannels*bbb];
	     }
	  }
	  ///saturate_cast函数,强制限定为uchar ： a>255则a=255,a<0，则a=0
	  *output++ = saturate_cast<uchar>(temp); 
	}
    }
}



//--------------------------------【 mygetGaussianKernel()函数 】----------------------------------------------
//	double ** mygetGaussianKernel(int ksize, double sigma);
//		参数
//			ksize      :模板大小
//			sigma      :高斯标准差
//------------------------------------------------------------------------------------------------- 
double ** mygetGaussianKernel(int ksize, double sigma)
{
    int i, j;  
    double sum = 0.0;  
    int center = ksize/2;
    
    ///建立一个size*size大小的动态二维数组  
    double **arr = new double*[ksize];
    for (i = 0; i < ksize; ++i)  
        arr[i] = new double[ksize];  
    
    ///高斯函数  
    for (i = 0; i < ksize; ++i)  
        for (j = 0; j < ksize; ++j) 
	{  
            arr[i][j] = exp(-((i - center)*(i - center) + (j - center)*(j - center)) / (sigma*sigma * 2));  
            sum += arr[i][j];  
        }
        
    ///归一化
    double chacksum=0;
    for (i = 0; i < ksize; ++i)  
        for (j = 0; j < ksize; ++j)  
	{   
	  arr[i][j] /= sum;
	  chacksum += arr[i][j];
	}
    
    ///输出
    printf("%.10f ",chacksum);
    for (i = 0; i < ksize; ++i)
      for (j = 0; j < ksize; ++j)
      {
	if ( j%ksize ==0) { printf("\n");}
	printf("%.10f ",arr[i][j]);
      }
    
    return arr;
}




//--------------------------------【 myGaussianBlur()函数 】----------------------------------------------
//	void myGaussianBlur(Mat& srcImage, Mat& dstImage, int ksize, int sigma);
//		描述	
//			高斯平滑滤波
//		参数 
//			& srcImage :输入图像
//			& dstImage :输出图像
//			ksize      :模板大小
//			sigma      :高斯标准差
//-------------------------------------------------------------------------------------------------    
void myGaussianBlur(const Mat& srcImage, Mat& dstImage, int ksize, double sigma)
{
    if(srcImage.empty()) {printf("Cannot read image file"); return; }
    //--------------------------------【 mygetGaussianKernel()函数 】----------------------------------------------
    //		获取高斯滤波器系数. 
    //		Returns Gaussian filter coefficients.
    //-------------------------------------------------------------------------------------------------
    double **kernel; 
    kernel = mygetGaussianKernel(ksize, sigma); 
    
    //--------------------------------【 convolution()函数 】----------------------------------------------
    //		卷积计算
    //-------------------------------------------------------------------------------------------------
    myconvolution(srcImage, dstImage, kernel, ksize);
}





static void help()
{
    cout << "\n这是一个演示自己实现的高斯滤波器与OpenCV滤波器的对比,以及进行高斯平滑滤波的程序"
            "\n使用OpenCV version " << CV_VERSION << endl;
    cout << "\n使用:\n"
            "\t./Gaussian_filter [图片名称 -- 默认值为 ubuntu.png]\n\n";
	
    cout << "\nBlurs an image using a Gaussian filter."
            "\nUsing OpenCV version " << CV_VERSION << endl;
    cout << "\nCall:\n"
            "    ./Gaussian_filter [image_name -- Default is ubuntu.png]\n\n";
}

const char* keys =
{
    "{help h||}{@image |ubuntu.png|input image name}"
};



/**
 * @function main
 * @author William Yu
 * @brief 
 * @param  None
 * @retval None
 */
int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, keys); 
    if (parser.has("help"))  { help();  return 0; }
    string filename = parser.get<string>(0);

    Mat srcImage;
    srcImage= imread(filename, 1);
    if(srcImage.empty()) {printf("Cannot read image file: %s\n", filename.c_str()); return -1; }
    
    Mat dst_myGaussianBlur;
    dst_myGaussianBlur.create(srcImage.size(), srcImage.type());
    
    Mat dst_opencvGaussianBlur;
    dst_opencvGaussianBlur.create(srcImage.size(), srcImage.type());




    //--------------------------------【 myGaussianBlur()函数 】----------------------------------------------
    //		高斯平滑滤波.
    //-------------------------------------------------------------------------------------------------
    //【方式一】指定模板大小,并由模板大小计算sigma
    //see: https://docs.opencv.org/3.3.1/d4/d86/group__imgproc__filter.html#gac05a120c1ae92a6060dd0db190a61afa
    int ksize= 15;   
    double sigma = 0.3*((ksize-1)*0.5 - 1) + 0.8;
    //【方式二】指定sigma,并由sigma计算模板大小
    //通过sigma得到模板大小 仅需要取均值周围3倍标准差内的值，以外部份直接去掉即可
    //double sigma= 0.7;
    //int ksize = 2* ceil(3*sigma)-1;//ceil函数向上取整，且需要保证为奇数
    
    myGaussianBlur(srcImage, dst_myGaussianBlur, ksize, sigma);
    //归定化到0~255  
    //normalize(dst_myGaussianBlur, dst_myGaussianBlur, 0, 255, CV_MINMAX);
    






    //--------------------------------【 GaussianBlur()函数 】----------------------------------------------
    //         opencv提供的高斯平滑滤波函数
    //         void GaussianBlur( InputArray src, OutputArray dst, Size ksize,
    //                            double sigmaX, double sigmaY = 0,
    //                            int borderType = BORDER_DEFAULT );
    //-------------------------------------------------------------------------------------------------
    double sigmax = sigma;
    double sigmay = sigmax; //此处取相等
    int ksizew = ksize;
    int ksizeh = ksizew;//此处取相等
    GaussianBlur(srcImage, dst_opencvGaussianBlur, Size(ksizew, ksizeh), sigmax, sigmay);
    
    





    namedWindow("srcImage", WINDOW_AUTOSIZE);
    imshow("srcImage", srcImage);
    namedWindow("dst_myGaussianBlur",WINDOW_AUTOSIZE);
    imshow("dst_myGaussianBlur",dst_myGaussianBlur);
    namedWindow("dst_opencvGaussianBlur", WINDOW_AUTOSIZE);
    imshow("dst_opencvGaussianBlur", dst_opencvGaussianBlur);
    
    waitKey(0);
    return 0;
}
