//-----------------------------------------------------------------------------
//
// PictBear Second Edition <Filter/Import/Export> Plugin Header File
//
// $Version 1.6 (last modified: 2004/05/08)
//
//                                Copyright(C) 1999-2004 by Yasuyuki Kashiwagi.
//                                                         All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef __PBPLUGIN_H
#define __PBPLUGIN_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif


///////////////////////////////////////////////////////////////////////////////
// PictBear �Ŏg�p������f�f�[�^
//-----------------------------------------------------------------------------
// ���ׂĂ̒l�� 0 �` 255 �ŕ\������A0 ���Œ�l�A255 ���ő�l��\�킷�B
// 255 �ɋ߂Â��قǁA���̉e���������Ȃ�ARGBA(255, 255, 255, 255) �Ŕ��ƂȂ�B
//-----------------------------------------------------------------------------

#pragma pack(1)

typedef union tagPixel32 
{
	DWORD color; // �J���[�l

	struct 
	{
		BYTE b;  // ��(Blue)
		BYTE g;  // ��(Green)
		BYTE r;  // ��(Red)
		BYTE a;  // �s�����x(Opacity)
	} item;
} Pixel32;

#pragma pack()


///////////////////////////////////////////////////////////////////////////////
// �v���O�C���p�萔
//-----------------------------------------------------------------------------
// �e�v���O�C���̃o�[�W�����E��ނ��`����B�v���O�C���� PB_GetPluginInfo �ŁA
// �����Œ�`����Ă���o�[�W�����E��ޒ萔��Ԃ��K�v������B
//-----------------------------------------------------------------------------

//#define PBPLUGIN_VERSION		0		// �����o�[�W�����݊�
										// �@�V�o�[�W�����ł͉��L�萔���g�p

#define PBPLUGIN_FILTER_VERSION	0x01	// �t�B���^�v���O�C���̃o�[�W����
#define PBPLUGIN_IMPORT_VERSION	0x00	// �C���|�[�g�v���O�C���̃o�[�W����
#define PBPLUGIN_EXPORT_VERSION	0x00	// �G�N�X�|�[�g�v���O�C���̃o�[�W����

#define PBPLUGIN_UNKNOWN		0x00	// �s���ȃv���O�C��
#define PBPLUGIN_FILTER			0x01	// �t�B���^�v���O�C��
#define PBPLUGIN_IMPORT			0x02	// �C���|�[�g�v���O�C��
#define PBPLUGIN_EXPORT			0x03	// �G�N�X�|�[�g�v���O�C��


///////////////////////////////////////////////////////////////////////////////
// �v���O�C���p�G���[�R�[�h
//-----------------------------------------------------------------------------
// �v���O�C���͎��s���ʂ�\�킷�G���[�R�[�h��Ԃ��K�v������B���s�֐��𔲂��鎞
// �́A�����ɒ�`���ꂽ�萔��Ԃ����ƁB
// 
//  F: �t�B���^�v���O�C���Ŏg�p�\
//  I: �C���|�[�g�v���O�C���Ŏg�p�\
//  E: �G�N�X�|�[�g�v���O�C���Ŏg�p�\
//-----------------------------------------------------------------------------

#define PB_ERR_SUCCESS				0	// �G���[����(F/I/E)
#define PB_ERR_UNKNOWN				1	// �s���ȃG���[(F/I/E)
#define PB_ERR_FILE_NOT_FOUND		2	// �t�@�C�������݂��Ȃ�(I)
#define PB_ERR_FILE_CANNOT_CREATE	3	// �t�@�C�����쐬�ł��Ȃ�(E)
#define PB_ERR_BAD_FORMAT			4	// �t�@�C���̃t�H�[�}�b�g���ُ�(I)
#define PB_ERR_WRITE_ERROR			5	// �������݃G���[(E)
#define PB_ERR_UNKNOWN_EXTENSION	6	// �s���Ȋg���q(I/E)
#define PB_ERR_CANCELED				7	// ���s������(F/I/E)


