#include "pch.h"
#include "Environment.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>


Environment::Environment()
{
    //bool result = InitializeTerrain(EnvironmentType::ENIVRONMENTTYPE_STARTUP);
    bool result = InitializeTerrain(EnvironmentType::ENVIRONMENTTYPE_CLEANTEST);
    LoadEnvironmentData();
    CreateDataStrings();
    const int startEnviron = 0;  // ToDo: add error checking 
    m_currentEnviron = m_environs[startEnviron];
    //BuildFlagVertex(m_environs[startEnviron].holePosition);
    //BuildHoleVertex(m_environs[startEnviron].holePosition);
    //LoadFixtureBucket12th(); 
}

void Environment::BuildFlagVertex(DirectX::SimpleMath::Vector3 aPos)
{
    const float poleHeight = .1;
    const float flagWidth = .02;
    const float flagHeight = .01;
    const DirectX::XMVECTORF32 flagColor = DirectX::Colors::Red;
    const DirectX::XMVECTORF32 poleColor = DirectX::Colors::White;

    DirectX::SimpleMath::Vector3 poleBase = DirectX::SimpleMath::Vector3::Zero;
    DirectX::SimpleMath::Vector3 poleTop = poleBase;
    poleTop.y += poleHeight;
    DirectX::SimpleMath::Vector3 flagTip = poleTop;
    flagTip.y -= flagHeight;
    flagTip.x -= flagWidth;
    flagTip.z -= flagWidth;
    double windDirection = GetWindDirection();
    flagTip = DirectX::SimpleMath::Vector3::Transform(flagTip, DirectX::SimpleMath::Matrix::CreateRotationY(static_cast<float>(windDirection)));

    DirectX::SimpleMath::Vector3 flagBottom = poleTop;
    flagBottom.y -= flagHeight + flagHeight;

    DirectX::VertexPositionColor poleBaseVertex(poleBase + aPos, poleColor);
    DirectX::VertexPositionColor poleTopVertex(poleTop + aPos, poleColor);
    DirectX::VertexPositionColor flagTopVertex(poleTop + aPos, flagColor);
    DirectX::VertexPositionColor flagTipVertex(flagTip + aPos, flagColor);
    DirectX::VertexPositionColor flagBottomVertex(flagBottom + aPos, flagColor);

    m_flagVertex.clear();
    m_flagVertex.push_back(poleBaseVertex);
    m_flagVertex.push_back(poleTopVertex);
    m_flagVertex.push_back(flagTopVertex);
    m_flagVertex.push_back(flagTipVertex);
    m_flagVertex.push_back(flagBottomVertex);
}

void Environment::BuildHoleVertex(DirectX::SimpleMath::Vector3 aPos)
{
    m_holeVertex.clear();
    const int vertexCount = m_holeResolution;
    const float holeRadius= static_cast<float>(m_holeRadius * m_currentEnviron.scale);
    for (int i = 0; i <= vertexCount; ++i)
    {
        double t = Utility::GetPi() * 2 * i / vertexCount;
        m_holeVertex.push_back(DirectX::VertexPositionColor(DirectX::SimpleMath::Vector3(static_cast<float>((holeRadius * cos(t))), static_cast<float>(m_landingHeight), static_cast<float>((holeRadius * -sin(t)))) + aPos, DirectX::Colors::Gray));
    }
}

bool Environment::BuildTerrainModel()
{
    m_terrainModel.clear();
    m_terrainModel.resize((m_terrainHeight - 1) * (m_terrainWidth - 1) * 6);
    if (m_terrainModel.size() < 1)
    {
        return false;
    }

    int index = 0;

    for (int j = 0; j < (m_terrainHeight - 1); ++j)
    {
        for (int i = 0; i < (m_terrainWidth - 1); ++i)
        {
            // Get the indexes to the four points of the quad
            int index1 = (m_terrainWidth * j) + i;          // Upper left.
            int index2 = (m_terrainWidth * j) + (i + 1);      // Upper right.
            int index3 = (m_terrainWidth * (j + 1)) + i;      // Bottom left.
            int index4 = (m_terrainWidth * (j + 1)) + (i + 1);  // Bottom right.

            // Now create two triangles for that quad
            // Triangle 1 - Upper left
            m_terrainModel[index].position = m_heightMap[index1].position;
            m_terrainModel[index].normal = m_heightMap[index1].normal;
            ++index;

            // Triangle 1 - Upper right
            m_terrainModel[index].position = m_heightMap[index2].position;
            m_terrainModel[index].normal = m_heightMap[index2].normal;
            ++index;

            // Triangle 1 - Bottom left
            m_terrainModel[index].position = m_heightMap[index3].position;
            m_terrainModel[index].normal = m_heightMap[index3].normal;
            ++index;

            // Triangle 2 - Bottom left
            m_terrainModel[index].position = m_heightMap[index3].position;
            m_terrainModel[index].normal = m_heightMap[index3].normal;
            ++index;

            // Triangle 2 - Upper right.
            m_terrainModel[index].position = m_heightMap[index2].position;
            m_terrainModel[index].normal = m_heightMap[index2].normal;
            ++index;

            // Triangle 2 - Bottom right.
            m_terrainModel[index].position = m_heightMap[index4].position;
            m_terrainModel[index].normal = m_heightMap[index4].normal;
            ++index;
        }
    }

    // to do: m_heightMap.clear(); once testing is complete
    return true;
}

