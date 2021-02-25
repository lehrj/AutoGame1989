//
// Game.cpp
//
#include <stdlib.h>  // for random numbers
#include <time.h>    // timer for random numbers

#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
    srand(time(NULL));
    m_auto = new Auto();
    m_environment = new Environment();

    m_camera = new Camera(m_outputWidth, m_outputHeight);
    m_camera->InintializePreSwingCamera(DirectX::SimpleMath::Vector3::Zero, 0.0);

    m_vehicle = new Vehicle();

    if (m_isInDebugMode == false)
    {
        m_currentGameState = GameState::GAMESTATE_INTROSCREEN;
    }
    else
    {
        m_currentGameState = GameState::GAMESTATE_STARTSCREEN;
    }
    m_currentGameState = GameState::GAMESTATE_GAMEPLAY;
    m_currentLightingState = LightingState::LIGHTINGSTATE_STARTUP;
    m_currentUiState = UiState::UISTATE_SWING;
    InitializeWorldGrid();
}

Game::~Game()
{
    if (m_audioEngine)
    {
        m_audioEngine->Suspend();
    }
    m_audioMusicStream.reset();
    m_audioEffectStream.reset();

    delete m_auto;
    delete m_camera;
    delete m_environment;
    delete m_vehicle;

    delete[] m_terrainVertexArray;
    m_terrainVertexArray = 0;
    delete[] m_terrainVertexArrayBase;
    m_terrainVertexArrayBase = 0;

    delete[] m_terrainVertexArray2;
    m_terrainVertexArray2 = 0;
    delete[] m_terrainVertexArrayBase2;
    m_terrainVertexArrayBase2 = 0;

}

void Game::AudioPlayMusic(XACT_WAVEBANK_AUDIOBANK aSFX)
{
    m_audioMusicStream = m_audioBank->CreateStreamInstance(aSFX);

    if (m_audioMusicStream)
    {
        m_audioMusicStream->SetVolume(m_musicVolume);
        m_audioMusicStream->Play(true);
    }
}

void Game::AudioPlaySFX(XACT_WAVEBANK_AUDIOBANK aSFX)
{
    m_audioEffectStream = m_audioBank->CreateStreamInstance(aSFX);
    if (m_audioEffectStream)
    {
        m_audioEffectStream->SetVolume(m_sfxVolume);
        m_audioEffectStream->Play();
    }
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::Black);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels[] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
    ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide[] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"Texture3.jpg", nullptr, m_texture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/TestNorm8.png", nullptr, m_normalMap.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/specularJI.png", nullptr, m_specular.ReleaseAndGetAddressOf()));
    // test textures
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/TestOP.png", nullptr, m_textureTest.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/TestOP.png", nullptr, m_normalMapTest.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/TestOP.png", nullptr, m_specularTest.ReleaseAndGetAddressOf()));
    // Jackson Industry textures
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/TestJI/TextureJI.png", nullptr, m_textureJI.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/TestJI/NormJI.png", nullptr, m_normalMapJI.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/TestJI/SpecJi.png", nullptr, m_specularJI.ReleaseAndGetAddressOf()));

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/TestJI/tex.png", nullptr, m_textureJI.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/TestJI/norm2.png", nullptr, m_normalMapJI.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/TestJI/spec.png", nullptr, m_specularJI.ReleaseAndGetAddressOf()));

    // BMW textures
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/TestLogo.png", nullptr, m_textureBMW.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/TestNorm.png", nullptr, m_normalMapBMW.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/TestNorm2.png", nullptr, m_normalMapBMW2.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/TestSpec.png", nullptr, m_specularBMW.ReleaseAndGetAddressOf()));
    // Start screen textures
    /*
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/logoAutoGame.png", nullptr, m_textureAutoGame.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/NormalMapAutoGame2.dds", nullptr, m_normalMapAutoGame.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/specularAutoGame.png", nullptr, m_specularAutoGame.ReleaseAndGetAddressOf()));
    */

    /*
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/MoonTex2.png", nullptr, m_textureAutoGame.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/MoonNorm.png", nullptr, m_normalMapAutoGame.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/MoonSpec.png", nullptr, m_specularAutoGame.ReleaseAndGetAddressOf()));
    */

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/textureUV.png", nullptr, m_textureAutoGame.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/normalUV.png", nullptr, m_normalMapAutoGame.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/Test/specularUV.png", nullptr, m_specularAutoGame.ReleaseAndGetAddressOf()));


    // Textures for teaser trailer
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/logoTeaser.png", nullptr, m_textureTeaser.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateDDSTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/NormalMapTeaser.dds", nullptr, m_normalMapTeaser.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/specularTeaser.png", nullptr, m_specularTeaser.ReleaseAndGetAddressOf()));

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/road.png", nullptr, m_backgroundTex.ReleaseAndGetAddressOf()));
    m_road = std::make_unique<ScrollingBackground>();
    m_road->Load(m_backgroundTex.Get());

   

    // TODO: Initialize device dependent objects here (independent of window size).
    m_world = DirectX::SimpleMath::Matrix::Identity;
    m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
    m_effect = std::make_unique<NormalMapEffect>(m_d3dDevice.Get());

    // Make sure you called CreateDDSTextureFromFile and CreateWICTextureFromFile before this point!
    m_effect->SetTexture(m_texture.Get());
    m_effect->SetNormalTexture(m_normalMap.Get());
    m_effect->SetLightDiffuseColor(0, Colors::White);
    m_effect->SetAlpha(1.0);

    m_effect->SetFogEnabled(false);
    m_effect->SetFogColor(DirectX::Colors::Black);
    m_effect->SetFogStart(1.0);
    m_effect->SetFogEnd(4.0);

    m_effect2 = std::make_unique<BasicEffect>(m_d3dDevice.Get());
    m_effect2->SetVertexColorEnabled(true);
    m_effect2->EnableDefaultLighting();
    m_effect2->SetLightDiffuseColor(0, Colors::Gray);

    m_effect3 = std::make_unique<BasicEffect>(m_d3dDevice.Get());
    m_effect3->SetVertexColorEnabled(true);

    void const* shaderByteCode2;
    size_t byteCodeLength2;
    m_effect2->GetVertexShaderBytecode(&shaderByteCode2, &byteCodeLength2);
    DX::ThrowIfFailed(m_d3dDevice->CreateInputLayout(VertexType2::InputElements, VertexType2::InputElementCount, shaderByteCode2, byteCodeLength2, m_inputLayout.ReleaseAndGetAddressOf()));
    m_batch2 = std::make_unique<PrimitiveBatch<VertexType2>>(m_d3dContext.Get());

    void const* shaderByteCode;
    size_t byteCodeLength;
    m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
    DX::ThrowIfFailed(m_d3dDevice->CreateInputLayout(VertexType::InputElements, VertexType::InputElementCount, shaderByteCode, byteCodeLength, m_inputLayout.ReleaseAndGetAddressOf()));
    m_batch = std::make_unique<PrimitiveBatch<VertexType>>(m_d3dContext.Get());

    void const* shaderByteCode3;
    size_t byteCodeLength3;
    m_effect3->GetVertexShaderBytecode(&shaderByteCode3, &byteCodeLength3);
    DX::ThrowIfFailed(m_d3dDevice->CreateInputLayout(VertexType3::InputElements, VertexType3::InputElementCount, shaderByteCode3, byteCodeLength3, m_inputLayout.ReleaseAndGetAddressOf()));
    m_batch3 = std::make_unique<PrimitiveBatch<VertexType3>>(m_d3dContext.Get());

    //CreateSphere(std::vector<VertexType>&vertices, std::vector<uint16_t>&indices, float diameter = 1, size_t tessellation = 16, bool rhcoords = true, bool invertn = false);
    m_shape = GeometricPrimitive::CreateSphere(m_d3dContext.Get());

    CD3D11_RASTERIZER_DESC rastDesc(D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
        D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
        D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, FALSE, TRUE);

    /* // For multisampling rendering
    CD3D11_RASTERIZER_DESC rastDesc(D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
        D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
        D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, TRUE, TRUE); // Multisampling
    */
    DX::ThrowIfFailed(m_d3dDevice->CreateRasterizerState(&rastDesc, m_raster.ReleaseAndGetAddressOf()));

    m_font = std::make_unique<SpriteFont>(m_d3dDevice.Get(), L"myfile.spritefont");
    m_titleFont = std::make_unique<SpriteFont>(m_d3dDevice.Get(), L"titleFont.spritefont");
    m_bitwiseFont = std::make_unique<SpriteFont>(m_d3dDevice.Get(), L"bitwise24.spritefont");
    m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dContext.Get());

    // Start Texture
    ComPtr<ID3D11Resource> resource;

    // Character Select Textures
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"../AutoGame1989/Art/PacSprite.png", nullptr, m_pacTexture.ReleaseAndGetAddressOf()));
    //DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"Chacter2SpriteSheet.png", nullptr, m_pacTexture.ReleaseAndGetAddressOf()));
    m_pacSprite = std::make_unique<AnimatedTexture>();
    m_pacSprite->Load(m_pacTexture.Get(), 4, 6);

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"ChacterSpriteSheet.png", nullptr, m_characterTexture.ReleaseAndGetAddressOf()));
    m_character = std::make_unique<AnimatedTexture>();
    m_character->Load(m_characterTexture.Get(), 4, 6);

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"Chacter0SpriteSheet.png", nullptr, m_character0Texture.ReleaseAndGetAddressOf()));
    m_character0 = std::make_unique<AnimatedTexture>();
    m_character0->Load(m_character0Texture.Get(), 4, 6);

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"Chacter1SpriteSheet.png", nullptr, m_character1Texture.ReleaseAndGetAddressOf()));
    m_character1 = std::make_unique<AnimatedTexture>();
    m_character1->Load(m_character1Texture.Get(), 4, 6);

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"Chacter2SpriteSheet.png", nullptr, m_character2Texture.ReleaseAndGetAddressOf()));
    m_character2 = std::make_unique<AnimatedTexture>();
    m_character2->Load(m_character2Texture.Get(), 4, 6);

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"CharacterBackground.png", resource.GetAddressOf(), m_characterBackgroundTexture.ReleaseAndGetAddressOf()));
    ComPtr<ID3D11Texture2D> characterBackground;
    DX::ThrowIfFailed(resource.As(&characterBackground));
    CD3D11_TEXTURE2D_DESC characterBackgroundDesc;
    characterBackground->GetDesc(&characterBackgroundDesc);
    m_characterBackgroundOrigin.x = float(characterBackgroundDesc.Width / 2);
    m_characterBackgroundOrigin.y = float(characterBackgroundDesc.Height / 2);

    // Environment Select textures
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"EnvironmentSelectCalm.png", nullptr, m_environSelectCalmTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"EnvironmentSelectCalm.png", resource.GetAddressOf(), m_environSelectCalmTexture.ReleaseAndGetAddressOf()));
    ComPtr<ID3D11Texture2D> pEnvironmentSelectCalm;
    DX::ThrowIfFailed(resource.As(&pEnvironmentSelectCalm));
    CD3D11_TEXTURE2D_DESC EnvironmentSelectCalmDesc;
    pEnvironmentSelectCalm->GetDesc(&EnvironmentSelectCalmDesc);
    m_environSelectCalmOrigin.x = float(EnvironmentSelectCalmDesc.Width / 2);
    m_environSelectCalmOrigin.y = float(EnvironmentSelectCalmDesc.Height / 2);

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"EnvironmentSelectBreezy.png", nullptr, m_environSelectBreezyTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"EnvironmentSelectBreezy.png", resource.GetAddressOf(), m_environSelectBreezyTexture.ReleaseAndGetAddressOf()));
    ComPtr<ID3D11Texture2D> pEnvironmentSelectBreezy;
    DX::ThrowIfFailed(resource.As(&pEnvironmentSelectBreezy));
    CD3D11_TEXTURE2D_DESC EnvironmentSelectBreezyDesc;
    pEnvironmentSelectBreezy->GetDesc(&EnvironmentSelectBreezyDesc);
    m_environSelectBreezyOrigin.x = float(EnvironmentSelectBreezyDesc.Width / 2);
    m_environSelectBreezyOrigin.y = float(EnvironmentSelectBreezyDesc.Height / 2);

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"EnvironmentSelectAlien.jpg", nullptr, m_environSelectAlienTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"EnvironmentSelectAlien.jpg", resource.GetAddressOf(), m_environSelectAlienTexture.ReleaseAndGetAddressOf()));
    ComPtr<ID3D11Texture2D> pEnvironmentSelectAlien;
    DX::ThrowIfFailed(resource.As(&pEnvironmentSelectAlien));
    CD3D11_TEXTURE2D_DESC EnvironmentSelectAlienDesc;
    pEnvironmentSelectAlien->GetDesc(&EnvironmentSelectAlienDesc);
    m_environSelectAlienOrigin.x = float(EnvironmentSelectAlienDesc.Width / 2);
    m_environSelectAlienOrigin.y = float(EnvironmentSelectAlienDesc.Height / 2);

    // Intro screen textures
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"logoBMW.png", nullptr, m_bmwLogoTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"logoBMW.png", resource.GetAddressOf(), m_bmwLogoTexture.ReleaseAndGetAddressOf()));
    ComPtr<ID3D11Texture2D> logoBMW;
    DX::ThrowIfFailed(resource.As(&logoBMW));
    CD3D11_TEXTURE2D_DESC logoBMWDesc;
    logoBMW->GetDesc(&logoBMWDesc);
    m_bmwLogoOrigin.x = float(logoBMWDesc.Width / 2);
    m_bmwLogoOrigin.y = float(logoBMWDesc.Height / 2);

    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"logoJI.png", nullptr, m_jiLogoTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"logoJI.png", resource.GetAddressOf(), m_jiLogoTexture.ReleaseAndGetAddressOf()));
    ComPtr<ID3D11Texture2D> logoJI;
    DX::ThrowIfFailed(resource.As(&logoJI));
    CD3D11_TEXTURE2D_DESC logoJIDesc;
    logoJI->GetDesc(&logoJIDesc);
    m_jiLogoOrigin.x = float(logoJIDesc.Width / 2);
    m_jiLogoOrigin.y = float(logoJIDesc.Height / 2);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews[] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    const UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    constexpr UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        //swapChainDesc.SampleDesc.Count = 8; // multisampling  RenderTesting
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            m_d3dDevice.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            m_swapChain.ReleaseAndGetAddressOf()
        ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
    //CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT, 0, 8, 0);  // For multisampling rendering

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    //CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS);  //for multisampling RenderTesting

    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.

    ////********* WLJ world start ----- deactivate to turn off world spin
    m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(2.f, 2.f, 2.f), DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

    const float viewPlaneNear = 0.1f;
    const float viewPlaneFar = 900.0f;
    m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(backBufferWidth) / float(backBufferHeight), viewPlaneNear, viewPlaneFar);

    m_effect->SetView(m_view);
    m_effect->SetProjection(m_proj);
    m_effect2->SetView(m_view);
    m_effect2->SetProjection(m_proj);
    m_effect3->SetView(m_view);
    m_effect3->SetProjection(m_proj);
    // world end

    // UI font positions
    m_fontPos.x = backBufferWidth / 2.f;
    m_fontPos.y = backBufferHeight / 2.f;
    m_fontPos2.x = backBufferWidth - 5.f;
    m_fontPos2.y = 35;
    m_fontPosDebug.x = 480;
    m_fontPosDebug.y = 35;
    m_fontMenuPos.x = backBufferWidth / 2.f;
    m_fontMenuPos.y = 35;
    m_bitwiseFontPos.x = backBufferWidth / 2.f;
    m_bitwiseFontPos.y = backBufferHeight / 2.f;

    m_road->SetWindow(backBufferWidth, backBufferHeight);

    // Character textures  

    m_pacPos.x = backBufferWidth / 2.f;
    m_pacPos.y = backBufferHeight / 2.f - 55.f;

    m_characterPos.x = backBufferWidth / 2.f;
    m_characterPos.y = backBufferHeight / 2.f;
    m_character0Pos.x = backBufferWidth / 2.f;
    m_character0Pos.y = backBufferHeight / 2.f;
    m_character1Pos.x = backBufferWidth / 2.f;
    m_character1Pos.y = backBufferHeight / 2.f;
    m_character2Pos.x = backBufferWidth / 2.f;
    m_character2Pos.y = backBufferHeight / 2.f;
    DX::ThrowIfFailed(CreateWICTextureFromFile(m_d3dDevice.Get(), L"CharacterBackground.png", nullptr, m_characterBackgroundTexture.ReleaseAndGetAddressOf()));
    m_characterBackgroundPos.x = backBufferWidth / 2.f;
    m_characterBackgroundPos.y = backBufferHeight / 2.f;

    // Environment select textures
    m_environSelectCalmPos.x = backBufferWidth / 2.f;
    m_environSelectCalmPos.y = backBufferHeight / 2.f;

    m_environSelectBreezyPos.x = backBufferWidth / 2.f;
    m_environSelectBreezyPos.y = backBufferHeight / 2.f;

    m_environSelectAlienPos.x = backBufferWidth / 2.f;
    m_environSelectAlienPos.y = backBufferHeight / 2.f;

    // Intro Sceen textures
    m_bmwLogoPos.x = backBufferWidth / 2.f;
    m_bmwLogoPos.y = backBufferHeight / 2.f;

    m_jiLogoPos.x = backBufferWidth / 2.f;
    m_jiLogoPos.y = backBufferHeight / 2.f;
    // End Texture
}

