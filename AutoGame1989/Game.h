//
// Game.h
//

#pragma once
#include "Keyboard.h"
#include "AnimatedTexture.h"
#include "SpriteSheet.h"
#include "WICTextureLoader.h"
#include "Auto.h"
#include "Camera.h"
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

    void DrawDebugLines();
    void DrawCameraFocus();
    void DrawCar();
    void DrawCar2();
    void DrawIntroScreen();
    void DrawIntroScene();
    void DrawLightFocus();
    void DrawLightFocus1();
    void DrawMenuEnvironmentSelect();
    void DrawMenuMain();
    void DrawShape();
    void DrawStartScreen();
    void DrawTeaserScreen();

    void DrawWorld();
    void DrawWorldCube();
    void DrawWorldCubeTextured();
    void InitializeWorldGrid();
    void OnDeviceLost();
    void Present();
    void Render();
    void ResetGamePlay();

    void Update(DX::StepTimer const& aTimer);
    void UpdateInput(DX::StepTimer const& aTimer);

    Camera* pCamera;
    Auto* pAuto;
    Vehicle* m_vehicle;


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
    //using VertexType = DirectX::VertexPositionColor;
    //using VertexType = DirectX::VertexPositionNormalColor;
    //using VertexType = DirectX::VertexPositionTexture;
    //using VertexType = DirectX::VertexPositionColorTexture;
    using VertexType = DirectX::VertexPositionNormalColorTexture;
    
    using VertexType2 = DirectX::VertexPositionNormalColor;

    std::unique_ptr<DirectX::CommonStates>          m_states;   
    std::unique_ptr<DirectX::NormalMapEffect>       m_effect;
    std::unique_ptr<DirectX::BasicEffect>           m_effect2;
    //std::unique_ptr<DirectX::NormalMapEffect>           m_effect2;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType2>> m_batch2;
    //std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalColor>> m_batchNormal;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_normalMap;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
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
    DirectX::SimpleMath::Vector3                m_lightPos1 = DirectX::SimpleMath::Vector3::Zero;
    DirectX::SimpleMath::Vector3                m_lightPos2 = DirectX::SimpleMath::Vector3::Zero;
    std::vector<std::pair<DirectX::VertexPositionNormalColor,DirectX::VertexPositionNormalColor>>   m_worldGrid;

    bool                                        m_moveLightWithCameraFocus1 = false;
    bool                                        m_moveLightWithCameraFocus2 = false;
};
