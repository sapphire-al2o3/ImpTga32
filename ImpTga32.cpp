
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "PbPlugin.h"

#ifdef __cplusplus
#define DLLExport extern "C" __declspec (dllexport)
#else
#define DLLExport __declspec (dllexport)
#endif


// TODO: １つのプラグインには複数の拡張子を登録できます。
//       登録する拡張子の数を定義してください。
#define SUPPORTED_TYPE_IN_PLUGIN 1

// TODO: 対応する拡張子を 拡張子|英語説明|日本語説明 で定義してください。拡張子
//       にドットはつけないで下さい。複数の拡張子を登録する場合は ';' で区切っ
//       てください。
//
//       拡張子が空の場合はインポートメニューに表示されます。[TWAIN 機器からの
//       読み込み]のようにファイル以外からインポートする場合に利用できます。
const char* g_szTypes[SUPPORTED_TYPE_IN_PLUGIN] = 
{
	"tga|TARGA Files|TGA ファイル"
};


// TODO: 任意のコピーライト文字列を設定してください。
const char* g_szCopyright = "Copyright (C) 2011 by tel. All Rights Reserved.";


HINSTANCE g_hInstance;

///////////////////////////////////////////////////////////////////////////////
// エントリーポイント

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) 
		g_hInstance = hinstDLL;
	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// プラグイン情報取得

DLLExport WORD WINAPI PB_GetPluginInfo(LPSTR buf, int buflen)
{
	// TODO: buf にコピーライト文字列を設定し、以下の値を返してください。
	strncpy(buf, g_szCopyright, buflen);

	return MAKEWORD(PBPLUGIN_IMPORT, PBPLUGIN_IMPORT_VERSION);
}


///////////////////////////////////////////////////////////////////////////////
// 対応している拡張子の数を取得

DLLExport int WINAPI PBI_GetCount(void)
{
	// TODO: プラグインに対応している拡張子の数を返してください。
	return SUPPORTED_TYPE_IN_PLUGIN;
}


///////////////////////////////////////////////////////////////////////////////
// 対応しているファイルの詳細を取得

DLLExport void WINAPI PBI_GetSupportedType(int nIndex, LPSTR szType, int nSize)
{
	// TODO: szTypes に対応している拡張子を設定してください。
	strncpy(szType, g_szTypes[nIndex], nSize);
}


#pragma pack(1)

struct TGAHeader {
	char idLength;
	char colorMapType;
	char dataTypeCode;
	short int colorMapOrigin;
	short int colorMapLength;
	char colorMapDepth;
	short int xOrigin;
	short int yOrigin;
	short width;
	short height;
	char bitsPerPixel;
	char imageDescriptor;
};

struct TGAFooter {
	int filePosition;
	int dirPosition;
	char id[17];
	char end;
};

#pragma pack()

struct Gray8 {
	unsigned char y;
	operator Pixel32() {
		Pixel32 p;
		p.item.b = p.item.g = p.item.r = y;
		p.item.a = 255;
		return p;
	}
};

struct BGR24 {
	unsigned char b, g, r;
	operator Pixel32() {
		Pixel32 p;
		p.item.b = b;
		p.item.g = g;
		p.item.r = r;
		p.item.a = 255;
		return p;
	}
};

struct BGRA32 {
	unsigned char b, g, r, a;
	operator Pixel32() {
		Pixel32 p;
		p.item.b = b;
		p.item.g = g;
		p.item.r = r;
		p.item.a = a;
		return p;
	}
};

template <typename T>
inline void ReadLine(int y, PImage* pImage, PProvidedFunc* pFunc, FILE *fp, int length) {
	T p;
	Pixel32 col;
	for(int j = 0; j < length; j++) {
		fread(&p, sizeof(T), 1, fp);
		col = p;
		pFunc->SetData(pImage, 0, j, y, col.color);
	}
}