///////////////////////////////////////////////////////////////////////////////
// �t�B���^�v���O�C���̑Ή��t�H�[�}�b�g�萔
//-----------------------------------------------------------------------------
// �t�B���^�v���O�C���͑Ή����Ă���C���[�W�^�C�v�萔�� PBF_GetSupportedFormat 
// �ŕԂ��K�v������BPBF_REFERENCE_IMAGE ���Z�b�g����ƁA�C���[�W�S�̂̃f�[�^��
// �t�B���^�ɓn���B
//-----------------------------------------------------------------------------

#define PBF_TYPE_MONO			0x01	// ���m�N���C���[�W
#define PBF_TYPE_GRAYSCALE		0x02	// �O���[�X�P�[��
#define PBF_TYPE_INDEXCOLOR		0x04	// �C���f�b�N�X�J���[
#define PBF_TYPE_FULLCOLOR		0x08	// �t���J���[

#define PBF_REFERENCE_IMAGE		0x10	// �C���[�W�f�[�^���Q��


///////////////////////////////////////////////////////////////////////////////
// �t�B���^�v���O�C���̃}�X�N�����萔
//-----------------------------------------------------------------------------
// �t�B���^�͏����̌��ʂ��}�X�N�̈�ɉ����ă}�X�N���邩�ǂ�����I�����邱�Ƃ���
// ����BPFilterImage::dwMaskFlag �Ŏw�肷�邱�ƁB�f�t�H���g�� PBF_MASK_CLIENT
// �ł���B
//-----------------------------------------------------------------------------

#define PBF_MASK_CLIENT			0x00	// �}�X�N�� PictBear ������
#define PBF_MASK_PLUGIN			0x01	// �}�X�N�̓v���O�C��������


///////////////////////////////////////////////////////////////////////////////
// �t�B���^�v���O�C�� Version 0 �p�̐i�s�󋵂�ʒm����֐�
//-----------------------------------------------------------------------------
// PictBear Version 1.xx �n�Ƃ̌݊��p�ł��B
//-----------------------------------------------------------------------------

typedef int (WINAPI *PBF_ProgressCallback)(int, int, long);


///////////////////////////////////////////////////////////////////////////////
// PBF_QueryClientInfo (PictBear ����e����𓾂�)
//-----------------------------------------------------------------------------
// PictBear Version 1.xx �n�Ƃ̌݊��p�ł��B
// PictBear Second Edition �ł� PFilterImage/PProvidedFunc ����擾���邱�ƁB
//-----------------------------------------------------------------------------

typedef BOOL (WINAPI* PBF_QueryClientInfoProc)(DWORD, void*);

#define PBF_QUERY_VERSION		0x00	// int    : PictBear �̃o�[�W����
#define PBF_QUERY_DRAWCOLOR		0x01	// Pixel32: �`��F
#define PBF_QUERY_BKGNDCOLOR	0x03	// Pixel32: �w�i�F�ia �̒l�͕s��j
#define PBF_QUERY_FGCOLOR		0x01	// PBF_QUERY_DRAWCOLOR �Ɠ���
#define PBF_QUERY_BGCOLOR		0x03	// PBF_QUERY_BKGNDCOLOR �Ɠ���

// �ȉ��� PictBear Second Edition �ȍ~�ł̓T�|�[�g����Ă��܂���B

// PictBear Version 1.40 �ȍ~�ŃT�|�[�g(QUERY_VERSION �ł͔��f�s��)
#define PBF_QUERY_MASK			0x04	// BYTE*  : �}�X�N�̈�ւ̃|�C���^
#define PBF_QUERY_ABORT			0x05	// �p�����[�^����

// PictBear Version 1.60 �ȍ~�ŃT�|�[�g
#define PBF_QUERY_EDIT_MODE		0x06	// BYTE   : MASK==0, IMAGE==1


