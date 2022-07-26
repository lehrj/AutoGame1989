#include "pch.h"
#include "Vehicle.h"


/*
Vehicle::Vehicle()
{
    //InitializeVehicle();
}
*/

void Vehicle::DebugEBrake()
{
    m_car.q.velocity = DirectX::SimpleMath::Vector3::Zero;
}

void Vehicle::DrawModel(DirectX::SimpleMath::Matrix aWorld, DirectX::SimpleMath::Matrix aView, DirectX::SimpleMath::Matrix aProj)
{
    DirectX::SimpleMath::Matrix world = aWorld;
    world.Up(m_environment->GetTerrainNormal(m_car.q.position));
    world = DirectX::SimpleMath::Matrix::CreateWorld(m_car.q.position, m_car.headingVec, m_environment->GetTerrainNormal(m_car.q.position));

    DirectX::SimpleMath::Matrix view = aView;
    DirectX::SimpleMath::Matrix proj = aProj;
    
    // testing variations of volvo yellow color
    float x = 252.0 / 256.0;
    float y = 255.0 / 256.0;
    float z = 164.0 / 256.0;
    DirectX::SimpleMath::Vector4 volvoYellow(x, y, z, 1.0);

    DirectX::SimpleMath::Vector4 tireColor(0.2, 0.2, 0.2, 1.0);
    DirectX::SimpleMath::Vector4 windowColor(0.09, 0.09, 0.09, 1.0);
    DirectX::SimpleMath::Vector4 windShieldColor(0.09, 0.09, 0.09, 0.7);
    DirectX::SimpleMath::Vector4 bumperColor = DirectX::Colors::Black;
    DirectX::SimpleMath::Vector4 grillColor(0.08, 0.08, 0.08, 0.9);
    DirectX::SimpleMath::Vector4 grillChromeColor(0.9, 0.9, 0.9, 1.5);
    DirectX::SimpleMath::Vector4 chromeColor(0.9, 0.9, 0.9, 1.1);
    DirectX::SimpleMath::Vector4 headLightColor(0.9, 0.9, 0.9, 1.1);
    DirectX::SimpleMath::Vector4 headLightColor2(0.9, 0.9, 0.9, 0.7);

    DirectX::SimpleMath::Vector4 blinkerLightOrange(1.000000000f, 0.647058845f, 0.000000000f, 0.8);
    DirectX::SimpleMath::Vector4 blinkerLightSilver(0.752941251f, 0.752941251f, 0.752941251f, 0.8);
    DirectX::SimpleMath::Vector4 reverseLightColor(0.9, 0.9, 0.9, 1.1);
    DirectX::SimpleMath::Vector4 fenderFlareShadowColor(0.0, 0.0, 0.0, 0.7);
    DirectX::SimpleMath::Vector4 testColor = DirectX::Colors::Red;
    DirectX::SimpleMath::Vector4 testColor2(0.1, 0.1, 0.1, 1.0);
    DirectX::SimpleMath::Vector4 testColor3 = DirectX::Colors::White;
    DirectX::SimpleMath::Vector4 testColor4(0.1, 0.1, 0.1, 0.6);

    DirectX::SimpleMath::Vector4 rimColor(0.4, 0.4, 0.4, 1.0);
    DirectX::SimpleMath::Vector4 spokeColor = rimColor;
    DirectX::SimpleMath::Vector4 sideMirrorColor(0.2, 0.2, 0.2, 1.0);
    DirectX::SimpleMath::Vector4 rockerSkirtColor(0.201960814f, 0.201960814f, 0.201960814f,  1.0);
    bumperColor = rockerSkirtColor;
    DirectX::SimpleMath::Vector4 tailLightColor;// (0.9, 0.0, 0.0, 1.0);

    //m_testIsBreakLightOn = false;
    //if (m_testIsBreakLightOn == true)
    //if (m_car.brakeInput > 0.0)
    if (m_car.isBrakePressed == true)
    {
        tailLightColor = DirectX::SimpleMath::Vector4(0.9, 0.0, 0.0, 5.0);
    }
    else
    {
        tailLightColor = DirectX::SimpleMath::Vector4(0.9, 0.0, 0.0,  1.0);
    }



    m_carModel.tire->Draw(m_carModel.wheelFrontRightMatrix, view, proj, tireColor);
    m_carModel.tire->Draw(m_carModel.wheelFrontLeftMatrix, view, proj, tireColor);
    m_carModel.tire->Draw(m_carModel.wheelRearRightMatrix, view, proj, tireColor);
    m_carModel.tire->Draw(m_carModel.wheelRearLeftMatrix, view, proj, tireColor);
    m_carModel.hubCap->Draw(m_carModel.wheelFrontLeftMatrix, view, proj, rimColor);
    m_carModel.hubCap->Draw(m_carModel.wheelFrontRightMatrix, view, proj, rimColor);
    m_carModel.hubCap->Draw(m_carModel.wheelRearRightMatrix, view, proj, rimColor);
    m_carModel.hubCap->Draw(m_carModel.wheelRearLeftMatrix, view, proj, rimColor);


    m_carModel.fenderFlare->Draw(m_carModel.fenderFlareFrontLeftMatrix, view, proj, volvoYellow);
    m_carModel.fenderFlareInterior->Draw(m_carModel.fenderFlareFrontLeftInteriorMatrix, view, proj, fenderFlareShadowColor);
    m_carModel.fenderFlare->Draw(m_carModel.fenderFlareFrontRightMatrix, view, proj, volvoYellow);
    m_carModel.fenderFlareInterior->Draw(m_carModel.fenderFlareFrontRightInteriorMatrix, view, proj, fenderFlareShadowColor);

    m_carModel.fenderRearFlare->Draw(m_carModel.fenderFlareRearLeftMatrix, view, proj, volvoYellow);
    m_carModel.fenderRearFlareInterior->Draw(m_carModel.fenderFlareRearLeftInteriorMatrix, view, proj, fenderFlareShadowColor);
    m_carModel.fenderRearFlare->Draw(m_carModel.fenderFlareRearRightMatrix, view, proj, volvoYellow);
    m_carModel.fenderRearFlareInterior->Draw(m_carModel.fenderFlareRearRightInteriorMatrix, view, proj, fenderFlareShadowColor);


    m_carModel.bodyTop->Draw(m_carModel.bodyTopMatrix, view, proj, volvoYellow);
    m_carModel.body->Draw(m_carModel.bodyMatrix, view, proj, volvoYellow);
    m_carModel.pinstripe->Draw(m_carModel.pinstripeMatrix, view, proj, rockerSkirtColor);
    m_carModel.rockerSkirt->Draw(m_carModel.rockerSkirtMatrix, view, proj, rockerSkirtColor);

    m_carModel.windShield->Draw(m_carModel.windShieldMatrix, view, proj, volvoYellow);
    m_carModel.rearSpoiler->Draw(m_carModel.rearSpoilerMatrix, view, proj, volvoYellow);
    m_carModel.airDam->Draw(m_carModel.airDamMatrix, view, proj, rockerSkirtColor);
    m_carModel.hood->Draw(m_carModel.hoodMatrix, view, proj, volvoYellow);
    
    m_carModel.windShieldWindow->Draw(m_carModel.windShieldWindowMatrix, view, proj, windShieldColor);
    m_carModel.frontSideWindows->Draw(m_carModel.frontSideWindowsMatrix, view, proj, windowColor);
    m_carModel.rearSideWindows->Draw(m_carModel.rearSideWindowsMatrix, view, proj, windowColor);
    m_carModel.backWindow->Draw(m_carModel.backWindowMatrix, view, proj, windowColor);
    
    m_carModel.triangleFrontWindow->Draw(m_carModel.triangleFrontWindowMatrix, view, proj, windowColor);
    m_carModel.bumperFront->Draw(m_carModel.bumperFrontMatrix, view, proj, bumperColor);
    m_carModel.bumperBack->Draw(m_carModel.bumperBackMatrix, view, proj, bumperColor);
    m_carModel.grillOutline->Draw(m_carModel.grillMatrix, view, proj, grillChromeColor);
    m_carModel.grill->Draw(m_carModel.grillMatrix, view, proj, grillColor);
    m_carModel.grillLogo->Draw(m_carModel.grillLogoMatrix, view, proj, chromeColor);
    m_carModel.grillSlat->Draw(m_carModel.grillSlatMatrix, view, proj, chromeColor);

    m_carModel.licensePlateRearOutline->Draw(m_carModel.licensePlateRearMatrix, view, proj, testColor4);
    m_carModel.licensePlateRear->Draw(m_carModel.licensePlateRearMatrix, view, proj, DirectX::Colors::White);



    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFL1, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFL2, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFL3, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFL4, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFL5, view, proj, spokeColor);

    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFR1, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFR2, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFR3, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFR4, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeFR5, view, proj, spokeColor);

    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearLeft1, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearLeft2, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearLeft3, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearLeft4, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearLeft5, view, proj, spokeColor);

    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearRight1, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearRight2, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearRight3, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearRight4, view, proj, spokeColor);
    m_carModel.wheelSpoke->Draw(m_carModel.wheelSpokeRearRight5, view, proj, spokeColor);



    m_carModel.sideMirror->Draw(m_carModel.sideMirrorLeftMatrix, view, proj, sideMirrorColor);
    m_carModel.sideMirror->Draw(m_carModel.sideMirrorRightMatrix, view, proj, sideMirrorColor);

    m_carModel.rimEdge->Draw(m_carModel.hubBackRightMatrix, view, proj, spokeColor);
    m_carModel.tireEdge->Draw(m_carModel.hubBackRightMatrix, view, proj, tireColor);
    m_carModel.rimEdge->Draw(m_carModel.hubInteriorBackRightMatrix, view, proj, spokeColor);
    m_carModel.tireEdge->Draw(m_carModel.hubInteriorBackRightMatrix, view, proj, tireColor);

    m_carModel.rimEdge->Draw(m_carModel.hubFrontRightMatrix, view, proj, spokeColor);
    m_carModel.tireEdge->Draw(m_carModel.hubFrontRightMatrix, view, proj, tireColor);
    m_carModel.rimEdge->Draw(m_carModel.hubInteriorFrontRightMatrix, view, proj, spokeColor);
    m_carModel.tireEdge->Draw(m_carModel.hubInteriorFrontRightMatrix, view, proj, tireColor);

    m_carModel.rimEdge->Draw(m_carModel.hubBackLeftMatrix, view, proj, spokeColor);
    m_carModel.tireEdge->Draw(m_carModel.hubBackLeftMatrix, view, proj, tireColor);
    m_carModel.rimEdge->Draw(m_carModel.hubInteriorBackLeftMatrix, view, proj, spokeColor);
    m_carModel.tireEdge->Draw(m_carModel.hubInteriorBackLeftMatrix, view, proj, tireColor);

    m_carModel.rimEdge->Draw(m_carModel.hubFrontLeftMatrix, view, proj, spokeColor);
    m_carModel.tireEdge->Draw(m_carModel.hubFrontLeftMatrix, view, proj, tireColor);
    m_carModel.rimEdge->Draw(m_carModel.hubInteriorFrontLeftMatrix, view, proj, spokeColor);
    m_carModel.tireEdge->Draw(m_carModel.hubInteriorFrontLeftMatrix, view, proj, tireColor);




    m_carModel.headLight->Draw(m_carModel.headLightLeftMatrix, view, proj, headLightColor);
    m_carModel.headLight->Draw(m_carModel.headLightRightMatrix, view, proj, headLightColor);
    m_carModel.blinkerLight->Draw(m_carModel.blinkerLightLowerLeftMatrix, view, proj, blinkerLightOrange);
    m_carModel.blinkerLight->Draw(m_carModel.blinkerLightUpperLeftMatrix, view, proj, blinkerLightSilver);
    m_carModel.blinkerLight->Draw(m_carModel.blinkerLightLowerRightMatrix, view, proj, blinkerLightOrange);
    m_carModel.blinkerLight->Draw(m_carModel.blinkerLightUpperRightMatrix, view, proj, blinkerLightSilver);
    m_carModel.tailLight->Draw(m_carModel.tailLightRightMatrix, view, proj, tailLightColor);
    m_carModel.tailLight->Draw(m_carModel.tailLightLeftMatrix, view, proj, tailLightColor);
    m_carModel.thirdBrakeLight->Draw(m_carModel.thirdBrakeLightMatrix, view, proj, tailLightColor);

    m_carModel.reverseLight->Draw(m_carModel.tailBlinkerLightLeftMatrix, view, proj, blinkerLightOrange);
    m_carModel.reverseLight->Draw(m_carModel.tailBlinkerLightRightMatrix, view, proj, blinkerLightOrange);

    m_carModel.reverseLight->Draw(m_carModel.reverseLightRightMatrix, view, proj, reverseLightColor);
    m_carModel.reverseLight->Draw(m_carModel.reverseLightLeftMatrix, view, proj, reverseLightColor);

    // test powered wheel model draw
    //m_carModel.testWheel->Draw(m_carModel.testWorldWheelMatrix, view, proj, testColor);
}

void Vehicle::GearDown()
{
    if (m_car.gearNumber > 0 && m_car.gearNumber <= m_car.numberOfGears)
    {
        m_car.shiftCooldown = m_car.shiftDelay;
        --m_car.gearNumber;
    }
}

void Vehicle::GearUp()
{
    if (m_car.gearNumber >= 0 && m_car.gearNumber < m_car.numberOfGears)
    {
        m_car.shiftCooldown = m_car.shiftDelay;
        ++m_car.gearNumber;
    }
}

float Vehicle::GetCarRotation()
{
    float turnRadius = GetTurnRadius();
    float steeringAngle = m_car.steeringAngle;

    DirectX::SimpleMath::Vector3 testRadVec(0.0, 0.0, static_cast<float>(-turnRadius));
    DirectX::SimpleMath::Matrix testTurnMat = DirectX::SimpleMath::Matrix::CreateRotationY(steeringAngle);

    testRadVec = DirectX::SimpleMath::Vector3::Transform(testRadVec, testTurnMat);
    DirectX::SimpleMath::Vector3 testRadVecNorm = testRadVec;
    testRadVecNorm.Normalize();

    DirectX::SimpleMath::Vector3 ballVec = testRadVecNorm;
    DirectX::SimpleMath::Vector3 zeroDirection = DirectX::SimpleMath::Vector3::UnitZ;

    DirectX::SimpleMath::Vector3 a = - DirectX::SimpleMath::Vector3::UnitZ;
    DirectX::SimpleMath::Vector3 b = DirectX::SimpleMath::Vector3::UnitX;
    b = testRadVecNorm;

    float testAngle = acos(a.Dot(b));

    if (testAngle > m_testMax)
    {
        m_testMax = testAngle;
    }
    if (testAngle < m_testMin)
    {
        m_testMin = testAngle;
    }

    float carRotation = testAngle + 0.1f; 

    return carRotation;
}

