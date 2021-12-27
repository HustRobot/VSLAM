/*=================================================================================
 *                      Copyleft! 2019 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2019 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : theme.cpp
 * Description             : 
 * Reference               : https://color.adobe.com/explore/?filter=most-popular&time=month
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2019.03.31, William Yu
                             
=================================================================================*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include "theme.h"

int main()
{  
	Theme theme=getThemeInfo(Chinese);
	std::cout << theme.title << std::endl;
	theme.title = "newTitle";
	std::cout << theme.title << std::endl;
    return 0;
}