void Game::DrawCameraFocus()
{
    const float line = .25f;
    DirectX::XMVECTORF32 lineColor = DirectX::Colors::Yellow;
    DirectX::SimpleMath::Vector3 focalPoint = m_camera->GetTargetPos();
    m_cameraFocusPos = focalPoint;
    if (m_moveLightWithCameraFocus3 == true)
    {
        lineColor = DirectX::Colors::White;
        DirectX::SimpleMath::Vector3 posNormalized = m_cameraFocusPos;
        posNormalized.Normalize();
        m_lightPos2 = posNormalized;
    }
    if (m_moveLightWithCameraFocus2 == true)
    {
        lineColor = DirectX::Colors::White;
        DirectX::SimpleMath::Vector3 posNormalized = m_cameraFocusPos;
        posNormalized.Normalize();
        m_lightPos1 = posNormalized;
    }
    if (m_moveLightWithCameraFocus1 == true)
    {
        lineColor = DirectX::Colors::White;
        DirectX::SimpleMath::Vector3 posNormalized = m_cameraFocusPos;
        posNormalized.Normalize();
        m_lightPos0 = posNormalized;
    }
    DirectX::SimpleMath::Vector3 yLine = focalPoint;
    yLine.y += line;
    DirectX::SimpleMath::Vector3 xLine = focalPoint;
    xLine.x += line;
    DirectX::SimpleMath::Vector3 zLine = focalPoint;
    zLine.z += line;

    DirectX::SimpleMath::Vector3 negZLine = focalPoint;
    negZLine.z -= line;
    DirectX::SimpleMath::Vector3 negXLine = focalPoint;
    negXLine.x -= line;

    DirectX::SimpleMath::Vector3 lineNormal = DirectX::SimpleMath::Vector3::UnitX;
    VertexPositionNormalColor origin(focalPoint, lineNormal, lineColor);
    VertexPositionNormalColor yOffset(yLine, lineNormal, lineColor);
    VertexPositionNormalColor xOffset(xLine, lineNormal, lineColor);
    VertexPositionNormalColor zOffset(zLine, lineNormal, lineColor);
    m_batch2->DrawLine(origin, yOffset);
    m_batch2->DrawLine(origin, xOffset);
    m_batch2->DrawLine(origin, zOffset);


    /*
    const float line = .25f;
    DirectX::SimpleMath::Vector3 focalPoint = m_camera->GetTargetPos();
    //pGolf->SetPosToTerrain(focalPoint);
    float height = pGolf->GetTerrainHeight(focalPoint);
    //height += .1;
    focalPoint.y = height;
    DirectX::SimpleMath::Vector3 yLine = focalPoint;
    yLine.y += line;
    DirectX::SimpleMath::Vector3 xLine = focalPoint;
    xLine.x += line;
    DirectX::SimpleMath::Vector3 zLine = focalPoint;
    zLine.z += line;
    DirectX::SimpleMath::Vector3 negZLine = focalPoint;
    negZLine.z -= line;
    DirectX::SimpleMath::Vector3 negXLine = focalPoint;
    negXLine.x -= line;
    VertexPositionColor origin(focalPoint, Colors::Yellow);
    VertexPositionColor yOffset(yLine, Colors::Yellow);
    VertexPositionColor xOffset(xLine, Colors::Yellow);
    VertexPositionColor zOffset(zLine, Colors::Yellow);
    VertexPositionColor negXOffset(negXLine, Colors::Yellow);
    VertexPositionColor negZOffset(negZLine, Colors::Yellow);
    m_debugHeight = focalPoint.y;
    m_debugXpoint = focalPoint.x;
    m_debugZpoint = focalPoint.z;
    m_batch->DrawLine(origin, yOffset);
    m_batch->DrawLine(origin, xOffset);
    m_batch->DrawLine(origin, zOffset);
    m_batch->DrawLine(origin, negZOffset);
    m_batch->DrawLine(origin, negXOffset);
    */
}

