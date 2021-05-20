#include "pch.h"
#include "Vehicle.h"


Vehicle::Vehicle()
{
    //InitializeVehicle();
}

Vehicle::~Vehicle()
{
    m_vehicleCamera = nullptr;
    delete m_vehicleCamera;
}

void Vehicle::DrawModel(DirectX::SimpleMath::Matrix aWorld, DirectX::SimpleMath::Matrix aView, DirectX::SimpleMath::Matrix aProj, const double aTimer)
{
    DirectX::SimpleMath::Matrix world = aWorld;
    DirectX::SimpleMath::Matrix view = aView;
    DirectX::SimpleMath::Matrix proj = aProj;
    
    m_carModel.bodyTop->Draw(m_carModel.bodyTopMatrix, view, proj, DirectX::Colors::DarkRed);
    m_carModel.body->Draw(m_carModel.bodyMatrix, view, proj, DirectX::Colors::Red);
    m_carModel.frontAxel->Draw(m_carModel.frontAxelMatrix, view, proj);
    m_carModel.rearAxel->Draw(m_carModel.rearAxelMatrix, view, proj);
    
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

double Vehicle::GetCarRotation2()
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

    /*
    if (DirectX::XMVectorGetY(DirectX::XMVector3Cross(ballVec, zeroDirection)) < 0.0f)
    {
        direction = -direction;
    }
    */
    DirectX::SimpleMath::Vector3 a = -DirectX::SimpleMath::Vector3::UnitZ;
    DirectX::SimpleMath::Vector3 b = DirectX::SimpleMath::Vector3::UnitX;
    b = testRadVecNorm;
    //static inline double GetAngle1(DirectX::SimpleMath::Vector3 aRef, DirectX::SimpleMath::Vector3 aPointA) { return acos(aRef.Dot(aPointA)); };
    double testAngle = acos(a.Dot(b));
    double testAngleDegrees = Utility::ToDegrees(testAngle);

    if (testAngle <= 0.0)
    {
        int testBreak = 0;
        ++testBreak;
    }

    double carRotation = testAngle + 1.0; // +(Utility::ToRadians(90.0));
    m_car.carRotation = testAngle;
    //m_car.carRotation = steeringAngle;
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
    //steeringAngle = Utility::ToRadians(10.0);
    //velocity = 10.0;
    double sinDelta = sin(steeringAngle);

    double omega = (velocity * sinDelta) / wheelBase;
    double omegaT = omega * aTimeDelta;

    double omegaDeg = Utility::ToDegrees(omega);
    double omegaTdeg = Utility::ToDegrees(omegaT);

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

    // temp for camera testing
    //m_car.position = frontAxelPos;
   
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
    
    return rotations;
}

void Vehicle::InitializeModel(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext)
{
    // porche boxter base dimensions - 4.3942m L x 1.8034m W x 1.27m H, wheel diameter 0.3186m
    const float wheelRadius = m_car.wheelRadius;
    const float length = 4.3942;
    const float width = 1.8034;
    const float heightTotal = 1.27;
    const float height = heightTotal * 0.6;
     
    const float axelLength = width + 0.6;
    const float wheelBase = m_car.wheelBase;
    DirectX::SimpleMath::Vector3 carBodySize(length, height - wheelRadius, width);

    m_carModel.body = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), carBodySize);
    m_carModel.frontAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, wheelRadius, 3);
    m_carModel.rearAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, wheelRadius, 6);

    m_carModel.bodyMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.frontAxelMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.rearAxelMatrix = DirectX::SimpleMath::Matrix::Identity;

    m_carModel.bodyMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, (height + (wheelRadius)), 0.0));

    DirectX::SimpleMath::Matrix axelRotation = DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0));

    m_carModel.frontAxelMatrix *= axelRotation;
    m_carModel.frontAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5, wheelRadius * 0.5, 0.0));
    m_carModel.rearAxelMatrix *= axelRotation;
    m_carModel.rearAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5, wheelRadius * 0.5, 0.0));
    
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

    m_carModel.frontAxelRotation *= axelRotation;
    m_carModel.frontAxelTranslation *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5, wheelRadius * 0.5, 0.0));

    m_carModel.rearAxelRotation = axelRotation;
    m_carModel.rearAxelTranslation = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5, wheelRadius * 0.5, 0.0));
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
    //m_car.gearRatio[0] = 0.0;
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
    m_car.accelerationInput = 0.0;
    m_car.brakeInput = 0.0;
    m_car.maxAccelerationRate = 1.0;
    m_car.maxBrakeRate = 5.0;
    m_car.carRotation = 0.0;
    //m_car.steeringInRads = 0.0;
    m_car.steeringAngle = Utility::ToRadians(180.0);
    m_car.steeringAngle2 = Utility::ToRadians(180.0);
    m_car.heading = DirectX::SimpleMath::Vector3::Zero;
    m_car.speed = 0.0;

    m_car.isAccelerating = false;
    m_car.isBraking = false;
    m_car.wheelBase = 2.41;

    InitializeModel(aContext);
}