float Vehicle::GetTurnRadius()
{
    float wheelBase = m_car.wheelBase;
    float sinDelta = sin(m_car.steeringAngle);
    float sinDelta2 = sin((10.0f * Utility::GetPi()) / 180.0f);
    float turnRadius = 1.0f;
    float turnRadius2 = 1.0f;
    if (sinDelta == 0.0f)
    {
        turnRadius = 0.0f;
    }
    else
    {
        turnRadius = wheelBase / sinDelta;
        turnRadius2 = wheelBase / sinDelta2;
    }

    return turnRadius;
}

float Vehicle::GetYawRate(double aTimeDelta)
{
    float wheelBase = m_car.wheelBase;
    float velocity = m_car.q.velocity.Length();
    float steeringAngle = m_car.steeringAngle;

    float sinDelta = sin(steeringAngle);

    float omega = (velocity * sinDelta) / wheelBase;
    float omegaT = omega * static_cast<float>(aTimeDelta);

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

float Vehicle::GetWheelRotationRadians(const double aTimeDelta)
{
    DirectX::SimpleMath::Vector3 velocity = m_car.q.velocity;
    float distance = DirectX::SimpleMath::Vector3::Distance(velocity, DirectX::SimpleMath::Vector3::Zero);
    float stepDistance;
    if (aTimeDelta != 0.0)
    {
        stepDistance = distance * static_cast<float>(aTimeDelta);
    }
    else
    {
        stepDistance = 0.0;
    }

    float circumference = 2.0f * Utility::GetPi() * m_car.wheelRadius;
    float turnRatio = stepDistance / circumference;
    float rotations = turnRatio * (2.0f * Utility::GetPi());
    float wheelMove = rotations * circumference;
    m_debugWheelDistance += wheelMove;

    // flip wheel spin direction if velocity is backwards
    if (m_car.isVelocityBackwards == true)
    {
        rotations *= -1;
    }

    return rotations;
}

float Vehicle::GetWheelRotationRadiansRear(const double aTimeDelta)
{
    float rotations = 0.0f;
    if (aTimeDelta != 0.0)
    {
        rotations = m_car.testRearAnglularVelocity * static_cast<float>(aTimeDelta);
    }
    else
    {
        rotations = 0.0f;
    }

    return rotations;
}

void Vehicle::InitializeModel(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext)
{
    // porche boxter base dimensions - 4.3942m L x 1.8034m W x 1.27m H, wheel diameter 0.3186m
    const float wheelRadius = m_car.wheelRadius;
    const float wheelDiameter = wheelRadius * 2.0f;
    const float axelRadius = wheelRadius * 0.275f;
    const float axelDiameter = axelRadius * 2.0f;
    const float length = 4.3942f;
    const float width = 1.8034f;
    const float heightTotal = 1.27f;
    const float height = heightTotal * 0.6f;
    const float windowPlaneOffset = 0.03f;
    const float axelLength = width + 0.56f;
    const float tireLength = width + 0.5f;
    const float wheelBase = m_car.wheelBase;
    const float frontAxelOffset = wheelBase * .55f;
    const float rearAxelOffset = wheelBase * .45f;
    const float zFightOffSet = 0.001f;
    DirectX::SimpleMath::Vector3 carBodySize(length, height - wheelRadius, width);




    m_carModel.body = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), carBodySize);
    m_carModel.frontAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, axelDiameter, 32);
    m_carModel.rearAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, axelDiameter, 32);

    m_carModel.frontTire = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), tireLength, wheelDiameter, 32);
    m_carModel.rearTire = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), tireLength, wheelDiameter, 32);
    const float rimWidth = m_car.wheelWidth * 1.25f;
    m_carModel.hubCap = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), rimWidth, axelDiameter, 32);

    m_carModel.bodyMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.frontAxelMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.rearAxelMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.frontTireMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.rearTireMatrix = DirectX::SimpleMath::Matrix::Identity;

    DirectX::SimpleMath::Vector3 bodyTranslation(0.0f, height + wheelRadius, 0.0f);
    m_carModel.bodyMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(bodyTranslation);

    // Axels
    DirectX::SimpleMath::Vector3 frontAxelTranslation(DirectX::SimpleMath::Vector3(wheelBase * .55f, wheelRadius, 0.0f));
    DirectX::SimpleMath::Matrix frontAxelTransMat = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .55f, wheelRadius, 0.0f));

    DirectX::SimpleMath::Matrix axelRotation = DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0f));
    m_carModel.frontAxelMatrix *= axelRotation;
    m_carModel.frontAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(frontAxelTranslation);
    m_carModel.rearAxelMatrix *= axelRotation;
    m_carModel.rearAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .45f, wheelRadius, 0.0f));
    
    m_carModel.frontTireMatrix *= axelRotation;
    m_carModel.frontTireMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5f, wheelRadius, 0.0f));
    m_carModel.rearTireMatrix *= axelRotation;
    m_carModel.rearTireMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5f, wheelRadius, 0.0f));

    m_carModel.frontAxelRotation *= axelRotation;
    m_carModel.frontAxelTranslation *= DirectX::SimpleMath::Matrix::CreateTranslation(frontAxelTranslation);

    m_carModel.rearAxelRotation = axelRotation;
    m_carModel.rearAxelTranslation = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5f, wheelRadius, 0.0f));

    m_carModel.frontTireRotation *= axelRotation;
    m_carModel.frontTireTranslation *= DirectX::SimpleMath::Matrix::CreateTranslation(frontAxelTranslation);

    m_carModel.rearTireRotation = axelRotation;
    m_carModel.rearTireTranslation = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5f, wheelRadius, 0.0f));
    /// Axels End ////////////////////////////////////////////////////////////
    /// 
    /// Independent Wheels Start /////////////////////////////////////////////
    m_carModel.tire = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), m_car.wheelWidth, m_car.wheelRadius * 2.0f, 32);
    /// Front Left Wheel Start
    m_carModel.wheelFrontLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.wheelFrontLeftMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0));
    m_carModel.wheelFrontLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(frontAxelOffset, m_car.wheelRadius, -(carBodySize.z * 0.5f)));
    m_carModel.wheelFrontLeftTranslation = m_carModel.wheelFrontLeftMatrix;
    /// Front Right Wheel Start
    m_carModel.wheelFrontRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.wheelFrontRightMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0));
    m_carModel.wheelFrontRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(frontAxelOffset, m_car.wheelRadius, (carBodySize.z * 0.5f)));
    m_carModel.wheelFrontRightTranslation = m_carModel.wheelFrontRightMatrix;
    /// Rear Left Wheel Start
    m_carModel.wheelRearLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.wheelRearLeftMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0));
    m_carModel.wheelRearLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-rearAxelOffset, m_car.wheelRadius, -(carBodySize.z * 0.5f)));
    m_carModel.wheelRearLeftTranslation = m_carModel.wheelRearLeftMatrix;
    /// Rear Right Wheel Start
    m_carModel.wheelRearRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.wheelRearRightMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0));
    m_carModel.wheelRearRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-rearAxelOffset, m_car.wheelRadius, (carBodySize.z * 0.5f)));
    m_carModel.wheelRearRightTranslation = m_carModel.wheelRearRightMatrix;
    /// Independent Wheels End   /////////////////////////////////////////////

    /// Independant Wheel Spoke Start ///////////////////////////////////////
    DirectX::SimpleMath::Vector3 spokeSize;
    spokeSize.x = wheelRadius * 0.25f;
    spokeSize.y = axelLength - 0.003;
    spokeSize.y = 2.35;
    spokeSize.y = m_car.wheelWidth * 1.21f;
    spokeSize.z = wheelRadius * 0.5f;
    m_carModel.wheelSpoke = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), spokeSize);
    const float spokeAngle = (2.0f * Utility::GetPi()) * 0.2f;
    const float spokeOffset = 0.15;
    const float spokeDeapth = 0.01;
    m_carModel.localWheelSpoke1 *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, 0.0, spokeOffset));
    m_carModel.localWheelSpoke1 *= DirectX::SimpleMath::Matrix::CreateRotationY(spokeAngle);
    m_carModel.localWheelSpoke2 *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, 0.0, spokeOffset));
    m_carModel.localWheelSpoke2 *= DirectX::SimpleMath::Matrix::CreateRotationY(spokeAngle * 2.0f);
    m_carModel.localWheelSpoke3 *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, 0.0, spokeOffset));
    m_carModel.localWheelSpoke3 *= DirectX::SimpleMath::Matrix::CreateRotationY(spokeAngle * 3.0f);
    m_carModel.localWheelSpoke4 *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, 0.0, spokeOffset));
    m_carModel.localWheelSpoke4 *= DirectX::SimpleMath::Matrix::CreateRotationY(spokeAngle * 4.0f);
    m_carModel.localWheelSpoke5 *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, 0.0, spokeOffset));
    m_carModel.localWheelSpoke5 *= DirectX::SimpleMath::Matrix::CreateRotationY(spokeAngle * 5.0f);

    m_carModel.wheelSpokeFL1 = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.wheelSpokeFL1 *= axelRotation;
    m_carModel.wheelSpokeFL1 *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5f, wheelRadius, 0.0f));
    m_carModel.wheelSpokeFL1 *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, spokeSize.y * 0.5f, 0.0f));
    /// Independant Wheel Spoke End /////////////////////////////////////////
    /// Wheel Rim Start /////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 rimSize;
    rimSize.x = 0.0;
    rimSize.y = 0.0;
    rimSize.z = 0.0;
    const float rimDiameter = (wheelRadius * 0.8f) * 2.0f;
    m_carModel.wheelRim = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), tireLength + 0.006, rimDiameter, 32);
    /// Wheel Rim End ///////////////////////////////////////////////////////
    /// Hub Back Left Start /////////////////////////////////////////////////////
    m_carModel.hubBackLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hubBackLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -m_car.wheelWidth * 0.5f, 0.0f));
    m_carModel.localHubBackLeftMatrix = m_carModel.hubBackLeftMatrix;
    m_carModel.hubBackLeftMatrix *= m_carModel.wheelRearLeftTranslation;
    /// Hub Back Left End ///////////////////////////////////////////////////////
    /// Hub Back InteriorLeft Start /////////////////////////////////////////////////////
    m_carModel.hubInteriorBackLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hubInteriorBackLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, m_car.wheelWidth * 0.5f, 0.0f));
    m_carModel.localHubInteriorBackLeftMatrix = m_carModel.hubInteriorBackLeftMatrix;
    m_carModel.hubInteriorBackLeftMatrix *= m_carModel.wheelRearLeftTranslation;
    /// Hub Back Interior Left End ///////////////////////////////////////////////////////
    /// Hub Back Right Start /////////////////////////////////////////////////////
    m_carModel.hubBackRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hubBackRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, m_car.wheelWidth * 0.5f, 0.0f));
    m_carModel.localHubBackRightMatrix = m_carModel.hubBackRightMatrix;
    m_carModel.hubBackRightMatrix *= m_carModel.wheelRearRightTranslation;
    /// Hub Back Right End ///////////////////////////////////////////////////////
    /// Hub Back InteriorRight Start /////////////////////////////////////////////////////
    m_carModel.hubInteriorBackRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hubInteriorBackRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -m_car.wheelWidth * 0.5f, 0.0f));
    m_carModel.localHubInteriorBackRightMatrix = m_carModel.hubInteriorBackRightMatrix;
    m_carModel.hubInteriorBackRightMatrix *= m_carModel.wheelRearRightTranslation;
    /// Hub Back InteriorRight End ///////////////////////////////////////////////////////
    /// Hub Front Left Start /////////////////////////////////////////////////////
    m_carModel.hubFrontLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hubFrontLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -m_car.wheelWidth * 0.5f, 0.0f));
    m_carModel.localHubFrontLeftMatrix = m_carModel.hubFrontLeftMatrix;
    m_carModel.hubFrontLeftMatrix *= m_carModel.wheelFrontLeftTranslation;
    /// Hub Front Left End /////////////////////////////////////////////////////// 
    /// Hub Interior Front Left Start /////////////////////////////////////////////////////
    m_carModel.hubInteriorFrontLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hubInteriorFrontLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, m_car.wheelWidth * 0.5f, 0.0f));
    m_carModel.localHubInteriorFrontLeftMatrix = m_carModel.hubInteriorFrontLeftMatrix;
    m_carModel.hubInteriorFrontLeftMatrix *= m_carModel.wheelFrontLeftTranslation;
    /// Hub Front Interior Left End /////////////////////////////////////////////////////// 
    /// Hub Front Right Start /////////////////////////////////////////////////////
    m_carModel.hubFrontRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hubFrontRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, m_car.wheelWidth * 0.5f, 0.0f));
    m_carModel.localHubFrontRightMatrix = m_carModel.hubFrontRightMatrix;
    m_carModel.hubFrontRightMatrix *= m_carModel.wheelFrontRightTranslation;
    /// Hub Front Right End /////////////////////////////////////////////////////// 
    /// Hub Interior Front Right Start /////////////////////////////////////////////////////
    m_carModel.hubInteriorFrontRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.hubInteriorFrontRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -m_car.wheelWidth * 0.5f, 0.0f));
    m_carModel.localHubInteriorFrontRightMatrix = m_carModel.hubInteriorFrontRightMatrix;
    m_carModel.hubInteriorFrontRightMatrix *= m_carModel.wheelFrontRightTranslation;
    /// Hub Front Interior Right End /////////////////////////////////////////////////////// 
    /// Rim Edge Start /////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 rimEdgeSize;
    rimEdgeSize.x = 0.0;
    rimEdgeSize.y = 0.0;
    rimEdgeSize.z = 0.0;
    float rimEdgeWidth = wheelDiameter - ((wheelRadius * 0.8f) * 2.0f);
    rimEdgeWidth *= 0.8;
    rimEdgeWidth = rimDiameter - ((wheelRadius * 0.8f) * 2.0f);
    rimEdgeWidth = 0.05;
    //rimEdgeWidth = 0.02;
    float rimEdgeDiameter = wheelDiameter - rimEdgeWidth;
    rimEdgeDiameter = rimDiameter - rimEdgeWidth;
    m_carModel.rimEdge = DirectX::GeometricPrimitive::CreateTorus(aContext.Get(), rimEdgeDiameter, rimEdgeWidth, 32);
    m_carModel.rimEdgeMatrix = DirectX::SimpleMath::Matrix::Identity;

    m_carModel.rimEdgeMatrix *= m_carModel.rearTireRotation;
    m_carModel.rimEdgeMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, 0.0, (tireLength * 0.5) - (0.0)));
    m_carModel.rimEdgeMatrix *= m_carModel.rearTireTranslation;
    /// Rim Edge End ///////////////////////////////////////////////////////
    
    /// Tire Edge Start /////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 tireEdgeSize;
    tireEdgeSize.x = 0.0;
    tireEdgeSize.y = 0.0;
    tireEdgeSize.z = 0.0;
    float edgeWidth = wheelDiameter - ((wheelRadius * 0.8f) * 2.0f);
    edgeWidth *= 0.8;


    float edgeDiameter = wheelDiameter - edgeWidth;

    m_carModel.tireEdge = DirectX::GeometricPrimitive::CreateTorus(aContext.Get(), edgeDiameter, edgeWidth, 32);
    m_carModel.tireEdgeMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.tireEdgeMatrix *= m_carModel.rearTireRotation;
    m_carModel.tireEdgeMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, 0.0, (tireLength * 0.5) - (0.0)));
    m_carModel.tireEdgeMatrix *= m_carModel.rearTireTranslation;
    /// Tire Edge End ///////////////////////////////////////////////////////
    
    const float topIndent = 0.2;   
    const float topHeight = heightTotal * 0.4;
    const float topLength = length * .6;
    const float roofHeightAlignment = height + wheelRadius + wheelRadius + topHeight;
    const float roofLengthAlignment = length - topLength - topIndent;
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
     
    /////// Rocker Skirt Panels Start ////////////////////////////////////////
    DirectX::SimpleMath::Vector3 rockerSkirtSize = carBodySize;
    rockerSkirtSize.x *= 0.77f;
    rockerSkirtSize.y *= 0.2f;
    rockerSkirtSize.z += 0.05f;
    m_carModel.rockerSkirt = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), rockerSkirtSize);
    DirectX::SimpleMath::Vector3 rockerSkirtTranslation;
    rockerSkirtTranslation.x = -(carBodySize.x * 0.5f) + (rockerSkirtSize.x * 0.5f) - zFightOffSet;
    rockerSkirtTranslation.y = height - carBodySize.y + (rockerSkirtSize.y * 0.5f) - zFightOffSet;
    rockerSkirtTranslation.z = 0.0f;
    m_carModel.rockerSkirtMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.rockerSkirtMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(rockerSkirtTranslation);
    m_carModel.localRockerSkirtMatrix = m_carModel.rockerSkirtMatrix;
    /////// Rocker Skirt Panels End   ////////////////////////////////////////
    
    /// rear spoiler start /////////////////////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 rearSpoilerSize = carBodyTopSize;
    rearSpoilerSize.x = 0.1f;
    rearSpoilerSize.y = 0.07f;   
    rearSpoilerSize.z *= 0.8f;
    m_carModel.rearSpoiler = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), rearSpoilerSize);

    m_carModel.rearSpoilerMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Vector3 spoilerTranslation(-roofLengthAlignment, roofHeightAlignment, 0.0);
    spoilerTranslation.y = roofHeightAlignment + (carBodyTopSize.y * 0.5f);

    spoilerTranslation.y = roofHeightAlignment + carBodyTopSize.y - rearSpoilerSize.y;
    spoilerTranslation.x = - rearSpoilerSize.x - roofLengthAlignment - carBodyTopSize.x;
    m_carModel.rearSpoilerMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(spoilerTranslation);
    m_carModel.locarearSpoilerMatrix = m_carModel.rearSpoilerMatrix;
    /// rear spoiler end //////////////////////////////////////////////////////////////////////////////////

    /// windshield start /////////////////////////////////////////////////////////////////////////////////  
    DirectX::SimpleMath::Vector3 windShieldSize(1.0, 0.5, carBodyTopSize.z - 0.0001f);
    const float windShieldLengthAlignment = (topLength * 2.0) - roofLengthAlignment;
    m_carModel.windShield = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), windShieldSize);
    m_carModel.windShieldMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Quaternion testWindShieldRotate = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, 0.0, Utility::ToRadians(-99.0));
    m_carModel.windShieldMatrix = DirectX::SimpleMath::Matrix::Transform(m_carModel.windShieldMatrix, testWindShieldRotate);
    DirectX::SimpleMath::Vector3 windShieldAllignment = DirectX::SimpleMath::Vector3::Zero;
    windShieldAllignment.x = windShieldLengthAlignment;
    windShieldAllignment.y = roofHeightAlignment;
    windShieldAllignment.x = 3.71535993;
    windShieldAllignment.y = 1.90719998;
    windShieldAllignment.x = 1.255;
    windShieldAllignment.y = 1.711;  
    m_carModel.windShieldMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(windShieldAllignment);
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
    m_carModel.frontSideWindowsMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(carBodyTopSize.x * 0.37f, 0.0f, 0.0f));
    m_carModel.localfrontSideWindowsMatrix = m_carModel.frontSideWindowsMatrix;
    /// FrontSide Windows End   ////////////////////////////////////////////////////////////////

    /// Rear Side Windows Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 rearSideWindowSize = frontSideWindowSize;
    rearSideWindowSize.x *= 2.5;
    m_carModel.rearSideWindows = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), rearSideWindowSize);
    m_carModel.rearSideWindowsMatrix = m_carModel.frontSideWindowsMatrix;
    m_carModel.rearSideWindowsMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3( - frontSideWindowSize.x * 2.0f, 0.0f, 0.0f));
    m_carModel.localrearSideWindowsMatrix = m_carModel.rearSideWindowsMatrix;
    /// Rear Side Windows End   ////////////////////////////////////////////////////////////////

    /// Back Window Start  ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 backWindowSize = rearSpoilerSize;
    backWindowSize.y = rearSideWindowSize.y;
    backWindowSize.z *= 1.1;
    m_carModel.backWindow = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), backWindowSize);
    m_carModel.backWindowMatrix = m_carModel.rearSpoilerMatrix;
    m_carModel.backWindowMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(rearSpoilerSize.x - windowPlaneOffset, (-backWindowSize.y * 0.5f) + (-rearSpoilerSize.y * 0.5f), 0.0f));
    m_carModel.localBackWindowMatrix = m_carModel.backWindowMatrix;
    /// Back Window End   ////////////////////////////////////////////////////////////////

    /// Triangle Front Window Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3  triangleWindowSize = windShieldSize;
    triangleWindowSize.z *= 1.0 + windowPlaneOffset;
    triangleWindowSize.x -= 0.0001;
    m_carModel.triangleFrontWindow = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), triangleWindowSize);
    m_carModel.triangleFrontWindowMatrix = m_carModel.windShieldMatrix;
    m_carModel.triangleFrontWindowMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-0.014, -0.064, 0.0));
    m_carModel.localtriangleFrontWindowMatrix = m_carModel.triangleFrontWindowMatrix;
    /// Triangle Front Window End   ////////////////////////////////////////////////////////////////

    /// Front Bumper Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 bumperFrontSize;
    bumperFrontSize.x = 0.4;
    bumperFrontSize.y = 0.2;
    bumperFrontSize.z = carBodySize.z + 0.1f;
    DirectX::SimpleMath::Vector3 bumperFrontSizeTest = bumperFrontSize;
    m_carModel.bumperFront = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), bumperFrontSize);
    m_carModel.bumperFrontMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.bumperFrontMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(carBodySize.x * 0.5f - bumperFrontSize.x * 0.25f, wheelRadius + bumperFrontSize.y * 0.5f + zFightOffSet, 0.0));
    m_carModel.localBumperFrontMatrix = m_carModel.bumperFrontMatrix;
    /// Front Bumper End   ////////////////////////////////////////////////////////////////

    /// Air Dam Start //////////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 airDamSize;
    airDamSize.x = bumperFrontSize.x * 0.9f;
    airDamSize.y = bumperFrontSize.y * 0.4f;
    airDamSize.z = bumperFrontSize.z * 0.85f;
    m_carModel.airDam = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), airDamSize);
    m_carModel.airDamMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.airDamMatrix = DirectX::SimpleMath::Matrix::Transform(m_carModel.airDamMatrix, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, 0.0, Utility::ToRadians(-55.0)));
    DirectX::SimpleMath::Vector3 airDamTransform;
    airDamTransform.x = (carBodySize.x * 0.5f) - (bumperFrontSize.x * 0.25f) - 0.05f;
    airDamTransform.y = wheelRadius + (-bumperFrontSize.y * 0.1f) + zFightOffSet + 0.05f;
    airDamTransform.z = 0.0;
    m_carModel.airDamMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(airDamTransform);
    m_carModel.localAirDamMatrix = m_carModel.airDamMatrix;
    /// Air Dam End ////////////////////////////////////////////////////////////////////////

    /// Back Bumper Start   ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 bumperBackSize = bumperFrontSize;
    bumperBackSize.y *= 0.8;
    m_carModel.bumperBack = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), bumperBackSize);
    m_carModel.bumperBackMatrix = m_carModel.bumperFrontMatrix;
    m_carModel.bumperBackMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-carBodySize.x + bumperBackSize.x * 0.5f, bumperBackSize.y * 0.15f, 0.0));
    m_carModel.localBumperBackMatrix = m_carModel.bumperBackMatrix;
    /// Back Bumper End    ////////////////////////////////////////////////////////////////

    /// Grill Start   ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 grillSize = hoodSize;
    grillSize.x = 0.09;
    grillSize.y *= 0.5;
    grillSize.z *= 0.9;
    m_carModel.grill = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), grillSize);
    m_carModel.grillMatrix = m_carModel.hoodMatrix;
    DirectX::SimpleMath::Vector3 grillTranslation((hoodSize.x * 0.5f), zFightOffSet, 0.0);
    grillTranslation.x = (hoodSize.x * 0.5f) - (grillSize.x * 0.3f);
    grillTranslation.y = bumperFrontSize.y * 0.45f;
    grillTranslation.z = 0.0;
    m_carModel.grillMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(grillTranslation);
    m_carModel.localGrillMatrix = m_carModel.grillMatrix;
    /// Grill End     ////////////////////////////////////////////////////////////////

    /// Grill Outline Start   ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 grillOutlineSize = grillSize;
    grillOutlineSize.x *= 0.9;
    grillOutlineSize.y *= 1.1;
    grillOutlineSize.z *= 1.05;
    m_carModel.grillOutline = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), grillOutlineSize);
    /// Grill Outline End     ////////////////////////////////////////////////////////////////

    /// Grill Logo Start /////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 grillLogoSize = grillSize;
    grillLogoSize.y *= 0.2;
    grillLogoSize.z = grillLogoSize.y;
    m_carModel.grillLogo = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), grillLogoSize);
    m_carModel.grillLogoMatrix = m_carModel.grillMatrix;
    m_carModel.grillLogoMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(zFightOffSet * 2.0, 0.0, 0.0));
    m_carModel.localGrillLogoMatrix = m_carModel.grillLogoMatrix;
    /// Grill Logo End ///////////////////////////////////////////////////////////////

    /// Grill Slat Start /////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 grillSlatSize = grillSize;
    grillSlatSize.y *= 0.04f;
    grillSlatSize.z *= 0.52f;
    m_carModel.grillSlat = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), grillSlatSize);
    m_carModel.grillSlatMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.grillSlatMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(28.0f));
    m_carModel.grillSlatMatrix *= m_carModel.grillLogoMatrix;
    m_carModel.grillSlatMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-zFightOffSet, 0.0f, 0.0f));
    m_carModel.localGrillSlatMatrix = m_carModel.grillSlatMatrix;
    /// Grill Slat End ///////////////////////////////////////////////////////////////
    
    /// Head Light Left Start  ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 headLightSize;
    headLightSize.x = bumperFrontSize.x * 0.5f;
    headLightSize.y = (carBodySize.y - bumperFrontSize.y) * 0.85f;
    headLightSize.z = (carBodySize.z - hoodSize.z) * 0.5f;

    m_carModel.headLight = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), headLightSize);

    m_carModel.headLightLeftMatrix = m_carModel.hoodMatrix;
    DirectX::SimpleMath::Vector3 headLightLeftTranslation;
    headLightLeftTranslation.x = (carBodySize.x * 0.5f) - (headLightSize.x * 0.9f);
    headLightLeftTranslation.y = (carBodySize.y * 0.5f) + (-headLightSize.y * 0.7f);
    headLightLeftTranslation.z = (- carBodySize.z * 0.5f) + (headLightSize.z * 0.45f);
    m_carModel.headLightLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(headLightLeftTranslation);
    m_carModel.localHeadLightLeftMatrix = m_carModel.headLightLeftMatrix;
    /// Head Light Left End    ////////////////////////////////////////////////////////////////

    /// Head Light Right Start  ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 headLightRightTranslation = headLightLeftTranslation;
    headLightRightTranslation.x -= headLightLeftTranslation.x;
    headLightRightTranslation.x = 0.0f;
    headLightRightTranslation.y = 0.0f;
    headLightRightTranslation.z *= -2.0f;

    m_carModel.headLightRightMatrix = m_carModel.headLightLeftMatrix;
    m_carModel.headLightRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(headLightRightTranslation);
    m_carModel.localHeadLightRightMatrix = m_carModel.headLightRightMatrix;
    /// Head Light Right End    ////////////////////////////////////////////////////////////////

    /// Blinker Light Lower Left Start ///////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 blinkerLightSize = headLightSize;
    blinkerLightSize.y *= 0.5f;
    blinkerLightSize.z *= 0.2f;
    m_carModel.blinkerLight = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), blinkerLightSize);
    DirectX::SimpleMath::Vector3 blinkerLeftTranslation;
    blinkerLeftTranslation.x = 0.001f;
    blinkerLeftTranslation.y = -blinkerLightSize.y * 0.5f;
    blinkerLeftTranslation.z = (-headLightSize.z * 0.5f) + (blinkerLightSize.z * 0.5f) - 0.001f;
    m_carModel.blinkerLightLowerLeftMatrix = m_carModel.headLightLeftMatrix;
    m_carModel.blinkerLightLowerLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(blinkerLeftTranslation);
    m_carModel.localBlinkerLightLowerLeftMatrix = m_carModel.blinkerLightLowerLeftMatrix;
    /// Blinker Light Lower Left End   ///////////////////////////////////////////////////////////////

    /// Blinker Light Upper Left Start ///////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 blinkerUpperLeftTranslation;
    blinkerUpperLeftTranslation.x = 0.001f;
    blinkerUpperLeftTranslation.y = +blinkerLightSize.y * 0.5f;
    blinkerUpperLeftTranslation.z = (-headLightSize.z * 0.5f) + (blinkerLightSize.z * 0.5f) - 0.001f;
    m_carModel.blinkerLightUpperLeftMatrix = m_carModel.headLightLeftMatrix;
    m_carModel.blinkerLightUpperLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(blinkerUpperLeftTranslation);
    m_carModel.localBlinkerLightUpperLeftMatrix = m_carModel.blinkerLightUpperLeftMatrix;
    /// Blinker Light Upper Left End   ///////////////////////////////////////////////////////////////
    
    /// Blinker Light Lower Right Start ///////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 blinkerRightTranslation = blinkerLeftTranslation;
    blinkerRightTranslation.z *= -1.0f;
    m_carModel.blinkerLightLowerRightMatrix = m_carModel.headLightRightMatrix;
    m_carModel.blinkerLightLowerRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(blinkerRightTranslation);
    m_carModel.localBlinkerLightLowerRightMatrix = m_carModel.blinkerLightLowerRightMatrix;
    /// Blinker Light Lower Right End  ///////////////////////////////////////////////////////////////

    /// Blinker Light Upper Right Start ///////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 blinkerUpperRightTranslation = blinkerUpperLeftTranslation;
    blinkerUpperRightTranslation.z *= -1.0f;
    m_carModel.blinkerLightUpperRightMatrix = m_carModel.headLightRightMatrix;
    m_carModel.blinkerLightUpperRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(blinkerUpperRightTranslation);
    m_carModel.localBlinkerLightUpperRightMatrix = m_carModel.blinkerLightUpperRightMatrix;
    /// Blinker Light Upper Right End  ///////////////////////////////////////////////////////////////
    
    /// Tail Light Right Start  ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 tailLightSize = headLightSize;
    tailLightSize.x *= 0.7f;
    tailLightSize.z = tailLightSize.x;
    m_carModel.tailLight = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), tailLightSize);
    m_carModel.tailLightRightMatrix = m_carModel.headLightRightMatrix;
    DirectX::SimpleMath::Vector3 tailLightRightTranslation;
    tailLightRightTranslation.x = -carBodySize.x + (tailLightSize.x * 0.999f);
    tailLightRightTranslation.y = 0.0f;
    tailLightRightTranslation.z = tailLightSize.z * 0.7f;
    m_carModel.tailLightRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(tailLightRightTranslation);
    m_carModel.localTailLightRightMatrix = m_carModel.tailLightRightMatrix;
    /// Tail Light Right End    ////////////////////////////////////////////////////////////////

    /// Tail Light Left Start  ////////////////////////////////////////////////////////////////
    m_carModel.tailLightLeftMatrix = m_carModel.headLightLeftMatrix;
    DirectX::SimpleMath::Vector3 tailLightLeftTranslation;
    tailLightLeftTranslation.x = -carBodySize.x + (tailLightSize.x * 0.999f);
    tailLightLeftTranslation.y = 0.0f;
    tailLightLeftTranslation.z = - tailLightSize.z * 0.7f;
    m_carModel.tailLightLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(tailLightLeftTranslation);
    m_carModel.localTailLightLeftMatrix = m_carModel.tailLightLeftMatrix;
    /// Tail Light Left End    ////////////////////////////////////////////////////////////////

    /// Reverse Light Right Start  ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 reverseLightSize = tailLightSize;
    reverseLightSize.x *= 1.005f;
    reverseLightSize.y *= 0.2f;
    reverseLightSize.z *= 1.005f;
    m_carModel.reverseLight = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), reverseLightSize);
    m_carModel.reverseLightRightMatrix = m_carModel.tailLightRightMatrix;
    DirectX::SimpleMath::Vector3 reverseLightTranslation;
    reverseLightTranslation.x = 0.0f;
    reverseLightTranslation.y = (reverseLightSize.y * 0.5f);
    reverseLightTranslation.z = 0.0f;
    m_carModel.reverseLightRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(reverseLightTranslation);
    m_carModel.localReverseLightRightMatrix = m_carModel.reverseLightRightMatrix;
    /// Reverse Light Right End    ////////////////////////////////////////////////////////////////

    /// Reverse Light Left Start  ////////////////////////////////////////////////////////////////
    m_carModel.reverseLightLeftMatrix = m_carModel.tailLightLeftMatrix;
    m_carModel.reverseLightLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(reverseLightTranslation);
    m_carModel.localReverseLightLeftMatrix = m_carModel.reverseLightLeftMatrix;
    /// Reverse Light Left End    ////////////////////////////////////////////////////////////////

    /// Tail Blinker Light Right Start  ////////////////////////////////////////////////////////////////
    m_carModel.tailBlinkerLightRightMatrix = m_carModel.tailLightRightMatrix;
    DirectX::SimpleMath::Vector3 tailBlinkerLightTranslation;
    tailBlinkerLightTranslation.x = 0.0f;
    tailBlinkerLightTranslation.y = - (reverseLightSize.y * 0.5f);
    tailBlinkerLightTranslation.z = 0.0f;
    m_carModel.tailBlinkerLightRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(tailBlinkerLightTranslation);
    m_carModel.localTailBlinkerLightRightMatrix = m_carModel.tailBlinkerLightRightMatrix;
    /// Tail Blinker Light Right End    ////////////////////////////////////////////////////////////////

    /// Tail Blinker Light Left Start  ////////////////////////////////////////////////////////////////
    m_carModel.tailBlinkerLightLeftMatrix = m_carModel.tailLightLeftMatrix;
    m_carModel.tailBlinkerLightLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(tailBlinkerLightTranslation);
    m_carModel.localTailBlinkerLightLeftMatrix = m_carModel.tailBlinkerLightLeftMatrix;
    /// Tail Blinker Light Left End    ////////////////////////////////////////////////////////////////

    /// Third Brake Light Start   ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 thirdBrakeLightSize;
    thirdBrakeLightSize.x = 0.10f;
    thirdBrakeLightSize.y = 0.09f;
    thirdBrakeLightSize.z = .15f;
    DirectX::SimpleMath::Vector3 thirdBrakeLightTranslation;
    thirdBrakeLightTranslation.x = - (carBodySize.x * 0.5f) + thirdBrakeLightSize.x;
    thirdBrakeLightTranslation.y = height + (thirdBrakeLightSize.y * 0.5f);
    thirdBrakeLightTranslation.z = 0.0f;
    m_carModel.thirdBrakeLight = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), thirdBrakeLightSize);
    m_carModel.thirdBrakeLightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.thirdBrakeLightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(thirdBrakeLightTranslation);
    m_carModel.localThirdBrakeLightMatrix = m_carModel.thirdBrakeLightMatrix;
    /// Third Brake Light End     ////////////////////////////////////////////////////////////////

    /// License Plate Rear Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 licensePlateRearSize = carBodySize;
    licensePlateRearSize.x = 0.02f;
    licensePlateRearSize.y *= 0.2f;
    licensePlateRearSize.z *= 0.25f;
    m_carModel.licensePlateRear = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), licensePlateRearSize);
    DirectX::SimpleMath::Vector3 licensePlateRearTranslation;
    licensePlateRearTranslation.x = -(carBodySize.x * 0.5f);
    licensePlateRearTranslation.y = (bodyTranslation.y * 0.5f) + (bumperBackSize.y * 0.5f) + (licensePlateRearSize.y * 0.5f);
    licensePlateRearTranslation.z = 0.0f;
    m_carModel.licensePlateRearMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.licensePlateRearMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(licensePlateRearTranslation);
    m_carModel.localLicensePlateRearMatrix = m_carModel.licensePlateRearMatrix;
    /// License Plate Rear End  ////////////////////////////////////////////////////////////////

    /// License Plate Rear Outline Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 licensePlateRearOutlineSize = licensePlateRearSize;
    licensePlateRearOutlineSize.x *= 1.03f;
    licensePlateRearOutlineSize.y *= 1.3f;
    licensePlateRearOutlineSize.z *= 1.05f;
    m_carModel.licensePlateRearOutline = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), licensePlateRearOutlineSize);
    /// License Plate Rear Outline End  ////////////////////////////////////////////////////////////////

    /// Side Mirror Left Start ///////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 sideMirrorSize;
    sideMirrorSize.x = 0.13f;
    sideMirrorSize.y = 0.144f;
    sideMirrorSize.z = 0.25f;
    m_carModel.sideMirror = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), sideMirrorSize);
    m_carModel.sideMirrorLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    const float sideMirrorAngle = Utility::ToRadians(20.0f);
    m_carModel.sideMirrorLeftMatrix *= DirectX::SimpleMath::Matrix::CreateRotationY(sideMirrorAngle);
    DirectX::SimpleMath::Vector3 sideMirrorLeftTranslation;
    sideMirrorLeftTranslation.x = (windShieldAllignment.x * 0.5f) + triangleWindowSize.x * 0.051f;
    sideMirrorLeftTranslation.y = windShieldAllignment.y * 0.5f;
    sideMirrorLeftTranslation.z = (- carBodySize.z * 0.5f) - (sideMirrorSize.z * 0.5f) + (topIndent * 0.2f);
    m_carModel.sideMirrorLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(sideMirrorLeftTranslation);
    m_carModel.localSideMirrorLeftMatrix = m_carModel.sideMirrorLeftMatrix;
    /// Side Mirror Left End ////////////////////////////////////////////////////////////////

    /// Side Mirror Right Start ///////////////////////////////////////////////////////////////
    m_carModel.sideMirrorRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.sideMirrorRightMatrix *= DirectX::SimpleMath::Matrix::CreateRotationY(-sideMirrorAngle);
    DirectX::SimpleMath::Vector3 sideMirrorRightTranslation;
    sideMirrorRightTranslation.x = (windShieldAllignment.x * 0.5f) + triangleWindowSize.x * 0.051f;
    sideMirrorRightTranslation.y = windShieldAllignment.y * 0.5f;
    sideMirrorRightTranslation.z = (carBodySize.z * 0.5f) + (sideMirrorSize.z * 0.5f) - (topIndent * 0.2f);
    m_carModel.sideMirrorRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(sideMirrorRightTranslation);
    m_carModel.localSideMirrorRightMatrix = m_carModel.sideMirrorRightMatrix;
    /// Side Mirror Right End ////////////////////////////////////////////////////////////////

    /// Pinstripe Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 pinstripeSize = carBodySize;
    pinstripeSize.x *= 0.88f;
    pinstripeSize.y *= 0.035f;
    pinstripeSize.z *= 1.01f;
    m_carModel.pinstripe = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), pinstripeSize);
    DirectX::SimpleMath::Vector3 pinStripeTranslation;
    pinStripeTranslation.x = -(carBodySize.x * 0.0145f);
    pinStripeTranslation.y = (carBodySize.y * 0.4f);
    pinStripeTranslation.z = 0.0f;
    m_carModel.pinstripeMatrix = m_carModel.bodyMatrix;
    m_carModel.pinstripeMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(pinStripeTranslation);
    m_carModel.localPinstripeMatrix = m_carModel.pinstripeMatrix;
    /// Pinstripe End  ////////////////////////////////////////////////////////////////

    /// Fender Flare Front Left Start ////////////////////////////////////////////////////////////////
    float fenderFlareHeight = m_car.wheelRadius * 2.005f;
    float fenderFlareWidth = m_car.wheelWidth * 0.3f;

    m_carModel.fenderFlare = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), fenderFlareWidth, fenderFlareHeight, 32);
    DirectX::SimpleMath::Vector3 fenderFlareFrontLeftTranslation;
    fenderFlareFrontLeftTranslation.x = 0.0f;
    fenderFlareFrontLeftTranslation.y = (fenderFlareHeight * 0.07f);
    fenderFlareFrontLeftTranslation.z = -fenderFlareWidth * 0.4f;
    fenderFlareFrontLeftTranslation.z = fenderFlareWidth * 0.31f;

    m_carModel.fenderFlareFrontLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.fenderFlareFrontLeftMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(-9.0f));
    m_carModel.fenderFlareFrontLeftMatrix *= m_carModel.wheelFrontLeftMatrix;
    m_carModel.fenderFlareFrontLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(fenderFlareFrontLeftTranslation);
    m_carModel.localfenderFlareFrontLeftMatrix = m_carModel.fenderFlareFrontLeftMatrix;

    m_carModel.fenderFlareInterior = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), fenderFlareWidth, fenderFlareHeight, 32);

    m_carModel.fenderFlareFrontLeftInteriorMatrix = m_carModel.fenderFlareFrontLeftMatrix;
    m_carModel.fenderFlareFrontLeftInteriorMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, -zFightOffSet * 10.0f, -zFightOffSet));
    m_carModel.localfenderFlareFrontLeftInteriorMatrix = m_carModel.fenderFlareFrontLeftInteriorMatrix;
    /// Fender Flare Front Left End  ////////////////////////////////////////////////////////////////

    /// Fender Flare Front Right Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 fenderFlareFrontRightTranslation;
    fenderFlareFrontRightTranslation.x = 0.0f;
    fenderFlareFrontRightTranslation.y = (fenderFlareHeight * 0.07f);
    fenderFlareFrontRightTranslation.z = -fenderFlareWidth * 0.31f;

    m_carModel.fenderFlareFrontRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.fenderFlareFrontRightMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(9.0));
    m_carModel.fenderFlareFrontRightMatrix *= m_carModel.wheelFrontRightMatrix;
    m_carModel.fenderFlareFrontRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(fenderFlareFrontRightTranslation);
    m_carModel.localfenderFlareFrontRightMatrix = m_carModel.fenderFlareFrontRightMatrix;

    m_carModel.fenderFlareFrontRightInteriorMatrix = m_carModel.fenderFlareFrontRightMatrix;
    m_carModel.fenderFlareFrontRightInteriorMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -zFightOffSet * 10.0f, zFightOffSet));
    m_carModel.localfenderFlareFrontRightInteriorMatrix = m_carModel.fenderFlareFrontRightInteriorMatrix;
    /// Fender Flare Front Right End  ////////////////////////////////////////////////////////////////

    /// Fender Flare Rear Left Start ////////////////////////////////////////////////////////////////
    float fenderRearFlareHeight = m_car.wheelRadius * 2.005f;
    float fenderRearFlareWidth = m_car.wheelWidth * 0.55f;

    m_carModel.fenderRearFlare = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), fenderRearFlareWidth, fenderRearFlareHeight, 32);
    m_carModel.fenderRearFlareInterior = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), fenderRearFlareWidth, fenderRearFlareHeight, 32);

    DirectX::SimpleMath::Vector3 fenderFlareRearLeftTranslation;
    fenderFlareRearLeftTranslation.x = 0.0f;
    fenderFlareRearLeftTranslation.y = (fenderFlareHeight * 0.07f);
    fenderFlareRearLeftTranslation.z = fenderFlareWidth * 0.00f;

    m_carModel.fenderFlareRearLeftMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.fenderFlareRearLeftMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(-14.0f));
    m_carModel.fenderFlareRearLeftMatrix *= m_carModel.wheelRearLeftMatrix;
    m_carModel.fenderFlareRearLeftMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(fenderFlareRearLeftTranslation);
    m_carModel.localfenderFlareRearLeftMatrix = m_carModel.fenderFlareRearLeftMatrix;

    m_carModel.fenderFlareRearLeftInteriorMatrix = m_carModel.fenderFlareRearLeftMatrix;
    m_carModel.fenderFlareRearLeftInteriorMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, -zFightOffSet * 10.0f, -zFightOffSet));
    m_carModel.localfenderFlareRearLeftInteriorMatrix = m_carModel.fenderFlareRearLeftInteriorMatrix;
    /// Fender Flare Rear Left End  ////////////////////////////////////////////////////////////////

    /// Fender Flare Rear Right Start ////////////////////////////////////////////////////////////////
    DirectX::SimpleMath::Vector3 fenderFlareRearRightTranslation;
    fenderFlareRearRightTranslation.x = 0.0f;
    fenderFlareRearRightTranslation.y = (fenderFlareHeight * 0.07f);
    fenderFlareRearRightTranslation.z = -fenderFlareWidth * 0.00f;

    m_carModel.fenderFlareRearRightMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.fenderFlareRearRightMatrix *= DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(14.0f));
    m_carModel.fenderFlareRearRightMatrix *= m_carModel.wheelRearRightMatrix;
    m_carModel.fenderFlareRearRightMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(fenderFlareRearRightTranslation);
    m_carModel.localfenderFlareRearRightMatrix = m_carModel.fenderFlareRearRightMatrix;

    m_carModel.fenderFlareRearRightInteriorMatrix = m_carModel.fenderFlareRearRightMatrix;
    m_carModel.fenderFlareRearRightInteriorMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -zFightOffSet * 10.0f, zFightOffSet));
    m_carModel.localfenderFlareRearRightInteriorMatrix = m_carModel.fenderFlareRearRightInteriorMatrix;
    /// Fender Flare Rear Right End  ////////////////////////////////////////////////////////////////

    // test powered wheel start
    m_carModel.testWheel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), rimWidth, wheelDiameter, 8);
    m_carModel.testLocalwheelMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.testLocalwheelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -m_car.wheelWidth * 1.5f, 0.0f));
    m_carModel.testWorldWheelMatrix = m_carModel.testLocalwheelMatrix;
    // test powered wheel end
}

