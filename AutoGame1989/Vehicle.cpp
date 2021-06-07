#include "pch.h"
#include "Vehicle.h"


Vehicle::Vehicle()
{
    //InitializeVehicle();
}

void Vehicle::DrawModel(DirectX::SimpleMath::Matrix aWorld, DirectX::SimpleMath::Matrix aView, DirectX::SimpleMath::Matrix aProj, const double aTimer)
{
    DirectX::SimpleMath::Matrix world = aWorld;
    DirectX::SimpleMath::Matrix view = aView;
    DirectX::SimpleMath::Matrix proj = aProj;
    
    // testing variations of volvo yellow color
    float x = 252.0 / 256.0;
    float y = 255.0 / 256.0;
    float z = 164.0 / 256.0;
    DirectX::SimpleMath::Vector4 volvoYellow(x, y, z, 1.0);
    DirectX::XMFLOAT4 testColor(0.984375, 0.91015625, 0.01171875, 1.0);  
    DirectX::SimpleMath::Vector4 testV = testColor;
    DirectX::SimpleMath::Vector4 tireColor(0.2, 0.2, 0.2, 1.0);
    DirectX::SimpleMath::Vector4 windowColor(0.09, 0.09, 0.09, 1.0);

    m_carModel.bodyTop->Draw(m_carModel.bodyTopMatrix, view, proj, volvoYellow);
    m_carModel.body->Draw(m_carModel.bodyMatrix, view, proj, volvoYellow);
    m_carModel.frontAxel->Draw(m_carModel.frontAxelMatrix, view, proj);
    m_carModel.rearAxel->Draw(m_carModel.rearAxelMatrix, view, proj);
    m_carModel.frontTire->Draw(m_carModel.frontTireMatrix, view, proj, tireColor);
    m_carModel.rearTire->Draw(m_carModel.rearTireMatrix, view, proj, tireColor);
    m_carModel.windShield->Draw(m_carModel.windShieldMatrix, view, proj, volvoYellow);
    m_carModel.rearSpoiler->Draw(m_carModel.rearSpoilerMatrix, view, proj, volvoYellow);
    m_carModel.hood->Draw(m_carModel.hoodMatrix, view, proj, volvoYellow);
    m_carModel.windShieldWindow->Draw(m_carModel.windShieldWindowMatrix, view, proj, windowColor);
    m_carModel.frontSideWindows->Draw(m_carModel.frontSideWindowsMatrix, view, proj, windowColor);
    m_carModel.rearSideWindows->Draw(m_carModel.rearSideWindowsMatrix, view, proj, windowColor);
    m_carModel.backWindow->Draw(m_carModel.backWindowMatrix, view, proj, windowColor);
    m_carModel.triangleFrontWindow->Draw(m_carModel.triangleFrontWindowMatrix, view, proj, windowColor);
}

void Vehicle::GearDown()
{
    if (m_car.gearNumber > 0 && m_car.gearNumber <= m_car.numberOfGears)
    {
        --m_car.gearNumber;
    }
}

void Vehicle::GearUp()
{
    if (m_car.gearNumber >= 0 && m_car.gearNumber < m_car.numberOfGears)
    {
        ++m_car.gearNumber;
    }
}

double Vehicle::GetCarRotation()
{
    double turnRadius = GetTurnRadius();
    double steeringAngle = m_car.steeringAngle;
    //steeringAngle = 0.0;

    DirectX::SimpleMath::Vector3 testRadVec(0.0, 0.0, -turnRadius);
    DirectX::SimpleMath::Matrix testTurnMat = DirectX::SimpleMath::Matrix::CreateRotationY(steeringAngle);

    testRadVec = DirectX::SimpleMath::Vector3::Transform(testRadVec, testTurnMat);
    DirectX::SimpleMath::Vector3 testRadVecNorm = testRadVec;
    testRadVecNorm.Normalize();

    DirectX::SimpleMath::Vector3 ballVec = testRadVecNorm;
    DirectX::SimpleMath::Vector3 zeroDirection = DirectX::SimpleMath::Vector3::UnitZ;

    double direction = DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(DirectX::XMVector3Normalize(ballVec), DirectX::XMVector3Normalize(zeroDirection)));

    DirectX::SimpleMath::Vector3 a = - DirectX::SimpleMath::Vector3::UnitZ;
    DirectX::SimpleMath::Vector3 b = DirectX::SimpleMath::Vector3::UnitX;
    b = testRadVecNorm;
    //static inline double GetAngle1(DirectX::SimpleMath::Vector3 aRef, DirectX::SimpleMath::Vector3 aPointA) { return acos(aRef.Dot(aPointA)); };
    double testAngle = acos(a.Dot(b));
    double testAngleDegrees = Utility::ToDegrees(testAngle);

    if (testAngle > m_testMax)
    {
        m_testMax = testAngle;
    }
    if (testAngle < m_testMin)
    {
        m_testMin = testAngle;
    }
    if (m_car.time > 15.0)
    {
        int testBreak;

    }

    double carRotation = testAngle + 0.1; // +(Utility::ToRadians(90.0));
    //m_car.carRotation = carRotation;
    //m_car.carRotation = testAngle;
    //m_car.carRotation = steeringAngle;

    //m_car.carRotation = m_car.time * 0.5;
    return carRotation;
}

