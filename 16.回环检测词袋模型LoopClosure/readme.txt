/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲 学习记录
 *                           回环检测（小规模字典创建 + 大规模字典创建 + 相似度计算）
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.05.06, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/




# 0.Requirements
 - DBoW3

## Install DBoW3
可以在Github下载，https://github.com/rmsalinas/DBoW3.git
或者使用/3rdparty文件夹下源码
建议将源码放在~/src文件夹下编译安装，install之后源码文件夹不建议删除

* 获取源码，Github下载，或者使用/3rdparty文件夹下源码
cd ~/src
git clone https://github.com/rmsalinas/DBoW3.git

* 编译安装
cd DBoW3
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

* 小批量训练，输出字典vocabulary.yml.gz
./feature_training

* 训练字典，输出字典vocab_larger.yml.gz
./gen_vocab <path_to_data>
eg: ./gen_vocab ../../data/rgbd_dataset_freiburg1_desk
我多下载了好几个数据集在data文件夹下面，都试一试，挺好玩der~

* 计算相似性评分
./loop_closure