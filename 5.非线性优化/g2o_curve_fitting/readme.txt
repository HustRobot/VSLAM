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

# 0.Requirements
 - g2o

## Install g2o

* 安装依赖库
sudo apt-get install libqt4-dev qt4-qmake libqglviewer-dev libsuitesparse-dev libcxsparse3.1.4 libcholmod3.0.6

* 安装g2o点云库,下载g2o源代码
git clone https://github.com/RainerKuemmerle/g2o.git
或者可以使用3rdparty/文件夹下的g2o库，建议使用书中的版本，说实话不是建议，是跪求，踩了的坑千万别再跳了  原因：查看第3条

* 编译安装
建议将源码复制在~/src文件夹下编译安装，install之后源码文件夹不建议删除
cd <path_to_g2o>
mkdir build
cd build
cmake ..
make -j4
sudo make install




# 1.Compile this program:

* cmake编译
mkdir build
cd build
cmake ..
make





# 2.Usage:
./curve_fitting





# 3.Others

注意：如果安装了Github版本的g2o，《视觉slam十四讲》中程序在ch6/g2o_curve_fitting/main.cpp-Line79 报错 【error】


* 报错部分代码ch6/g2o_curve_fitting/main.cpp-Line74-84：
typedef g2o::BlockSolver< g2o::BlockSolverTraits<3,1> > Block; 
Block::LinearSolverType* linearSolver = new g2o::LinearSolverDense<Block::PoseMatrixType>(); 
Block* solver_ptr = new Block( linearSolver );     
g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg( solver_ptr );
g2o::SparseOptimizer optimizer;   
optimizer.setAlgorithm( solver );   
optimizer.setVerbose( true ); 


* 按照书上的例程编写代码，编译时报错（部分）如下：

/home/will/code/slambook/useg2o/main.cpp:77:49: error: no matching function for call to ‘g2o::BlockSolver<g2o::BlockSolverTraits<3, 1> >::BlockSolver(g2o::BlockSolver<g2o::BlockSolverTraits<3, 1> >::LinearSolverType*&)’
 Block* solver_ptr = new Block( linearSolver );

/home/will/code/slambook/useg2o/main.cpp:77:49: note: candidate is:
In file included from     /usr/local/include/g2o/core/block_solver.h:199:0,
             from /home/will/code/slambook/useg2o/main.cpp:4:
/usr/local/include/g2o/core/block_solver.hpp:40:1: note: g2o::BlockSolver<Traits>::BlockSolver(std::unique_ptr<typename Traits::LinearSolverType>) [with Traits = g2o::BlockSolverTraits<3, 1>; typename Traits::LinearSolverType = g2o::LinearSolver<Eigen::Matrix<double, 3, 3> >]
 BlockSolver<Traits>::BlockSolver(std::unique_ptr<LinearSolverType> linearSolver)




* 修改代码如下：

typedef g2o::BlockSolver< g2o::BlockSolverTraits<3,1> > Block; 
Block::LinearSolverType* linearSolver = new g2o::LinearSolverDense<Block::PoseMatrixType>();
//---以下改动---
Block* solver_ptr = new Block( std::unique_ptr<Block::LinearSolverType>(linearSolver) );
g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg(std::unique_ptr<Block>(solver_ptr) );
//---以上改动---
g2o::SparseOptimizer optimizer;   
optimizer.setAlgorithm( solver );   
optimizer.setVerbose( true ); 
前面还是用普通指针来赋值，之后再转换成unique_ptr.



* 资料
1、std::unique_ptr 智能指针 http://blog.csdn.net/pi9nc/article/details/12227887 unique_ptr“唯一”拥有其所指对象，同一时刻只能有一个unique_ptr指向给定对象（通过禁止拷贝语义、只有移动语义来实现）。
2、关于g2o的初始化问题 https://www.cnblogs.com/serser/p/7815588.html



--------------------------------------------------------------------
Q： 程序员最想有个女朋友的时刻？
A： 当他解决了一个困扰了许久的问题，超级激动得想砸电脑，想跟别人分享的时候，
然而在华科这样的环境下。。。还是好好爱惜我的电脑吧。。。
--------------------------------------------------------------------