bool Environment::CalculateTerrainNormals()
{
    std::vector<DirectX::SimpleMath::Vector3> normals;
    normals.clear();
    normals.resize((m_terrainHeight - 1) * (m_terrainWidth - 1));
    if (normals.size() < 1)
    {
        return false;
    }

    for (int j = 0; j < (m_terrainHeight - 1); ++j)
    {
        for (int i = 0; i < (m_terrainWidth - 1); ++i)
        {
            int index1 = ((j + 1) * m_terrainWidth) + i;      // Bottom left vertex.
            int index2 = ((j + 1) * m_terrainWidth) + (i + 1);  // Bottom right vertex.
            int index3 = (j * m_terrainWidth) + i;          // Upper left vertex.

            // Get three vertices from the face.
            DirectX::SimpleMath::Vector3 vertex1 = m_heightMap[index1].position;
            DirectX::SimpleMath::Vector3 vertex2 = m_heightMap[index2].position;
            DirectX::SimpleMath::Vector3 vertex3 = m_heightMap[index3].position;

            // Calculate the two vectors for this face.
            DirectX::SimpleMath::Vector3 vector1 = vertex1 - vertex3;
            DirectX::SimpleMath::Vector3 vector2 = vertex3 - vertex2;

            DirectX::SimpleMath::Vector3 vector3 = vertex2 - vertex1;
            DirectX::SimpleMath::Vector3 vector4 = vertex3 - vertex1;

            //DirectX::SimpleMath::Vector3 vector5 = DirectX::XMVector3Cross(vector1, vector2);
            DirectX::SimpleMath::Vector3 vector5 = DirectX::XMVector3Cross(vector2, vector1);
            vector5.Normalize();
            DirectX::SimpleMath::Vector3 vector6 = DirectX::XMVector3Cross(vector3, vector4);
            vector6.Normalize();

            if (vector5 != vector6)
            {
                int testBreak = 0;
            }


            int index = (j * (m_terrainWidth - 1)) + i;

            normals[index] = DirectX::XMVector3Cross(vector1, vector2);
            normals[index] = vector6;
            normals[index].Normalize();
        }
    }

    // Now go through all the vertices and take a sum of the face normals that touch this vertex.
    for (int j = 0; j < m_terrainHeight; j++)
    {
        for (int i = 0; i < m_terrainWidth; i++)
        {
            DirectX::SimpleMath::Vector3 sum = DirectX::SimpleMath::Vector3::Zero;

            // Bottom left face.
            if (((i - 1) >= 0) && ((j - 1) >= 0))
            {
                int index = ((j - 1) * (m_terrainWidth - 1)) + (i - 1);
                sum += normals[index];
            }
            // Bottom right face.
            if ((i < (m_terrainWidth - 1)) && ((j - 1) >= 0))
            {
                int index = ((j - 1) * (m_terrainWidth - 1)) + i;
                sum += normals[index];
            }
            // Upper left face.
            if (((i - 1) >= 0) && (j < (m_terrainHeight - 1)))
            {
                int index = (j * (m_terrainWidth - 1)) + (i - 1);
                sum += normals[index];
            }
            // Upper right face.
            if ((i < (m_terrainWidth - 1)) && (j < (m_terrainHeight - 1)))
            {
                int index = (j * (m_terrainWidth - 1)) + i;
                sum += normals[index];
            }

            int index = (j * m_terrainWidth) + i;

            sum.Normalize();
            m_heightMap[index].normal = sum;
        }
    }

    return true;
}

bool Environment::CheckTerrainTriangleHeight(DirectX::XMFLOAT3& aPos, DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2) const
{
    // Starting position of the ray that is being cast
    DirectX::XMFLOAT3 startVector(aPos.x, 0.0f, aPos.z);

    // The direction the ray is being cast
    DirectX::XMFLOAT3 directionVector(0.0f, -1.0f, 0.0f);

    // Calculate the two edges from the three points given
    DirectX::SimpleMath::Vector3 edge1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
    DirectX::SimpleMath::Vector3 edge2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

    // Calculate the normal of the triangle from the two edges // ToDo use cross prod funcs
    DirectX::SimpleMath::Vector3 normal;

    edge1.Cross(edge2, normal);
    normal.Normalize();

    // Find the distance from the origin to the plane.
    float distance = ((-normal.x * v0.x) + (-normal.y * v0.y) + (-normal.z * v0.z));

    // Get the denominator of the equation.
    float denominator = ((normal.x * directionVector.x) + (normal.y * directionVector.y) + (normal.z * directionVector.z));

    // Make sure the result doesn't get too close to zero to prevent divide by zero.
    if (fabs(denominator) < 0.0001f)
    {
        return false;
    }

    // Get the numerator of the equation.
    float numerator = -1.0f * (((normal.x * startVector.x) + (normal.y * startVector.y) + (normal.z * startVector.z)) + distance);

    // Calculate where we intersect the triangle.
    float t = numerator / denominator;

    // Find the intersection vector.
    DirectX::SimpleMath::Vector3 Q;
    Q.x = startVector.x + (directionVector.x * t);
    Q.y = startVector.y + (directionVector.y * t);
    Q.z = startVector.z + (directionVector.z * t);

    // Find the three edges of the triangle.
    DirectX::SimpleMath::Vector3 e1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
    DirectX::SimpleMath::Vector3 e2(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
    DirectX::SimpleMath::Vector3 e3(v0.x - v2.x, v0.y - v2.y, v0.z - v2.z);

    // Calculate the normal for the first edge.
    DirectX::SimpleMath::Vector3 edgeNormal;
    e1.Cross(normal, edgeNormal);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    DirectX::SimpleMath::Vector3 temp(Q.x - v1.x, Q.y - v1.y, Q.z - v1.z);

    float determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return false;
    }

    // Calculate the normal for the second edge
    e2.Cross(normal, edgeNormal);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    temp.x = Q.x - v1.x;
    temp.y = Q.y - v1.y;
    temp.z = Q.z - v1.z;

    determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return false;
    }

    // Calculate the normal for the third edge.
    e3.Cross(normal, edgeNormal);

    // Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
    temp.x = Q.x - v2.x;
    temp.y = Q.y - v2.y;
    temp.z = Q.z - v2.z;

    determinant = ((edgeNormal.x * temp.x) + (edgeNormal.y * temp.y) + (edgeNormal.z * temp.z));

    // Check if it is outside.
    if (determinant > 0.001f)
    {
        return false;
    }

    // Now we have our height.
    aPos.y = Q.y;
    return true;
}

