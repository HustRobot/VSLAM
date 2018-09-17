/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲 学习记录
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.26, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/



0.准备Dataset

将/tool文件夹脚本associate.py复制到/data文件夹，运行
python associate.py rgb.txt depth.txt > associate.txt

修改/config/default.yaml中的数据集路径




1.Compile this program:

* cmake编译
mkdir build
cd build
cmake ..
make




2.Usage:

bin/run_vo <path_to_yaml> 
eg:
bin/run_vo config/default.yaml 
