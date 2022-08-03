#pragma once
#include "atlwin.h"
#include "resource.h"

class InputDlg : public CDialogImpl<InputDlg>
{
public:
	enum { IDD = IDD_DIALOG1 };

	BEGIN_MSG_MAP(InputDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOK)
		COMMAND_HANDLER(IDOK3, BN_CLICKED, OnBnClickedOK3)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()

	void LoadDefaults()
	{
		WCHAR buf[1000];
		swprintf_s(buf, 100, L"%.*f", 2, tx);
		transx.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.*f", 2, ty);
		transy.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.*f", 2, tz);
		transz.SetWindowText(buf);

		ZeroMemory(&lf, sizeof lf);
		lf.lfWidth = 16;
		wcscpy_s(lf.lfFaceName, 32, L"Ariel");
		rgbCurrent = 0x000000FF;
	}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Do some initialization code
		sampletext = GetDlgItem(IDC_STATIC1);
		transx = GetDlgItem(IDC_EDIT1);
		transy = GetDlgItem(IDC_EDIT3);
		transz = GetDlgItem(IDC_EDIT4);
		LoadDefaults();
		bHandled = true;
		return 1;
	}

public:
	LRESULT OnBnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		WCHAR buf[1000];
		btranslate = IsDlgButtonChecked(IDC_CHECK3);
		if (btranslate)
		{
			transx.GetWindowText(buf, 100);
			if (buf[0] != 0)
				tx = _wtof(buf);

			transy.GetWindowText(buf, 100);
			if (buf[0] != 0)
				ty = _wtof(buf);

			transz.GetWindowText(buf, 100);
			if (buf[0] != 0)
				tz = _wtof(buf);
		}

		::PostMessage(GetParent(), WM_COMMAND, IDOK, 0);
		return 0;
	}

	LRESULT OnBnClickedOK3(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// Initialize CHOOSEFONT
		ZeroMemory(&cf, sizeof(cf));
		cf.lStructSize = sizeof(cf);
		cf.hwndOwner = sampletext.m_hWnd;
		cf.lpLogFont = &lf;
		cf.rgbColors = rgbCurrent;
		cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

		if (ChooseFont(&cf) == TRUE)
		{
			HDC hdc = sampletext.GetDC();
			HFONT hfont = CreateFontIndirect(cf.lpLogFont);
			SelectObject(hdc, hfont);
			rgbCurrent = cf.rgbColors;
			SetTextColor(hdc, rgbCurrent);
			sampletext.SetWindowText(L"");
			TextOutA(hdc, 0, 0, "Hellow orld!", 13);
			sampletext.ReleaseDC(hdc);
		}
			
		bHandled = true;

		return 0;
	};

	LRESULT OnBnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		::PostMessage(GetParent(), WM_COMMAND, IDCANCEL, 0);
		DestroyWindow();
		return 0;
	};



public:
	LOGFONT lf;
	COLORREF rgbCurrent;
	double tx = 0.0, ty = 0.0, tz = 0.0;
	int  btranslate;

private:
	CWindow sampletext;
	CWindow transx;
	CWindow transy;
	CWindow transz;
	CHOOSEFONT cf;

};

