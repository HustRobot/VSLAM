#include "eigenGeometryTransfer.h"

int main(int argc, char **argv)
{
    //------------------------------------------------------------------------------------------------------------------//

    // cout << "Examples:" << endl;
    // //this is euler2Quaternion transform function,please input your euler angle//
    // euler2Quaternion(0,0,0);

    //this is Quaternion2Euler transform function,please input your Quaternion parameter//
    //Quaterniond2Euler(0,0,0,1);

    // //this is Quaternion2RotationMatrix transform function,please input your Quaternion parameter//
    // Quaternion2RotationMatrix(0,0,0,1);

    // //this is rotationMatrix2Euler transform function,please input your RotationMatrix parameter like following//
    // Eigen::Vector3d x_axiz,y_axiz,z_axiz;
    // x_axiz << 1,0,0;
    // y_axiz << 0,1,0;
    // z_axiz << 0,0,1;
    // Eigen::Matrix3d R;
    // R << x_axiz,y_axiz,z_axiz;
    // rotationMatrix2Quaterniond(R);

    // //this is euler2RotationMatrix transform function,please input your euler angle for the function parameter//
    // euler2RotationMatrix(0,0,0);

    // //this is RotationMatrix2euler transform function,please input your euler angle for the function parameter//
    // RotationMatrix2euler(R);

    // cout << "All transform is done!" << endl;

    //------------------------------------------------------------------------------------------------------------------//


    cout << "code/4.拼接点云/3、4/两张图片的真实旋转平移矩阵变换" << endl;
    
    Eigen::Matrix<double,3,1> t_3, t_4, t_true;
    Eigen::Matrix3d R_3, R_4, R_3_inv, R_4_inv, R_true;
    
    t_3 << -0.970912, -0.185889, 0.872353;
    t_4 << -1.41952, -0.279885, 1.43657;
    R_3 = Quaternion2RotationMatrix(-0.00662576, -0.278681, -0.0736078, 0.957536);
    R_4 = Quaternion2RotationMatrix(-0.00926933, -0.222761, -0.0567118, 0.973178);
    

    cout<<"---------------------------------------------"<<endl;

    /*-----------------------------[Note]---------------------------
    * Pw = R3*Pc3 + t3
    * Pw = R4*Pc4 + t4
    * --> Pc3 = R3.inv * Pc4 + R3.inv * (t4-t3)
    --------------------------------------------------------------*/
    R_3_inv = R_3.inverse();
    R_true = R_3_inv*R_4;
    t_true = R_3_inv*(t_4 - t_3);
    cout << "R_true_34 = " << endl << R_true << endl<< endl;
    cout << "t_true_34 = " << endl << t_true << endl<< endl;
    RotationMatrix2euler(R_true);
    
    /*-----------------------------[Note]---------------------------
    * --> Pc4 = R4.inv * Pc3 + R4.inv * (t3-t4)
    --------------------------------------------------------------*/
    R_4_inv = R_4.inverse();
    R_true = R_4_inv*R_3;
    t_true = R_4_inv*(t_3 - t_4);
    cout << "R_true_43 = " << endl << R_true << endl<< endl;
    cout << "t_true_43 = " << endl << t_true << endl<< endl;








    cout<<"----------------------ICP 1-1-----------------------"<<endl;
    Eigen::Vector3d x_axiz,y_axiz,z_axiz;
    x_axiz << 1,0,0;
    y_axiz << 0,1,0;
    z_axiz << 0,0,1;
    Eigen::Matrix3d R;
    R << x_axiz,y_axiz,z_axiz;
    rotationMatrix2Quaterniond(R);

    cout<<"----------------------ICP 1-2-----------------------"<<endl;
    t_3 << -0.228993, 0.00645704, 0.0287837;
    t_4 << -0.50237, -0.0661803, 0.322012;
    R_3 = Quaternion2RotationMatrix(-0.0004327, -0.113131, -0.0326832, 0.993042);
    R_4 = Quaternion2RotationMatrix(-0.00152174, -0.32441, -0.0783827, 0.942662);
    R_4_inv = R_4.inverse();
    R_true = R_4_inv*R_3;
    t_true = R_4_inv*(t_3 - t_4);
    cout << "R_true_12 = " << endl << R_true << endl<< endl;
    cout << "t_true_12 = " << endl << t_true << endl<< endl;
    rotationMatrix2Quaterniond(R_true);//因为变化比较大，匹配点太少，ICP求不出来，所以暂且使用真值

    cout<<"----------------------ICP 2-3-----------------------"<<endl;
    x_axiz << 0.9938829955003455, -0.01838296677362556, 0.1088974645612097;
    y_axiz <<  0.01787285143498423, 0.9998242549722147, 0.005658652740609116;
    z_axiz <<  -0.1089823491985887, -0.0036777305305839, 0.9940368815397654;
    R << x_axiz,y_axiz,z_axiz;
    rotationMatrix2Quaterniond(R_true);   

    cout<<"----------------------ICP 3-4-----------------------"<<endl;
    x_axiz << 0.9975243902348213, -0.03695521416175974, 0.05982811239632072;
    y_axiz <<  0.04015574207059773, 0.9977757556605985, -0.05320768548509934;
    z_axiz <<  -0.0577287386438326, 0.05547840626928725, 0.9967896163044697;
    R << x_axiz,y_axiz,z_axiz;
    rotationMatrix2Quaterniond(R_true);

    cout<<"----------------------ICP 4-5-----------------------"<<endl;
    x_axiz << 0.9985645417791684, -0.03695038106186452, -0.03877531741640938;
    y_axiz << 0.03850479447857155, 0.9984519860931806, 0.04013741731534293;
    z_axiz << 0.03723219982116646, -0.04157283735765521, 0.9984415168103294;
    R << x_axiz,y_axiz,z_axiz;
    rotationMatrix2Quaterniond(R_true);





    cout<<"----------------------对极几何角度精度测试-----------------------"<<endl;
    x_axiz << 0.9975243902348213, -0.03695521416175974, 0.05982811239632072;
    y_axiz << 0.04015574207059773, 0.9977757556605985, -0.05320768548509934;
    z_axiz << -0.0577287386438326, 0.05547840626928725, 0.9967896163044697;
    R << x_axiz,y_axiz,z_axiz;
    RotationMatrix2euler(R);
}