void Game::DrawCar()
{
    DirectX::SimpleMath::Vector3 origin = DirectX::SimpleMath::Vector3::Zero;
    //origin += m_carPos;
    //origin.y += .1;

    DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::UnitX;

    DirectX::SimpleMath::Vector3 leftFrontBumper = origin;
    leftFrontBumper.z -= 1.0;

    DirectX::SimpleMath::Vector3 backLeft = origin;
    backLeft.x -= 1.0f;
    backLeft.z += -0.2f;
    DirectX::SimpleMath::Vector3 backRight = origin;
    backRight.x -= 1.0f;
    backRight.z += 0.2f;

    float aim = m_carAim + Utility::ToRadians(90.0);
    origin = DirectX::SimpleMath::Vector3::Transform(origin, DirectX::SimpleMath::Matrix::CreateRotationY(aim));
    backLeft = DirectX::SimpleMath::Vector3::Transform(backLeft, DirectX::SimpleMath::Matrix::CreateRotationY(aim));
    backRight = DirectX::SimpleMath::Vector3::Transform(backRight, DirectX::SimpleMath::Matrix::CreateRotationY(aim));


    DirectX::SimpleMath::Vector3 tailTop = origin;
    tailTop.x -= 1.0f;
    tailTop.y += 1.0f;
    tailTop = DirectX::SimpleMath::Vector3::Transform(tailTop, DirectX::SimpleMath::Matrix::CreateRotationY(aim));
    DirectX::SimpleMath::Vector3 tailBase = tailTop;
    tailBase.y -= 1.0f;
    tailBase.y = origin.y;

    origin += m_carPos;
    backLeft += m_carPos;
    backRight += m_carPos;
    tailTop += m_carPos;
    tailBase += m_carPos;

    DirectX::SimpleMath::Vector3 frontTop = origin;
    frontTop.y += 1.0f;

    DirectX::SimpleMath::Vector3 lineColor = DirectX::Colors::Red;
    /*
    DirectX::VertexPositionColor frontVert(origin, lineColor);
    DirectX::VertexPositionColor backLeftVert(backLeft, lineColor);
    DirectX::VertexPositionColor backRightVert(backRight, lineColor);
    */
    DirectX::VertexPositionNormalColor frontVert(origin, DirectX::SimpleMath::Vector3::UnitY, lineColor);
    DirectX::VertexPositionNormalColor backLeftVert(backLeft, DirectX::SimpleMath::Vector3::UnitY, lineColor);
    DirectX::VertexPositionNormalColor backRightVert(backRight, DirectX::SimpleMath::Vector3::UnitY, lineColor);
    DirectX::VertexPositionNormalColor tailTopVert(tailTop, DirectX::SimpleMath::Vector3::UnitX, lineColor);
    DirectX::VertexPositionNormalColor tailBaseVert(tailBase, DirectX::SimpleMath::Vector3::UnitX, lineColor);
    DirectX::VertexPositionNormalColor frontTailVert(origin, DirectX::SimpleMath::Vector3::UnitX, lineColor);


    VertexPositionNormalTexture v1(origin, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionNormalTexture v2(backLeft, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture v3(backRight, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(0, 1));

    VertexPositionNormalTexture v4(origin, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionNormalTexture v5(tailTop, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture v6(tailBase, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(0, 1));

    VertexPositionNormalTexture v7(origin, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionNormalTexture v8(backLeft, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture v9(tailTop, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(0, 1));

    VertexPositionNormalTexture v10(origin, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionNormalTexture v11(backRight, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture v12(tailTop, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(0, 1));


    /*
    VertexPositionTexture v1(origin, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionTexture v2(backLeft, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionTexture v3(backRight, DirectX::SimpleMath::Vector2(0, 1));
    VertexPositionColorTexture v4(origin, lineColor, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionColorTexture v5(backLeft, lineColor, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionColorTexture v6(backRight, lineColor, DirectX::SimpleMath::Vector2(0, 1));
    DirectX::VertexPositionColorTexture v7(tailTop, lineColor, DirectX::SimpleMath::Vector2(.5f, 0));
    DirectX::VertexPositionColorTexture v8(tailBase, lineColor, DirectX::SimpleMath::Vector2(1, 1));
    DirectX::VertexPositionColorTexture v9(origin, lineColor, DirectX::SimpleMath::Vector2(0, 1));
    */

    /*
    m_effect->SetTexture(m_texture.Get());
    m_batch->DrawTriangle(v1, v3, v2);
    m_effect->Apply(m_d3dContext.Get());
    m_batch->End();
    m_batch->Begin();
    m_batch->DrawTriangle(v4, v5, v6);
    //m_effect->SetTexture(m_jiLogoTexture.Get());
    m_batch->DrawTriangle(v7, v8, v9);
    m_effect->Apply(m_d3dContext.Get());
    m_batch->End();
    m_batch->Begin();

    //m_effect->SetTexture(m_bmwLogoTexture.Get());
    m_effect->Apply(m_d3dContext.Get());
    m_batch->DrawTriangle(v10, v11, v12);
    */


    /*
    VertexPositionNormalTexture topLeft(frontTop, DirectX::SimpleMath::Vector3::UnitZ, DirectX::SimpleMath::Vector2(0, 0));
    VertexPositionNormalTexture topRight(tailTop, DirectX::SimpleMath::Vector3::UnitZ, DirectX::SimpleMath::Vector2(1, 0));
    VertexPositionNormalTexture bottomRight(tailBase, DirectX::SimpleMath::Vector3::UnitZ, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture bottomLeft(origin, DirectX::SimpleMath::Vector3::UnitZ, DirectX::SimpleMath::Vector2(0, 1));
    */

    DirectX::SimpleMath::Vector3 testColor = DirectX::Colors::White;

    VertexPositionNormalColorTexture topLeft(frontTop, DirectX::SimpleMath::Vector3::UnitZ, testColor, DirectX::SimpleMath::Vector2(0, 0));
    VertexPositionNormalColorTexture topRight(tailTop, DirectX::SimpleMath::Vector3::UnitZ, testColor, DirectX::SimpleMath::Vector2(1, 0));
    VertexPositionNormalColorTexture bottomRight(tailBase, DirectX::SimpleMath::Vector3::UnitZ, testColor, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalColorTexture bottomLeft(origin, DirectX::SimpleMath::Vector3::UnitZ, testColor, DirectX::SimpleMath::Vector2(0, 1));

    //m_effect->SetTexture(m_texture.Get());
    m_batch->DrawQuad(topLeft, topRight, bottomRight, bottomLeft);
    //m_effect->SetTexture(m_texture.Get());
    //m_effect->Apply(m_d3dContext.Get());
    //m_batch->DrawTriangle(frontVert, backLeftVert, backRightVert);
    //m_batch->DrawTriangle(frontTailVert, tailBaseVert, tailTopVert);

    //m_batch->DrawLine(frontVert, backLeftVert);
    //m_batch->DrawLine(backLeftVert, backRightVert);
    //m_batch->DrawLine(backRightVert, frontVert);

}

void Game::DrawCar2()
{
    DirectX::SimpleMath::Vector3 origin = DirectX::SimpleMath::Vector3::Zero;
    //origin += m_carPos;
    //origin.y += .1;
    DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::UnitX;

    DirectX::SimpleMath::Vector3 leftFrontBumper = origin;
    leftFrontBumper.z -= 1.0;

    DirectX::SimpleMath::Vector3 backLeft = origin;
    backLeft.x -= 1.0f;
    backLeft.z += -0.2f;
    DirectX::SimpleMath::Vector3 backRight = origin;
    backRight.x -= 1.0f;
    backRight.z += 0.2f;

    origin = DirectX::SimpleMath::Vector3::Transform(origin, DirectX::SimpleMath::Matrix::CreateRotationY(m_carAim));
    backLeft = DirectX::SimpleMath::Vector3::Transform(backLeft, DirectX::SimpleMath::Matrix::CreateRotationY(m_carAim));
    backRight = DirectX::SimpleMath::Vector3::Transform(backRight, DirectX::SimpleMath::Matrix::CreateRotationY(m_carAim));


    DirectX::SimpleMath::Vector3 tailTop = origin;
    tailTop.x -= 1.0f;
    tailTop.y += 0.2f;
    tailTop = DirectX::SimpleMath::Vector3::Transform(tailTop, DirectX::SimpleMath::Matrix::CreateRotationY(m_carAim));
    DirectX::SimpleMath::Vector3 tailBase = tailTop;
    tailBase.y -= 0.2f;

    origin += m_carPos;
    backLeft += m_carPos;
    backRight += m_carPos;
    tailTop += m_carPos;
    tailBase += m_carPos;

    DirectX::SimpleMath::Vector3 lineColor = DirectX::Colors::Red;
    /*
    DirectX::VertexPositionColor frontVert(origin, lineColor);
    DirectX::VertexPositionColor backLeftVert(backLeft, lineColor);
    DirectX::VertexPositionColor backRightVert(backRight, lineColor);
    */
    DirectX::VertexPositionNormalColor frontVert(origin, DirectX::SimpleMath::Vector3::UnitY, lineColor);
    DirectX::VertexPositionNormalColor backLeftVert(backLeft, DirectX::SimpleMath::Vector3::UnitY, lineColor);
    DirectX::VertexPositionNormalColor backRightVert(backRight, DirectX::SimpleMath::Vector3::UnitY, lineColor);
    DirectX::VertexPositionNormalColor tailTopVert(tailTop, DirectX::SimpleMath::Vector3::UnitX, lineColor);
    DirectX::VertexPositionNormalColor tailBaseVert(tailBase, DirectX::SimpleMath::Vector3::UnitX, lineColor);
    DirectX::VertexPositionNormalColor frontTailVert(origin, DirectX::SimpleMath::Vector3::UnitX, lineColor);


    VertexPositionNormalTexture v1(origin, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionNormalTexture v2(backLeft, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture v3(backRight, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(0, 1));

    VertexPositionNormalTexture v4(origin, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionNormalTexture v5(tailTop, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture v6(tailBase, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(0, 1));

    VertexPositionNormalTexture v7(origin, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionNormalTexture v8(backLeft, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture v9(tailTop, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(0, 1));

    VertexPositionNormalTexture v10(origin, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionNormalTexture v11(backRight, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalTexture v12(tailTop, DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector2(0, 1));


    /*
    VertexPositionTexture v1(origin, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionTexture v2(backLeft, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionTexture v3(backRight, DirectX::SimpleMath::Vector2(0, 1));
    VertexPositionColorTexture v4(origin, lineColor, DirectX::SimpleMath::Vector2(.5f, 0));
    VertexPositionColorTexture v5(backLeft, lineColor, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionColorTexture v6(backRight, lineColor, DirectX::SimpleMath::Vector2(0, 1));
    DirectX::VertexPositionColorTexture v7(tailTop, lineColor, DirectX::SimpleMath::Vector2(.5f, 0));
    DirectX::VertexPositionColorTexture v8(tailBase, lineColor, DirectX::SimpleMath::Vector2(1, 1));
    DirectX::VertexPositionColorTexture v9(origin, lineColor, DirectX::SimpleMath::Vector2(0, 1));
    */


    /*
    m_effect->SetTexture(m_texture.Get());
    m_batch->DrawTriangle(v1, v3, v2);
    m_effect->Apply(m_d3dContext.Get());
    m_batch->End();
    m_batch->Begin();
    m_batch->DrawTriangle(v4, v5, v6);
    m_effect->SetTexture(m_jiLogoTexture.Get());
    m_batch->DrawTriangle(v7, v8, v9);
    m_effect->Apply(m_d3dContext.Get());
    m_batch->End();
    m_batch->Begin();
    m_effect->SetTexture(m_bmwLogoTexture.Get());
    m_effect->Apply(m_d3dContext.Get());
    m_batch->DrawTriangle(v10, v11, v12);
    */




    //m_effect->SetTexture(m_texture.Get());
    //m_effect->Apply(m_d3dContext.Get());
    //m_batch->DrawTriangle(frontVert, backLeftVert, backRightVert);
    //m_batch->DrawTriangle(frontTailVert, tailBaseVert, tailTopVert);

    //m_batch->DrawLine(frontVert, backLeftVert);
    //m_batch->DrawLine(backLeftVert, backRightVert);
    //m_batch->DrawLine(backRightVert, frontVert);

}

void Game::DrawDebugLines()
{
    /*
    std::vector<std::pair<DirectX::VertexPositionColor, DirectX::VertexPositionColor>> debugLines = pAuto->GetBallDebugLines();
    for (int i = 0; i < debugLines.size(); ++i)
    {
        m_batch->DrawLine(debugLines[i].first, debugLines[i].second);
    }
    */
}

void Game::DrawDebugValue()
{
    std::string textLine = "Value1 = " + std::to_string(m_debugValue1);
    float textLinePosX = m_bitwiseFontPos.x;
    float textLinePosY = m_bitwiseFontPos.y;
    //float textLinePosY = m_bitwiseFontPos.y + 100;
    DirectX::SimpleMath::Vector2 textLinePos(textLinePosX, textLinePosY);
    DirectX::SimpleMath::Vector2 textLineOrigin = m_bitwiseFont->MeasureString(textLine.c_str()) / 2.f;
    m_font->DrawString(m_spriteBatch.get(), textLine.c_str(), m_fontPosDebug, Colors::White, 0.f, textLineOrigin);

    textLine = "Value2 = " + std::to_string(m_debugValue2);
    textLinePosX = m_bitwiseFontPos.x;
    //textLinePosY += 300;
    //float textLinePosY = m_bitwiseFontPos.y + 100;
    textLinePos = m_fontPosDebug;
    textLinePos.y += 100.;
    textLineOrigin = DirectX::SimpleMath::Vector2(m_bitwiseFont->MeasureString(textLine.c_str()) / 2.f);
    m_font->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, Colors::White, 0.f, textLineOrigin);

    textLine = "Value3 = " + std::to_string(m_debugValue3);
    textLinePosX = m_bitwiseFontPos.x;
    //textLinePosY = m_bitwiseFontPos.y + 200;
    //float textLinePosY = m_bitwiseFontPos.y + 100;
    textLinePos.y += 100.;
    textLineOrigin = DirectX::SimpleMath::Vector2(m_bitwiseFont->MeasureString(textLine.c_str()) / 2.f);
    m_font->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, Colors::White, 0.f, textLineOrigin);
}

void Game::DrawGridForStartScreen()
{
    DirectX::XMVECTORF32 gridColor = DirectX::Colors::LawnGreen;
    DirectX::XMVECTORF32 baseColor = DirectX::Colors::Black;
    const float xBase = 1.0;
    const float yBase = - 0.0;
    const float zBase = -4.0;
    const float xLength = 4.0;
    const float zSpacing = 0.2;
    const int verticleLineCount = 80;
    const float timeStamp = static_cast<float>(m_timer.GetTotalSeconds());
    const float zLength = zSpacing * verticleLineCount;
    const float xSpacing = 0.2;
    const int horizontalLineCount = 25;
    const float baseOffset = -0.01;
    DirectX::SimpleMath::Vector3 verticleStart(xBase, yBase, zBase);
    for (int i = 0; i <= verticleLineCount; ++i)
    {
        DirectX::SimpleMath::Vector3 verticleEnd = verticleStart;
        verticleEnd.x -= xLength;
        DirectX::VertexPositionColor vertexStart(verticleStart, gridColor);
        DirectX::VertexPositionColor vertexEnd(verticleEnd, gridColor);
        m_batch3->DrawLine(vertexStart, vertexEnd);
        verticleStart.z += zSpacing;
    }

    DirectX::SimpleMath::Vector3 horizontalStart(xBase, yBase, zBase);
    //horizontalStart.y -= 2.0;
    float horizontalLinePos = fmod(timeStamp, xLength) * .2;
    horizontalLinePos += 3.0;
    //horizontalLinePos = 0;
    for (int i = 0; i < horizontalLineCount; ++i)
    {
        horizontalStart.x = - horizontalLinePos + (xSpacing * i);

        DirectX::SimpleMath::Vector3 horizontalEnd = horizontalStart;
        horizontalEnd.z += zLength;
        DirectX::VertexPositionColor vertexStart(horizontalStart, gridColor);
        DirectX::VertexPositionColor vertexEnd(horizontalEnd, gridColor);

        //m_batch3->DrawLine(vertexStart, vertexEnd);

        if (horizontalStart.x < xBase && horizontalStart.x > xBase - xLength)
        {
            m_batch3->DrawLine(vertexStart, vertexEnd);
        }
    }
   
    DirectX::SimpleMath::Vector3 nw(xBase, yBase + baseOffset, zBase);
    DirectX::SimpleMath::Vector3 ne = nw;
    ne.z += zLength;

    DirectX::SimpleMath::Vector3 sw = nw;
    sw.x -= xLength;
    DirectX::SimpleMath::Vector3 se = nw;
    se.z += zLength;
    se.x -= xLength;
    
    nw.y -= baseOffset;
    ne.y -= baseOffset;
    DirectX::VertexPositionColor v1(nw, baseColor);
    DirectX::VertexPositionColor v2(ne, baseColor);   
    DirectX::VertexPositionColor v3(se, baseColor);
    DirectX::VertexPositionColor v4(sw, baseColor);
    m_batch3->DrawQuad(v1, v2, v3, v4);

    nw.y += baseOffset;
    ne.y += baseOffset;

    // draw scrolling grid outline
    nw.y -= baseOffset;
    ne.y -= baseOffset;
    se.y -= baseOffset;
    sw.y -= baseOffset;

    v1 = DirectX::VertexPositionColor(nw, gridColor);
    v2 = DirectX::VertexPositionColor(ne, gridColor);
    v3 = DirectX::VertexPositionColor(se, gridColor);
    v4 = DirectX::VertexPositionColor(sw, gridColor);

    m_batch3->DrawLine(v1, v2);
    m_batch3->DrawLine(v2, v3);
    m_batch3->DrawLine(v3, v4);
    m_batch3->DrawLine(v4, v1);

    m_batch3->Draw(D3D_PRIMITIVE_TOPOLOGY_LINELIST, m_terrainVertexArray, m_terrainVertexCount);
}

void Game::DrawIntroScene()
{
    const float fadeDuration = 1.5f;
    const float logoDisplayDuration = 10.0f;
    const float logoDisplayGap = 1.0f;
    const float startDelay = 4.2f;

    const float fogGap1 =  0.5;
    const float fogGap2 = 1.1;


    const float timeStamp = static_cast<float>(m_timer.GetTotalSeconds());

    const float fadeInStart1 = startDelay;
    const float fadeInStart2 = startDelay + logoDisplayDuration + logoDisplayGap;
    const float fadeInStart3 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap;
    const float fadeInStart4 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap;

    const float fadeInEnd1 = startDelay + fadeDuration;
    const float fadeInEnd2 = startDelay + logoDisplayDuration + logoDisplayGap + fadeDuration;
    const float fadeInEnd3 = startDelay + logoDisplayDuration + logoDisplayGap + fadeDuration + logoDisplayDuration + logoDisplayGap;
    const float fadeInEnd4 = startDelay + logoDisplayDuration + logoDisplayGap + fadeDuration + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap;

    const float fadeOutStart1 = startDelay + logoDisplayDuration - fadeDuration;
    const float fadeOutStart2 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration - fadeDuration;
    const float fadeOutStart3 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap + logoDisplayDuration - fadeDuration;
    const float fadeOutStart4 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap + logoDisplayDuration - fadeDuration;

    const float fadeOutEnd1 = startDelay + logoDisplayDuration;
    const float fadeOutEnd2 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration;
    const float fadeOutEnd3 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap + logoDisplayDuration;
    const float fadeOutEnd4 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap + logoDisplayDuration + logoDisplayGap + logoDisplayDuration;

    /*
    float fadeInStartX1 = fadeInStart2 - fadeInStart1;
    float fadeInStartX2 = fadeInStart3 - fadeInStart2;
    float fadeInStartX3 = fadeInStart4 - fadeInStart3;
    float fadeInEndX1 = fadeInEnd2 - fadeInEnd1;
    float fadeInEndX2 = fadeInEnd3 - fadeInEnd2;
    float fadeInEndX3 = fadeInEnd4 - fadeInEnd3;
    float fadeOutStartX1 = fadeOutStart2 - fadeOutStart1;
    float fadeOutStartX2 = fadeOutStart3- fadeOutStart2;
    float fadeOutStartX3 = fadeOutStart4 - fadeOutStart3;
    float fadeOutEndX1 = fadeOutEnd2 - fadeOutEnd1;
    float fadeOutEndX2 = fadeOutEnd3 - fadeOutEnd2;
    float fadeOutEndX3 = fadeOutEnd4 - fadeOutEnd3;
    */

    m_effect->SetFogEnabled(true);
    m_effect->SetFogStart(0.0);
    m_effect->SetFogEnd(1.0);

    if (timeStamp < fadeInStart1)
    {
        m_projectileTimer = 0.0;
        // Render nothing
    }
    else if (timeStamp < fadeOutEnd1)  // Render Jackson Industries Logo
    {
        SetLighting(LightingState::LIGHTINGSTATE_JI);
        m_effect->SetTexture(m_textureJI.Get());
        m_effect->SetNormalTexture(m_normalMapJI.Get());
        m_effect->SetSpecularTexture(m_specularJI.Get());
        if (timeStamp < fadeInEnd1)  // fade in
        {
            float colorIntensity = (timeStamp - fadeInStart1) / fadeDuration;
            float fogStart = colorIntensity + fogGap1;
            float fogEnd = colorIntensity + fogGap2;
            m_effect->SetFogStart(fogStart);
            m_effect->SetFogEnd(fogEnd);

            m_debugValue1 = colorIntensity;
            m_debugValue2 = fogStart;
            m_debugValue3 = fogEnd;
        }
        else if (timeStamp > fadeOutStart1) // fade out
        {
            float colorIntensity = (fadeOutEnd1 - timeStamp) / (fadeDuration);
            float fogStart = colorIntensity + fogGap1;
            float fogEnd = colorIntensity + fogGap2;
            m_effect->SetFogStart(fogStart);
            m_effect->SetFogEnd(fogEnd);

            m_debugValue1 = colorIntensity;
            m_debugValue2 = fogStart;
            m_debugValue3 = fogEnd;
        }
        else // display at full intesity
        {
            m_effect->SetFogEnabled(false);
        }
    }
    else if (timeStamp < fadeInStart2)
    {
        // render nothing
    }
    else if (timeStamp < fadeOutEnd2) // Render BMW Logo
    {
        SetLighting(LightingState::LIGHTINGSTATE_BMW);
        m_effect->SetTexture(m_textureBMW.Get());
        m_effect->SetNormalTexture(m_normalMapBMW.Get());
        m_effect->SetSpecularTexture(m_specularBMW.Get());
        if (timeStamp < fadeInEnd2)  // fade in
        {
            float colorIntensity = (timeStamp - fadeInStart2) / (fadeDuration);
            float fogStart = colorIntensity + fogGap1;
            float fogEnd = colorIntensity + fogGap2;
            m_effect->SetFogStart(fogStart);
            m_effect->SetFogEnd(fogEnd);

            m_debugValue1 = colorIntensity;
            m_debugValue2 = fogStart;
            m_debugValue3 = fogEnd;
        }
        else if (timeStamp > fadeOutStart2) // fade out
        {
            float colorIntensity = (fadeOutEnd2 - timeStamp) / (fadeDuration);
            float fogStart = colorIntensity + fogGap1;
            float fogEnd = colorIntensity + fogGap2;
            m_effect->SetFogStart(fogStart);
            m_effect->SetFogEnd(fogEnd);

            m_debugValue1 = colorIntensity;
            m_debugValue2 = fogStart;
            m_debugValue3 = fogEnd;
        }
        else
        {
            m_effect->SetFogEnabled(false);
            //AudioPlaySFX(XACT_WAVEBANK_AUDIOBANK::XACT_WAVEBANK_AUDIOBANK_COINSFX);
            //m_spriteBatch->Draw(m_bmwLogoTexture.Get(), m_bmwLogoPos, nullptr, fadeColor, 0.f, m_bmwLogoOrigin);
            //m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
    }
    else if (timeStamp < fadeInStart3)
    {
        // render nothing
    }
    else if (timeStamp < fadeOutEnd3) // Render Start Screen
    {
        SetLighting(LightingState::LIGHTINGSTATE_STARTSCREEN);
        m_effect->SetTexture(m_textureAutoGame.Get());
        m_effect->SetNormalTexture(m_normalMapAutoGame.Get());
        m_effect->SetSpecularTexture(m_specularAutoGame.Get());
        if (timeStamp < fadeInEnd3)  // fade in
        {
            float colorIntensity = (timeStamp - fadeInStart3) / (fadeDuration);
            float fogStart = colorIntensity + fogGap1;
            float fogEnd = colorIntensity + fogGap2;
            m_effect->SetFogStart(fogStart);
            m_effect->SetFogEnd(fogEnd);

            m_debugValue1 = colorIntensity;
            m_debugValue2 = fogStart;
            m_debugValue3 = fogEnd;
        }
        else if (timeStamp > fadeOutStart3) // fade out
        {
            float colorIntensity = (fadeOutEnd3 - timeStamp) / (fadeDuration);
            float fogStart = colorIntensity + fogGap1;
            float fogEnd = colorIntensity + fogGap2;
            m_effect->SetFogStart(fogStart);
            m_effect->SetFogEnd(fogEnd);

            m_debugValue1 = colorIntensity;
            m_debugValue2 = fogStart;
            m_debugValue3 = fogEnd;
        }
        else
        {
            m_effect->SetFogEnabled(false);
            //AudioPlaySFX(XACT_WAVEBANK_AUDIOBANK::XACT_WAVEBANK_AUDIOBANK_COINSFX);
            //m_spriteBatch->Draw(m_bmwLogoTexture.Get(), m_bmwLogoPos, nullptr, fadeColor, 0.f, m_bmwLogoOrigin);
            //m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
    }
    else if (timeStamp < fadeInStart4)
    {
        // render nothing
    }
    else if (timeStamp < fadeOutEnd4)  // Render Teaser Screen
    {
    SetLighting(LightingState::LIGHTINGSTATE_TEASERSCREEN);
        m_effect->SetTexture(m_textureTeaser.Get());
        m_effect->SetNormalTexture(m_normalMapTeaser.Get());
        m_effect->SetSpecularTexture(m_specularTeaser.Get());
        if (timeStamp < fadeInEnd4)  // fade in
        {
            float colorIntensity = (timeStamp - fadeInStart4) / (fadeDuration);
            float fogStart = colorIntensity + fogGap1;
            float fogEnd = colorIntensity + fogGap2;
            m_effect->SetFogStart(fogStart);
            m_effect->SetFogEnd(fogEnd);

            m_debugValue1 = colorIntensity;
            m_debugValue2 = fogStart;
            m_debugValue3 = fogEnd;
        }
        else if (timeStamp > fadeOutStart4) // fade out
        {
            float colorIntensity = (fadeOutEnd4 - timeStamp) / (fadeDuration);
            float fogStart = colorIntensity + fogGap1;
            float fogEnd = colorIntensity + fogGap2;
            m_effect->SetFogStart(fogStart);
            m_effect->SetFogEnd(fogEnd);

            m_debugValue1 = colorIntensity;
            m_debugValue2 = fogStart;
            m_debugValue3 = fogEnd;
        }
        else
        {
            m_effect->SetFogEnabled(false);
            //AudioPlaySFX(XACT_WAVEBANK_AUDIOBANK::XACT_WAVEBANK_AUDIOBANK_COINSFX);
            //m_spriteBatch->Draw(m_bmwLogoTexture.Get(), m_bmwLogoPos, nullptr, fadeColor, 0.f, m_bmwLogoOrigin);
            //m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
    }
    if (timeStamp > fadeOutEnd4 + logoDisplayGap)
    {
        float time = m_projectileTimer;

        int x = 0;
        //m_projectileTimer = 0.0;
        //AudioPlayMusic(XACT_WAVEBANK_AUDIOBANK::XACT_WAVEBANK_AUDIOBANK_MUSIC01);
        //m_currentGameState = GameState::GAMESTATE_STARTSCREEN;
    }

    DrawLogoScreen();
}

void Game::DrawIntroScreen()
{
    float fadeDuration = 1.5f;
    float logoDisplayDuration = 5.f;
    float logoDisplayGap = 1.f;
    float startDelay = 4.2f;
    float timeStamp = static_cast<float>(m_timer.GetTotalSeconds());

    float fadeInStart1 = startDelay;
    float fadeInStart2 = startDelay + logoDisplayDuration + logoDisplayGap;
    float fadeInEnd1 = startDelay + fadeDuration;
    float fadeInEnd2 = startDelay + logoDisplayDuration + logoDisplayGap + fadeDuration;
    float fadeOutStart1 = startDelay + logoDisplayDuration - fadeDuration;
    float fadeOutStart2 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration - fadeDuration;
    float fadeOutEnd1 = startDelay + logoDisplayDuration;
    float fadeOutEnd2 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration;

    DirectX::XMVECTORF32 fadeColor = DirectX::Colors::White;

    if (timeStamp < fadeInStart1)
    {
        // Render nothing
    }
    else if (timeStamp < fadeOutEnd1)
    {
        std::string textLine = "Proudly Presents";
        float textLinePosX = m_bitwiseFontPos.x;
        float textLinePosY = m_bitwiseFontPos.y + 100;
        DirectX::SimpleMath::Vector2 textLinePos(textLinePosX, textLinePosY);
        DirectX::SimpleMath::Vector2 textLineOrigin = m_bitwiseFont->MeasureString(textLine.c_str()) / 2.f;

        if (timeStamp < fadeInEnd1)  // fade in
        {
            float colorIntensity = (timeStamp - fadeInStart1) / fadeDuration;
            fadeColor.f[0] = colorIntensity;
            fadeColor.f[1] = colorIntensity;
            fadeColor.f[2] = colorIntensity;
            m_spriteBatch->Draw(m_jiLogoTexture.Get(), m_jiLogoPos, nullptr, fadeColor, 0.f, m_jiLogoOrigin);
            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
        else if (timeStamp > fadeOutStart1) // fade out
        {
            float colorIntensity = (fadeOutEnd1 - timeStamp) / (fadeDuration);
            fadeColor.f[0] = colorIntensity;
            fadeColor.f[1] = colorIntensity;
            fadeColor.f[2] = colorIntensity;
            m_spriteBatch->Draw(m_jiLogoTexture.Get(), m_jiLogoPos, nullptr, fadeColor, 0.f, m_jiLogoOrigin);
            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
        else // display at full intesity
        {
            m_spriteBatch->Draw(m_jiLogoTexture.Get(), m_jiLogoPos, nullptr, fadeColor, 0.f, m_jiLogoOrigin);
            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
    }
    else if (timeStamp < fadeInStart2)
    {
        // render nothing
    }
    else if (timeStamp < fadeOutEnd2)
    {
        std::string textLine = "A Baltimore Magic Werks Production";
        float textLinePosX = m_bitwiseFontPos.x;
        float textLinePosY = m_bitwiseFontPos.y + 450;
        DirectX::SimpleMath::Vector2 textLinePos(textLinePosX, textLinePosY);
        DirectX::SimpleMath::Vector2 textLineOrigin = m_bitwiseFont->MeasureString(textLine.c_str()) / 2.f;
        if (timeStamp < fadeInEnd2)  // fade in
        {
            float colorIntensity = (timeStamp - fadeInStart2) / (fadeDuration);
            fadeColor.f[0] = colorIntensity;
            fadeColor.f[1] = colorIntensity;
            fadeColor.f[2] = colorIntensity;
            m_spriteBatch->Draw(m_bmwLogoTexture.Get(), m_bmwLogoPos, nullptr, fadeColor, 0.f, m_bmwLogoOrigin);
            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
        else if (timeStamp > fadeOutStart2) // fade out
        {
            float colorIntensity = (fadeOutEnd2 - timeStamp) / (fadeDuration);
            fadeColor.f[0] = colorIntensity;
            fadeColor.f[1] = colorIntensity;
            fadeColor.f[2] = colorIntensity;
            m_spriteBatch->Draw(m_bmwLogoTexture.Get(), m_bmwLogoPos, nullptr, fadeColor, 0.f, m_bmwLogoOrigin);
            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
        else
        {
            AudioPlaySFX(XACT_WAVEBANK_AUDIOBANK::XACT_WAVEBANK_AUDIOBANK_COINSFX);
            m_spriteBatch->Draw(m_bmwLogoTexture.Get(), m_bmwLogoPos, nullptr, fadeColor, 0.f, m_bmwLogoOrigin);
            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
    }
    if (timeStamp > fadeOutEnd2 + logoDisplayGap)
    {
        AudioPlayMusic(XACT_WAVEBANK_AUDIOBANK::XACT_WAVEBANK_AUDIOBANK_MUSIC01);
        m_currentGameState = GameState::GAMESTATE_STARTSCREEN;
    }
}

void Game::DrawLightBar()
{
    DirectX::XMVECTORF32 color1 = DirectX::Colors::Red;
    DirectX::XMVECTORF32 color2 = DirectX::Colors::Black;
    const float timeStamp = static_cast<float>(m_timer.GetTotalSeconds());
    float focusPoint = cosf(timeStamp * 3.) / 2.0;
    DirectX::SimpleMath::Vector3 normal = -DirectX::SimpleMath::Vector3::UnitX;
    float x = 1.09;
    float y = -0.1;
    float z = .7;
    DirectX::SimpleMath::Vector3 left(x, y, -z);
    DirectX::SimpleMath::Vector3 right(x, y, z);
    DirectX::SimpleMath::Vector3 focus(x, y, focusPoint);

    float spacing = .002;
    for (int i = 0; i < 10; ++i)
    {
        left.y += spacing;
        right.y += spacing;
        focus.y += spacing;
        VertexPositionNormalColor v0(left, normal, color2);
        VertexPositionNormalColor v1(right, normal, color2);
        VertexPositionNormalColor v2(focus, normal, color1);
        m_batch2->DrawLine(v0, v2);
        m_batch2->DrawLine(v2, v1);
    }
}

void Game::DrawLightFocus1()
{
    const float line = .25f;
    //DirectX::SimpleMath::Vector3 focalPoint = m_camera->GetTargetPos();
    //DirectX::SimpleMath::Vector3 focalPoint = m_lightPos0;
    DirectX::SimpleMath::Vector3 focalPoint = DirectX::SimpleMath::Vector3::Zero;
    DirectX::SimpleMath::Vector3 yLine = focalPoint;
    yLine.y += line;
    DirectX::SimpleMath::Vector3 yLine2 = focalPoint;
    yLine2.y -= line;
    DirectX::SimpleMath::Vector3 xLine = focalPoint;
    xLine.x += line;
    DirectX::SimpleMath::Vector3 xLine2 = focalPoint;
    xLine2.x -= line;
    DirectX::SimpleMath::Vector3 zLine = focalPoint;
    zLine.z += line;
    DirectX::SimpleMath::Vector3 zLine2 = focalPoint;
    zLine2.z -= line;

    DirectX::SimpleMath::Matrix rotMatrix1 = DirectX::SimpleMath::Matrix::CreateRotationX(Utility::ToRadians(45.0));
    DirectX::SimpleMath::Matrix rotMatrix2 = DirectX::SimpleMath::Matrix::CreateRotationY(Utility::ToRadians(45.0));
    //rotMatrix += DirectX::SimpleMath::Matrix::CreateRotationZ(Utility::ToRadians(45.0));
    DirectX::SimpleMath::Matrix rotMatrix = rotMatrix1 * rotMatrix2;
    //rotMatrix = DirectX::SimpleMath::Matrix::T

    DirectX::SimpleMath::Vector3::Transform(yLine, rotMatrix, yLine);
    DirectX::SimpleMath::Vector3::Transform(xLine, rotMatrix, xLine);
    DirectX::SimpleMath::Vector3::Transform(zLine, rotMatrix, zLine);
    DirectX::SimpleMath::Vector3::Transform(yLine2, rotMatrix, yLine2);
    DirectX::SimpleMath::Vector3::Transform(xLine2, rotMatrix, xLine2);
    DirectX::SimpleMath::Vector3::Transform(zLine2, rotMatrix, zLine2);

    focalPoint = m_lightPos0;
    yLine += focalPoint;
    xLine += focalPoint;
    zLine += focalPoint;
    yLine2 += focalPoint;
    xLine2 += focalPoint;
    zLine2 += focalPoint;

    VertexPositionNormalColor origin(focalPoint, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor yOffset(yLine, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor xOffset(xLine, DirectX::SimpleMath::Vector3::UnitZ, Colors::Yellow);
    VertexPositionNormalColor zOffset(zLine, DirectX::SimpleMath::Vector3::UnitY, Colors::Yellow);

    VertexPositionNormalColor yOffset2(yLine2, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor xOffset2(xLine2, DirectX::SimpleMath::Vector3::UnitZ, Colors::Yellow);
    VertexPositionNormalColor zOffset2(zLine2, DirectX::SimpleMath::Vector3::UnitY, Colors::Yellow);
    m_batch2->DrawLine(origin, yOffset);
    m_batch2->DrawLine(origin, xOffset);
    m_batch2->DrawLine(origin, zOffset);

    m_batch2->DrawLine(origin, yOffset2);
    m_batch2->DrawLine(origin, xOffset2);
    m_batch2->DrawLine(origin, zOffset2);
}

void Game::DrawLightFocus2()
{
    const float line = .25f;
    //DirectX::SimpleMath::Vector3 focalPoint = m_camera->GetTargetPos();
    DirectX::SimpleMath::Vector3 focalPoint = m_lightPos1;
    DirectX::SimpleMath::Vector3 yLine = focalPoint;
    yLine.y += line;
    DirectX::SimpleMath::Vector3 yLine2 = focalPoint;
    yLine2.y -= line;
    DirectX::SimpleMath::Vector3 xLine = focalPoint;
    xLine.x += line;
    DirectX::SimpleMath::Vector3 xLine2 = focalPoint;
    xLine2.x -= line;
    DirectX::SimpleMath::Vector3 zLine = focalPoint;
    zLine.z += line;
    DirectX::SimpleMath::Vector3 zLine2 = focalPoint;
    zLine2.z -= line;
    VertexPositionNormalColor origin(focalPoint, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor yOffset(yLine, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor xOffset(xLine, DirectX::SimpleMath::Vector3::UnitZ, Colors::Yellow);
    VertexPositionNormalColor zOffset(zLine, DirectX::SimpleMath::Vector3::UnitY, Colors::Yellow);

    VertexPositionNormalColor yOffset2(yLine2, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor xOffset2(xLine2, DirectX::SimpleMath::Vector3::UnitZ, Colors::Yellow);
    VertexPositionNormalColor zOffset2(zLine2, DirectX::SimpleMath::Vector3::UnitY, Colors::Yellow);
    m_batch2->DrawLine(origin, yOffset);
    m_batch2->DrawLine(origin, xOffset);
    m_batch2->DrawLine(origin, zOffset);

    m_batch2->DrawLine(origin, yOffset2);
    m_batch2->DrawLine(origin, xOffset2);
    m_batch2->DrawLine(origin, zOffset2);
}

void Game::DrawLightFocus3()
{
    const float line = .25f;
    //DirectX::SimpleMath::Vector3 focalPoint = m_camera->GetTargetPos();
    //DirectX::SimpleMath::Vector3 focalPoint = m_lightPos0;
    DirectX::SimpleMath::Vector3 focalPoint = DirectX::SimpleMath::Vector3::Zero;
    DirectX::SimpleMath::Vector3 yLine = focalPoint;
    yLine.y += line;
    DirectX::SimpleMath::Vector3 yLine2 = focalPoint;
    yLine2.y -= line;
    DirectX::SimpleMath::Vector3 xLine = focalPoint;
    xLine.x += line;
    DirectX::SimpleMath::Vector3 xLine2 = focalPoint;
    xLine2.x -= line;
    DirectX::SimpleMath::Vector3 zLine = focalPoint;
    zLine.z += line;
    DirectX::SimpleMath::Vector3 zLine2 = focalPoint;
    zLine2.z -= line;

    DirectX::SimpleMath::Matrix rotMatrix1 = DirectX::SimpleMath::Matrix::CreateRotationY(Utility::ToRadians(45.0));
    DirectX::SimpleMath::Matrix rotMatrix2 = DirectX::SimpleMath::Matrix::CreateRotationZ(Utility::ToRadians(45.0));
    //rotMatrix += DirectX::SimpleMath::Matrix::CreateRotationZ(Utility::ToRadians(45.0));
    DirectX::SimpleMath::Matrix rotMatrix = rotMatrix1 * rotMatrix2;
    //rotMatrix = DirectX::SimpleMath::Matrix::T

    DirectX::SimpleMath::Vector3::Transform(yLine, rotMatrix, yLine);
    DirectX::SimpleMath::Vector3::Transform(xLine, rotMatrix, xLine);
    DirectX::SimpleMath::Vector3::Transform(zLine, rotMatrix, zLine);
    DirectX::SimpleMath::Vector3::Transform(yLine2, rotMatrix, yLine2);
    DirectX::SimpleMath::Vector3::Transform(xLine2, rotMatrix, xLine2);
    DirectX::SimpleMath::Vector3::Transform(zLine2, rotMatrix, zLine2);

    focalPoint = m_lightPos2;
    yLine += focalPoint;
    xLine += focalPoint;
    zLine += focalPoint;
    yLine2 += focalPoint;
    xLine2 += focalPoint;
    zLine2 += focalPoint;

    VertexPositionNormalColor origin(focalPoint, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor yOffset(yLine, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor xOffset(xLine, DirectX::SimpleMath::Vector3::UnitZ, Colors::Yellow);
    VertexPositionNormalColor zOffset(zLine, DirectX::SimpleMath::Vector3::UnitY, Colors::Yellow);

    VertexPositionNormalColor yOffset2(yLine2, DirectX::SimpleMath::Vector3::UnitX, Colors::Yellow);
    VertexPositionNormalColor xOffset2(xLine2, DirectX::SimpleMath::Vector3::UnitZ, Colors::Yellow);
    VertexPositionNormalColor zOffset2(zLine2, DirectX::SimpleMath::Vector3::UnitY, Colors::Yellow);
    m_batch2->DrawLine(origin, yOffset);
    m_batch2->DrawLine(origin, xOffset);
    m_batch2->DrawLine(origin, zOffset);

    m_batch2->DrawLine(origin, yOffset2);
    m_batch2->DrawLine(origin, xOffset2);
    m_batch2->DrawLine(origin, zOffset2);
}

void Game::DrawLogoScreen()
{ 
    DirectX::SimpleMath::Vector3 testNorm(0.0, 0.0, 1.0);
    testNorm.Normalize();

    auto time = static_cast<float>(m_timer.GetTotalSeconds());
    float yaw = time * 0.4f;
    float pitch = time * 0.7f;
    float roll = time * 1.1f;
    auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
    quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);
    auto norm01 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat0);

    testNorm = norm01;
    //DirectX::SimpleMath::Vector3 vertexNormal = testNorm;
    const DirectX::SimpleMath::Vector3 vertexNormal = DirectX::SimpleMath::Vector3::UnitX;
    const DirectX::SimpleMath::Vector3 vertexColor = DirectX::Colors::White;
    const float height = .5f;
    const float width = .888888888f;
    const float distance = 1.1f;

    m_camera->SetPos(DirectX::SimpleMath::Vector3::Zero);
    m_camera->SetTargetPos(DirectX::SimpleMath::Vector3(distance, 0.0, 0.0));

    DirectX::SimpleMath::Vector3 topLeft(distance, height, -width);
    DirectX::SimpleMath::Vector3 topRight(distance, height, width);
    DirectX::SimpleMath::Vector3 bottomRight(distance, -height, width);
    DirectX::SimpleMath::Vector3 bottomLeft(distance, -height, -width);
    VertexPositionNormalColorTexture vertTopLeft(topLeft, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(0, 0));
    VertexPositionNormalColorTexture vertTopRight(topRight, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(1, 0));
    VertexPositionNormalColorTexture vertBottomRight(bottomRight, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalColorTexture vertBottomLeft(bottomLeft, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(0, 1));

    m_batch->DrawQuad(vertTopLeft, vertTopRight, vertBottomRight, vertBottomLeft);
}

void Game::DrawMenuEnvironmentSelect()
{

}

void Game::DrawMenuMain()
{
    float lineDrawY = m_fontMenuPos.y + 15;
    float lineDrawSpacingY = 15;
    std::string menuTitle = "Main Menu";
    float menuTitlePosX = m_fontMenuPos.x;
    float menuTitlePosY = lineDrawY;
    DirectX::SimpleMath::Vector2 menuTitlePos(menuTitlePosX, menuTitlePosY);
    DirectX::SimpleMath::Vector2 menuOrigin = m_titleFont->MeasureString(menuTitle.c_str()) / 2.f;
    m_titleFont->DrawString(m_spriteBatch.get(), menuTitle.c_str(), menuTitlePos + DirectX::SimpleMath::Vector2(4.f, 4.f), Colors::Green, 0.f, menuOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), menuTitle.c_str(), menuTitlePos + DirectX::SimpleMath::Vector2(3.f, 3.f), Colors::Green, 0.f, menuOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), menuTitle.c_str(), menuTitlePos + DirectX::SimpleMath::Vector2(2.f, 2.f), Colors::Green, 0.f, menuOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), menuTitle.c_str(), menuTitlePos + DirectX::SimpleMath::Vector2(1.f, 1.f), Colors::Green, 0.f, menuOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), menuTitle.c_str(), menuTitlePos + DirectX::SimpleMath::Vector2(-1.f, -1.f), Colors::LawnGreen, 0.f, menuOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), menuTitle.c_str(), menuTitlePos, Colors::White, 0.f, menuOrigin);

    lineDrawY += menuTitlePosY + lineDrawSpacingY;
    std::string menuObj0String = "Play!";
    DirectX::SimpleMath::Vector2 menuObj0Pos(menuTitlePosX, lineDrawY);
    DirectX::SimpleMath::Vector2 menuObj0Origin = m_font->MeasureString(menuObj0String.c_str()) / 2.f;

    lineDrawY += menuObj0Pos.y;
    std::string menuObj1String = "Character Select";
    DirectX::SimpleMath::Vector2 menuObj1Pos(menuTitlePosX, lineDrawY);
    DirectX::SimpleMath::Vector2 menuObj1Origin = m_font->MeasureString(menuObj1String.c_str()) / 2.f;

    lineDrawY += menuObj0Pos.y;
    std::string menuObj2String = "Environment Select";
    DirectX::SimpleMath::Vector2 menuObj2Pos(menuTitlePosX, lineDrawY);
    DirectX::SimpleMath::Vector2 menuObj2Origin = m_font->MeasureString(menuObj2String.c_str()) / 2.f;

    // Demo
    ////////////////////////////

    lineDrawY += menuObj0Pos.y;
    std::string menuObjHydraString = "Hole 12 Golden Bell";
    DirectX::SimpleMath::Vector2 menuObjHydraPos(menuTitlePosX, lineDrawY);
    DirectX::SimpleMath::Vector2 menuObjHydraOrigin = m_font->MeasureString(menuObjHydraString.c_str()) / 2.f;

    ///////////////////////////

    lineDrawY += menuObj0Pos.y;
    std::string menuObj3String = "Quit";
    DirectX::SimpleMath::Vector2 menuObj3Pos(menuTitlePosX, lineDrawY);
    DirectX::SimpleMath::Vector2 menuObj3Origin = m_font->MeasureString(menuObj3String.c_str()) / 2.f;

    if (m_menuSelect < 0 || m_menuSelect > 4)
    {
        m_menuSelect = 0;
    }
    if (m_menuSelect == 0)
    {
        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos + DirectX::SimpleMath::Vector2(4.f, 4.f), Colors::White, 0.f, menuObj0Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos + DirectX::SimpleMath::Vector2(-4.f, 4.f), Colors::White, 0.f, menuObj0Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos + DirectX::SimpleMath::Vector2(-4.f, -4.f), Colors::White, 0.f, menuObj0Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos + DirectX::SimpleMath::Vector2(4.f, -4.f), Colors::White, 0.f, menuObj0Origin);

        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos + DirectX::SimpleMath::Vector2(2.f, 2.f), Colors::Black, 0.f, menuObj0Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos + DirectX::SimpleMath::Vector2(-2.f, 2.f), Colors::Black, 0.f, menuObj0Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos + DirectX::SimpleMath::Vector2(-2.f, -2.f), Colors::Black, 0.f, menuObj0Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos + DirectX::SimpleMath::Vector2(2.f, -2.f), Colors::Black, 0.f, menuObj0Origin);

        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos, Colors::White, 0.f, menuObj0Origin);
    }
    else
    {
        m_font->DrawString(m_spriteBatch.get(), menuObj0String.c_str(), menuObj0Pos, Colors::White, 0.f, menuObj0Origin);
    }

    if (m_menuSelect == 1)
    {
        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos + DirectX::SimpleMath::Vector2(4.f, 4.f), Colors::White, 0.f, menuObj1Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos + DirectX::SimpleMath::Vector2(-4.f, 4.f), Colors::White, 0.f, menuObj1Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos + DirectX::SimpleMath::Vector2(-4.f, -4.f), Colors::White, 0.f, menuObj1Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos + DirectX::SimpleMath::Vector2(4.f, -4.f), Colors::White, 0.f, menuObj1Origin);

        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos + DirectX::SimpleMath::Vector2(2.f, 2.f), Colors::Black, 0.f, menuObj1Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos + DirectX::SimpleMath::Vector2(-2.f, 2.f), Colors::Black, 0.f, menuObj1Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos + DirectX::SimpleMath::Vector2(-2.f, -2.f), Colors::Black, 0.f, menuObj1Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos + DirectX::SimpleMath::Vector2(2.f, -2.f), Colors::Black, 0.f, menuObj1Origin);

        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos, Colors::White, 0.f, menuObj1Origin);
    }
    else
    {
        m_font->DrawString(m_spriteBatch.get(), menuObj1String.c_str(), menuObj1Pos, Colors::White, 0.f, menuObj1Origin);
    }

    if (m_menuSelect == 2)
    {
        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos + DirectX::SimpleMath::Vector2(4.f, 4.f), Colors::White, 0.f, menuObj2Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos + DirectX::SimpleMath::Vector2(-4.f, 4.f), Colors::White, 0.f, menuObj2Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos + DirectX::SimpleMath::Vector2(-4.f, -4.f), Colors::White, 0.f, menuObj2Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos + DirectX::SimpleMath::Vector2(4.f, -4.f), Colors::White, 0.f, menuObj2Origin);

        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos + DirectX::SimpleMath::Vector2(2.f, 2.f), Colors::Black, 0.f, menuObj2Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos + DirectX::SimpleMath::Vector2(-2.f, 2.f), Colors::Black, 0.f, menuObj2Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos + DirectX::SimpleMath::Vector2(-2.f, -2.f), Colors::Black, 0.f, menuObj2Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos + DirectX::SimpleMath::Vector2(2.f, -2.f), Colors::Black, 0.f, menuObj2Origin);

        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos, Colors::White, 0.f, menuObj2Origin);
    }
    else
    {
        m_font->DrawString(m_spriteBatch.get(), menuObj2String.c_str(), menuObj2Pos, Colors::White, 0.f, menuObj2Origin);
    }

    if (m_menuSelect == 3)
    {
        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos + DirectX::SimpleMath::Vector2(4.f, 4.f), Colors::White, 0.f, menuObjHydraOrigin);
        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos + DirectX::SimpleMath::Vector2(-4.f, 4.f), Colors::White, 0.f, menuObjHydraOrigin);
        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos + DirectX::SimpleMath::Vector2(-4.f, -4.f), Colors::White, 0.f, menuObjHydraOrigin);
        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos + DirectX::SimpleMath::Vector2(4.f, -4.f), Colors::White, 0.f, menuObjHydraOrigin);

        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos + DirectX::SimpleMath::Vector2(2.f, 2.f), Colors::Black, 0.f, menuObjHydraOrigin);
        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos + DirectX::SimpleMath::Vector2(-2.f, 2.f), Colors::Black, 0.f, menuObjHydraOrigin);
        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos + DirectX::SimpleMath::Vector2(-2.f, -2.f), Colors::Black, 0.f, menuObjHydraOrigin);
        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos + DirectX::SimpleMath::Vector2(2.f, -2.f), Colors::Black, 0.f, menuObjHydraOrigin);

        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos, Colors::White, 0.f, menuObjHydraOrigin);
    }
    else
    {
        m_font->DrawString(m_spriteBatch.get(), menuObjHydraString.c_str(), menuObjHydraPos, Colors::White, 0.f, menuObjHydraOrigin);
    }

    if (m_menuSelect == 4)
    {
        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos + DirectX::SimpleMath::Vector2(4.f, 4.f), Colors::White, 0.f, menuObj3Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos + DirectX::SimpleMath::Vector2(-4.f, 4.f), Colors::White, 0.f, menuObj3Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos + DirectX::SimpleMath::Vector2(-4.f, -4.f), Colors::White, 0.f, menuObj3Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos + DirectX::SimpleMath::Vector2(4.f, -4.f), Colors::White, 0.f, menuObj3Origin);

        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos + DirectX::SimpleMath::Vector2(2.f, 2.f), Colors::Black, 0.f, menuObj3Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos + DirectX::SimpleMath::Vector2(-2.f, 2.f), Colors::Black, 0.f, menuObj3Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos + DirectX::SimpleMath::Vector2(-2.f, -2.f), Colors::Black, 0.f, menuObj3Origin);
        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos + DirectX::SimpleMath::Vector2(2.f, -2.f), Colors::Black, 0.f, menuObj3Origin);

        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos, Colors::White, 0.f, menuObj3Origin);
    }
    else
    {
        m_font->DrawString(m_spriteBatch.get(), menuObj3String.c_str(), menuObj3Pos, Colors::White, 0.f, menuObj3Origin);
    }
}

void Game::DrawShape()
{
    DirectX::SimpleMath::Matrix testMatrix = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix world = m_world;
    DirectX::SimpleMath::Matrix view = m_view;
    DirectX::SimpleMath::Matrix proj = m_proj;

    DirectX::SimpleMath::Matrix testView = DirectX::SimpleMath::Matrix::CreateLookAt(SimpleMath::Vector3::Zero, SimpleMath::Vector3(1.0, 1.0, 1.0), SimpleMath::Vector3::UnitY);

    SimpleMath::Matrix testMat1 = SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(1.0, 1.0, 1.0));

    //m_projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.f, static_cast<float>(m_clientWidth) / static_cast<float>(m_clientHeight), m_nearPlane, m_farPlane);
    //m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_target, m_up);

    SimpleMath::Vector3 transVec(0.5, 0.5, 1.5);
    SimpleMath::Matrix transWorld = SimpleMath::Matrix::CreateWorld(SimpleMath::Vector3::Zero, SimpleMath::Vector3::UnitX, SimpleMath::Vector3::UnitY);

    SimpleMath::Matrix transMat = SimpleMath::Matrix::CreateTranslation(transVec);
    //proj += transMat;

   // world = SimpleMath::Matrix::Transform(

    m_shape->Draw(m_world, m_view, m_proj);
    //m_shape->Draw(m_world, m_view, m_proj, Colors::White, m_textureBMW.Get());
    //m_shape->Draw(transWorld, testMatrix, testMatrix);
}

void Game::DrawStartScreen()
{
    float timeStamp = static_cast<float>(m_timer.GetTotalSeconds());
    SetLighting(LightingState::LIGHTINGSTATE_STARTSCREEN);
    //SetLighting(LightingState::LIGHTINGSTATE_NULL);
    //SetLighting(LightingState::LIGHTINGSTATE_BMW);
    //SetLighting(LightingState::LIGHTINGSTATE_);
    
    
    m_effect->SetTexture(m_textureAutoGame.Get());
    m_effect->SetNormalTexture(m_normalMapAutoGame.Get());
    m_effect->SetSpecularTexture(m_specularAutoGame.Get());
    
    /*
    m_effect->SetTexture(m_textureJI.Get());
    m_effect->SetNormalTexture(m_normalMapJI.Get());
    m_effect->SetSpecularTexture(m_specularJI.Get());
    */

    const DirectX::SimpleMath::Vector3 vertexColor = DirectX::Colors::White;
    DirectX::SimpleMath::Vector3 testNorm(0.0, 0.0, 1.0);
    testNorm.Normalize();

    auto time = static_cast<float>(m_timer.GetTotalSeconds());
    float yaw = time * 0.4f;
    float pitch = time * 0.7f;
    float roll = time * 1.1f;
    roll = cosf(-timeStamp ) +1.7;
    //roll = 3.1f;
    //auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
    auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, roll);
    //auto norm01 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat0);
    auto norm01 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitZ, quat0);
    testNorm = norm01;

    //DirectX::SimpleMath::Vector3 vertexNormal = testNorm;
    DirectX::SimpleMath::Vector3 vertexNormal = DirectX::SimpleMath::Vector3::UnitX;

    //m_camera->SetPos(DirectX::SimpleMath::Vector3::Zero);
    //m_camera->SetTargetPos(DirectX::SimpleMath::Vector3(distance, 0.0, 0.0));

    // start background drawing
    const float distance = 1.1f;
    const float height = 1.5f;
    const float width = 1.77777777;
    const float heightBase = 0.0f;
    DirectX::SimpleMath::Vector3 topLeft(distance, height, -width);
    DirectX::SimpleMath::Vector3 topRight(distance, height, width);
    DirectX::SimpleMath::Vector3 bottomRight(distance, heightBase, width);
    DirectX::SimpleMath::Vector3 bottomLeft(distance, heightBase, -width);

    float uStart = 0.0;
    float uStop = 1.0;
    float vStart = 0.25;
    float vStop = 1.0;
    /////////////////////////////
    //DirectX::SimpleMath::Vector3 holdNorm = vertexNormal;
    //vertexNormal = -m_lightPos0 + testNorm;
    /////////////////////////////

    //vertexNormal = testNorm;
    vertexNormal = DirectX::SimpleMath::Vector3::UnitX;
    
    VertexPositionNormalColorTexture vertTopLeft(topLeft, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStart, vStart));
    VertexPositionNormalColorTexture vertTopRight(topRight, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStop, vStart));
    VertexPositionNormalColorTexture vertBottomRight(bottomRight, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStop, vStop));
    VertexPositionNormalColorTexture vertBottomLeft(bottomLeft, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStart, vStop));

    m_batch->DrawQuad(vertTopLeft, vertTopRight, vertBottomRight, vertBottomLeft);

    //vertexNormal = holdNorm;
    vertexNormal = DirectX::SimpleMath::Vector3::UnitX;

    // Start moon drawing
    const float moonHeight = 0.2;
    const float moonWidth = 0.2;
    const float moonSize = 0.2;
    const float moonOriginY = 0.8;
    const float moonOriginZ = -0.7;
    const float moonDepth = -0.01;
    DirectX::SimpleMath::Vector3 moonOrigin(distance, moonOriginY, moonOriginZ);
    topLeft = DirectX::SimpleMath::Vector3(moonDepth + distance, moonOriginY, moonOriginZ);
    topRight = DirectX::SimpleMath::Vector3(moonDepth, 0.0, moonSize);
    bottomRight = DirectX::SimpleMath::Vector3(moonDepth, -moonSize, moonSize);
    bottomLeft = DirectX::SimpleMath::Vector3(moonDepth, -moonSize, 0.0);

    topRight += moonOrigin;
    bottomRight += moonOrigin;
    bottomLeft += moonOrigin;

    uStart = 0.0;
    uStop = 0.1588541666666667;
    vStart = 0.0;
    vStop = 0.2824074074074074;

    
    vertexNormal = DirectX::SimpleMath::Vector3::UnitX;
    
    //vertexNormal = -m_lightPos0 * cosf(timeStamp);
    //vertexNormal = testNorm;
    //vertexNormal = DirectX::SimpleMath::Vector3::UnitX;
    vertTopLeft = VertexPositionNormalColorTexture (topLeft, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStart, vStart));
    vertTopRight = VertexPositionNormalColorTexture (topRight, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStop, vStart));
    vertBottomRight = VertexPositionNormalColorTexture (bottomRight, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStop, vStop));
    vertBottomLeft = VertexPositionNormalColorTexture (bottomLeft, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStart, vStop));

    /*
    m_batch->End();
    m_batch->Begin();
    auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
    if (ilights)
    {
        //ilights->EnableDefaultLighting();
        ilights->SetLightEnabled(0, false);
        ilights->SetLightEnabled(1, false);
        ilights->SetLightEnabled(2, false);
        m_effect->Apply(m_d3dContext.Get());

        m_batch->DrawQuad(vertTopLeft, vertTopRight, vertBottomRight, vertBottomLeft);

        ilights->SetLightEnabled(0, false);
        ilights->SetLightEnabled(1, false);
        ilights->SetLightEnabled(2, true);
        m_effect->Apply(m_d3dContext.Get());
    }
    */
    /*
    m_batch->End();
    m_batch->Begin();
    SetLighting(LightingState::LIGHTINGSTATE_BMW);
    UpdateLighting();
    SetLighting(LightingState::LIGHTINGSTATE_STARTSCREEN);

    m_effect->SetLightEnabled(1, false);
    m_effect->Apply(m_d3dContext.Get());
    */


    m_batch->DrawQuad(vertTopLeft, vertTopRight, vertBottomRight, vertBottomLeft);



    //m_effect->SetLightEnabled(1, true);
    //m_effect->Apply(m_d3dContext.Get());



    ////////////////////////////////
    // Start Text drawing
    const float titleWidth = 0.4;
    //const float titleHeight = 0.2;
    const float titleHeight = titleWidth * 0.111864406779661;
    const float titleSize = 0.2;
    const float titleOriginY = 0.5;
    const float titleOriginZ = 0.0;
    const float titleDepth = distance - 0.01;
    DirectX::SimpleMath::Vector3 titleOrigin(0.0, titleOriginY, titleOriginZ);
    topLeft = DirectX::SimpleMath::Vector3(titleDepth, titleHeight, -titleWidth);
    topRight = DirectX::SimpleMath::Vector3(titleDepth, titleHeight, titleWidth);
    bottomRight = DirectX::SimpleMath::Vector3(titleDepth, -titleHeight, titleWidth);
    bottomLeft = DirectX::SimpleMath::Vector3(titleDepth, -titleHeight, -titleWidth);


    topLeft += titleOrigin;
    topRight += titleOrigin;
    bottomRight += titleOrigin;
    bottomLeft += titleOrigin;

    uStart = 0.6927083333333333;
    uStop = 1.0;
    vStart = 0.0;
    vStop = 0.0611111111111111;

    vertexNormal = DirectX::SimpleMath::Vector3::UnitX;
    vertTopLeft = VertexPositionNormalColorTexture(topLeft, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStart, vStart));
    vertTopRight = VertexPositionNormalColorTexture(topRight, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStop, vStart));
    vertBottomRight = VertexPositionNormalColorTexture(bottomRight, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStop, vStop));
    vertBottomLeft = VertexPositionNormalColorTexture(bottomLeft, vertexNormal, vertexColor, DirectX::SimpleMath::Vector2(uStart, vStop));

    m_batch->DrawQuad(vertTopLeft, vertTopRight, vertBottomRight, vertBottomLeft);
}