void Environment::CreateDataStrings()
{
    std::stringstream inVal;
    inVal.precision(Utility::GetNumericalPrecisionForUI());

    for (int i = 0; i < m_environs.size(); ++i)
    {
        inVal.str(std::string());
        inVal << std::fixed << m_environs[i].airDensity;
        m_environs[i].airDensityStr = inVal.str();

        inVal.str(std::string());
        inVal << std::fixed << m_environs[i].gravity;
        m_environs[i].gravityStr = inVal.str();

        inVal.str(std::string());
        inVal << std::fixed << m_environs[i].wind.x;
        m_environs[i].windXStr = inVal.str();

        inVal.str(std::string());
        inVal << std::fixed << m_environs[i].wind.y;
        m_environs[i].windYStr = inVal.str();

        inVal.str(std::string());
        inVal << std::fixed << m_environs[i].wind.z;
        m_environs[i].windZStr = inVal.str();

        inVal.str(std::string());
        inVal << std::fixed << m_environs[i].landingFrictionScale;
        m_environs[i].landingFrictionScaleStr = inVal.str();

        inVal.str(std::string());
        inVal << std::fixed << m_environs[i].landingHardnessScale;
        m_environs[i].landingHardnessScaleStr = inVal.str();
    }
}

std::vector<DirectX::VertexPositionColor> Environment::GetTerrainColorVertex()
{
    std::vector<DirectX::VertexPositionColor> vertPosColor;
    vertPosColor.clear();
    vertPosColor.resize(m_terrainModel.size());
    DirectX::XMFLOAT4 terrainColor(1.0, 1.0, 1.0, 1.0); // ToDo: for testing, implement color control

    for (int i = 0; i < vertPosColor.size(); ++i)
    {
        vertPosColor[i].position = m_terrainModel[i].position;
        vertPosColor[i].color = terrainColor;
    }

    return vertPosColor;
}

float Environment::GetTerrainHeightAtPos(DirectX::XMFLOAT3 aPos) const
{
    DirectX::SimpleMath::Vector3 prePos = aPos;
    bool foundHeightBarry = false;
    bool foundHeight = false;

    int i = 0;

    for (i; i < m_terrainModel.size(); ++i)
    {
        DirectX::XMFLOAT3 vertex1 = m_terrainModel[i].position;
        ++i;
        DirectX::XMFLOAT3 vertex2 = m_terrainModel[i].position;
        ++i;
        DirectX::XMFLOAT3 vertex3 = m_terrainModel[i].position;

        /*
        if (abs(aPos.x - vertex1.x) < .3 && abs(aPos.z - vertex1.z) < .3)
        {
            int brakeTest = 0;

            foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);

            if (foundHeight == true)
            {
                int testBreak = 0;
            }
            else
            {
                int testBreak = 0;
            }
        }
        if (abs(aPos.x - vertex3.x) < .3 && abs(aPos.z - vertex3.z) < .3)
        {
            int brakeTest = 0;

            foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);

            if (foundHeight == true)
            {
                int testBreak = 0;
            }
            else
            {
                int testBreak = 0;
            }
        }
        */

        /*
        if (abs(aPos.x - vertex2.x) < .3 && abs(aPos.z - vertex2.z) < .3)
        {
            int brakeTest = 0;

            foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);

            if (foundHeight == true)
            {
                int testBreak = 0;
            }
            else
            {
                int testBreak = 0;
            }
        }
        */

        foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);
        if (foundHeight == true)
        {
            return aPos.y;
        }

        float f = prePos.x;
        float g = prePos.z;
        DirectX::SimpleMath::Vector3 baryPos = DirectX::SimpleMath::Vector3::Barycentric(vertex1, vertex2, vertex3, f, g);

        if (baryPos.x <= 1.0f && baryPos.x >= 0.0f && baryPos.y <= 1.0f && baryPos.y >= 0.0f && baryPos.z <= 1.0f && baryPos.z >= 0.0f)
        {
            foundHeightBarry = true;
        }
        else
        {
            foundHeightBarry = false;
        }


        if (foundHeight == true && foundHeightBarry == false)
        {
            int testBreak2 = 0;
            testBreak2++;
        }

        if (foundHeight == false && foundHeightBarry == true)
        {
            int testBreak2 = 0;
            testBreak2++;
        }

        if (foundHeight != foundHeightBarry)
        {
            int testBreak2 = 0;
            testBreak2++;
        }

        int testBreak = 0;

        if (foundHeight)
        {
            f = prePos.x;
            g = prePos.z;
            baryPos = DirectX::SimpleMath::Vector3::Barycentric(vertex1, vertex2, vertex3, f, g);

            testBreak = 0;
            return aPos.y;
        }
    }
    float errorHeight = aPos.y;
    return errorHeight;
}

