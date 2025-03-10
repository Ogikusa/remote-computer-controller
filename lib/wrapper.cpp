#include <windows.h>
#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#define DLL_EXPORT __declspec(dllexport)

extern "C" DLL_EXPORT HRESULT __stdcall setVolume(float volume)
{
    CoInitialize(nullptr);

    IMMDeviceEnumerator *pEnumerator = nullptr;
    IMMDevice *pDevice = nullptr;
    IAudioEndpointVolume *pVolume = nullptr;

    // デフォルトのオーディオデバイスを取得
    HRESULT hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        nullptr,
        CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (void **)&pEnumerator);

    if (FAILED(hr))
    {
        std::cerr << "IMMDeviceEnumerator の作成に失敗しました。" << std::endl;
        return hr;
    }

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
    if (FAILED(hr))
    {
        std::cerr << "デフォルトオーディオデバイスの取得に失敗しました。" << std::endl;
        pEnumerator->Release();
        return hr;
    }

    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void **)&pVolume);
    if (FAILED(hr))
    {
        std::cerr << "IAudioEndpointVolume のアクティベートに失敗しました。" << std::endl;
        pDevice->Release();
        pEnumerator->Release();
        return hr;
    }

    // 音量を設定 (0.0f - 1.0f)
    hr = pVolume->SetMasterVolumeLevelScalar(volume, nullptr);
    if (FAILED(hr))
    {
        std::cerr << "音量の設定に失敗しました。" << std::endl;
    }

    // リソースの解放
    pVolume->Release();
    pDevice->Release();
    pEnumerator->Release();

    // COM の終了
    CoUninitialize();

    return hr;
}

extern "C" DLL_EXPORT float getVolume()
{
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr))
    {
        std::cerr << "COM library initialization failed." << std::endl;
        return -1.0f;
    }

    // MMDeviceEnumeratorの作成
    IMMDeviceEnumerator *pEnumerator = nullptr;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void **)&pEnumerator);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create MMDeviceEnumerator instance." << std::endl;
        CoUninitialize();
        return -1.0f;
    }

    // デフォルトのオーディオデバイスを取得
    IMMDevice *pDevice = nullptr;
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get default audio endpoint." << std::endl;
        pEnumerator->Release();
        CoUninitialize();
        return -1.0f;
    }

    // IAudioEndpointVolumeを取得
    IAudioEndpointVolume *pVolume = nullptr;
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void **)&pVolume);
    if (FAILED(hr))
    {
        std::cerr << "Failed to activate IAudioEndpointVolume interface." << std::endl;
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();
        return -1.0f;
    }

    // 音量の取得（0.0f - 1.0f）
    float currentVolume = 0.0f;
    hr = pVolume->GetMasterVolumeLevelScalar(&currentVolume);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get the current volume." << std::endl;
    }

    // 解放
    pVolume->Release();
    pDevice->Release();
    pEnumerator->Release();
    CoUninitialize();

    return currentVolume;
}

extern "C" DLL_EXPORT void monitorOff()
{
    SendMessageW(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
}

extern "C" DLL_EXPORT void monitorOn()
{
    SendMessageW(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)-1);
    mouse_event(MOUSEEVENTF_MOVE, 1, 0, 0, 0);
    mouse_event(MOUSEEVENTF_MOVE, -1, 0, 0, 0);
}