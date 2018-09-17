/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch6/Ceres 学习记录
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.05, William Yu
                            
=================================================================================*/


# 0.Requirements
 - ceres

## Install ceres

* 安装依赖库
sudo apt-get install liblapack-dev libsuitesparse-dev libcxsparse3.1.4 libgflags-dev libgoogle-glog-dev libgtest-dev

* 安装Ceres,下载Ceres源代码
git clone https://github.com/ceres-solver/ceres-solver.git 
或者可以使用3rdparty/文件夹下的Ceres库

* 编译安装
建议将源码放在~/src文件夹下，install之后源码文件夹不建议删除
cd <path_to_Ceres>
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

