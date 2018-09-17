/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch4 学习记录
 *                           李代数库
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.03.26, William Yu
                            
=================================================================================*/
/// Include files
#include <iostream>
#include <cmath>
using namespace std; 

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "sophus/so3.h"
#include "sophus/se3.h"



/// Global Variables



/// Function definitions


/**
 * @function main
 * @author William Yu
 * @brief 
 * @param  None
 * @retval None
 */
int main( int argc, char** argv )
{
    //--------------------------------------------------------------------------------------------------
    //  旋转矩阵
    //--------------------------------------------------------------------------------------
    // 沿Z轴转90度的旋转矩阵
    // 　                                  角度　　                  轴　　罗德里格公式进行转换为旋转矩阵　
    Eigen::Matrix3d R = Eigen::AngleAxisd(M_PI/2, Eigen::Vector3d(0,0,1)).toRotationMatrix();
    

    //--------------------------------------------------------------------------------------------------
    //  李群
    //--------------------------------------------------------------------------------------
    Sophus::SO3 SO3_R(R);               // Sophus::SO(3)可以直接从旋转矩阵构造
    Sophus::SO3 SO3_v( 0, 0, M_PI/2 );  // 亦可从旋转向量构造
    Eigen::Quaterniond q(R);            // 或者四元数
    Sophus::SO3 SO3_q( q );
    // 上述表达方式都是等价的
    // 输出SO(3)时，以so(3)形式输出
    cout<<"SO(3) from matrix: "<<SO3_R<<endl;
    cout<<"SO(3) from vector: "<<SO3_v<<endl;
    cout<<"SO(3) from quaternion :"<<SO3_q<<endl;
    

    //--------------------------------------------------------------------------------------------------
    //  李代数
    //--------------------------------------------------------------------------------------
    // 使用对数映射获得它的李代数
    Eigen::Vector3d so3 = SO3_R.log();
    cout<<"so3 = "<<so3.transpose()<<endl;
    // hat 为向量到反对称矩阵
    cout<<"so3 hat=\n"<<Sophus::SO3::hat(so3)<<endl;
    // 相对的，vee为反对称到向量
    cout<<"so3 hat vee= "<<Sophus::SO3::vee( Sophus::SO3::hat(so3) ).transpose()<<endl; // transpose纯粹是为了输出美观一些
    

    //--------------------------------------------------------------------------------------------------
    //  李代数求导 更新
    //--------------------------------------------------------------------------------------
    // 增量扰动模型的更新
    Eigen::Vector3d update_so3(1e-4, 0, 0); //假设更新量为这么多
    Sophus::SO3 SO3_updated = Sophus::SO3::exp(update_so3)*SO3_R;
    cout<<"SO3 updated = "<<SO3_updated<<endl;
    





    /********************我是萌萌的分割线*****萌萌是谁？************************/
    //说实话，在这份代码里面你可能经常看到一些略微卖萌的成分，
    //要么是高翔博士的原版代码在卖萌，
    //要么是我被高博的代码带上了一条不归路，总之，开心就好：）
    
    cout<<"************我是分割线*************"<<endl;
 
 
    //--------------------------------------------------------------------------------------------------
    //  特殊欧式群  变换矩阵群
    //--------------------------------------------------------------------------------------
    // 对SE(3)操作大同小异
    Eigen::Vector3d t(1,0,0);           // 沿X轴平移1
    Sophus::SE3 SE3_Rt(R, t);           // 从R,t构造SE(3)
    Sophus::SE3 SE3_qt(q,t);            // 从q,t构造SE(3)
    cout<<"SE3 from R,t= "<<endl<<SE3_Rt<<endl;
    cout<<"SE3 from q,t= "<<endl<<SE3_qt<<endl;
    // 李代数se(3) 是一个六维向量，方便起见先typedef一下
    typedef Eigen::Matrix<double,6,1> Vector6d;
    Vector6d se3 = SE3_Rt.log();
    cout<<"se3 = "<<se3.transpose()<<endl;
    // 观察输出，会发现在Sophus中，se(3)的平移在前，旋转在后.
    // 同样的，有hat和vee两个算符
    cout<<"se3 hat = "<<endl<<Sophus::SE3::hat(se3)<<endl;
    cout<<"se3 hat vee = "<<Sophus::SE3::vee( Sophus::SE3::hat(se3) ).transpose()<<endl;
    
    // 最后，演示一下更新
    Vector6d update_se3; //更新量
    update_se3.setZero();
    update_se3(0,0) = 1e-4d;
    Sophus::SE3 SE3_updated = Sophus::SE3::exp(update_se3)*SE3_Rt;
    cout<<"SE3 updated = "<<endl<<SE3_updated.matrix()<<endl;
    
    return 0;
}