void Game::DrawStartScreenOld()
{
    const std::string title = "AutoGame1989";
    const std::string author = "By Lehr Jackson";
    const std::string startText = "Press Enter to Start";
    DirectX::XMVECTORF32 logoColor1 = DirectX::Colors::OrangeRed;
    DirectX::XMVECTORF32 logoColor2 = DirectX::Colors::White;
    DirectX::XMVECTORF32 logoColor3 = DirectX::Colors::Silver;

    float fontTitlePosX = m_fontPos.x;
    float fontTitlePosY = m_fontPos.y / 2.f;
    DirectX::SimpleMath::Vector2 titlePos(fontTitlePosX, fontTitlePosY);
    float fontAuthorPosX = m_fontPos.x;
    float fontAuthorPosY = m_fontPos.y;
    DirectX::SimpleMath::Vector2 authorPos(fontAuthorPosX, fontAuthorPosY);
    DirectX::SimpleMath::Vector2 startTextPos(m_fontPos.x, m_fontPos.y + fontTitlePosY);

    DirectX::SimpleMath::Vector2 titleOrigin = m_titleFont->MeasureString(title.c_str()) / 2.f;
    DirectX::SimpleMath::Vector2 authorOrigin = m_font->MeasureString(author.c_str()) / 2.f;
    DirectX::SimpleMath::Vector2 startTextOrigin = m_font->MeasureString(startText.c_str()) / 2.f;

    //m_titleFont->DrawString(m_spriteBatch.get(), title.c_str(), titlePos + DirectX::SimpleMath::Vector2(7.f, 7.f), logoColor1, 0.f, titleOrigin);
    //m_titleFont->DrawString(m_spriteBatch.get(), title.c_str(), titlePos + DirectX::SimpleMath::Vector2(6.f, 6.f), logoColor1, 0.f, titleOrigin);
    /*
    m_titleFont->DrawString(m_spriteBatch.get(), title.c_str(), titlePos + DirectX::SimpleMath::Vector2(5.f, 5.f), logoColor1, 0.f, titleOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), title.c_str(), titlePos + DirectX::SimpleMath::Vector2(4.f, 4.f), logoColor1, 0.f, titleOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), title.c_str(), titlePos + DirectX::SimpleMath::Vector2(3.f, 3.f), logoColor1, 0.f, titleOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), title.c_str(), titlePos + DirectX::SimpleMath::Vector2(2.f, 2.f), logoColor1, 0.f, titleOrigin);
    m_titleFont->DrawString(m_spriteBatch.get(), title.c_str(), titlePos + DirectX::SimpleMath::Vector2(-1.f, -1.f), logoColor3, 0.f, titleOrigin);
    */
    m_titleFont->DrawString(m_spriteBatch.get(), title.c_str(), titlePos, logoColor2, 0.f, titleOrigin);

    m_font->DrawString(m_spriteBatch.get(), author.c_str(), authorPos, Colors::White, 0.f, authorOrigin);
    m_font->DrawString(m_spriteBatch.get(), startText.c_str(), startTextPos, Colors::White, 0.f, startTextOrigin);
}

