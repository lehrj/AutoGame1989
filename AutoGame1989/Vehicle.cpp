#include "pch.h"
#include "Vehicle.h"


Vehicle::Vehicle()
{
    InitializeVehicle();
}

void Vehicle::InitializeVehicle()
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
    m_car.mode = "accelerating";   //  accelerating, cruising, or braking
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

    m_car.steeringAngle = 0.0;
    m_car.position = DirectX::SimpleMath::Vector3::Zero;
    m_car.heading = DirectX::SimpleMath::Vector3::Zero;
    m_car.speed = 0.0;
    m_car.velocity = DirectX::SimpleMath::Vector3::Zero;
    

}

void Vehicle::UpdateVehicle()
{

}