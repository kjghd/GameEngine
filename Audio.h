#pragma once
#include<atlcomcli.h> // CComPtr
#include <xaudio2.h>
#include <array>


enum eAudioID
{
	AID_TestSound,

	AID_COUNT
};

class Audio
{
	struct AudioData
	{
		DWORD fileType{};
		WAVEFORMATEX waveFormatx = { 0 };
		XAUDIO2_BUFFER dataBuffer = { 0 };
	};

public:

	CComPtr<IXAudio2> pEngine;
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoiceA; // Placeholder

	std::array<AudioData, AID_COUNT> aAudioData;

	~Audio()
	{
		pMasterVoice->DestroyVoice();
		CoUninitialize();
	}

	void Setup()
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (SUCCEEDED(hr))
		{
			hr = XAudio2Create(&pEngine);
			if (SUCCEEDED(hr))
			{
				hr = pEngine->CreateMasteringVoice(&pMasterVoice);
				if (SUCCEEDED(hr))
				{
					LoadAudioFile(L"Assets/Audio/TestSound.wav", AID_TestSound);
				}
			}
		}
	}

	void LoadAudioFile(LPCWSTR lpFileName, int AudioID)
	{
		HRESULT hr;

		HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		bool fmt{false};
		bool bfr{false};
		bool typ{false};

		DWORD chunkSize{};
		DWORD tmp_buffer{};
		DWORD bytesRead{};

		while (!fmt || !bfr || !typ)
		{
			// Read FOURCC
			if (0 == ReadFile(hFile, &tmp_buffer, 4, &bytesRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			// Break down data
			switch (tmp_buffer)
			{
			case 'FFIR':
			{
				if (0 == ReadFile(hFile, &chunkSize, 4, &bytesRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());

				if (0 == ReadFile(hFile, &aAudioData.at(AudioID).fileType, 4, &bytesRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());

				chunkSize -= 4; // Chunk size includes file type FOURCC.
				typ = true; // Found file type.
				break;
			}
				
			case ' tmf':
			{
				if (0 == ReadFile(hFile, &chunkSize, 4, &bytesRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());

				if (0 == ReadFile(hFile, &aAudioData.at(AudioID).waveFormatx, chunkSize, &bytesRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());

				fmt = true; // Found wave format.
				break;
			}
				
			case 'atad':
			{
				if(0 == ReadFile(hFile, &chunkSize, 4, &bytesRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());

				BYTE* pData = new BYTE[chunkSize]{};

				if(0 == ReadFile(hFile, pData, chunkSize, &bytesRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());

				aAudioData.at(AudioID).dataBuffer.pAudioData = pData;
				aAudioData.at(AudioID).dataBuffer.AudioBytes = chunkSize;
				aAudioData.at(AudioID).dataBuffer.Flags = XAUDIO2_END_OF_STREAM;

				bfr = true; // Found audio data buffer.
				break;
			}
				
			default:
			{
				if (0 == ReadFile(hFile, &chunkSize, 4, &bytesRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());

				if (0 == SetFilePointer(hFile, chunkSize, NULL, FILE_CURRENT))
					hr = HRESULT_FROM_WIN32(GetLastError());

				break;
			}
			}
		}
	}

	void PlayAudio(IXAudio2SourceVoice** ppSourceVoice, int AudioID)
	{
		pEngine->CreateSourceVoice(ppSourceVoice, (WAVEFORMATEX*)&aAudioData.at(AudioID).waveFormatx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
		(*ppSourceVoice)->SubmitSourceBuffer(&aAudioData.at(AudioID).dataBuffer);

		(*ppSourceVoice)->Start();
	}
	void StopAudio(IXAudio2SourceVoice** ppSourceVoice)
	{
		(*ppSourceVoice)->Stop();
		(*ppSourceVoice)->FlushSourceBuffers();
	}
};