void Game::DrawTeaserScreen()
{
    float timePad = static_cast<float>(m_timer.GetTotalSeconds());

    float fadeDuration = 1.5f;
    float logoDisplayDuration = 5.f;
    float logoDisplayGap = 1.f;
    float startDelay = 4.2f;
    //float timeStamp = static_cast<float>(m_timer.GetTotalSeconds());
    float timeStamp = static_cast<float>(m_flightStepTimer.GetTotalSeconds()); // Piggy backing on this variable for the moment
    //float timeStamp = static_cast<float>(m_flightStepTimer.GetElapsedSeconds()); // Piggy backing on this variable for the moment

    float fadeInStart1 = startDelay;
    float fadeInStart2 = startDelay + logoDisplayDuration + logoDisplayGap;
    float fadeInEnd1 = startDelay + fadeDuration;
    float fadeInEnd2 = startDelay + logoDisplayDuration + logoDisplayGap + fadeDuration;
    float fadeOutStart1 = startDelay + logoDisplayDuration - fadeDuration;
    float fadeOutStart2 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration - fadeDuration;
    float fadeOutEnd1 = startDelay + logoDisplayDuration;
    float fadeOutEnd2 = startDelay + logoDisplayDuration + logoDisplayGap + logoDisplayDuration;

    DirectX::XMVECTORF32 fadeColor = DirectX::Colors::White;

    if (timeStamp < fadeInStart1)
    {
        // Render nothing
    }
    else if (timeStamp < fadeOutEnd1)
    {
        std::string textLine = "Coming Soon...";
        float textLinePosX = m_bitwiseFontPos.x;
        float textLinePosY = m_bitwiseFontPos.y;
        //float textLinePosY = m_bitwiseFontPos.y + 100;
        DirectX::SimpleMath::Vector2 textLinePos(textLinePosX, textLinePosY);
        DirectX::SimpleMath::Vector2 textLineOrigin = m_bitwiseFont->MeasureString(textLine.c_str()) / 2.f;

        if (timeStamp < fadeInEnd1)  // fade in
        {
            float colorIntensity = (timeStamp - fadeInStart1) / fadeDuration;
            fadeColor.f[0] = colorIntensity;
            fadeColor.f[1] = colorIntensity;
            fadeColor.f[2] = colorIntensity;

            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
        else if (timeStamp > fadeOutStart1) // fade out
        {
            float colorIntensity = (fadeOutEnd1 - timeStamp) / (fadeDuration);
            fadeColor.f[0] = colorIntensity;
            fadeColor.f[1] = colorIntensity;
            fadeColor.f[2] = colorIntensity;

            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
        else // display at full intesity
        {
            m_bitwiseFont->DrawString(m_spriteBatch.get(), textLine.c_str(), textLinePos, fadeColor, 0.f, textLineOrigin);
        }
    }
    else if (timeStamp < fadeInStart2)
    {
        // render nothing
    }
    else if (timeStamp < fadeOutEnd2)
    {

    }
    if (timeStamp > fadeOutEnd2 + logoDisplayGap)
    {

        m_currentGameState = GameState::GAMESTATE_STARTSCREEN;
    }
}

void Game::DrawTerrain()
{
    m_batch3->Draw(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_terrainVertexArrayBase, m_terrainVertexCount);
    m_batch3->Draw(D3D_PRIMITIVE_TOPOLOGY_LINELIST, m_terrainVertexArray, m_terrainVertexCount);
}

void Game::DrawTerrain2()
{
    m_batch2->Draw(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_terrainVertexArrayBase2, m_terrainVertexCount2);
    //m_batch2->Draw(D3D_PRIMITIVE_TOPOLOGY_LINELIST, m_terrainVertexArray2, m_terrainVertexCount2);
}

void Game::DrawTimer()
{
    // m_flightStepTimer
    // m_projectileTimer
    std::string textLine = "Timer = " + std::to_string(m_projectileTimer);
    float textLinePosX = m_bitwiseFontPos.x;
    float textLinePosY = m_bitwiseFontPos.y;
    //float textLinePosY = m_bitwiseFontPos.y + 100;
    DirectX::SimpleMath::Vector2 textLinePos(textLinePosX, textLinePosY);
    DirectX::SimpleMath::Vector2 textLineOrigin = m_bitwiseFont->MeasureString(textLine.c_str()) / 2.f;
    m_font->DrawString(m_spriteBatch.get(), textLine.c_str(), m_fontPosDebug, Colors::White, 0.f, textLineOrigin);
}

void Game::DrawWorld()
{
    for (int i = 0; i < m_worldGrid.size(); ++i)
    {
        m_batch2->DrawLine(m_worldGrid[i].first, m_worldGrid[i].second);
    }

    DirectX::XMVECTORF32 gridColor = DirectX::Colors::Red;
    DirectX::XMVECTORF32 gridColor1 = DirectX::Colors::LawnGreen;
    DirectX::XMVECTORF32 gridColor2 = DirectX::Colors::White;

    DirectX::SimpleMath::Vector3 p0 = DirectX::SimpleMath::Vector3::Zero;
    DirectX::SimpleMath::Vector3 p4(1.0, 0.0, 0.0);
    DirectX::SimpleMath::Vector3 p5(0.0, 0.0, 1.0);
    DirectX::SimpleMath::Vector3 p3(0.0, 1.0, 0.0);

    VertexPositionNormalColor v0(p0, DirectX::SimpleMath::Vector3::UnitY, gridColor);
    VertexPositionNormalColor v1(p4, DirectX::SimpleMath::Vector3::UnitY, gridColor);
    VertexPositionNormalColor v2(p5, DirectX::SimpleMath::Vector3::UnitY, gridColor);
    VertexPositionNormalColor v3(p3, -DirectX::SimpleMath::Vector3::UnitX, gridColor);

    VertexPositionNormalColor v4(p5, -DirectX::SimpleMath::Vector3::UnitX, gridColor);
    VertexPositionNormalColor v5(p0, -DirectX::SimpleMath::Vector3::UnitX, gridColor);
    VertexPositionNormalColor v6(p3, -DirectX::SimpleMath::Vector3::UnitX, gridColor);

    //m_batch2->DrawTriangle(v0, v1, v2);
    //m_batch2->DrawTriangle(v5, v4, v3);

    DirectX::SimpleMath::Vector3 nw(2.0, 0.0, -2.0);
    DirectX::SimpleMath::Vector3 ne(2.0, 0.0, 2.0);
    DirectX::SimpleMath::Vector3 se(-2.0, 0.0, 2.0);
    DirectX::SimpleMath::Vector3 sw(-2.0, 0.0, -2.0);
    DirectX::XMVECTORF32 planeColor = DirectX::Colors::White;
    DirectX::SimpleMath::Vector3 planeNorm = DirectX::SimpleMath::Vector3::UnitY;

    VertexPositionNormalColor vNw(nw, planeNorm, planeColor);
    VertexPositionNormalColor vNe(ne, planeNorm, planeColor);
    VertexPositionNormalColor vSe(se, planeNorm, planeColor);
    VertexPositionNormalColor vSw(sw, planeNorm, planeColor);
    //m_batch2->DrawQuad(vNw, vNe, vSe, vSw);
}

void Game::DrawWorldCube()
{
    DirectX::XMVECTORF32 planeColor0 = DirectX::Colors::White;
    DirectX::XMVECTORF32 planeColor1 = DirectX::Colors::White;
    DirectX::XMVECTORF32 planeColor2 = DirectX::Colors::Red;
    const float halfLength = 2.0;

    DirectX::SimpleMath::Vector3 topFrontLeft(-halfLength, halfLength, -halfLength);
    DirectX::SimpleMath::Vector3 topFrontRight(-halfLength, halfLength, halfLength);
    DirectX::SimpleMath::Vector3 topBackLeft(halfLength, halfLength, -halfLength);
    DirectX::SimpleMath::Vector3 topBackRight(halfLength, halfLength, halfLength);
    DirectX::SimpleMath::Vector3 bottomFrontLeft(-halfLength, -halfLength, -halfLength);
    DirectX::SimpleMath::Vector3 bottomFrontRight(-halfLength, -halfLength, halfLength);
    DirectX::SimpleMath::Vector3 bottomBackLeft(halfLength, -halfLength, -halfLength);
    DirectX::SimpleMath::Vector3 bottomBackRight(halfLength, -halfLength, halfLength);

    DirectX::SimpleMath::Vector3 normX = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3 normY = DirectX::SimpleMath::Vector3::UnitY;
    DirectX::SimpleMath::Vector3 normZ = DirectX::SimpleMath::Vector3::UnitZ;

    DirectX::SimpleMath::Vector3 testNorm = DirectX::SimpleMath::Vector3::UnitX;
    //normX = testNorm;
    //normY = testNorm;
    //normZ = testNorm;

    // Top
    VertexPositionNormalColor vTopFrontLeft(topFrontLeft, -normY, planeColor1);
    VertexPositionNormalColor vTopFrontRight(topFrontRight, -normY, planeColor1);
    VertexPositionNormalColor vTopBackLeft(topBackLeft, -normY, planeColor1);
    VertexPositionNormalColor vTopBackRight(topBackRight, -normY, planeColor1);

    // Bottom
    VertexPositionNormalColor vBottomFrontLeft(bottomFrontLeft, normY, planeColor1);
    VertexPositionNormalColor vBottomFrontRight(bottomFrontRight, normY, planeColor1);
    VertexPositionNormalColor vBottomBackLeft(bottomBackLeft, normY, planeColor1);
    VertexPositionNormalColor vBottomBackRight(bottomBackRight, normY, planeColor1);

    // Left Side
    VertexPositionNormalColor vLeftSideFrontBottom(bottomFrontLeft, normX, planeColor1);
    VertexPositionNormalColor vLeftSideBackBottom(bottomBackLeft, normX, planeColor1);
    VertexPositionNormalColor vLeftSideBackTop(topBackLeft, normX, planeColor1);
    VertexPositionNormalColor vLeftSideFrontTop(topFrontLeft, normX, planeColor1);

    // Right Side
    VertexPositionNormalColor vRightSideFrontBottom(bottomFrontRight, -normZ, planeColor1);
    VertexPositionNormalColor vRightSideBackBottom(bottomBackRight, -normZ, planeColor1);
    VertexPositionNormalColor vRightSideBackTop(topBackRight, -normZ, planeColor1);
    VertexPositionNormalColor vRightSideFrontTop(topFrontRight, -normZ, planeColor1);

    // Back
    VertexPositionNormalColor vBackLeftBottom(bottomBackLeft, -normZ, planeColor1);
    VertexPositionNormalColor vBackRightBottom(bottomBackRight, -normZ, planeColor1);
    VertexPositionNormalColor vBackRightTop(topBackRight, -normZ, planeColor1);
    VertexPositionNormalColor vBackLeftTop(topBackLeft, -normZ, planeColor1);

    // Front
    VertexPositionNormalColor vFrontLeftBottom(bottomFrontLeft, normX, planeColor1);
    VertexPositionNormalColor vFrontRightBottom(bottomFrontRight, normX, planeColor1);
    VertexPositionNormalColor vFrontRightTop(topFrontRight, normX, planeColor1);
    VertexPositionNormalColor vFrontLeftTop(topFrontLeft, normX, planeColor1);

    m_batch2->DrawQuad(vTopFrontLeft, vTopFrontRight, vTopBackRight, vTopBackLeft);
    m_batch2->DrawQuad(vBottomFrontLeft, vBottomFrontRight, vBottomBackRight, vBottomBackLeft);
    m_batch2->DrawQuad(vLeftSideFrontBottom, vLeftSideBackBottom, vLeftSideBackTop, vLeftSideFrontTop);
    m_batch2->DrawQuad(vRightSideFrontBottom, vRightSideBackBottom, vRightSideBackTop, vRightSideFrontTop);
    m_batch2->DrawQuad(vBackLeftBottom, vBackRightBottom, vBackRightTop, vBackLeftTop);
    m_batch2->DrawQuad(vFrontLeftBottom, vFrontRightBottom, vFrontRightTop, vFrontLeftTop);
}

void Game::DrawWorldCubeTextured()
{
    DirectX::XMVECTORF32 planeColor0 = DirectX::Colors::White;
    DirectX::XMVECTORF32 planeColor1 = DirectX::Colors::White;
    DirectX::XMVECTORF32 planeColor2 = DirectX::Colors::Red;
    const float halfLength = 2.0;

    DirectX::SimpleMath::Vector3 topFrontLeft(-halfLength, halfLength, -halfLength);
    DirectX::SimpleMath::Vector3 topFrontRight(-halfLength, halfLength, halfLength);
    DirectX::SimpleMath::Vector3 topBackLeft(halfLength, halfLength, -halfLength);
    DirectX::SimpleMath::Vector3 topBackRight(halfLength, halfLength, halfLength);
    DirectX::SimpleMath::Vector3 bottomFrontLeft(-halfLength, -halfLength, -halfLength);
    DirectX::SimpleMath::Vector3 bottomFrontRight(-halfLength, -halfLength, halfLength);
    DirectX::SimpleMath::Vector3 bottomBackLeft(halfLength, -halfLength, -halfLength);
    DirectX::SimpleMath::Vector3 bottomBackRight(halfLength, -halfLength, halfLength);

    DirectX::SimpleMath::Vector3 normX = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3 normY = DirectX::SimpleMath::Vector3::UnitY;
    DirectX::SimpleMath::Vector3 normZ = DirectX::SimpleMath::Vector3::UnitZ;

    DirectX::SimpleMath::Vector3 testNorm = DirectX::SimpleMath::Vector3::UnitY;
    normX = testNorm;
    normY = testNorm;
    normZ = testNorm;

    // Top
    VertexPositionNormalColorTexture vTopFrontLeft(topFrontLeft, -normY, planeColor1, DirectX::SimpleMath::Vector2(0, 0));
    VertexPositionNormalColorTexture vTopFrontRight(topFrontRight, -normY, planeColor1, DirectX::SimpleMath::Vector2(1, 0));
    VertexPositionNormalColorTexture vTopBackLeft(topBackLeft, -normY, planeColor1, DirectX::SimpleMath::Vector2(0, 1));
    VertexPositionNormalColorTexture vTopBackRight(topBackRight, -normY, planeColor1, DirectX::SimpleMath::Vector2(1, 1));

    // Bottom
    VertexPositionNormalColorTexture vBottomFrontLeft(bottomFrontLeft, -normY, planeColor1, DirectX::SimpleMath::Vector2(0, 1));
    VertexPositionNormalColorTexture vBottomFrontRight(bottomFrontRight, -normY, planeColor1, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalColorTexture vBottomBackLeft(bottomBackLeft, -normY, planeColor1, DirectX::SimpleMath::Vector2(0, 0));
    VertexPositionNormalColorTexture vBottomBackRight(bottomBackRight, -normY, planeColor1, DirectX::SimpleMath::Vector2(1, 0));

    // Left Side
    VertexPositionNormalColorTexture vLeftSideFrontBottom(bottomFrontLeft, -normZ, planeColor1, DirectX::SimpleMath::Vector2(0, 1));
    VertexPositionNormalColorTexture vLeftSideBackBottom(bottomBackLeft, -normZ, planeColor1, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalColorTexture vLeftSideBackTop(topBackLeft, -normZ, planeColor1, DirectX::SimpleMath::Vector2(1, 0));
    VertexPositionNormalColorTexture vLeftSideFrontTop(topFrontLeft, -normZ, planeColor1, DirectX::SimpleMath::Vector2(0, 0));

    // Right Side
    VertexPositionNormalColorTexture vRightSideFrontBottom(bottomFrontRight, -normZ, planeColor1, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalColorTexture vRightSideBackBottom(bottomBackRight, -normZ, planeColor1, DirectX::SimpleMath::Vector2(0, 1));
    VertexPositionNormalColorTexture vRightSideBackTop(topBackRight, -normZ, planeColor1, DirectX::SimpleMath::Vector2(0, 0));
    VertexPositionNormalColorTexture vRightSideFrontTop(topFrontRight, -normZ, planeColor1, DirectX::SimpleMath::Vector2(1, 0));

    // Back
    VertexPositionNormalColorTexture vBackLeftBottom(bottomBackLeft, -normX, planeColor1, DirectX::SimpleMath::Vector2(0, 1));
    VertexPositionNormalColorTexture vBackRightBottom(bottomBackRight, -normX, planeColor1, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalColorTexture vBackRightTop(topBackRight, -normX, planeColor1, DirectX::SimpleMath::Vector2(1, 0));
    VertexPositionNormalColorTexture vBackLeftTop(topBackLeft, -normX, planeColor1, DirectX::SimpleMath::Vector2(0, 0));

    // Front
    VertexPositionNormalColorTexture vFrontLeftBottom(bottomFrontLeft, -normX, planeColor1, DirectX::SimpleMath::Vector2(1, 1));
    VertexPositionNormalColorTexture vFrontRightBottom(bottomFrontRight, -normX, planeColor1, DirectX::SimpleMath::Vector2(0, 1));
    VertexPositionNormalColorTexture vFrontRightTop(topFrontRight, -normX, planeColor1, DirectX::SimpleMath::Vector2(0, 0));
    VertexPositionNormalColorTexture vFrontLeftTop(topFrontLeft, -normX, planeColor1, DirectX::SimpleMath::Vector2(1, 0));

    m_batch->DrawQuad(vTopFrontLeft, vTopFrontRight, vTopBackRight, vTopBackLeft);
    m_batch->DrawQuad(vBottomFrontLeft, vBottomFrontRight, vBottomBackRight, vBottomBackLeft);
    m_batch->DrawQuad(vLeftSideFrontBottom, vLeftSideBackBottom, vLeftSideBackTop, vLeftSideFrontTop);
    m_batch->DrawQuad(vRightSideFrontBottom, vRightSideBackBottom, vRightSideBackTop, vRightSideFrontTop);
    m_batch->DrawQuad(vBackLeftBottom, vBackRightBottom, vBackRightTop, vBackLeftTop);
    m_batch->DrawQuad(vFrontLeftBottom, vFrontRightBottom, vFrontRightTop, vFrontLeftTop);
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1600;
    height = 900;
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    // WLJ add for mouse and keybord interface
    m_keyboard = std::make_unique<DirectX::Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    // Audio
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef DEBUG  //#ifdef _DEBUG
    eflags |= AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);
    m_retryAudio = false;
    m_audioBank = std::make_unique<WaveBank>(m_audioEngine.get(), L"audioBank.xwb");

    // height map intit
    bool result;
    bool isInitSuccessTrue = true;
    result = InitializeTerrainArray();
    if (!result)
    {
        isInitSuccessTrue = false;
    }
    result = InitializeTerrainArray2();
    if (!result)
    {
        isInitSuccessTrue = false;
    }
    if (!isInitSuccessTrue)
    {
        // add initialization failure testing  here;
        int errorBreak = 0;
        errorBreak++;
    }

    // testing new terrain map
    m_terrainVector.clear();
}

// Testing Terrain Vertex
bool Game::InitializeTerrainArray()
{
    //std::vector<DirectX::VertexPositionColor> vertexPC = pGolf->GetTerrainVertex();
    std::vector<DirectX::VertexPositionColor> vertexPC = m_environment->GetTerrainColorVertex();

    m_terrainVertexCount = vertexPC.size();
    m_terrainVertexArray = new DirectX::VertexPositionColor[m_terrainVertexCount];
    m_terrainVertexArrayBase = new DirectX::VertexPositionColor[m_terrainVertexCount];

    //lawngreen = { { { 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f } } };
    //DirectX::XMFLOAT4 lineColor(0.0, 0.501960814f, 0.0, 1.0);
    DirectX::XMFLOAT4 lineColor(.486274540f, .988235354f, 0.0, 1.0);
    DirectX::XMFLOAT4 baseColor(0.0, 0.0, 0.0, 1.0);
    //DirectX::XMFLOAT4 baseColor2(0.3, 0.3, 0.3, 1.0);
    //DirectX::XMFLOAT4 baseColor2(1.0, 1.0, 1.0, 1.0);
    DirectX::XMFLOAT4 baseColor2(0.0, 0.0, 0.0, 1.0);

    DirectX::XMFLOAT4 sandColor1(0.956862807f, 0.643137276f, 0.376470625f, 1.0);
    DirectX::XMFLOAT4 sandColor2(0.960784376f, 0.960784376f, 0.862745166f, 1.0);
    DirectX::XMFLOAT4 greenColor1 = DirectX::XMFLOAT4(0.0, 0.501960814f, 0.0, 1.0);
    DirectX::XMFLOAT4 greenColor2 = DirectX::XMFLOAT4(0.486274540f, 0.988235354f, 0.0, 1.0);

    DirectX::XMFLOAT4 grassColor1 = DirectX::XMFLOAT4(0.133333340f, 0.545098066f, 0.133333340f, 1.0);
    DirectX::XMFLOAT4 grassColor2 = DirectX::XMFLOAT4(0.000000000f, 0.392156899f, 0.0, 1.0);

    DirectX::Colors::ForestGreen;
    DirectX::Colors::DarkGreen;
    //baseColor = DirectX::XMFLOAT4(0.0, 0.501960814f, 0.0, 1.0);
    //baseColor2 = DirectX::XMFLOAT4(0.486274540f, 0.988235354f, 0.0, 1.0);
    //XMGLOBALCONST XMVECTORF32 SandyBrown = { { { 0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 Beige = { { { 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 Green = { { { 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 LawnGreen = { { { 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 ForestGreen          = { { { 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 DarkGreen            = { { { 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f } } };
    for (int i = 0; i < m_terrainVertexCount; ++i)
    {
        m_terrainVertexArray[i].position = vertexPC[i].position;
        m_terrainVertexArray[i].position.y += 0.0005; // Move up the lines so they don't get hidden by terrain
        m_terrainVertexArray[i].color = lineColor;
        m_terrainVertexArrayBase[i].position = vertexPC[i].position;

        if (i % 2 == 0)
        {
            m_terrainVertexArrayBase[i].color = baseColor;
        }
        else
        {
            m_terrainVertexArrayBase[i].color = baseColor2;
        }
    }

    return true;
}

bool Game::InitializeTerrainArray2()
{
    std::vector<DirectX::VertexPositionNormalColor> vertexPC = m_environment->GetTerrainPositionNormalColorVertex();

    m_terrainVertexCount2 = vertexPC.size();
    m_terrainVertexArray2 = new DirectX::VertexPositionNormalColor[m_terrainVertexCount2];
    m_terrainVertexArrayBase2 = new DirectX::VertexPositionNormalColor[m_terrainVertexCount2];

    //lawngreen = { { { 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f } } };
    //DirectX::XMFLOAT4 lineColor(0.0, 0.501960814f, 0.0, 1.0);
    DirectX::XMFLOAT4 lineColor(.486274540f, .988235354f, 0.0, 1.0);
    DirectX::XMFLOAT4 baseColor(0.0, 0.0, 0.0, 1.0);
    //DirectX::XMFLOAT4 baseColor2(0.3, 0.3, 0.3, 1.0);
    DirectX::XMFLOAT4 baseColor2(1.0, 1.0, 1.0, 1.0);
    //DirectX::XMFLOAT4 baseColor2(0.0, 0.0, 0.0, 1.0);

    DirectX::XMFLOAT4 sandColor1(0.956862807f, 0.643137276f, 0.376470625f, 1.0);
    DirectX::XMFLOAT4 sandColor2(0.960784376f, 0.960784376f, 0.862745166f, 1.0);
    DirectX::XMFLOAT4 greenColor1 = DirectX::XMFLOAT4(0.0, 0.501960814f, 0.0, 1.0);
    DirectX::XMFLOAT4 greenColor2 = DirectX::XMFLOAT4(0.486274540f, 0.988235354f, 0.0, 1.0);

    DirectX::XMFLOAT4 grassColor1 = DirectX::XMFLOAT4(0.133333340f, 0.545098066f, 0.133333340f, 1.0);
    DirectX::XMFLOAT4 grassColor2 = DirectX::XMFLOAT4(0.000000000f, 0.392156899f, 0.0, 1.0);
    DirectX::XMFLOAT4 testRed = DirectX::XMFLOAT4(1.000000000f, 0.000000000f, 0.0, 1.0);
    DirectX::XMFLOAT4 testBlue = DirectX::XMFLOAT4(0.000000000f, 0.000000000f, 1.0, 1.0);
    DirectX::XMFLOAT4 testGray = DirectX::XMFLOAT4(0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f);

    DirectX::Colors::DarkGray;
    //baseColor = DirectX::XMFLOAT4(0.0, 0.501960814f, 0.0, 1.0);
    //baseColor2 = DirectX::XMFLOAT4(0.486274540f, 0.988235354f, 0.0, 1.0);
    //XMGLOBALCONST XMVECTORF32 SandyBrown = { { { 0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 Beige = { { { 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 Green = { { { 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 LawnGreen = { { { 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 ForestGreen          = { { { 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f } } };
    //XMGLOBALCONST XMVECTORF32 DarkGreen            = { { { 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f } } };
    for (int i = 0; i < m_terrainVertexCount2; ++i)
    {
        m_terrainVertexArray2[i].position = vertexPC[i].position;
        m_terrainVertexArray2[i].normal = vertexPC[i].normal;
        m_terrainVertexArray2[i].color = lineColor;
        
        m_terrainVertexArrayBase2[i].position = vertexPC[i].position;
        m_terrainVertexArrayBase2[i].normal = vertexPC[i].normal;
        if (i % 2 == 0)
        {
            m_terrainVertexArrayBase2[i].color = baseColor;
        }
        else
        {
            m_terrainVertexArrayBase2[i].color = baseColor;
        }

        if (i == i)
        //if (i < 96)
        //if (i % 96 == 0)
        {
            if ((i + 5) % 6 == 0)
            {
                m_terrainVertexArrayBase2[i].color = baseColor;
            }
            if ((i + 2) % 6 == 0)
            {
                m_terrainVertexArrayBase2[i].color = testGray;
            }
            if (i % 6 == 0)
            {
                m_terrainVertexArrayBase2[i].color = testGray;
            }
        }
    }

    return true;
}

void Game::InitializeWorldGrid()
{
    m_worldGrid.clear();

    const DirectX::SimpleMath::Vector3 gridNorm = DirectX::SimpleMath::Vector3::UnitY;
    const int hightLightIncrement = 10;
    const float maxX = 2.0;
    const float maxY = 1.0;
    const float maxZ = 2.0;
    const float minX = -2.0;
    const float minY = -1.0;
    const float minZ = -2.0;
    const float increment = 0.1;
    DirectX::SimpleMath::Vector3 p1 = DirectX::SimpleMath::Vector3::Zero;
    DirectX::SimpleMath::Vector3 p2 = DirectX::SimpleMath::Vector3::Zero;

    DirectX::XMVECTORF32 gridColor0 = DirectX::Colors::Red;
    DirectX::XMVECTORF32 gridColor1 = DirectX::Colors::LawnGreen;
    DirectX::XMVECTORF32 gridColor2 = DirectX::Colors::White;
    DirectX::XMVECTORF32 gridColor3 = DirectX::Colors::Gray;
    DirectX::XMVECTORF32 gridColor4 = DirectX::Colors::LightGray;
    DirectX::XMVECTORF32 gridColor5 = DirectX::Colors::Yellow;

    // positive z draws
    p1 = DirectX::SimpleMath::Vector3::Zero;
    p2 = DirectX::SimpleMath::Vector3::Zero;
    p1.x = minX;
    p2.x = maxX;
    VertexPositionNormalColor v1(p1, gridNorm, gridColor0);
    VertexPositionNormalColor v2(p2, gridNorm, gridColor0);
    m_worldGrid.push_back(std::pair(v1, v2));
    int i = 0;
    while (p1.z <= maxZ)
    {
        ++i;
        if (i % hightLightIncrement == 0)
        {
            v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x, p1.y, p1.z + increment), gridNorm, gridColor2);
            v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x, p2.y, p2.z + increment), gridNorm, gridColor2);
        }
        else
        {
            v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x, p1.y, p1.z + increment), gridNorm, gridColor1);
            v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x, p2.y, p2.z + increment), gridNorm, gridColor1);
        }

        m_worldGrid.push_back(std::pair(v1, v2));
        p1.z += increment;
        p2.z += increment;
    }

    // negative z draws
    p1.z = 0.0;
    p2.z = 0.0;
    p1.y = 0.0;
    p1.y = 0.0;
    i = 0;
    while (p1.z >= minZ)
    {
        ++i;
        if (i % hightLightIncrement == 0)
        {
            v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x, p1.y, p1.z - increment), gridNorm, gridColor2);
            v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x, p2.y, p2.z - increment), gridNorm, gridColor2);
        }
        else
        {
            v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x, p1.y, p1.z - increment), gridNorm, gridColor1);
            v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x, p2.y, p2.z - increment), gridNorm, gridColor1);
        }
        m_worldGrid.push_back(std::pair(v1, v2));
        p1.z -= increment;
        p2.z -= increment;
    }

    // positve x draws
    p1 = DirectX::SimpleMath::Vector3::Zero;
    p1.z = minZ;
    p2 = DirectX::SimpleMath::Vector3::Zero;
    p2.z = maxZ;
    v1 = DirectX::VertexPositionNormalColor(p1, gridNorm, gridColor0);
    v2 = DirectX::VertexPositionNormalColor(p2, gridNorm, gridColor0);
    m_worldGrid.push_back(std::pair(v1, v2));
    i = 0;
    while (p1.x <= maxX)
    {
        ++i;
        if (i % hightLightIncrement == 0)
        {
            v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x + increment, p1.y, p1.z), gridNorm, gridColor2);
            v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x + increment, p2.y, p2.z), gridNorm, gridColor2);
        }
        else
        {
            v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x + increment, p1.y, p1.z), gridNorm, gridColor1);
            v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x + increment, p2.y, p2.z), gridNorm, gridColor1);
        }

        m_worldGrid.push_back(std::pair(v1, v2));
        p1.x += increment;
        p2.x += increment;
    }

    // negative x draws
    p1.x = 0.0;
    p2.x = 0.0;
    p1.y = 0.0;
    p1.y = 0.0;
    i = 0;
    while (p1.x >= minX)
    {
        ++i;
        if (i % hightLightIncrement == 0)
        {
            v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x - increment, p1.y, p1.z), gridNorm, gridColor2);
            v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x - increment, p2.y, p2.z), gridNorm, gridColor2);
        }
        else
        {
            v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x - increment, p1.y, p1.z), gridNorm, gridColor1);
            v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x - increment, p2.y, p2.z), gridNorm, gridColor1);
        }
        m_worldGrid.push_back(std::pair(v1, v2));
        p1.x -= increment;
        p2.x -= increment;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // positve y draws
    if (1 == 1) // Toggle y draw on and off
    {
        p1 = DirectX::SimpleMath::Vector3::Zero;
        p1.x = minX;
        p2 = DirectX::SimpleMath::Vector3::Zero;
        p2.x = maxX;
        v1 = DirectX::VertexPositionNormalColor(p1, gridNorm, gridColor0);
        v2 = DirectX::VertexPositionNormalColor(p2, gridNorm, gridColor0);
        //m_worldGrid.push_back(std::pair(v1, v2));
        i = 0;
        while (p1.y <= maxY)
        {
            ++i;
            if (i % hightLightIncrement == 0)
            {
                v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x, p1.y + increment, p1.z), gridNorm, gridColor2);
                v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x, p2.y + increment, p2.z), gridNorm, gridColor2);
            }
            else
            {
                v1 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p1.x, p1.y + increment, p1.z), gridNorm, gridColor1);
                v2 = VertexPositionNormalColor(DirectX::SimpleMath::Vector3(p2.x, p2.y + increment, p2.z), gridNorm, gridColor1);
            }

            m_worldGrid.push_back(std::pair(v1, v2));
            p1.y += increment;
            p2.y += increment;
        }
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
    m_kbStateTracker.Reset();
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.

}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_raster.Reset(); // anti-aliased lines
    m_states.reset();
    m_effect.reset();
    m_effect2.reset();
    m_effect3.reset();
    m_batch.reset();
    m_batch2.reset();
    m_batch3.reset();
    m_shape.reset();
    m_normalMap.Reset();
    m_texture.Reset();
    m_specular.Reset();

    m_normalMapTest.Reset();
    m_textureTest.Reset();
    m_specularTest.Reset();

    m_normalMapJI.Reset();
    m_specularJI.Reset();
    m_textureJI.Reset();
    m_normalMapBMW.Reset();
    m_normalMapBMW2.Reset();
    m_specularBMW.Reset();
    m_textureBMW.Reset();
    m_normalMapAutoGame.Reset();
    m_specularAutoGame.Reset();
    m_textureAutoGame.Reset();
    m_normalMapTeaser.Reset();
    m_specularTeaser.Reset();
    m_textureTeaser.Reset();

    m_inputLayout.Reset();
    m_font.reset();
    m_titleFont.reset();
    m_bitwiseFont.reset();
    m_spriteBatch.reset();
    m_kbStateTracker.Reset();

    // Charcters
    m_pacSprite.reset();
    m_pacTexture.Reset();

    m_character.reset();
    m_characterTexture.Reset();
    m_character0.reset();
    m_character0Texture.Reset();
    m_character1.reset();
    m_character1Texture.Reset();
    m_character2.reset();
    m_character2Texture.Reset();
    m_characterBackgroundTexture.Reset();

    // Environment Select
    m_environSelectCalmTexture.Reset();
    m_environSelectBreezyTexture.Reset();
    m_environSelectAlienTexture.Reset();

    // Intro Screen
    m_bmwLogoTexture.Reset();
    m_jiLogoTexture.Reset();

    m_road.reset();
    m_backgroundTex.Reset();

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();

    CreateDevice();
    CreateResources();
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
    m_audioEngine->Suspend();
}

