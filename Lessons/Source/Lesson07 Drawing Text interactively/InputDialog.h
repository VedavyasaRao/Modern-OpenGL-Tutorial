#pragma once
#include "atlwin.h"
#include <string>
#include "resource.h"

DWORD WINAPI ThreadFunction2(LPVOID lpParam);

class InputDlg : public CDialogImpl<InputDlg>
{
public:
	enum { IDD = IDD_DIALOG1 };

	BEGIN_MSG_MAP(InputDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDBTNRESIZE, BN_CLICKED, OnBnClickedResize)
		COMMAND_HANDLER(IDBTNFONT, BN_CLICKED, OnBnClickedChangeFont)
		COMMAND_HANDLER(IDBTNIMAGE, BN_CLICKED, OnBnClickedLoadImage)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDC_BTNDRAW, BN_CLICKED, OnBnClickedBtndraw)
		COMMAND_HANDLER(IDC_BTNRESET, BN_CLICKED, OnBnClickedBtnreset)
	END_MSG_MAP()

	void LoadDefaultValues()
	{
		wd = 256;
		ht = 256;
		txtx = 5;
		txty = 5;
		text = L"ಖ್ರಿಷಾ Rao👸\r\nII standard\r\nBaldwin Girls school\r\nBengaluru";
		ZeroMemory(&lf, sizeof lf);
		lf.lfHeight = -27;
		lf.lfWeight = 400;
		lf.lfClipPrecision = 2;
		lf.lfOutPrecision = 3;
		lf.lfQuality= 1;
		lf.lfPitchAndFamily= 34;

		wcscpy_s(lf.lfFaceName, 32, L"Segoe UI Emoji");
		hf = CreateFontIndirectW(&lf);
		rgbCurrent = 0x00ff00ff;
		Color gdipColor;
		gdipColor.SetFromCOLORREF(rgbCurrent);
		pbrush = new SolidBrush(gdipColor);

		imgx = 180;
		imgy = 5;
		imgclipwd = 48;
		imgclipht = 80;
		imagfilename = L"..\\resources\\textures\\Khri$ha2.jpg";
	}

	void getdata()
	{
		int sz = 200;
		wstring buffer(sz, 0);
		wdctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		wd = stoi(buffer);

		htctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		ht = stoi(buffer);

		sampletextctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		text = buffer;

		txtxctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		txtx = stof(buffer);
		txtyctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		txty = stof(buffer);


		imgxctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		imgx = stoi(buffer);
		imgyctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		imgy = stoi(buffer);
		imgclipwdctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		imgclipwd = stoi(buffer);
		imgcliphtctl.GetWindowTextW((LPTSTR)buffer.data(), sz);
		imgclipht = stoi(buffer);
	}

	void LoadDefaults()
	{
		LoadDefaultValues();

		wdctl.SetWindowText(to_wstring(wd).c_str());
		htctl.SetWindowText(to_wstring(ht).c_str());

		sampletextctl.SetWindowText(text.c_str());
		txtxctl.SetWindowText(to_wstring((int)txtx).c_str());
		txtyctl.SetWindowText(to_wstring((int)txty).c_str());

		imgxctl.SetWindowText(to_wstring(imgx).c_str());
		imgyctl.SetWindowText(to_wstring(imgy).c_str());
		imgclipwdctl.SetWindowText(to_wstring(imgclipwd).c_str());
		imgcliphtctl.SetWindowText(to_wstring(imgclipht).c_str());
	}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		wdctl = GetDlgItem(IDC_EDITWD);
		htctl = GetDlgItem(IDC_EDITHT);

		txtxctl = GetDlgItem(IDC_EDITTEXTX);
		txtyctl = GetDlgItem(IDC_EDITTEXTY);
		sampletextctl = GetDlgItem(IDC_EDITTEXT);

		imgxctl = GetDlgItem(IDC_EDITIMAGEX);
		imgyctl = GetDlgItem(IDC_EDITIMAGEY);
		picturectl = GetDlgItem(IDC_PICBOX);
		imgclipwdctl = GetDlgItem(IDC_EDITCLIPWD);
		imgcliphtctl = GetDlgItem(IDC_EDITCLIPHT);

		LoadDefaults();
		CreateThread(NULL, 0, ThreadFunction2, this, 0, NULL);

		bHandled = true;
		return 1;
	}