double Vehicle::GetTurnRadius()
{
    double wheelBase = m_car.wheelBase;
    double sinDelta = sin(m_car.steeringAngle);
    double sinDelta2 = sin((10.0 * Utility::GetPi()) / 180.0);
    double testVal = (10.0 * Utility::GetPi()) / 180.0;
    double testDeg = Utility::ToDegrees(testVal);
    double turnRadius = 1.0;
    double turnRadius2 = 1.0;
    if (sinDelta == 0.0)
    {
        turnRadius = 0.0;
    }
    else
    {
        turnRadius = wheelBase / sinDelta;
        turnRadius2 = wheelBase / sinDelta2;
    }

    return turnRadius;
}

double Vehicle::GetYawRate(double aTimeDelta)
{
    double turnRadius = GetTurnRadius();
    double wheelBase = m_car.wheelBase;
    double velocity = m_car.q.velocity.Length();
    double steeringAngle = m_car.steeringAngle;

    double sinDelta = sin(steeringAngle);

    double omega = (velocity * sinDelta) / wheelBase;
    double omegaT = omega * aTimeDelta;

    double omegaDeg = Utility::ToDegrees(omega);
    double omegaTdeg = Utility::ToDegrees(omegaT);

    
    /// ///////////////////
    // testing wheel slip eqations and verables
    DirectX::SimpleMath::Vector3 testMuK = m_car.mass * ((m_car.q.velocity * m_car.q.velocity) / turnRadius);
    DirectX::SimpleMath::Vector3 testMuK2 = testMuK;
    testMuK = testMuK / (m_car.mass * m_car.gravity);

    double Fn = m_car.mass * -m_car.gravity * cos(0.0);

    if (steeringAngle > 0.1 || steeringAngle < -0.1)
    {
        double aaTestTorque = m_car.testTorque;
        double aaTestAngularVelocity = m_car.testRearAnglularVelocity;
        int testBreak = 0;
        testBreak++;
    }

    return omegaT;
}

DirectX::SimpleMath::Vector3 Vehicle::GetVehicleDirection()
{
    DirectX::SimpleMath::Vector3 frontAxelPos;
    DirectX::SimpleMath::Vector3 tempScale;
    DirectX::SimpleMath::Quaternion tempQuat;
    m_carModel.frontAxelMatrix.Decompose(tempScale, tempQuat, frontAxelPos);
    DirectX::SimpleMath::Vector3 rearAxelPos;
    m_carModel.rearAxelMatrix.Decompose(tempScale, tempQuat, rearAxelPos);
    DirectX::SimpleMath::Vector3 direction = rearAxelPos - frontAxelPos;
    direction.Normalize();
   
    DirectX::SimpleMath::Vector3 testPos;
    m_carModel.bodyTopMatrix.Decompose(tempScale, tempQuat, testPos);
    m_debugPoint = testPos;

    return direction;
}

double Vehicle::GetWheelRotationRadians(const double aTimeDelta)
{
    DirectX::SimpleMath::Vector3 velocity = m_car.q.velocity;
    float distance = DirectX::SimpleMath::Vector3::Distance(velocity, DirectX::SimpleMath::Vector3::Zero);
    float stepDistance;
    if (aTimeDelta != 0.0)
    {
        stepDistance = distance * aTimeDelta;
    }
    else
    {
        stepDistance = 0.0;
    }

    double circumference = 2.0 * Utility::GetPi() * m_car.wheelRadius;
    double turnRatio = stepDistance / circumference;

    double rotations = turnRatio * (2.0 * Utility::GetPi());
    
    // testing wheel rotation and distance
    double circumferance = 2.0 * Utility::GetPi() * m_car.wheelRadius;

    double wheelMove = rotations * circumferance;
    //double wheelMove = turnRatio * circumferance;

    m_debugWheelDistance += wheelMove;

    if (m_debugWheelDistance > 0.5)
    {
        int testBreak = 0;
        testBreak++;
    }
    //return turnRatio;
    return rotations;
}

double Vehicle::GetWheelRotationRadiansRear(const double aTimeDelta)
{
    double rotations = 0.0;
    if (aTimeDelta != 0.0)
    {
        rotations = m_car.testRearAnglularVelocity * aTimeDelta;
    }
    else
    {
        rotations = 0.0;
    }

    return rotations;
}

