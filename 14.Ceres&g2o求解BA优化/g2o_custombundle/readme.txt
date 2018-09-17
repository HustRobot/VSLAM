/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲 学习记录
 *                           g2o后端优化
 * Reference               : 
 * Programmer(s)           : William Yu, widmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.26, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/


# 0.准备数据集
将/tool文件夹下的脚本associate.py复制到/data/数据集文件夹，运行
$ python associate.py rgb.txt depth.txt > associate.txt

然后修改/config/default.yaml中的数据集路径





# 1.To build the code : 
mkdir build
cd ./build
cmake ..
make





# 2.How to run the code :

./g2o_customBundle -input ../data/problem-.....txt

* see more detail settings by :
./g2o_customBundle -help

输出final.ply和initial.ply


3.Others

如果安装了Github版本的g2o，《视觉slam十四讲》中程序在ch10/g2o_custombundle/g2o_bundle.cpp 报错 【error】
与非线性优化时情况相同