public:
	LRESULT OnBnClickedChangeFont(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// Initialize CHOOSEFONT
		ZeroMemory(&cf, sizeof(cf));
		cf.lStructSize = sizeof(cf);
		cf.hwndOwner = sampletextctl.m_hWnd;
		cf.lpLogFont = &lf;
		cf.rgbColors = rgbCurrent;
		cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;

		if (ChooseFont(&cf) == TRUE)
		{
			hf = CreateFontIndirectW(&lf);
			rgbCurrent = cf.rgbColors;
			Color gdipColor(255, 0, 0, 255);
			gdipColor.SetFromCOLORREF(rgbCurrent);
			pbrush = new SolidBrush(gdipColor);
			getdata();
			::PostMessage(GetParent(), WM_COMMAND, IDOK, 1);
		}
		bHandled = true;
		return 0;
	};


	void LoadImage()
	{
		Bitmap* sourceBmp = Bitmap::FromFile(imagfilename.c_str());
		RECT rect;
		picturectl.GetClientRect(&rect);
		int targetWidth = rect.right - rect.left;
		int targetHeight = rect.bottom - rect.top;
		Bitmap* targetBmp = new Bitmap(targetWidth, targetHeight, sourceBmp->GetPixelFormat());
		Graphics* graphics = Graphics::FromImage(targetBmp);
		graphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);
		graphics->DrawImage(sourceBmp, 0, 0, targetWidth, targetHeight);

		auto htwd = 64;
		BitmapData bdata;
		sourceBmp->LockBits(&Rect(0, 0, htwd, htwd), ImageLockModeRead, PixelFormat32bppARGB, &bdata);
		targetBmp->LockBits(&Rect(0, 0, htwd, htwd), ImageLockModeRead, PixelFormat32bppARGB, &bdata);

		delete graphics;
		delete sourceBmp;
		HBITMAP hBmp = NULL;
		Status status = targetBmp->GetHBITMAP(Color(255, 255, 255), &hBmp); // Using white background
		SendDlgItemMessage(IDC_PICBOX, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
		DeleteObject(hBmp);
	}

	LRESULT OnBnClickedLoadImage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		OPENFILENAME ofn;       // common dialog box structure
		wchar_t szFile[260];       // buffer for file name
		HWND hwnd = m_hWnd;              // owner window

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Display the Open dialog box. 
		if (GetOpenFileName(&ofn) == TRUE)
		{
			imagfilename = ofn.lpstrFile;
			LoadImage();
			getdata();
			::PostMessage(GetParent(), WM_COMMAND, IDOK, 1);
		}
		return 0;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = true;
		::PostMessage(GetParent(), WM_COMMAND, IDCANCEL, 0);
		DestroyWindow();

		return 0;
	}

	LRESULT OnBnClickedResize(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		getdata();
		::PostMessage(GetParent(), WM_COMMAND, IDOK, 0);
		return 0;
	}


	LRESULT OnBnClickedBtndraw(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		bHandled = true;
		getdata();
		::PostMessage(GetParent(), WM_COMMAND, IDOK, 1);
		return 0;
	}

	LRESULT OnBnClickedBtnreset(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		bHandled = true;
		LoadDefaults();
		::PostMessage(GetParent(), WM_COMMAND, IDOK, 0);
		return 0;
	}


public:
	int wd,ht;

	HFONT	hf;
	LOGFONT lf;
	COLORREF rgbCurrent;
	wstring text;
	float txtx, txty;
	StringFormat fmt;
	SolidBrush *pbrush;

	wstring imagfilename;
	int imgx, imgy;
	int  imgclipwd, imgclipht;

private:

	CWindow wdctl;
	CWindow htctl;

	CWindow sampletextctl;
	CWindow txtxctl;
	CWindow txtyctl;
	
	CWindow picturectl;
	CWindow imgxctl;
	CWindow imgyctl;
	CWindow imgclipwdctl;
	CWindow imgcliphtctl;
	
	CHOOSEFONT cf;
};

DWORD WINAPI ThreadFunction2(LPVOID lpParam)
{
	InputDlg* pdlg = (InputDlg*)lpParam;

	::Sleep(2000);
	pdlg->LoadImage();
	return 0;
}