template <typename T>
inline void ReadLineRLE(int y, PImage* pImage, PProvidedFunc* pFunc, FILE *fp, int length) {
	int j = 0;
	Pixel32 col;
	while(j < length) {
		unsigned char buf[1];
		fread(buf, 1, 1, fp);
		int n = (buf[0] & 127) + 1;
		T p;
		if(buf[0] & 128) {
			fread(&p, sizeof(T), 1, fp);
			col = p;
			for(int k = 0; k < n; k++) {
				pFunc->SetData(pImage, 0, j + k, y, col.color);
			}
		} else {
			for(int k = 0; k < n; k++) {
				fread(&p, sizeof(T), 1, fp);
				col = p;
				pFunc->SetData(pImage, 0, j + k, y, col.color);
			}
		}
		j += n;
	}
}

///////////////////////////////////////////////////////////////////////////////
// インポート実行

DLLExport int WINAPI PBI_Execute(int nIndex, HWND hWnd, LPCSTR szFileName, PImage* pImage, PProvidedFunc* pFunc)
{
	FILE *fp = NULL;

	if((fp = fopen(szFileName, "rb")) == NULL) {
		return PB_ERR_FILE_NOT_FOUND;
	}

	
	TGAHeader header;
	
	fread(&header, sizeof(TGAHeader), 1, fp);

	// skip id field
	if(header.idLength > 0) {
		char buf[256];
		fread(&buf, header.idLength, 1, fp);
	}

	if(header.dataTypeCode == 0) {
		fclose(fp);
		return PB_ERR_BAD_FORMAT;
	}

	switch(header.bitsPerPixel) {
	case 8:
	case 24:
	case 32:
		break;
	default:
		return PB_ERR_BAD_FORMAT;
	}

	int width = header.width;
	int height = header.height;

	pFunc->CreateFullColor(pImage, width, height, 1, FALSE);

	pImage->nPaletteSize = 0;
	pFunc->ProgressStart(0, height);

	for(int i = 0; i < height; i++) {
		int y;
		if(header.imageDescriptor & 32) {
			y = i;
		} else {
			y = height - i - 1;
		}

		switch(header.dataTypeCode) {
		case 0x2:
			switch(header.bitsPerPixel) {
			case 24:
				ReadLine<BGR24>(y, pImage, pFunc, fp, width);
				break;
			case 32:
				ReadLine<BGRA32>(y, pImage, pFunc, fp, width);
				break;
			}
			break;
		case 0x3:
			ReadLine<Gray8>(y, pImage, pFunc, fp, width);
			break;
		case 0xA:
			switch(header.bitsPerPixel) {
			case 24:
				ReadLineRLE<BGR24>(y, pImage, pFunc, fp, width);
				break;
			case 32:
				ReadLineRLE<BGRA32>(y, pImage, pFunc, fp, width);
				break;
			}
			break;
		case 0xB:
			ReadLineRLE<Gray8>(y, pImage, pFunc, fp, width);
			break;
		}

		pFunc->ProgressSetPos(i);
	}

	pFunc->ProgressEnd();

	fclose(fp);

	return PB_ERR_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
// プラグインの設定

/*

DLLExport void WINAPI PBI_Setup(HWND hWnd, PProvidedFunc* pFunc)
{
	// TODO: プラグインの設定を行います。
}

*/


///////////////////////////////////////////////////////////////////////////////
// Visual C++ 用 エクスポート名の変換

#ifdef _MSC_VER
#pragma comment(linker, "/EXPORT:PB_GetPluginInfo=_PB_GetPluginInfo@8")
#pragma comment(linker, "/EXPORT:PBI_GetCount=_PBI_GetCount@0")
#pragma comment(linker, "/EXPORT:PBI_GetSupportedType=_PBI_GetSupportedType@12")
#pragma comment(linker, "/EXPORT:PBI_Execute=_PBI_Execute@20")
//#pragma comment(linker, "/EXPORT:PBI_Setup=_PBI_Setup@8")
#endif

