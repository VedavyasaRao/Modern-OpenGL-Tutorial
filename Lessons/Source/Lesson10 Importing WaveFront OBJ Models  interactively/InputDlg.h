#pragma once
#include "atlwin.h"
#include "resource.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>

// InputDlg dialog
class InputDlg : public ATL::CDialogImpl<InputDlg>
{
public:
	enum { IDD = IDD_DIALOG1 };
	class Scene;

	BEGIN_MSG_MAP(InputDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BTNLGTCLRPKR, BN_CLICKED, PickColor)
		COMMAND_HANDLER(IDC_BTNBGKCLRPKR, BN_CLICKED, PickColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		COMMAND_HANDLER(IDReset, BN_CLICKED, OnBnClickedReset)
		COMMAND_HANDLER(IDAPPLY, BN_CLICKED, OnBnClickedApply)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDC_BUTTON_OBJFILE, BN_CLICKED, OnBnClickedDirButton)
	END_MSG_MAP()


public:

	InputDlg(WFObjParser*& pwfobjparser) :pobjparser((GenericParser*&)pwfobjparser)  {}
	InputDlg(AssertImpParser*& paiobjparser) :pobjparser((GenericParser*&)paiobjparser) {}

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Do some initialization code
		objfilenamectl = GetDlgItem(IDC_EDIT_OBJ_FILENAME);
		objinfoctl = GetDlgItem(IDC_EDIT_INFO);

		lposxctl = GetDlgItem(IDC_EDITLIGHTPOSX);
		lposyctl = GetDlgItem(IDC_EDITLIGHTPOSY);
		lposzctl = GetDlgItem(IDC_EDITLIGHTPOSZ);

		lambctl = GetDlgItem(IDC_EDITLIGHTAMB);
		lclrctl = GetDlgItem(IDC_EDITLGTCLR);
		bclrctl = GetDlgItem(IDC_EDITBGKCLR);

		cposxctl = GetDlgItem(IDC_EDITCAMPOSX);
		cposyctl = GetDlgItem(IDC_EDITCAMPOSY);
		cposzctl = GetDlgItem(IDC_EDITCAMPOSZ);

		scalexctl = GetDlgItem(IDC_EDIT_SCALEX);
		scaleyctl = GetDlgItem(IDC_EDIT_SCALEY);
		scalezctl = GetDlgItem(IDC_EDIT_SCALEZ);