//void Vehicle::LinkCamera(Camera const* aCamera)
void Vehicle::LinkCamera(Camera* aCamera)
{
    m_vehicleCamera = aCamera;
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
    /*
    newQ.position.x = q->position.x + static_cast<float>(aQScale) * deltaQ->position.x;
    newQ.position.y = q->position.y + static_cast<float>(aQScale) * deltaQ->position.y;
    newQ.position.z = q->position.z + static_cast<float>(aQScale) * deltaQ->position.z;
    newQ.velocity.x = q->velocity.x + static_cast<float>(aQScale) * deltaQ->velocity.x;
    newQ.velocity.y = q->velocity.y + static_cast<float>(aQScale) * deltaQ->velocity.y;
    newQ.velocity.z = q->velocity.z + static_cast<float>(aQScale) * deltaQ->velocity.z;
    */
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
    if (m_car.isAccelerating == true)
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
    }
    // braking
    else if (m_car.isBraking == true)
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
            //dq[0] = 0.0;
            aDQ->velocity.x = 0.0;
            aDQ->velocity.y = 0.0;
            aDQ->velocity.z = 0.0;
        }
    }
    else
    {
        //dq[0] = 0.0;
        aDQ->velocity.x = 0.0;
        aDQ->velocity.y = 0.0;
        aDQ->velocity.z = 0.0;
    }

    //  Compute right-hand side values.
    /*
    aDQ->position.z = aTimeDelta * newQ.velocity.z;
    aDQ->velocity.y = 0.0;
    aDQ->position.y = 0.0;
    aDQ->velocity.x = 0.0;
    aDQ->position.x = 0.0;
    */
    /*
    aDQ->position.x = aTimeDelta * newQ.velocity.x;
    aDQ->velocity.y = 0.0;
    aDQ->position.y = 0.0;
    aDQ->velocity.z = 0.0;
    aDQ->position.z = 0.0;
    */
    aDQ->position.x = aTimeDelta * newQ.velocity.x;
    //aDQ->velocity.y = 0.0;
    aDQ->position.y = aTimeDelta * newQ.velocity.y;
    //aDQ->velocity.z = 0.0;
    aDQ->position.z = aTimeDelta * newQ.velocity.z;

    DirectX::SimpleMath::Matrix testTurn = DirectX::SimpleMath::Matrix::CreateRotationY(m_car.carRotation);
    //DirectX::SimpleMath::Matrix testTurn = DirectX::SimpleMath::Matrix::CreateRotationY(m_car.steeringAngle);
    //DirectX::SimpleMath::Matrix testTurn = DirectX::SimpleMath::Matrix::CreateRotationY(Utility::ToRadians(-90.0));
    //aDQ->velocity.Transform(aDQ->velocity, testTurn, aDQ->velocity);
    //aDQ->velocity = DirectX::SimpleMath::Vector3::Transform(aDQ->velocity, testTurn, aDQ->velocity);

    //aDQ->velocity = DirectX::SimpleMath::Vector3::Transform(aDQ->velocity, testTurn);

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
    //double time = car->time;
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

    /*
    q.position.x = static_cast<float>(q.position.x + (dq1.position.x + 2.0 * dq2.position.x + 2.0 * dq3.position.x + dq4.position.x) / numEqns);
    aCar->q.position.x = q.position.x; 
    q.position.y = static_cast<float>(q.position.y + (dq1.position.y + 2.0 * dq2.position.y + 2.0 * dq3.position.y + dq4.position.y) / numEqns);
    aCar->q.position.y = q.position.y;
    q.position.z = static_cast<float>(q.position.z + (dq1.position.z + 2.0 * dq2.position.z + 2.0 * dq3.position.z + dq4.position.z) / numEqns);
    aCar->q.position.z = q.position.z;
    q.velocity.x = static_cast<float>(q.velocity.x + (dq1.velocity.x + 2.0 * dq2.velocity.x + 2.0 * dq3.velocity.x + dq4.velocity.x) / numEqns);
    aCar->q.velocity.x = q.velocity.x;
    q.velocity.y = static_cast<float>(q.velocity.y + (dq1.velocity.y + 2.0 * dq2.velocity.y + 2.0 * dq3.velocity.y + dq4.velocity.y) / numEqns);
    aCar->q.velocity.y = q.velocity.y;
    q.velocity.z = static_cast<float>(q.velocity.z + (dq1.velocity.z + 2.0 * dq2.velocity.z + 2.0 * dq3.velocity.z + dq4.velocity.z) / numEqns);
    aCar->q.velocity.z = q.velocity.z;
    */
    
    q.position.x = static_cast<float>(q.position.x + (dq1.position.x + 2.0 * dq2.position.x + 2.0 * dq3.position.x + dq4.position.x) / numEqns);
    q.position.y = static_cast<float>(q.position.y + (dq1.position.y + 2.0 * dq2.position.y + 2.0 * dq3.position.y + dq4.position.y) / numEqns);
    q.position.z = static_cast<float>(q.position.z + (dq1.position.z + 2.0 * dq2.position.z + 2.0 * dq3.position.z + dq4.position.z) / numEqns);
    q.velocity.x = static_cast<float>(q.velocity.x + (dq1.velocity.x + 2.0 * dq2.velocity.x + 2.0 * dq3.velocity.x + dq4.velocity.x) / numEqns);
    q.velocity.y = static_cast<float>(q.velocity.y + (dq1.velocity.y + 2.0 * dq2.velocity.y + 2.0 * dq3.velocity.y + dq4.velocity.y) / numEqns);
    q.velocity.z = static_cast<float>(q.velocity.z + (dq1.velocity.z + 2.0 * dq2.velocity.z + 2.0 * dq3.velocity.z + dq4.velocity.z) / numEqns);

    aCar->q.position = q.position;
    aCar->q.velocity = q.velocity;
    
    return;
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
    m_car.steeringAngle += aTurnInput * 0.5;
}

