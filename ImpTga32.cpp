
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "PbPlugin.h"

#ifdef __cplusplus
#define DLLExport extern "C" __declspec (dllexport)
#else
#define DLLExport __declspec (dllexport)
#endif


// TODO: �P�̃v���O�C���ɂ͕����̊g���q��o�^�ł��܂��B
//       �o�^����g���q�̐����`���Ă��������B
#define SUPPORTED_TYPE_IN_PLUGIN 1

// TODO: �Ή�����g���q�� �g���q|�p�����|���{����� �Œ�`���Ă��������B�g���q
//       �Ƀh�b�g�͂��Ȃ��ŉ������B�����̊g���q��o�^����ꍇ�� ';' �ŋ�؂�
//       �Ă��������B
//
//       �g���q����̏ꍇ�̓C���|�[�g���j���[�ɕ\������܂��B[TWAIN �@�킩���
//       �ǂݍ���]�̂悤�Ƀt�@�C���ȊO����C���|�[�g����ꍇ�ɗ��p�ł��܂��B
const char* g_szTypes[SUPPORTED_TYPE_IN_PLUGIN] = 
{
	"tga|TARGA Files|TGA �t�@�C��"
};


// TODO: �C�ӂ̃R�s�[���C�g�������ݒ肵�Ă��������B
const char* g_szCopyright = "Copyright (C) 2011 by tel. All Rights Reserved.";


HINSTANCE g_hInstance;

///////////////////////////////////////////////////////////////////////////////
// �G���g���[�|�C���g

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) 
		g_hInstance = hinstDLL;
	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// �v���O�C�����擾

DLLExport WORD WINAPI PB_GetPluginInfo(LPSTR buf, int buflen)
{
	// TODO: buf �ɃR�s�[���C�g�������ݒ肵�A�ȉ��̒l��Ԃ��Ă��������B
	strncpy(buf, g_szCopyright, buflen);

	return MAKEWORD(PBPLUGIN_IMPORT, PBPLUGIN_IMPORT_VERSION);
}


///////////////////////////////////////////////////////////////////////////////
// �Ή����Ă���g���q�̐����擾

DLLExport int WINAPI PBI_GetCount(void)
{
	// TODO: �v���O�C���ɑΉ����Ă���g���q�̐���Ԃ��Ă��������B
	return SUPPORTED_TYPE_IN_PLUGIN;
}


///////////////////////////////////////////////////////////////////////////////
// �Ή����Ă���t�@�C���̏ڍׂ��擾

DLLExport void WINAPI PBI_GetSupportedType(int nIndex, LPSTR szType, int nSize)
{
	// TODO: szTypes �ɑΉ����Ă���g���q��ݒ肵�Ă��������B
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
// �C���|�[�g���s

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
// �v���O�C���̐ݒ�

/*

DLLExport void WINAPI PBI_Setup(HWND hWnd, PProvidedFunc* pFunc)
{
	// TODO: �v���O�C���̐ݒ���s���܂��B
}

*/


///////////////////////////////////////////////////////////////////////////////
// Visual C++ �p �G�N�X�|�[�g���̕ϊ�

#ifdef _MSC_VER
#pragma comment(linker, "/EXPORT:PB_GetPluginInfo=_PB_GetPluginInfo@8")
#pragma comment(linker, "/EXPORT:PBI_GetCount=_PBI_GetCount@0")
#pragma comment(linker, "/EXPORT:PBI_GetSupportedType=_PBI_GetSupportedType@12")
#pragma comment(linker, "/EXPORT:PBI_Execute=_PBI_Execute@20")
//#pragma comment(linker, "/EXPORT:PBI_Setup=_PBI_Setup@8")
#endif

