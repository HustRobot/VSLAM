/*=================================================================================
 *                      Copyleft! 2017 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2017 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : OpenCV测试
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2017.10.08, William Yu
                            
=================================================================================*/

# 0.Requirements
 - OpenCV

## Install opencv

推荐使用3.3.1版本
可以在OpenCV官网下载，或者使用/3rdparty文件夹下源码
建议将源码放在~/src文件夹下编译安装，install之后源码文件夹不建议删除

* 安装依赖项
$ sudo apt-get install -y build-essential
$ sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
$ sudo apt-get install -y libgfortran3
$ sudo apt-get install -y --allow-unauthenticated libgfortran3
$ sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
$ sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

* 获取源码，opencv官网下载，或者使用/3rdparty文件夹下源码
$ cd ~/src/opencv-3.3.1
$ ls
opencv-master.zip   opencv-master   opencv_contrib-master.zip  opencv_contrib-master
$ cd opencv-master
$ ls
3rdparty cmake CONTRIBUTING.md doc LICENSE platforms samples apps CMakeLists.txt data include modules README.md

* 编译安装
$ mkdir build
$ cd build/
$ cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D INSTALL_PYTHON_EXAMPLES=ON -D INSTALL_C_EXAMPLES=ON -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib_master/modules -D PYTHON_EXECUTABLE=/usr/bin/python3.5 -D BUILD_EXAMPLES=ON ..
$ make -j4
$ sudo make install
[注意]：sudo make install之后，~/src/opencv3.3.1/opencv/build文件夹切勿删除







# 1.Compile this program:

* cmake编译
mkdir build
cd build
cmake ..
make

* g++编译
g++ opencvtest.cpp -o opencvtest `pkg-config  --cflags --libs opencv`






2.Usage:
./opencvtest <path_to_image>
eg:
如果使用的是cmake编译，那么./opencvtest ../ubuntu.png
如果使用的是g++编译，那么./opencvtest ./ubuntu.png
这样讲应该是比较清楚了，个人习惯而已，或者干脆把cmake编译的几句命令写个脚本吧ycmake.sh

获取帮助信息：
./opencvtest -h
