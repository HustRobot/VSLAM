cmake_minimum_required( VERSION 2.8 )
project( useSophus )

set(Sophus_DIR /home/will/src/Sophus/build)
# 为使用 sophus，您需要使用find_package命令找到它
find_package( Sophus REQUIRED )
include_directories( ${Sophus_INCLUDE_DIRS} )

add_executable( useSophus useSophus.cpp )
target_link_libraries( useSophus ${Sophus_LIBRARIES} )

