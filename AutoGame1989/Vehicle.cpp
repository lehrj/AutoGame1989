#include "pch.h"
#include "Vehicle.h"


Vehicle::Vehicle()
{
    //InitializeVehicle();
}

//*************************************************************
//  This method loads the right-hand sides for the car ODEs
//*************************************************************
void Vehicle::carRightHandSide(struct Car* car, double* q, double* deltaQ, double ds, double qScale, double* dq) 
{
    //  q[0] = vx = dxdt
    //  q[1] = x
    //  q[2] = vy = dydt
    //  q[3] = y
    //  q[4] = vz = dzdt
    //  q[5] = z
    double newQ[6]; // intermediate dependent variable values.
    double mass;
    double area;
    double density;
    double Cd;
    double vx;
    double vy;
    double vz;
    double v;
    double Fd;
    double G = -9.81;
    double b;
    double d;
    double omegaE;
    double Fr;
    double c1;
    double c2;
    double c3;
    double tmp;
    double muR;
    double redline;
    double finalDriveRatio;
    double gearRatio;
    int gearNumber;
    double wheelRadius;
    double pi = acos(-1.0);

    int i;

    mass = car->mass;
    area = car->area;
    density = car->density;
    Cd = car->Cd;
    omegaE = car->omegaE;
    muR = car->muR;
    redline = car->redline;
    finalDriveRatio = car->finalDriveRatio;
    wheelRadius = car->wheelRadius;
    gearNumber = car->gearNumber;
    gearRatio = car->gearRatio[gearNumber];

    //  Compute the intermediate values of the 
    //  dependent variables.
    for (i = 0; i < 6; ++i) 
    {
        newQ[i] = q[i] + qScale * deltaQ[i];
    }

    //  Compute the constants that define the
    //  torque curve line.
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
    vx = newQ[0];
    vy = newQ[2];
    vz = newQ[4];

    //  Compute the velocity magnitude. The 1.0e-8 term
    //  ensures there won't be a divide by zero later on
    //  if all of the velocity components are zero.
    v = sqrt(vx * vx + vy * vy + vz * vz) + 1.0e-8;

    //  Compute the total drag force.
    Fd = 0.5 * density * area * Cd * v * v;

    //  Compute the force of rolling friction. Because
    //  the G constant has a negative sign, the value 
    //  computed here will be negative
    Fr = muR * mass * G;

    //  Compute the right-hand sides of the six ODEs
    //  newQ[0] is the intermediate value of velocity.
    //  The acceleration of the car is determined by 
    //  whether the car is accelerating, cruising, or
    //  braking. The braking acceleration is assumed to
    //  be a constant -5.0 m/s^2.   
    //if (!strcmp(car->mode, "accelerating")) 
    //if (m_car.accelerationInput < m_car.inputDeadZone)
    if (1 == 1)
    {
        c1 = -Fd / mass;
        tmp = gearRatio * finalDriveRatio / wheelRadius;
        c2 = 60.0 * tmp * tmp * b * v / (2.0 * pi * mass);
        c3 = (tmp * d + Fr) / mass;
        dq[0] = ds * (c1 + c2 + c3);
    }
    //else if (!strcmp(car->mode, "braking")) 
    //if(m_car.brakeInput < m_car.inputDeadZone)
    else if(1 == 0)
    {
        //  Only brake if the velocity is positive.
        if (newQ[0] > 0.1) 
        {
            dq[0] = ds * (-m_car.maxBrakeRate); // temp for testing, ToDO: modify braking rate by brake input 
        }
        else 
        {
            dq[0] = 0.0;
        }
    }
    else 
    {
        dq[0] = 0.0;
    }

    //  Compute right-hand side values.
    dq[1] = ds * newQ[0];
    dq[2] = 0.0;
    dq[3] = 0.0;
    dq[4] = 0.0;
    dq[5] = 0.0;

    return;
}

//************************************************************
//  This method solves for the car motion using a
//  4th-order Runge-Kutta solver
//************************************************************
void Vehicle::carRungeKutta4(struct Car* car, double ds) 
{

    int j;
    int numEqns;
    double s;
    double* q;
    double* dq1;
    double* dq2;
    double* dq3;
    double* dq4;

    //  Define a convenience variable to make the
    //  code more readable
    numEqns = car->numEqns;

    //  Allocate memory for the arrays.
    q = (double*)malloc(numEqns * sizeof(double));
    dq1 = (double*)malloc(numEqns * sizeof(double));
    dq2 = (double*)malloc(numEqns * sizeof(double));
    dq3 = (double*)malloc(numEqns * sizeof(double));
    dq4 = (double*)malloc(numEqns * sizeof(double));

    //  Retrieve the current values of the dependent
    //  and independent variables.
    s = car->s;
    for (j = 0; j < numEqns; ++j) 
    {
        q[j] = car->q[j];
    }

    // Compute the four Runge-Kutta steps, The return 
    // value of carRightHandSide method is an array
    // of delta-q values for each of the four steps.
    carRightHandSide(car, q, q, ds, 0.0, dq1);
    carRightHandSide(car, q, dq1, ds, 0.5, dq2);
    carRightHandSide(car, q, dq2, ds, 0.5, dq3);
    carRightHandSide(car, q, dq3, ds, 1.0, dq4);

    //  Update the dependent and independent variable values
    //  at the new dependent variable location and store the
    //  values in the ODE object arrays.
    car->s = car->s + ds;

    for (j = 0; j < numEqns; ++j) 
    {
        q[j] = q[j] + (dq1[j] + 2.0 * dq2[j] + 2.0 * dq3[j] + dq4[j]) / 6.0;
        car->q[j] = q[j];
    }

    //  Free up memory
    free(q);
    free(dq1);
    free(dq2);
    free(dq3);
    free(dq4);

    return;
}

