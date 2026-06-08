#pragma once
#include "atlwin.h"
#include "resource.h"
#include <string>
#include <sstream>
#include <map>
#include "GL/GL.h"
#include "SOIL2.h"


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
		channelscmbctl = GetDlgItem(IDC_COMBO_CHANNELS);
		channelscmbctl.GetWindowRect(&rect);
		::MapWindowPoints(nullptr, this->m_hWnd, (LPPOINT)&rect, 2);
		channelscmbctl.MoveWindow(rect.left, rect.top, rect.right - rect.left, 200, TRUE);
		addcombo(channelscmbctl, channles);

		flagslstctl = GetDlgItem(IDC_LIST_FLAGS);
		addcombo(flagslstctl, flags, false);

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

		cubeszctl = GetDlgItem(IDC_EDIT_CUBESZ);
		texunitctl = GetDlgItem(IDC_EDIT_TEXUNIT);

		
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

	void update(TextureUtil::TexInfo* ptexinfo)
	{
		getdata();

		ptexinfo->filename = getfilename();
		ptexinfo->channels = channelscmb;
		ptexinfo->flags = flagslst;
		ptexinfo->swrap = swrapcmb;
		ptexinfo->twrap = twrapcmb;
		ptexinfo->minfilter = minfiltercmb;
		ptexinfo->magfilter = magfiltercmb;
		ptexinfo->texunit = texunit;
	}

	void getdata()
	{
		filenamectl.GetWindowText((LPTSTR)filename.data(), 500);

		channelscmb = getdata(channelscmbctl);
		flagslst = getdata(flagslstctl, FALSE);
		swrapcmb = getdata(swrapcmbctl);
		twrapcmb = getdata(twrapcmbctl);
		minfiltercmb = getdata(minfiltercmbctl);
		magfiltercmb = getdata(magfiltercmbctl);

		wstring buf(100, 0);
		cubeszctl.GetWindowTextW((LPTSTR)buf.data(), 100);
		cubesz = stof(buf);

		texunitctl.GetWindowTextW((LPTSTR)buf.data(), 100);
		texunit = stoi(buf);

	}

	void SetDefaultValues()
	{
		filename = LR"(..\resources\textures\bricks2.jpg)";
		channelscmb = SOIL_LOAD_AUTO;
		flagslst = SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y;
		swrapcmb = twrapcmb = GL_REPEAT;
		minfiltercmb = magfiltercmb = GL_LINEAR;
		cubesz = 0.5;
		texunit = 10;
	}

	void PopulateGUI()
	{
		filenamectl.SetWindowText((LPTSTR)filename.data());
		setdata(channelscmbctl, channelscmb);
		setdata(flagslstctl, flagslst, FALSE);
		setdata(swrapcmbctl, swrapcmb);
		setdata(twrapcmbctl, twrapcmb);
		setdata(minfiltercmbctl, minfiltercmb);
		setdata(magfiltercmbctl, magfiltercmb);

		wstring buf = to_wstring(cubesz);
		cubeszctl.SetWindowTextW((LPTSTR)buf.substr(0, buf.find('.')+3).data());
		texunitctl.SetWindowTextW((LPTSTR)to_wstring(texunit).data());
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
		else
		{
			uint itemCount = (int)combo.SendMessage(LB_GETCOUNT, 0, 0);
			combo.SendMessage(LB_SETSEL, FALSE, -1);
			for (uint i = 0; i < itemCount; ++i)
			{
				auto result = combo.SendMessage(LB_GETITEMDATA, (WPARAM)i, 0);
				if ((flagslst & result) != 0)
					combo.SendMessage(LB_SETSEL, TRUE, i);
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
	map<wstring, uint> channles = {
		{L"image format",SOIL_LOAD_AUTO},
		{L"luminous",SOIL_LOAD_L},
		{L"luminous/alpha",SOIL_LOAD_LA},
		{L"RGB",SOIL_LOAD_RGB},
		{L"RGBA",SOIL_LOAD_RGBA}
	};

	map<wstring, uint> flags = {
		{L"SOIL_FLAG_POWER_OF_TWO", SOIL_FLAG_POWER_OF_TWO},
		{L"SOIL_FLAG_MIPMAPS", SOIL_FLAG_MIPMAPS},
		{L"SOIL_FLAG_TEXTURE_REPEATS", SOIL_FLAG_TEXTURE_REPEATS},
		{L"SOIL_FLAG_MULTIPLY_ALPHA", SOIL_FLAG_MULTIPLY_ALPHA},
		{L"SOIL_FLAG_INVERT_Y", SOIL_FLAG_INVERT_Y},
		{L"SOIL_FLAG_COMPRESS_TO_DXT", SOIL_FLAG_COMPRESS_TO_DXT},
		{L"SOIL_FLAG_DDS_LOAD_DIRECT", SOIL_FLAG_DDS_LOAD_DIRECT},
		{L"SOIL_FLAG_NTSC_SAFE_RGB", SOIL_FLAG_NTSC_SAFE_RGB},
		{L"SOIL_FLAG_CoCg_Y", SOIL_FLAG_CoCg_Y},
		{L"SOIL_FLAG_TEXTURE_RECTANGLE", SOIL_FLAG_TEXTURE_RECTANGLE},
		{L"SOIL_FLAG_PVR_LOAD_DIRECT", SOIL_FLAG_PVR_LOAD_DIRECT},
		{L"SOIL_FLAG_ETC1_LOAD_DIRECT", SOIL_FLAG_ETC1_LOAD_DIRECT},
		{L"SOIL_FLAG_GL_MIPMAPS", SOIL_FLAG_GL_MIPMAPS},
		{L"SOIL_FLAG_SRGB_COLOR_SPACE", SOIL_FLAG_SRGB_COLOR_SPACE}
	};

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
	uint	channelscmb, flagslst, swrapcmb, twrapcmb, minfiltercmb, magfiltercmb;
	float cubesz;
	uint texunit;

private:
	CWindow filenamectl;
	CWindow channelscmbctl;
	CWindow flagslstctl;

	CWindow swrapcmbctl;
	CWindow twrapcmbctl;

	CWindow minfiltercmbctl;
	CWindow magfiltercmbctl;

	CWindow cubeszctl;
	CWindow texunitctl;

};