void Vehicle::InitializeModel(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext)
{
    // porche boxter base dimensions - 4.3942m L x 1.8034m W x 1.27m H, wheel diameter 0.3186m
    const float wheelRadius = m_car.wheelRadius;
    const float wheelDiameter = wheelRadius * 2.0;
    const float axelRadius = wheelRadius * 0.8;
    const float axelDiameter = axelRadius * 2.0;
    const float length = 4.3942;
    const float width = 1.8034;
    const float heightTotal = 1.27;
    const float height = heightTotal * 0.6;
    const float windowPlaneOffset = 0.03;
    const float axelLength = width + 0.6;
    const float tireLength = width + 0.5;
    const float wheelBase = m_car.wheelBase;
    DirectX::SimpleMath::Vector3 carBodySize(length, height - wheelRadius, width);

    m_carModel.body = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), carBodySize);
    m_carModel.frontAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, axelDiameter, 3);
    m_carModel.rearAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, axelDiameter, 3);

    m_carModel.frontTire = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), tireLength, wheelDiameter, 16);
    m_carModel.rearTire = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), tireLength, wheelDiameter, 16);

    m_carModel.bodyMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.frontAxelMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.rearAxelMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.frontTireMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.rearTireMatrix = DirectX::SimpleMath::Matrix::Identity;

    m_carModel.bodyMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, (height + (wheelRadius)), 0.0));

    DirectX::SimpleMath::Matrix axelRotation = DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0));

    m_carModel.frontAxelMatrix *= axelRotation;
    m_carModel.frontAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5, wheelRadius, 0.0));
    m_carModel.rearAxelMatrix *= axelRotation;
    m_carModel.rearAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5, wheelRadius, 0.0));
    
    m_carModel.frontTireMatrix *= axelRotation;
    m_carModel.frontTireMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5, wheelRadius, 0.0));
    m_carModel.rearTireMatrix *= axelRotation;
    m_carModel.rearTireMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5, wheelRadius, 0.0));

    const float topIndent = 0.2;   
    const float topHeight = heightTotal * 0.4;
    const float topLength = length * .6;
    const float roofHeightAlignment = height + wheelRadius + wheelRadius + topHeight;
    const float roofLengthAlignment = length - topLength - topIndent;
    //const float roofLengthAlignment = 1.0;
    DirectX::SimpleMath::Vector3 carBodyTopSize(topLength, topHeight, width - topIndent);
    m_carModel.bodyTop = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), carBodyTopSize);
    m_carModel.bodyTopMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.bodyTopMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3( - roofLengthAlignment, roofHeightAlignment, 0.0));
    
    m_carModel.localBodyMatrix = m_carModel.bodyMatrix;
    m_carModel.localBodyTopMatrix = m_carModel.bodyTopMatrix;

    // hood start ///////////////////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 hoodSize = carBodySize;
    const float hoodVerticalAdjust = 0.05;
    const float hoodHorizontalAdjust = 0.1;
    hoodSize.x -= hoodHorizontalAdjust;
    hoodSize.y += hoodVerticalAdjust;
    hoodSize.z *= 0.6;
    m_carModel.hood = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), hoodSize);
    m_carModel.hoodMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hoodMatrix = m_carModel.bodyMatrix;
    m_carModel.hoodMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(hoodHorizontalAdjust, (hoodVerticalAdjust * 0.5), 0.0));
    m_carModel.localHoodMatrix = m_carModel.hoodMatrix;
    // hood end /////////////////////////////////////////////////////////////////////////////////

    // rear spoiler start /////////////////////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 rearSpoilerSize = carBodyTopSize;
    rearSpoilerSize.x = 0.1;
    rearSpoilerSize.y = 0.07;   
    rearSpoilerSize.z *= 0.8;
    m_carModel.rearSpoiler = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), rearSpoilerSize);

    m_carModel.rearSpoilerMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Vector3 spoilerTranslation(-roofLengthAlignment, roofHeightAlignment, 0.0);
    spoilerTranslation.y = roofHeightAlignment + (carBodyTopSize.y * 0.5);

    spoilerTranslation.y = roofHeightAlignment + carBodyTopSize.y - rearSpoilerSize.y;
    spoilerTranslation.x = - rearSpoilerSize.x - roofLengthAlignment - carBodyTopSize.x;
    m_carModel.rearSpoilerMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(spoilerTranslation);
    m_carModel.locarearSpoilerMatrix = m_carModel.rearSpoilerMatrix;
    // rear spoiler end //////////////////////////////////////////////////////////////////////////////////

    /// windshield start /////////////////////////////////////////////////////////////////////////////////  
    DirectX::SimpleMath::Vector3 windShieldSize(1.0, 0.5, carBodyTopSize.z - 0.0001);
    const float windShieldLengthAlignment = (topLength * 2.0) - roofLengthAlignment;
    m_carModel.windShield = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), windShieldSize);
    m_carModel.windShieldMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Quaternion testWindShieldRotate = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, 0.0, Utility::ToRadians(-99.0));
    m_carModel.windShieldMatrix = DirectX::SimpleMath::Matrix::Transform(m_carModel.windShieldMatrix, testWindShieldRotate);
    DirectX::SimpleMath::Vector3 testWindShieldAllignment = DirectX::SimpleMath::Vector3::Zero;
    testWindShieldAllignment.x = windShieldLengthAlignment;
    testWindShieldAllignment.y = roofHeightAlignment;
    testWindShieldAllignment.x = 3.71535993;
    testWindShieldAllignment.y = 1.90719998;
    testWindShieldAllignment.x = 1.255;
    testWindShieldAllignment.y = 1.711;
    //m_carModel.windShieldMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(testWindShieldAllignment));    
    m_carModel.windShieldMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(testWindShieldAllignment);
    m_carModel.localWindShieldMatrix = m_carModel.windShieldMatrix;   
    /// windshield end /////////////////////////////////////////////////////////////////////////

    /// Windshield Window Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 windShieldWindowSize = windShieldSize;
    const float windowIndent = 0.9;
    windShieldWindowSize.x *= windowIndent;
    windShieldWindowSize.y *= windowIndent;
    windShieldWindowSize.z *= windowIndent;
    m_carModel.windShieldWindow = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), windShieldWindowSize);
    m_carModel.windShieldWindowMatrix = m_carModel.windShieldMatrix;
    m_carModel.windShieldWindowMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(windowPlaneOffset, 0.0, 0.0));
    m_carModel.localWindShieldWindowMatrix = m_carModel.windShieldWindowMatrix;
    /// Windshield Window Stop  ////////////////////////////////////////////////////////////////
    
    /// FrontSide Windows Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 frontSideWindowSize = carBodyTopSize;
    frontSideWindowSize.x *= 0.25;
    frontSideWindowSize.y *= 0.755;
    frontSideWindowSize.z *= 1.0 + windowPlaneOffset;
    m_carModel.frontSideWindows = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), frontSideWindowSize);
    m_carModel.frontSideWindowsMatrix = m_carModel.bodyTopMatrix;
    m_carModel.frontSideWindowsMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(carBodyTopSize.x * 0.37, 0.0, 0.0));
    m_carModel.localfrontSideWindowsMatrix = m_carModel.frontSideWindowsMatrix;
    /// FrontSide Windows End   ////////////////////////////////////////////////////////////////

    /// Rear Side Windows Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 rearSideWindowSize = frontSideWindowSize;
    rearSideWindowSize.x *= 2.5;
    m_carModel.rearSideWindows = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), rearSideWindowSize);
    m_carModel.rearSideWindowsMatrix = m_carModel.frontSideWindowsMatrix;
    m_carModel.rearSideWindowsMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3( - frontSideWindowSize.x * 2.0, 0.0, 0.0));
    m_carModel.localrearSideWindowsMatrix = m_carModel.rearSideWindowsMatrix;
    /// Rear Side Windows End   ////////////////////////////////////////////////////////////////

    /// Back Window Start  ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 backWindowSize = rearSpoilerSize;
    backWindowSize.y = rearSideWindowSize.y;
    backWindowSize.z *= 1.1;
    m_carModel.backWindow = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), backWindowSize);
    m_carModel.backWindowMatrix = m_carModel.rearSpoilerMatrix;
    m_carModel.backWindowMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(rearSpoilerSize.x - windowPlaneOffset, (-backWindowSize.y * 0.5) + (-rearSpoilerSize.y * 0.5), 0.0));
    m_carModel.localBackWindowMatrix = m_carModel.backWindowMatrix;
    /// Back Window End   ////////////////////////////////////////////////////////////////

    /// Triangle Front Window Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3  triangleWindowSize = windShieldSize;
    triangleWindowSize.z *= 1.0 + windowPlaneOffset;
    triangleWindowSize.x -= 0.0001;
    m_carModel.triangleFrontWindow = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), triangleWindowSize);
    m_carModel.triangleFrontWindowMatrix = m_carModel.windShieldMatrix;
    m_carModel.triangleFrontWindowMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-0.014, -0.064, 0.0));
    /// Triangle Front Window End   ////////////////////////////////////////////////////////////////

    m_carModel.frontAxelRotation *= axelRotation;
    m_carModel.frontAxelTranslation *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5, wheelRadius, 0.0));

    m_carModel.rearAxelRotation = axelRotation;
    m_carModel.rearAxelTranslation = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5, wheelRadius, 0.0));

    m_carModel.frontTireRotation *= axelRotation;
    m_carModel.frontTireTranslation *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5, wheelRadius, 0.0));

    m_carModel.rearTireRotation = axelRotation;
    m_carModel.rearTireTranslation = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5, wheelRadius, 0.0));
}

