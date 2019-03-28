/*=================================================================================
 *	                    Copyleft! 2019 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2019 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch6/Ceres 学习记录
 * Reference               : http://ceres-solver.org/tutorial.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2019.03.26, William Yu
                            
=================================================================================*/

/// Include Files
#include <iostream>
#include <opencv2/opencv.hpp>
#include <ceres/ceres.h>


/// Global Variables



/// Function Declarations
bool function_show(std::vector<cv::Point2d>& in_point, int x_start_pix, int x_end_pix, int y_start_pix, int y_end_pix, double x_lim_l, double x_lim_h, double y_lim_l, double y_lim_h, cv::Mat& src); 

/**
 * @struct
 * @brief 代价函数的计算模型
 */
struct CURVE_FITTING_COST
{
    CURVE_FITTING_COST (double x,double y) : _x (x), _y (y) {}
    //残差的计算
    template <typename T>
    bool operator() ( const T* const abc, T* residual ) const
    {
        residual[0] = T ( _y ) - 
            /* Input your function here. */
            abc[0]*T(_x)*T(_x) + abc[1]*T(_x) + abc[2] ; // y=a*x^2+b*x+c
        return true;
    }
    const double _x, _y;    // x,y数据
};



/// Function Definitions

/**
 * @function main
 * @author William Yu
 * @brief 非线性优化求解最小二乘问题
 * @param  None
 * @retval None
 */
int main (int argc, char** argv)
{
    //--生成实验数据
    double a=0.01, b=0.23, c=1.0;  //真值
    int N=100;                   //点数
    double w_sigma=1.0;          //高斯噪声sigma值
    cv::RNG rng;                 //opencv随机数生成器
    double abc[3] = {0,0,0};     //abc参数估计值

    std::vector<double> x_data, y_data;      // 数据
    std::vector<cv::Point2d> dataset;
    std::cout<<"generating data: "<<std::endl;
    for ( int i=0; i<N; i++ )
    {
        //double x = i/100.0; //x范围从0到1，分辨率0.01
        double x = i;
        double /* Input your function here. */
            y = ( a*x*x + b*x + c ) //y=(ax^2+bx+c)
                + rng.gaussian (w_sigma); //+随机高斯噪声
        x_data.push_back ( x );
        y_data.push_back ( y );

        cv::Point2d point;
        point.x = x;
        point.y = y;
        dataset.push_back(point);
        std::cout<<x_data[i]<<" "<<y_data[i]<<std::endl;
    }





    //--构建最小二乘问题
    ceres::Problem problem;
    for ( int i=0; i<N; i++ )
    {
        problem.AddResidualBlock 
        (
            // 使用自动求导，模板参数：误差类型，输出维度，输入维度，维数要与前面struct中一致
            new ceres::AutoDiffCostFunction<CURVE_FITTING_COST, 1, 3> ( new CURVE_FITTING_COST ( x_data[i], y_data[i] ) ),
            nullptr,            // 核函数，这里不使用，为空
            abc                 // 待估计参数
        );
    }

    //--配置求解器
    ceres::Solver::Options options; //配置选项
    options.linear_solver_type = ceres::DENSE_QR; //增量方程如何求解
    options.minimizer_progress_to_stdout = true;  // 输出优化过程
    //Q:什么叫增量方程？
    //Q:还有那些求解方式？

    //--优化
    ceres::Solver::Summary summary;
    //计时
    int st1, et1;
	st1 = cvGetTickCount();
    ceres::Solve ( options, &problem, &summary );  // 开始优化
    et1 = cvGetTickCount();//计时结束
	std::cout<<"solve time cost = " << (et1 - st1) / (double)cvGetTickFrequency() / 1000.0 << "milliseconds\n\n";

    //输出结果
    std::cout<<summary.BriefReport() <<std::endl;
    std::cout<<"estimated a,b,c = ";
    for ( auto a:abc )  std::cout<<a<<" ";
    std::cout<<std::endl;

    cv::Mat background(320, 480, CV_8UC3, cv::Scalar(255,255,255));
    function_show(dataset, 0,200, 0,100, -100,100, 200,0, background);
    return 0;
}







/**
 * @function data2pix
 * @author William YU
 * @brief 数值转换为像素值
 * @param   
 * @retval None
 */