void Vehicle::InitializeVehicle(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext)
{
    // roughly based on porsche boxster
    m_car.mass = 1393.0f;
    m_car.area = 1.94f;
    m_car.density = 1.2f;
    m_car.Cd = 0.31f;
    m_car.redline = 7200.f;
    m_car.revlimit = 7800.f;
    m_car.finalDriveRatio = 3.44f;
    m_car.wheelRadius = 0.3186f;
    m_car.wheelWidth = 0.235f;
    m_car.numberOfGears = 7;
    m_car.muR = 0.015f;             //  coefficient of rolling friction
    m_car.airResistance = 0.0f;
    //m_car.airDensity = 1.225; // ToDo : pull air density from environment data
    m_car.totalResistance = m_car.muR + m_car.airResistance;
    m_car.omegaE = 1000.0;         //  engine rpm
    m_car.gearNumber = 2;          //  gear the car is in
    m_car.gearRatio[0] = -3.82f;
    m_car.gearRatio[1] = 0.0f;
    m_car.gearRatio[2] = 3.82f;
    m_car.gearRatio[3] = 2.20f;
    m_car.gearRatio[4] = 1.52f;
    m_car.gearRatio[5] = 1.22f;
    m_car.gearRatio[6] = 1.02f;
    m_car.gearRatio[7] = 0.84f;
    m_car.wheelMass = 22.68f;    // Total guess, not confirmed with porsche or volvo stats

    m_car.gravity = DirectX::SimpleMath::Vector3(0.0, -9.81, 0.0);
    m_car.numEqns = 6;
    m_car.time = 0.0;  
    m_car.q.position = DirectX::SimpleMath::Vector3::Zero;
    m_car.q.velocity = DirectX::SimpleMath::Vector3::Zero;
    m_car.q.bodyVelocity = DirectX::SimpleMath::Vector3::Zero;
    m_car.q.brakeForce = DirectX::SimpleMath::Vector3::Zero;

    m_car.q.slopeForce = DirectX::SimpleMath::Vector3::Zero;
    m_car.q.airResistance = DirectX::SimpleMath::Vector3::Zero;
    m_car.q.gravityForce = DirectX::SimpleMath::Vector3::Zero;

    m_car.q.engineForce = DirectX::SimpleMath::Vector3::Zero;
    m_car.q.totalVelocity = DirectX::SimpleMath::Vector3::Zero;

    m_car.inputDeadZone = 0.05;
    m_car.throttleInput = 0.0;
    m_car.brakeInput = 0.0;
    m_car.maxThrottleInput = 1.0;
    m_car.maxThrottleRate = 1.0;
    m_car.maxBrakeInput = 1.0;
    m_car.maxBrakeRate = 15.0;

    m_car.brakeDecayRate = 1.2;
    m_car.throttleDecayRate = 1.2;
    m_car.steeringAngleDecay = -0.3;
    m_car.steeringSpeed = 0.1;

    m_car.carRotation = 0.0;
    m_car.steeringAngle = Utility::ToRadians(0.0);
    m_car.steeringAngleMax = Utility::ToRadians(26.0);
    m_car.headingVec = -DirectX::SimpleMath::Vector3::UnitZ;
    m_car.forward = DirectX::SimpleMath::Vector3::UnitX;
    m_car.up = DirectX::SimpleMath::Vector3::UnitY;
    m_car.right = m_car.forward.Cross(m_car.up);
    m_car.shiftCooldown = 0.0;
    m_car.shiftDelay = 0.5;
    m_car.speed = 0.0;

    m_car.isClutchPressed = false;
    m_car.isThrottlePressed = false;
    m_car.isBrakePressed = false;
    m_car.isTurningPressed = false;
    m_car.isAccelerating = false;
    m_car.isBraking = false;
    m_car.isRevlimitHit = false;
    m_car.isTransmissionManual = true;
    m_car.isCarAirborne = false;
    m_car.isCarLanding = false;
    m_car.isVelocityBackwards = false;
    m_car.wheelBase = 2.41;

    m_car.terrainHightAtPos = 0.0;
    // test values for wheel slip
    m_car.testRearCylinderMass = 75.0;
    m_car.testTorque = 0.0;
    m_car.testRearAnglularVelocity = 0.0;
    m_car.testRearAngularVelocityAngle = 0.0;

    m_car.terrainNormal = DirectX::SimpleMath::Vector3::UnitY;
    m_car.testModelPos = m_car.q.position;
    m_car.testModelRotation = m_car.carRotation;
    m_car.testTerrainNormal = m_car.terrainNormal;
    m_car.testHeadingVec = m_car.headingVec;

    m_car.testHeading1 = DirectX::SimpleMath::Vector3::UnitX;
    m_car.testHeading2 = DirectX::SimpleMath::Vector3::UnitZ;
    m_car.testHeading2 = - DirectX::SimpleMath::Vector3::UnitX;
    InitializeModel(aContext);

    InitializeWheels();
}

