//--------------------------------------------------------------
//	<:))))><®	 <----->	FishMessenger®. -IPv6.
//--------------------------------------------------------------
//	Main Entry File:		MainEntry.cpp.
//	06/08/2013.				by bubo.
//	07/05/2014.				Modified.
//	Product Version:		1.4.0.62.
//--------------------------------------------------------------
//	Libraries and Headers.
//--------------------------------------------------------------
#pragma comment (lib, "Ws2_32")
#pragma comment (lib, "comctl32")
#pragma comment (lib, "Shlwapi")
#pragma comment (lib, "Gdiplus")

//--------------------------------------------------------------
#include "Commander.h"
#include "resource.h"

//--------------------------------------------------------------
//	Title.
//--------------------------------------------------------------
#if _WIN64
static TCHAR szAppTitle[] = L"Colin FishMessenger 64Bit";
#else
static TCHAR szAppTitle[] = L"Colin FishMessenger 32Bit";
#endif

//--------------------------------------------------------------
//	Function and variable Declarations...
//--------------------------------------------------------------
using namespace cinderella;

//--------------------------------------------------------------
LRESULT CALLBACK FishMessengerProcedure (HWND hwnd, UINT message, 
	WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------
const unsigned short TCP_UDP_CTRL_PORT = 39911;
extern const unsigned short UDP_TEXT_PORT = 39912;

//--------------------------------------------------------------
//	Main Entry.
//--------------------------------------------------------------
int WINAPI wWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
					PTSTR szCmdLine, int iCmdShow)
{
	//--------------------------------------------------------------
	// Test Singleton of application...
	//--------------------------------------------------------------
	if (!TestSingleton())
	{
		::MessageBox(nullptr, L"Only one FishMessenger® can run at" 
			L" the same time!", L"<:))))><   FishMessenger® Colin software", 
			MB_OK | MB_ICONWARNING);

		return 0;
	}

	//--------------------------------------------------------------
    static wchar_t szAppName[] = L"FishMessenger";
    HWND         hWnd = nullptr;
	MSG          msg = { 0 };
    WNDCLASSEX   wndclassex = { 0 };

    wndclassex.cbSize        = sizeof(WNDCLASSEX);
    wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc	 = FishMessengerProcedure;
    wndclassex.cbClsExtra    = 0;
    wndclassex.cbWndExtra    = 30;
    wndclassex.hInstance     = hInstance;
	wndclassex.hIcon		 = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FISH_OPT));
    wndclassex.hCursor       = ::LoadCursor(nullptr, IDC_ARROW);
	wndclassex.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
    wndclassex.lpszMenuName  = nullptr;
    wndclassex.lpszClassName = szAppName;
	wndclassex.hIconSm		 = wndclassex.hIcon;
	
    if (!RegisterClassEx (&wndclassex))
    {
        ::MessageBox (NULL,  L"RegisterClassEx failed!", szAppName, MB_ICONERROR);
        return 0;
    }

	hWnd = ::CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOGMAIN), nullptr, nullptr);
	
	::ShowWindow(hWnd, iCmdShow);
	::UpdateWindow(hWnd);
	
	while (::GetMessage (&msg, nullptr, 0, 0))
	{

		if (!UserDialog::IsDialogMsg(&msg) && 
			!ProgressDialog::IsDialogMsg(&msg) &&
			!TransferIndicator::IsDialogMsg(&msg) &&
			!SettingsDialog::IsDialogMsg(&msg))
		{
			::TranslateMessage (&msg);
			::DispatchMessage (&msg);
		}

	}

	Commander::DestroySingleton();

    return msg.wParam;
}


//--------------------------------------------------------------
//	Window Procedure... 
//--------------------------------------------------------------
LRESULT CALLBACK FishMessengerProcedure(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	//--------------------------------------------------------------
	HDC hDC = nullptr;
	INT_PTR pResult = NULL;

	//--------------------------------------------------------------
    switch (message)
    {
    case WM_CREATE:

		//--------------------------------------------------------------
		// Do initialization using facade Commander...
		//--------------------------------------------------------------
		Commander::GetSingleton ()->Initialize (reinterpret_cast<HINSTANCE>
			(::GetWindowLongPtr(hWnd, GWLP_HINSTANCE)), hWnd, TCP_UDP_CTRL_PORT);

		Commander::GetSingleton ()->BroadcastInfo ();
		Commander::GetSingleton ()->BroadcastQueryInfo ();

		//--------------------------------------------------------------
		::SetWindowText(hWnd, szAppTitle);

		//--------------------------------------------------------------

        return 0;
		
    case WM_DESTROY:
		Commander::GetSingleton ()->Shutdown ();
        ::PostQuitMessage (0);

        return 0;

	case WM_COMMAND:
		//--------------------------------------------------------------
		if(Commander::GetSingleton()->GetUIManager()->ProcessEvent(
			LOWORD(wParam), lParam))

			return 0;
		//--------------------------------------------------------------

		break;

	default:
		//--------------------------------------------------------------
		if (pResult = Commander::GetSingleton()->GetUIManager()->
			ProcessEvent(message, lParam))

			return pResult;

		//--------------------------------------------------------------
		break;
    }

	//--------------------------------------------------------------
	return ::DefWindowProc(hWnd, message, wParam, lParam);

	//--------------------------------------------------------------
}


//--------------------------------------------------------------





