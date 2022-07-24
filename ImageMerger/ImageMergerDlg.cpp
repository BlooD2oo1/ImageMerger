
// ImageMergerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ImageMerger.h"
#include "ImageMergerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageMergerDlg dialog



CImageMergerDlg::CImageMergerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEMERGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageMergerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageMergerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED( IDC_BUTTON_GO, &CImageMergerDlg::OnBnClickedButtonGo )
END_MESSAGE_MAP()


// CImageMergerDlg message handlers

BOOL CImageMergerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	GetDlgItem(IDC_EDIT_LAYERCOUNT)->SetWindowText( _T( "6" ) );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageMergerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageMergerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


class CD3D9
{
public:
	CD3D9()
	{
		m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

		UINT uAdapter = D3DADAPTER_DEFAULT;

		D3DFORMAT eAdapterFormat = D3DFMT_X8R8G8B8;

		int nAdapterModeCount = m_pD3D->GetAdapterModeCount( uAdapter, eAdapterFormat );

		if( nAdapterModeCount == 0 )
		{
			eAdapterFormat = D3DFMT_A8R8G8B8;
		}

		D3DPRESENT_PARAMETERS PresentParams;

		PresentParams.BackBufferFormat = eAdapterFormat;
		PresentParams.AutoDepthStencilFormat = D3DFMT_D24S8;
		PresentParams.BackBufferCount = 0;
		PresentParams.BackBufferWidth = 1280;
		PresentParams.BackBufferHeight = 720;
		PresentParams.EnableAutoDepthStencil = TRUE;
		PresentParams.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		PresentParams.FullScreen_RefreshRateInHz = 0;
		PresentParams.hDeviceWindow = nullptr;
		PresentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
		PresentParams.MultiSampleQuality = 0;
		PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		PresentParams.Windowed = TRUE;

		m_pD3D->CreateDevice( uAdapter, D3DDEVTYPE_HAL, PresentParams.hDeviceWindow, D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING, &PresentParams, &m_pDevice );
	}

	~CD3D9()
	{
		SAFE_RELEASE( m_pDevice );
		SAFE_RELEASE( m_pD3D );
	}

	IDirect3DDevice9*	GetDevice()	{ return m_pDevice; }

protected:
	IDirect3D9*			m_pD3D;
	IDirect3DDevice9*	m_pDevice;
};

