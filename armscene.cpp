#include "armscene.h"
#include <QtCore/QDebug>


#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender>

// 各个机械臂的长度
const double armLength[6] = {89.459f, 425.0f, 392.25f, 109.15f, 94.65f, 82.3f};

ArmScene::ArmScene(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity)
{
    QVector3D p0 = QVector3D(0,0,0);
    QVector3D p1 = QVector3D(0,armLength[0],0)/100;
    QVector3D p2 = QVector3D(armLength[1], armLength[0],0)/100;
    QVector3D p3 = QVector3D(armLength[1]+armLength[2], armLength[0], 0)/100;
    QVector3D p4 = QVector3D(armLength[1]+armLength[2], armLength[0], 0-armLength[3])/100;

    QVector3D p5 = QVector3D(armLength[1]+armLength[2], armLength[0]-armLength[4], 0-armLength[3])/100;
    QVector3D p6 = QVector3D(armLength[1]+armLength[2], armLength[0]-armLength[4], 0-armLength[3]-armLength[5])/100;


    initArm(0,p0,p1,QColor(Qt::green));
    initArm(1,p1,p2,QColor(Qt::blue));
    initArm(2,p2,p3,QColor(Qt::yellow));
    initArm(3,p3,p4,QColor(Qt::red));
    initArm(4,p4,p5,QColor(Qt::green));
    initArm(5,p5,p6,QColor(Qt::blue));

    // Plane shape data
    Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
    planeMesh->setWidth(8);
    planeMesh->setHeight(8);
//    planeMesh->

    // Plane mesh transform
    Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform();
    planeTransform->setScale(1.0f);

    planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));
    planeTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));

    Qt3DExtras::QPhongMaterial *planeMaterial = new Qt3DExtras::QPhongMaterial();
    planeMaterial->setDiffuse(QColor(QRgb(0xa693379)));

    // Plane
    m_planeEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_planeEntity->addComponent(planeMesh);
    m_planeEntity->addComponent(planeMaterial);
    m_planeEntity->addComponent(planeTransform);

}

ArmScene::~ArmScene()
{
}

//! [4]
//void ArmScene::enableTorus(bool enabled)
//{
//    m_torusEntity->setEnabled(enabled);
//}
////! [4]

//void ArmScene::enableCone(bool enabled)
//{
//    m_coneEntity->setEnabled(enabled);
//}

void ArmScene::enableCylinder(bool enabled)
{
//    m_armEntity->setEnabled(enabled);
    setArm(0,-1,1,0,1,1,0);
}


void ArmScene::setArm(const int ID, double x0, double y0, double z0, double x1, double y1, double z1)
{

    QVector3D distanceV = QVector3D(x1-x0,y1-y0,z1-z0)/100;
    arm[ID]->setLength(distanceV.length());
    armTransform[ID]->setTranslation(distanceV/2+QVector3D(x0,y0,z0)/100);
    armTransform[ID]->setRotation(QQuaternion::rotationTo(QVector3D(0,1,0),distanceV.normalized()));
}
void ArmScene::setArm(const int ID, double p0[3], double p1[3])
{

    QVector3D distanceV = QVector3D(p1[0]-p0[0],p1[1]-p0[1],p1[2]-p0[2])/100;
    arm[ID]->setLength(distanceV.length());
    armTransform[ID]->setTranslation(distanceV/2+QVector3D(p0[0],p0[1],p0[2])/100);
    armTransform[ID]->setRotation(QQuaternion::rotationTo(QVector3D(0,1,0),distanceV.normalized()));
}

void ArmScene::setArms(double p0[3], double p1[3] , double p2[3], double p3[3], double p4[3], double p5[3]){
    double ze[3] ={0};
    setArm(0, ze, p0);
    setArm(1, p0, p1);
    setArm(2, p1, p2);
    setArm(3, p2, p3);
    setArm(4, p3, p4);
    setArm(5, p4, p5);
}

void ArmScene::initArm(const int ID, const QVector3D &start, const QVector3D &end, const QColor &color)
{

    arm[ID] = new Qt3DExtras::QCylinderMesh();
    arm[ID]->setRadius(0.2f);
    arm[ID]->setLength((end-start).length());
    arm[ID]->setRings(100);
    arm[ID]->setSlices(20);



    // armMesh Transform
    armTransform[ID] = new Qt3DCore::QTransform();
    armTransform[ID]->setScale(1.0f);

    armTransform[ID]->setTranslation((end-start)/2+start);
    armTransform[ID]->setRotation(QQuaternion::rotationTo(QVector3D(0,1,0),(end-start)));
//    QQuaternion::fromDirection()
    Qt3DExtras::QPhongMaterial *armMaterial = new Qt3DExtras::QPhongMaterial();
    armMaterial->setDiffuse(color);

    // arm
    m_armEntity[ID] = new Qt3DCore::QEntity(m_rootEntity);
    m_armEntity[ID]->addComponent(arm[ID]);
    m_armEntity[ID]->addComponent(armMaterial);
    m_armEntity[ID]->addComponent(armTransform[ID]);
}