float Environment::GetTerrainHeightAtPos2(DirectX::XMFLOAT3 aPos) const
{
    DirectX::SimpleMath::Vector3 prePos = aPos;
    bool foundHeightBarry = false;
    bool foundHeight = false;

    int i = 0;

    for (i; i < m_terrainModel.size(); ++i)
    {
        DirectX::XMFLOAT3 vertex1 = m_terrainModel[i].position;
        ++i;
        DirectX::XMFLOAT3 vertex2 = m_terrainModel[i].position;
        ++i;
        DirectX::XMFLOAT3 vertex3 = m_terrainModel[i].position;

        /*
        if (abs(aPos.x - vertex1.x) < .3 && abs(aPos.z - vertex1.z) < .3)
        {
            int brakeTest = 0;

            foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);

            if (foundHeight == true)
            {
                int testBreak = 0;
            }
            else
            {
                int testBreak = 0;
            }
        }
        if (abs(aPos.x - vertex3.x) < .3 && abs(aPos.z - vertex3.z) < .3)
        {
            int brakeTest = 0;

            foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);

            if (foundHeight == true)
            {
                int testBreak = 0;
            }
            else
            {
                int testBreak = 0;
            }
        }
        */
        if (abs(aPos.x - vertex2.x) < .3 && abs(aPos.z - vertex2.z) < .3)
        {
            int brakeTest = 0;

            foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);

            if (foundHeight == true)
            {
                int testBreak = 0;
            }
            else
            {
                int testBreak = 0;
            }
        }
        float f = prePos.x;
        float g = prePos.z;
        DirectX::SimpleMath::Vector3 baryPos = DirectX::SimpleMath::Vector3::Barycentric(vertex1, vertex2, vertex3, f, g);

        if (baryPos.x <= 1.0f && baryPos.x >= 0.0f && baryPos.y <= 1.0f && baryPos.y >= 0.0f && baryPos.z <= 1.0f && baryPos.z >= 0.0f)
        {
            foundHeightBarry = true;
        }
        else
        {
            foundHeightBarry = false;
        }


        if (foundHeight == true && foundHeightBarry == false)
        {
            int testBreak2 = 0;
            testBreak2++;
        }

        if (foundHeight == false && foundHeightBarry == true)
        {
            int testBreak2 = 0;
            testBreak2++;
        }

        if (foundHeight != foundHeightBarry)
        {
            int testBreak2 = 0;
            testBreak2++;
        }

        int testBreak = 0;

        if (foundHeight)
        {
            f = prePos.x;
            g = prePos.z;
            baryPos = DirectX::SimpleMath::Vector3::Barycentric(vertex1, vertex2, vertex3, f, g);

            testBreak = 0;
            return aPos.y;
        }
    }
    float errorHeight = aPos.y;
    return errorHeight;
}

DirectX::SimpleMath::Vector3 Environment::GetTerrainNormal(DirectX::SimpleMath::Vector3 aPos) const
{
    bool foundHeight = false;
    int i = 0;
    /*
    if (aPos.z >= 0.0)
    {
        //i = m_terrainModel.size() / 2;
        //i -= 3;
        i == 2001;
    }
    */

    for (i; i < m_terrainModel.size(); ++i)
    {
        DirectX::XMFLOAT3 vertex1 = m_terrainModel[i].position;
        ++i;
        DirectX::XMFLOAT3 vertex2 = m_terrainModel[i].position;
        ++i;
        DirectX::XMFLOAT3 vertex3 = m_terrainModel[i].position;

        DirectX::SimpleMath::Vector3 pos = aPos;
        /*
        if (abs(aPos.x - vertex2.x) < .3 && abs(aPos.z - vertex2.z) < .3)
        {
            foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);
        }
        */
        foundHeight = CheckTerrainTriangleHeight(aPos, vertex1, vertex2, vertex3);
        if (foundHeight)
        {
            
            DirectX::SimpleMath::Vector3 norm1 = m_terrainModel[i - 2].normal;
            DirectX::SimpleMath::Vector3 norm2 = m_terrainModel[i - 1].normal;
            DirectX::SimpleMath::Vector3 norm3 = m_terrainModel[i].normal;
           
            float x = (norm1.x + norm2.x + norm3.x) / 3.0f;
            float y = (norm1.y + norm2.y + norm3.y) / 3.0f;
            float z = (norm1.z + norm2.z + norm3.z) / 3.0f;

            DirectX::SimpleMath::Vector3 norm(x, y, z);

            //norm = norm3;
            norm *= -1;            
            norm.Normalize();
            ///////////////////////////////////////////////////////////
            DirectX::SimpleMath::Vector3 a = m_terrainModel[i].position;
            DirectX::SimpleMath::Vector3 b = m_terrainModel[i - 1].position;
            DirectX::SimpleMath::Vector3 c;
            c.x = a.y * b.z - a.z * b.y;
            c.y = a.z * b.x - a.x * b.z;
            c.z = a.x * b.y - a.y * b.x;
            
            /// //
            DirectX::SimpleMath::Vector3 p3 = m_terrainModel[i - 2].position;
            DirectX::SimpleMath::Vector3 p2 = m_terrainModel[i - 1].position;
            DirectX::SimpleMath::Vector3 p1 = m_terrainModel[i].position;

            DirectX::SimpleMath::Vector3 U = p2 - p1;
            DirectX::SimpleMath::Vector3 V = p3 - p1;
            DirectX::SimpleMath::Vector3 testNormal;
            testNormal.x = (U.y * V.z) - (U.z * V.y);
            testNormal.y = (U.z * V.x) - (U.x * V.z);
            testNormal.z = (U.x * V.y) - (U.y * V.x);
            testNormal.Normalize();
            norm = testNormal;
            return norm;
        }
    }


    return -DirectX::SimpleMath::Vector3::UnitX;
}

