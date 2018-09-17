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
 * 
=================================================================================*/

#ifndef CONFIG_H
#define CONFIG_H

#include "myslam/common_include.h" 

namespace myslam 
{
class Config
{
private:
    static std::shared_ptr<Config> config_; 
    cv::FileStorage file_;
    
    Config () {} // private constructor makes a singleton
public:
    ~Config();  // close the file when deconstructing 
    
    // set a new config file 
    static void setParameterFile( const std::string& filename ); 
    
    // access the parameter values
    template< typename T >
    static T get( const std::string& key )
    {
        return T( Config::config_->file_[key] );
    }
};
}

#endif // CONFIG_H