void Game::OnResuming()
{
    // TODO: Game is being power-resumed (or returning from minimize).
    m_timer.ResetElapsedTime();
    m_kbStateTracker.Reset();
    m_audioEngine->Resume();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
    m_camera->OnResize(m_outputWidth, m_outputHeight);
    m_proj = m_camera->GetProjectionMatrix();
    m_effect->SetProjection(m_proj);
    m_effect2->SetProjection(m_proj);
    m_effect3->SetProjection(m_proj);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

void Game::SetLighting(LightingState aLightState)
{
    m_currentLightingState = aLightState;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();
     
    UpdateLighting();

    // TODO: Add your rendering code here.
    // WLJ start
    m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    //m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0); // doesnt facecull
    m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
    //m_d3dContext->RSSetState(m_states->CullNone());
    //m_d3dContext->RSSetState(m_states->CullClockwise());
    //10  m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
    //11  m_d3dContext->RSSetState(m_states->CullCounterClockwise());

    //world start
    m_d3dContext->RSSetState(m_raster.Get()); // WLJ anti-aliasing  RenderTesting
    //m_d3dContext->RSSetState(m_states->CullNone());

    void const* shaderByteCode;
    size_t byteCodeLength;
    m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
    DX::ThrowIfFailed(m_d3dDevice->CreateInputLayout(VertexType::InputElements, VertexType::InputElementCount, shaderByteCode, byteCodeLength, m_inputLayout.ReleaseAndGetAddressOf()));
    m_batch = std::make_unique<PrimitiveBatch<VertexType>>(m_d3dContext.Get());
    m_effect->SetWorld(m_world);
    //world end
    
    m_effect->Apply(m_d3dContext.Get());
  
    auto sampler = m_states->LinearClamp();
    m_d3dContext->PSSetSamplers(0, 1, &sampler);

    m_d3dContext->IASetInputLayout(m_inputLayout.Get());

    m_batch->Begin();

    //DrawDebugLines();
    if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
    {
        DrawIntroScene();
        //DrawStartScreen();
        //DrawShape();
        //DrawCar();
        //DrawWorldCubeTextured();
        if (m_camera->GetCameraState() == CameraState::CAMERASTATE_SWINGVIEW || m_camera->GetCameraState() == CameraState::CAMERASTATE_PROJECTILEFLIGHTVIEW)
        {

        }
        if (m_camera->GetCameraState() == CameraState::CAMERASTATE_PRESWINGVIEW || m_camera->GetCameraState() == CameraState::CAMERASTATE_PROJECTILEFLIGHTVIEW || m_camera->GetCameraState() == CameraState::CAMERASTATE_FIRSTPERSON)
        {
            m_flightStepTimer.ResetElapsedTime();
        }
        if (m_isInDebugMode == true)
        {
            //DrawCameraFocus();
            //DrawDebugLines();
        }
    }
    m_batch->End();
    
    void const* shaderByteCode2;
    size_t byteCodeLength2;
    m_effect2->GetVertexShaderBytecode(&shaderByteCode2, &byteCodeLength2);
    DX::ThrowIfFailed(m_d3dDevice->CreateInputLayout(VertexType2::InputElements, VertexType2::InputElementCount, shaderByteCode2, byteCodeLength2, m_inputLayout.ReleaseAndGetAddressOf()));
    m_batch2 = std::make_unique<PrimitiveBatch<VertexType2>>(m_d3dContext.Get());
    
    //m_effect2->SetWorld(m_world);
    m_effect2->Apply(m_d3dContext.Get());

    //m_d3dContext->PSSetSamplers(0, 1, &sampler);
    m_d3dContext->IASetInputLayout(m_inputLayout.Get());
    m_batch2->Begin();
    if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
    {

        //DrawLightBar();
        //DrawCameraFocus();
        //DrawLightFocus1();
        //DrawLightFocus2();
        //DrawLightFocus3();
        //DrawWorld();
        //DrawWorldCube();
    }
    if (m_currentGameState == GameState::GAMESTATE_STARTSCREEN)
    {
        DrawTerrain2();
    }
    m_batch2->End();

    void const* shaderByteCode3;
    size_t byteCodeLength3;
    m_effect3->GetVertexShaderBytecode(&shaderByteCode3, &byteCodeLength3);
    DX::ThrowIfFailed(m_d3dDevice->CreateInputLayout(VertexType3::InputElements, VertexType3::InputElementCount, shaderByteCode3, byteCodeLength3, m_inputLayout.ReleaseAndGetAddressOf()));
    m_batch3 = std::make_unique<PrimitiveBatch<VertexType3>>(m_d3dContext.Get());
    m_d3dContext->IASetInputLayout(m_inputLayout.Get());

    m_effect3->Apply(m_d3dContext.Get());

    m_batch3->Begin();
    if (m_currentGameState == GameState::GAMESTATE_STARTSCREEN)
    {
        DrawGridForStartScreen();
        //DrawTerrain();
    }
    m_batch3->End();

    m_spriteBatch->Begin();
    DrawDebugValue();
    //DrawTimer();
    if (m_currentGameState == GameState::GAMESTATE_INTROSCREEN)
    {
        DrawIntroScreen();
    }
    if (m_currentGameState == GameState::GAMESTATE_STARTSCREEN)
    {
        DrawStartScreenOld();
    }
    if (m_currentGameState == GameState::GAMESTATE_MAINMENU)
    {
        DrawMenuMain();
    }
    if (m_currentGameState == GameState::GAMESTATE_CHARACTERSELECT)
    {

    }
    if (m_currentGameState == GameState::GAMESTATE_ENVIRONTMENTSELECT)
    {
        DrawMenuEnvironmentSelect();
    }
    if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
    {
        //DrawUI();
    }
    if (m_currentGameState == GameState::GAMESTATE_TEASERSCREEN)
    {
        //DrawTeaserScreen();
    }

    m_spriteBatch->End();

    Present();
}

void Game::ResetGamePlay()
{
    //pAuto->ZeroUIandRenderData();
    m_projectileTimer = 0;
    m_swingPathStep = 0;
    m_projectilePathStep = 0;
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    m_flightStepTimer.Tick([&]()
        {
        });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& aTimer)
{
    double elapsedTime = double(aTimer.GetElapsedSeconds());
    m_projectileTimer += elapsedTime;
    // TODO: Add your game logic here.
    m_road->Update(elapsedTime * 500);
    m_pacSprite->Update(elapsedTime);

    if (m_currentGameState == GameState::GAMESTATE_CHARACTERSELECT)
    {
        if (m_menuSelect == 0)
        {
            m_character0->Update(static_cast<float>(elapsedTime));
        }
        if (m_menuSelect == 1)
        {
            m_character1->Update(static_cast<float>(elapsedTime));
        }
        if (m_menuSelect == 2)
        {
            m_character2->Update(static_cast<float>(elapsedTime));
        }
    }

    if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
    {

    }

    // audio
    if (m_retryAudio)
    {
        m_retryAudio = false;
        if (m_audioEngine->Reset())
        {
            // ToDo: restart any looped sounds here
            if (m_audioMusicStream)
            {
                m_audioMusicStream->Play(true);
            }
            if (m_audioEffectStream)
            {
                m_audioEffectStream->Play(); // WLJ this could lead to problems and might not be needed, maybe cause unwanted effect to play after reset?
            }
        }
    }
    else if (!m_audioEngine->Update())
    {
        if (m_audioEngine->IsCriticalError())
        {
            m_retryAudio = true;
        }
    }

    m_camera->UpdateCamera(aTimer);
    DirectX::SimpleMath::Matrix viewMatrix = m_camera->GetViewMatrix();
    m_effect->SetView(viewMatrix);
    m_effect2->SetView(viewMatrix);
    m_effect3->SetView(viewMatrix);
    UpdateInput(aTimer);
}

void Game::UpdateInput(DX::StepTimer const& aTimer)
{
    // WLJ add for mouse and keybord interface   
    auto kb = m_keyboard->GetState();
    m_kbStateTracker.Update(kb);

    if (kb.Escape)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_camera->SetCameraState(CameraState::CAMERASTATE_PRESWINGVIEW);
            ResetGamePlay();
        }
        m_currentGameState = GameState::GAMESTATE_MAINMENU;
    }
    if (m_kbStateTracker.pressed.Enter)
    {
        if (m_currentGameState == GameState::GAMESTATE_ENVIRONTMENTSELECT)
        {
            if (m_menuSelect == 0)
            {

            }
            if (m_menuSelect == 1)
            {

            }
            if (m_menuSelect == 2)
            {

            }
            m_menuSelect = 0;
            m_currentGameState = GameState::GAMESTATE_STARTSCREEN;
        }
        if (m_currentGameState == GameState::GAMESTATE_CHARACTERSELECT)
        {
            if (m_menuSelect == 0)
            {

            }
            if (m_menuSelect == 1)
            {

            }
            if (m_menuSelect == 2)
            {

            }
            m_menuSelect = 0;
            //m_currentGameState = GameState::GAMESTATE_MAINMENU; // Return to Main Menu after selecting character, ToDo: using value of 1 doesn't return to main menu
            m_currentGameState = GameState::GAMESTATE_STARTSCREEN;// Return to Main Menu after selecting character, ToDo: using value of 1 doesn't return to main menu
        }
        if (m_currentGameState == GameState::GAMESTATE_MAINMENU)
        {
            if (m_menuSelect == 0) // GoTo Game State
            {
                m_currentGameState = GameState::GAMESTATE_GAMEPLAY;
            }
            if (m_menuSelect == 1) // GoTo Character Select State
            {
                m_currentGameState = GameState::GAMESTATE_CHARACTERSELECT;
            }
            if (m_menuSelect == 2) // GoTo Environment Select State
            {
                m_currentGameState = GameState::GAMESTATE_ENVIRONTMENTSELECT;
            }
            if (m_menuSelect == 3) // GoTo Demo Select State
            {
                m_currentGameState = GameState::GAMESTATE_GAMEPLAY;
            }
            if (m_menuSelect == 4) // Quit Game
            {
                ExitGame();
            }
            m_menuSelect = 0;
        }
        if (m_currentGameState == GameState::GAMESTATE_STARTSCREEN)
        {
            m_flightStepTimer.ResetElapsedTime(); // piggy backing on this variable for the moment
            m_currentGameState = GameState::GAMESTATE_TEASERSCREEN;
            //m_currentGameState = GameState::GAMESTATE_MAINMENU;
        }
    }
    if (m_kbStateTracker.pressed.Up)
    {
        if (m_currentGameState == GameState::GAMESTATE_MAINMENU)
        {
            --m_menuSelect;
        }
        if (m_currentGameState == GameState::GAMESTATE_CHARACTERSELECT)
        {
            --m_menuSelect;
        }
        if (m_currentGameState == GameState::GAMESTATE_ENVIRONTMENTSELECT)
        {
            --m_menuSelect;
        }
    }
    if (m_kbStateTracker.pressed.Down)
    {
        if (m_currentGameState == GameState::GAMESTATE_MAINMENU)
        {
            ++m_menuSelect;
        }
        if (m_currentGameState == GameState::GAMESTATE_CHARACTERSELECT)
        {
            ++m_menuSelect;
        }
        if (m_currentGameState == GameState::GAMESTATE_ENVIRONTMENTSELECT)
        {
            ++m_menuSelect;
        }
    }
    if (m_kbStateTracker.pressed.Left)
    {
        if (m_currentGameState == GameState::GAMESTATE_CHARACTERSELECT)
        {
            --m_menuSelect;
        }
        if (m_currentGameState == GameState::GAMESTATE_ENVIRONTMENTSELECT)
        {
            --m_menuSelect;
        }
    }
    if (m_kbStateTracker.pressed.Right)
    {
        if (m_currentGameState == GameState::GAMESTATE_CHARACTERSELECT)
        {
            ++m_menuSelect;
        }
        if (m_currentGameState == GameState::GAMESTATE_ENVIRONTMENTSELECT)
        {
            ++m_menuSelect;
        }
    }
    if (m_kbStateTracker.pressed.R)
    {
        m_timer.ResetElapsedTime();
    }
    if (kb.D)
    {
        m_camera->UpdatePos(0.0f + static_cast<float>(aTimer.GetElapsedSeconds()), 0.0f, 0.0f);
    }
    if (kb.S)
    {
        m_camera->UpdatePos(0.0f, 0.0f, 0.0f - static_cast<float>(aTimer.GetElapsedSeconds()));
    }
    if (kb.A)
    {
        m_camera->UpdatePos(0.0f - static_cast<float>(aTimer.GetElapsedSeconds()), 0.0f, 0.0f);
    }
    if (kb.W)
    {
        m_camera->UpdatePos(0.0f, 0.0f, 0.0f + static_cast<float>(aTimer.GetElapsedSeconds()));
    }
    if (kb.Q)
    {
        m_camera->UpdatePitchYaw(0.0f, 0.0f + static_cast<float>(aTimer.GetElapsedSeconds()));
    }
    if (kb.E)
    {
        m_camera->UpdatePitchYaw(0.0f, 0.0f - static_cast<float>(aTimer.GetElapsedSeconds()));
    }
    if (kb.F)
    {
        m_camera->UpdatePos(0.0f, 0.0f + static_cast<float>(aTimer.GetElapsedSeconds()), 0.0f);
    }
    if (kb.C)
    {
        m_camera->UpdatePos(0.0f, 0.0f - static_cast<float>(aTimer.GetElapsedSeconds()), 0.0f);
    }
    if (m_kbStateTracker.pressed.D1)
    {
        if (m_moveLightWithCameraFocus1 == true)
        {
            m_moveLightWithCameraFocus1 = false;
        }
        else
        {
            m_moveLightWithCameraFocus1 = true;
        }
    }
    if (m_kbStateTracker.pressed.D2)
    {
        if (m_moveLightWithCameraFocus2 == true)
        {
            m_moveLightWithCameraFocus2 = false;
        }
        else
        {
            m_moveLightWithCameraFocus2 = true;
        }
    }
    if (m_kbStateTracker.pressed.D3)
    {
        if (m_moveLightWithCameraFocus3 == true)
        {
            m_moveLightWithCameraFocus3 = false;
        }
        else
        {
            m_moveLightWithCameraFocus3 = true;
        }
    }
    if (m_kbStateTracker.pressed.T)
    {
        SetLighting(LightingState::LIGHTINGSTATE_MANUAL);
    }
    if (m_kbStateTracker.pressed.Y)
    {
        m_camera->SetCameraState(CameraState::CAMERASTATE_FIRSTPERSON);
    }
    if (m_kbStateTracker.pressed.F1)
    {
        m_camera->SetCameraState(CameraState::CAMERASTATE_SWINGVIEW);
    }
    if (kb.NumPad8)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_carPos.x += static_cast<float>(aTimer.GetElapsedSeconds()) * 0.1f;
            m_lightPos1.x += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.NumPad2)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_carPos.x -= static_cast<float>(aTimer.GetElapsedSeconds()) * 0.1f;
            m_lightPos1.x -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.NumPad6)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_carPos.z += static_cast<float>(aTimer.GetElapsedSeconds()) * 0.1f;
            m_lightPos1.z += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.NumPad4)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_carPos.z -= static_cast<float>(aTimer.GetElapsedSeconds()) * 0.1f;
            m_lightPos1.z -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.NumPad7)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_carAim += static_cast<float>(aTimer.GetElapsedSeconds()) * 0.11f;
            m_lightPos1.y -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.NumPad9)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_carAim -= static_cast<float>(aTimer.GetElapsedSeconds()) * 0.11f;
            m_lightPos1.y += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.Up)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos0.x += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.Down)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos0.x -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.Left)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos0.z -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.Right)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos0.z += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.NumPad1)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos0.y -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.NumPad3)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos0.y += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.I)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos2.x += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.K)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos2.x -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.J)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos2.z -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.L)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos2.z += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.U)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos2.y -= static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (kb.O)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            m_lightPos2.y += static_cast<float>(aTimer.GetElapsedSeconds()) * m_lightMovementSpeed;
        }
    }
    if (m_kbStateTracker.pressed.N)
    {
        if (m_currentGameState == GameState::GAMESTATE_GAMEPLAY)
        {
            ++m_testDisplayCount;
            if (m_testDisplayCount > m_testDisplayCountMax)
            {
                m_testDisplayCount = 0;
            }
        }
    }


    auto mouse = m_mouse->GetState();

    if (m_camera->GetCameraState() == CameraState::CAMERASTATE_FIRSTPERSON)
    {
        if (mouse.positionMode == Mouse::MODE_RELATIVE)
        {
            const float ROTATION_GAIN = 0.004f;
            DirectX::SimpleMath::Vector3 delta = DirectX::SimpleMath::Vector3(float(mouse.x), float(mouse.y), 0.f) * ROTATION_GAIN;

            float pitch = -delta.y;
            float yaw = -delta.x;

            m_camera->UpdatePitchYaw(pitch, yaw);
        }

        m_mouse->SetMode(mouse.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
    }
}