///////////////////////////////////////////////////////////////////////////////
// �G�N�X�|�[�g�v���O�C���̑Ή��t�H�[�}�b�g�萔
//-----------------------------------------------------------------------------
// �G�N�X�|�[�g�v���O�C���͑Ή����Ă���C���[�W�^�C�v�萔�� 
// PBE_GetSupportedType �ŕԂ��K�v������B���C���[���T�|�[�g����ɂ�
// PBF_SUPPORT_LAYER ���A���ߐF���T�|�[�g����ɂ� PBE_SUPPORT_TRANSPARENT ��
// �ݒ肵�Ă��������B
//-----------------------------------------------------------------------------

#define PBE_TYPE_MONO			0x01	// ���m�N���C���[�W
#define PBE_TYPE_GRAYSCALE		0x02	// �O���[�X�P�[��
#define PBE_TYPE_INDEXCOLOR		0x04	// �C���f�b�N�X�J���[
#define PBE_TYPE_FULLCOLOR		0x08	// �t���J���[

#define PBE_SUPPORT_LAYER		0x10	// ���C���[���T�|�[�g
#define PBE_SUPPORT_TRANSPARENT	0x20	// ���ߐF���T�|�[�g


//////////////////////////////////////////////////////////////////////////////
// ���C���[���[�h
//-----------------------------------------------------------------------------
// PBF_REFERENCE_IMAGE ���w�肵�Ď擾�����C���[�W���ێ����郌�C���[�̍������@��
// �Q�Ƃ���ɂ͈ȉ��̒萔���g�p���Ă��������B
//-----------------------------------------------------------------------------

#define LM_NORMAL		0x00	// �ʏ�
#define LM_PLUS			0x01	// ���Z
#define LM_MINUS		0x02	// ���Z
#define LM_TIMES		0x03	// ��Z
#define LM_SCREEN		0x04	// �X�N���[��
#define LM_OVERLAY		0x05	// �I�[�o�[���C
#define LM_HARDLIGHT	0x06	// �n�[�h���C�g
#define LM_SOFTLIGHT	0x07	// �\�t�g���C�g
#define LM_OVER			0x08	// �����Ă�
#define LM_FIRE			0x09	// �Ă�����
#define LM_CMPLIGHT		0x0A	// ��r(��)
#define LM_CMPDARK		0x0B	// ��r(��)
#define LM_ABS			0x0C	// ���̐�Βl
#define LM_EXCEPT		0x0D	// ���O
#define LM_VIVIDLIGHT	0x0E	// �r�r�b�g���C�g
#define LM_LINEARLIGHT	0x0F	// ���j�A���C�g
#define LM_PINLIGHT		0x10	// �s�����C�g


//////////////////////////////////////////////////////////////////////////////
// �C���[�W�^�C�v
//-----------------------------------------------------------------------------
// PFilterImage::nImageType �Ɋi�[����Ă���C���[�W�^�C�v�p�萔�ł��B
//-----------------------------------------------------------------------------

#define IT_MONO			0x01	// ���m�N��
#define IT_GRAYSCALE	0x02	// �O���[�X�P�[��
#define IT_INDEXCOLOR	0x03	// �C���f�b�N�X���[�h	
#define IT_FULLCOLOR	0x04	// �t���J���[


//////////////////////////////////////////////////////////////////////////////
// ���C���[�\����
//-----------------------------------------------------------------------------
// �C���|�[�g�v���O�C���͒��ڃA�N�Z�X�����ɕK�� PProvidedFunc �o�R�ŃA�N�Z�X��
// �Ă��������B
//-----------------------------------------------------------------------------

#pragma pack(1)

typedef struct tagPLayer
{
	int    cbSize;			// �\���̃T�C�Y

	char   szName[1024];	// ���C���[��
	int    nLayerMode;		// ���[�h(LM_???)
	int    bVisible;		// �����
	int    nOpacity;		// �s�����x
	int    nLeft;			// ���I�t�Z�b�g
	int    nTop;			// ��I�t�Z�b�g
	int    nWidth;			// ��
	int    nHeight;			// ����
	DWORD* pData;			// ���C���[�f�[�^

	BYTE   Reserved[32];	// �\��ς݃f�[�^
} PLayer;