cv::Point data2pix(int origin_point_x_pix, int origin_point_y_pix, double x_dev, double y_dev, cv::Point2d data)
{
    cv::Point pixpoint;
    pixpoint.x = (int)(data.x/x_dev) + origin_point_x_pix;
    pixpoint.y = (int)(data.y/y_dev) + origin_point_y_pix;
    return pixpoint;
}

/**
 * @function data2pix
 * @author William YU
 * @brief 数值转换为像素值
 * @param   
 * @retval None
 */
cv::Point2d pix2data(int origin_point_x_pix, int origin_point_y_pix, double x_dev, double y_dev, cv::Point pix)
{
    cv::Point2f datapoint;
    datapoint.x = x_dev*(pix.x - origin_point_x_pix);
    datapoint.y = y_dev*(pix.y - origin_point_y_pix);
    return datapoint;
}


/**
 * @function polyfit_test
 * @author William YU
 * @brief 数学函数图像可视化
 * @param   in_point:锚点
 *          x_start:绘制曲线x像素起点
 *          x_end:绘制曲线x像素终点
 *          y_start:绘制曲线y像素起点
 *          y_end:绘制曲线y像素终点
 *          x_lim_l:x起始数值
 *          x_lim_h:x结束数值
 *          y_lim_l:y起始数值
 *          y_lim_h:y结束数值
 *          src:画布
 * @retval None
 */
//Reference:https://github.com/jack-Dong/testPolt/

bool function_show(std::vector<cv::Point2d>& in_point, int x_start_pix, int x_end_pix, int y_start_pix, int y_end_pix, double x_lim_l, double x_lim_h, double y_lim_l, double y_lim_h, cv::Mat& src) 
{	
    //--坐标转换
	int x_pix = x_end_pix-x_start_pix;//x像素区间
    double x_data = x_lim_h - x_lim_l;//x数值区间
    double x_dev = x_data/x_pix;//x分辨率:每个像素代表的x增量
    int y_pix = y_end_pix-y_start_pix;//y像素区间
    double y_data = y_lim_h - y_lim_l;//y数值区间
    double y_dev=y_data/y_pix;//y分辨率:每个像素代表的y增量
    int origin_point_x_pix,origin_point_y_pix;
    //计算原点
    origin_point_x_pix = -x_lim_l / x_dev + x_start_pix;
    origin_point_y_pix = -y_lim_l / y_dev + y_start_pix;
    
    //--绘制坐标轴
    cv::circle(src, cv::Point(origin_point_x_pix,origin_point_y_pix), 3, cv::Scalar(0, 0, 0), CV_FILLED, 4);
    cv::line(src, cv::Point(x_start_pix,origin_point_y_pix), cv::Point(x_end_pix,origin_point_y_pix), cv::Scalar(0, 0, 0), 1, 4);
    cv::line(src, cv::Point(origin_point_x_pix,y_start_pix), cv::Point(origin_point_x_pix,y_end_pix), cv::Scalar(0, 0, 0), 1, 4);
    
    //--绘制锚点
	for (int i = 0; i < in_point.size(); i++)
	{
		cv::Point ipt_pix = data2pix(origin_point_x_pix,origin_point_y_pix,x_dev,y_dev, in_point[i]);
		cv::circle(src, ipt_pix, 1, cv::Scalar(0, 0, 255), CV_FILLED, 4);
	}

    // //--绘制函数曲线
    // for (int i = x_start_pix; i < x_end_pix+1; i++)
	// {
	// 	cv::Point ipt_pix;
    //     cv::Point2d ipt_data;
	// 	ipt_pix.x = i;
    //     ipt_pix.y = 0;
    //     ipt_data = pix2data(origin_point_x_pix,origin_point_y_pix,x_dev,y_dev, ipt_pix);
    //     /* Input your function here. */
    //     ipt_data.y = 0.25*ipt_data.x*ipt_data.x + 2*ipt_data.x - 21;  //ipt_data.y=a*x*x+b*x+c;  //y=(ax^2+bx+c)
	// 	ipt_pix = data2pix(origin_point_x_pix,origin_point_y_pix,x_dev,y_dev, ipt_data);
	// 	cv::circle(src, ipt_pix, 1, cv::Scalar(255, 255, 255), CV_FILLED, CV_AA);
	// }
	


	cv::imshow("polyfit_show", src);
	cv::waitKey(0);
    return 1;
}


