/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch4 学习记录
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.03.26, William Yu
                            
=================================================================================*/



# 0.Requirements
 - Sophus

## Install Sophus
可以在Github下载，https://github.com/strasdat/Sophus.git
或者使用/3rdparty文件夹下源码
建议将源码复制在~/src文件夹下编译安装，编译源码文件夹不要删除

* 获取源码，Github下载，或者使用/3rdparty文件夹下源码
cd ~/src
git clone https://github.com/strasdat/Sophus.git

* 编译
cd ~/src/Sophus
mkdir build
cd build
cmake ..
make
只需编译即可，无需安装，编译之后的Sophus文件夹不要删除！



# 1.Compile this program:

* 然后编译此程序
makmkdir build
cd build
cmake ..
make



# 2.Usage:
./useSophus