void CImageMergerDlg::OnBnClickedButtonGo()
{	
	CString sLayerCount;
	GetDlgItem(IDC_EDIT_LAYERCOUNT)->GetWindowText( sLayerCount );

	int iLayerCount = _tstoi( sLayerCount );

	CString sExePath;
	{
		TCHAR pExePath[1024];
		GetModuleFileName( NULL, pExePath, 1023 );
		TCHAR pDrive[_MAX_DRIVE];
		TCHAR pDir[_MAX_DIR];
		TCHAR pFileName[_MAX_FNAME];
		TCHAR pExt[_MAX_EXT];
		_tsplitpath_s( pExePath, pDrive, pDir, pFileName, pExt );
		CString s = pDrive;
		s += pDir;
		SetCurrentDirectory( s );
		sExePath = s;
	}
	CFolderPickerDialog dlg;	
	dlg.m_ofn.lpstrInitialDir = sExePath;

	CString sDstFolder;
	A2Array< CString >* paFiles = new A2Array<CString>[iLayerCount];
	for ( int i = 0; i < iLayerCount; i++ )
	{
		CString sFolder;
		dlg.m_ofn.lpstrTitle = _T("Layers");
		if (dlg.DoModal() == IDOK)
		{
			sFolder = dlg.GetPathName();
		}		
		FindFiles( paFiles[i], sFolder );
	}

	dlg.m_ofn.lpstrTitle = _T("Destination");
	if (dlg.DoModal() == IDOK)
	{
		sDstFolder = dlg.GetPathName();
	}

	CD3D9 dev9;
	LPDIRECT3DDEVICE9 pDevice = dev9.GetDevice();

	struct SRGBPixel
	{
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE a;

		BYTE Blend( BYTE src, BYTE dst, BYTE srca )
		{
			float fSrc = (float)src / 255.0f;
			float fDst = (float)dst / 255.0f;
			float fSrcA = (float)srca / 255.0f;
			fDst = fDst * ( 1.0f - fSrcA ) + fSrc * fSrcA;
			fDst *= 255.0f;
			fDst = min( 255.99f, fDst );
			fDst = max( 0.0f, fDst );
			return (BYTE)fDst;
		}

		void Blend( SRGBPixel& src )
		{
			r = Blend( src.r, r, src.a );
			g = Blend( src.g, g, src.a );
			b = Blend( src.b, r, src.a );
			a = 1;
		}
	};

	int* piFileIndicesPrev = new int[iLayerCount];
	int* piFileIndices = new int[iLayerCount];
	int* piDiv = new int[iLayerCount];
	int iOutputCount = 1;
	LPDIRECT3DTEXTURE9* ppTex = new LPDIRECT3DTEXTURE9[iLayerCount];
	D3DSURFACE_DESC* pSurfDesc = new D3DSURFACE_DESC[iLayerCount];
	D3DLOCKED_RECT* pLockedRect = new D3DLOCKED_RECT[iLayerCount];

	for ( int iLayerInd = 0; iLayerInd < iLayerCount; iLayerInd++ )
	{
		ppTex[iLayerInd] = nullptr;
		piFileIndicesPrev[iLayerInd] = -1;
		piFileIndices[iLayerInd] = 0;
		iOutputCount *= paFiles[iLayerInd].GetCount();
		if ( iLayerInd == 0 ) 
		{
			piDiv[iLayerInd] = 1;
		}
		else
		{
			piDiv[iLayerInd] = piDiv[iLayerInd-1] * paFiles[iLayerInd-1].GetCount();
		}
	}

	DWORD dwWidth = 0;
	DWORD dwHeight = 0;

	for ( int iOutputInd = 0; iOutputInd < iOutputCount; iOutputInd++ )
	{		
		{
			int iTemp = iOutputInd;
			for ( int iLayerInd = 0; iLayerInd < iLayerCount; iLayerInd++ )
			{	
				piFileIndices[iLayerInd] = iTemp / piDiv[iLayerCount-iLayerInd-1];
				iTemp -= piFileIndices[iLayerInd] * piDiv[iLayerCount-iLayerInd-1];			
			}
		}

		for ( int iLayerInd = 0; iLayerInd < iLayerCount; iLayerInd++ )
		{
			if ( piFileIndices[iLayerCount-iLayerInd-1] != piFileIndicesPrev[iLayerCount-iLayerInd-1] )
			{
				if ( ppTex[iLayerInd] )
				{
					ppTex[iLayerInd]->UnlockRect(0);	
					SAFE_RELEASE( ppTex[iLayerInd] );
				}
				HRESULT hr = D3DXCreateTextureFromFileEx( pDevice, paFiles[iLayerInd][piFileIndices[iLayerCount-iLayerInd-1]], D3DX_DEFAULT, D3DX_DEFAULT, 1, D3DUSAGE_DYNAMIC, D3DFMT_FROM_FILE, D3DPOOL_SYSTEMMEM, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &ppTex[iLayerInd] );
				if ( FAILED(hr) )
				{
					continue;
				}
				ppTex[iLayerInd]->GetLevelDesc(0, &pSurfDesc[iLayerInd]);
				ppTex[iLayerInd]->LockRect(0, &pLockedRect[iLayerInd], NULL, 0);
				if ( iOutputInd == 0 )
				{
					dwWidth = max( dwWidth, pSurfDesc[iLayerInd].Width );
					dwHeight = max( dwHeight, pSurfDesc[iLayerInd].Height );
				}
			}
		}

		LPDIRECT3DTEXTURE9 pTexOut = nullptr;
		D3DXCreateTexture( pDevice, dwWidth, dwHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTexOut );
		D3DLOCKED_RECT sLockedRectOut;
		pTexOut->LockRect(0, &sLockedRectOut, NULL, 0);
		for ( int iLayerInd = 0; iLayerInd < iLayerCount; iLayerInd++ )
		{
			SRGBPixel* pPixelsSrc = (SRGBPixel*)pLockedRect[iLayerInd].pBits;
			SRGBPixel* pPixelsDst = (SRGBPixel*)sLockedRectOut.pBits;
			for ( DWORD y = 0; y < dwHeight; y++ )
			for ( DWORD x = 0; x < dwWidth; x++ )
			{
				DWORD dwInd = y*dwWidth+x;
				pPixelsDst[dwInd].Blend( pPixelsSrc[dwInd] );
			}
		}

		pTexOut->UnlockRect(0);

		CString sFile = sDstFolder;
		sFile += _T( "\\" );
		for ( int iLayerInd = 0; iLayerInd < iLayerCount; iLayerInd++ )
		{
			CString s;
			s.Format( _T( "%4.4d_" ), piFileIndices[iLayerInd] );
			sFile += s;
		}
		sFile += _T( ".png" );
		D3DXSaveTextureToFile( sFile, D3DXIFF_PNG, pTexOut, nullptr );
		SAFE_RELEASE( pTexOut );

		for ( int iLayerInd = 0; iLayerInd < iLayerCount; iLayerInd++ )
		{	
			piFileIndicesPrev[iLayerInd] = piFileIndices[iLayerInd];
		}

// 		{
// 			int iOutputIndCheck = 0;
// 			for ( int iLayerInd = 0; iLayerInd < iLayerCount; iLayerInd++ )
// 			{
// 				iOutputIndCheck += piFileIndices[iLayerInd] * piDiv[iLayerCount-iLayerInd-1];
// 			}
// 			if ( iOutputIndCheck != iOutputInd )
// 			{
// 				int szar = 0;
// 				szar++;
// 			}
// 		}
	}

	for ( int iLayerInd = 0; iLayerInd < iLayerCount; iLayerInd++ )
	{	
		if ( ppTex[iLayerInd] )
		{
			ppTex[iLayerInd]->UnlockRect(0);	
			SAFE_RELEASE( ppTex[iLayerInd] );
		}
	}

	SAFE_DELETE_ARRAY( pLockedRect );
	SAFE_DELETE_ARRAY( pSurfDesc );
	SAFE_DELETE_ARRAY( ppTex );
	SAFE_DELETE_ARRAY( paFiles );
	SAFE_DELETE_ARRAY( piFileIndices );

}