std::vector<DirectX::VertexPositionNormalColor> Environment::GetTerrainPositionNormalColorVertex()
{
    std::vector<DirectX::VertexPositionNormalColor> vertPosNormColor;
    vertPosNormColor.clear();
    vertPosNormColor.resize(m_terrainModel.size());
    DirectX::XMFLOAT4 terrainColor(1.0, 1.0, 1.0, 1.0); // ToDo: for testing, implement color control

    for (int i = 0; i < vertPosNormColor.size(); ++i)
    {
        vertPosNormColor[i].position = m_terrainModel[i].position;
        vertPosNormColor[i].color = terrainColor;
        vertPosNormColor[i].normal = m_terrainModel[i].normal;
    }

    return vertPosNormColor;
}

// While this could be done once per environment update, future updates could have moment to moment wind changes
double Environment::GetWindDirection() const
{ 
    DirectX::SimpleMath::Vector3 windVec = m_currentEnviron.wind;
    DirectX::SimpleMath::Vector3 zeroDirection(-1.0, 0.0, -1.0);
    double direction = DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(DirectX::XMVector3Normalize(windVec), DirectX::XMVector3Normalize(zeroDirection)));
    if (DirectX::XMVectorGetY(DirectX::XMVector3Cross(windVec, zeroDirection)) > 0.0f)
    {
        direction = -direction;
    }

    return direction;
}

bool Environment::InitializeTerrain(EnvironmentType aEnviron)
{
    bool result = LoadHeightMap(aEnviron);
    if (!result)
    {
        return false;
    }
    
    result = CalculateTerrainNormals();
    if (!result)
    {
        return false;
    }

    result = BuildTerrainModel();
    if (!result)
    {
        return false;
    }

    ScaleTerrain(aEnviron);
    

    return true;
}

void Environment::LoadEnvironmentData()
{
    m_environs.clear();
    m_environs.resize(m_environsAvailable);

    int i = 0;

    m_environs[i].name = "12th";   
    m_environs[i].airDensity = 1.225;
    m_environs[i].gravity = -9.8;
    //m_environs[i].holePosition = DirectX::SimpleMath::Vector3(4.9f, 0.0f, 0.82f);
    m_environs[i].holePosition = DirectX::SimpleMath::Vector3(3.0f, 0.0f, -0.5f); // 12th positon
    m_environs[i].landingFrictionScale = 1.0;
    m_environs[i].landingHardnessScale = 1.0;
    m_environs[i].par = 3;
    m_environs[i].scale = 0.02;
    m_environs[i].teeDirection = 0.0f;
    //m_environs[i].teePosition = DirectX::SimpleMath::Vector3(2.0f, 0.0f, 0.0f);
    m_environs[i].teePosition = DirectX::SimpleMath::Vector3(0.2f, 0.0f, 0.1f); // 155 yards ish pos
    m_environs[i].terrainColor = DirectX::Colors::Green;
    m_environs[i].wind = DirectX::SimpleMath::Vector3(-0.4f, 0.0f, -0.9f);

    ++i;
    m_environs[i].name = "Breezy";    
    m_environs[i].airDensity = 1.225;
    m_environs[i].gravity = -9.8;
    m_environs[i].holePosition = DirectX::SimpleMath::Vector3(2.1f, 0.0f, 1.0f);
    m_environs[i].landingFrictionScale = 1.0;
    m_environs[i].landingHardnessScale = 1.0;
    m_environs[i].par = 5;
    m_environs[i].scale = 0.02;
    m_environs[i].teeDirection = 0.0f;
    m_environs[i].teePosition = DirectX::SimpleMath::Vector3(-2.0f, 0.0f, 0.0f);
    m_environs[i].terrainColor = DirectX::Colors::Green;
    m_environs[i].wind = DirectX::SimpleMath::Vector3(-10.0f, 0.0f, 0.0f);

    m_environs[i].name = "Calm";
    m_environs[i].airDensity = 1.225;
    m_environs[i].gravity = -9.8;
    //m_environs[i].holePosition = DirectX::SimpleMath::Vector3(4.9f, 0.0f, 0.82f);
    m_environs[i].holePosition = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 1.5f);
    m_environs[i].landingFrictionScale = 1.0;
    m_environs[i].landingHardnessScale = 1.0;
    m_environs[i].par = 3;
    m_environs[i].scale = 0.02;
    m_environs[i].teeDirection = 0.0f;
    m_environs[i].teePosition = DirectX::SimpleMath::Vector3(-2.0f, 0.0f, 0.0f);
    m_environs[i].terrainColor = DirectX::Colors::Green;
    m_environs[i].wind = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

    /*
    ++i;
    m_environs[i].name = "Non Terrestrial (Alien Golf!!)";    
    m_environs[i].airDensity = 11.2;
    m_environs[i].gravity = -5.8;
    m_environs[i].holePosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
    m_environs[i].landingFrictionScale = 1.0;
    m_environs[i].landingHardnessScale = 1.0;
    m_environs[i].par = 5;
    m_environs[i].scale = 0.02;
    m_environs[i].teeDirection = 45.0f;
    m_environs[i].teePosition = DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
    m_environs[i].terrainColor = DirectX::Colors::Blue;
    m_environs[i].wind = DirectX::SimpleMath::Vector3(3.0f, 0.0f, 0.69f);
    */
}

