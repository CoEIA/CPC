/****************************** Module Header ******************************\
Module Name:  FileContextMenuExt.cpp
Project:      CPCShellExtContextMenuHandler
Copyright (c) Microsoft Corporation.

The code sample demonstrates creating a Shell context menu handler with C++. 

A context menu handler is a shell extension handler that adds commands to an 
existing context menu. Context menu handlers are associated with a particular 
file class and are called any time a context menu is displayed for a member 
of the class. While you can add items to a file class context menu with the 
registry, the items will be the same for all members of the class. By 
implementing and registering such a handler, you can dynamically add items to 
an object's context menu, customized for the particular object.

The example context menu handler adds the menu item "Display File Name (C++)"
to the context menu when you right-click a .cpp file in the Windows Explorer. 
Clicking the menu item brings up a message box that displays the full path 
of the .cpp file.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include "FileContextMenuExt.h"
#include "resource.h"
#include <strsafe.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

extern HINSTANCE g_hInst;
extern long g_cDllRef;

#define IDM_DISPLAY 0
#define IDM_QUICK 1
#define IDM_SAFE 2
#define IDM_THROUGH 3
#define IDM_SHRED 4

FileContextMenuExt::FileContextMenuExt(void) :
	m_cRef(1), 
    m_pszMenuText(L"&CPC Computer Scanning"),
    m_pszVerb("cppdisplay"),
    m_pwszVerb(L"cppdisplay"),
    m_pszVerbCanonicalName("CppDisplayFileName"),
    m_pwszVerbCanonicalName(L"CppDisplayFileName"),
    m_pszVerbHelpText("CPC Computer Scanning"),
    m_pwszVerbHelpText(L"CPC Computer Scanning"),

	m_pszMenuText2(L"&CPC Shreding"),
    m_pszVerb2("cppremove"),
    m_pwszVerb2(L"cppremove"),
    m_pszVerbCanonicalName2("CppRemoveFileName"),
    m_pwszVerbCanonicalName2(L"CppRomveFileName"),
    m_pszVerbHelpText2("CPC Shreding"),
    m_pwszVerbHelpText2(L"CPC Shreding")
{
    InterlockedIncrement(&g_cDllRef);

    // Load the bitmap for the menu item. 
    // If you want the menu item bitmap to be transparent, the color depth of 
    // the bitmap must not be greater than 8bpp.
    m_hMenuBmp = LoadImage(g_hInst, MAKEINTRESOURCE(IDB_OK), 
        IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADTRANSPARENT);
}

FileContextMenuExt::~FileContextMenuExt(void)
{
    if (m_hMenuBmp)
    {
        DeleteObject(m_hMenuBmp);
        m_hMenuBmp = NULL;
    }

    InterlockedDecrement(&g_cDllRef);
}

void FileContextMenuExt::OnVerbScanFileName(HWND hWnd)
{
	ShellExecute( NULL, L"open", GetApplicationPath().c_str(), L"-Scan", NULL, SW_SHOWNORMAL );
}


void FileContextMenuExt::OnVerbQuickItem(HWND hWnd)
{
	wchar_t szMessage[2000];

    if (SUCCEEDED(StringCchPrintf(szMessage, ARRAYSIZE(szMessage),L"-Shred_Quick \"%s\"", getSelectedNames().c_str())))
    {
        ShellExecute( NULL, L"open", GetApplicationPath().c_str(), szMessage, NULL, SW_SHOWNORMAL );
    }
}

std::basic_string<wchar_t> FileContextMenuExt::getSelectedNames() {
	std::basic_string<wchar_t> names = L"";

	for(int i=0; i<m_Files.size(); i++) {
		names += m_Files[i];

		if ( i < m_Files.size() - 1 )
			names += L",";
	}

	return names;
}

void FileContextMenuExt::OnVerbSafeItem(HWND hWnd) 
{
	wchar_t szMessage[2000];
    if (SUCCEEDED(StringCchPrintf(szMessage, ARRAYSIZE(szMessage),L"-Shred_Safe \"%s\"", getSelectedNames().c_str())))
    {
        ShellExecute( NULL, L"open", GetApplicationPath().c_str(), szMessage, NULL, SW_SHOWNORMAL );
    }
}

void FileContextMenuExt::OnVerbThroughItem(HWND hWnd)
{
	wchar_t szMessage[2000];
	if (SUCCEEDED(StringCchPrintf(szMessage, ARRAYSIZE(szMessage),L"-Shred_Through \"%s\"", getSelectedNames().c_str())))
	{
		ShellExecute( NULL, L"open", GetApplicationPath().c_str(), szMessage, NULL, SW_SHOWNORMAL );
	}
}

std::wstring FileContextMenuExt::GetApplicationPath() 
{
	HKEY hKey;
	LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\CoEIA\\CPC", 0, KEY_READ, &hKey);

	bool bExistsAndSuccess (lRes == ERROR_SUCCESS);
	bool bDoesNotExistsSpecifically (lRes == ERROR_FILE_NOT_FOUND);

	std::wstring strValueOfBinDir;
	GetStringRegKey(hKey, L"APP_PATH", strValueOfBinDir, L"bad");

	std::wstring fullpath = strValueOfBinDir + L"\\CPC.exe";
	return fullpath;
}

LONG FileContextMenuExt::GetStringRegKey(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue)
{
    strValue = strDefaultValue;
    WCHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    ULONG nError;
    nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        strValue = szBuffer;
    }
    return nError;
}

#pragma region IUnknown

// Query to the interface the component supported.
IFACEMETHODIMP FileContextMenuExt::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(FileContextMenuExt, IContextMenu),
        QITABENT(FileContextMenuExt, IShellExtInit), 
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

// Increase the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) FileContextMenuExt::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

// Decrease the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) FileContextMenuExt::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

#pragma endregion


#pragma region IShellExtInit

// Initialize the context menu handler.
IFACEMETHODIMP FileContextMenuExt::Initialize(
    LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID)
{
    if (NULL == pDataObj)
    {
        return E_INVALIDARG;
    }


    HRESULT hr = E_FAIL;
	TCHAR szFile[MAX_PATH];
	m_Files.clear();

    FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm;

    // The pDataObj pointer contains the objects being acted upon. In this 
    // example, we get an HDROP handle for enumerating the selected files and 
    // folders.
    if (SUCCEEDED(pDataObj->GetData(&fe, &stm)))
    {
        // Get an HDROP handle.
        HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != NULL)
        {
            // Determine how many files are involved in this operation. This 
            // code sample displays the custom context menu item when only 
            // one file is selected. 
            UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

			for (UINT i = 0; i < nFiles; i++) {
				// Get path length in chars
				UINT len = DragQueryFile(hDrop, i, NULL, 0);
				if (len == 0 || len >= MAX_PATH)
					continue;

				// Get the name of the file
				if (DragQueryFile(hDrop, i, szFile, len+1) == 0)
					continue;

				std::basic_string<wchar_t> str = std::basic_string<wchar_t>(szFile);
				if (str.empty())
					continue;

				m_Files.push_back(str);
			}

			if (!m_Files.empty()) {
				hr = S_OK;
			}

            GlobalUnlock(stm.hGlobal);
        }

        ReleaseStgMedium(&stm);
    }

    // If any value other than S_OK is returned from the method, the context 
    // menu item is not displayed.
    return hr;
}

#pragma endregion


#pragma region IContextMenu

//
//   FUNCTION: FileContextMenuExt::QueryContextMenu
//
//   PURPOSE: The Shell calls IContextMenu::QueryContextMenu to allow the 
//            context menu handler to add its menu items to the menu. It 
//            passes in the HMENU handle in the hmenu parameter. The 
//            indexMenu parameter is set to the index to be used for the 
//            first menu item that is to be added.
//
IFACEMETHODIMP FileContextMenuExt::QueryContextMenu(
    HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    // If uFlags include CMF_DEFAULTONLY then we should not do anything.
    if (CMF_DEFAULTONLY & uFlags)
    {
        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
    }

    // Use either InsertMenu or InsertMenuItem to add menu items.
    // Learn how to add sub-menu from:
    // http://www.codeproject.com/KB/shell/ctxextsubmenu.aspx

	UINT uID = idCmdFirst;

    MENUITEMINFO mii = { sizeof(mii) };
    mii.fMask = MIIM_BITMAP | MIIM_STRING | MIIM_FTYPE | MIIM_ID | MIIM_STATE;
    mii.wID = uID++;
    mii.fType = MFT_STRING;
    mii.dwTypeData = m_pszMenuText;
    mii.fState = MFS_ENABLED;
    mii.hbmpItem = static_cast<HBITMAP>(m_hMenuBmp);
    if (!InsertMenuItem(hMenu, indexMenu, TRUE, &mii))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

	HMENU hSubmenu = CreatePopupMenu();
    InsertMenu ( hSubmenu, 0, MF_BYPOSITION, uID++, L"&Shred Quick" );
	InsertMenu ( hSubmenu, 1, MF_BYPOSITION, uID++, L"&Shred Safe" );
	InsertMenu ( hSubmenu, 1, MF_BYPOSITION, uID++, L"&Shred Through" );

	MENUITEMINFO mii2 = { sizeof(mii2) };
    mii2.fMask = MIIM_BITMAP | MIIM_STRING | MIIM_FTYPE | MIIM_ID | MIIM_SUBMENU | MIIM_STATE;
    mii2.fType = MFT_STRING;
	mii2.wID = IDM_SHRED;
    mii2.dwTypeData = m_pszMenuText2;
    mii2.hbmpItem = static_cast<HBITMAP>(m_hMenuBmp);
	mii.fState = MFS_ENABLED;
	mii2.hSubMenu = hSubmenu;

    if (!InsertMenuItem(hMenu, indexMenu + 1, TRUE, &mii2))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Add a separator.
    MENUITEMINFO sep = { sizeof(sep) };
    sep.fMask = MIIM_TYPE;
    sep.fType = MFT_SEPARATOR;
    if (!InsertMenuItem(hMenu, indexMenu + 2, TRUE, &sep))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Return an HRESULT value with the severity set to SEVERITY_SUCCESS. 
    // Set the code value to the offset of the largest command identifier 
    // that was assigned, plus one (1).
    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(5));
}


//
//   FUNCTION: FileContextMenuExt::InvokeCommand
//
//   PURPOSE: This method is called when a user clicks a menu item to tell 
//            the handler to run the associated command. The lpcmi parameter 
//            points to a structure that contains the needed information.
//
IFACEMETHODIMP FileContextMenuExt::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
	if (!HIWORD(pici->lpVerb))
    {
        UINT idCmd = LOWORD(pici->lpVerb);

        switch (idCmd)
        {
        case 0:
			OnVerbScanFileName(pici->hwnd);
            break;
        case 1:
            OnVerbQuickItem(pici->hwnd);
            break;
        case 2:
			OnVerbSafeItem(pici->hwnd);
            break;
        case 3:
			OnVerbThroughItem(pici->hwnd);
            break;
        }

        return S_OK;
    }
    else
    {
        return E_INVALIDARG;
    }

}


//
//   FUNCTION: CFileContextMenuExt::GetCommandString
//
//   PURPOSE: If a user highlights one of the items added by a context menu 
//            handler, the handler's IContextMenu::GetCommandString method is 
//            called to request a Help text string that will be displayed on 
//            the Windows Explorer status bar. This method can also be called 
//            to request the verb string that is assigned to a command. 
//            Either ANSI or Unicode verb strings can be requested. This 
//            example only implements support for the Unicode values of 
//            uFlags, because only those have been used in Windows Explorer 
//            since Windows 2000.
//
IFACEMETHODIMP FileContextMenuExt::GetCommandString(UINT_PTR idCommand, 
    UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
    HRESULT hr = E_INVALIDARG;

    if (idCommand == IDM_DISPLAY)
    {
        switch (uFlags)
        {
        case GCS_HELPTEXTW:
            // Only useful for pre-Vista versions of Windows that have a 
            // Status bar.
            hr = StringCchCopy(reinterpret_cast<PWSTR>(pszName), cchMax, 
                m_pwszVerbHelpText);
            break;

        case GCS_VERBW:
            // GCS_VERBW is an optional feature that enables a caller to 
            // discover the canonical name for the verb passed in through 
            // idCommand.
            hr = StringCchCopy(reinterpret_cast<PWSTR>(pszName), cchMax, 
                m_pwszVerbCanonicalName);
            break;

        default:
            hr = S_OK;
        }
    }

	else if (idCommand == IDM_QUICK)
    {
        switch (uFlags)
        {
        case GCS_HELPTEXTW:
            // Only useful for pre-Vista versions of Windows that have a 
            // Status bar.
            hr = StringCchCopy(reinterpret_cast<PWSTR>(pszName), cchMax, 
                m_pwszVerbHelpText2);
            break;

        case GCS_VERBW:
            // GCS_VERBW is an optional feature that enables a caller to 
            // discover the canonical name for the verb passed in through 
            // idCommand.
            hr = StringCchCopy(reinterpret_cast<PWSTR>(pszName), cchMax, 
                m_pwszVerbCanonicalName2);
            break;

        default:
            hr = S_OK;
        }
    }

    // If the command (idCommand) is not supported by this context menu 
    // extension handler, return E_INVALIDARG.

    return hr;
}

#pragma endregion