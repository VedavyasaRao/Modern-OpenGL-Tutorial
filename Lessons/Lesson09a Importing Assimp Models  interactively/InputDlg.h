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

	BEGIN_MSG_MAP(InputDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_EDITLGTCLRPKR, BN_CLICKED, PickColor)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		COMMAND_HANDLER(IDTBTNLOAD, BN_CLICKED, OnBnClickedLoad)
		COMMAND_HANDLER(IDReset, BN_CLICKED, OnBnClickedReset)
		COMMAND_HANDLER(IDAPPLY, BN_CLICKED, OnBnClickedApply)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDC_BUTTON_MTLFILE, BN_CLICKED, OnBnClickedDirButton)
		COMMAND_HANDLER(IDC_BUTTON_OBJFILE, BN_CLICKED, OnBnClickedDirButton)
		COMMAND_HANDLER(IDC_BUTTON_TXTFILE, BN_CLICKED, OnBnClickedDirButton)
	END_MSG_MAP()


public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Do some initialization code
		objfilenamectl = GetDlgItem(IDC_EDIT_OBJ_FILENAME);
		mtlfilenamectl = GetDlgItem(IDC_EDIT_MTL_FILENAME);
		txtfilenamectl = GetDlgItem(IDC_EDIT_TXT_FILENAME);
		objinfoctl = GetDlgItem(IDC_EDIT_INFO);

		lposxctl = GetDlgItem(IDC_EDITLIGHTPOSX);
		lposyctl = GetDlgItem(IDC_EDITLIGHTPOSY);
		lposzctl = GetDlgItem(IDC_EDITLIGHTPOSZ);

		lambctl = GetDlgItem(IDC_EDITLIGHTAMB);
		lclrctl = GetDlgItem(IDC_EDITLGTCLR);

		cposxctl = GetDlgItem(IDC_EDITCAMPOSX);
		cposyctl = GetDlgItem(IDC_EDITCAMPOSY);
		cposzctl = GetDlgItem(IDC_EDITCAMPOSZ);

		bHandled = true;
		return 1;
	}


	wstring converts(const string& s)
	{
		return wstring(begin(s), end(s));
	}

	void getobjdata()
	{
		HCURSOR hCursor = LoadCursor(NULL, IDC_WAIT);
		SetCursor(hCursor);
		auto mesh = std::unique_ptr<GenericObjMesh>(new GenericObjMesh(getfilename(objfilename), getfilename(mtlfilename)));
		hCursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCursor);

		wostringstream oss;
		oss  << setfill(L' ');
		oss << L"OBJ File:  " << objfilename << "\r\n\r\n";
		oss << L"Vertices:\t\t" << setw(10) << right << mesh->vertices.size() << L"\r\n";
		oss << L"Textures:\t\t" << setw(10) << right << mesh->texturemap.size() << L"\r\n";
		oss << L"Normals:\t\t" << setw(10) <<  right << mesh->normals.size() << L"\r\n";
		oss << L"Faces:\t\t" << setw(10) << right << mesh->faces.size() << L"\r\n";

		oss << L"\r\n" << L"\r\n";

		oss << L"MTL File:  " << converts(mesh->mtlfilename) << "\r\n\r\n";

		auto k = 1;
		for (auto& kv : mesh->matinfomap)
		{
			oss << L"Material:\t\t#" << k++ << L"\r\n";
			oss << L"Name:\t\t" << converts(kv.first) << L"\r\n";
			oss << L"Range:\t\t" << kv.second.range.first + 1 << " - " << kv.second.range.second + 1 << L"\r\n";
			oss << L"Ambient:\t\t" << kv.second.ambientclr[0] << "  " << kv.second.ambientclr[1] << "  " << kv.second.ambientclr[2] << L"\r\n";
			oss << L"Diffuse:\t\t" << kv.second.diffuseclr[0] << "  " << kv.second.diffuseclr[1] << "  " << kv.second.diffuseclr[2] << L"\r\n";
			oss << L"Specular:\t\t" << kv.second.specularclr[0] << "  " << kv.second.specularclr[1] << "  " << kv.second.specularclr[2] << L"\r\n";
			oss << L"Shininess:\t\t" << left << kv.second.shininess << L"\r\n";
			oss << L"Emissive:\t\t" << kv.second.emissiveclr[0] << "  " << kv.second.emissiveclr[1] << "  " << kv.second.emissiveclr[2] << L"\r\n";
			oss << L"Diffuse Texture:\t\t" << converts(kv.second.diffusetxtfilename) << L"\r\n";
			oss << L"\r\n";
		}

		objinfoctl.SetWindowText((LPTSTR)oss.str().data());
	}
	
	LRESULT OnBnClickedLoad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bHandled = true;
	
		getfilenamesdata();
		getobjdata();
	
		return 0;
	};

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
		Invalidate();
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
		mtlfilenamectl.GetWindowText((LPTSTR)mtlfilename.data(), 500);
		txtfilenamectl.GetWindowText((LPTSTR)txtfilename.data(), 500);
	}

	AssimpModel::WFOBJInfo getdata()
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

		ldir = vec3(0.0, 3.0, -9.0);
		lposxctl.GetWindowText((LPTSTR)buf.data(), 100);
		ldir[0] = stof(buf);
		lposyctl.GetWindowText((LPTSTR)buf.data(), 100);
		ldir[1] = stof(buf);
		lposzctl.GetWindowText((LPTSTR)buf.data(), 100);
		ldir[2] = stof(buf);

		AssimpModel::WFOBJInfo::Light light;
		light.ambientCoefficient = lamb;
		light.Color = vec3(GetRValue(lclr)/ 255.0f, GetGValue(lclr)/ 255.0f, GetBValue(lclr)/ 255.0f);
		light.Position = ldir;

		auto txinf = TextureUtil::TexInfo(10, getfilename(txtfilename),  GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
		return AssimpModel::WFOBJInfo(getfilename(objfilename), getfilename(mtlfilename), txinf, cpos, light);
	}

	void SetDefaultValues()
	{
		objfilename = LR"(..\resources\WFObj\crate.obj)";
		mtlfilename = LR"(..\resources\WFObj\crate.mtl)";
		txtfilename = LR"(..\resources\textures\woodcrate_diffuse.jpg)";

		lamb = 0.05f;
		lclr = 16777215;
		cpos = vec3(2.0, 0.0, -4.0);
		ldir = vec3(0.0, 3.0, -9.0);
	}

	void PopulateGUI()
	{
		objfilenamectl.SetWindowText((LPTSTR)objfilename.data());
		mtlfilenamectl.SetWindowText((LPTSTR)mtlfilename.data());
		txtfilenamectl.SetWindowText((LPTSTR)txtfilename.data());

		cposxctl.SetWindowText((LPTSTR)tos(cpos[0]).data());
		cposyctl.SetWindowText((LPTSTR)tos(cpos[1]).data());
		cposzctl.SetWindowText((LPTSTR)tos(cpos[2]).data());

		lposxctl.SetWindowText((LPTSTR)tos(ldir[0]).data());
		lposyctl.SetWindowText((LPTSTR)tos(ldir[1]).data());
		lposzctl.SetWindowText((LPTSTR)tos(ldir[2]).data());

		lambctl.SetWindowText((LPTSTR)tos(lamb).data());

		objinfoctl.SetWindowText(L"");


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
		else if (wID == IDC_BUTTON_MTLFILE)
			ofn.lpstrFilter = L"MTL\0*.MTL\0";
		else if (wID == IDC_BUTTON_TXTFILE)
			ofn.lpstrFilter = L"All\0*.*\0JPG\0*.JPG\0PNG\0*.PNG\0BMP\0*.BMP\0TGA\0*.TGA\0";
		else
			ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";

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
			mtlfilename = objfilename.substr(0, p + 1) + L"mtl";
			mtlfilenamectl.SetWindowText((LPTSTR)mtlfilename.data());

			txtfilenamectl.SetWindowText(L"");
			objinfoctl.SetWindowText(L"");

		}
		else if (wID == IDC_BUTTON_MTLFILE)
		{
			mtlfilename = filename;
			mtlfilenamectl.SetWindowText((LPTSTR)mtlfilename.data());
		}
		else if (wID == IDC_BUTTON_TXTFILE)
		{
			txtfilename = filename;
			txtfilenamectl.SetWindowText((LPTSTR)txtfilename.data());
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

		rgbCurrent = lclr;

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
			lclr = rgbCurrent;
		}
		Invalidate();
		return 0;

	}

	LRESULT OnCtlColorStatic(UINT , WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO: Add your message handler code here and/or call default
		bHandled = TRUE;

		if ((HWND)lParam == lclrctl.m_hWnd)
			SetBkColor((HDC)wParam, lclr);
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

private:
	wstring objfilename{ 500, '\0' }, mtlfilename{ 500, '\0' }, txtfilename{ 500, '\0' };
	wstring  cposx{ 100,0 }, cposy{ 100,0 }, cposz{ 100,0 };
	wstring lposx{ 100,0 }, lposy{ 100,0 }, lposz{ 100,0 };

	float lamb;
	ULONG lclr;
	vec3 ldir;
	vec3 cpos;

private:
	CWindow objfilenamectl, mtlfilenamectl, txtfilenamectl, objinfoctl;
	CWindow cposxctl, cposyctl, cposzctl;
	CWindow lposxctl, lposyctl, lposzctl;
	CWindow lambctl;
	CWindow lclrctl;

};