void Environment::LoadFixtureBucket()
{
    m_fixtureBucket.clear();
    

    // add FlagStick   
    Fixture flagStick;
    flagStick.idNumber = 0;
    flagStick.position = m_currentEnviron.holePosition;
    flagStick.fixtureType = FixtureType::FIXTURETYPE_FLAGSTICK;
    flagStick.animationVariation = 0.0;
    flagStick.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(flagStick.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(flagStick);
    
    // add Tee Box;
    Fixture teeBox;
    teeBox.idNumber = 1;
    teeBox.position = m_currentEnviron.teePosition;
    teeBox.fixtureType = FixtureType::FIXTURETYPE_TEEBOX;
    teeBox.animationVariation = 0.0;
    teeBox.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(teeBox.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(teeBox);

    // randomized fixtures
    const float varMax = 10.0;
    
    const float posMin = -2.0;
    const float posMax = 4.0;
    const float xPosMin = 0.0;
    const float xPosMax = 7.0;
    const float yPos = m_landingHeight;
    const float zPosMin = -2.0;
    const float zPosMax = -1.6;

    const int fixtureTypeNumMin = 1;
    const int fixtureTypeNumMax = 6;
    const int fixtureCount = 55;
    int leftOrRightFairwayPlacement = 1;
    for (int i = 2; i < fixtureCount; ++i)  // start at 2 due to 0 being flag/hole and 1 being the tee box
    {
        float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (xPosMax))) - 1.0;
        float y = yPos;
        float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (zPosMax - zPosMin)) - 2.0;
        z *= leftOrRightFairwayPlacement; // to alternate tree placement or right or left side of fairway
        leftOrRightFairwayPlacement *= -1;
        float aVar = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/ varMax);

        Fixture fixt;

        fixt.idNumber = i;
        fixt.position = DirectX::SimpleMath::Vector3(x, y, z);

        int fixtureNum = fixtureTypeNumMin + rand() / (RAND_MAX / (fixtureTypeNumMax - fixtureTypeNumMin));
        if (fixtureNum == 1)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
        }
        else if (fixtureNum == 2)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
        }
        else if (fixtureNum == 3)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE06;
        }
        else if (fixtureNum == 4)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE07;
        }
        else if (fixtureNum == 5)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
        }
        else
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
        }

        fixt.animationVariation = aVar;
        fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
        
        m_fixtureBucket.push_back(fixt);
    }

    // FIXTURETYPE_TREE03  V yellow green
    // FIXTURETYPE_TREE04  Circle Orange
    // FIXTURETYPE_TREE06  Triangle dark sides
    // FIXTURETYPE_TREE07  A tree
    // FIXTURETYPE_TREE09  A outline

    int i = 0;
    i = fixtureCount;
    Fixture fixt;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
    fixt.position = DirectX::SimpleMath::Vector3(5.9, 0.0, 0.1);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
    fixt.position = DirectX::SimpleMath::Vector3(5.5, 0.0, -1.4);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE06;
    fixt.position = DirectX::SimpleMath::Vector3(5.65, 0.0, -1.14);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
    fixt.position = DirectX::SimpleMath::Vector3(5.85, 0.0, -0.99);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
    fixt.position = DirectX::SimpleMath::Vector3(5.89, 0.0, -0.81);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
    fixt.position = DirectX::SimpleMath::Vector3(5.91, 0.0, -0.72);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
    fixt.position = DirectX::SimpleMath::Vector3(5.85, 0.0, -0.39);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE06;
    fixt.position = DirectX::SimpleMath::Vector3(5.76, 0.0, -0.17);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ///////////////////// start positive z locs

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
    fixt.position = DirectX::SimpleMath::Vector3(5.92, 0.0, 0.27);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
    fixt.position = DirectX::SimpleMath::Vector3(5.96, 0.0, 0.47);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE06;
    fixt.position = DirectX::SimpleMath::Vector3(5.82, 0.0, 0.66);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
    fixt.position = DirectX::SimpleMath::Vector3(5.79, 0.0, 0.93);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
    fixt.position = DirectX::SimpleMath::Vector3(5.69, 0.0, 1.23);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE07;
    fixt.position = DirectX::SimpleMath::Vector3(5.539, 0.0, 1.45);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    // start trees to create dog leg
    ////////////////////////////////////////////

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE07;
    fixt.position = DirectX::SimpleMath::Vector3(3.0, 0.0, .75);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
    fixt.position = DirectX::SimpleMath::Vector3(2.7, 0.0, .97);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
    fixt.position = DirectX::SimpleMath::Vector3(2.4, 0.0, 1.17);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);
}

