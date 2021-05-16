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
    const float wheelBase = 2.47396;
    DirectX::SimpleMath::Vector3 carBodySize(length, height - wheelRadius, width);

    m_carModel.body = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), carBodySize);
    m_carModel.frontAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, wheelRadius, 6);
    m_carModel.rearAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, wheelRadius, 6);

    m_carModel.bodyMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.frontAxelMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.rearAxelMatrix = DirectX::SimpleMath::Matrix::Identity;

    m_carModel.bodyMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, (height + (wheelRadius)), 0.0));

    DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0));

    m_carModel.frontAxelMatrix *= rotMat;    
    m_carModel.frontAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5, wheelRadius * 0.5, 0.0));
    m_carModel.rearAxelMatrix *= rotMat;
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
    m_carModel.localFrontAxelMatrix = m_carModel.frontAxelMatrix;
    m_carModel.localRearAxelMatrix = m_carModel.rearAxelMatrix;
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
    m_car.gearRatio[1] = 3.82;
    m_car.gearRatio[2] = 2.20;
    m_car.gearRatio[3] = 1.52;
    m_car.gearRatio[4] = 1.22;
    m_car.gearRatio[5] = 1.02;
    m_car.gearRatio[6] = 0.84;
    m_car.gravity = - 9.81;
    m_car.numEqns = 6;
    m_car.s = 0.0;      //  time 
    m_car.q.position = DirectX::SimpleMath::Vector3::Zero;
    m_car.q.velocity = DirectX::SimpleMath::Vector3::Zero;

    m_car.inputDeadZone = 0.05;
    m_car.accelerationInput = 0.0;
    m_car.brakeInput = 0.0;
    m_car.maxAccelerationRate = 1.0;
    m_car.maxBrakeRate = 5.0;
    m_car.steeringAngle = 0.0;
    m_car.heading = DirectX::SimpleMath::Vector3::Zero;
    m_car.speed = 0.0;

    m_car.isAccelerating = true;
    m_car.isBraking = false;

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
void Vehicle::RightHandSide(struct Car* car, Motion* q, Motion* deltaQ, double ds, double aQScale, Motion* dq)
{
    //  Compute the intermediate values of the 
    //  dependent variables.
    Motion newQ;
    //DirectX::SimpleMath::Vector3 newQ;
    /*
    newQ.position.x = q->position.x + static_cast<float>(aQScale) * deltaQ->position.x;
    newQ.position.y = q->position.y + static_cast<float>(aQScale) * deltaQ->position.y;
    newQ.position.z = q->position.z + static_cast<float>(aQScale) * deltaQ->position.z;
    newQ.velocity.x = q->velocity.x + static_cast<float>(aQScale) * deltaQ->velocity.x;
    newQ.velocity.y = q->velocity.y + static_cast<float>(aQScale) * deltaQ->velocity.y;
    newQ.velocity.z = q->velocity.z + static_cast<float>(aQScale) * deltaQ->velocity.z;
    */
    newQ.position = q->velocity + static_cast<float>(aQScale) * deltaQ->position;
    newQ.velocity = q->velocity + static_cast<float>(aQScale) * deltaQ->velocity;

    //newQ = q->velocity + static_cast<float>(aQScale) * deltaQ->velocity;

    //  Compute the constants that define the
    //  torque curve line.
    double b;
    double d;
    double omegaE = car->omegaE;
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
    double density = car->density;
    double Cd = car->Cd;
    double area = car->area;
    double Fd = 0.5 * density * area * Cd * v * v;

    //  Compute the force of rolling friction. Because
    //  the G constant has a negative sign, the value 
    //  computed here will be negative
    double gravity = car->gravity;
    double muR = car->muR;
    double mass = car->mass;
    double Fr = muR * mass * gravity;

    //  Compute the right-hand sides of the six ODEs
    //  newQ[0] is the intermediate value of velocity.
    //  The acceleration of the car is determined by 
    //  whether the car is accelerating, cruising, or
    //  braking. The braking acceleration is assumed to
    //  be a constant -5.0 m/s^2.   
    // Accelerating
    int gearNumber = car->gearNumber;
    double gearRatio = car->gearRatio[gearNumber];
    double finalDriveRatio = car->finalDriveRatio;
    double wheelRadius = car->wheelRadius;
    double pi = acos(-1.0);
    //if (m_car.accelerationInput < m_car.inputDeadZone)
    if (m_car.isAccelerating == true)
    {
        double c1 = -Fd / mass;
        double tmp = gearRatio * finalDriveRatio / wheelRadius;
        double c2 = 60.0 * tmp * tmp * b * v / (2.0 * pi * mass);
        double c3 = (tmp * d + Fr) / mass;
        //dq[0] = ds * (c1 + c2 + c3);
        dq->velocity.x = ds * (c1 + c2 + c3); // ToDo: update 3D x, y, z motion;
    }
    // braking
    else if (m_car.isBraking == true)
    {
        //  Only brake if the velocity is positive.
        //if (newQ[0] > 0.1) 
        if (newQ.velocity.Length() > 0.1)
        {
            //dq[0] = ds * (-m_car.maxBrakeRate); // temp for testing, ToDO: modify braking rate by brake input 
            dq->velocity.x = ds * (-m_car.maxBrakeRate); // temp for testing, ToDO: modify braking rate by brake input 
        }
        else
        {
            //dq[0] = 0.0;
            dq->velocity.x = 0.0;
        }
    }
    else
    {
        //dq[0] = 0.0;
        dq->velocity.x = 0.0;
    }

    //  Compute right-hand side values.
    dq->position.x = ds * newQ.velocity.x;
    dq->velocity.y = 0.0;
    dq->position.y = 0.0;
    dq->velocity.z = 0.0;
    dq->position.z = 0.0;

    return;
}

