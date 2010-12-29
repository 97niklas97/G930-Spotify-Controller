// Spotify.cpp
// Created by Marcus L�nnberg on 2010-12-29.

#include "stdafx.h"
#include <time.h>

typedef union _t{
	FILETIME    ft;
	__int64		tlong;
} FTIME;

static LPCWSTR SPOTIFY_WINDOW_CLASS = L"SpotifyMainWindow";

static unsigned int lastClick = 1000;
static FTIME lastClickTime;


static WCHAR* CommandListENG[] = {
	L"Play/pause",
	L"Next track",
	L"Previous track",
	L"Play/pause and next track",
	L"Play/pause and previous track",
	L"Next track and previous track",
	L"Previous track and next track",
	NULL
};

static WCHAR* CommandListSWE[] = {
	L"Spela/paus",
	L"N�sta l�t",
	L"F�reg�ende l�t",
	L"Spela/paus samt n�sta l�t",
	L"Spela/paus samt f�reg�ende l�t",
	L"N�sta l�t samt f�reg�ende l�t",
	L"F�reg�ende l�t samt n�sta l�t",
	NULL
};

WCHAR** GetGkeyCommandList(unsigned int languageCode){
	WCHAR** cmdList = CommandListENG;
	
	if(languageCode == 1053)
		cmdList = CommandListSWE;
	
	return cmdList;
}


void SendMediaCommand(int mediaCommand){
	HWND hwnd = FindWindow(SPOTIFY_WINDOW_CLASS, NULL);

	LRESULT res = SendMessage(hwnd, WM_APPCOMMAND , 0, mediaCommand*0x10000);
}

BOOL RunGkeyCommand (unsigned int commandID){
	BOOL retVal = TRUE;
	BOOL doubleClick = FALSE;

	SYSTEMTIME systime;
	FTIME timeNow;
	GetSystemTime(&systime);
	SystemTimeToFileTime(&systime, &timeNow.ft);

	// If last click was the same button and it was less then 0.6 seconds ago
	doubleClick = (lastClick == commandID && (timeNow.tlong - 6000000 < lastClickTime.tlong));
	
	switch (commandID){
		case 0:
			SendMediaCommand(APPCOMMAND_MEDIA_PLAY_PAUSE);
			break;
		case 1:
			SendMediaCommand(APPCOMMAND_MEDIA_NEXTTRACK);
			break;
		case 2:
			SendMediaCommand(APPCOMMAND_MEDIA_PREVIOUSTRACK);
			break;
		case 3:
		case 4:
			if(doubleClick){
				if(commandID == 3)
					SendMediaCommand(APPCOMMAND_MEDIA_NEXTTRACK);
				else
					SendMediaCommand(APPCOMMAND_MEDIA_PREVIOUSTRACK);
			}else
				SendMediaCommand(APPCOMMAND_MEDIA_PLAY_PAUSE);
			break;
		case 5:
		case 6:
			if(doubleClick^(commandID == 5))
				SendMediaCommand(APPCOMMAND_MEDIA_NEXTTRACK);
			else
				SendMediaCommand(APPCOMMAND_MEDIA_PREVIOUSTRACK);
			break;
		default:
			retVal = FALSE;
			break;
	}

	lastClick = commandID;
	SystemTimeToFileTime(&systime, &lastClickTime.ft);
	
	return retVal;
}