#pragma pack()


//////////////////////////////////////////////////////////////////////////////
// �C���[�W�\����
//-----------------------------------------------------------------------------
// �C���|�[�g�v���O�C���͒��ڃA�N�Z�X�����ɕK�� PProvidedFunc �o�R�ŃA�N�Z�X��
// �Ă��������B
//-----------------------------------------------------------------------------

#pragma pack(1)

typedef struct tagPImage
{
	int     cbSize;				// �\���̃T�C�Y

	int     nImageType;			// �C���[�W�^�C�v(IT_???)
	PLayer* pLayer;				// ���C���[���f�f�[�^
	int     nLayerCount;		// ���C���[�̐� (1 �` )
	int     nWidth;				// �C���[�W�̕�
	int     nHeight;			// �C���[�W�̍���
	int     bEnableBg;			// �w�i���C���[���g�p���邩(IT_FULLCOLOR)
	int     nDPI;				// DPI
	Pixel32 rgbPalettes[256];	// �p���b�g
	int     nPaletteSize;		// �p���b�g�T�C�Y(1 �` 256)
	int     nTransparent;		// ���ߐF�ԍ�(0 �` nPaletteSize-1)
	int     bEnableTrans;		// ���ߐF���L����
	int     nCurrentLayer;		// �J�����g���C���[(0 �` nLayerCount-1)

	BYTE    Reserved[32];		// �\��ς݃f�[�^
} PImage;


///////////////////////////////////////////////////////////////////////////////
// �t�B���^�p�C���[�W�f�[�^�\����

typedef struct tagPFilterImage
{
	int     cbSize;			// �\���̃T�C�Y

	int     nWidth;			// �^�[�Q�b�g���C���[�̕�
	int     nHeight;		// �^�[�Q�b�g���C���[�̍���
	int     nTargetLayer;	// �t�B���^��K�p���郌�C���[
	DWORD*  pSrc;			// �^�[�Q�b�g���C���[�f�[�^
	DWORD*  pDst;			// ���ʏ������ݗp�f�[�^
	BOOL    bMaskMode;		// �}�X�N���[�h(false=�ʏ탂�[�h, true=�}�X�N���[�h)
	BYTE*   pMask;			// �}�X�N�f�[�^(�}�X�N���[�h���͂��ׂă[��)
	DWORD   dwMaskFlag;		// �}�X�N�̏������@(PBF_MASK_CLIENT/PBF_MASK_PLUGIN)
	RECT    rcSelection;	// �I��͈̗͂̈�(MEMO:�I�t�Z�b�g���l������***************)
	PImage* pSrcImage;		// �Q�Ɨp�C���[�W (PBF_REFERENCE_IMAGE ���w�肳��Ă��Ȃ��Ƃ��� NULL)
	LPSTR*  szArgs;			// �X�N���v�g����Ă΂ꂽ�Ƃ��̈���
	int     nArgCount;		// �����̐�(�ʏ�Ăяo������ 0)
	DWORD	dwImageType;	// �C���[�W���[�h(IT_???)

	BYTE    Reserved[32];	// �\��̈�
} PFilterImage;


//////////////////////////////////////////////////////////////////////////////
// �N���C�A���g���񋟂���֐�

typedef void  (WINAPI *PB_CreateMono)(PImage* pImage, int nWidth, int Height);
typedef void  (WINAPI *PB_CreateGrayScale)(PImage* pImage, int nWidth, int nHeight);
typedef void  (WINAPI *PB_CreateIndexColor)(PImage* pImage, int nWidth, int nHeight);
typedef void  (WINAPI *PB_CreateFullColor)(PImage* pImage, int nWidth, int nHeight, int nLayer, int bEnableBgLayer);

