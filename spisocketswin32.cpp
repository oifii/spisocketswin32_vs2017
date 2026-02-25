/*
 * Copyright (c) 2010-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3501 rue Jeanne-Mance, #3
 * Montreal, QC, H2X 2K2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// spisocketswin32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "spisocketswin32.h"
#include "FreeImage.h"
#include <shellapi.h> //for CommandLineToArgW()
#include <mmsystem.h> //for timeSetEvent()
#include <stdio.h> //for swprintf()
#include <assert.h>
#include "spiwavsetlib.h"
//2023april29, spi begin
#include <SFML/Network.hpp>
#include <iostream>
//2023april29, spi end
// Global Variables:

//2023july28, spi, begin
const UINT IDT_TIMER1 = 1001;
//2023july28, spi, end

#define MAX_LOADSTRING 100
FIBITMAP* global_dib;
HFONT global_hFont;
HWND global_hwnd=NULL;
MMRESULT global_timer=0;
//#define MAX_GLOBALTEXT	4096
//WCHAR global_text[MAX_GLOBALTEXT+1];
//2023april29, spi, begin
/*
string global_filename="testcode.cpp";
float global_duration_sec=180;
float global_sleeptimeperline_sec=0.25;
*/
string global_servername="server"; //"server" waits for message or "client" for sending message
string global_ipaddress=""; //if this app runs as a client, that's the destination server's ipv4 address, otherwise not used
string global_message="done"; //if this app runs as a client, that's the message send to the server, otherwise it's the message that the server replies
float global_duration_sec=-1.0; //only used when app is a server. number of seconds the server app should persist, smaller than zero for no ending
bool udp = true; //false for tcp comm. instead of udp
const unsigned short port = 50001; // Choose an arbitrary port for opening sockets
//2023april29, spi end
int global_x=100;
int global_y=200;
int global_xwidth=400;
int global_yheight=400;
BYTE global_alpha=200;
int global_fontheight=24;
int global_fontwidth=-1; //will be computed within WM_PAINT handler
int global_staticalignment = 0; //0 for left, 1 for center and 2 for right
int global_staticheight=-1; //will be computed within WM_SIZE handler
int global_staticwidth=-1; //will be computed within WM_SIZE handler 
//COLORREF global_statictextcolor=RGB(0xFF, 0xFF, 0xFF); //white
COLORREF global_statictextcolor=RGB(0xFF, 0x00, 0x00); //red
//spi, begin
int global_imageheight=-1; //will be computed within WM_SIZE handler
int global_imagewidth=-1; //will be computed within WM_SIZE handler 
//spi, end
int global_titlebardisplay=1; //0 for off, 1 for on
int global_acceleratoractive=0; //0 for off, 1 for on
int global_menubardisplay=0; //0 for off, 1 for on

DWORD global_startstamp_ms;
//FILE* global_pFILE=NULL;
string global_line;
std::ifstream global_ifstream;

#define IDC_MAIN_EDIT	100
#define IDC_MAIN_STATIC	101

HINSTANCE hInst;								// current instance
//TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
//TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR szTitle[1024]={L"spisocketswin32title"};					// The title bar text
TCHAR szWindowClass[1024]={L"spisocketswin32class"};				// the main window class name