void Vehicle::InitializeWheels()
{
    m_wheels.resize(m_numberOfWheels);
    
    // front left wheel 
    DirectX::SimpleMath::Vector3 wheelLocPos = DirectX::SimpleMath::Vector3::Zero;
    wheelLocPos = wheelLocPos.Transform(wheelLocPos, m_carModel.wheelFrontLeftTranslation);
    DirectX::SimpleMath::Vector3 wheelWorldPos = wheelLocPos + m_car.q.position;
    m_wheels[0].InitializeWheel(wheelLocPos, wheelWorldPos, true, m_car.wheelRadius, m_car.wheelWidth, m_car.wheelMass);

    // front right wheel
    wheelLocPos = wheelLocPos.Transform(DirectX::SimpleMath::Vector3::Zero, m_carModel.wheelFrontRightTranslation);
    wheelWorldPos = wheelLocPos + m_car.q.position;
    m_wheels[1].InitializeWheel(wheelLocPos, wheelWorldPos, true, m_car.wheelRadius, m_car.wheelWidth, m_car.wheelMass);

    // rear right wheel
    wheelLocPos = wheelLocPos.Transform(DirectX::SimpleMath::Vector3::Zero, m_carModel.wheelRearRightTranslation);
    wheelWorldPos = wheelLocPos + m_car.q.position;
    m_wheels[2].InitializeWheel(wheelLocPos, wheelWorldPos, true, m_car.wheelRadius, m_car.wheelWidth, m_car.wheelMass);

    // rear left wheel
    wheelLocPos = wheelLocPos.Transform(DirectX::SimpleMath::Vector3::Zero, m_carModel.wheelRearLeftTranslation);
    wheelWorldPos = wheelLocPos + m_car.q.position;
    m_wheels[3].InitializeWheel(wheelLocPos, wheelWorldPos, true, m_car.wheelRadius, m_car.wheelWidth, m_car.wheelMass);
}

