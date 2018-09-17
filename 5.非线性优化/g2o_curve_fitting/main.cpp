/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch6/g2o 学习记录
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.05, William Yu
                            
=================================================================================*/

/// Include Files
#include <iostream>
#include <g2o/core/base_vertex.h> //定点类型
#include <g2o/core/base_unary_edge.h> //一元边类型
#include <g2o/core/block_solver.h> //求解器
#include <g2o/core/optimization_algorithm_levenberg.h> //莱文贝格-马夸特方法 Levenberg-Marquardt算法
#include <g2o/core/optimization_algorithm_gauss_newton.h> //高斯牛顿法
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <Eigen/Core>
#include <opencv2/core/core.hpp>
#include <cmath>
#include <chrono> //计时库
using namespace std; 



/*-----------------------------[Note]---------------------------
# G2O图优化
深入理解图优化与g2o：图优化篇 http://www.cnblogs.com/gaoxiang12/p/5244828.html
深入理解图优化与g2o：g2o篇 https://www.cnblogs.com/gaoxiang12/p/5304272.html
--------------------------------------------------------------*/


/// Global Variables

/**
 * @class 
 * @brief 待优化变量
 */
// 曲线模型的顶点，模板参数：优化变量维度和数据类型
class CurveFittingVertex: public g2o::BaseVertex<3, Eigen::Vector3d>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void setToOriginImpl() // 重置
    {
        _estimate << 0,0,0;
    }
    
    virtual void oplusImpl( const double* update ) // 更新
    {
        _estimate += Eigen::Vector3d(update);
    }
    // 存盘和读盘：留空
    virtual bool read( istream& in ) {}
    virtual bool write( ostream& out ) const {}
};








/**
 * @class 
 * @brief 误差模型
 */
// 误差模型 模板参数：观测值维度，类型，连接顶点类型
class CurveFittingEdge: public g2o::BaseUnaryEdge<1,double,CurveFittingVertex>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    CurveFittingEdge( double x ): BaseUnaryEdge(), _x(x) {}
    // 计算曲线模型误差
    void computeError()
    {
        const CurveFittingVertex* v = static_cast<const CurveFittingVertex*> (_vertices[0]);
        const Eigen::Vector3d abc = v->estimate();
        _error(0,0) = _measurement - std::exp( abc(0,0)*_x*_x + abc(1,0)*_x + abc(2,0) ) ;
    }
    virtual bool read( istream& in ) {}
    virtual bool write( ostream& out ) const {}
public:
    double _x;  // x 值， y 值为 _measurement
};








/// Function Definitions

/**
 * @function main
 * @author William Yu
 * @brief 
 * @param  None
 * @retval None
 */
int main( int argc, char** argv )
{
    double a=1.0, b=2.0, c=1.0;         // 真实参数值
    int N=100;                          // 数据点
    double w_sigma=1.0;                 // 噪声Sigma值
    cv::RNG rng;                        // OpenCV随机数产生器
    double abc[3] = {0,0,0};            // abc参数的估计值

    vector<double> x_data, y_data;      // 数据
    
    cout<<"generating data: "<<endl;
    for ( int i=0; i<N; i++ )
    {
        double x = i/100.0;
        x_data.push_back ( x );
        y_data.push_back (
            exp ( a*x*x + b*x + c ) + rng.gaussian ( w_sigma ) //人为叠加高斯噪声
        );
        cout<<x_data[i]<<" "<<y_data[i]<<endl;
    }
    
    //-- 图优化过程
    typedef g2o::BlockSolver< g2o::BlockSolverTraits<3,1> > Block;   //误差项优化变量维度为3，误差值维度为1
    //线性求解器
    Block::LinearSolverType* linearSolver = new g2o::LinearSolverDense<Block::PoseMatrixType>(); 
    //稀疏矩阵求解
    Block* solver_ptr = new Block( linearSolver );     
    //迭代算法 从下面这三行 梯度下降方法，从高斯牛顿GN,  莱文贝格－马夸特方法LM, DogLeg中选择一个
    g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg( solver_ptr );
    // g2o::OptimizationAlgorithmGaussNewton* solver = new g2o::OptimizationAlgorithmGaussNewton( solver_ptr );
    // g2o::OptimizationAlgorithmDogleg* solver = new g2o::OptimizationAlgorithmDogleg( solver_ptr );
    //优化模型
    g2o::SparseOptimizer optimizer;   
    optimizer.setAlgorithm( solver );   
    optimizer.setVerbose( true ); 

    // //--[ERROR]see: https://www.cnblogs.com/xueyuanaichiyu/p/7921382.html
    // // 构建图优化，先设定g2o
    // typedef g2o::BlockSolver< g2o::BlockSolverTraits<3,1> > Block;  // 每个误差项优化变量维度为3，误差值维度为1
    // Block::LinearSolverType* linearSolver = new g2o::LinearSolverDense<Block::PoseMatrixType>(); // 线性方程求解器
    // Block* solver_ptr = new Block( std::unique_ptr<Block::LinearSolverType>(linearSolver) );      // 矩阵块求解器
    // // 梯度下降方法，从GN, LM, DogLeg 中选
    // g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg( std::unique_ptr<Block>(solver_ptr) );
    // // g2o::OptimizationAlgorithmGaussNewton* solver = new g2o::OptimizationAlgorithmGaussNewton( std::unique_ptr<Block>(solver_ptr) );
    // // g2o::OptimizationAlgorithmDogleg* solver = new g2o::OptimizationAlgorithmDogleg( std::unique_ptr<Block>(solver_ptr) );
    // g2o::SparseOptimizer optimizer;     // 图模型
    // optimizer.setAlgorithm( solver );   // 设置求解器
    // optimizer.setVerbose( true );       // 打开调试输出
    
    // 往图中增加顶点
    CurveFittingVertex* v = new CurveFittingVertex();
    v->setEstimate( Eigen::Vector3d(0,0,0) );
    v->setId(0);
    optimizer.addVertex( v );
    
    // 往图中增加边
    for ( int i=0; i<N; i++ )
    {
        CurveFittingEdge* edge = new CurveFittingEdge( x_data[i] );
        edge->setId(i);
        edge->setVertex( 0, v );                // 设置连接的顶点
        edge->setMeasurement( y_data[i] );      // 观测数值
        edge->setInformation( Eigen::Matrix<double,1,1>::Identity()*1/(w_sigma*w_sigma) ); // 信息矩阵：协方差矩阵之逆
        optimizer.addEdge( edge );
    }
    
    // 执行优化
    cout<<"start optimization"<<endl;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    optimizer.initializeOptimization();
    optimizer.optimize(100);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    cout<<"solve time cost = "<<time_used.count()<<" seconds. "<<endl;
    
    // 输出优化值
    Eigen::Vector3d abc_estimate = v->estimate();
    cout<<"estimated model: "<<abc_estimate.transpose()<<endl;
    
    return 0;
}