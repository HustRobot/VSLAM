/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲 学习记录
 *                           后端优化方案(SE位姿下的g2o优化 + 李代数位姿下的g2o优化 + gtsam因子图优化)
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.27, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/


# 0.Requirements
 - gtsam

## Install gtsam
可以在Github下载，https://bitbucket.org/gtborg/gtsam.git
或者使用/3rdparty文件夹下源码
建议将源码复制在~/src文件夹下编译安装，install之后源码文件夹不建议删除

* 安装依赖项
$ sudo apt-get install libtbb-dev

* 获取源码，Github下载，或者使用/3rdparty文件夹下源码
cd ~/src
git clone https://bitbucket.org/gtborg/gtsam.git

* 编译安装
cd gtsam
mkdir build
cd build
cmake ..
make -j4
sudo make install









# 1.To build the code : 
mkdir build
cd ./build
cmake ..
make








# 2.How to run the code :

* SE位姿下的g2o优化 
./pose_graph_g2o_SE3 ../sphere.g2o
输出result.g2o
查看:
$ g2o_viewer result.g2o


* 李代数位姿下的g2o优化
./pose_graph_g2o_lie ../sphere.g2o
输出result_lie.g2o
查看:
$ g2o_viewer result_lie.g2o


* gtsam因子图优化
./pose_graph_gtsam ./sphere.g2o
输出result_gtsam.g2o
查看:
$ g2o_viewer result_gtsam.g2o