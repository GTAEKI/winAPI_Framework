#pragma once

#include "../Game.h"

class CResourcesManager
{
	DECLARE_SINGLE(CResourcesManager)

private:
	unordered_map<string, class CTexture*> m_mapTexture;
	HINSTANCE m_hInst;
	HDC m_hdc;
	class CTexture* m_pBackBuffer;

public:
	bool Init(HINSTANCE hInst, HDC hdc);
	class CTexture* LoadTexture(const string& strKey, const wchar_t* pFileName, const string& strPathKey = TEXTURE_PATH);
	class CTexture* FindTexture(const string& strKey);

	class CTexture* GetBackBuffer() const;
};