void Vehicle::InitializeVehicle(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext)
{
    // roughly based on porsche boxster
    m_car.mass = 1393.0;
    m_car.area = 1.94;
    m_car.density = 1.2;
    m_car.Cd = 0.31;
    m_car.redline = 7200;
    m_car.finalDriveRatio = 3.44;
    m_car.wheelRadius = 0.3186;

    m_car.numberOfGears = 6;
    m_car.muR = 0.015;             //  coefficient of rolling friction
    m_car.omegaE = 1000.0;         //  engine rpm
    m_car.gearNumber = 1;          //  gear the car is in
    m_car.gearRatio[0] = 0.0;
    m_car.gearRatio[1] = 3.82;
    m_car.gearRatio[2] = 2.20;
    m_car.gearRatio[3] = 1.52;
    m_car.gearRatio[4] = 1.22;
    m_car.gearRatio[5] = 1.02;
    m_car.gearRatio[6] = 0.84;
    m_car.gravity = - 9.81;
    m_car.numEqns = 6;
    m_car.time = 0.0;  
    m_car.q.position = DirectX::SimpleMath::Vector3::Zero;
    m_car.q.velocity = DirectX::SimpleMath::Vector3::Zero;

    m_car.inputDeadZone = 0.05;
    m_car.throttleInput = 0.0;
    m_car.brakeInput = 0.0;
    m_car.maxThrottleRate = 1.0;
    //m_car.maxBrakeRate = 5.0;
    m_car.maxBrakeRate = 15.0;

    m_car.brakeDecayRate = 1.2;
    m_car.throttleDecayRate = 1.2;
    m_car.steeringAngleDecay = -0.2;
    m_car.steeringSpeed = 0.5;

    m_car.carRotation = 0.0;
    m_car.steeringAngle = Utility::ToRadians(0.0);
    m_car.steeringAngleMax = Utility::ToRadians(33.0);
    m_car.heading = DirectX::SimpleMath::Vector3::Zero;
    m_car.speed = 0.0;

    m_car.isThrottlePressed = false;
    m_car.isBrakePressed = false;
    m_car.isTurningPressed = false;
    m_car.isAccelerating = false;
    m_car.isBraking = false;
    m_car.wheelBase = 2.41;

    // test values for wheel slip
    m_car.testRearCylinderMass = 75.0;
    m_car.testTorque = 0.0;
    m_car.testRearAnglularVelocity = 0.0;
    m_car.testRearAngularVelocityAngle = 0.0;


    InitializeModel(aContext);
}

void Vehicle::PressBrake(const double aBrakeInput)
{
    if (aBrakeInput > 0.0)
    {
        m_car.isBrakePressed = true;
    }
    double updatedBrake = aBrakeInput + m_car.brakeInput;
    if (updatedBrake > m_car.maxBrakeInput)
    {
        m_car.brakeInput = m_car.maxBrakeInput;
    }
    else
    {
        m_car.brakeInput += aBrakeInput;
    }
}

