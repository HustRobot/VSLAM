/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch5 学习记录
 * Reference               : 拼接点云
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.01, William Yu

=================================================================================*/

# 0.Requirements
- PCL

## Install PCL

* 1、下载PCL源代码
git clone https://github.com/PointCloudLibrary/pcl.git 或者使用/3rdparty文件夹下源码
建议将源码复制在~/src文件夹下编译安装，install之后源码文件夹不建议删除

* 2、安装依赖库
sudo apt-get install cmake g++ libboost1.58-all-dev libeigen3-dev libflann-dev python libusb-1.0-0-dev libudev-dev freeglut3-dev doxygen graphviz libpng12-dev libgtest-dev libxmu-dev libxi-dev libpcap-dev libqhull-dev libvtk5-qt4-dev python-vtk libvtk-java
安装完成后便可以编译pcl库

* 3、编译安装
说实话编译时间非常长，可能1小时内结束不了，编译前请考虑清楚。睡个午觉？起来再看
线程数根据情况选择。
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=None  -D BUILD_GPU=ON  -D BUILD_apps=ON  -D BUILD_examples=ON ..
make -j4
sudo make install




# 1.Compile this program:

* cmake编译
mkdir build
cd build
cmake ..
make



# 2.Usage:

生成点云
./joinMap
查看点云图

```
sudo apt-get install  pcl-tools
pcl_viewer map.pcd 
```

# 3.Others:
载入信息是彩色图片，深度图以及pose.txt

可能出现的问题
https://github.com/gaoxiang12/slambook/issues/129
这个问题很奇怪，修正之后，过几个月不小心干了什么就又坏掉了。