//************************************************************
//  This method solves for the car motion using a
//  4th-order Runge-Kutta solver
//************************************************************
void Vehicle::RungeKutta4(struct Car* car, double ds)
{
    //  Define a convenience variable to make the
    //  code more readable
    int numEqns = car->numEqns;

    //  Retrieve the current values of the dependent
    //  and independent variables.
    double s = car->s;
    Motion q = car->q;

    Motion dq1;
    Motion dq2;
    Motion dq3;
    Motion dq4;

    // Compute the four Runge-Kutta steps, The return 
    // value of carRightHandSide method is an array
    // of delta-q values for each of the four steps.
    RightHandSide(car, &q, &q, ds, 0.0, &dq1);
    RightHandSide(car, &q, &dq1, ds, 0.5, &dq2);
    RightHandSide(car, &q, &dq2, ds, 0.5, &dq3);
    RightHandSide(car, &q, &dq3, ds, 1.0, &dq4);

    //  Update the dependent and independent variable values
    //  at the new dependent variable location and store the
    //  values in the ODE object arrays.
    car->s = car->s + ds;
    q.position.x = static_cast<float>(q.position.x + (dq1.position.x + 2.0 * dq2.position.x + 2.0 * dq3.position.x + dq4.position.x) / numEqns);
    car->q.position.x = q.position.x;
    q.position.y = static_cast<float>(q.position.y + (dq1.position.y + 2.0 * dq2.position.y + 2.0 * dq3.position.y + dq4.position.y) / numEqns);
    car->q.position.y = q.position.y;
    q.position.z = static_cast<float>(q.position.z + (dq1.position.z + 2.0 * dq2.position.z + 2.0 * dq3.position.z + dq4.position.z) / numEqns);
    car->q.position.z = q.position.z;
    q.velocity.x = static_cast<float>(q.velocity.x + (dq1.velocity.x + 2.0 * dq2.velocity.x + 2.0 * dq3.velocity.x + dq4.velocity.x) / numEqns);
    car->q.velocity.x = q.velocity.x;
    q.velocity.y = static_cast<float>(q.velocity.y + (dq1.velocity.y + 2.0 * dq2.velocity.y + 2.0 * dq3.velocity.y + dq4.velocity.y) / numEqns);
    car->q.velocity.y = q.velocity.y;
    q.velocity.z = static_cast<float>(q.velocity.z + (dq1.velocity.z + 2.0 * dq2.velocity.z + 2.0 * dq3.velocity.z + dq4.velocity.z) / numEqns);
    car->q.velocity.z = q.velocity.z;

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

void Vehicle::UpdateModel(const double aTimer)
{
    double wheelTurnRads = GetWheelRotationRadians(aTimer) + m_testRotation;
    m_testRotation = wheelTurnRads;
    DirectX::SimpleMath::Matrix updateMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_car.q.position);
    DirectX::SimpleMath::Quaternion testQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, 0.0, 0.0);
    DirectX::SimpleMath::Quaternion rotQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(wheelTurnRads, 0.0, 0.0);
    DirectX::SimpleMath::Matrix rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(-wheelTurnRads);

    m_carModel.bodyMatrix.Transform(updateMatrix, testQuat, m_carModel.bodyMatrix);
    m_carModel.bodyTopMatrix.Transform(updateMatrix, testQuat, m_carModel.bodyTopMatrix);
    m_carModel.frontAxelMatrix.Transform(updateMatrix, testQuat, m_carModel.frontAxelMatrix);
    
  
    m_carModel.bodyMatrix *= m_carModel.localBodyMatrix;
    m_carModel.bodyTopMatrix *= m_carModel.localBodyTopMatrix;
    m_carModel.frontAxelMatrix *= m_carModel.localFrontAxelMatrix;

    DirectX::SimpleMath::Matrix testAxel = DirectX::SimpleMath::Matrix::Identity;
    testAxel = rotMatrix;
    testAxel *= updateMatrix;
    m_carModel.rearAxelMatrix = testAxel;
    m_carModel.rearAxelMatrix *= m_carModel.localRearAxelMatrix;

    m_carModel.frontAxelMatrix = testAxel;
    m_carModel.frontAxelMatrix *= m_carModel.localFrontAxelMatrix;
    ///m_carModel.rearAxelMatrix = testAxel;


    /*
    m_carModel.zeroRearAxelMatrix *= rotMatrix;
    m_carModel.rearAxelMatrix = updateMatrix * m_carModel.localRearAxelMatrix * m_carModel.zeroRearAxelMatrix;
    m_carModel.rearAxelMatrix = updateMatrix * m_carModel.localRearAxelMatrix * rotMatrix;
    */

    /*
    m_carModel.rearAxelMatrix *= m_carModel.localRearAxelMatrix;
    m_carModel.rearAxelMatrix.Transform(updateMatrix, rotQuat, m_carModel.rearAxelMatrix);
    m_carModel.rearAxelMatrix *= rotMatrix;
    */


    /*
    m_carModel.bodyMatrix = updateMatrix * m_carModel.localBodyMatrix;
    m_carModel.bodyTopMatrix = updateMatrix * m_carModel.localBodyTopMatrix;
    m_carModel.frontAxelMatrix = updateMatrix * m_carModel.localFrontAxelMatrix;
    m_carModel.rearAxelMatrix = updateMatrix * m_carModel.localRearAxelMatrix;  
    */
}

void Vehicle::UpdateVehicle(const double aTimer, const double aTimeDelta)
{   
    RungeKutta4(&m_car, aTimeDelta);

    double testV2 = m_car.speed;
    double time = m_car.s;
    double x = m_car.q.position.x;
    double vx = m_car.q.velocity.x;

    int gear = m_car.gearNumber;
    double rpm = m_car.omegaE;

    double oldGearRatio;
    double newGearRatio;
    //  Compute the new engine rpm value
    int gearNumber = m_car.gearNumber;
    double gearRatio = m_car.gearRatio[gearNumber];
    m_car.omegaE = vx * 60.0 * gearRatio * m_car.finalDriveRatio / (2.0 * Utility::GetPi() * m_car.wheelRadius);

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