void Vehicle::PressThrottle(const double aThrottleInput)
{
    if (aThrottleInput > 0.0)
    {
        m_car.isThrottlePressed = true;
    }
    double updatedThrottle = aThrottleInput + m_car.throttleInput;
    if (updatedThrottle > m_car.maxThrottleInput)
    {
        m_car.throttleInput = m_car.maxThrottleInput;
    }
    else
    {
        m_car.throttleInput += aThrottleInput;
    }
}

void Vehicle::ResetVehicle()
{
    m_car.gearNumber = 1;         

    m_car.steeringAngle = 0.0;
    m_car.q.position = DirectX::SimpleMath::Vector3::Zero;
    m_car.heading = DirectX::SimpleMath::Vector3::Zero;
    m_car.speed = 0.0;
    m_car.q.velocity = DirectX::SimpleMath::Vector3::Zero;
}

//*************************************************************
//  This method loads the right-hand sides for the car ODEs
//*************************************************************
void Vehicle::RightHandSide(struct Car* aCar, Motion* aQ, Motion* aDeltaQ, double aTimeDelta, double aQScale, Motion* aDQ)
{
    //  Compute the intermediate values of the 
    //  dependent variables.
    Motion newQ;

    newQ.position = aQ->velocity + static_cast<float>(aQScale) * aDeltaQ->position;
    newQ.velocity = aQ->velocity + static_cast<float>(aQScale) * aDeltaQ->velocity;

    //  Compute the constants that define the
    //  torque curve line.
    double b;
    double d;
    double omegaE = aCar->omegaE;

    if (omegaE <= 1000.0)
    {
        b = 0.0;
        d = 220.0;
    }
    else if (omegaE < 4600.0)
    {
        b = 0.025;
        d = 195.0;
    }
    else
    {
        b = -0.032;
        d = 457.2;
    }

    //  Declare some convenience variables representing
    //  the intermediate values of velocity.
    double vx = newQ.velocity.x;
    double vy = newQ.velocity.y;
    double vz = newQ.velocity.z;

    //  Compute the velocity magnitude. The 1.0e-8 term
    //  ensures there won't be a divide by zero later on
    //  if all of the velocity components are zero.
    double v = sqrt(vx * vx + vy * vy + vz * vz) + 1.0e-8;

    //  Compute the total drag force.
    double density = aCar->density;
    double Cd = aCar->Cd;
    double area = aCar->area;
    double Fd = 0.5 * density * area * Cd * v * v;
    // drag force without wind
    double Fdx = -Fd * v;
    double Fdy = -Fd * v;
    double Fdz = -Fd * v;

    //  Compute the force of rolling friction. Because
    //  the G constant has a negative sign, the value 
    //  computed here will be negative
    double gravity = aCar->gravity;
    double muR = aCar->muR;
    double mass = aCar->mass;
    double Fr = muR * mass * gravity;

    //  Compute the right-hand sides of the six ODEs
    //  newQ[0] is the intermediate value of velocity.
    //  The acceleration of the car is determined by 
    //  whether the car is accelerating, cruising, or
    //  braking. The braking acceleration is assumed to
    //  be a constant -5.0 m/s^2.   
    // Accelerating
    int gearNumber = aCar->gearNumber;
    double gearRatio = aCar->gearRatio[gearNumber];
    double finalDriveRatio = aCar->finalDriveRatio;
    double wheelRadius = aCar->wheelRadius;
    double pi = acos(-1.0);
    double carHeading = m_car.carRotation;
    DirectX::SimpleMath::Matrix headingRotation = DirectX::SimpleMath::Matrix::CreateRotationY(carHeading);
    DirectX::SimpleMath::Vector3 headingVec = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitX, headingRotation);

    //if (m_car.accelerationInput < m_car.inputDeadZone)
    //if (m_car.isAccelerating == true || m_car.throttleInput > 0.0)
    if (m_car.throttleInput > 0.0)
    {
        double c1 = -Fd / mass;
        double tmp = gearRatio * finalDriveRatio / wheelRadius;
        double c2 = 60.0 * tmp * tmp * b * v / (2.0 * pi * mass);
        double c3 = (tmp * d + Fr) / mass;
        //dq[0] = ds * (c1 + c2 + c3);
        //aDQ->velocity.x = aTimeDelta * (c1 + c2 + c3); // ToDo: update 3D x, y, z motion;
        //aDQ->velocity.x = (aTimeDelta * (c1 + c2 + c3)) * headingVec.x;
        //aDQ->velocity.y = (aTimeDelta * (c1 + c2 + c3)) * headingVec.y;
        //aDQ->velocity.z = (aTimeDelta * (c1 + c2 + c3)) * headingVec.z;

        aDQ->velocity = (aTimeDelta * (c1 + c2 + c3)) * headingVec;
        m_car.testTorque = (c1 + c2 + c3) / aTimeDelta;
    }
    // braking
    //else if (m_car.isBraking == true || m_car.brakeInput > 0.0)
    else if (m_car.brakeInput > 0.0)
    {
        //  Only brake if the velocity is positive.
        //if (newQ[0] > 0.1) 
        if (newQ.velocity.Length() > 0.1)
        {
            //dq[0] = ds * (-m_car.maxBrakeRate); // temp for testing, ToDO: modify braking rate by brake input 
            //aDQ->velocity.x = aTimeDelta * (-aCar->maxBrakeRate); // temp for testing, ToDO: modify braking rate by brake input 
            aDQ->velocity = (aTimeDelta * (-aCar->maxBrakeRate)) * headingVec;
        }
        else
        {
            aDQ->velocity = DirectX::SimpleMath::Vector3::Zero;
            //aDQ->velocity.x = 0.0;
            //aDQ->velocity.y = 0.0;
            //aDQ->velocity.z = 0.0;
        }
    }
    else
    {
        aDQ->velocity = DirectX::SimpleMath::Vector3::Zero;
        //aDQ->velocity.x = 0.0;
        //aDQ->velocity.y = 0.0;
        //aDQ->velocity.z = 0.0;
    }

    
    //if (m_car.brakeInput == 0.0 && m_car.maxThrottleInput == 0.0)
    if (m_car.isBrakePressed == false && m_car.isThrottlePressed == false && aQ->velocity.Length() > 0.1)
    //if (aQ->velocity.Length() > 0.1)
    {
        //newQ.velocity *= 0.5;
        //aDQ->velocity = (aTimeDelta * (- m_car.muR)) * headingVec;
        double rollingResistance = Fr / mass;
        aDQ->velocity = (aTimeDelta * (rollingResistance)) * headingVec;
        //aDQ->velocity *=  0.5;
    }
    

    //  Compute right-hand side values.
    aDQ->position.x = aTimeDelta * newQ.velocity.x;
    aDQ->position.y = aTimeDelta * newQ.velocity.y;
    aDQ->position.z = aTimeDelta * newQ.velocity.z;

    DirectX::SimpleMath::Matrix testTurn = DirectX::SimpleMath::Matrix::CreateRotationY(m_car.carRotation);

    return;
}