typedef void  (WINAPI *PB_ResizeLayer)(PImage* pImage, int nLayer, int nWidth, int nHeight, int nLeftOffset, int nTopOffset);

typedef void  (WINAPI *PB_SetData)(PImage* pImage, int nLayer, int x, int y, DWORD dwData);
typedef DWORD (WINAPI *PB_GetData)(PImage* pImage, int nLayer, int x, int y);

typedef void  (WINAPI *PB_ProgressStart)(int nMin, int nMax);
typedef int   (WINAPI *PB_ProgressSetPos)(int nPos);
typedef void  (WINAPI *PB_ProgressEnd)();

typedef void* (WINAPI *PB_Alloc)(int nSize);
typedef void  (WINAPI *PB_Free)(void* pData);

typedef BOOL  (WINAPI *PB_ChooseColor)(HWND hWnd, Pixel32* pColor);

typedef void  (WINAPI *PB_WriteInteger)(LPCSTR szSection, LPCSTR szKey, int nValue);
typedef void  (WINAPI *PB_WriteString) (LPCSTR szSection, LPCSTR szKey, LPCSTR szBuf);
typedef void  (WINAPI *PB_GetInteger)  (LPCSTR szSection, LPCSTR szKey, int* pResult, int nDefault);
typedef void  (WINAPI *PB_GetString)   (LPCSTR szSection, LPCSTR szKey, LPSTR szBuf, int nSize, LPCSTR szDefault);

typedef int   (WINAPI *PB_GetVersion)  ();
typedef void  (WINAPI *PB_GetFgColor)  (Pixel32* pColor);
typedef void  (WINAPI *PB_GetBgColor)  (Pixel32* pColor);
typedef void  (WINAPI *PB_GetLanguage) (LPSTR szLang, int nSize);


#pragma pack(1)

typedef struct tagPProvidedFunc
{
	int					    cbSize;				// �\���̃T�C�Y

	PB_CreateMono			CreateMono;			// ���m�N���C���[�W�̍쐬
	PB_CreateGrayScale		CreateGrayScale;	// �O���[�X�P�[���C���[�W�̍쐬
	PB_CreateIndexColor		CreateIndexColor;	// �C���f�b�N�X�J���[�C���[�W�̍쐬
	PB_CreateFullColor		CreateFullColor;	// �t���J���[�C���[�W�̍쐬
	PB_ResizeLayer			ResizeLayer;		// ���C���[�̃T�C�Y�ύX
	PB_SetData				SetData;			// ���C���[�Ƀf�[�^��������
	PB_GetData				GetData;			// ���C���[����f�[�^�ǂݍ���
	PB_ChooseColor			ChooseColor;		// �F�I���_�C�A���O��\��
	PB_ProgressStart		ProgressStart;		// �i�s�󋵒ʒm�J�n
	PB_ProgressSetPos		ProgressSetPos;		// �i�s�󋵒ʒm
	PB_ProgressEnd			ProgressEnd;		// �i�s�󋵒ʒm�I��
	PB_Alloc				Alloc;				// �������m��
	PB_Free					Free;				// �������J��
	PB_WriteInteger			WriteInteger;		// Plugins.ini �ɐ��������o��
	PB_WriteString			WriteString;		// Plugins.ini �ɕ����񏑂��o��
	PB_GetInteger			GetInteger;			// Plugins.ini ���琮���擾
	PB_GetString			GetString;			// Plugins.ini ���當����擾
	PB_GetVersion			GetVersion;			// �N���C�A���g�̃o�[�W�������擾
	PB_GetFgColor			GetFgColor;			// �`��F���擾
	PB_GetBgColor			GetBgColor;			// �w�i�F���擾
	PB_GetLanguage			GetLanguage;		// �g�p������擾

	BYTE					Reserved[44];		// �\��ς݃f�[�^
} PProvidedFunc;

#pragma pack()


#ifdef __cplusplus
}
#endif


#endif // __PBPLUGIN_H
//-----------------------------------------------------------------------------
