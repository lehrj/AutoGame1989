#pragma once
#include "Camera.h"
#include "pch.h"
#include "Utility.h"


struct Car
{
    int numEqns;
    double s;
    double q[6];
    /*
    m_car.q[0] = 0.0;   //  vx 
    m_car.q[1] = 0.0;   //  x  
    m_car.q[2] = 0.0;   //  vy 
    m_car.q[3] = 0.0;   //  y  
    m_car.q[4] = 0.0;   //  vz 
    m_car.q[5] = 0.0;   //  z  
    */
    double mass;
    double area;
    double density;
    double Cd;

    double muR;
    double omegaE;
    double redline;
    double finalDriveRatio;
    double wheelRadius;
    int gearNumber;     //  gear the car is in
    int numberOfGears;  //  total number of gears
    //char* mode;
    double gearRatio[7];  //  gear ratios
    std::vector<double> testRation[6];

    //////////////////////
    double inputDeadZone;  // small deadzone to ignore gas and brake peddle input
    double accelerationInput;
    double brakeInput;
    double maxAccelerationRate;
    double maxBrakeRate;
    double steeringAngle;
    double steeringAngleMax;
    DirectX::SimpleMath::Vector3 position;         // world position
    DirectX::SimpleMath::Vector3 heading;          // direction the vehicle is facing
    double speed;        // speed vehicle is traveling
    DirectX::SimpleMath::Vector3 velocity;     // direction the vehicle is traveling as it could be sliding or fishtailing
};

struct CarModel
{
    std::unique_ptr<DirectX::GeometricPrimitive>    body;
    std::unique_ptr<DirectX::GeometricPrimitive>    bodyTop;
    std::unique_ptr<DirectX::GeometricPrimitive>    frontAxel;
    std::unique_ptr<DirectX::GeometricPrimitive>    rearAxel;

    DirectX::SimpleMath::Matrix bodyMatrix;
    DirectX::SimpleMath::Matrix bodyTopMatrix;
    DirectX::SimpleMath::Matrix frontAxelMatrix;
    DirectX::SimpleMath::Matrix rearAxelMatrix;

    DirectX::SimpleMath::Matrix localBodyMatrix;
    DirectX::SimpleMath::Matrix localBodyTopMatrix;
    DirectX::SimpleMath::Matrix localFrontAxelMatrix;
    DirectX::SimpleMath::Matrix localRearAxelMatrix;

    DirectX::SimpleMath::Matrix zeroRearAxelMatrix;
};

class Vehicle
{
public:
    Vehicle();
    ~Vehicle();

    void DrawModel(DirectX::SimpleMath::Matrix aWorld, DirectX::SimpleMath::Matrix aView, DirectX::SimpleMath::Matrix aProj, const double aTimer);
    void GearDown();
    void GearUp();

    DirectX::SimpleMath::Vector3 GetDebugPoint() { return  m_debugPoint; };
    DirectX::SimpleMath::Vector3 GetHeading() { return m_car.heading; };
    DirectX::SimpleMath::Vector3 GetPos() { return m_car.position; };
    float GetSpeed() { return m_car.speed; };
    DirectX::SimpleMath::Vector3 GetVelocity() { return m_car.velocity; };

    void InitializeVehicle(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext);

    //void LinkCamera(Camera const* aCamera);
    void LinkCamera(Camera* aCamera);

    void ResetVehicle();
    void UpdateModel(const double aTimer);
    void UpdateVehicle(const double aTimer, const double aTimeDelta);
    void UpdateVehicle2(const double aTimer, const double aTimeDelta);
    
private:
    void DebugTestMove(const double aTimer, const double aTimeDelta);

    void InitializeModel(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext);
    DirectX::SimpleMath::Vector3 GetVehicleDirection();
    double GetWheelRotationRadians(const double aTimeDelta);
    void RightHandSide(struct Car* car, double* q, double* deltaQ, double ds, double qScale, double* dq);
    void RungeKutta4(struct Car* car, double ds);
    void UpdateVehicleCamera();
    

    Car                             m_car;
    CarModel                        m_carModel;

    DirectX::SimpleMath::Vector3    m_pos;          // world position
    DirectX::SimpleMath::Vector3    m_heading;   // direction the vehicle is facing
    double                          m_speed;        // speed vehicle is traveling
    DirectX::SimpleMath::Vector3    m_velocity;     // direction the vehicle is traveling

    DirectX::SimpleMath::Vector4    m_defaultForward = DirectX::XMVectorSet(1.0, 0.0, 0.0, 0.0);
    DirectX::SimpleMath::Vector4    m_forward = DirectX::XMVectorSet(1.0, 0.0, 0.0, 0.0);
    DirectX::SimpleMath::Vector4    m_defaultRight = DirectX::XMVectorSet(0.0, 0.0, 1.0, 0.0);
    DirectX::SimpleMath::Vector4    m_right = DirectX::XMVectorSet(0.0, 0.0, 1.0, 0.0);

    double                          m_moveBackForward = 0.0;
    double                          m_moveLeftRight = 0.0;
    double                          m_moveUpDown = 0.0;

    DirectX::SimpleMath::Matrix     m_testBody;
    DirectX::SimpleMath::Matrix     m_testBodyTop;
    DirectX::SimpleMath::Matrix     m_testFrontAxle;
    DirectX::SimpleMath::Matrix     m_testRearAxle;

    //Camera const* m_vehicleCamera;
    Camera* m_vehicleCamera;

    DirectX::SimpleMath::Vector3 m_debugPoint = DirectX::SimpleMath::Vector3::Zero;

    double m_testRotation = 0.0;
};