//************************************************************
//  This method solves for the car motion using a
//  4th-order Runge-Kutta solver
//************************************************************
void Vehicle::RungeKutta4(struct Car* aCar, double aTimeDelta)
{
    //  Define a convenience variable to make the
    //  code more readable
    int numEqns = aCar->numEqns;

    //  Retrieve the current values of the dependent
    //  and independent variables.
    Motion q = aCar->q;

    Motion dq1;
    Motion dq2;
    Motion dq3;
    Motion dq4;

    // Compute the four Runge-Kutta steps, The return 
    // value of carRightHandSide method is an array
    // of delta-q values for each of the four steps.
    RightHandSide(aCar, &q, &q, aTimeDelta, 0.0, &dq1);
    RightHandSide(aCar, &q, &dq1, aTimeDelta, 0.5, &dq2);
    RightHandSide(aCar, &q, &dq2, aTimeDelta, 0.5, &dq3);
    RightHandSide(aCar, &q, &dq3, aTimeDelta, 1.0, &dq4);

    //  Update the dependent and independent variable values
    //  at the new dependent variable location and store the
    //  values in the ODE object arrays.
    aCar->time = aCar->time + aTimeDelta;

    DirectX::SimpleMath::Vector3 posUpdate = (dq1.position + 2.0 * dq2.position + 2.0 * dq3.position + dq4.position) / numEqns;
    q.position += posUpdate;

    DirectX::SimpleMath::Vector3 velocityUpdate = (dq1.velocity + 2.0 * dq2.velocity + 2.0 * dq3.velocity + dq4.velocity) / numEqns;
    const float stopTolerance = 0.1;
    // To prevent the car from continuing to roll forward if car velocity is less thatn the tollerance value and update velocity is zero
    if (q.velocity.Length() < stopTolerance && velocityUpdate == DirectX::SimpleMath::Vector3::Zero)
    {
        q.velocity = DirectX::SimpleMath::Vector3::Zero;
    }
    else
    {
        q.velocity += velocityUpdate;
    }

    aCar->q.position = q.position;
    aCar->q.velocity = q.velocity;
    
    // Test rear torque
    //m_car.testRearAnglularVelocity += (total torque / (Mass * radius ^ 2 / 2)) * time step
    //m_car.testRearAnglularVelocity = (m_car.testTorque / (m_car.testRearCylinderMass * (m_car.wheelRadius * m_car.wheelRadius) / 2)) * aTimeDelta;

    return;
}

void Vehicle::SteeringInputDecay(const double aTimeDelta)
{
    m_car.steeringAngleDecay = -0.2;
    if (m_car.isTurningPressed == false)
    {
        if (m_car.steeringAngle != 0.0)
        {
            if (m_car.steeringAngle > 0.0)
            {
                if (m_car.steeringAngle + (m_car.steeringAngleDecay * aTimeDelta) < 0.0)
                {
                    m_car.steeringAngle = 0.0;
                }
                else
                {
                    m_car.steeringAngle += m_car.steeringAngleDecay * aTimeDelta;
                }
            }
            else
            {
                if (m_car.steeringAngle - (m_car.steeringAngleDecay * aTimeDelta) > 0.0)
                {
                    m_car.steeringAngle = 0.0;
                }
                else
                {
                    m_car.steeringAngle -= m_car.steeringAngleDecay * aTimeDelta;
                }
            }
        }
    }

    /*
    if (m_car.isTurningPressed == false)
    {
        if (m_car.steeringAngle != 0.0)
        {
            if (m_car.steeringAngle + (m_car.steeringAngleDecay * aTimeDelta) < 0.0)
            {
                m_car.steeringAngle = 0.0;
            }
            else
            {
                m_car.steeringAngle -= m_car.steeringAngleDecay * aTimeDelta;
            }
        }
    }
    */
    //m_car.isTurningPressed
}

void Vehicle::ToggleGas()
{
    if (m_car.isAccelerating == true)
    {
        m_car.isAccelerating = false;
    }
    else
    {
        m_car.isAccelerating = true;
    }
}