void Vehicle::UpdateModel(const double aTimer)
{
    double wheelTurnRads = GetWheelRotationRadians(aTimer) + m_testRotation;
    m_testRotation = wheelTurnRads;
    DirectX::SimpleMath::Matrix updateMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_car.q.position);
    /*
    DirectX::SimpleMath::Quaternion testQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, 0.0, 0.0);

    m_carModel.bodyMatrix.Transform(updateMatrix, testQuat, m_carModel.bodyMatrix);
    m_carModel.bodyTopMatrix.Transform(updateMatrix, testQuat, m_carModel.bodyTopMatrix);   

    m_carModel.bodyMatrix *= m_carModel.localBodyMatrix;
    m_carModel.bodyTopMatrix *= m_carModel.localBodyTopMatrix;
    */
    /////////////////////////////

    //DirectX::SimpleMath::Matrix testTurn = DirectX::SimpleMath::Matrix::CreateRotationY(cos(m_car.time));
    DirectX::SimpleMath::Matrix testTurn = DirectX::SimpleMath::Matrix::CreateRotationY(m_car.carRotation);
    
    /*
    m_carModel.bodyMatrix = testTurn;
    m_carModel.bodyMatrix *= m_carModel.localBodyMatrix * updateMatrix;
    */
    m_carModel.bodyMatrix = m_carModel.localBodyMatrix;
    m_carModel.bodyMatrix *= testTurn;
    m_carModel.bodyMatrix *=  updateMatrix;

    m_carModel.bodyTopMatrix = m_carModel.localBodyTopMatrix;
    m_carModel.bodyTopMatrix *= testTurn;
    m_carModel.bodyTopMatrix *= updateMatrix;
    ///////

    DirectX::SimpleMath::Matrix wheelSpinMat = DirectX::SimpleMath::Matrix::CreateRotationZ(-wheelTurnRads);
    /*
    m_carModel.frontAxelMatrix = m_carModel.frontAxelRotation * wheelSpinMat;
    m_carModel.frontAxelMatrix *= m_carModel.frontAxelTranslation * updateMatrix;
    */

    DirectX::SimpleMath::Matrix stearingTurn = DirectX::SimpleMath::Matrix::CreateRotationY(-m_car.steeringAngle);
    m_carModel.frontAxelMatrix = m_carModel.frontAxelRotation * wheelSpinMat * stearingTurn;
    m_carModel.frontAxelMatrix *= m_carModel.frontAxelTranslation;
    m_carModel.frontAxelMatrix *= testTurn;
    m_carModel.frontAxelMatrix *= updateMatrix;

    /*
    m_carModel.rearAxelMatrix = m_carModel.rearAxelRotation * wheelSpinMat;
    m_carModel.rearAxelMatrix *= m_carModel.rearAxelTranslation * updateMatrix;
    */

    m_carModel.rearAxelMatrix = m_carModel.rearAxelRotation * wheelSpinMat;
    m_carModel.rearAxelMatrix *= m_carModel.rearAxelTranslation;
    m_carModel.rearAxelMatrix *= testTurn;
    m_carModel.rearAxelMatrix *= updateMatrix;
}

