#pragma once
#include "atlwin.h"
#include "resource.h"
#include <string>
#include <sstream>

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
		COMMAND_HANDLER(IDC_EDITLGTAMBPKR, BN_CLICKED, PickColor)
		COMMAND_HANDLER(IDC_EDITLGTDIFPKR, BN_CLICKED, PickColor)
		COMMAND_HANDLER(IDC_EDITLGTSPCPKR, BN_CLICKED, PickColor)
		COMMAND_HANDLER(IDReset, BN_CLICKED, OnBnClickedReset)
		COMMAND_HANDLER(IDAPPLY, BN_CLICKED, OnBnClickedApply)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		COMMAND_HANDLER(IDC_COMBO_LIGHTSRC, CBN_SELCHANGE, OnCbnSelchangeComboLightsrc)
	END_MSG_MAP()

	LRESULT InputDlg::OnCbnSelchangeComboLightsrc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// TODO: Add your control notification handler code here
		cursel = lightsrctypecmbctl.SendMessageW(CB_GETCURSEL, 0, 0);
		disablecontrols();
		enablecontrols();
		SetDefaultValues();
		PopulateGUI();
		return 0;
	}

	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO: Add your message handler code here and/or call default
		bHandled = TRUE;

		if ((HWND)lParam == lambclrctl.m_hWnd)
			SetBkColor((HDC)wParam, lambclr);
		else if ((HWND)lParam == ldifclrctl.m_hWnd)
			SetBkColor((HDC)wParam, ldifclr);
		else if ((HWND)lParam == lspcclrctl.m_hWnd)
			SetBkColor((HDC)wParam, lspcclr);

		else if ((HWND)lParam == mambctl.m_hWnd)
			SetBkColor((HDC)wParam, mamb);
		else if ((HWND)lParam == mdifctl.m_hWnd)
			SetBkColor((HDC)wParam, mdif);
		else if ((HWND)lParam == mspcctl.m_hWnd)
			SetBkColor((HDC)wParam, mspc);
		else
			return 0;
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}

	// Call base class handler for other controls
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

		lambclrctl = GetDlgItem(IDC_EDITLGTAMB);
		ldifclrctl = GetDlgItem(IDC_EDITLGTDIF);
		lspcclrctl = GetDlgItem(IDC_EDITLGTSPC);

		mambctl = GetDlgItem(IDC_EDITMATAMB);
		mdifctl = GetDlgItem(IDC_EDITMATDIF);
		mspcctl = GetDlgItem(IDC_EDITMATSPC);

		mshctl = GetDlgItem(IDC_EDITMATSH);
		lblinnctl = GetDlgItem(IDC_CHECKBLINN);

		ldirxctl = GetDlgItem(IDC_EDITLIGHTDIRX);
		ldiryctl = GetDlgItem(IDC_EDITLIGHTDIRY);
		ldirzctl = GetDlgItem(IDC_EDITLIGHTDIRZ);

		lattcctl = GetDlgItem(IDC_EDITLIGHTATTC);
		lattlctl = GetDlgItem(IDC_EDITLIGHTATTL);
		lattqctl = GetDlgItem(IDC_EDITLIGHTATTQ);

		lsptinnerctl = GetDlgItem(IDC_EDITLIGHTSPOTINNER);
		lsptouterctl = GetDlgItem(IDC_EDITLIGHTSPOTOUTER);

		lightsrctypecmbctl = GetDlgItem(IDC_COMBO_LIGHTSRC);

		lightsrctypecmbctl.SendMessageW(CB_ADDSTRING, 0, (LPARAM)lighttypestrings[0].c_str());
		lightsrctypecmbctl.SendMessageW(CB_ADDSTRING, 0, (LPARAM)lighttypestrings[1].c_str());
		lightsrctypecmbctl.SendMessageW(CB_ADDSTRING, 0, (LPARAM)lighttypestrings[2].c_str());
		lightsrctypecmbctl.SendMessageW(CB_ADDSTRING, 0, (LPARAM)lighttypestrings[3].c_str());
		
		RECT rect;
		lightsrctypecmbctl.GetWindowRect(&rect);
		::MapWindowPoints(nullptr, this->m_hWnd, (LPPOINT)&rect, 2);
		lightsrctypecmbctl.MoveWindow( rect.left, rect.top, rect.right - rect.left, 200, TRUE);

		lightsrctypecmbctl.SendMessageW(CB_SETCURSEL, cursel, 0);
		OnCbnSelchangeComboLightsrc(0, 0, 0, bHandled);
		bHandled = true;
		return 1;
	}


	void update(LightingUtil* putl)
	{
		getdata();
		updatevalues(putl->lightsrc, putl->mat);
	}

	void updatevalues(LightSrcInfo& lightsrc, MaterialInfo& mat)
	{
		lightsrc.src = (LightSourceType)(cursel+1);
		lightsrc.position = vec3(stof(lposx), stof(lposy), stof(lposz));
		lightsrc.direction = vec3(stof(ldirx), stof(ldiry), stof(ldirz));
		lightsrc.viewerPosition = vec3(stof(cposx), stof(cposy), stof(cposz));

		lightsrc.ambientCoefficient = stof(lamb);
		lightsrc.diffuseCoefficient = stof(ldif);
		lightsrc.specularCoefficient = stof(lspc);

		lightsrc.ambientColor = vec3(GetRValue(lambclr), GetGValue(lambclr), GetBValue(lambclr));
		lightsrc.diffuseColor = vec3(GetRValue(ldifclr), GetGValue(ldifclr), GetBValue(ldifclr));
		lightsrc.specularColor = vec3(GetRValue(lspcclr), GetGValue(lspcclr), GetBValue(lspcclr));

		lightsrc.attconstant = stof(lattc);
		lightsrc.attlinear = stof(lattl);
		lightsrc.attquadratic = stof(lattq);

		lightsrc.spotlightinner = stof(lsptinner);
		lightsrc.spotlightouter = stof(lsptouter);

		lightsrc.blinn = lblinn;

		mat.ambientColor = vec3(GetRValue(mamb), GetGValue(mamb), GetBValue(mamb));
		mat.diffuseColor = vec3(GetRValue(mdif), GetGValue(mdif), GetBValue(mdif));
		mat.specularColor = vec3(GetRValue(mspc), GetGValue(mspc), GetBValue(mspc));

		mat.Shininess = stof(msh);
	}

	void disablecontrols()
	{
		lposxctl.EnableWindow(FALSE);
		lposyctl.EnableWindow(FALSE);
		lposzctl.EnableWindow(FALSE);

		ldirxctl.EnableWindow(FALSE);
		ldiryctl.EnableWindow(FALSE);
		ldirzctl.EnableWindow(FALSE);

		lposxctl.EnableWindow(FALSE);
		lposyctl.EnableWindow(FALSE);
		lposzctl.EnableWindow(FALSE);
		lattcctl.EnableWindow(FALSE);;
		lattlctl.EnableWindow(FALSE);;
		lattqctl.EnableWindow(FALSE);;

		lsptinnerctl.EnableWindow(FALSE);;
		lsptouterctl.EnableWindow(FALSE);;

	}

	void enablecontrols()
	{
		auto src  = (LightSourceType)cursel+1;

		if (src == LightSourceType::Basic)
		{
			lposxctl.EnableWindow();
			lposyctl.EnableWindow();
			lposzctl.EnableWindow();
		}
		else if (src == LightSourceType::Directional)
		{
			ldirxctl.EnableWindow();
			ldiryctl.EnableWindow();
			ldirzctl.EnableWindow();
		}
		else if (src == LightSourceType::Point)
		{
			lposxctl.EnableWindow();
			lposyctl.EnableWindow();
			lposzctl.EnableWindow();
			lattcctl.EnableWindow();;
			lattlctl.EnableWindow();;
			lattqctl.EnableWindow();;
		}
		else if (src == LightSourceType::Spot)
		{
			lposxctl.EnableWindow();
			lposyctl.EnableWindow();
			lposzctl.EnableWindow();

			ldirxctl.EnableWindow();
			ldiryctl.EnableWindow();
			ldirzctl.EnableWindow();

			lposxctl.EnableWindow();
			lposyctl.EnableWindow();
			lposzctl.EnableWindow();
			lattcctl.EnableWindow();;
			lattlctl.EnableWindow();;
			lattqctl.EnableWindow();;

			lsptinnerctl.EnableWindow();;
			lsptouterctl.EnableWindow();;
		}
	}

	LRESULT OnBnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		::PostMessage(GetParent(), WM_COMMAND, IDOK, 0);
		return 0;
	}

	LRESULT OnBnClickedReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		SetDefaultValues();
		PopulateGUI();
		return 0;
	};

	LRESULT OnBnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		::PostMessage(GetParent(), WM_COMMAND, IDCANCEL, 0);
		DestroyWindow();
		return 0;
	};

	void SetDefaultValues()
	{
		cposx = L"2.0";
		cposy = L"0.0";
		cposz = L"-4.0";

		lposx = L"0.0";
		lposy = L"3.0";
		lposz = (cursel < LightSourceType::Point) ? L"-9.0" : L"0.0";

		lamb = L"0.005";
		ldif = L"0.005";
		lspc = L"0.8";

		lblinn = true;

		mamb = 6452;
		mdif = 26316;
		mspc = 16777215;

		lambclr = 65793;
		ldifclr = 16777215;
		lspcclr = 16777215;

		ldirx = L"0.0";
		ldiry = L"-1.0";
		ldirz = L"0.0";

		lattc = L"1.0";
		lattl = L"0.07";
		lattq = L"0.017";

		lsptinner = L"15.0";
		lsptouter = L"20.0";

		msh = L"9000.0";
	}

	void PopulateGUI()
	{
		cposxctl.SetWindowText(cposx.c_str());
		cposyctl.SetWindowText(cposy.c_str());
		cposzctl.SetWindowText(cposz.c_str());

		lposxctl.SetWindowText(lposx.c_str());
		lposyctl.SetWindowText(lposy.c_str());
		lposzctl.SetWindowText(lposz.c_str());

		ldirxctl.SetWindowText(ldirx.c_str());
		ldiryctl.SetWindowText(ldiry.c_str());
		ldirzctl.SetWindowText(ldirz.c_str());

		lambctl.SetWindowText(lamb.c_str());
		ldifctl.SetWindowText(ldif.c_str());
		lspcctl.SetWindowText(lspc.c_str());

		lattcctl.SetWindowText(lattc.c_str());
		lattlctl.SetWindowText(lattl.c_str());
		lattqctl.SetWindowText(lattq.c_str());

		lsptinnerctl.SetWindowText(lsptinner.c_str());
		lsptouterctl.SetWindowText(lsptouter.c_str());

		CheckDlgButton(IDC_CHECKBLINN, lblinn ? BST_CHECKED : BST_UNCHECKED);

		ldirxctl.SetWindowText(ldirx.c_str());
		ldiryctl.SetWindowText(ldiry.c_str());
		ldirzctl.SetWindowText(ldirz.c_str());


		mshctl.SetWindowText(msh.c_str());
	}

	LRESULT PickColor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = True;

		CHOOSECOLOR cc;                 // common dialog box structure 
		static COLORREF acrCustClr[16]; // array of custom colors 
		static DWORD rgbCurrent;        // initial color selection

		if (wID == IDC_BUTTONCLRPKRAMB)
			rgbCurrent = mamb;
		else if (wID == IDC_BUTTONCLRPKRDIF)
			rgbCurrent = mdif;
		else if (wID == IDC_BUTTONCLRPKRSPC)
			rgbCurrent = mspc;
		else if (wID == IDC_EDITLGTAMBPKR)
			rgbCurrent = lambclr;
		else if (wID == IDC_EDITLGTDIFPKR)
			rgbCurrent = ldifclr;
		else if (wID == IDC_EDITLGTSPCPKR)
			rgbCurrent = lspcclr;

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

			if (wID == IDC_BUTTONCLRPKRAMB)
				mamb = rgbCurrent;
			else if (wID == IDC_BUTTONCLRPKRDIF)
				mdif = rgbCurrent;
			else if (wID == IDC_BUTTONCLRPKRSPC)
				mspc = rgbCurrent;
			else if (wID == IDC_EDITLGTAMBPKR)
				lambclr = rgbCurrent;
			else if (wID == IDC_EDITLGTDIFPKR)
				ldifclr = rgbCurrent;
			else if (wID == IDC_EDITLGTSPCPKR)
				lspcclr = rgbCurrent;
		}
		Invalidate();
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

		ldirxctl.GetWindowTextW((LPTSTR)ldirx.data(), 100);
		ldiryctl.GetWindowTextW((LPTSTR)ldiry.data(), 100);
		ldirzctl.GetWindowTextW((LPTSTR)ldirz.data(), 100);

		lattcctl.GetWindowTextW((LPTSTR)lattc.data(), 100);
		lattlctl.GetWindowTextW((LPTSTR)lattl.data(), 100);
		lattqctl.GetWindowTextW((LPTSTR)lattq.data(), 100);

		lsptinnerctl.GetWindowTextW((LPTSTR)lsptinner.data(), 100);
		lsptouterctl.GetWindowTextW((LPTSTR)lsptouter.data(), 100);

		lblinn = (IsDlgButtonChecked(IDC_CHECKBLINN) == BST_CHECKED);
		mshctl.GetWindowTextW((LPTSTR)msh.data(), 100);

	}

