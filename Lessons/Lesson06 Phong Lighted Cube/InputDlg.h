#pragma once
#include "atlwin.h"
#include "resource.h"
#include <string>

using namespace std;

class InputDlg : public ATL::CDialogImpl<InputDlg>
{
public:
	enum { IDD = IDD_DIALOG1 };

	BEGIN_MSG_MAP(InputDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BUTTONCLRPKRAMB, BN_CLICKED, PickColor)
		COMMAND_HANDLER(IDC_BUTTONCLRPKRDIF, BN_CLICKED, PickColor)
		COMMAND_HANDLER(IDC_BUTTONCLRPKRSPC, BN_CLICKED, PickColor)
		COMMAND_HANDLER(IDReset, BN_CLICKED, OnBnClickedReset)
		COMMAND_HANDLER(IDAPPLY, BN_CLICKED, OnBnClickedApply)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Do some initialization code
		cposxctl = GetDlgItem(IDC_EDITCAMPOSX);
		cposyctl = GetDlgItem(IDC_EDITCAMPOSY);
		cposzctl = GetDlgItem(IDC_EDITCAMPOSZ);

		lposxctl = GetDlgItem(IDC_EDITLIGHTPOSX);
		lposyctl = GetDlgItem(IDC_EDITLIGHTPOSY);
		lposzctl = GetDlgItem(IDC_EDITLIGHTPOSZ);

		lambctl = GetDlgItem(IDC_EDITLIGHTAMB);
		ldifctl = GetDlgItem(IDC_EDITLIGHTDIF);
		lspcctl = GetDlgItem(IDC_EDITLIGHTSPC);

		mambctl = GetDlgItem(IDC_EDITMATAMB);
		mdifctl = GetDlgItem(IDC_EDITMATDIF);
		mspcctl = GetDlgItem(IDC_EDITMATSPC);
		mshctl = GetDlgItem(IDC_EDITMATSH);

		lattctl = GetDlgItem(IDC_EDITATT);
		lblinnctl = GetDlgItem(IDC_CHECKBLINN);

		LoadDefaults();
		bHandled = true;
		return 1;
	}


	void update(PhongLightingUtil* putl)
	{
		putl->light.ambientCoefficient = stof(lamb);
		putl->light.diffuseCoefficient = stof(ldif);
		putl->light.specularCoefficient = stof(lspc);
		putl->light.attenuation = stof(latt);
		putl->light.position = vec3(stof(lposx), stof(lposy), stof(lposz));
		putl->light.blinn = lblinn;
		auto clr = stoul(mamb);
		putl->material.ambientColor = vec3(GetRValue(clr), GetGValue(clr), GetBValue(clr));
		clr = stoul(mdif);
		putl->material.diffuseColor = vec3(GetRValue(clr), GetGValue(clr), GetBValue(clr));
		clr = stoul(mspc);
		putl->material.specularColor = vec3(GetRValue(clr), GetGValue(clr), GetBValue(clr));
		putl->material.Shininess = stof(msh);

		putl->viewerPosition = vec3(stof(cposx), stof(cposy), stof(cposz));
	}