void Vehicle::DrawModel(DirectX::SimpleMath::Matrix aWorld, DirectX::SimpleMath::Matrix aView, DirectX::SimpleMath::Matrix aProj, const double aTimer)
{
    DirectX::SimpleMath::Matrix world = aWorld;

    DirectX::SimpleMath::Matrix view = aView;
    DirectX::SimpleMath::Matrix proj = aProj;

    m_carModel.bodyTop->Draw(m_carModel.bodyTopMatrix, view, proj);
    m_carModel.body->Draw(m_carModel.bodyMatrix, view, proj);
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

void Vehicle::InitializeModel(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> aContext)
{
    // porche boxter base dimensions - 4.3942m L x 1.8034m W x 1.27m H, wheel diameter 0.3186m
    const float wheelDiameter = 0.508;
    const float length = 4.3942;
    const float width = 1.8034;
    const float height = 1.27;
    const float axelLength = width + 0.6;
    const float wheelBase = 2.47396;
    DirectX::SimpleMath::Vector3 carBodySize(length, height - wheelDiameter, width);

    m_carModel.body = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), carBodySize);
    m_carModel.frontAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, wheelDiameter, 16);
    m_carModel.rearAxel = DirectX::GeometricPrimitive::CreateCylinder(aContext.Get(), axelLength, wheelDiameter, 16);

    m_carModel.bodyMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.frontAxelMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.rearAxelMatrix = DirectX::SimpleMath::Matrix::Identity;

    m_carModel.bodyMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0, (height + (wheelDiameter)), 0.0));

    DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(90.0));

    m_carModel.frontAxelMatrix *= rotMat;    
    m_carModel.frontAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(wheelBase * .5, wheelDiameter * 0.5, 0.0));
    m_carModel.rearAxelMatrix *= rotMat;
    m_carModel.rearAxelMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-wheelBase * .5, wheelDiameter * 0.5, 0.0));
    
    const float topIndent = 0.2;
    const float topHeight = height - wheelDiameter;
    const float topLength = length * .6;
    const float roofHeightAlignment = height + wheelDiameter + topHeight + topHeight;
    const float roofLengthAlignment = length - topLength - topIndent;
    DirectX::SimpleMath::Vector3 carBodyTopSize(topLength, topHeight, width - topIndent);
    m_carModel.bodyTop = DirectX::GeometricPrimitive::CreateBox(aContext.Get(), carBodyTopSize);
    m_carModel.bodyTopMatrix = DirectX::SimpleMath::Matrix::Identity;
    m_carModel.bodyTopMatrix += DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3( - roofLengthAlignment, roofHeightAlignment, 0.0));
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
    //m_car.mode = "accelerating";   //  accelerating, cruising, or braking
    m_car.gearRatio[1] = 3.82;
    m_car.gearRatio[2] = 2.20;
    m_car.gearRatio[3] = 1.52;
    m_car.gearRatio[4] = 1.22;
    m_car.gearRatio[5] = 1.02;
    m_car.gearRatio[6] = 0.84;

    m_car.numEqns = 6;
    m_car.s = 0.0;      //  time 
    m_car.q[0] = 0.0;   //  vx 
    m_car.q[1] = 0.0;   //  x  
    m_car.q[2] = 0.0;   //  vy 
    m_car.q[3] = 0.0;   //  y  
    m_car.q[4] = 0.0;   //  vz 
    m_car.q[5] = 0.0;   //  z  

    m_car.inputDeadZone = 0.05;
    m_car.accelerationInput = 0.0;
    m_car.brakeInput = 0.0;
    m_car.maxAccelerationRate = 1.0;
    m_car.maxBrakeRate = 1.0;
    m_car.steeringAngle = 0.0;
    m_car.position = DirectX::SimpleMath::Vector3::Zero;
    m_car.heading = DirectX::SimpleMath::Vector3::Zero;
    m_car.speed = 0.0;
    m_car.velocity = DirectX::SimpleMath::Vector3::Zero;

    InitializeModel(aContext);
}

void Vehicle::ResetVehicle()
{
    m_car.gearNumber = 1;         

    m_car.steeringAngle = 0.0;
    m_car.position = DirectX::SimpleMath::Vector3::Zero;
    m_car.heading = DirectX::SimpleMath::Vector3::Zero;
    m_car.speed = 0.0;
    m_car.velocity = DirectX::SimpleMath::Vector3::Zero;
}

void Vehicle::UpdateModel(const double aTimer)
{
    DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateRotationY(cos(aTimer) * 0.021);
    DirectX::SimpleMath::Matrix updateMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(cos(aTimer) * 0.02, 0.0, 0.0));
    updateMatrix *= rotMat;
    m_carModel.bodyTopMatrix *= updateMatrix;
    m_carModel.bodyMatrix *= updateMatrix;
    m_carModel.frontAxelMatrix *= updateMatrix;
    m_carModel.rearAxelMatrix *= updateMatrix;
}

void Vehicle::UpdateVehicle(const double aTimer, const double aTimeDelta)
{
    double testV1 = m_car.speed;
    carRungeKutta4(&m_car, aTimeDelta);

    double testV2 = m_car.speed;
    double time = m_car.s;
    double x = m_car.q[1];
    double vx = m_car.q[0];
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

    UpdateModel(aTimer);
}
