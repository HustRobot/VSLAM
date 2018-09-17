/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch3/visualizeGeometry/visualizeGeometry.cpp 学习记录
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.03.29, William Yu
                            
=================================================================================*/



----------------------------------------------------------------------------------
                                    2018 William Yu 译    
----------------------------------------------------------------------------------

1.如何编译此程序：
* 使用pangolin: slambook/3rdpart/Pangolin 或者从github下载: https://github.com/stevenlovegrove/Pangolin

* 安装pangolin的依赖(主要是OpenGL):
sudo apt-get install libglew-dev

* 编译安装pangolin
建议将源码复制在~/src文件夹下编译安装，install之后源码文件夹不建议删除
cd <path_to_pangolin>
mkdir build
cd build
cmake ..
make 
sudo make install 
安装之后,pangolin源码文件夹不建议删除

* 编译此程序:
mkdir build
cd build
cmake ..
make 

* 运行文件 
./visualizeGeometry





2. 如何使用这个程序:

* 左边标签里面的界面展示了T_w_c（相机坐标系到世界坐标系）的几种不同的表示方法。 他显示了旋转矩阵, 变换向量, 欧拉角 (按照 roll-pitch-yaw 顺序) 以及四元素.
拖动鼠标左键移动相机, 右键围绕方块旋转, 中键旋转相机本身, 同时按住左右按键旋转视角. 
注意在这个程序中原始 X 坐标向右 (红线), Y 向上 (绿线) 以及 Z 坐标向后 (蓝线). 你 (相机) 刚开始是站在(3,3,3)看向 (0,0,0). 





3. 可能会出现的问题:
* 我发现在虚拟机上运行时pangolin可能会有一些问题, 这些问题的解决方案在issue列表里面: https://github.com/stevenlovegrove/Pangolin/issues/74 .
如果你恰巧遇到这个问题的话，你需要注释掉paulinus提到的那两行, 然后重新编译重新安装Pangolin。 

* 如果你依旧有问题, 请联系: gaoxiang12@mails.tsinghua.edu.cn

当然联系我也可以windmillyucong@163.com，我可以陪你聊聊天，但是写代码什么的，就算了吧，我什么都不懂










----------------------------------------------------------------------------------
                                    原文 by 高翔
----------------------------------------------------------------------------------


1. How to compile this program:

* use pangolin: slambook/3rdpart/Pangolin or download it from github: https://github.com/stevenlovegrove/Pangolin

* install dependency for pangolin (mainly the OpenGL): 
sudo apt-get install libglew-dev

* compile and install pangolin
cd [path-to-pangolin]
mkdir build
cd build
cmake ..
make 
sudo make install 

* compile this program:
mkdir build
cd build
cmake ..
make 

* run the build/visualizeGeometry



2. How to use this program:

* The UI in the left panel displays different representations of T_w_c ( camera to world ). It shows the rotation matrix, tranlsation vector, euler angles (in roll-pitch-yaw order) and the quaternion.
Drag your left mouse button to move the camera, right button to rotate it around the box, center button to rotate the camera itself, and press both left and right button to roll the view. 
Note that in this program the original X axis is right (red line), Y is up (green line) and Z in back axis (blue line). You (camera) are looking at (0,0,0) standing on (3,3,3) at first. 




3. Problems may happen:
* I found that in virtual machines there may be an error in pangolin, which was solved in its issue: https://github.com/stevenlovegrove/Pangolin/issues/74 . You need to comment the two lines mentioned by paulinus, and then recompile and reinstall Pangolin, if you happen to find this problem. 

* If you still have problems using this program, please contact: gaoxiang12@mails.tsinghua.edu.cn
