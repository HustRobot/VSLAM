/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲 学习记录
 *                           g2o solver进行位姿图优化
 * Reference               : http://www.cnblogs.com/reinforce/p/5510539.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.27, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/

/// Include Files
#include <iostream>
#include <fstream>
#include <string>

#include <g2o/types/slam3d/types_slam3d.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <g2o/solvers/cholmod/linear_solver_cholmod.h>
using namespace std;

/************************************************
 * 本程序演示如何用g2o solver进行位姿图优化
 * sphere.g2o是人工生成的一个Pose graph，我们来优化它。
 * 尽管可以直接通过load函数读取整个图，但我们还是自己来实现读取代码，以期获得更深刻的理解
 * 这里使用g2o/types/slam3d/中的SE3表示位姿，它实质上是四元数而非李代数.
 * **********************************************/




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
    if ( argc != 2 )
    {
        cout<<"Usage: pose_graph_g2o_SE3 sphere.g2o"<<endl;
        return 1;
    }
    ifstream fin( argv[1] );
    if ( !fin )
    {
        cout<<"file "<<argv[1]<<" does not exist."<<endl;
        return 1;
    }

    typedef g2o::BlockSolver<g2o::BlockSolverTraits<6,6>> Block;  // 6x6 BlockSolver
    Block::LinearSolverType* linearSolver = new g2o::LinearSolverCholmod<Block::PoseMatrixType>(); // 线性方程求解器
    Block* solver_ptr = new Block( linearSolver );      // 矩阵块求解器
    // 梯度下降方法，从GN, LM, DogLeg 中选
    g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg( solver_ptr );
    g2o::SparseOptimizer optimizer;     // 图模型
    optimizer.setAlgorithm( solver );   // 设置求解器

    int vertexCnt = 0, edgeCnt = 0; // 顶点和边的数量
    while ( !fin.eof() )
    {
        string name;
        fin>>name;
        if ( name == "VERTEX_SE3:QUAT" )
        {
            // SE3 顶点
            g2o::VertexSE3* v = new g2o::VertexSE3();
            int index = 0;
            fin>>index;
            v->setId( index );
            v->read(fin);
            optimizer.addVertex(v);
            vertexCnt++;
            if ( index==0 )
                v->setFixed(true);
        }
        else if ( name=="EDGE_SE3:QUAT" )
        {
            // SE3-SE3 边
            g2o::EdgeSE3* e = new g2o::EdgeSE3();
            int idx1, idx2;     // 关联的两个顶点
            fin>>idx1>>idx2;
            e->setId( edgeCnt++ );
            e->setVertex( 0, optimizer.vertices()[idx1] );
            e->setVertex( 1, optimizer.vertices()[idx2] );
            e->read(fin);
            optimizer.addEdge(e);
        }
        if ( !fin.good() ) break;
    }
    
    cout<<"read total "<<vertexCnt<<" vertices, "<<edgeCnt<<" edges."<<endl;
    
    cout<<"prepare optimizing ..."<<endl;
    optimizer.setVerbose(true);
    optimizer.initializeOptimization();
    cout<<"calling optimizing ..."<<endl;
    optimizer.optimize(30);
    
    cout<<"saving optimization results ..."<<endl;
    optimizer.save("result.g2o");

    return 0;
}