void Vehicle::Jump(double aTimer)
{
    float jumpHeight = 1000.0;
    m_car.q.position.y += jumpHeight;
}

void Vehicle::LandVehicle()
{   
    DirectX::SimpleMath::Vector3 terrainNorm = m_car.terrainNormal;
    DirectX::SimpleMath::Vector3 landingVelocity = m_car.q.velocity;
    DirectX::SimpleMath::Vector3 landingVelocityNorm = landingVelocity;
    landingVelocityNorm.Normalize();

    float impact = landingVelocityNorm.Dot(m_car.terrainNormal);

    DirectX::SimpleMath::Vector3 updateVelocity;
    updateVelocity.x = m_car.q.velocity.x * abs(impact);
    updateVelocity.y = m_car.q.velocity.y * abs(impact);
    updateVelocity.z = m_car.q.velocity.z * abs(impact);

    updateVelocity = m_car.q.velocity * -impact;
    updateVelocity = m_car.q.velocity;
    updateVelocity.y = 0.0;
    
    ///////////////////////////////
    // Testing new landing equation, from golf ball
    //  ::: Vnew = b * (-2 * (V dot N) * N + V)
    // const float b = .3f;
    // DirectX::SimpleMath::Vector3 terrainNorm = pBallEnvironment->GetTerrainNormal(GetBallPosInEnviron(m_ball.q.position));
    // m_ball.q.velocity = b * (-2 * (preVelocity.Dot(terrainNorm)) * terrainNorm + preVelocity);
    DirectX::SimpleMath::Vector3 v = m_car.q.velocity;
    DirectX::SimpleMath::Vector3 n = m_car.terrainNormal;
    const float b = .9f;

    DirectX::SimpleMath::Vector3 testUpdateVel = b * (-2 * (v.Dot(n)) * n + v);
    testUpdateVel.y = 0.0;

    m_car.q.velocity = testUpdateVel;
}

void Vehicle::PressBrake(const float aBrakeInput)
{
    m_testIsBreakLightOn = true;
    //if(m_car.brakeInput > 0.0)
    if (aBrakeInput > 0.0)
    {
        m_car.isBrakePressed = true;
    }
    float updatedBrake = aBrakeInput + m_car.brakeInput;
    if (updatedBrake > m_car.maxBrakeInput)
    {
        m_car.brakeInput = m_car.maxBrakeInput;
    }
    else
    {
        m_car.brakeInput += aBrakeInput;
    }
}

void Vehicle::PressClutch(const bool aClutchInput)
{
    m_car.isClutchPressed = aClutchInput;
}

void Vehicle::PressThrottle(const float aThrottleInput)
{
    if (aThrottleInput > 0.0)
    {
        m_car.isThrottlePressed = true;
    }
    float updatedThrottle = aThrottleInput + m_car.throttleInput;
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
    m_car.headingVec = DirectX::SimpleMath::Vector3::UnitX;
    m_car.forward = DirectX::SimpleMath::Vector3::UnitX;
    m_car.up = DirectX::SimpleMath::Vector3::UnitY;
    m_car.right = DirectX::SimpleMath::Vector3::UnitZ;
    m_car.speed = 0.0;
    m_car.q.velocity = DirectX::SimpleMath::Vector3::Zero;
}

void Vehicle::RevLimiter()
{
    if (m_car.omegaE < m_car.redline)
    {
        m_car.isRevlimitHit = false;
    }
    if (m_car.omegaE > m_car.revlimit)
    {
        m_car.isRevlimitHit = true;
    }
    if (m_car.isRevlimitHit == true)
    {
        m_car.isThrottlePressed = false;
        m_car.throttleInput = 0.0;
    }
}

