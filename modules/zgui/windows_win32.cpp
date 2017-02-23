#include "zgui\zgui.h"
#include "windows_win32.h"
#include "zcore\debug.h"
#include <WinUser.h>

using namespace z;

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static zWindow* hg_windows = 0;
static HINSTANCE hg_hinstance = 0;


static const char* highGUIclassName = "HighGUI class";
static const char* mainHighGUIclassName = "Main HighGUI class";

static void fillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	if (!(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32)))
		_log_("err\n");

	BITMAPINFOHEADER * bmih = &(bmi->bmiHeader);

	memset(bmih, 0, sizeof(*bmih));

	bmih->biSize = sizeof(BITMAPINFOHEADER);                // �ṹ��С = 40
	bmih->biWidth = width;                                  // �����ؼƵ�ͼ��Ŀ��
	bmih->biHeight = origin ? abs(height) : -abs(height);   // �����ؼƵ�ͼ��ĸ߶�
	bmih->biPlanes = 1;                                     // = 1
	bmih->biBitCount = (unsigned short)bpp;                 // ÿ�����ص�λ��
	bmih->biCompression = BI_RGB;                           // ѹ������

	if (bpp == 8)
	{
		RGBQUAD *palette = bmi->bmiColors;

		for (int i = 0; i < 255; ++i) {
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}
static zWindow* findWindowByName(const char* name)
{
	zWindow* window = hg_windows;

	for (; window != 0 && strcmp(name, window->name) != 0; window = window->next)
		;

	return window;
}
int zNamedWindow(const char *name, int flags)
{
	HWND hWnd;
	zWindow * window;
	DWORD style = WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
	int nameLen = 0;
	zRect rect;

	initSystem(0, 0);

	if (!name)
		_log_("NULL name string");


	if (!(flags & WINDOW_AUTOSIZE))        //YV add border in order to resize the window
		style |= WS_SIZEBOX;

	rect.x = rect.y = 100;
	rect.width = rect.height = 50;

	// ��������
	hWnd = CreateWindow(
		highGUIclassName,                // ��������   
		name,                            // ���ڱ���
		style | WS_OVERLAPPED,           // ���ڷ��
		rect.x, rect.y,                  // ��ʾλ��
		rect.width, rect.height,         // ���ڴ�С
		0,                               // ��������
		0,                               // �Ӳ˵���
		hg_hinstance,                    // �ô���Ӧ�ó����ʵ�����
		0);

	if (!hWnd)
		_log_("Frame window can not be create\n");
	ShowWindow(hWnd, SW_SHOW);

	nameLen = (int)strlen(name);

	window = (zWindow *)malloc(sizeof(zWindow) + nameLen + 1);           // Ϊ������Ϣ�����ڴ�
	window->hwnd = hWnd;                                                 // ��ǰ���ڵľ��
	window->name = (char *)(window + 1);                                 // ���ڵ�����
	memcpy(window->name, name, nameLen + 1);

	window->flags = flags;
	window->image = 0;

	window->hdc = CreateCompatibleDC(0);                                 // �ú�������һ����ָ���豸���ݵ��ڴ��豸�����Ļ���
	window->last_key = 0;
	window->status = WINDOW_NORMAL;

	window->on_mouse = 0;

	window->next = hg_windows;
	window->prev = 0;
	if (hg_windows)
		hg_windows->prev = window;

	hg_windows = window;

	SetWindowLong(hWnd, GWL_USERDATA, (size_t)window);

	return 1;
}
static bool getBitmapData(zWindow* window, SIZE* size, int* channels, void** data)
{
	BITMAP bmp;
	GdiFlush();
	HGDIOBJ h = GetCurrentObject(window->hdc, OBJ_BITMAP);

	if (size)
		size->cx = size->cy = 0;
	if (data)
		*data = 0;

	if (h == NULL)
		return true;
	if (GetObject(h, sizeof(bmp), &bmp) == 0)   //�õ�ָ��ͼ�ζ������Ϣ
		return true;

	if (size)
	{
		size->cx = abs(bmp.bmWidth);
		size->cy = abs(bmp.bmHeight);
	}

	if (channels)
		*channels = bmp.bmBitsPixel / 8;

	if (data)
		*data = bmp.bmBits;

	return false;
}


void zShowImage(const char *name, void * arr)
{
	zWindow * window;
	SIZE size = { 0, 0 };
	int channels = 0;
	void *dst_ptr = 0;
	const int channels0 = 3;
	Matrix dst, *img;
	bool change_size = false;

	if (!name)
		_log_("NULL name\n");

	window = findWindowByName(name);
    if (!window) {
        zNamedWindow(name, WINDOW_AUTOSIZE);
        window = findWindowByName(name);
    }

	if (!window)
		_log_("Error\n");

	img = (Matrix *)arr;
    
    if (window->image)
        if (getBitmapData(window, &size, &channels, &dst_ptr))
            return;

    channels = img->chs;

	if (size.cx != img->cols || size.cy != img->rows || channels != channels0)
	{
		change_size = true;

		unsigned char * buffer[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
		BITMAPINFO*binfo = (BITMAPINFO *)buffer;

		DeleteObject(SelectObject(window->hdc, window->image));
		window->image = 0;

		size.cx = img->cols;
		size.cy = img->rows;
		channels = channels0;

		fillBitmapInfo(binfo, size.cx, size.cy, channels * 8, 1);
		// CreateDIBSection�����BITMAPINFOHEADER����һ���ڴ����򣬰�����ڴ��ָ�������ṩ��pBits������
		window->image = SelectObject(window->hdc, 
            CreateDIBSection(window->hdc, binfo, DIB_RGB_COLORS, &dst_ptr, 0, 0));
	}
	if (change_size) {
		getBitmapData(window, &size, 0, 0);
		MoveWindow(window->hwnd, 50, 50, size.cx, size.cy, TRUE);
	}
	convertImage(img, &dst);
	copyToArray(dst, (char *)dst_ptr);
}


extern "C" int initSystem(int argc, char** argv)
{
	static int wasInitialized = 0;

	if (!wasInitialized) {

		hg_windows = 0;

		// ����һ�����������
		WNDCLASS win;

		win.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
		win.lpfnWndProc = WindowProc;                                          // ָ�򴰿ڹ��̺�����ָ��
		win.cbClsExtra = 0;                                                    // ��������չ��
		win.cbWndExtra = 0;                                                    // ����ʵ����չ��
		win.hInstance = hg_hinstance;                                          // ����ʵ�����
		win.lpszClassName = highGUIclassName;                                  // ָ�򴰿�������ֵ�ָ��
		win.lpszMenuName = highGUIclassName;
		win.hIcon = LoadIcon(0, IDI_APPLICATION);                              // ʹ��ȱʡͼ��
		win.hCursor = (HCURSOR)LoadCursor(0, (LPSTR)(size_t)IDC_CROSS);
		win.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);

		// ע�ᴰ����
		RegisterClass(&win);

		wasInitialized = 1;
	}
	return 0;
}


static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	zWindow * window = (zWindow *)GetWindowLong(hwnd, GWL_USERDATA);
	if (!window)
		return DefWindowProc(hwnd, uMsg, wParam, lParam);


	switch (uMsg)
	{

	case WM_SIZE:
		window->height = LOWORD(lParam);
		window->width = HIWORD(lParam);


	case WM_WINDOWPOSCHANGING:
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
		break;

	case WM_PAINT:
		if (window->image != 0)
		{
			int nchannels = 3;
			SIZE size = { 0,0 };
			HDC hdc;
			PAINTSTRUCT paint;

			getBitmapData(window, &size, &nchannels, 0);

			hdc = BeginPaint(hwnd, &paint);

			BitBlt(hdc, 0, 0, size.cx, size.cy, window->hdc, 0, 0, SRCCOPY);

			EndPaint(hwnd, &paint);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		return 0;

	case WM_ERASEBKGND:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SETCURSOR:
		return 0;

	case WM_KEYDOWN:
		window->last_key = (int)wParam;
		return 0;

	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
			SetFocus(window->hwnd);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}




int zWaitKey(int delay)
{
	int time0 = GetTickCount();

	for (;;)
	{
		zWindow * window;
		MSG message;
		int is_processed = 0;

		if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay) || hg_windows == 0)
			return -1;

		if (delay <= 0) {
			GetMessage(&message, 0, 0, 0);
		}
		else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
			Sleep(1);
			continue;
		}

		for (window = hg_windows; window != 0 && is_processed == 0; window = window->next) {
			if (window->hwnd == message.hwnd) {
				is_processed = 1;
				switch (message.message)
				{
				case WM_DESTROY:
				case WM_CHAR:
					DispatchMessage(&message);
					return (int)message.wParam;

				case WM_SYSKEYDOWN:
					if (message.wParam == VK_F10)
					{
						is_processed = 1;
						return (int)(message.wParam << 16);
					}
					break;


				case WM_KEYDOWN:
					TranslateMessage(&message);
					if ((message.wParam >= VK_F1 && message.wParam <= VK_F24) ||
						message.wParam == VK_HOME || message.wParam == VK_END ||
						message.wParam == VK_UP || message.wParam == VK_DOWN ||
						message.wParam == VK_LEFT || message.wParam == VK_RIGHT ||
						message.wParam == VK_INSERT || message.wParam == VK_DELETE ||
						message.wParam == VK_PRIOR || message.wParam == VK_NEXT)
					{
						DispatchMessage(&message);
						is_processed = 1;
						return (int)(message.wParam << 16);
					}

				default:
					DispatchMessage(&message);
					is_processed = 1;
					break;
				}
			}
		}
		if (!is_processed)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}