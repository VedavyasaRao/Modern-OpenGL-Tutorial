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
		COMMAND_HANDLER(IDOK2, BN_CLICKED, OnBnClickedReset)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()

	void LoadDefaults()
	{
		WCHAR buf[100];
		swprintf_s(buf, 100, L"%.*f", 2, tx);
		transx.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.*f", 2, ty);
		transy.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.*f", 2, tz);
		transz.SetWindowText(buf);

		swprintf_s(buf, 100, L"%d", rx);
		rotx.SetWindowText(buf);

		swprintf_s(buf, 100, L"%d", ry);
		roty.SetWindowText(buf);

		swprintf_s(buf, 100, L"%d", rz);
		rotz.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.*f", 2, sx);
		scalex.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.*f", 2, sy);
		scaley.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.*f", 2, sz);
		scalez.SetWindowText(buf);
	}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Do some initialization code
		transx = GetDlgItem(IDC_EDIT1);
		transy = GetDlgItem(IDC_EDIT2);
		transz = GetDlgItem(IDC_EDIT3);

		rotx = GetDlgItem(IDC_EDIT4);
		roty = GetDlgItem(IDC_EDIT5);
		rotz = GetDlgItem(IDC_EDIT6);

		scalex = GetDlgItem(IDC_EDIT7);
		scaley = GetDlgItem(IDC_EDIT8);
		scalez = GetDlgItem(IDC_EDIT9);

		LoadDefaults();
		bHandled = true;
		return 1;
	}

public:
	LRESULT OnBnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		WCHAR buf[100];
		transx.GetWindowText(buf, 100);
		if (buf[0] != 0)
			tx = _wtof(buf);

		transy.GetWindowText(buf, 100);
		if (buf[0] != 0)
			ty = _wtof(buf);

		transz.GetWindowText(buf, 100);
		if (buf[0] != 0)
			tz = _wtof(buf);

		rotx.GetWindowText(buf, 100);
		if (buf[0] != 0)
			rx = _wtol(buf);

		roty.GetWindowText(buf, 100);
		if (buf[0] != 0)
			ry = _wtol(buf);

		rotz.GetWindowText(buf, 100);
		if (buf[0] != 0)
			rz = _wtol(buf);

		scalex.GetWindowText(buf, 100);
		if (buf[0] != 0)
			sx = _wtof(buf);

		scaley.GetWindowText(buf, 100);
		if (buf[0] != 0)
			sy = _wtof(buf);

		scalez.GetWindowText(buf, 100);
		if (buf[0] != 0)
			sz = _wtof(buf);
		::PostMessage(GetParent(), WM_COMMAND, IDOK, 0);
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

	LRESULT OnBnClickedReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		tx = 0.0; ty = 0.0; tz = 0.0;
		rx = 0; ry = 0; rz = 0;
		sx = 1.0; sy = 1.0; sz = 1.0;

		LoadDefaults();
		bHandled = true;

		return 0;
	};


public:
	double tx = 0.0, ty = 0.0, tz = 0.0;
	int rx = 0, ry = 0, rz = 0;
	double sx = 1.0, sy = 1.0, sz = 1.0;

private:
	CWindow transx;
	CWindow transy;
	CWindow transz;

	CWindow rotx;
	CWindow roty;
	CWindow rotz;

	CWindow scalex;
	CWindow scaley;
	CWindow scalez;
};

