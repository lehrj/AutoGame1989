#include "pch.h"
#include "Auto.h"
#include <fstream>
#include <sstream>

Auto::Auto()
{
    pEnvironment = new Environment();
    //SetCharacter(0); // ToDo WLJ : add error checking 

    BuildEnvironSelectStrings();

}

Auto::~Auto()
{
    delete pEnvironment;
}

void Auto::BuildEnvironSelectStrings()
{
    m_environSelectStrings.clear();
    const int environCount = pEnvironment->GetNumerOfEnvirons();

    std::vector<std::string> strVec;

    for (int i = 0; i < environCount; ++i)
    {
        std::string inString = pEnvironment->GetEnvironName(i);
        strVec.push_back(inString);
    }
    m_environSelectStrings.push_back(strVec);
    strVec.clear();

    for (int i = 0; i < environCount; ++i)
    {
        std::string inString = "Air Density = " + pEnvironment->GetAirDensityString(i) + " kg/m^3";
        strVec.push_back(inString);
    }
    m_environSelectStrings.push_back(strVec);
    strVec.clear();

    for (int i = 0; i < environCount; ++i)
    {
        std::string inString = "Gravity = " + pEnvironment->GetGravityString(i) + " m/s^2";
        strVec.push_back(inString);
    }
    m_environSelectStrings.push_back(strVec);
    strVec.clear();

    for (int i = 0; i < environCount; ++i)
    {
        std::string inString = "Wind X = " + pEnvironment->GetWindXString(i) + " m/s ";
        strVec.push_back(inString);
    }
    m_environSelectStrings.push_back(strVec);
    strVec.clear();

    for (int i = 0; i < environCount; ++i)
    {
        std::string inString = "Wind Z = " + pEnvironment->GetWindZString(i) + " m/s ";
        strVec.push_back(inString);
    }
    m_environSelectStrings.push_back(strVec);
}

void Auto::LoadEnvironment(const int aIndex)
{
    if (aIndex < 0 || aIndex > pEnvironment->GetNumerOfEnvirons() - 1)
    {
        m_selectedEnvironment = 0;
        pEnvironment->UpdateEnvironment(0);

    }
    else
    {
        m_selectedEnvironment = aIndex;
        pEnvironment->UpdateEnvironment(m_selectedEnvironment);
 
    }
}

void Auto::SetEnvironment(const int aEnvironmentIndex)
{
    if (aEnvironmentIndex < 0 || aEnvironmentIndex > pEnvironment->GetNumerOfEnvirons() - 1)
    {
        m_selectedEnvironment = 0;
    }
    else
    {
        m_selectedEnvironment = aEnvironmentIndex;
    }
    LoadEnvironment(aEnvironmentIndex);
}