#pragma once



struct WheelData
{
    int wheelID{};
    bool isSteerable{};
    bool isGrounded{};
    DirectX::SimpleMath::Vector3 localPosition;
    DirectX::SimpleMath::Vector3 worldPosition;
    DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;

    DirectX::SimpleMath::Vector3 axle;
    DirectX::SimpleMath::Vector3 terrainNormal;
    float radius{};
    float friction{};
    float sidewallHeight{};
    float width{};
    float tireDampening{};
    float mass{};
    float angularVelocity{};  // in radians per second


    // Pacejka magic formula data
    float C; // Shape factor	a0	C = a0
    float D; // Peak factor	a1, a2, a15	D = Fz *(a1·Fz + a2) *(1 – a15·γ2)
    float BCD; //	Stiffness	a3, a4, a5	BCD = a3 · sin(atan(Fz / a4) * 2) *(1 – a5 * | γ | )
    float B; //	Stiffness factor	BCD, C, D	B = BCD / (C*D)
    float E; //	Curvature factor	a6, a7, a16, a17	E = (a6·Fz + a7) *(1 –(a16·γ + a17)*sign(slip + H))
    float H; //	Horizontal shift	a8, a9, a10	H = a8·Fz + a9 + a10*γ
    float V; // Vertical shift	a11, a12, a13, a14	V = a11·Fz + a12 + (a13·Fz + a14)·γ*Fz
    float Bx1; // (composite)Bx1 = B *(slip + H)
};

class Wheel
{
    public:      
        void InitializeWheel(DirectX::SimpleMath::Vector3 aLocalPos, DirectX::SimpleMath::Vector3 aWorldPos, bool aIsSteerable, float aRadius, float aWidth, float aMass);
        void UpdateWheel();


    private:
        void SetAngularVelocity(const float aAngVel) { m_wheelData.angularVelocity = aAngVel; };
        void SetAxelPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.axle = aPos; };
        void SetForward(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.forward = aPos; };
        void SetLocalPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.localPosition = aPos; };
        void SetMass(const float aMass) { m_wheelData.mass = aMass; };
        void SetRadius(float aRadius) { m_wheelData.radius = aRadius; };
        void SetSteerable(const bool aIsSteerable) { m_wheelData.isSteerable = aIsSteerable; };
        void SetTerrainNormal(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.terrainNormal = aPos; };
        void SetUp(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.up = aPos; };
        void SetWidth(float aWidth) { m_wheelData.radius = aWidth; };
        void SetWorldPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.worldPosition = aPos; };
        
        void UpdateMagicFormula();
        WheelData m_wheelData;

};