//new parameters
int global_statictextcolor_red=255;
int global_statictextcolor_green=0;
int global_statictextcolor_blue=0;
string global_begin="begin.ahk";
string global_end="end.ahk";


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte                  (CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar                  (CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void CALLBACK StartGlobalProcess(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	WavSetLib_Initialize(global_hwnd, IDC_MAIN_STATIC, global_staticwidth, global_staticheight, global_fontwidth, global_fontheight, global_staticalignment);

	DWORD nowstamp_ms = GetTickCount();
	while( (global_duration_sec<0.0f) || ((nowstamp_ms-global_startstamp_ms)/1000.0f)<global_duration_sec )
	{
		//Sleep((int)(1000));
		Sleep((int)(100));
		
		//2023april29, spi begin
		/*
		std::getline(global_ifstream, global_line);
		if(global_ifstream.eof()) 
		{
			global_ifstream.close();
			global_ifstream.open(global_filename.c_str(), ios_base::in);
			global_line="";
		}
		global_line += "\n";
		StatusAddText(global_line.c_str());
		*/
		//runUdpServer(port);
		
		// Create a socket to receive a message from anyone
		sf::UdpSocket socket;

		// Listen to messages on the specified port
		if (socket.bind(port) != sf::Socket::Done)
			break;
		//std::cout << "Server is listening to port " << port << ", waiting for a message... " << std::endl;
		global_line = global_servername;
		global_line += " is listening to port, waiting for a message... \n";
		StatusAddText(global_line.c_str());
		// Wait for a message
		const int maxsize = 128;
		char in[maxsize];
		memset(in, 0, maxsize);
		//sprintf(in,"\n");
		std::size_t received;
		sf::IpAddress sender;
		unsigned short senderPort;
		if (socket.receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
			break;
		//std::cout << "Message received from client " << sender << ": \"" << in << "\"" << std::endl;
		//2023july28, spi, begin
		ShowWindow(global_hwnd, SW_RESTORE);
		KillTimer(global_hwnd, IDT_TIMER1);
		SetTimer(global_hwnd, // handle to main window 
			IDT_TIMER1,       // timer identifier 
			5000,             // 5-second interval 
			(TIMERPROC)NULL); // no timer callback 
		//2023july28, spi, end
		global_line = "Message received from client ";
		StatusAddText(global_line.c_str());
		global_line = sender.toString();
		StatusAddText(global_line.c_str());
		StatusAddText("\n");
		global_line = ""; // to start a new empty line, erase the string
		global_line = in; 
		global_line += "\0"; //ensure null terminated string
		StatusAddText(global_line.c_str());
		StatusAddText("\n");

		/*
		// Send an answer to the client
		const char out[] = "Hi, I'm the server. And I received your message";
		if (socket.send(out, sizeof(out), sender, senderPort) != sf::Socket::Done)
			break;
		//std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;
		*/
		unsigned int xaos_command = 0;
		string xaos_parameters = "";
		if (global_line.length() >= 20)
		{
			xaos_command = atoi((global_line.substr(15, 5)).c_str());
			xaos_parameters = global_line.substr(20, global_line.length() - 20);
		}
		int nShowCmd = false;
		if (xaos_command==1)
		{
			ShellExecuteA(NULL, "open", "xaos_cmd_1.bat", xaos_parameters.c_str(), NULL, nShowCmd);
		}
		else if (xaos_command == 10)
		{
			ShellExecuteA(NULL, "open", "xaos_cmd_10.bat", xaos_parameters.c_str(), NULL, nShowCmd);
		}
		else if (xaos_command == 100)
		{
			ShellExecuteA(NULL, "open", "xaos_cmd_100.bat", xaos_parameters.c_str(), NULL, nShowCmd);
		}
		else if (xaos_command == 1000)
		{
			ShellExecuteA(NULL, "open", "xaos_cmd_1000.bat", xaos_parameters.c_str(), NULL, nShowCmd);
		}
		else
		{
			StatusAddText("WARNING: invalid xaos command received\n");
			StatusAddText(global_line.c_str());
			StatusAddText("\n\n");
		}
		//2023april29, spi end

		nowstamp_ms = GetTickCount();
	}
	PostMessage(global_hwnd, WM_DESTROY, 0, 0);
}




PCHAR*
    CommandLineToArgvA(
        PCHAR CmdLine,
        int* _argc
        )
    {
        PCHAR* argv;
        PCHAR  _argv;
        ULONG   len;
        ULONG   argc;
        CHAR   a;
        ULONG   i, j;

        BOOLEAN  in_QM;
        BOOLEAN  in_TEXT;
        BOOLEAN  in_SPACE;

        len = strlen(CmdLine);
        i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

        argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
            i + (len+2)*sizeof(CHAR));

        _argv = (PCHAR)(((PUCHAR)argv)+i);

        argc = 0;
        argv[argc] = _argv;
        in_QM = FALSE;
        in_TEXT = FALSE;
        in_SPACE = TRUE;
        i = 0;
        j = 0;

        while( a = CmdLine[i] ) {
            if(in_QM) {
                if(a == '\"') {
                    in_QM = FALSE;
                } else {
                    _argv[j] = a;
                    j++;
                }
            } else {
                switch(a) {
                case '\"':
                    in_QM = TRUE;
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    in_SPACE = FALSE;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if(in_TEXT) {
                        _argv[j] = '\0';
                        j++;
                    }
                    in_TEXT = FALSE;
                    in_SPACE = TRUE;
                    break;
                default:
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    _argv[j] = a;
                    j++;
                    in_SPACE = FALSE;
                    break;
                }
            }
            i++;
        }
        _argv[j] = '\0';
        argv[argc] = NULL;

        (*_argc) = argc;
        return argv;
    }

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	global_startstamp_ms = GetTickCount();

	//LPWSTR *szArgList;
	LPSTR *szArgList;
	int nArgs;
	int i;

	//szArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	szArgList = CommandLineToArgvA(GetCommandLineA(), &nArgs);
	if( NULL == szArgList )
	{
		//wprintf(L"CommandLineToArgvW failed\n");
		return FALSE;
	}
	LPWSTR *szArgListW;
	int nArgsW;
	szArgListW = CommandLineToArgvW(GetCommandLineW(), &nArgsW);
	if( NULL == szArgListW )
	{
		//wprintf(L"CommandLineToArgvW failed\n");
		return FALSE;
	}

	//2023april29, spi, begin
	/*
	if(nArgs>1)
	{
		global_filename = szArgList[1];
	}
	if(nArgs>2)
	{
		global_duration_sec = atof(szArgList[2]);
	}
	if(nArgs>3)
	{
		global_sleeptimeperline_sec = atof(szArgList[3]);
	}
	*/
	if(nArgs>1)
	{
		global_servername = szArgList[1]; //"server" waits for message or "client" for sending message
	}
	if(nArgs>2)
	{
		global_ipaddress = szArgList[2]; //if this app runs as a client, that's the destination server's ipv4 address, otherwise not used
	}
	if(nArgs>3)
	{
		global_message = szArgList[3]; //if this app runs as a client, that's the message send to the server, otherwise it's the message that the server replies
	}
	//2023april29, spi, end
	if(nArgs>4)
	{
		global_x = atoi(szArgList[4]);
	}
	if(nArgs>5)
	{
		global_y = atoi(szArgList[5]);
	}
	if(nArgs>6)
	{
		global_xwidth = atoi(szArgList[6]);
	}
	if(nArgs>7)
	{
		global_yheight = atoi(szArgList[7]);
	}
	if(nArgs>8)
	{
		global_alpha = atoi(szArgList[8]);
	}
	if(nArgs>9)
	{
		global_titlebardisplay = atoi(szArgList[9]);
	}
	if(nArgs>10)
	{
		global_menubardisplay = atoi(szArgList[10]);
	}
	if(nArgs>11)
	{
		global_acceleratoractive = atoi(szArgList[11]);
	}
	if(nArgs>12)
	{
		global_fontheight = atoi(szArgList[12]);
	}

	//new parameters
	if(nArgs>13)
	{
		global_statictextcolor_red = atoi(szArgList[13]);
	}
	if(nArgs>14)
	{
		global_statictextcolor_green = atoi(szArgList[14]);
	}
	if(nArgs>15)
	{
		global_statictextcolor_blue = atoi(szArgList[15]);
	}
	if(nArgs>16)
	{
		wcscpy(szWindowClass, szArgListW[16]); 
	}
	if(nArgs>17)
	{
		wcscpy(szTitle, szArgListW[17]); 
	}
	if(nArgs>18)
	{
		global_begin = szArgList[18]; 
	}
	if(nArgs>19)
	{
		global_end = szArgList[19]; 
	}
	global_statictextcolor=RGB(global_statictextcolor_red, global_statictextcolor_green, global_statictextcolor_blue);
	LocalFree(szArgList);
	LocalFree(szArgListW);

	int nShowCmd = false;
	//ShellExecuteA(NULL, "open", "begin.bat", "", NULL, nShowCmd);
	if(!global_begin.empty()) ShellExecuteA(NULL, "open", global_begin.c_str(), "", NULL, nCmdShow);

	//2023april29, spi, begin
	if(global_servername=="client" || global_servername == "CLIENT" || global_servername == "Client")
	{
		//runUdpClient(port);
	
		/*
		sf::IpAddress server;
		// Ask for the server address
		do
		{
			std::cout << "Type the address or name of the server to connect to: ";
			std::cin >> server;
		} while (server == sf::IpAddress::None);
		*/
		sf::IpAddress server(global_ipaddress);

		// Create a socket for communicating with the server
		sf::UdpSocket socket;

		// Send a message to the server
		//const char out[] = "Hi, I'm a client";
		//if (socket.send(out, sizeof(out), server, port) != sf::Socket::Done)
		if (socket.send(global_message.c_str(), global_message.length(), server, port) != sf::Socket::Done)
			return FALSE;
		//std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
		/*
		// Receive an answer from anyone (but most likely from the server)
		char in[128];
		std::size_t received;
		sf::IpAddress sender;
		unsigned short senderPort;
		if (socket.receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
			return FALSE;
		//std::cout << "Message received from " << sender << ": \"" << in << "\"" << std::endl;
		*/
		return 0; //return 0 when no error, safer for OS
	}
	//2023april29, spi, end


	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_SPIWAVWIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	if(global_acceleratoractive)
	{
		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPIWAVWIN32));
	}
	else
	{
		hAccelTable = NULL;
	}
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	//wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPIWAVWIN32));
	wcex.hIcon			= (HICON)LoadImage(NULL, L"background_32x32x16.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);

	if(global_menubardisplay)
	{
		wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SPIWAVWIN32); //original with menu
	}
	else
	{
		wcex.lpszMenuName = NULL; //no menu
	}
	wcex.lpszClassName	= szWindowClass;
	//wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hIconSm		= (HICON)LoadImage(NULL, L"background_16x16x16.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	global_dib = FreeImage_Load(FIF_JPEG, "background.jpg", JPEG_DEFAULT);

	//global_hFont=CreateFontW(global_fontheight,0,0,0,FW_NORMAL,0,0,0,0,0,0,2,0,L"SYSTEM_FIXED_FONT");
	global_hFont=CreateFontW(global_fontheight,0,0,0,FW_NORMAL,0,0,0,0,0,0,2,0,L"Segoe Script");

	if(global_titlebardisplay)
	{
		hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, //original with WS_CAPTION etc.
			global_x, global_y, global_xwidth, global_yheight, NULL, NULL, hInstance, NULL);
	}
	else
	{
		hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP | WS_VISIBLE, //no WS_CAPTION etc.
			global_x, global_y, global_xwidth, global_yheight, NULL, NULL, hInstance, NULL);
	}
	if (!hWnd)
	{
		return FALSE;
	}
	global_hwnd = hWnd;

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, global_alpha, LWA_ALPHA);

	//2023july28, spi, begin
	ShowWindow(hWnd, nCmdShow);
	//ShowWindow(hWnd, SW_SHOWMINIMIZED);
	SetTimer(hWnd, // handle to main window 
		IDT_TIMER1,       // timer identifier 
		5000,             // 5-second interval 
		(TIMERPROC)NULL); // no timer callback 

	//2023july28, spi, end
	UpdateWindow(hWnd);

	//global_pFILE = fopen(global_filename.c_str(), "r");
	//if(global_pFILE==NULL) return FALSE;
	//2023april29, spi, begin
	/*
	global_ifstream.open(global_filename.c_str(), ios_base::in);
	*/
	//2023april29, spi, end
	global_timer=timeSetEvent(500,25,(LPTIMECALLBACK)&StartGlobalProcess,0,TIME_ONESHOT);
	return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HGDIOBJ hOldBrush;
	HGDIOBJ hOldPen;
	int iOldMixMode;
	COLORREF crOldBkColor;
	COLORREF crOldTextColor;
	int iOldBkMode;
	HFONT hOldFont, hFont;
	TEXTMETRIC myTEXTMETRIC;

	switch (message)
	{
	case WM_CREATE:
		{
			/*
			HWND hStatic = CreateWindowEx(WS_EX_TRANSPARENT, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,  
				0, 100, 100, 100, hWnd, (HMENU)IDC_MAIN_STATIC, GetModuleHandle(NULL), NULL);
			*/
			HWND hStatic = CreateWindowEx(WS_EX_TRANSPARENT, L"STATIC", L"", WS_CHILD | WS_VISIBLE | global_staticalignment,  
				0, 100, 100, 100, hWnd, (HMENU)IDC_MAIN_STATIC, GetModuleHandle(NULL), NULL);
			if(hStatic == NULL)
				MessageBox(hWnd, L"Could not create static text.", L"Error", MB_OK | MB_ICONERROR);
			SendMessage(hStatic, WM_SETFONT, (WPARAM)global_hFont, MAKELPARAM(FALSE, 0));
		}
		break;
	case WM_SIZE:
		{
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			HWND hStatic = GetDlgItem(hWnd, IDC_MAIN_STATIC);
			global_staticwidth = rcClient.right - 0;
			//global_staticheight = rcClient.bottom-(rcClient.bottom/2);
			global_staticheight = rcClient.bottom-0;
			//spi, begin
			global_imagewidth = rcClient.right - 0;
			global_imageheight = rcClient.bottom - 0; 
			WavSetLib_Initialize(global_hwnd, IDC_MAIN_STATIC, global_staticwidth, global_staticheight, global_fontwidth, global_fontheight, global_staticalignment);
			//spi, end
			//SetWindowPos(hStatic, NULL, 0, rcClient.bottom/2, global_staticwidth, global_staticheight, SWP_NOZORDER);
			SetWindowPos(hStatic, NULL, 0, 0, global_staticwidth, global_staticheight, SWP_NOZORDER);
		}
		break;
	case WM_CTLCOLOREDIT:
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor((HDC)wParam, RGB(0xFF, 0xFF, 0xFF));
			return (INT_PTR)::GetStockObject(NULL_PEN);
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			SetBkMode((HDC)wParam, TRANSPARENT);
			//SetTextColor((HDC)wParam, RGB(0xFF, 0xFF, 0xFF));
			SetTextColor((HDC)wParam, global_statictextcolor);
			return (INT_PTR)::GetStockObject(NULL_PEN);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		SetStretchBltMode(hdc, COLORONCOLOR);
		//spi, begin
		/*
		StretchDIBits(hdc, 0, 0, global_xwidth, global_yheight,
						0, 0, FreeImage_GetWidth(global_dib), FreeImage_GetHeight(global_dib),
						FreeImage_GetBits(global_dib), FreeImage_GetInfo(global_dib), DIB_RGB_COLORS, SRCCOPY);
		*/
		StretchDIBits(hdc, 0, 0, global_imagewidth, global_imageheight,
						0, 0, FreeImage_GetWidth(global_dib), FreeImage_GetHeight(global_dib),
						FreeImage_GetBits(global_dib), FreeImage_GetInfo(global_dib), DIB_RGB_COLORS, SRCCOPY);
		//spi, end
		hOldBrush = SelectObject(hdc, (HBRUSH)GetStockObject(GRAY_BRUSH));
		hOldPen = SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
		//iOldMixMode = SetROP2(hdc, R2_MASKPEN);
		iOldMixMode = SetROP2(hdc, R2_MERGEPEN);
		//Rectangle(hdc, 100, 100, 200, 200);

		crOldBkColor = SetBkColor(hdc, RGB(0xFF, 0x00, 0x00));
		crOldTextColor = SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
		iOldBkMode = SetBkMode(hdc, TRANSPARENT);
		//hFont=CreateFontW(70,0,0,0,FW_BOLD,0,0,0,0,0,0,2,0,L"SYSTEM_FIXED_FONT");
		//hOldFont=(HFONT)SelectObject(hdc,global_hFont);
		hOldFont=(HFONT)SelectObject(hdc,global_hFont);
		GetTextMetrics(hdc, &myTEXTMETRIC);
		global_fontwidth = myTEXTMETRIC.tmAveCharWidth;
		//TextOutW(hdc, 100, 100, L"test string", 11);

		SelectObject(hdc, hOldBrush);
		SelectObject(hdc, hOldPen);
		SetROP2(hdc, iOldMixMode);
		SetBkColor(hdc, crOldBkColor);
		SetTextColor(hdc, crOldTextColor);
		SetBkMode(hdc, iOldBkMode);
		SelectObject(hdc,hOldFont);
		//DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		break;
	//2023july28, spi, begin
	case WM_TIMER:
		switch (wParam)
		{
		case IDT_TIMER1:
			// process the 5-second timer 
			KillTimer(hWnd, IDT_TIMER1);
			ShowWindow(hWnd, SW_SHOWMINIMIZED);
			return 0;
		}
		break;
	//2023july28, spi, end
	case WM_DESTROY:
		{
			//2023july28, spi, begin
			KillTimer(global_hwnd, IDT_TIMER1);
			//2023july28, spi, end
			WavSetLib_Terminate();
			if (global_timer) timeKillEvent(global_timer);
			FreeImage_Unload(global_dib);
			DeleteObject(global_hFont);
			//if(global_pFILE) fclose(global_pFILE);
			global_ifstream.close();

			//int nShowCmd = false;
			//ShellExecuteA(NULL, "open", "end.bat", "", NULL, nShowCmd);
			if(!global_end.empty()) ShellExecuteA(NULL, "open", global_end.c_str(), "", NULL, 0);
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