void Vehicle::UpdateVehicle(const double aTimer, const double aTimeDelta)
{   
    //m_car.steeringAngle = m_car.carRotation;
    //m_car.steeringAngle += Utility::ToRadians(90.0);
    double preRot = m_car.carRotation;
    m_car.carRotation += GetYawRate(aTimeDelta);
    double postRot = m_car.carRotation;
    double deltaSteer = preRot - postRot;
    //m_car.steeringAngle = m_car.carRotation + m_car.carRotation + m_car.steeringAngle2 - deltaSteer - deltaSteer;
    //m_car.steeringAngle += deltaSteer + m_car.steeringAngle2;
    //m_car.steeringAngle -= m_car.carRotation;
    //m_car.carRotation += 0.005;
    if (m_car.time > 10.0)
    {
        int testBreak = 0;
    }

    RungeKutta4(&m_car, aTimeDelta);

    double testV2 = m_car.speed;
    double time = m_car.time;
    /*
    double x = m_car.q.position.x;
    double vx = m_car.q.velocity.x;

    double z = m_car.q.position.z;
    double vz = m_car.q.velocity.z;
    */
    double velocity = m_car.q.velocity.Length();

    int gear = m_car.gearNumber;
    double rpm = m_car.omegaE;

    double oldGearRatio;
    double newGearRatio;
    //  Compute the new engine rpm value
    int gearNumber = m_car.gearNumber;
    double gearRatio = m_car.gearRatio[gearNumber];
    //m_car.omegaE = vx * 60.0 * gearRatio * m_car.finalDriveRatio / (2.0 * Utility::GetPi() * m_car.wheelRadius);
    //m_car.omegaE = vz * 60.0 * gearRatio * m_car.finalDriveRatio / (2.0 * Utility::GetPi() * m_car.wheelRadius);
    m_car.omegaE = velocity * 60.0 * gearRatio * m_car.finalDriveRatio / (2.0 * Utility::GetPi() * m_car.wheelRadius);

    //  If the engine is at the redline rpm value,
    //  shift gears upward.
    if (m_car.omegaE > m_car.redline) 
    {
        oldGearRatio = gearRatio;
        ++m_car.gearNumber;
        newGearRatio = m_car.gearRatio[m_car.gearNumber];
        m_car.omegaE = m_car.omegaE * newGearRatio / oldGearRatio;
    }

    m_car.speed = m_car.q.velocity.Length();

    UpdateModel(aTimeDelta);
    UpdateVehicleCamera();

}

void Vehicle::DebugTestMove(const double aTimer, const double aTimeDelta)
{
    float move = 3.3;
    //m_car.q2[1] += move;
    m_car.q.position.x += move;
}

void Vehicle::UpdateVehicleCamera()
{
    m_vehicleCamera->SetFollowCamUp(DirectX::SimpleMath::Vector3::UnitY);
    m_vehicleCamera->SetFollowCamTarget(m_car.q.position);
    m_vehicleCamera->SetFollowCamDirection(GetVehicleDirection());
}
