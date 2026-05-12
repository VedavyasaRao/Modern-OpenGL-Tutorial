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
		WCHAR buf[1000];
		swprintf_s(buf, 100, L"%.02f, %.02f, %.02f", positionvec.x, positionvec.y, positionvec.z);
		position.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.02f, %.02f, %.02f", targetvec.x, targetvec.y, targetvec.z);
		target.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.02f, %.02f, %.02f", upvec.x, upvec.y, upvec.z);
		up.SetWindowText(buf);

		swprintf_s(buf, 100, L"%0.2f", fovflt);
		FOV.SetWindowText(buf);

		swprintf_s(buf, 100, L"%0.2f", nearflt);
		nearplane.SetWindowText(buf);

		swprintf_s(buf, 100, L"%0.2f", farflt);
		farplane.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.02f, %.02f", xminmaxvec.x, xminmaxvec.y);
		xminmax.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.02f, %.02f", yminmaxvec.x, yminmaxvec.y);
		yminmax.SetWindowText(buf);

		swprintf_s(buf, 100, L"%.02f, %.02f", zminmaxvec.x, zminmaxvec.y);
		zminmax.SetWindowText(buf);
	
	}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Do some initialization code
		position = GetDlgItem(IDC_EDIT1);
		target = GetDlgItem(IDC_EDIT2);
		up = GetDlgItem(IDC_EDIT3);

		FOV = GetDlgItem(IDC_EDIT4);
		nearplane = GetDlgItem(IDC_EDIT5);
		farplane = GetDlgItem(IDC_EDIT6);

		xminmax = GetDlgItem(IDC_EDIT7);
		yminmax = GetDlgItem(IDC_EDIT8);
		zminmax = GetDlgItem(IDC_EDIT9);

		LoadDefaults();
		bHandled = true;
		return 1;
	}

public:
	LRESULT OnBnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		WCHAR buf[1000];
		blookat = IsDlgButtonChecked(IDC_CHECK2);
		bperspective = IsDlgButtonChecked(IDC_CHECK3);
		bortho = IsDlgButtonChecked(IDC_CHECK4);

		if (blookat)
		{
			position.GetWindowText(buf, 100);
			if (buf[0] != 0)
				swscanf_s(buf, L"%f,%f,%f", &positionvec.x, &positionvec.y, &positionvec.z);

			target.GetWindowText(buf, 100);
			if (buf[0] != 0)
				swscanf_s(buf, L"%f,%f,%f", &targetvec.x, &targetvec.y, &targetvec.z);

			up.GetWindowText(buf, 100);
			if (buf[0] != 0)
				swscanf_s(buf, L"%f,%f,%f", &upvec.x, &upvec.y, &upvec.z);
		}

		if (bperspective)
		{
			FOV.GetWindowText(buf, 100);
			if (buf[0] != 0)
				fovflt = (float)_wtof(buf);

			nearplane.GetWindowText(buf, 100);
			if (buf[0] != 0)
				nearflt = (float)_wtof(buf);

			farplane.GetWindowText(buf, 100);
			if (buf[0] != 0)
				farflt = (float)_wtof(buf);
		}
	
		if (bortho)
		{
			xminmax.GetWindowText(buf, 100);
			if (buf[0] != 0)
				swscanf_s(buf, L"%f,%f", &xminmaxvec.x, &xminmaxvec.y);

			yminmax.GetWindowText(buf, 100);
			if (buf[0] != 0)
				swscanf_s(buf, L"%f,%f", &yminmaxvec.x, &yminmaxvec.y);

			zminmax.GetWindowText(buf, 100);
			if (buf[0] != 0)
				swscanf_s(buf, L"%f,%f", &zminmaxvec.x, &zminmaxvec.y);
		}

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
		positionvec = { 0.0, 0.0, 5.0 }; targetvec = { 0.0, 0.0, 0.0 }; upvec = { 0.0, 1.0, 0.0 };
		float fovflt = 45.0, nearflt = 1.0, farflt = 100.0;
		xminmaxvec = { -aspectratio, aspectratio };  yminmaxvec = { -1.0, 1.0 }; zminmaxvec = { 1.0, 100.0 };
		
		LoadDefaults();
		bHandled = true;

		return 0;
};


public:
	vec3 positionvec{ 0.0, 0.0, 5.0 }, targetvec{ 0.0, 0.0, 0.0 }, upvec{ 0.0, 1.0, 0.0 };
	float fovflt = 45.0, nearflt = 1.0, farflt = 100.0, aspectratio = 1.0;
	vec2 xminmaxvec{ -aspectratio, aspectratio }, yminmaxvec{ -1.0, 1.0 }, zminmaxvec{ 1.0, 100.0 };
	BOOL blookat, bperspective, bortho;
		
private:
	CWindow position;
	CWindow target;
	CWindow up;

	CWindow FOV;
	CWindow nearplane;
	CWindow farplane;

	CWindow xminmax;
	CWindow yminmax;
	CWindow zminmax;
};

