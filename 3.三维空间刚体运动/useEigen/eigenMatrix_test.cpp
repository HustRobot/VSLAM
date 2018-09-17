/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch3/useEigen/eigenMatrix.cpp 学习记录
								Eigen矩阵库                    
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.03.26, William Yu
                            
=================================================================================*/
/// Include Files
#include<iostream>
#include<ctime>
#include <Eigen/Core>
//稠密矩阵的代数运算（求逆，特征值等）
#include <Eigen/Dense>

using namespace std;


/// Global Variables
#define MATRIX_SIZE 50 //矩阵大小



/// Function Definitions


/**
 * @function main
 * @author William Yu
 * @brief Eigen基本类型使用示例
 * @param  None
 * @retval None
 */
int main(int argc, char** argv )
{
    //---------------------------------------------------------------------------
    //  创建矩阵&初始化操作
    //----------------------------------------------------------------
    // 声明一个2*3的float矩阵
    Eigen::Matrix<float,2,3> matrix_23;
    
    // 声明一个3*1的矩阵
    Eigen::Matrix<float,3,1>vd_3d;
    // 或者下列方式也可以
    Eigen::Vector3d v_3d;
    
    // Matrix3d 实质上是 Eigen::Matrix<double, 3, 3>
    Eigen::Matrix3d matrix_33 = Eigen::Matrix3d::Zero(); //初始化为零

    // 建立动态大小的矩阵
    Eigen::Matrix< double, Eigen::Dynamic, Eigen::Dynamic > matrix_dynamic;
    // 更简单的写法
    Eigen::MatrixXd matrix_x;
    
    // 随机数矩阵Random填充
    matrix_33 = Eigen::Matrix3d::Random();
    cout << "创建随机矩阵" << endl;
    cout << matrix_33 << endl << endl;
    





    //---------------------------------------------------------------------------
    //  矩阵元素操作
    //----------------------------------------------------------------
    // 输入数据（初始化）
    matrix_23 << 1, 2, 3, 4, 5, 6;
    // 直接整体输出
    cout << "matrix_23:" << endl;
    cout << matrix_23 << endl;
    // 用for()访问矩阵中的元素
    for (int i=0; i<2; i++) {
        for (int j=0; j<3; j++)
            cout<<matrix_23(i,j)<<"\t";
        cout<<endl;
    }
    




    //---------------------------------------------------------------------------
    //  矩阵运算
    //----------------------------------------------------------------
    // 矩阵和向量相乘（实际上仍是矩阵和矩阵相乘）
    v_3d << 3, 2, 1;
    vd_3d << 4, 5, 6;
    // 但是在Eigen里你不能混合两种不同类型的矩阵，像这样是错的
    // Eigen::Matrix<double, 2, 1> result_wrong_type = matrix_23 * v_3d;
    // 应该显式转换
    Eigen::Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
    cout << result << endl;

    Eigen::Matrix<float, 2, 1> result2 = matrix_23 * vd_3d;
    cout << result2 << endl;
    // 同样你不能搞错矩阵的维度
    // 试着取消下面的注释，看看Eigen会报什么错
    // Eigen::Matrix<double, 2, 3> result_wrong_dimension = matrix_23.cast<double>() * v_3d;


    //-- 矩阵运算
    // 四则运算就不演示了，直接用+-*/即可。
    cout << matrix_33.transpose() << endl;      // 转置
    cout << matrix_33.sum() << endl;            // 各元素和
    cout << matrix_33.trace() << endl;          // 迹
    cout << 10*matrix_33 << endl;               // 数乘
    cout << matrix_33.inverse() << endl;        // 逆
    cout << matrix_33.determinant() << endl;    // 行列式
    





    //---------------------------------------------------------------------------
    //  特征值求解
    //----------------------------------------------------------------
    // 实对称矩阵可以保证对角化成功
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigen_solver ( matrix_33.transpose()*matrix_33 );
    cout << "Eigen values = \n" << eigen_solver.eigenvalues() << endl;
    cout << "Eigen vectors = \n" << eigen_solver.eigenvectors() << endl;

    



    //---------------------------------------------------------------------------
    //  矩阵方程
    //----------------------------------------------------------------
    // 求解 matrix_NN * x = v_Nd 这个方程
    // N的大小在前边的宏里定义，它由随机数生成
    // 直接求逆自然是最直接的，但是求逆运算量大
    Eigen::Matrix< double, MATRIX_SIZE, MATRIX_SIZE > matrix_NN;
    matrix_NN = Eigen::MatrixXd::Random( MATRIX_SIZE, MATRIX_SIZE );
    Eigen::Matrix< double, MATRIX_SIZE,  1> v_Nd;
    v_Nd = Eigen::MatrixXd::Random( MATRIX_SIZE,1 );
    
    clock_t time_stt = clock(); // 计时函数
    //[1]-- 直接求逆
    Eigen::Matrix<double,MATRIX_SIZE,1> x = matrix_NN.inverse()*v_Nd;
    cout <<"time use in normal inverse is " << 1000* (clock() - time_stt)/(double)CLOCKS_PER_SEC << "ms"<< endl;
    
    //[2]-- 通常用矩阵分解来求，例如QR分解，速度会快很多
    time_stt = clock();
    x = matrix_NN.colPivHouseholderQr().solve(v_Nd);
    cout <<"time use in Qr decomposition is " <<1000*  (clock() - time_stt)/(double)CLOCKS_PER_SEC <<"ms" << endl;

    return 0;
}



