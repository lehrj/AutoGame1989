//
// Game.h
//

#pragma once
#include "Keyboard.h"
#include "AnimatedTexture.h"
#include "SpriteSheet.h"
#include "ScrollingBackground.h"
#include "WICTextureLoader.h"
#include "Auto.h"
#include "Camera.h"
#include "Environment.h"
#include "sounds.h"
#include "Vehicle.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:
    Game() noexcept;
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;

    // Audio
    void OnNewAudioDevice() { m_retryAudio = true; };

private:
    void AudioPlayMusic(XACT_WAVEBANK_AUDIOBANK aSFX);
    void AudioPlaySFX(XACT_WAVEBANK_AUDIOBANK aSFX);

    void Clear();
    void CreateDevice();
    void CreateResources();
  
    void DrawCameraFocus();
    void DrawCar();
    void DrawCar2();
    void DrawDebugLines();
    void DrawGridForStartScreen();
    void DrawIntroScreen();
    void DrawIntroScene();
    void DrawLightBar();
    void DrawLightFocus2();
    void DrawLightFocus1();
    void DrawLightFocus3();
    void DrawLogoScreen();
    void DrawMenuEnvironmentSelect();
    void DrawMenuMain();
    void DrawShape();
    void DrawStartScreen();
    void DrawStartScreenOld();
    void DrawTeaserScreen();
    void DrawTerrain();
    void DrawTerrain2();
    void DrawTimer();
    void DrawWorld();
    void DrawWorldCube();
    void DrawWorldCubeTextured();
    void InitializeWorldGrid();
    void OnDeviceLost();
    void Present();
    void Render();
    void ResetGamePlay();

    void DrawDebugValue();


    void Update(DX::StepTimer const& aTimer);
    void UpdateInput(DX::StepTimer const& aTimer);

    Auto* m_auto;
    Camera* m_camera;
    Vehicle* m_vehicle;
    Environment* m_environment;

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
    DX::StepTimer                                   m_flightStepTimer;
    double                                          m_projectileTimer = 0.0;

    // WLJ add start
    using VertexType = DirectX::VertexPositionNormalColorTexture;   
    using VertexType2 = DirectX::VertexPositionNormalColor;
    using VertexType3 = DirectX::VertexPositionColor;

    std::unique_ptr<DirectX::CommonStates>          m_states;   
    std::unique_ptr<DirectX::NormalMapEffect>       m_effect;
    std::unique_ptr<DirectX::BasicEffect>           m_effect2;
    std::unique_ptr<DirectX::BasicEffect>           m_effect3;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType2>> m_batch2;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType3>> m_batch3;


    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMap;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_specular;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMapTest;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureTest;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_specularTest;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMapJI;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_specularJI;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureJI;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMapBMW;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMapBMW2;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_specularBMW;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureBMW;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMapAutoGame;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_specularAutoGame;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureAutoGame;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMapTeaser;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_specularTeaser;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureTeaser;

    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayout;
    // End

    std::shared_ptr<DirectX::IEffect>               m_lightEffect;
    //std::unique_ptr<DirectX::IEffect>               m_lightEffect;

    std::unique_ptr<DirectX::GeometricPrimitive>    m_shape;

    //world start 
    DirectX::SimpleMath::Matrix                     m_world;
    DirectX::SimpleMath::Matrix                     m_view;
    DirectX::SimpleMath::Matrix                     m_proj;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_raster; // WLJ anti-aliasing
    //world end

    // WLJ added for drawing projectile and swing  
    int                                             m_swingPathStep = 0;
    int                                             m_projectilePathStep = 0;

    // WLJ add for mouse and keybord interface
    std::unique_ptr<DirectX::Keyboard>              m_keyboard;
    std::unique_ptr<DirectX::Mouse>                 m_mouse;
    DirectX::Keyboard::KeyboardStateTracker         m_kbStateTracker;

    // WLJ added for displaying text
    std::unique_ptr<DirectX::SpriteFont>            m_font;
    std::unique_ptr<DirectX::SpriteFont>            m_titleFont;
    std::unique_ptr<DirectX::SpriteFont>            m_bitwiseFont;
    DirectX::SimpleMath::Vector2                    m_fontPos;
    DirectX::SimpleMath::Vector2                    m_fontPos2;
    DirectX::SimpleMath::Vector2                    m_fontPosDebug;
    DirectX::SimpleMath::Vector2                    m_fontMenuPos;
    DirectX::SimpleMath::Vector2                    m_bitwiseFontPos;
    std::unique_ptr<DirectX::SpriteBatch>           m_spriteBatch;

    // Character Textures
    std::unique_ptr<AnimatedTexture>                m_character;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_characterTexture;
    DirectX::SimpleMath::Vector2                    m_characterPos;

    std::unique_ptr<AnimatedTexture>                m_pacSprite;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pacTexture;
    DirectX::SimpleMath::Vector2                    m_pacPos;

    std::unique_ptr<AnimatedTexture>                m_character0;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_character0Texture;
    DirectX::SimpleMath::Vector2                    m_character0Pos;

    std::unique_ptr<AnimatedTexture>                m_character1;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_character1Texture;
    DirectX::SimpleMath::Vector2                    m_character1Pos;

    std::unique_ptr<AnimatedTexture>                m_character2;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_character2Texture;
    DirectX::SimpleMath::Vector2                    m_character2Pos;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_characterBackgroundTexture;
    DirectX::SimpleMath::Vector2                    m_characterBackgroundPos;
    DirectX::SimpleMath::Vector2                    m_characterBackgroundOrigin;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_environSelectCalmTexture;
    DirectX::SimpleMath::Vector2                    m_environSelectCalmPos;
    DirectX::SimpleMath::Vector2                    m_environSelectCalmOrigin;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_environSelectBreezyTexture;
    DirectX::SimpleMath::Vector2                    m_environSelectBreezyPos;
    DirectX::SimpleMath::Vector2                    m_environSelectBreezyOrigin;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_environSelectAlienTexture;
    DirectX::SimpleMath::Vector2                    m_environSelectAlienPos;
    DirectX::SimpleMath::Vector2                    m_environSelectAlienOrigin;

    // WLJ Intro screen logos
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bmwLogoTexture;
    DirectX::SimpleMath::Vector2                    m_bmwLogoPos;
    DirectX::SimpleMath::Vector2                    m_bmwLogoOrigin;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_jiLogoTexture;
    DirectX::SimpleMath::Vector2                    m_jiLogoPos;
    DirectX::SimpleMath::Vector2                    m_jiLogoOrigin;

    // WLJ variable to hold selected menu item
    int                                             m_menuSelect = 0;

    enum class GameState
    {
        GAMESTATE_INTROSCREEN,
        GAMESTATE_STARTSCREEN,
        GAMESTATE_MAINMENU,
        GAMESTATE_CHARACTERSELECT,
        GAMESTATE_COURSESELECT,
        GAMESTATE_ENVIRONTMENTSELECT,
        GAMESTATE_GAMEPLAY,
        GAMESTATE_TEASERSCREEN
    };
    GameState                                   m_currentGameState;

    enum class UiState
    {
        UISTATE_SWING,
        UISTATE_SHOT,
        UISTATE_SCORE
    };
    UiState                                     m_currentUiState;

    enum class LightingState
    {
        LIGHTINGSTATE_JI,
        LIGHTINGSTATE_BMW,
        LIGHTINGSTATE_STARTSCREEN,
        LIGHTINGSTATE_TEASERSCREEN,
        LIGHTINGSTATE_MANUAL,
        LIGHTINGSTATE_NULL,
        LIGHTINGSTATE_STARTUP,
        LIGHTINGSTATE_TEST01,
        LIGHTINGSTATE_,
    };
    LightingState                               m_currentLightingState;

    // audio 
    std::unique_ptr<DirectX::AudioEngine>       m_audioEngine;
    bool                                        m_retryAudio;
    float                                       m_musicVolume = 0.9f;
    float                                       m_sfxVolume = 0.5f;
    std::unique_ptr<DirectX::WaveBank>          m_audioBank;
    std::unique_ptr<DirectX::SoundStreamInstance> m_audioMusicStream;
    std::unique_ptr<DirectX::SoundStreamInstance> m_audioEffectStream;

    const bool                                  m_isInDebugMode = true;

    DirectX::SimpleMath::Vector3                m_carPos = DirectX::SimpleMath::Vector3(0.3, -0.3, -0.45);
    DirectX::SimpleMath::Vector3                m_carDirection = DirectX::SimpleMath::Vector3::UnitX;
    float                                       m_carAim = 0.0f;

    DirectX::SimpleMath::Vector3                m_cameraFocusPos = DirectX::SimpleMath::Vector3::Zero;
    DirectX::SimpleMath::Vector3                m_lightPos0 = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3                m_lightPos1 = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3                m_lightPos2 = DirectX::SimpleMath::Vector3::UnitX;

    // light pos for m_effect2
    DirectX::SimpleMath::Vector3                m_lightEffect2Pos0 = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3                m_lightEffect2Pos1 = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3                m_lightEffect2Pos2 = DirectX::SimpleMath::Vector3::UnitX;

    std::vector<std::pair<DirectX::VertexPositionNormalColor,DirectX::VertexPositionNormalColor>>   m_worldGrid;

    bool                                        m_moveLightWithCameraFocus1 = false;
    bool                                        m_moveLightWithCameraFocus2 = false;
    bool                                        m_moveLightWithCameraFocus3 = false;

    const float                                 m_lightMovementSpeed = 0.4;

    std::unique_ptr<ScrollingBackground>             m_road;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTex;


    int                                         m_testDisplayCount = 0;
    const int                                   m_testDisplayCountMax = 3;

    // for height map
    std::vector<DirectX::VertexPositionColor>   m_terrainVector;

    DirectX::VertexPositionColor*               m_terrainVertexArray;
    DirectX::VertexPositionColor*               m_terrainVertexArrayBase;
    int                                         m_terrainVertexCount;
    
    bool InitializeTerrainArray();
    bool InitializeTerrainArray2();
    /////////////////////////////

    std::vector<DirectX::VertexPositionNormalColor>   m_terrainVector2;

    DirectX::VertexPositionNormalColor* m_terrainVertexArray2;
    DirectX::VertexPositionNormalColor* m_terrainVertexArrayBase2;
    int                                         m_terrainVertexCount2;

    
    void SetLighting(LightingState aLightState);
    void UpdateLighting();

    float                               m_debugValue1 = 0.0;
    float                               m_debugValue2 = 0.0;
    float                               m_debugValue3 = 0.0;
};