		bHandled = true;
		return 1;
	}

	wstring converts(const string& s)
	{
		return wstring(begin(s), end(s));
	}

	bool checkfileexists(const wstring& filename)
	{
		struct stat buffer;
		return (stat(getfilename(filename).c_str(), &buffer) == 0);
	}


	bool validate()
	{
		pobjparser->clear();
		if (!pobjparser->Parse(getfilename(objfilename)))
		{
			wstring message(L"parse failed\r\n");
			message += objfilename;
			MessageBox(message.c_str(), L"Input", 0);
			return false;
		}

		for (auto& mat : pobjparser->matlinfolst)
		{
			auto texfn = converts(pobjparser->matltextmap[&mat]);
			if (!texfn.empty())
			{
				if (!checkfileexists(texfn))
				{
					wstring message(L"texture file not found\r\n");
					message += texfn;
					MessageBox(message.c_str(), L"Input", 0);
					return false;
				}
			}
		}
		return true;
	}

	void getobjdata()
	{
		wostringstream oss;
		oss  << setfill(L' ');
		oss << L"OBJ File:\t\t" << objfilename << "\r\n";
		oss << L"MTL File:\t\t" << mtlfilename << "\r\n";
		oss << L"Parse Time:\t" << pobjparser->parsetime() << " secs\r\n\r\n";


		oss << L"Mesh Info\r\n\r\n";
		for (auto k=0u; k < pobjparser->meshlst.size(); ++k)
		{
			oss << L"Mesh:\t\t#" << (k+1) << L"\r\n";
			oss << L"Vertices:\t\t" << right << pobjparser->vertex_count(k) << L"\r\n";
			oss << L"Textures:\t\t" << right << boolalpha << pobjparser->hastexture(k) << L"\r\n";
			oss << L"Normals:\t\t" << right << pobjparser->hasnormal(k) << L"\r\n";
			oss << L"Material:\t\t" << converts(pobjparser->getmat4mesh(k).name) << L"\r\n";
			oss << L"\r\n" << L"\r\n";
		}

		auto k = 0;
		oss << L"Material Info\r\n\r\n";
		for (auto& mat : pobjparser->matlinfolst)
		{
			oss << L"Material:\t\t#" << (k + 1) << L"\r\n";
			oss << L"Name:\t\t" << converts(mat.name) << L"\r\n";
			oss << L"Ambient:\t\t" << mat.ambientColor[0] << "  " << mat.ambientColor[1] << "  " << mat.ambientColor[2] << L"\r\n";
			oss << L"Diffuse:\t\t" << mat.diffuseColor[0] << "  " << mat.diffuseColor[1] << "  " << mat.diffuseColor[2] << L"\r\n";
			oss << L"Specular:\t\t" << mat.specularColor[0] << "  " << mat.specularColor[1] << "  " << mat.specularColor[2] << L"\r\n";
			oss << L"Shininess:\t" << left << mat.Shininess << L"\r\n";

			string difftxtname = pobjparser->matltextmap[&mat];
			oss << L"Diffuse Texture:\t" << converts(difftxtname) << L"\r\n";
			oss << L"\r\n";
			++k;
		}
		objinfoctl.SetWindowText((LPTSTR)oss.str().data());
	}
	
	LRESULT OnBnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (pobjparser->objfilename != getfilename(objfilename))
		{
			if (!validate())
				return 0;
			getobjdata();
		}
		::PostMessage(GetParent(), WM_COMMAND, IDAPPLY, 0);
		return 0;
	}

	LRESULT OnBnClickedReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		SetDefaultValues();
		PopulateGUI();
		if (validate())
		{
			getobjdata();
			Invalidate();
		}
		return 0;
	};

	LRESULT OnBnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
		::PostMessage(GetParent(), WM_COMMAND, IDCANCEL, 0);
		DestroyWindow();
		return 0;
	};

	void getfilenamesdata()
	{ 
		objfilenamectl.GetWindowText((LPTSTR)objfilename.data(), 500);
	}

	LightSrcInfo getdata()
	{
		getfilenamesdata();

		wstring buf(100, 0);
		lambctl.GetWindowText((LPTSTR)buf.data(), 100);
		lamb = stof(buf);
		
		cposxctl.GetWindowText((LPTSTR)buf.data(), 100);
		cpos[0] = stof(buf);
		cposyctl.GetWindowText((LPTSTR)buf.data(), 100);
		cpos[1] = stof(buf);
		cposzctl.GetWindowText((LPTSTR)buf.data(), 100);
		cpos[2] = stof(buf);

		lposxctl.GetWindowText((LPTSTR)buf.data(), 100);
		ldir[0] = stof(buf);
		lposyctl.GetWindowText((LPTSTR)buf.data(), 100);
		ldir[1] = stof(buf);
		lposzctl.GetWindowText((LPTSTR)buf.data(), 100);
		ldir[2] = stof(buf);

		scalexctl.GetWindowText((LPTSTR)buf.data(), 100);
		scale[0] = stof(buf);
		scaleyctl.GetWindowText((LPTSTR)buf.data(), 100);
		scale[1] = stof(buf);
		scalezctl.GetWindowText((LPTSTR)buf.data(), 100);
		scale[2] = stof(buf);

		LightSrcInfo lightsrc;
		lightsrc.src = LightSourceType::Basic;
		lightsrc.ambientCoefficient = lamb;
		lightsrc.specularColor = vec3(GetRValue(lclr)/ 255.0f, GetGValue(lclr)/ 255.0f, GetBValue(lclr)/ 255.0f);
		lightsrc.position = ldir;
		lightsrc.viewerPosition = cpos;
		lightsrc.blinn = true;
		return lightsrc;
	}

	void SetDefaultValues()
	{
		objfilename = LR"(..\resources\Models\crate.obj)";

		lamb = 0.05f;
		lclr = 16777215;
		bclr = 0;
		cpos = vec3(2.0, 0.0, -4.0);
		ldir = vec3(0.0, 3.0, -9.0);
		scale = vec3(1.0, 1.0, 1.0);
	}

	void PopulateGUI()
	{
		objfilenamectl.SetWindowText((LPTSTR)objfilename.data());

		cposxctl.SetWindowText((LPTSTR)tos(cpos[0]).data());
		cposyctl.SetWindowText((LPTSTR)tos(cpos[1]).data());
		cposzctl.SetWindowText((LPTSTR)tos(cpos[2]).data());

		lposxctl.SetWindowText((LPTSTR)tos(ldir[0]).data());
		lposyctl.SetWindowText((LPTSTR)tos(ldir[1]).data());
		lposzctl.SetWindowText((LPTSTR)tos(ldir[2]).data());

		lambctl.SetWindowText((LPTSTR)tos(lamb).data());

		objinfoctl.SetWindowText(L"");

		scalexctl.SetWindowText((LPTSTR)tos(scale[0]).data());
		scaleyctl.SetWindowText((LPTSTR)tos(scale[1]).data());
		scalezctl.SetWindowText((LPTSTR)tos(scale[2]).data());

	}

	string getfilename(const wstring& filename)
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

	LRESULT OnBnClickedDirButton(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
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
		if (wID == IDC_BUTTON_OBJFILE)
			ofn.lpstrFilter = L"OBJ\0*.OBJ\0";

		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		wstring filename = L"";
		// Display the Open dialog box. 
		if (GetOpenFileName(&ofn) == TRUE)
			filename = ofn.lpstrFile;

		if (wID == IDC_BUTTON_OBJFILE)
		{
			objfilename = filename;
			objfilenamectl.SetWindowText((LPTSTR)objfilename.data());
			
			int p = objfilename.find_last_of('.');
			objinfoctl.SetWindowText(L"");

		}
		bHandled = TRUE;
		return 0;
	}

	LRESULT PickColor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = True;

		CHOOSECOLOR cc;                 // common dialog box structure 
		static COLORREF acrCustClr[16]; // array of custom colors 
		static DWORD rgbCurrent;        // initial color selection

		if (wID == IDC_BTNLGTCLRPKR)
			rgbCurrent = lclr;
		else if (wID == IDC_BTNBGKCLRPKR)
			rgbCurrent = bclr;


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
			if (wID == IDC_BTNLGTCLRPKR)
				lclr = rgbCurrent;
			else if (wID == IDC_BTNBGKCLRPKR)
				bclr = rgbCurrent;
		}
		Invalidate();
		return 0;

	}

	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO: Add your message handler code here and/or call default
		bHandled = TRUE;

		if ((HWND)lParam == lclrctl.m_hWnd)
			SetBkColor((HDC)wParam, lclr);
		else if ((HWND)lParam == bclrctl.m_hWnd)
			SetBkColor((HDC)wParam, bclr);
		else
			return 0;
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}

	wstring tos(float f)
	{
		auto s = to_wstring(f);
		int p = s.find('.');
		return s.substr(0, p + 3);
	}

	ULONG getblclr()
	{
		return bclr;
	}

public:
	wstring objfilename{ 500, '\0' };
	ULONG bclr;
	vec3 scale;

private:
	GenericParser*& pobjparser;

	wstring mtlfilename;
	wstring  cposx{ 100,0 }, cposy{ 100,0 }, cposz{ 100,0 };
	wstring lposx{ 100,0 }, lposy{ 100,0 }, lposz{ 100,0 };
	wstring scalex{ 100,0 }, scaley{ 100,0 }, scalez{ 100,0 };

	float lamb;
	ULONG lclr;
	vec3 ldir;
	vec3 cpos;

private:
	CWindow objfilenamectl, objinfoctl;
	CWindow cposxctl, cposyctl, cposzctl;
	CWindow lposxctl, lposyctl, lposzctl;
	CWindow scalexctl, scaleyctl, scalezctl;
	CWindow lambctl;
	CWindow lclrctl;
	CWindow bclrctl;

};