void Vehicle::ToggleBrake()
{
    if (m_car.isBraking == true)
    {
        m_car.isBraking = false;
    }
    else
    {
        m_car.isBraking = true;
    }
}

void Vehicle::Turn(double aTurnInput)
{
    m_car.isTurningPressed = true;

    m_car.steeringAngle += aTurnInput * m_car.steeringSpeed;

    if (m_car.steeringAngle > m_car.steeringAngleMax)
    {
        m_car.steeringAngle = m_car.steeringAngleMax;
    }
    else if (m_car.steeringAngle < -m_car.steeringAngleMax)
    {
        m_car.steeringAngle = - m_car.steeringAngleMax;
    }

}

void Vehicle::ThrottleBrakeDecay(const double aTimeDelta)
{
    if (m_car.isThrottlePressed == false)
    {
        if (m_car.throttleInput - (m_car.throttleDecayRate * aTimeDelta) < 0.0)
        {
            m_car.throttleInput = 0.0;
        }
        else
        {
            double testDecay = m_car.throttleDecayRate * aTimeDelta;
            m_car.throttleInput -= m_car.throttleDecayRate * aTimeDelta;
        }
    }

    if (m_car.isBrakePressed == false)
    {
        if (m_car.brakeInput - (m_car.brakeDecayRate * aTimeDelta) < 0.0)
        {
            m_car.brakeInput = 0.0;
        }
        else
        {
            m_car.brakeInput -= m_car.brakeDecayRate * aTimeDelta;
        }
    }
}

void Vehicle::UpdateModel(const double aTimer)
{
    double wheelTurnRads = GetWheelRotationRadians(aTimer) + m_testRotation;
    m_testRotation = wheelTurnRads;
    //double wheelTurnRadsRear = GetWheelRotationRadiansRear(aTimer) + m_testRotationRear;
    double wheelTurnRadsRear = GetWheelRotationRadiansRear(aTimer) + m_testRotation;
    //m_testRotationRear = (wheelTurnRadsRear + wheelTurnRads) / 2;
    m_testRotationRear = wheelTurnRadsRear;
    DebugPushUILine("m_testRotation", m_testRotation);
    DebugPushUILine("m_testRotationRear", m_testRotationRear);
    //DebugPushUILine("GetWheelRotationRadiansRear(aTimer)", GetWheelRotationRadiansRear(aTimer));
    
    DirectX::SimpleMath::Matrix testTurn = DirectX::SimpleMath::Matrix::CreateRotationY(m_car.carRotation);
    DirectX::SimpleMath::Matrix updateMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_car.q.position);
    DirectX::SimpleMath::Matrix updateMat = testTurn;
    updateMat *= updateMatrix;
    
    m_carModel.bodyMatrix = m_carModel.localBodyMatrix;
    m_carModel.bodyMatrix *= testTurn;
    m_carModel.bodyMatrix *=  updateMatrix;

    m_carModel.bodyTopMatrix = m_carModel.localBodyTopMatrix;
    m_carModel.bodyTopMatrix *= testTurn;
    m_carModel.bodyTopMatrix *= updateMatrix;
    ///////

    DirectX::SimpleMath::Matrix wheelSpinMat = DirectX::SimpleMath::Matrix::CreateRotationZ(-wheelTurnRads);
    DirectX::SimpleMath::Matrix wheelSpinRearMat = DirectX::SimpleMath::Matrix::CreateRotationZ(-wheelTurnRadsRear);

    DirectX::SimpleMath::Matrix stearingTurn = DirectX::SimpleMath::Matrix::CreateRotationY(-m_car.steeringAngle);
    
    m_carModel.frontAxelMatrix = m_carModel.frontAxelRotation * wheelSpinMat * stearingTurn;
    m_carModel.frontAxelMatrix *= m_carModel.frontAxelTranslation;
    m_carModel.frontAxelMatrix *= testTurn;
    m_carModel.frontAxelMatrix *= updateMatrix;

    m_carModel.frontTireMatrix = m_carModel.frontTireRotation * wheelSpinMat * stearingTurn;
    m_carModel.frontTireMatrix *= m_carModel.frontTireTranslation;
    m_carModel.frontTireMatrix *= testTurn;
    m_carModel.frontTireMatrix *= updateMatrix;

    m_carModel.rearAxelMatrix = m_carModel.rearAxelRotation * wheelSpinRearMat;
    m_carModel.rearAxelMatrix *= m_carModel.rearAxelTranslation;
    m_carModel.rearAxelMatrix *= testTurn;
    m_carModel.rearAxelMatrix *= updateMatrix;
  
    m_carModel.rearTireMatrix = m_carModel.rearTireRotation * wheelSpinRearMat;
    m_carModel.rearTireMatrix *= m_carModel.rearTireTranslation;
    m_carModel.rearTireMatrix *= testTurn;
    m_carModel.rearTireMatrix *= updateMatrix;

    // windshield
    m_carModel.windShieldMatrix = m_carModel.localWindShieldMatrix;
    m_carModel.windShieldMatrix *= testTurn;
    m_carModel.windShieldMatrix *= updateMatrix;
    // windshield window
    m_carModel.windShieldWindowMatrix = m_carModel.localWindShieldWindowMatrix;
    m_carModel.windShieldWindowMatrix *= testTurn;
    m_carModel.windShieldWindowMatrix *= updateMatrix;
    // front side window
    m_carModel.frontSideWindowsMatrix = m_carModel.localfrontSideWindowsMatrix;
    m_carModel.frontSideWindowsMatrix *= testTurn;
    m_carModel.frontSideWindowsMatrix *= updateMatrix;
    // rear side window
    m_carModel.rearSideWindowsMatrix = m_carModel.localrearSideWindowsMatrix;
    m_carModel.rearSideWindowsMatrix *= updateMat;
    // back window
    m_carModel.backWindowMatrix = m_carModel.localBackWindowMatrix;
    m_carModel.backWindowMatrix *= updateMat;
    // rear spoiler
    m_carModel.rearSpoilerMatrix = m_carModel.locarearSpoilerMatrix;
    m_carModel.rearSpoilerMatrix *= testTurn;
    m_carModel.rearSpoilerMatrix *= updateMatrix;

    // hood
    m_carModel.hoodMatrix = m_carModel.localHoodMatrix;
    m_carModel.hoodMatrix *= testTurn;
    m_carModel.hoodMatrix *= updateMatrix;
}