private:
	wstring  cposx{ 100,0 }, cposy{ 100,0 }, cposz{ 100,0 };
	wstring lposx{ 100,0 }, lposy{ 100,0 }, lposz{ 100,0 };

	wstring lamb{ 100,0 };
	wstring ldif{ 100,0 };
	wstring lspc{ 100,0 };

	ULONG	mamb;
	ULONG	mdif;
	ULONG	mspc;

	ULONG	lambclr;
	ULONG	ldifclr;
	ULONG	lspcclr;

	wstring ldirx{ 100,0 };
	wstring ldiry{ 100,0 };
	wstring ldirz{ 100,0 };

	wstring lattc{ 100,0 };
	wstring lattl{ 100,0 };
	wstring lattq{ 100,0 };

	wstring lsptinner{ 100,0 };
	wstring lsptouter{ 100,0 };

	wstring msh{ 100,0 };
	bool lblinn = true;
	
	uint cursel = 0;
	wstring lighttypestrings[4] =  { L"Basic", L"Directional", L"Point",  L"Spot" };

	CWindow cposxctl;
	CWindow cposyctl;
	CWindow cposzctl;

	CWindow lposxctl;
	CWindow lposyctl;
	CWindow lposzctl;

	CWindow lambctl;
	CWindow ldifctl;
	CWindow lspcctl;

	CWindow lambclrctl;
	CWindow ldifclrctl;
	CWindow lspcclrctl;

	CWindow lblinnctl;

	CWindow mambctl;
	CWindow mdifctl;
	CWindow mspcctl;
	CWindow mshctl;

	CWindow ldirxctl;
	CWindow ldiryctl;
	CWindow ldirzctl;

	CWindow lattcctl;
	CWindow lattlctl;
	CWindow lattqctl;

	CWindow lsptinnerctl;
	CWindow lsptouterctl;

	CWindow lightsrctypecmbctl;

};