void Environment::LoadFixtureBucket12th()
{
    m_fixtureBucket.clear();

    // add FlagStick   
    Fixture flagStick;
    flagStick.idNumber = 0;
    flagStick.position = m_currentEnviron.holePosition;
    flagStick.fixtureType = FixtureType::FIXTURETYPE_FLAGSTICK;
    flagStick.animationVariation = 0.0;
    flagStick.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(flagStick.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(flagStick);

    // add Tee Box;
    Fixture teeBox;
    teeBox.idNumber = 1;
    teeBox.position = m_currentEnviron.teePosition;
    teeBox.fixtureType = FixtureType::FIXTURETYPE_TEEBOX;
    teeBox.animationVariation = 0.0;
    teeBox.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(teeBox.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(teeBox);

    // randomized fixtures
    const float varMax = 10.0;

    const float posMin = -2.0;
    const float posMax = 4.0;
    const float xPosMin = 0.0;
    const float xPosMax = 6.0;
    const float yPos = m_landingHeight;
    const float zPosMin = -2.0;
    const float zPosMax = -1.6;

    const int fixtureTypeNumMin = 1;
    const int fixtureTypeNumMax = 6;
    const int fixtureCount = 25;
    int leftOrRightFairwayPlacement = 1;
    for (int i = 2; i < fixtureCount; ++i)  // start at 2 due to 0 being flag/hole and 1 being the tee box
    {
        float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (xPosMax))) - 1.0;
        float y = yPos;
        float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (zPosMax - zPosMin)) - 2.0;
        z *= leftOrRightFairwayPlacement; // to alternate tree placement or right or left side of fairway
        leftOrRightFairwayPlacement *= -1;
        float aVar = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / varMax);

        Fixture fixt;

        fixt.idNumber = i;
        fixt.position = DirectX::SimpleMath::Vector3(x, y, z);

        int fixtureNum = fixtureTypeNumMin + rand() / (RAND_MAX / (fixtureTypeNumMax - fixtureTypeNumMin));
        if (fixtureNum == 1)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
        }
        else if (fixtureNum == 2)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE07;
        }
        else if (fixtureNum == 3)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE06;
        }
        else if (fixtureNum == 4)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE07;
        }
        else if (fixtureNum == 5)
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
        }
        else
        {
            fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
        }

        fixt.animationVariation = aVar;
        fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);

        m_fixtureBucket.push_back(fixt);
    }

    int i = 0;
    i = fixtureCount;
    Fixture fixt;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
    fixt.position = DirectX::SimpleMath::Vector3(4.9, 0.0, 0.1);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
    fixt.position = DirectX::SimpleMath::Vector3(4.5, 0.0, -1.4);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE06;
    fixt.position = DirectX::SimpleMath::Vector3(4.65, 0.0, -1.14);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
    fixt.position = DirectX::SimpleMath::Vector3(4.85, 0.0, -0.99);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
    fixt.position = DirectX::SimpleMath::Vector3(4.89, 0.0, -0.81);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
    fixt.position = DirectX::SimpleMath::Vector3(4.91, 0.0, -0.72);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
    fixt.position = DirectX::SimpleMath::Vector3(4.85, 0.0, -0.39);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
    fixt.position = DirectX::SimpleMath::Vector3(4.76, 0.0, -0.17);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ///////////////////// start positive z locs

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE04;
    fixt.position = DirectX::SimpleMath::Vector3(4.92, 0.0, -0.17);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE06;
    fixt.position = DirectX::SimpleMath::Vector3(4.96, 0.0, 0.47);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE07;
    fixt.position = DirectX::SimpleMath::Vector3(4.82, 0.0, 0.66);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE09;
    fixt.position = DirectX::SimpleMath::Vector3(4.79, 0.0, 0.93);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE03;
    fixt.position = DirectX::SimpleMath::Vector3(4.69, 0.0, 1.23);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 10.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_TREE07;
    fixt.position = DirectX::SimpleMath::Vector3(4.539, 0.0, 1.45);
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);
    
    DirectX::SimpleMath::Vector3 bridge1(1.225, 0.0, -.9);
    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = Utility::ToRadians(45.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_BRIDGE;
    fixt.position = bridge1;
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);

    DirectX::SimpleMath::Vector3 bridge2(3.6, 0.0, 1.5);
    ++i;
    fixt.idNumber = i;
    fixt.animationVariation = Utility::ToRadians(45.0);
    fixt.fixtureType = FixtureType::FIXTURETYPE_BRIDGE;
    fixt.position = bridge2;
    fixt.distanceToCamera = DirectX::SimpleMath::Vector3::Distance(fixt.position, m_currentEnviron.teePosition);
    m_fixtureBucket.push_back(fixt);
}

