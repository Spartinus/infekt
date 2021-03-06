/**
 * Copyright (C) 2010 syndicode
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 **/

#include "updater.h"


/************************************************************************/
/* SOME STATIC VARS                                                     */
/************************************************************************/

static HWND s_hDlg = 0;
static std::wstring s_installerPath;

/************************************************************************/
/* TASK KILL JOB                                                        */
/************************************************************************/

static void __cdecl TaskKillThread(void *pvStartupInfo)
{
	HANDLE hStartEvent = static_cast<HANDLE>(pvStartupInfo);

	HWND l_hDlg = s_hDlg;

	::SetEvent(hStartEvent);

	// Terminate any running iNFekt exe instances (using taskkill.exe):

	std::wstring l_taskKillExePath = GetSysDirPath() + L"\\taskkill.exe";

	ShellExecuteAndWait(l_taskKillExePath.c_str(), L"/IM infekt-win32.exe", SW_HIDE);
	ShellExecuteAndWait(l_taskKillExePath.c_str(), L"/IM infekt-cmd.exe", SW_HIDE);
	ShellExecuteAndWait(l_taskKillExePath.c_str(), L"/IM infekt-win64.exe", SW_HIDE);

	// small pause fwiw:
	Sleep(1250);

	SendMessage(l_hDlg, WM_TASKKILL_COMPLETE, 0, 0);
}


bool StartTaskKill(HWND hDlg)
{
	s_hDlg = hDlg;

	HANDLE hStartEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

	if (_beginthread(TaskKillThread, 0, hStartEvent))
	{
		::WaitForSingleObject(hStartEvent, INFINITE);
	}

	CloseHandle(hStartEvent);

	return true;
}


/************************************************************************/
/* INSTALLER JOBS                                                       */
/************************************************************************/

static void __cdecl InstallerThread(void *pvStartupInfo)
{
	HANDLE hStartEvent = pvStartupInfo;

	HWND l_hDlg = s_hDlg;
	std::wstring l_installerPath = s_installerPath;

	::SetEvent(hStartEvent);

	bool b = ShellExecuteAndWait(l_installerPath.c_str(),
		L"/VERYSILENT /NOCANCEL", SW_HIDE, true, INFINITE);

	Sleep(1000);

	SendMessage(l_hDlg, WM_INSTALLER_COMPLETE, (b ? 0 : -1), 0);
}


bool StartInstaller(HWND hDlg, const std::wstring& a_installerPath)
{
	s_hDlg = hDlg;
	s_installerPath = a_installerPath;

	HANDLE hStartEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

	if (_beginthread(InstallerThread, 0, hStartEvent))
	{
		::WaitForSingleObject(hStartEvent, INFINITE);
	}

	CloseHandle(hStartEvent);

	return true;
}

