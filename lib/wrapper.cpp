#include <windows.h>
#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#define DLL_EXPORT __declspec(dllexport)

HRESULT setVolume(float volume)
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

extern "C" DLL_EXPORT void setVolumeToZero()
{
    setVolume(0.0f);
}