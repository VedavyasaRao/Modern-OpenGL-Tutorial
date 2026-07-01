#pragma once
#include "atlwin.h"
#include "resource.h"
#include <string>
#include <sstream>
#include <map>
#include "GL/GL.h"


// InputDlg dialog
class InputDlg : public ATL::CDialogImpl<InputDlg>
{
public:
	enum { IDD = IDD_DIALOG1 };

	BEGIN_MSG_MAP(InputDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDReset, BN_CLICKED, OnBnClickedReset)
		COMMAND_HANDLER(IDAPPLY, BN_CLICKED, OnBnClickedApply)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDC_BUTTON_DIR, BN_CLICKED, OnBnClickedDirButton)
	END_MSG_MAP()


public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Do some initialization code
		RECT rect;

		filenamectl = GetDlgItem(IDC_EDIT_FILENAME);

		swrapcmbctl = GetDlgItem(IDC_COMBO_S_WRAP);
		swrapcmbctl.GetWindowRect(&rect);
		::MapWindowPoints(nullptr, this->m_hWnd, (LPPOINT)&rect, 2);
		swrapcmbctl.MoveWindow(rect.left, rect.top, rect.right - rect.left, 200, TRUE);
		addcombo(swrapcmbctl, wraps);

		twrapcmbctl = GetDlgItem(IDC_COMBO_T_WRAP);
		twrapcmbctl.GetWindowRect(&rect);
		::MapWindowPoints(nullptr, this->m_hWnd, (LPPOINT)&rect, 2);
		twrapcmbctl.MoveWindow(rect.left, rect.top, rect.right - rect.left, 200, TRUE);
		addcombo(twrapcmbctl, wraps);

		minfiltercmbctl = GetDlgItem(IDC_COMBO_MIN_FILTER);
		minfiltercmbctl.GetWindowRect(&rect);
		::MapWindowPoints(nullptr, this->m_hWnd, (LPPOINT)&rect, 2);
		minfiltercmbctl.MoveWindow(rect.left, rect.top, rect.right - rect.left, 200, TRUE);
		addcombo(minfiltercmbctl, filter);

		magfiltercmbctl = GetDlgItem(IDC_COMBO_MAG_FILTER);
		magfiltercmbctl.GetWindowRect(&rect);
		::MapWindowPoints(nullptr, this->m_hWnd, (LPPOINT)&rect, 2);
		magfiltercmbctl.MoveWindow(rect.left, rect.top, rect.right - rect.left, 200, TRUE);
		addcombo(magfiltercmbctl, filter);


		
		bHandled = true;
		return 1;
	}

	LRESULT OnBnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		::PostMessage(GetParent(), WM_COMMAND, IDAPPLY, 0);
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


	TextureUtil::TexInfo   getdata()
	{
		filenamectl.GetWindowText((LPTSTR)filename.data(), 500);

		swrapcmb = getdata(swrapcmbctl);
		twrapcmb = getdata(twrapcmbctl);
		minfiltercmb = getdata(minfiltercmbctl);
		magfiltercmb = getdata(magfiltercmbctl);

		return TextureUtil::TexInfo(0, getfilename(), swrapcmb, twrapcmb, minfiltercmb, magfiltercmb);
	}

	void SetDefaultValues()
	{
		filename = LR"(..\resources\textures\bricks2.jpg)";
		swrapcmb = twrapcmb = GL_REPEAT;
		minfiltercmb = magfiltercmb = GL_LINEAR;
	}

	void PopulateGUI()
	{
		filenamectl.SetWindowText((LPTSTR)filename.data());
		setdata(swrapcmbctl, swrapcmb);
		setdata(twrapcmbctl, twrapcmb);
		setdata(minfiltercmbctl, minfiltercmb);
		setdata(magfiltercmbctl, magfiltercmb);

	}

	string getfilename()
	{
		if (filename.empty()) 
			return string();

		// Get size needed for target string
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &filename[0], (int)filename.size(), NULL, 0, NULL, NULL);

		string strTo(size_needed, 0);
		// Perform actual conversion
		WideCharToMultiByte(CP_UTF8, 0, &filename[0], (int)filename.size(), &strTo[0], size_needed, NULL, NULL);

		return strTo;
	}

	LRESULT OnBnClickedDirButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
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
			filename = ofn.lpstrFile;
			filenamectl.SetWindowText((LPTSTR)filename.data());
			getdata();
			::PostMessage(GetParent(), WM_COMMAND, IDAPPLY, 1);
		}
		return 0;

		
		bHandled = TRUE;
		return 0;
	}


private:
	void addcombo(CWindow& combo, map<wstring, uint>& data, bool bcombo = TRUE)
	{
		for (auto& item : data)
		{
			auto index = combo.SendMessageW((bcombo ? CB_ADDSTRING : LB_ADDSTRING), 0, (LPARAM)item.first.c_str());
			combo.SendMessage((bcombo ? CB_SETITEMDATA : LB_SETITEMDATA), (WPARAM)index, (LPARAM)item.second);
		}
	}

	void setdata(CWindow& combo, uint val, bool bcombo = TRUE)
	{
		if (bcombo)
		{
			int itemCount = (int)combo.SendMessage(CB_GETCOUNT, 0, 0);
			for (auto i = 0; i < itemCount; ++i)
			{
				auto result = combo.SendMessage(CB_GETITEMDATA, (WPARAM)i, 0);
				if (val == result)
				{
					combo.SendMessage(CB_SETCURSEL, i, 0);
					return;
				}
			}			
		}
	}

	LPARAM getdata(CWindow& combo, bool bcombo = TRUE)
	{
		if (bcombo)
		{
			int selectedIndex = combo.SendMessage(CB_GETCURSEL, 0, 0);

			if (selectedIndex != CB_ERR)
				return combo.SendMessage(CB_GETITEMDATA, (WPARAM)selectedIndex, 0);
		}
		else
		{
			int count = (int)combo.SendMessage(LB_GETSELCOUNT, 0, 0);
			if (count == LB_ERR || count == 0)
				return 0;

			vector<int> selectedIndices(20);
			int itemsRetrieved = (int)combo.SendMessage(LB_GETSELITEMS, (WPARAM)count, (LPARAM)selectedIndices.data());
			if (itemsRetrieved == LB_ERR)
				return 0;

			uint result = 0;
			for (int i = 0; i < itemsRetrieved; ++i)
			{
				result += combo.SendMessage(LB_GETITEMDATA, (WPARAM)selectedIndices[i], 0);
			}
			return result;
		}
		return 0;
	}


public:
	map<wstring, uint> wraps = {
		{L"GL_CLAMP", 0x2900},
		{L"GL_REPEAT",GL_REPEAT}
	};

	map<wstring, uint> filter = {
		{L"GL_NEAREST",GL_NEAREST},
		{L"GL_LINEAR",GL_LINEAR}
	};


public:
	wstring filename{500, '\0'};
	uint	swrapcmb, twrapcmb, minfiltercmb, magfiltercmb;

private:
	CWindow filenamectl;

	CWindow swrapcmbctl;
	CWindow twrapcmbctl;

	CWindow minfiltercmbctl;
	CWindow magfiltercmbctl;


};