//*************************************************************
//  This method loads the right-hand sides for the car ODEs
//*************************************************************
void Vehicle::RightHandSide(struct Car* aCar, Motion* aQ, Motion* aDeltaQ, double aTimeDelta, float aQScale, Motion* aDQ)
{
    //  Compute the intermediate values of the 
    //  dependent variables.
    Motion newQ;
    newQ.velocity = aQ->velocity + static_cast<float>(aQScale) * aDeltaQ->velocity;
    newQ.position = aQ->position + static_cast<float>(aQScale) * aDeltaQ->position;

    //  Compute the constants that define the
    //  torque curve line.
    float powerCurve;
    float torque; // in newton meters
    float omegaE = aCar->omegaE;

    if (omegaE <= 1000.0)
    {
        powerCurve = 0.0;
        torque = 220.0;
    }
    else if (omegaE < 4600.0)
    {
        powerCurve = 0.025;
        torque = 195.0;
    }
    else
    {
        powerCurve = -0.032;
        torque = 457.2;
    }

    if (m_isFuelOn == false)
    {
        torque = 0.0;
    }

    //  Compute the velocity magnitude. The 1.0e-8 term
    //  ensures there won't be a divide by zero later on
    //  if all of the velocity components are zero.
    float v = sqrt(newQ.velocity.Length() * newQ.velocity.Length()) + 1.0e-8f;

    //  Compute the total drag force.
    float airDensity = aCar->density;
    float dragCoefficient = aCar->Cd;
    float frontSurfaceArea = aCar->area;
    float frontDragResistance = 0.5f * airDensity * frontSurfaceArea * dragCoefficient * v * v;

    //  Compute the force of rolling friction. Because
    //  the G constant has a negative sign, the value 
    //  computed here will be negative
    float gravity = aCar->gravity.y;
    float rollingFrictionCoefficient = aCar->muR;
    float mass = aCar->mass;
    float rollingFriction = rollingFrictionCoefficient * mass * gravity;
    rollingFriction = rollingFrictionCoefficient * mass * (m_car.terrainNormal * m_car.gravity).y;
    //  Compute the right-hand sides of the six ODEs
    //  newQ[0] is the intermediate value of velocity.
    //  The acceleration of the car is determined by 
    //  whether the car is accelerating, cruising, or
    //  braking.   
    // Accelerating
    int gearNumber = aCar->gearNumber;
    float gearRatio = aCar->gearRatio[gearNumber];
    float finalDriveRatio = aCar->finalDriveRatio;
    float wheelRadius = aCar->wheelRadius;
    float pi = acos(-1.0f);

    DirectX::SimpleMath::Vector3 headingVec = m_car.forward;

    float c1 = -frontDragResistance / mass;
    float tmp = gearRatio * finalDriveRatio / wheelRadius;
    float testTmp = ((gearRatio * aCar->throttleInput) * finalDriveRatio) / wheelRadius;
    tmp = testTmp;
    float c2 = 60.0f * tmp * tmp * powerCurve * v / (2.0f * pi * mass);
    float c3 = (tmp * torque + rollingFriction) / mass;
    float c4 = headingVec.Dot(m_car.terrainNormal * m_car.gravity);

    DirectX::SimpleMath::Vector3 velocityUpdate = (static_cast<float>(aTimeDelta) * (c1 + c2 + c3 + c4)) * headingVec;

    DirectX::SimpleMath::Vector3 slopeForce = m_car.forward * m_car.forward.Dot(m_car.terrainNormal * m_car.gravity);
    slopeForce = slopeForce * static_cast<float>(aTimeDelta);

    DirectX::SimpleMath::Vector3 velocityNorm = m_car.q.velocity;
    velocityNorm.Normalize();

    DirectX::SimpleMath::Vector3 airResistance = velocityNorm * (static_cast<float>(aTimeDelta) * (-frontDragResistance / mass));
    DirectX::SimpleMath::Vector3 brakeForce = (static_cast<float>(aTimeDelta) * ((-aCar->brakeInput * aCar->maxBrakeRate))) * velocityNorm;
    if (m_car.isVelocityBackwards == true)
    {
        rollingFriction *= -1.0;
    }

    DirectX::SimpleMath::Vector3 engineForce = (static_cast<float>(aTimeDelta) * (c2 + ((tmp * torque + rollingFriction) / mass))) * headingVec;
    DirectX::SimpleMath::Vector3 rollingResistance = (static_cast<float>(aTimeDelta) * (rollingFriction) / mass) * velocityNorm;

    float testEngineForce = (static_cast<float>(aTimeDelta) * (c2 + ((tmp * torque + rollingFriction) / mass)));
    testEngineForce = (static_cast<float>(aTimeDelta) * (c2 + ((tmp * torque) / mass)));
    testEngineForce = (static_cast<float>(aTimeDelta) * (c2 + ((tmp * torque))));
    testEngineForce = (static_cast<float>(aTimeDelta) * (((tmp * torque))));

    if (testEngineForce > m_testEnginePower)
    {
        m_testEnginePower = testEngineForce;
    }

    if (aCar->isClutchPressed == true)
    {
        engineForce = (static_cast<float>(aTimeDelta) * (c2 + ((rollingFriction) / mass))) * headingVec;
    }

    velocityUpdate = (static_cast<float>(aTimeDelta) * (c1 + c2 + c3 + c4)) * headingVec;
    if (m_car.throttleInput > 0.0 || m_isFuelOn == false)
    {
        aDQ->velocity = velocityUpdate;
    }
    else if (m_car.brakeInput > 0.0)  // braking 
    {
        //  Only brake if the velocity is positive.
        if (newQ.velocity.Length() > 0.1)
        {
            velocityUpdate = ((static_cast<float>(aTimeDelta) * (c1 + c2 + c3 + c4)) * headingVec) + brakeForce;
            aDQ->velocity = ((static_cast<float>(aTimeDelta) * (c1 + c2 + c3 + c4)) * headingVec) + brakeForce;

        }
        else
        {
            velocityUpdate = DirectX::SimpleMath::Vector3::Zero;
            aDQ->velocity = DirectX::SimpleMath::Vector3::Zero;
        }
    }
    else  // cruise
    {     
        if (newQ.velocity.Length() < 0.000001 && m_car.throttleInput < 0.01)
        {
            velocityUpdate = DirectX::SimpleMath::Vector3::Zero;
            aDQ->velocity = DirectX::SimpleMath::Vector3::Zero;
            velocityUpdate = (static_cast<float>(aTimeDelta) * (c1 + c4)) * headingVec;
        }
        else
        {
            velocityUpdate = (static_cast<float>(aTimeDelta) * (c1 + c2 + c3 + c4)) * headingVec;
            velocityUpdate = (static_cast<float>(aTimeDelta) * (c1  + c3 + c4)) * headingVec;
            aDQ->velocity = (static_cast<float>(aTimeDelta) * (c1 + c2 + c3 + c4)) * headingVec;
            velocityUpdate = (static_cast<float>(aTimeDelta) * (c1 + c4)) * headingVec;
        }        
    }
    
    DirectX::SimpleMath::Vector3 gravForce = m_car.gravity * static_cast<float>(aTimeDelta);
    DirectX::SimpleMath::Vector3 terrainNormalForce = (m_car.terrainNormal * -m_car.gravity.y) * static_cast<float>(aTimeDelta);

    if (m_car.isCarAirborne == true)
    {
        terrainNormalForce = DirectX::SimpleMath::Vector3::Zero;
    }

    float gravDown = gravForce.y + terrainNormalForce.y;
    
    velocityUpdate = engineForce + brakeForce + slopeForce + airResistance;
    velocityUpdate.y += gravDown;
    
    if (m_car.shiftCooldown > 0.0)
    {
        velocityUpdate =  brakeForce + slopeForce + airResistance + rollingResistance;
    }

    if (m_car.isCarAirborne == true)
    {
        velocityUpdate = m_car.gravity;
        velocityUpdate.x = 0.0;
        velocityUpdate.z = 0.0;
        velocityUpdate.y = gravity * static_cast<float>(aTimeDelta);
        gravForce = m_car.gravity * static_cast<float>(aTimeDelta);

    }

    if (m_car.isCarLanding == true)
    {
        //velocityUpdate.y = -m_car.q.velocity.y;
        //velocityUpdate = DirectX::SimpleMath::Vector3::Zero;
    }

    //  Compute right-hand side values.
    //aDQ->engineForce = engineForce;
    aDQ->engineForce = testEngineForce * headingVec;
   
    aDQ->brakeForce = brakeForce;
    aDQ->slopeForce = slopeForce;
    aDQ->airResistance = airResistance;
    aDQ->gravityForce = gravForce;
    aDQ->velocity = velocityUpdate;
    aDQ->totalVelocity = velocityUpdate;
  
    aDQ->position = static_cast<float>(aTimeDelta) * newQ.velocity;

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
    // int numEqns = aCar->numEqns;
    const float numEqns = static_cast<float>(aCar->numEqns);
    //  Retrieve the current values of the dependent
    //  and independent variables.
    Motion q = aCar->q;

    Motion dq1;
    Motion dq2;
    Motion dq3;
    Motion dq4;

    if (m_car.isCarLanding == true)
    {
        //q.velocity.y = 0.0;
        //q.velocity.x = 0.0;
        //q.velocity *= -1;
        //q.velocity = DirectX::SimpleMath::Vector3::Zero;

        //aCar->q.velocity = DirectX::SimpleMath::Vector3::Zero;
    }
    if (m_car.isCarAirborne == false)
    {
        m_car.q.gravityForce = DirectX::SimpleMath::Vector3::Zero;
    }

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
    DirectX::SimpleMath::Vector3 bodyVelocityyUpdate = (dq1.bodyVelocity + 2.0 * dq2.bodyVelocity + 2.0 * dq3.bodyVelocity + dq4.bodyVelocity) / numEqns;
    DirectX::SimpleMath::Vector3 engineVelocityUpdate = (dq1.engineForce + 2.0 * dq2.engineForce + 2.0 * dq3.engineForce + dq4.engineForce) / numEqns;
    DirectX::SimpleMath::Vector3 brakeVelocityUpdate = (dq1.brakeForce + 2.0 * dq2.brakeForce + 2.0 * dq3.brakeForce + dq4.brakeForce) / numEqns;
    DirectX::SimpleMath::Vector3 slopeVelocityUpdate = (dq1.slopeForce + 2.0 * dq2.slopeForce + 2.0 * dq3.slopeForce + dq4.slopeForce) / numEqns;
    DirectX::SimpleMath::Vector3 airResistnaceVelocityUpdate = (dq1.airResistance + 2.0 * dq2.airResistance + 2.0 * dq3.airResistance + dq4.airResistance) / numEqns;
    DirectX::SimpleMath::Vector3 gravityVelocityUpdate = (dq1.gravityForce + 2.0 * dq2.gravityForce + 2.0 * dq3.gravityForce + dq4.gravityForce) / numEqns;
    DirectX::SimpleMath::Vector3 totalVelocityUpdate = (dq1.totalVelocity + 2.0 * dq2.totalVelocity + 2.0 * dq3.totalVelocity + dq4.totalVelocity) / numEqns;

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
    q.velocity += velocityUpdate;

    q.engineForce += engineVelocityUpdate;
    q.brakeForce += brakeVelocityUpdate;
    q.airResistance += airResistnaceVelocityUpdate;
    q.gravityForce += gravityVelocityUpdate;
    q.slopeForce += slopeVelocityUpdate;
    q.totalVelocity += velocityUpdate;

    aCar->q.engineForce = q.engineForce;
    aCar->q.position = q.position;
    aCar->q.velocity = q.velocity;

    aCar->q = q;

    return;
}

void Vehicle::SetEnvironment(Environment* aEnviron)
{
    m_environment = aEnviron;
}