void Vehicle::UpdateVehicle(const double aTimer, const double aTimeDelta)
{   
    DirectX::SimpleMath::Vector3 prevPos = m_car.q.position;
    DebugClearUI();
    ThrottleBrakeDecay(aTimeDelta);
    SteeringInputDecay(aTimeDelta);
    double preRot = m_car.carRotation;
    m_car.carRotation -= GetYawRate(aTimeDelta);
    m_car.carRotation = Utility::WrapAngle(m_car.carRotation);

    double postRot = m_car.carRotation;
    double deltaSteer = preRot - postRot;

    DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateRotationY(- deltaSteer);
    m_car.q.velocity = DirectX::SimpleMath::Vector3::Transform(m_car.q.velocity, rotMat);

    RungeKutta4(&m_car, aTimeDelta);

    double velocity = m_car.q.velocity.Length();
    
    //  Compute the new engine rpm value
    m_car.omegaE = velocity * 60.0 * m_car.gearRatio[m_car.gearNumber] * m_car.finalDriveRatio / (2.0 * Utility::GetPi() * m_car.wheelRadius);

    //  If the engine is at the redline rpm value,
    //  shift gears upward.
    if (m_car.omegaE > m_car.redline) 
    {
        double oldGearRatio = m_car.gearRatio[m_car.gearNumber];
        ++m_car.gearNumber;
        double newGearRatio = m_car.gearRatio[m_car.gearNumber];
        m_car.omegaE = m_car.omegaE * newGearRatio / oldGearRatio;
    }

    m_car.speed = velocity;

    DebugPushUILine("Speed", m_car.speed);
    DebugPushUILine("RPM", m_car.omegaE);
    DebugPushUILine("steeringAngle;", m_car.steeringAngle);
    DebugPushUILine("carRotation", m_car.carRotation);
    //DebugPushUILine("m_car.throttleInput", m_car.throttleInput);
    //DebugPushUILine("m_car.brakeInput", m_car.brakeInput);

    TestGetForceLateral();

    UpdateModel(aTimeDelta);
    m_car.isBrakePressed = false;
    m_car.isThrottlePressed = false;
    m_car.isTurningPressed = false;
    /*
    m_debugVehicleDistanceTraveled += DirectX::SimpleMath::Vector3::Distance(prevPos, m_car.q.position);
    DebugPushUILine("m_debugVehicleDistanceTraveled", m_debugVehicleDistanceTraveled);
    DebugPushUILine("m_debugWheelDistance          ", m_debugWheelDistance);
    DebugPushUILine("x Pos                           ", m_car.q.position.x);
    */

}

void Vehicle::DebugTestMove(const double aTimer, const double aTimeDelta)
{
    float move = 3.3;
    m_car.q.position.x += move;
}

void Vehicle::DebugPushUILine(std::string aString, double aVal)
{
    std::pair<std::string, double> newPair = std::make_pair(aString, aVal);
    m_debugUI.push_back(newPair);
}

void Vehicle::TestGetForceLateral()
{
    double radius = GetTurnRadius();
    double mass = m_car.mass;
    double velocity = m_car.q.velocity.Length();
    velocity *= velocity;
    double muK = 0.7; // guess at this point

    double forceLat;
    forceLat = ((mass * velocity) / radius) - (muK * mass * m_car.gravity * cos(0.0));  
    DebugPushUILine("Force Lateral", forceLat);

    DirectX::SimpleMath::Vector3 testForceLat;
    DirectX::SimpleMath::Vector3 gravVec(0.0, -9.8, 0.0);

    testForceLat = ((mass * (m_car.q.velocity * m_car.q.velocity)) / radius) - (muK * mass * gravVec * cos(0.0));

    DebugPushUILine("Force Lateral X ", testForceLat.x);
    DebugPushUILine("Force Lateral Y ", testForceLat.y);
    DebugPushUILine("Force Lateral Z ", testForceLat.z);


    DirectX::SimpleMath::Vector3 testHeading = m_car.q.velocity;
    DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateRotationY(-m_car.carRotation);
    testHeading = DirectX::SimpleMath::Vector3::Transform(testHeading, rotMat);

    double slipAngle;
    slipAngle = -atan(testHeading.x / abs(testHeading.z));
    DebugPushUILine("slipAngle", Utility::ToDegrees(slipAngle)  + 90.0);
    DebugPushUILine("slipAngle 2", Utility::ToDegrees(slipAngle) - Utility::ToDegrees(m_car.carRotation));
    DebugPushUILine("slipAngle 3", Utility::ToDegrees(slipAngle) + 90.0 - Utility::ToDegrees(m_car.carRotation));
    DebugPushUILine("m_car.carRotation ", Utility::ToDegrees(m_car.carRotation));
}
