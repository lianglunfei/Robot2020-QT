#ifndef KINEMETICS_H
#define KINEMETICS_H

#include <iostream>
#include <cmath>

#define PI 3.1415926

#include <string>
#include <iostream>
#include <cmath>
#include "Eigen/Core"
#include "Eigen/Dense"


class link
{
public:
    // link();
    // link(double a, double alpha, double d, double theta);
    void setVal(double a, double alpha, double d, double theta){
        DH_a=a;
        DH_alpha=alpha;
        DH_d=d;
        DH_theta=theta;
    }
    Eigen::Matrix4d T(double q){
        double cTheta=cos(DH_theta+q);
        double sTheta=sin(DH_theta+q);
        double cAlpha=cos(DH_alpha);
        double sAlpha=sin(DH_alpha);
        Eigen::Matrix4d T;
        T << cTheta,	-sTheta*cAlpha, sTheta* sAlpha,	DH_a*cTheta,
             sTheta,	cTheta*cAlpha,	-cTheta*sAlpha,	DH_a*sTheta,
             0,			sAlpha,			cAlpha,			DH_d,
             0,			0,				0,				1;
        return T;
    }
    double DH_a;
    double DH_alpha;
    double DH_d;
    double DH_theta;
};

class arm
{
public:
    arm(Eigen::Matrix<double, 6, 1> a,Eigen::Matrix<double, 6, 1> alpha,Eigen::Matrix<double, 6, 1> d,Eigen::Matrix<double, 6, 1> theta){
        for (int i = 0; i < 6; ++i)
        {
            links[i].setVal(a(i), alpha(i), d(i), theta(i));
        }
    }

    Eigen::Matrix4d fkine(Eigen::Matrix<double, 6, 1> q){
        Eigen::Matrix4d T=links[0].T(q(0));;
        for (int i = 1; i < 6; ++i)
        {
            T*=links[i].T(q(i));
        }
        return T;
    }

    Eigen::Matrix<double, 3, 6> fkine2(Eigen::Matrix<double, 6, 1> q){
        Eigen::Matrix4d T=links[0].T(q(0));
        Eigen::Matrix<double, 3, 6> rtn = Eigen::Matrix<double, 3, 6>::Zero();
        for (int i = 1; i < 6; ++i)
        {
            T*=links[i].T(q(i));
            rtn.col(i) = T.col(3).head(3);
        }
        return rtn;
    }

    Eigen::Matrix<double,8,6> ikine(Eigen::Matrix4d TT){
        Eigen::Matrix<double, 5, 5> T;
        T.block<4, 4>(1, 1) = TT;
        Eigen::Matrix<double,9,7> theta;
        double m=links[5].DH_d*T(2,3)-T(2,4);
        double n=links[5].DH_d*T(1,3)-T(1,4);
        theta(1,1)=theta(2,1)=theta(3,1)=theta(4,1)=
            atan2(m,n)-atan2(links[3].DH_d,sqrt(m*m+n*n-links[3].DH_d*links[3].DH_d));
        theta(5,1)=theta(6,1)=theta(7,1)=theta(8,1)=
            atan2(m,n)-atan2(links[3].DH_d,-sqrt(m*m+n*n-links[3].DH_d*links[3].DH_d));

        theta(1,5)=theta(2,5)=acos(T(1,3)*sin(theta(1,1))-T(2,3)*cos(theta(1,1)));
        theta(3,5)=theta(4,5)=-acos(T(1,3)*sin(theta(1,1))-T(2,3)*cos(theta(1,1)));
        theta(5,5)=theta(6,5)=acos(T(1,3)*sin(theta(5,1))-T(2,3)*cos(theta(5,1)));
        theta(7,5)=theta(8,5)=-acos(T(1,3)*sin(theta(5,1))-T(2,3)*cos(theta(5,1)));

        m=T(1,1)*sin(theta(1,1))-T(2,1)*cos(theta(1,1));
        n=T(1,2)*sin(theta(1,1))-T(2,2)*cos(theta(1,1));
        theta(1, 6) = theta(2, 6) = atan2(m, n) - atan2(sin(theta(1, 5)), 0);
        theta(3, 6) = theta(4, 6) = atan2(m, n) - atan2(sin(theta(3, 5)), 0);
        m=T(1,1)*sin(theta(5,1))-T(2,1)*cos(theta(5,1));
        n=T(1,2)*sin(theta(5,1))-T(2,2)*cos(theta(5,1));
        theta(5, 6) = theta(6, 6) = atan2(m, n) - atan2(sin(theta(5, 5)), 0);
        theta(7, 6) = theta(8, 6) = atan2(m, n) - atan2(sin(theta(7, 5)), 0);
        for (int i = 0; i < 8; ++i)
        {
            if (theta(i+1,6)>PI)
            {
                theta(i+1,6)-=2*PI;
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            int k=i*2+1;
            m=links[4].DH_d*(sin(theta(k,6))*(T(1,1)*cos(theta(k,1))+T(2,1)*sin(theta(k,1)))+
                                   cos(theta(k,6))*(T(1,2)*cos(theta(k,1))+T(2,2)*sin(theta(k,1))))-
                     links[5].DH_d*(T(1,3)*cos(theta(k,1))+T(2,3)*sin(theta(k,1)))+
                     T(1,4)*cos(theta(k,1))+T(2,4)*sin(theta(k,1));
            n=T(3,4)-links[0].DH_d-T(3,3)*links[5].DH_d+
                     links[4].DH_d*(T(3,2)*cos(theta(k,6))+T(3,1)*sin(theta(k,6)));
            theta(k,3)=acos((m*m+n*n-links[1].DH_a*links[1].DH_a-links[2].DH_a*links[2].DH_a)/(2*links[1].DH_a*links[2].DH_a));
            theta(k+1,3)=-theta(k,3);
            for (int i = 0; i < 2; ++i)
            {
                double mm=((links[2].DH_a*cos(theta(k+i,3))+links[1].DH_a)*n-
                            links[2].DH_a*sin(theta(k+i,3))*m)/
                          (links[1].DH_a*links[1].DH_a+links[2].DH_a*links[2].DH_a+
                            2*links[1].DH_a*links[2].DH_a*cos(theta(k+i,3)));
                double nn=(m+links[2].DH_a*sin(theta(k+i,3))*mm)/
                          (links[2].DH_a*cos(theta(k+i,3))+links[1].DH_a);
                theta(k+i,2)=atan2(mm,nn);
            }
        }

        for (int i = 1; i <= 8; ++i)
        {
            m=-sin(theta(i,6))*(T(1,1)*cos(theta(i,1))+T(2,1)*sin(theta(i,1)))
                     -cos(theta(i,6))*(T(1,2)*cos(theta(i,1))+T(2,2)*sin(theta(i,1)));
            n=T(3,2)*cos(theta(i,6))+T(3,1)*sin(theta(i,6));
            theta(i,4)=atan2(m,n)-theta(i,2)-theta(i,3);
        }
        return theta.block<8, 6>(1, 1);
    }

    int getAllPos(double in[6], double* out){
        Eigen::Matrix<double, 6, 1> q(in);
        Eigen::Matrix<double, 3, 6> T2 = fkine2(q);
        for(int x=0;x<3;x++){
            for(int y=0;y<6;y++){
                out[x+y] = T2(x,y);
            }
        }
        return 0;
    }

private:
    link links[6];
};







#endif // KINEMETICS_H
