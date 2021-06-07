#pragma once
//#include "Camera.h"
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
    double muR;  // rolling friction
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
    double brakeInput;
    double throttleInput;    
    double brakeDecayRate;
    double throttleDecayRate;
    const double maxThrottleInput = 1.0;
    const double maxBrakeInput = 1.0;
    double maxThrottleRate;
    double maxBrakeRate;
    bool isThrottlePressed;
    bool isBrakePressed;
    bool isTurningPressed;

    double steeringAngle;
    double steeringAngleDecay;
    double steeringAngleMax;
    double steeringSpeed; 

    double carRotation;
    DirectX::SimpleMath::Vector3 heading;          // direction the vehicle is facing
    double speed;        // speed vehicle is traveling
    double wheelBase;
    bool isAccelerating;
    bool isBraking;

    // test values for wheel slip
    double testRearCylinderMass;
    double testTorque;
    double testRearAnglularVelocity;
    double testRearAngularVelocityAngle;
};

struct CarModel
{
    std::unique_ptr<DirectX::GeometricPrimitive>    body;
    DirectX::SimpleMath::Matrix bodyMatrix;
    DirectX::SimpleMath::Matrix localBodyMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    bodyTop;
    DirectX::SimpleMath::Matrix localBodyTopMatrix;
    DirectX::SimpleMath::Matrix bodyTopMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    frontAxel;
    DirectX::SimpleMath::Matrix frontAxelMatrix;
    DirectX::SimpleMath::Matrix frontAxelRotation;
    DirectX::SimpleMath::Matrix frontAxelTranslation;

    std::unique_ptr<DirectX::GeometricPrimitive>    frontTire;
    DirectX::SimpleMath::Matrix frontTireMatrix;
    DirectX::SimpleMath::Matrix frontTireRotation;
    DirectX::SimpleMath::Matrix frontTireTranslation;

    std::unique_ptr<DirectX::GeometricPrimitive>    rearAxel;
    DirectX::SimpleMath::Matrix rearAxelMatrix;
    DirectX::SimpleMath::Matrix rearAxelRotation;
    DirectX::SimpleMath::Matrix rearAxelTranslation;

    std::unique_ptr<DirectX::GeometricPrimitive>    rearTire;
    DirectX::SimpleMath::Matrix rearTireMatrix;
    DirectX::SimpleMath::Matrix rearTireRotation;
    DirectX::SimpleMath::Matrix rearTireTranslation;

    std::unique_ptr<DirectX::GeometricPrimitive>    rearSpoiler;
    DirectX::SimpleMath::Matrix rearSpoilerMatrix;
    DirectX::SimpleMath::Matrix locarearSpoilerMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    windShield;
    DirectX::SimpleMath::Matrix windShieldMatrix;    
    DirectX::SimpleMath::Matrix localWindShieldMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    windShieldWindow;
    DirectX::SimpleMath::Matrix windShieldWindowMatrix;
    DirectX::SimpleMath::Matrix localWindShieldWindowMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    frontSideWindows;
    DirectX::SimpleMath::Matrix frontSideWindowsMatrix;
    DirectX::SimpleMath::Matrix localfrontSideWindowsMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    rearSideWindows;
    DirectX::SimpleMath::Matrix rearSideWindowsMatrix;
    DirectX::SimpleMath::Matrix localrearSideWindowsMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    backWindow;
    DirectX::SimpleMath::Matrix backWindowMatrix;
    DirectX::SimpleMath::Matrix localBackWindowMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    hood;
    DirectX::SimpleMath::Matrix hoodMatrix;
    DirectX::SimpleMath::Matrix localHoodMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    triangleFrontWindow;
    DirectX::SimpleMath::Matrix triangleFrontWindowMatrix;
    DirectX::SimpleMath::Matrix localtriangleFrontWindowMatrix;
};

class Vehicle
{
public:
    Vehicle();

    std::vector<std::pair<std::string, double>> DebutGetUI() { return m_debugUI; };

    void DrawModel(DirectX::SimpleMath::Matrix aWorld, DirectX::SimpleMath::Matrix aView, DirectX::SimpleMath::Matrix aProj, const double aTimer);
    void GearDown();
    void GearUp();

    double GetDebugValue() { return m_car.carRotation; };
    DirectX::SimpleMath::Vector3 GetDebugPoint() { return  m_debugPoint; };
    int GetGear() { return m_car.gearNumber; };
    DirectX::SimpleMath::Vector3 GetHeading() { return m_car.heading; };
    DirectX::SimpleMath::Vector3 GetPos() const { return m_car.q.position; };
    double GetRPM() { return m_car.omegaE; };
    double GetRotation() const { return m_car.carRotation; };
    float GetSpeed() { return m_car.speed; };
    double GetSteering() const { return m_car.steeringAngle; };
    double GetTime() { return m_car.time; };
    
    DirectX::SimpleMath::Vector3 GetVelocity() { return m_car.q.velocity; };

    void InitializeVehicle(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext);

    void PressBrake(const double aBrakeInput);
    void PressThrottle(const double aThrottleInput);
    void ThrottleBrakeDecay(const double aTimeDelta);
    void SteeringInputDecay(const double aTimeDelta);

    void ResetVehicle();
    void ToggleGas();
    void ToggleBrake();
    void Turn(double aTurnInput);
    void UpdateVehicle(const double aTimer, const double aTimeDelta);

private:
    void DebugClearUI() { m_debugUI.clear(); };
    void DebugPushUILine(std::string aString, double aVal);
    
    void DebugTestMove(const double aTimer, const double aTimeDelta);

    void InitializeModel(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext);
    double GetCarRotation();
    double GetTurnRadius();
    double GetYawRate(double aTimeDelta);
    DirectX::SimpleMath::Vector3 GetVehicleDirection();
    double GetWheelRotationRadians(const double aTimeDelta);
    double GetWheelRotationRadiansRear(const double aTimeDelta);

    void RightHandSide(struct Car* aCar, Motion* aQ, Motion* aDeltaQ, double aTimeDelta, double aQScale, Motion* aDQ);
    void RungeKutta4(struct Car* aCar, double aTimeDelta);
    
    void TestGetForceLateral();

    void UpdateModel(const double aTimer);
    
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

    DirectX::SimpleMath::Vector3 m_debugPoint = DirectX::SimpleMath::Vector3::Zero;

    double m_testRotation = 0.0;
    double m_testRotationRear = 0.0;

    double m_testMax = 0.0;
    double m_testMin = 1.0;

    std::vector<std::pair<std::string, double>> m_debugUI;

    double m_debugWheelDistance = 0.0;
    double m_debugVehicleDistanceTraveled = 0.0;
};

