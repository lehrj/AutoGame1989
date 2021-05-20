#pragma once
#include "Camera.h"
#include "pch.h"
#include "Utility.h"


struct Motion
{
    DirectX::SimpleMath::Vector3 position;
    DirectX::SimpleMath::Vector3 velocity;
};

struct Car
{
    int numEqns;
    //double s;
    double time;
    Motion q;
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
    double gearRatio[7];  //  gear ratios
    double gravity;

    //////////////////////
    double inputDeadZone;  // small deadzone to ignore gas and brake peddle input
    double accelerationInput;
    double brakeInput;
    double maxAccelerationRate;
    double maxBrakeRate;

    double steeringAngle;
    double steeringAngle2;
    double steeringAngleMax;
    double carRotation;
    DirectX::SimpleMath::Vector3 heading;          // direction the vehicle is facing
    double speed;        // speed vehicle is traveling
    double wheelBase;
    bool isAccelerating;
    bool isBraking;
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

    DirectX::SimpleMath::Matrix frontAxelRotation;
    DirectX::SimpleMath::Matrix frontAxelTranslation;

    DirectX::SimpleMath::Matrix rearAxelRotation;
    DirectX::SimpleMath::Matrix rearAxelTranslation;
};

class Vehicle
{
public:
    Vehicle();
    ~Vehicle();

    void DrawModel(DirectX::SimpleMath::Matrix aWorld, DirectX::SimpleMath::Matrix aView, DirectX::SimpleMath::Matrix aProj, const double aTimer);
    void GearDown();
    void GearUp();

    double GetDebugValue() { return m_car.carRotation; };
    DirectX::SimpleMath::Vector3 GetDebugPoint() { return  m_debugPoint; };
    int GetGear() { return m_car.gearNumber; };
    DirectX::SimpleMath::Vector3 GetHeading() { return m_car.heading; };
    DirectX::SimpleMath::Vector3 GetPos() { return m_car.q.position; };
    double GetRPM() { return m_car.omegaE; };
    float GetSpeed() { return m_car.speed; };
    double GetTime() { return m_car.time; };
    
    DirectX::SimpleMath::Vector3 GetVelocity() { return m_car.q.velocity; };

    void InitializeVehicle(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext);

    void LinkCamera(Camera* aCamera);

    void ResetVehicle();
    void ToggleGas();
    void ToggleBrake();
    void Turn(double aTurnInput);
    void UpdateVehicle(const double aTimer, const double aTimeDelta);

private:
    void DebugTestMove(const double aTimer, const double aTimeDelta);

    void InitializeModel(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext);
    double GetCarRotation();
    double GetCarRotation2();
    double GetTurnRadius();
    double GetYawRate(double aTimeDelta);
    DirectX::SimpleMath::Vector3 GetVehicleDirection();
    double GetWheelRotationRadians(const double aTimeDelta);

    void RightHandSide(struct Car* aCar, Motion* aQ, Motion* aDeltaQ, double aTimeDelta, double aQScale, Motion* aDQ);
    void RungeKutta4(struct Car* aCar, double aTimeDelta);
    void UpdateModel(const double aTimer);
    void UpdateVehicleCamera();
    
    Car                             m_car;
    CarModel                        m_carModel;

    DirectX::SimpleMath::Vector3    m_heading;   // direction the vehicle is facing
    double                          m_speed;        // speed vehicle is traveling

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

    double m_testMax = 0.0;
    double m_testMin = 1.0;
};

