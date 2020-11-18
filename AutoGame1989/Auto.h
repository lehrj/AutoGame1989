#pragma once
#include <vector>
#include "Environment.h"
#include "Utility.h"
#include "AutoPlay.h"

class Auto
{
public:

    Auto();
    ~Auto();

    void BuildEnvironSelectStrings();
    float GetEnvironScale() const { return pEnvironment->GetScale(); };
    std::vector<std::vector<std::string>> GetEnvironSelectStrings() const { return m_environSelectStrings; };
    DirectX::SimpleMath::Vector3 GetEnvironWindVector() const { return pEnvironment->GetWindVector(); };
    std::vector<Fixture> GetEnvironFixtureBucket() { return pEnvironment->GetFixtureBucket(); };
    DirectX::XMVECTORF32 GetTerrainColor() const { return pEnvironment->GetEnvironColor(); };
    std::vector<std::string> GetUIStrings() const { return m_uiStrings; };
    double GetWindDirectionRad() const { return pEnvironment->GetWindDirection(); };

    void LoadEnvironment(const int aIndex);
    void SetCharacter(const int aCharacterIndex);
    void SetEnvironment(const int aEnvironmentIndex);

    void ZeroUIandRenderData();


private:

    void LoadCharacterTraits();
    Environment*                                pEnvironment;
    AutoPlay*                                   pPlay;
    double                                      m_xWindow;
    double                                      m_yWindow;
    double                                      m_zWindow;

    int                                         m_selectedCharacter;
    int                                         m_selectedEnvironment;

    std::vector<std::string>                    m_uiStrings;
    std::vector<std::vector<std::string>>       m_environSelectStrings;

};

