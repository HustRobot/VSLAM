/*=====================================================================================
 *                Copyleft! 2017 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                Copyleft! 2017 William Yu
 *       版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : .cpp
 * Description             :  lk光流 (编译环境：OpenCV3.3.1 + Ubuntu16.04)
 * Reference               : https://docs.opencv.org/3.3.1/df/d74/classcv_1_1FastFeatureDetector.html
 *                           https://docs.opencv.org/3.3.1/dc/d6b/group__video__track.html#ga473e4b886d0bcc6b65831eb88ed93323
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST
 * Modification History	   : ver1.0, 2017.05.10, William Yu
                             ver1.1, 2017.05.12, William Yu, add notes
=====================================================================================*/





# 1.To build the code : 
g++ lkdemo.cpp -o lkdemo `pkg-config  --cflags --libs opencv`




# 2.Usage：
./lkdemo