void Game::UpdateLighting()
{
    const float timeStamp = static_cast<float>(m_timer.GetTotalSeconds());
    m_effect->EnableDefaultLighting();
    if (m_currentLightingState == LightingState::LIGHTINGSTATE_JI)
    {
        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            ilights->SetLightEnabled(0, false);
            ilights->SetLightEnabled(1, false);
            ilights->SetLightEnabled(2, true);
            auto time = static_cast<float>(m_timer.GetTotalSeconds());
            float yaw = time * 0.4f;
            float pitch = time * 0.7f;
            float roll = time * 1.1f;
            auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(-pitch, 0.0, 0.0);
            auto quat1 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, 0.0, 0.0);
            auto quat2 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(-roll, 0.0, 0.0);
            auto light0 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat0);
            auto light1 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat1);
            auto light2 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat2);
            light0 = DirectX::SimpleMath::Vector3::UnitX;
            //light1 = light0;
            //light2 = light0;
            ilights->SetLightDirection(0, light0);
            ilights->SetLightDirection(1, light1);
            ilights->SetLightDirection(2, light2);
            m_lightPos0 = light0;
            m_lightPos1 = light1;
            m_lightPos2 = light2;
        }
    }
    else if (m_currentLightingState == LightingState::LIGHTINGSTATE_BMW)
    {
        // BMW effects

        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            ilights->SetLightEnabled(0, true);
            ilights->SetLightEnabled(1, true);
            ilights->SetLightEnabled(2, true);

            auto time = static_cast<float>(m_timer.GetTotalSeconds());

            float yaw = time * 0.4f;
            float pitch = time * 0.7f;
            float roll = time * 1.1f;

            auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, pitch, 0.0);
            auto quat1 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, yaw, 0.0);
            auto quat2 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, roll, 0.0);

            auto quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, -roll, 0.0);

            //quat0 = quat;
            //quat1 = quat;
            //quat2 = quat;
            DirectX::SimpleMath::Vector3 axis = -DirectX::SimpleMath::Vector3::UnitZ;
            //auto light0 = XMVector3Rotate(axis, quat0);
            //auto light1 = XMVector3Rotate(axis, quat1);
            //auto light2 = XMVector3Rotate(axis, quat2);

            DirectX::SimpleMath::Vector3 light0 = XMVector3Rotate(axis, quat0);
            DirectX::SimpleMath::Vector3 light1 = XMVector3Rotate(axis, quat1);
            DirectX::SimpleMath::Vector3 light2 = XMVector3Rotate(axis, quat2);
            light0.x += 1.0;
            light0.Normalize();
            light1.x += 1.0;
            light1.Normalize();
            light2.x += 1.0;
            light2.Normalize();

            quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);
            DirectX::SimpleMath::Vector3 light = XMVector3Rotate(axis, quat);
            //light.x += 1.0;
            //light.z += .5;
            light.Normalize();
            light0 = light;
            light1 = light;
            light2 = light;

            //light = - DirectX::SimpleMath::Vector3::UnitX;

            light0 = light;
            light1 = light;
            light2 = light;

            ilights->SetLightDirection(0, light0);
            ilights->SetLightDirection(1, light1);
            ilights->SetLightDirection(2, light2);

            m_lightPos0 = light0;
            m_lightPos1 = light1;
            m_lightPos2 = light2;

        }
    }
    else if (m_currentLightingState == LightingState::LIGHTINGSTATE_STARTSCREEN)
    {
        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            //ilights->EnableDefaultLighting();
            ilights->SetLightEnabled(0, true);
            ilights->SetLightEnabled(1, true);
            ilights->SetLightEnabled(2, true);

            auto time = static_cast<float>(m_timer.GetTotalSeconds());

            float yaw = time * 0.4f;
            float pitch = time * 0.7f;
            float roll = time * 1.1f;

            auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, pitch, 0.0);
            auto quat1 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, yaw, 0.0);
            auto quat2 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, roll, 0.0);

            auto quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, -roll, 0.0);

            //quat0 = quat;
            //quat1 = quat;
            //quat2 = quat;
            DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::UnitZ;
            //auto light0 = XMVector3Rotate(axis, quat0);
            //auto light1 = XMVector3Rotate(axis, quat1);
            //auto light2 = XMVector3Rotate(axis, quat2);

            DirectX::SimpleMath::Vector3 light0 = XMVector3Rotate(axis, quat0);
            DirectX::SimpleMath::Vector3 light1 = XMVector3Rotate(axis, quat1);
            DirectX::SimpleMath::Vector3 light2 = XMVector3Rotate(axis, quat2);
            light0.x += 1.0;
            light0.Normalize();
            light1.x += 1.0;
            light1.Normalize();
            light2.x += 1.0;
            light2.Normalize();

            quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);
            DirectX::SimpleMath::Vector3 light = XMVector3Rotate(axis, quat);
            //light.x += 1.0;
            //light.z += .5;
            //light.Normalize();

            //light = -DirectX::SimpleMath::Vector3::UnitX;
            light0 = light;
            light1 = light;
            //light = -DirectX::SimpleMath::Vector3::UnitX;
            light2 = - light;


            /*
            light0 = -DirectX::SimpleMath::Vector3::UnitX;
            light1 = -DirectX::SimpleMath::Vector3::UnitX;
            //light = -DirectX::SimpleMath::Vector3::UnitX;
            light2 = -DirectX::SimpleMath::Vector3::UnitX;
            */

            //ilights->SetLightEnabled(0, false);
            //ilights->SetLightEnabled(1, false);
            m_effect->Apply(m_d3dContext.Get());
            ilights->SetLightDirection(0, light0);
            ilights->SetLightDirection(1, light1);
            ilights->SetLightDirection(2, light2);

            m_lightPos0 = light0;
            m_lightPos1 = light1;
            m_lightPos2 = light2;

        }
    }
    else if (m_currentLightingState == LightingState::LIGHTINGSTATE_TEASERSCREEN)
    {
        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            ilights->SetLightEnabled(0, false);
            ilights->SetLightEnabled(1, false);
            ilights->SetLightEnabled(2, true);
            auto time = static_cast<float>(m_timer.GetTotalSeconds());
            float yaw = time * 0.4f;
            float pitch = time * 0.7f;
            float roll = time * 1.1f;
            roll = cosf(-timeStamp);
            auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);
            auto quat1 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);
            auto quat2 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);

            //auto light0 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat0);
            //auto light1 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat1);
            //auto light2 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat2);

            DirectX::SimpleMath::Vector3 axis = DirectX::SimpleMath::Vector3::UnitX;
            DirectX::SimpleMath::Vector3 light0 = XMVector3Rotate(axis, quat0);
            DirectX::SimpleMath::Vector3 light1 = XMVector3Rotate(axis, quat1);
            DirectX::SimpleMath::Vector3 light2 = XMVector3Rotate(axis, quat2);

            float shift = -0.2;
            light0.y += shift;
            light0.Normalize();
            light1.y += shift;
            light1.Normalize();
            light2.y += shift;
            light2.Normalize();

            //light1 = light0;
            //light2 = light0;
            ilights->SetLightDirection(0, light0);
            ilights->SetLightDirection(1, light1);
            ilights->SetLightDirection(2, light2);
            m_lightPos0 = light0;
            m_lightPos1 = light1;
            m_lightPos2 = light2;
        }
    }
    else if (m_currentLightingState == LightingState::LIGHTINGSTATE_MANUAL)
    {
        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            ilights->SetLightDirection(0, m_lightPos0);
            ilights->SetLightDirection(1, m_lightPos1);
            ilights->SetLightDirection(2, m_lightPos2);
        }
    }
    else if (m_currentLightingState == LightingState::LIGHTINGSTATE_NULL)
    {
    /*
        m_effect->EnableDefaultLighting();
        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            DirectX::SimpleMath::Vector3 direction = - DirectX::SimpleMath::Vector3::UnitX;
            ilights->SetLightDirection(0, direction);
            ilights->SetLightDirection(1, direction);
            ilights->SetLightDirection(2, direction);
        }
        //m_effect->Apply(m_d3dContext.Get());
        */
        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            ilights->SetLightEnabled(0, true);
            ilights->SetLightEnabled(1, true);
            ilights->SetLightEnabled(2, true);

            DirectX::SimpleMath::Vector3 light = - DirectX::SimpleMath::Vector3::UnitX;
            light.x = - 0.8;
            DirectX::SimpleMath::Vector3 light0 = light;
            DirectX::SimpleMath::Vector3 light1 = light;
            DirectX::SimpleMath::Vector3 light2 = light;

            ilights->SetLightDirection(0, light0);
            ilights->SetLightDirection(1, light1);
            ilights->SetLightDirection(2, light2);

            m_lightPos0 = light0;
            m_lightPos1 = light1;
            m_lightPos2 = light2;
        }

    }
    else if (m_currentLightingState == LightingState::LIGHTINGSTATE_STARTUP)
    {
    /*
        m_effect->EnableDefaultLighting();
        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::UnitX;
            ilights->SetLightDirection(0, direction);
            ilights->SetLightDirection(1, direction);
            ilights->SetLightDirection(2, direction);
        }
        */

        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            ilights->SetLightEnabled(0, true);
            ilights->SetLightEnabled(1, true);
            ilights->SetLightEnabled(2, true);

            auto time = static_cast<float>(m_timer.GetTotalSeconds());

            float yaw = time * 0.4f;
            float pitch = time * 0.7f;
            float roll = time * 1.1f;

            auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, pitch, 0.0);
            auto quat1 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, yaw, 0.0);
            auto quat2 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, roll, 0.0);

            auto quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, -roll, 0.0);

            //quat0 = quat;
            //quat1 = quat;
            //quat2 = quat;
            DirectX::SimpleMath::Vector3 axis = -DirectX::SimpleMath::Vector3::UnitZ;
            //auto light0 = XMVector3Rotate(axis, quat0);
            //auto light1 = XMVector3Rotate(axis, quat1);
            //auto light2 = XMVector3Rotate(axis, quat2);

            DirectX::SimpleMath::Vector3 light0 = XMVector3Rotate(axis, quat0);
            DirectX::SimpleMath::Vector3 light1 = XMVector3Rotate(axis, quat1);
            DirectX::SimpleMath::Vector3 light2 = XMVector3Rotate(axis, quat2);
            light0.x += 1.0;
            light0.Normalize();
            light1.x += 1.0;
            light1.Normalize();
            light2.x += 1.0;
            light2.Normalize();

            quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);
            DirectX::SimpleMath::Vector3 light = XMVector3Rotate(axis, quat);
            //light.x += 1.0;
            //light.z += .5;
            light.Normalize();
            light0 = light;
            light1 = light;
            light2 = light;

            //light = DirectX::SimpleMath::Vector3::UnitX;

            light0 = light;
            light1 = light;
            light2 = light;

            ilights->SetLightDirection(0, light0);
            ilights->SetLightDirection(1, light1);
            ilights->SetLightDirection(2, light2);

            m_lightPos0 = light0;
            m_lightPos1 = light1;
            m_lightPos2 = light2;

        }
    }
    else if (m_currentLightingState == LightingState::LIGHTINGSTATE_)
    {
        m_effect->EnableDefaultLighting();

        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            ilights->SetLightEnabled(0, true);
            ilights->SetLightEnabled(1, true);
            ilights->SetLightEnabled(2, true);

            auto time = static_cast<float>(m_timer.GetTotalSeconds());

            float yaw = time * 0.4f;
            float pitch = time * 0.7f;
            float roll = time * 11.1f;

            auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, pitch, 0.0);
            auto quat1 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, yaw, 0.0);
            auto quat2 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, roll, 0.0);

            auto quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, -roll, 0.0);

            //quat0 = quat;
            //quat1 = quat;
            //quat2 = quat;

            DirectX::SimpleMath::Vector3 axis = -DirectX::SimpleMath::Vector3::UnitZ;

            //auto light0 = XMVector3Rotate(axis, quat0);
            //auto light1 = XMVector3Rotate(axis, quat1);
            //auto light2 = XMVector3Rotate(axis, quat2);

            DirectX::SimpleMath::Vector3 light0 = XMVector3Rotate(axis, quat0);
            DirectX::SimpleMath::Vector3 light1 = XMVector3Rotate(axis, quat1);
            DirectX::SimpleMath::Vector3 light2 = XMVector3Rotate(axis, quat2);
            float offset = 7.0;
            light0.x -= offset;
            light0.Normalize();
            light1.x -= offset;
            light1.Normalize();
            light2.x -= offset;
            light2.Normalize();

            /*
            quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);
            DirectX::SimpleMath::Vector3 light = XMVector3Rotate(axis, quat);
            //light.x += 1.0;
            //light.z += .5;
            light.Normalize();
            light0 = light;
            light1 = light;
            light2 = light;
            */
            auto light = - DirectX::SimpleMath::Vector3::UnitX;

            light0 = light;
            light1 = light;
            light2 = light;
            
            ilights->SetLightDirection(0, light0);
            ilights->SetLightDirection(1, light1);
            ilights->SetLightDirection(2, light2);

            m_lightPos0 = light0;
            m_lightPos1 = light1;
            m_lightPos2 = light2;
        }
    }
    else if (m_currentLightingState == LightingState::LIGHTINGSTATE_TEST01)
    {
        auto ilights = dynamic_cast<IEffectLights*>(m_effect.get());
        if (ilights)
        {
            ilights->SetLightEnabled(0, false);
            ilights->SetLightEnabled(1, false);
            ilights->SetLightEnabled(2, true);
            auto time = static_cast<float>(m_timer.GetTotalSeconds());
            float yaw = time * 0.4f;
            float pitch = time * 0.7f;
            float roll = time * 1.1f;
            roll = cosf(-timeStamp * 1.2);
            auto quat0 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(-roll, 0.0, 0.0);
            auto quat1 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(roll, 0.0, 0.0);
            auto quat2 = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, 0.0, roll);
            auto light0 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat0);
            auto light1 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat1);
            auto light2 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat2);
            ////////////////////////////////////////////
            float roll2 = time * 3.1f;
            auto quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0, roll2, 0.0);
            DirectX::SimpleMath::Vector3 axis = -DirectX::SimpleMath::Vector3::UnitZ;
            DirectX::SimpleMath::Vector3 light = XMVector3Rotate(axis, quat);
            light.x += 1.0;
            //light.z += .5;
            light.Normalize();
            light0 = light;
            //light0 = light2;
            //light1 = light2;
            //light2 = light2;

            ilights->SetLightDirection(0, light0);
            ilights->SetLightDirection(1, light1);
            ilights->SetLightDirection(2, light2);
            m_lightPos0 = light0;
            m_lightPos1 = light1;
            m_lightPos2 = light2;
        }
    }

    m_effect->EnableDefaultLighting();

    ///////////////////////////////////////////////////////////////
    // m_effect2 update
    /*
    auto ilights2 = dynamic_cast<IEffectLights*>(m_effect2.get());
    if (ilights2)
    {
        ilights2->SetLightEnabled(0, true);

        auto time = static_cast<float>(m_timer.GetTotalSeconds());

        float yaw = time * 0.4f;
        float pitch = time * 0.7f;
        float roll = time * 1.1f;

        auto quat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(pitch, yaw, roll);

        //auto light = XMVector3Rotate(g_XMOne, quat);
        auto light2 = XMVector3Rotate(DirectX::SimpleMath::Vector3::UnitX, quat);


        m_effect2->SetFogEnabled(true);
        m_effect2->SetFogStart(.4);
        m_effect2->SetFogEnd(1.9);
        light2 = m_lightPos1;
        light2 = DirectX::SimpleMath::Vector3::UnitY;
        ilights2->SetLightDirection(0, light2);
        //ilights2->SetLightDirection(1, light2);
        ilights2->SetLightDirection(2, light2);
    }
    */
}