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
    double airResistance;
    //double airDensity;
    double totalResistance;
    double omegaE;
    double redline;
    double finalDriveRatio;
    double wheelRadius;
    double wheelWidth;
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

    DirectX::SimpleMath::Vector3 testAcceleration = DirectX::SimpleMath::Vector3::Zero;
    double testAccel = 0.0;
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

    std::unique_ptr<DirectX::GeometricPrimitive>    rockerSkirt;
    DirectX::SimpleMath::Matrix localRockerSkirtMatrix;
    DirectX::SimpleMath::Matrix rockerSkirtMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    airDam;
    DirectX::SimpleMath::Matrix localAirDamMatrix;
    DirectX::SimpleMath::Matrix airDamMatrix;

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

    std::unique_ptr<DirectX::GeometricPrimitive>    axel;

    // testing independant wheels
    std::unique_ptr<DirectX::GeometricPrimitive>    tire;
    DirectX::SimpleMath::Matrix wheelFrontLeftMatrix;
    DirectX::SimpleMath::Matrix wheelFrontLeftTranslation;
    DirectX::SimpleMath::Matrix wheelFrontRightMatrix;
    DirectX::SimpleMath::Matrix wheelFrontRightTranslation;
    DirectX::SimpleMath::Matrix wheelRearLeftMatrix;
    DirectX::SimpleMath::Matrix wheelRearLeftTranslation;
    DirectX::SimpleMath::Matrix wheelRearRightMatrix;
    DirectX::SimpleMath::Matrix wheelRearRightTranslation;

    std::unique_ptr<DirectX::GeometricPrimitive>    hubCap;

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

    std::unique_ptr<DirectX::GeometricPrimitive>    bumperFront;
    DirectX::SimpleMath::Matrix bumperFrontMatrix;
    DirectX::SimpleMath::Matrix localBumperFrontMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    bumperBack;
    DirectX::SimpleMath::Matrix bumperBackMatrix;
    DirectX::SimpleMath::Matrix localBumperBackMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    grill;
    DirectX::SimpleMath::Matrix grillMatrix;
    DirectX::SimpleMath::Matrix localGrillMatrix;
    std::unique_ptr<DirectX::GeometricPrimitive> grillSlat;
    DirectX::SimpleMath::Matrix grillSlatMatrix;
    DirectX::SimpleMath::Matrix localGrillSlatMatrix;
    std::unique_ptr<DirectX::GeometricPrimitive> grillLogo;
    DirectX::SimpleMath::Matrix grillLogoMatrix;
    DirectX::SimpleMath::Matrix localGrillLogoMatrix;
    std::unique_ptr<DirectX::GeometricPrimitive>    grillOutline;

    std::unique_ptr<DirectX::GeometricPrimitive>    headLight;
    DirectX::SimpleMath::Matrix headLightLeftMatrix;
    DirectX::SimpleMath::Matrix localHeadLightLeftMatrix;
    DirectX::SimpleMath::Matrix headLightRightMatrix;
    DirectX::SimpleMath::Matrix localHeadLightRightMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive> blinkerLight;
    DirectX::SimpleMath::Matrix blinkerLightLowerLeftMatrix;
    DirectX::SimpleMath::Matrix localBlinkerLightLowerLeftMatrix;
    DirectX::SimpleMath::Matrix blinkerLightLowerRightMatrix;
    DirectX::SimpleMath::Matrix localBlinkerLightLowerRightMatrix;
    DirectX::SimpleMath::Matrix blinkerLightUpperLeftMatrix;
    DirectX::SimpleMath::Matrix localBlinkerLightUpperLeftMatrix;
    DirectX::SimpleMath::Matrix blinkerLightUpperRightMatrix;
    DirectX::SimpleMath::Matrix localBlinkerLightUpperRightMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    tailLight;
    DirectX::SimpleMath::Matrix tailLightLeftMatrix;
    DirectX::SimpleMath::Matrix localTailLightLeftMatrix;
    DirectX::SimpleMath::Matrix tailLightRightMatrix;
    DirectX::SimpleMath::Matrix localTailLightRightMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    reverseLight;
    DirectX::SimpleMath::Matrix reverseLightLeftMatrix;
    DirectX::SimpleMath::Matrix localReverseLightLeftMatrix;
    DirectX::SimpleMath::Matrix reverseLightRightMatrix;
    DirectX::SimpleMath::Matrix localReverseLightRightMatrix;

    DirectX::SimpleMath::Matrix tailBlinkerLightLeftMatrix;
    DirectX::SimpleMath::Matrix localTailBlinkerLightLeftMatrix;
    DirectX::SimpleMath::Matrix tailBlinkerLightRightMatrix;
    DirectX::SimpleMath::Matrix localTailBlinkerLightRightMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    thirdBrakeLight;
    DirectX::SimpleMath::Matrix thirdBrakeLightMatrix;
    DirectX::SimpleMath::Matrix localThirdBrakeLightMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive>    licensePlateRear;
    DirectX::SimpleMath::Matrix licensePlateRearMatrix;
    DirectX::SimpleMath::Matrix localLicensePlateRearMatrix;
    std::unique_ptr<DirectX::GeometricPrimitive>    licensePlateRearOutline;

    std::unique_ptr<DirectX::GeometricPrimitive>    pinstripe;
    DirectX::SimpleMath::Matrix pinstripeMatrix;
    DirectX::SimpleMath::Matrix localPinstripeMatrix;

    std::unique_ptr<DirectX::GeometricPrimitive> wheelSpoke;
    DirectX::SimpleMath::Matrix wheelSpokeFront1;
    DirectX::SimpleMath::Matrix localWheelSpokeFront1;
    DirectX::SimpleMath::Matrix spokeTranslation;
    DirectX::SimpleMath::Matrix spokeRotation;

    DirectX::SimpleMath::Matrix wheelSpokeFront2 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFront3 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFront4 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFront5 = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Matrix wheelSpokeBack1 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeBack2 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeBack3 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeBack4 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeBack5 = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Matrix wheelSpokeFL1 = DirectX::SimpleMath::Matrix::Identity;    
    DirectX::SimpleMath::Matrix wheelSpokeFL2 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFL3 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFL4 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFL5 = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Matrix wheelSpokeFR1 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFR2 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFR3 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFR4 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeFR5 = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Matrix wheelSpokeRearLeft1 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeRearLeft2 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeRearLeft3 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeRearLeft4 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeRearLeft5 = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Matrix wheelSpokeRearRight1 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeRearRight2 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeRearRight3 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeRearRight4 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelSpokeRearRight5 = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Matrix localWheelSpoke1 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localWheelSpoke2 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localWheelSpoke3 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localWheelSpoke4 = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localWheelSpoke5 = DirectX::SimpleMath::Matrix::Identity;

    std::unique_ptr<DirectX::GeometricPrimitive> wheelRim;
    DirectX::SimpleMath::Matrix wheelRimFrontMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix wheelRimBackMatrix = DirectX::SimpleMath::Matrix::Identity;

    std::unique_ptr<DirectX::GeometricPrimitive> tireEdge;
    DirectX::SimpleMath::Matrix tireEdgeMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localtireEdgeMatrix = DirectX::SimpleMath::Matrix::Identity;

    std::unique_ptr<DirectX::GeometricPrimitive> rimEdge;
    DirectX::SimpleMath::Matrix rimEdgeMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localrimEdgeMatrix = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Matrix hubFrontLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localHubFrontLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix hubFrontRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localHubFrontRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix hubBackLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localHubBackLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix hubBackRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localHubBackRightMatrix = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Matrix hubInteriorFrontLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localHubInteriorFrontLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix hubInteriorFrontRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localHubInteriorFrontRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix hubInteriorBackLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localHubInteriorBackLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix hubInteriorBackRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix localHubInteriorBackRightMatrix = DirectX::SimpleMath::Matrix::Identity;

    std::unique_ptr<DirectX::GeometricPrimitive> sideMirror;
    DirectX::SimpleMath::Matrix sideMirrorLeftMatrix;
    DirectX::SimpleMath::Matrix localSideMirrorLeftMatrix;
    DirectX::SimpleMath::Matrix sideMirrorRightMatrix;
    DirectX::SimpleMath::Matrix localSideMirrorRightMatrix;


    std::unique_ptr<DirectX::GeometricPrimitive> fenderFlare;
    DirectX::SimpleMath::Matrix fenderFlareFrontLeftMatrix;
    DirectX::SimpleMath::Matrix localfenderFlareFrontLeftMatrix;
    std::unique_ptr<DirectX::GeometricPrimitive> fenderFlareInterior;
    DirectX::SimpleMath::Matrix fenderFlareFrontLeftInteriorMatrix;
    DirectX::SimpleMath::Matrix localfenderFlareFrontLeftInteriorMatrix;
    
    DirectX::SimpleMath::Matrix fenderFlareFrontRightMatrix;
    DirectX::SimpleMath::Matrix localfenderFlareFrontRightMatrix;
    DirectX::SimpleMath::Matrix fenderFlareFrontRightInteriorMatrix;
    DirectX::SimpleMath::Matrix localfenderFlareFrontRightInteriorMatrix;

    DirectX::SimpleMath::Matrix fenderFlareRearLeftMatrix;
    DirectX::SimpleMath::Matrix localfenderFlareRearLeftMatrix;
    DirectX::SimpleMath::Matrix fenderFlareRearLeftInteriorMatrix;
    DirectX::SimpleMath::Matrix localfenderFlareRearLeftInteriorMatrix;

    DirectX::SimpleMath::Matrix fenderFlareRearRightMatrix;
    DirectX::SimpleMath::Matrix localfenderFlareRearRightMatrix;
    DirectX::SimpleMath::Matrix fenderFlareRearRightInteriorMatrix;
    DirectX::SimpleMath::Matrix localfenderFlareRearRightInteriorMatrix;
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
    DirectX::SimpleMath::Vector3 GetHeading() const { return m_car.heading; };
    DirectX::SimpleMath::Vector3 GetPos() const { return m_car.q.position; };
    double GetCarRotation() const { return m_car.carRotation; };
    DirectX::SimpleMath::Vector3 GetVelocity() const { return m_car.q.velocity; };
    double GetAccel() const { return m_car.testAccel; };
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

    void UpdateResistance();
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

    bool m_testIsBreakLightOn = false;
};