private:
	LRESULT OnBnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		getdata();
		::PostMessage(GetParent(), WM_COMMAND, IDOK, 0);
		return 0;
	}

	LRESULT OnBnClickedReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		LoadDefaults();
		return 0;
	};

	LRESULT OnBnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		::PostMessage(GetParent(), WM_COMMAND, IDCANCEL, 0);
		DestroyWindow();
		return 0;
	};

	void LoadDefaults()
	{
		cposx = L"2.0";
		cposxctl.SetWindowText(cposx.c_str());

		cposy = L"0.0";
		cposyctl.SetWindowText(cposy.c_str());

		cposz = L"-4.0";
		cposzctl.SetWindowText(cposz.c_str());

		lposx = L"0.0";
		lposxctl.SetWindowText(lposx.c_str());

		lposy = L"3.0";
		lposyctl.SetWindowText(lposy.c_str());

		lposz = L"-9.0";
		lposzctl.SetWindowText(lposz.c_str());


		lamb = L"0.005";
		lambctl.SetWindowText(lamb.c_str());

		ldif = L"0.005";
		ldifctl.SetWindowText(ldif.c_str());

		lspc = L"0.8";
		lspcctl.SetWindowText(lspc.c_str());

		latt = L"0.02";
		lattctl.SetWindowText(latt.c_str());

		lblinn = true;
		CheckDlgButton(IDC_CHECKBLINN, lblinn?BST_CHECKED: BST_UNCHECKED);

		mamb = L"6452";
		mambctl.SetWindowText(mamb.c_str());

		mdif = L"26316";
		mdifctl.SetWindowText(mdif.c_str());

		mspc = L"16777215";
		mspcctl.SetWindowText(mspc.c_str());

		msh = L"3000.0";
		mshctl.SetWindowText(msh.c_str());
	}

	LRESULT PickColor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = True;

		CHOOSECOLOR cc;                 // common dialog box structure 
		static COLORREF acrCustClr[16]; // array of custom colors 
		static DWORD rgbCurrent;        // initial color selection

		wstring ret(100, 0);
		if (wID == IDC_BUTTONCLRPKRAMB)
			mambctl.GetWindowTextW((LPTSTR)ret.data(), 100);
		else if (wID == IDC_BUTTONCLRPKRDIF)
			mdifctl.GetWindowTextW((LPTSTR)ret.data(), 100);
		else if (wID == IDC_BUTTONCLRPKRSPC)
			mspcctl.GetWindowTextW((LPTSTR)ret.data(), 100);


		if (!ret.empty())
		{
			try
			{
				auto clr = stoul(ret);
				if (clr != 0)
					rgbCurrent = clr;
			}
			catch(...) {}
		}

		// Initialize CHOOSECOLOR
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = NULL;
		cc.lpCustColors = (LPDWORD)acrCustClr;
		cc.rgbResult = rgbCurrent;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;

		if (ChooseColor(&cc) == TRUE)
		{
			rgbCurrent = cc.rgbResult; // User selected color

			// Use GetRValue(rgbCurrent), GetGValue(rgbCurrent), etc.
			ret = to_wstring(rgbCurrent);
			if (wID == IDC_BUTTONCLRPKRAMB)
				mambctl.SetWindowTextW(ret.c_str());
			else if (wID == IDC_BUTTONCLRPKRDIF)
				mdifctl.SetWindowTextW(ret.c_str());
			else if (wID == IDC_BUTTONCLRPKRSPC)
				mspcctl.SetWindowTextW(ret.c_str());
		}
		return 0;

	}


	void getdata()
	{
		cposxctl.GetWindowTextW((LPTSTR)cposx.data(), 100);
		cposyctl.GetWindowTextW((LPTSTR)cposy.data(), 100);
		cposzctl.GetWindowTextW((LPTSTR)cposz.data(), 100);

		lposxctl.GetWindowTextW((LPTSTR)lposx.data(), 100);
		lposyctl.GetWindowTextW((LPTSTR)lposy.data(), 100);
		lposzctl.GetWindowTextW((LPTSTR)lposz.data(), 100);


		lambctl.GetWindowTextW((LPTSTR)lamb.data(), 100);
		ldifctl.GetWindowTextW((LPTSTR)ldif.data(), 100);
		lspcctl.GetWindowTextW((LPTSTR)lspc.data(), 100);

		lattctl.GetWindowTextW((LPTSTR)latt.data(), 100);
		lblinn = (IsDlgButtonChecked(IDC_CHECKBLINN) == BST_CHECKED);

		mambctl.GetWindowTextW((LPTSTR)mamb.data(), 100);
		mdifctl.GetWindowTextW((LPTSTR)mdif.data(), 100);
		mspcctl.GetWindowTextW((LPTSTR)mspc.data(), 100);
		mshctl.GetWindowTextW((LPTSTR)msh.data(), 100);
	}

public:
	wstring  cposx{ 100,0 }, cposy{ 100,0 }, cposz{ 100,0 };

	wstring lposx{ 100,0 }, lposy{ 100,0 },lposz{ 100,0 };

	wstring lamb{ 100,0 };
	wstring ldif{ 100,0 };
	wstring lspc{ 100,0 };
	wstring latt{ 100,0 };

	wstring mamb{ 100,0 };
	wstring mdif{ 100,0 };
	wstring mspc{ 100,0 };
	wstring msh{ 100,0 };
	bool lblinn=true;

private:
	CWindow cposxctl;
	CWindow cposyctl;
	CWindow cposzctl;

	CWindow lposxctl;
	CWindow lposyctl;
	CWindow lposzctl;

	CWindow lambctl;
	CWindow ldifctl;
	CWindow lspcctl;

	CWindow lattctl;
	CWindow lblinnctl;

	CWindow mambctl;
	CWindow mdifctl;
	CWindow mspcctl;
	CWindow mshctl;
};