bool Environment::LoadHeightMap(EnvironmentType aEnviron)
{
    FILE* filePtr;
    char* filename;
    if (aEnviron == EnvironmentType::ENIVRONMENTTYPE_STARTUP)
    {
        filename = "../AutoGame1989/Art/HeightMaps/heightmapStartScreen.bmp";
    }
    if (aEnviron == EnvironmentType::ENVIRONMENTTYPE_CLEANTEST)
    {
        filename = "../AutoGame1989/Art/HeightMaps/heightmapCleanTest2.bmp";
    }
    else
    {
        filename = "../AutoGame1989/Art/HeightMaps/heightmapCleanTest.bmp";
    }

    // Open the height map file 
    int error = fopen_s(&filePtr, filename, "rb");
    if (error != 0)
    {
        return false;
    }

    // Read in the file header
    BITMAPFILEHEADER bitmapFileHeader;
    size_t count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
    if (count != 1)
    {
        return false;
    }

    // Read in the bitmap info header.
    BITMAPINFOHEADER bitmapInfoHeader;
    count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    if (count != 1)
    {
        return false;
    }

    // Save the dimensions of the terrain.
    m_terrainWidth = bitmapInfoHeader.biWidth;
    m_terrainHeight = bitmapInfoHeader.biHeight;

    // Calculate the size of the bitmap image data.
    int imageSize = m_terrainWidth * m_terrainHeight * 3 + m_terrainWidth;

    // Allocate memory for the bitmap image data.
    unsigned char* bitmapImage = new unsigned char[imageSize];
    if (!bitmapImage)
    {
        return false;
    }

    // Move to the beginning of the bitmap data.
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    UINT pitch = m_terrainWidth * 3;
    UINT excessPitch = 0;
    while (double(pitch / 4) != double(pitch) / 4.0)
    {
        pitch++;
        excessPitch++;
    }

    // Read in the bitmap image data.
    count = fread(bitmapImage, 1, pitch * m_terrainHeight, filePtr);
    if (count != imageSize)
    {
        return false;
    }

    std::vector<char> testMap;
    testMap.clear();
    for (int i = 0; i < imageSize; ++i)
    {
        testMap.push_back(bitmapImage[i]);
    }

    // Close the file.
    error = fclose(filePtr);
    if (error != 0)
    {
        return false;
    }

    // Create the structure to hold the height map data.
    m_heightMap.clear();
    m_heightMap.resize(m_terrainWidth * m_terrainHeight);

    // Initialize the position in the image data buffer.
    int k = 0;
    int index;
    unsigned char height;

    // Read the image data into the height map.
    for (int j = 0; j < m_terrainWidth; j++)
    {
        for (int i = 0; i < m_terrainHeight; i++)
        {
            height = bitmapImage[k];

            // To read values in backwards since bitmap read in is fliped
            index = (m_terrainWidth * (m_terrainHeight - 1 - j)) + i;

            m_heightMap[index].position.x = (float)j;
            m_heightMap[index].position.y = (float)height * m_heightScale; // scale height during input
            m_heightMap[index].position.z = (float)i;

            k += 3;
        }
        k += excessPitch;
    }

    // Release the bitmap image data.
    delete[] bitmapImage;
    bitmapImage = 0;

    return true;
}

void Environment::ScaleTerrain(EnvironmentType aEnviron)
{
    double scale = 1.0;
    double xTransform = 0.0;
    double yTransform = 0.0;
    double zTransform = 0.0;
    if (aEnviron == EnvironmentType::ENIVRONMENTTYPE_STARTUP)
    {
        scale = m_mapScaleStartScreen;
        xTransform = m_mapXtransformStartScreen;
        yTransform = m_mapYtransformStartScreen;
        zTransform = m_mapZtransformStartScreen;
    }
    else if (aEnviron == EnvironmentType::ENVIRONMENTTYPE_CLEANTEST)
    {
        scale = m_mapScaleGamePlay;
        xTransform = m_mapXtransformGamePlay;
        yTransform = m_mapYtransformGamePlay;
        zTransform = m_mapZtransformGamePlay;
    }

    for (int i = 0; i < m_heightMap.size(); ++i)
    {
        m_heightMap[i].position.x *= scale;
        m_heightMap[i].position.y *= scale;
        m_heightMap[i].position.z *= scale;

        m_heightMap[i].position.x += xTransform;
        m_heightMap[i].position.y += yTransform;
        m_heightMap[i].position.z += zTransform;
    }

    for (int i = 0; i < m_terrainModel.size(); ++i)
    {
        m_terrainModel[i].position.x *= scale;
        m_terrainModel[i].position.y *= scale;
        m_terrainModel[i].position.z *= scale;

        m_terrainModel[i].position.x += xTransform;
        m_terrainModel[i].position.y += yTransform;
        m_terrainModel[i].position.z += zTransform;
    }
}

void Environment::SetLandingHeight(double aLandingHeight)
{
    m_landingHeight = aLandingHeight;
}

void Environment::SetLauchHeight(double aLaunchHeight)
{
    m_launchHeight = aLaunchHeight;
}

void Environment::SortFixtureBucketByDistance()
{
    std::sort(m_fixtureBucket.begin(), m_fixtureBucket.end(),
        [](const auto& i, const auto& j) {return i.distanceToCamera > j.distanceToCamera; });
}

void Environment::UpdateEnvironment(const int aIndex)
{
    m_currentEnviron = m_environs[aIndex];
    m_landingFriction = m_environs[aIndex].landingFrictionScale;
    m_landingHardness = m_environs[aIndex].landingHardnessScale;
    BuildFlagVertex(m_environs[aIndex].holePosition);
    BuildHoleVertex(m_environs[aIndex].holePosition);
}

void Environment::UpdateFixtures(const DirectX::SimpleMath::Vector3 &aPos)
{
    UpdateFixtureDistanceToCamera(aPos);
    SortFixtureBucketByDistance();
}

void Environment::UpdateFixtureDistanceToCamera(const DirectX::SimpleMath::Vector3 &aCameraPos)
{
    for (int i = 0; i < m_fixtureBucket.size(); ++i)
    {
        m_fixtureBucket[i].distanceToCamera = DirectX::SimpleMath::Vector3::Distance(m_fixtureBucket[i].position, aCameraPos);
    }
}