void Vehicle::SteeringInputDecay(const double aTimeDelta)
{
    if (m_car.isTurningPressed == false)
    {
        const float timeDelta = static_cast<float>(aTimeDelta);
        if (m_car.steeringAngle != 0.0)
        {
            if (m_car.steeringAngle > 0.0)
            {
                if (m_car.steeringAngle + (m_car.steeringAngleDecay * timeDelta) < 0.0)
                {
                    m_car.steeringAngle = 0.0;
                }
                else
                {
                    m_car.steeringAngle += m_car.steeringAngleDecay * timeDelta;
                }
            }
            else
            {
                if (m_car.steeringAngle - (m_car.steeringAngleDecay * timeDelta) > 0.0)
                {
                    m_car.steeringAngle = 0.0;
                }
                else
                {
                    m_car.steeringAngle -= m_car.steeringAngleDecay * timeDelta;
                }
            }
        }
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

void Vehicle::ToggleFuel()
{
    if (m_isFuelOn == true)
    {
        m_isFuelOn = false;
    }
    else
    {
        m_isFuelOn = true;
    }
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

void Vehicle::TurnInput(float aTurnInput)
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

void Vehicle::TurnVehicle(double aTimeDelta)
{  
    if (m_car.isVelocityBackwards == false)
    {
        m_car.carRotation -= GetYawRate(aTimeDelta);
    }
    else
    {
        m_car.carRotation += GetYawRate(aTimeDelta);
    }
    m_car.carRotation = Utility::WrapAngle(m_car.carRotation);
}

void Vehicle::ThrottleBrakeDecay(const double aTimeDelta)
{
    const float timeDelta = static_cast<float>(aTimeDelta);
    if (m_car.isThrottlePressed == false)
    {
        if (m_car.throttleInput - (m_car.throttleDecayRate * timeDelta) < 0.0)
        {
            m_car.throttleInput = 0.0;
        }
        else
        {
            m_car.throttleInput -= m_car.throttleDecayRate * timeDelta;
        }
    }

    if (m_car.isBrakePressed == false)
    {
        if (m_car.brakeInput - (m_car.brakeDecayRate * timeDelta) < 0.0)
        {
            m_car.brakeInput = 0.0;
        }
        else
        {
            m_car.brakeInput -= m_car.brakeDecayRate * timeDelta;
        }
    }
}

void Vehicle::UpdateCarAlignment()
{
    // Set up to match terrain norm until car jump is implemented
    if (m_car.isCarAirborne == false)
    {
        DirectX::SimpleMath::Vector3 newUp = m_car.terrainNormal;
        DirectX::SimpleMath::Vector3 oldUp = m_car.up;
        DirectX::SimpleMath::Vector3 updateUp = DirectX::SimpleMath::Vector3::SmoothStep(oldUp, newUp, 0.2);
        updateUp.Normalize();
        m_car.up = updateUp;
    }
    m_car.forward = DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::UnitX, DirectX::SimpleMath::Matrix::CreateFromAxisAngle(m_car.up, m_car.carRotation));
    m_car.right = m_car.forward.Cross(m_car.up);
    m_car.right.Normalize();
    m_car.forward = m_car.up.Cross(m_car.right);
}

void Vehicle::UpdateHeadingVec()
{
    m_car.headingVec = m_car.forward;
}

void Vehicle::UpdateModel(const double aTimer)
{
    float wheelTurnRadsFront = GetWheelRotationRadians(aTimer) + m_testRotation;
    wheelTurnRadsFront = Utility::WrapAngle(wheelTurnRadsFront);
    m_testRotation = wheelTurnRadsFront;
    float wheelTurnRadsRear = GetWheelRotationRadiansRear(aTimer) + m_testRotation;

    m_testRotationRear = wheelTurnRadsRear;

    //DebugPushUILineDecimalNumber("wheelTurnRads ", m_testRotation, " ");

    DirectX::SimpleMath::Matrix updateMat = DirectX::SimpleMath::Matrix::Identity;
    updateMat *= DirectX::SimpleMath::Matrix::CreateWorld(m_car.q.position, -m_car.right, m_car.up);



    m_carModel.bodyMatrix = m_carModel.localBodyMatrix;
    m_carModel.bodyMatrix *= updateMat;

    m_carModel.bodyTopMatrix = m_carModel.localBodyTopMatrix;
    m_carModel.bodyTopMatrix *= updateMat;
    ///////

    DirectX::SimpleMath::Matrix wheelSpinMat = DirectX::SimpleMath::Matrix::CreateRotationZ(-wheelTurnRadsFront);
    DirectX::SimpleMath::Matrix wheelSpinRearMat = DirectX::SimpleMath::Matrix::CreateRotationZ(-wheelTurnRadsRear);

    DirectX::SimpleMath::Matrix stearingTurn = DirectX::SimpleMath::Matrix::CreateRotationY(-m_car.steeringAngle);
    
    m_carModel.frontAxelMatrix = m_carModel.frontAxelRotation * wheelSpinMat * stearingTurn;
    m_carModel.frontAxelMatrix *= m_carModel.frontAxelTranslation;
    m_carModel.frontAxelMatrix *= updateMat;

    m_carModel.frontTireMatrix = m_carModel.frontTireRotation * wheelSpinMat * stearingTurn;
    m_carModel.frontTireMatrix *= m_carModel.frontTireTranslation;
    m_carModel.frontTireMatrix *= updateMat;

    m_carModel.rearAxelMatrix = m_carModel.rearAxelRotation * wheelSpinRearMat;
    m_carModel.rearAxelMatrix *= m_carModel.rearAxelTranslation;
    m_carModel.rearAxelMatrix *= updateMat;
  
    m_carModel.rearTireMatrix = m_carModel.rearTireRotation * wheelSpinRearMat;
    m_carModel.rearTireMatrix *= m_carModel.rearTireTranslation;
    m_carModel.rearTireMatrix *= updateMat;

    /// Independant Wheels
    DirectX::SimpleMath::Matrix wheelSpin = DirectX::SimpleMath::Matrix::CreateRotationY(-wheelTurnRadsFront);
    DirectX::SimpleMath::Matrix stearingRotation = DirectX::SimpleMath::Matrix::CreateRotationZ(m_car.steeringAngle);
    m_carModel.wheelFrontRightMatrix = wheelSpin * stearingRotation;
    m_carModel.wheelFrontRightMatrix *= m_carModel.wheelFrontRightTranslation;
    m_carModel.wheelFrontRightMatrix *= updateMat;

    m_carModel.wheelFrontLeftMatrix = wheelSpin * stearingRotation;
    m_carModel.wheelFrontLeftMatrix *= m_carModel.wheelFrontLeftTranslation;
    m_carModel.wheelFrontLeftMatrix *= updateMat;

    m_carModel.wheelRearLeftMatrix = wheelSpin;
    m_carModel.wheelRearLeftMatrix *= m_carModel.wheelRearLeftTranslation;
    m_carModel.wheelRearLeftMatrix *= updateMat;

    m_carModel.wheelRearRightMatrix = wheelSpin;
    m_carModel.wheelRearRightMatrix *= m_carModel.wheelRearRightTranslation;
    m_carModel.wheelRearRightMatrix *= updateMat;

    /// Independant Wheels Spokes
    // front left wheel spokes
    m_carModel.wheelSpokeFL1 = m_carModel.localWheelSpoke1;
    m_carModel.wheelSpokeFL1 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFL1 *= m_carModel.wheelFrontLeftTranslation;
    m_carModel.wheelSpokeFL1 *= updateMat;

    m_carModel.wheelSpokeFL2 = m_carModel.localWheelSpoke2;
    m_carModel.wheelSpokeFL2 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFL2 *= m_carModel.wheelFrontLeftTranslation;
    m_carModel.wheelSpokeFL2 *= updateMat;

    m_carModel.wheelSpokeFL3 = m_carModel.localWheelSpoke3;
    m_carModel.wheelSpokeFL3 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFL3 *= m_carModel.wheelFrontLeftTranslation;
    m_carModel.wheelSpokeFL3 *= updateMat;

    m_carModel.wheelSpokeFL4 = m_carModel.localWheelSpoke4;
    m_carModel.wheelSpokeFL4 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFL4 *= m_carModel.wheelFrontLeftTranslation;
    m_carModel.wheelSpokeFL4 *= updateMat;

    m_carModel.wheelSpokeFL5 = m_carModel.localWheelSpoke5;
    m_carModel.wheelSpokeFL5 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFL5 *= m_carModel.wheelFrontLeftTranslation;
    m_carModel.wheelSpokeFL5 *= updateMat;
    // front right wheel spokes
    m_carModel.wheelSpokeFR1 = m_carModel.localWheelSpoke1;
    m_carModel.wheelSpokeFR1 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFR1 *= m_carModel.wheelFrontRightTranslation;
    m_carModel.wheelSpokeFR1 *= updateMat;

    m_carModel.wheelSpokeFR2 = m_carModel.localWheelSpoke2;
    m_carModel.wheelSpokeFR2 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFR2 *= m_carModel.wheelFrontRightTranslation;
    m_carModel.wheelSpokeFR2 *= updateMat;

    m_carModel.wheelSpokeFR3 = m_carModel.localWheelSpoke3;
    m_carModel.wheelSpokeFR3 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFR3 *= m_carModel.wheelFrontRightTranslation;
    m_carModel.wheelSpokeFR3 *= updateMat;

    m_carModel.wheelSpokeFR4 = m_carModel.localWheelSpoke4;
    m_carModel.wheelSpokeFR4 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFR4 *= m_carModel.wheelFrontRightTranslation;
    m_carModel.wheelSpokeFR4 *= updateMat;

    m_carModel.wheelSpokeFR5 = m_carModel.localWheelSpoke5;
    m_carModel.wheelSpokeFR5 *= wheelSpin * stearingRotation;
    m_carModel.wheelSpokeFR5 *= m_carModel.wheelFrontRightTranslation;
    m_carModel.wheelSpokeFR5 *= updateMat;
    // rear left wheel spokes
    m_carModel.wheelSpokeRearLeft1 = m_carModel.localWheelSpoke1;
    m_carModel.wheelSpokeRearLeft1 *= wheelSpin;
    m_carModel.wheelSpokeRearLeft1 *= m_carModel.wheelRearLeftTranslation;
    m_carModel.wheelSpokeRearLeft1 *= updateMat;

    m_carModel.wheelSpokeRearLeft2 = m_carModel.localWheelSpoke2;
    m_carModel.wheelSpokeRearLeft2 *= wheelSpin;
    m_carModel.wheelSpokeRearLeft2 *= m_carModel.wheelRearLeftTranslation;
    m_carModel.wheelSpokeRearLeft2 *= updateMat;

    m_carModel.wheelSpokeRearLeft3 = m_carModel.localWheelSpoke3;
    m_carModel.wheelSpokeRearLeft3 *= wheelSpin;
    m_carModel.wheelSpokeRearLeft3 *= m_carModel.wheelRearLeftTranslation;
    m_carModel.wheelSpokeRearLeft3 *= updateMat;

    m_carModel.wheelSpokeRearLeft4 = m_carModel.localWheelSpoke4;
    m_carModel.wheelSpokeRearLeft4 *= wheelSpin;
    m_carModel.wheelSpokeRearLeft4 *= m_carModel.wheelRearLeftTranslation;
    m_carModel.wheelSpokeRearLeft4 *= updateMat;

    m_carModel.wheelSpokeRearLeft5 = m_carModel.localWheelSpoke5;
    m_carModel.wheelSpokeRearLeft5 *= wheelSpin;
    m_carModel.wheelSpokeRearLeft5 *= m_carModel.wheelRearLeftTranslation;
    m_carModel.wheelSpokeRearLeft5 *= updateMat;
    // rear right wheel spokes
    m_carModel.wheelSpokeRearRight1 = m_carModel.localWheelSpoke1;
    m_carModel.wheelSpokeRearRight1 *= wheelSpin;
    m_carModel.wheelSpokeRearRight1 *= m_carModel.wheelRearRightTranslation;
    m_carModel.wheelSpokeRearRight1 *= updateMat;

    m_carModel.wheelSpokeRearRight2 = m_carModel.localWheelSpoke2;
    m_carModel.wheelSpokeRearRight2 *= wheelSpin;
    m_carModel.wheelSpokeRearRight2 *= m_carModel.wheelRearRightTranslation;
    m_carModel.wheelSpokeRearRight2 *= updateMat;

    m_carModel.wheelSpokeRearRight3 = m_carModel.localWheelSpoke3;
    m_carModel.wheelSpokeRearRight3 *= wheelSpin;
    m_carModel.wheelSpokeRearRight3 *= m_carModel.wheelRearRightTranslation;
    m_carModel.wheelSpokeRearRight3 *= updateMat;

    m_carModel.wheelSpokeRearRight4 = m_carModel.localWheelSpoke4;
    m_carModel.wheelSpokeRearRight4 *= wheelSpin;
    m_carModel.wheelSpokeRearRight4 *= m_carModel.wheelRearRightTranslation;
    m_carModel.wheelSpokeRearRight4 *= updateMat;

    m_carModel.wheelSpokeRearRight5 = m_carModel.localWheelSpoke5;
    m_carModel.wheelSpokeRearRight5 *= wheelSpin;
    m_carModel.wheelSpokeRearRight5 *= m_carModel.wheelRearRightTranslation;
    m_carModel.wheelSpokeRearRight5 *= updateMat;


    // Rims
    m_carModel.wheelRimFrontMatrix = m_carModel.frontAxelMatrix;
    m_carModel.wheelRimBackMatrix = m_carModel.rearAxelMatrix;

    // Hubs
    DirectX::SimpleMath::Matrix testHubRotate = DirectX::SimpleMath::Matrix::CreateRotationY(-wheelTurnRadsRear);
    DirectX::SimpleMath::Matrix hubStearingTurn = DirectX::SimpleMath::Matrix::CreateRotationY(-m_car.steeringAngle);



    // Front Left Hub
    m_carModel.hubFrontLeftMatrix = wheelSpin * m_carModel.localHubFrontLeftMatrix * stearingRotation * m_carModel.wheelFrontLeftTranslation * updateMat;
    m_carModel.hubInteriorFrontLeftMatrix = wheelSpin * m_carModel.localHubInteriorFrontLeftMatrix * stearingRotation * m_carModel.wheelFrontLeftTranslation * updateMat;
    // Front Right Hub
    m_carModel.hubFrontRightMatrix = wheelSpin * m_carModel.localHubFrontRightMatrix * stearingRotation * m_carModel.wheelFrontRightTranslation * updateMat;
    m_carModel.hubInteriorFrontRightMatrix = wheelSpin * m_carModel.localHubInteriorFrontRightMatrix * stearingRotation * m_carModel.wheelFrontRightTranslation * updateMat;
    // Back Left Hub
    m_carModel.hubBackLeftMatrix = wheelSpin * m_carModel.localHubBackLeftMatrix * m_carModel.wheelRearLeftTranslation * updateMat;
    m_carModel.hubInteriorBackLeftMatrix = wheelSpin * m_carModel.localHubInteriorBackLeftMatrix * m_carModel.wheelRearLeftTranslation * updateMat;
    // Back Right Hub
    m_carModel.hubBackRightMatrix = wheelSpin * m_carModel.localHubBackRightMatrix * m_carModel.wheelRearRightTranslation * updateMat;
    m_carModel.hubInteriorBackRightMatrix = wheelSpin * m_carModel.localHubInteriorBackRightMatrix * m_carModel.wheelRearRightTranslation * updateMat;
    // fender flares
    
    m_carModel.fenderFlareFrontLeftMatrix = m_carModel.localfenderFlareFrontLeftMatrix;
    m_carModel.fenderFlareFrontLeftMatrix *= updateMat;
    m_carModel.fenderFlareFrontLeftInteriorMatrix = m_carModel.localfenderFlareFrontLeftInteriorMatrix;
    m_carModel.fenderFlareFrontLeftInteriorMatrix *= updateMat;
    m_carModel.fenderFlareFrontRightMatrix = m_carModel.localfenderFlareFrontRightMatrix;
    m_carModel.fenderFlareFrontRightMatrix *= updateMat;
    m_carModel.fenderFlareFrontRightInteriorMatrix = m_carModel.localfenderFlareFrontRightInteriorMatrix;
    m_carModel.fenderFlareFrontRightInteriorMatrix *= updateMat;

    m_carModel.fenderFlareRearLeftMatrix = m_carModel.localfenderFlareRearLeftMatrix;
    m_carModel.fenderFlareRearLeftMatrix *= updateMat;
    m_carModel.fenderFlareRearLeftInteriorMatrix = m_carModel.localfenderFlareRearLeftInteriorMatrix;
    m_carModel.fenderFlareRearLeftInteriorMatrix *= updateMat;
    m_carModel.fenderFlareRearRightMatrix = m_carModel.localfenderFlareRearRightMatrix;
    m_carModel.fenderFlareRearRightMatrix *= updateMat;
    m_carModel.fenderFlareRearRightInteriorMatrix = m_carModel.localfenderFlareRearRightInteriorMatrix;
    m_carModel.fenderFlareRearRightInteriorMatrix *= updateMat;

    // rocker skirt 
    m_carModel.rockerSkirtMatrix = m_carModel.localRockerSkirtMatrix;
    m_carModel.rockerSkirtMatrix *= updateMat;
    // windshield
    m_carModel.windShieldMatrix = m_carModel.localWindShieldMatrix;
    m_carModel.windShieldMatrix *= updateMat;
    // windshield window
    m_carModel.windShieldWindowMatrix = m_carModel.localWindShieldWindowMatrix;
    m_carModel.windShieldWindowMatrix *= updateMat;
    // front side window
    m_carModel.frontSideWindowsMatrix = m_carModel.localfrontSideWindowsMatrix;
    m_carModel.frontSideWindowsMatrix *= updateMat;
    // rear side window
    m_carModel.rearSideWindowsMatrix = m_carModel.localrearSideWindowsMatrix;
    m_carModel.rearSideWindowsMatrix *= updateMat;
    // back window
    m_carModel.backWindowMatrix = m_carModel.localBackWindowMatrix;
    m_carModel.backWindowMatrix *= updateMat;
    // triangle front window
    m_carModel.triangleFrontWindowMatrix = m_carModel.localtriangleFrontWindowMatrix;
    m_carModel.triangleFrontWindowMatrix *= updateMat;
    // front bumper 
    m_carModel.bumperFrontMatrix = m_carModel.localBumperFrontMatrix;
    m_carModel.bumperFrontMatrix *= updateMat;
    // back bumper
    m_carModel.bumperBackMatrix = m_carModel.localBumperBackMatrix;
    m_carModel.bumperBackMatrix *= updateMat;
    // air dam
    m_carModel.airDamMatrix = m_carModel.localAirDamMatrix;
    m_carModel.airDamMatrix *= updateMat;
    // grill
    m_carModel.grillMatrix = m_carModel.localGrillMatrix;
    m_carModel.grillMatrix *= updateMat;
    m_carModel.grillLogoMatrix = m_carModel.localGrillLogoMatrix;
    m_carModel.grillLogoMatrix *= updateMat;
    m_carModel.grillSlatMatrix = m_carModel.localGrillSlatMatrix;
    m_carModel.grillSlatMatrix *= updateMat;
    // License Plate Rear
    m_carModel.licensePlateRearMatrix = m_carModel.localLicensePlateRearMatrix;
    m_carModel.licensePlateRearMatrix *= updateMat;
    // left headlight
    m_carModel.headLightLeftMatrix = m_carModel.localHeadLightLeftMatrix;
    m_carModel.headLightLeftMatrix *= updateMat;
    // right headlight
    m_carModel.headLightRightMatrix = m_carModel.localHeadLightRightMatrix;
    m_carModel.headLightRightMatrix *= updateMat;
    // left blinker light
    m_carModel.blinkerLightLowerLeftMatrix = m_carModel.localBlinkerLightLowerLeftMatrix;
    m_carModel.blinkerLightLowerLeftMatrix *= updateMat;
    m_carModel.blinkerLightUpperLeftMatrix = m_carModel.localBlinkerLightUpperLeftMatrix;
    m_carModel.blinkerLightUpperLeftMatrix *= updateMat;
    // right blinker light
    m_carModel.blinkerLightLowerRightMatrix = m_carModel.localBlinkerLightLowerRightMatrix;
    m_carModel.blinkerLightLowerRightMatrix *= updateMat;
    m_carModel.blinkerLightUpperRightMatrix = m_carModel.localBlinkerLightUpperRightMatrix;
    m_carModel.blinkerLightUpperRightMatrix *= updateMat;
    // right tail light
    m_carModel.tailLightRightMatrix = m_carModel.localTailLightRightMatrix;
    m_carModel.tailLightRightMatrix *= updateMat;
    // left tail light
    m_carModel.tailLightLeftMatrix = m_carModel.localTailLightLeftMatrix;
    m_carModel.tailLightLeftMatrix *= updateMat;
    // reverse light
    m_carModel.reverseLightLeftMatrix = m_carModel.localReverseLightLeftMatrix;
    m_carModel.reverseLightLeftMatrix *= updateMat;
    m_carModel.reverseLightRightMatrix = m_carModel.localReverseLightRightMatrix;
    m_carModel.reverseLightRightMatrix *= updateMat;
    // tail blinker light
    m_carModel.tailBlinkerLightLeftMatrix = m_carModel.localTailBlinkerLightLeftMatrix;
    m_carModel.tailBlinkerLightLeftMatrix *= updateMat;
    m_carModel.tailBlinkerLightRightMatrix = m_carModel.localTailBlinkerLightRightMatrix;
    m_carModel.tailBlinkerLightRightMatrix *= updateMat;
    // third brake light
    m_carModel.thirdBrakeLightMatrix = m_carModel.localThirdBrakeLightMatrix;
    m_carModel.thirdBrakeLightMatrix *= updateMat;
    // rear spoiler
    m_carModel.rearSpoilerMatrix = m_carModel.locarearSpoilerMatrix;
    m_carModel.rearSpoilerMatrix *= updateMat;
    // side mirrors
    m_carModel.sideMirrorLeftMatrix = m_carModel.localSideMirrorLeftMatrix;
    m_carModel.sideMirrorLeftMatrix *= updateMat;
    m_carModel.sideMirrorRightMatrix = m_carModel.localSideMirrorRightMatrix;
    m_carModel.sideMirrorRightMatrix *= updateMat;    
    // hood
    m_carModel.hoodMatrix = m_carModel.localHoodMatrix;
    m_carModel.hoodMatrix *= updateMat;
    // pinstripe
    m_carModel.pinstripeMatrix = m_carModel.localPinstripeMatrix;
    m_carModel.pinstripeMatrix *= updateMat;



    // Test powered wheel prototype model start
    m_carModel.testWorldWheelMatrix = m_carModel.testLocalwheelMatrix;
    m_carModel.testWorldWheelMatrix *= wheelSpin;
    m_carModel.testWorldWheelMatrix *= m_carModel.wheelRearLeftTranslation;
    m_carModel.testWorldWheelMatrix *= updateMat;
    // Test powered wheel prototype model end
}

void Vehicle::UpdateResistance()
{
    /*
    Rair = (1/2) rho[mass ensity of air] V^2 Sp  Cd
        Sp = projected frontal area of car normalto the direction V
        Cd = drag coeffient == 0.4?ish
        */
    float velocity = m_car.q.velocity.Length();
    float drag = .5f * m_car.Cd * m_car.density * m_car.area * (velocity * velocity);

    m_car.airResistance = drag;
}

void Vehicle::UpdateTerrainNorm()
{
    DirectX::SimpleMath::Vector3 newTerrainNorm = m_environment->GetTerrainNormal(m_car.q.position);
    DirectX::SimpleMath::Vector3 oldTerrainNorm = m_car.terrainNormal;
    DirectX::SimpleMath::Vector3 updateTerrainNorm = DirectX::SimpleMath::Vector3::SmoothStep(newTerrainNorm, oldTerrainNorm, 0.8);
    updateTerrainNorm.Normalize();
    m_car.terrainNormal = updateTerrainNorm;

    // test switch smoothing to m_car.up
    m_car.terrainNormal = m_environment->GetTerrainNormal(m_car.q.position);
}

void Vehicle::UpdateTransmission(const double aTimeDelta)
{
    // update shift delay cooldown
    m_car.shiftCooldown -= static_cast<float>(aTimeDelta);
    if (m_car.shiftCooldown < 0.0)
    {
        m_car.shiftCooldown = 0.0;
    }

    float velocity = m_car.q.velocity.Length();
    //  Compute the new engine rpm value
    // test rpm with clutch depressed
    if (m_car.isClutchPressed == true)
    {
        m_car.omegaE = velocity * 60.0f * m_car.gearRatio[m_car.gearNumber] * m_car.finalDriveRatio / (2.0f * Utility::GetPi() * (m_car.wheelRadius * 0.1f));
    }
    else
    {
        m_car.omegaE = velocity * 60.0f * m_car.gearRatio[m_car.gearNumber] * m_car.finalDriveRatio / (2.0f * Utility::GetPi() * m_car.wheelRadius);
    }

    if (m_car.omegaE < 800.0)
    {
        m_car.omegaE = 800.0;
    }
    //  If the engine is at the redline rpm value,
    //  shift gears upward.
    if (m_car.isTransmissionManual == false)
    {
        if (m_car.omegaE > m_car.redline)
        {
            float oldGearRatio = m_car.gearRatio[m_car.gearNumber];
            GearUp();
            float newGearRatio = m_car.gearRatio[m_car.gearNumber];
            m_car.omegaE = m_car.omegaE * newGearRatio / oldGearRatio;
        }
        /*
        if (m_car.omegaE < downShiftLimit && m_car.gearNumber > 1)
        {
            float oldGearRatio = m_car.gearRatio[m_car.gearNumber];
            --m_car.gearNumber;
            float newGearRatio = m_car.gearRatio[m_car.gearNumber];
            m_car.omegaE = m_car.omegaE * newGearRatio / oldGearRatio;
        }
        */
    }
}

void Vehicle::UpdateVehicle(const double aTimeDelta)
{
    DebugClearUI();
    DirectX::SimpleMath::Vector3 prevVelocity = m_car.q.velocity;
    DirectX::SimpleMath::Vector3 prevPos = m_car.q.position;

    m_car.testModelRotation = m_car.carRotation;
    m_car.testTerrainNormal = m_car.terrainNormal;
    m_car.testHeadingVec = m_car.headingVec;

    m_car.isCarLanding = false;
    m_car.terrainHightAtPos = m_environment->GetTerrainHeightAtPos(m_car.q.position);
    
    if (m_car.q.position.y - m_car.terrainHightAtPos > 0.1)
    {
        m_car.isCarAirborne = true;
        m_testTimer += aTimeDelta;
    }
    else
    {
        if (m_car.isCarAirborne == true)
        {
            LandVehicle();
            m_car.isCarLanding = true;
            m_testTimer2 = m_testTimer;
            m_testTimer = 0.0f;
        }
        m_car.isCarAirborne = false;
        m_car.q.position.y = m_car.terrainHightAtPos;
    }
    
    //RevLimiter();
    ThrottleBrakeDecay(aTimeDelta);
    SteeringInputDecay(aTimeDelta);
    TurnVehicle(aTimeDelta);
    UpdateTerrainNorm();   
    m_car.testModelPos = m_car.q.position;
    UpdateCarAlignment();
    RungeKutta4(&m_car, aTimeDelta);

    if (m_car.forward.Dot(m_car.q.velocity) < 0.0)
    {
        m_car.isVelocityBackwards = true;
    }
    else
    {
        m_car.isVelocityBackwards = false;
    }
  
    UpdateVelocity(aTimeDelta);
    UpdateHeadingVec();
    UpdateTransmission(aTimeDelta);

    m_car.speed = m_car.q.velocity.Length();
    UpdateModel(aTimeDelta);

    m_car.isThrottlePressed = false;
    m_car.isTurningPressed = false;
    m_car.isBrakePressed = false;
    m_testIsBreakLightOn = false;

    m_car.testAccel = (m_car.q.velocity.Length() - prevVelocity.Length()) / static_cast<float>(aTimeDelta);
    m_car.testAcceleration = (m_car.q.velocity - prevVelocity) / static_cast<float>(aTimeDelta);
    m_car.testAcceleration = m_car.testAcceleration / m_car.q.velocity;
    
    DirectX::XMVECTORF32 testVec;// = DirectX::XMVECTORF32::f(1.0, 1.0, 1.0, 1.0);
    testVec.f[0] = 1.0f;
    testVec.f[1] = 0.0f;
    testVec.f[2] = 0.0f;
    testVec.f[3] = 1.0f;

    DebugPushTestLine(m_car.q.position, DirectX::SimpleMath::Vector3::UnitY, 5.0, 0.0, testVec);

    UpdateResistance();

    DirectX::SimpleMath::Vector3 postPos = m_car.q.position;
    DirectX::SimpleMath::Vector3 deltaPos = prevPos - postPos;
    float deltaLength = deltaPos.Length();
    m_testVelocity = deltaLength / static_cast<float>(aTimeDelta);
    DebugPushUILineDecimalNumber("m_testTimer : ", m_testTimer, "");
    DebugPushUILineDecimalNumber("m_testTimer2 : ", m_testTimer2, "");
}

void Vehicle::UpdateVelocity(double aTimeDelta)
{
    float lateralFrictionFactor = 0.2;
    float backwardsFrictionFactor = 0.0;
    DirectX::SimpleMath::Vector3 testVelocity = m_car.q.velocity;
    DirectX::SimpleMath::Vector3 lateralVelocity;
    lateralVelocity = m_car.right * testVelocity.Dot(m_car.right);
    DirectX::SimpleMath::Vector3 lateralFriction = -lateralVelocity * lateralFrictionFactor;
    DirectX::SimpleMath::Vector3 backwardsFriction = -testVelocity * backwardsFrictionFactor;
    testVelocity += (backwardsFriction + lateralFriction) * static_cast<float>(aTimeDelta);

    const float lerpSize = 0.5;
    if (m_car.isCarAirborne == true)
    {

    }
    else
    {
        if (m_car.forward.Dot(m_car.q.velocity) < 0.0)
        {
            m_car.q.totalVelocity = DirectX::SimpleMath::Vector3::Lerp(m_car.q.totalVelocity, -m_car.forward * m_car.q.totalVelocity.Length(), lerpSize);
            m_car.q.velocity = DirectX::SimpleMath::Vector3::Lerp(m_car.q.velocity, -m_car.forward * m_car.q.velocity.Length(), lerpSize);
        }
        else
        {
            m_car.q.totalVelocity = DirectX::SimpleMath::Vector3::Lerp(m_car.q.totalVelocity, m_car.forward * m_car.q.totalVelocity.Length(), lerpSize);
            m_car.q.velocity = DirectX::SimpleMath::Vector3::Lerp(m_car.q.velocity, m_car.forward * m_car.q.velocity.Length(), lerpSize);
        }
    }
}

void Vehicle::DebugPushTestLine(DirectX::SimpleMath::Vector3 aLineBase, DirectX::SimpleMath::Vector3 aLineEnd, float aLength, float aYOffset, DirectX::XMVECTORF32 aColor)
{
    DirectX::SimpleMath::Vector3 scaledLineBase = aLineBase;
    scaledLineBase.y += aYOffset;
    DirectX::SimpleMath::Vector3 scaledLineEnd = aLineEnd;
    scaledLineEnd.Normalize();
    scaledLineEnd *= aLength;
    scaledLineEnd += scaledLineBase;
    std::tuple<DirectX::SimpleMath::Vector3, DirectX::SimpleMath::Vector3, DirectX::XMVECTORF32> lineTup(scaledLineBase, scaledLineEnd, aColor);
    m_debugLinesVec.push_back(lineTup);
}

void Vehicle::DebugPushUILine(std::string aString, float aVal)
{
    std::pair<std::string, float> newPair = std::make_pair(aString, aVal);
    m_debugUI.push_back(newPair);
}

void Vehicle::DebugPushUILineDecimalNumber(std::string aString1, float aVal, std::string aString2)
{
    std::string textLine = aString1 + " " + std::to_string(aVal) + " " +aString2;
    m_debugUIVector.push_back(textLine);
}

void Vehicle::DebugPushUILineWholeNumber(std::string aString1, int aVal, std::string aString2)
{
    std::string textLine = aString1 + " " + std::to_string(aVal) + " " + aString2;
    m_debugUIVector.push_back(textLine);
}

void Vehicle::TestGetForceLateral()
{
    float radius = GetTurnRadius();
    float mass = m_car.mass;
    float velocity = m_car.q.velocity.Length();
    velocity *= velocity;
    float muK = 0.7f; // guess at this point

    float forceLat;
    forceLat = ((mass * velocity) / radius) - (muK * mass * m_car.gravity.y * cos(0.0f));  
    DebugPushUILine("Force Lateral", forceLat);

    DirectX::SimpleMath::Vector3 testForceLat;
    DirectX::SimpleMath::Vector3 gravVec(0.0f, -9.8f, 0.0f);

    testForceLat = ((mass * (m_car.q.velocity * m_car.q.velocity)) / radius) - (muK * mass * gravVec * cos(0.0f));

    DebugPushUILine("Force Lateral X ", testForceLat.x);
    DebugPushUILine("Force Lateral Y ", testForceLat.y);
    DebugPushUILine("Force Lateral Z ", testForceLat.z);

    DirectX::SimpleMath::Vector3 testHeading = m_car.q.velocity;
    DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateRotationY(-m_car.carRotation);
    testHeading = DirectX::SimpleMath::Vector3::Transform(testHeading, rotMat);

    float slipAngle;
    slipAngle = -atan(testHeading.x / abs(testHeading.z));
    DebugPushUILine("slipAngle", Utility::ToDegrees(slipAngle)  + 90.0f);
    DebugPushUILine("slipAngle 2", Utility::ToDegrees(slipAngle) - Utility::ToDegrees(m_car.carRotation));
    DebugPushUILine("slipAngle 3", Utility::ToDegrees(slipAngle) + 90.0f - Utility::ToDegrees(m_car.carRotation));
    DebugPushUILine("m_car.carRotation ", Utility::ToDegrees(m_car.carRotation));
}

