#include "ChildWindow.h"
#include <windows.h>
#include <string>

HWND hTitle, hDirector, hYear, hDuration, hRating, hGenre;

LRESULT CALLBACK ChildProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static Movie* editingMovie = nullptr;
    static HWND hParent;
    static HFONT hLabelFont, hEditFont, hBtnFont;

    switch(msg) {
        case WM_CREATE: {
            editingMovie = (Movie*)((CREATESTRUCT*)lParam)->lpCreateParams;
            hParent = ((CREATESTRUCT*)lParam)->hwndParent;
            
            // Создаём шрифты
            hLabelFont = CreateFontW(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            hEditFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            hBtnFont = CreateFontW(14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                  DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                  DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

            // Создаём элементы с эмодзи и красивыми названиями
            CreateWindowW(L"STATIC", L"🎬 Название:", WS_CHILD | WS_VISIBLE, 20, 20, 100, 25, hwnd, NULL, NULL, NULL);
            hTitle = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 130, 18, 250, 28, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"STATIC", L"👨 Режиссер:", WS_CHILD | WS_VISIBLE, 20, 55, 100, 25, hwnd, NULL, NULL, NULL);
            hDirector = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 130, 53, 250, 28, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"STATIC", L"📅 Год:", WS_CHILD | WS_VISIBLE, 20, 90, 100, 25, hwnd, NULL, NULL, NULL);
            hYear = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 130, 88, 100, 28, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"STATIC", L"⏱️ Длительность (мин):", WS_CHILD | WS_VISIBLE, 20, 125, 130, 25, hwnd, NULL, NULL, NULL);
            hDuration = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 123, 100, 28, hwnd, NULL, NULL, NULL);
            
            CreateWindowW(L"STATIC", L"⭐ Рейтинг (0-10):", WS_CHILD | WS_VISIBLE, 20, 160, 110, 25, hwnd, NULL, NULL, NULL);
            hRating = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 158, 100, 28, hwnd, NULL, NULL, NULL);
            
            CreateWindowW(L"STATIC", L"🎭 Жанр:", WS_CHILD | WS_VISIBLE, 20, 195, 100, 25, hwnd, NULL, NULL, NULL);
            hGenre = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 130, 193, 200, 28, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"BUTTON", L"💾 Сохранить", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                         120, 240, 120, 35, hwnd, (HMENU)1, NULL, NULL);
            
            // Устанавливаем шрифты
            SendMessageW(hTitle, WM_SETFONT, (WPARAM)hEditFont, TRUE);
            SendMessageW(hDirector, WM_SETFONT, (WPARAM)hEditFont, TRUE);
            SendMessageW(hYear, WM_SETFONT, (WPARAM)hEditFont, TRUE);
            SendMessageW(hDuration, WM_SETFONT, (WPARAM)hEditFont, TRUE);
            SendMessageW(hRating, WM_SETFONT, (WPARAM)hEditFont, TRUE);
            SendMessageW(hGenre, WM_SETFONT, (WPARAM)hEditFont, TRUE);
            
            HWND hBtn = GetDlgItem(hwnd, 1);
            SendMessageW(hBtn, WM_SETFONT, (WPARAM)hBtnFont, TRUE);

            // Если редактируем существующий фильм — заполняем поля
            if(editingMovie) {
            SetWindowTextW(hTitle, editingMovie->title);
            SetWindowTextW(hDirector, editingMovie->director);
            SetWindowTextW(hGenre, editingMovie->genre);
    
            wchar_t buf[64];
    
            // Год
            _snwprintf_s(buf, 64, _TRUNCATE, L"%d", editingMovie->year);
            SetWindowTextW(hYear, buf);
    
            // Длительность
            _snwprintf_s(buf, 64, _TRUNCATE, L"%d", editingMovie->duration);
         SetWindowTextW(hDuration, buf);
    
            // Рейтинг - ОЧЕНЬ ВАЖНО: используем правильный формат
         _snwprintf_s(buf, 64, _TRUNCATE, L"%.1f", editingMovie->rating);
         SetWindowTextW(hRating, buf);
        }
            break;
        }
        
        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORBTN: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(255, 255, 255));
            SetBkColor(hdcStatic, RGB(48, 49, 52));
            return (LRESULT)CreateSolidBrush(RGB(48, 49, 52));
        }
        
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;
            RECT rc;
            GetClientRect(hwnd, &rc);
            HBRUSH hBrush = CreateSolidBrush(RGB(48, 49, 52));
            FillRect(hdc, &rc, hBrush);
            DeleteObject(hBrush);
            return TRUE;
        }

        case WM_COMMAND: {
            if(LOWORD(wParam) == 1) {
                Movie movie;

                GetWindowTextW(hTitle, movie.title, 100);
                GetWindowTextW(hDirector, movie.director, 50);
                GetWindowTextW(hGenre, movie.genre, 30);

                wchar_t buf[50];
                GetWindowTextW(hYear, buf, 20); movie.year = _wtoi(buf);
                GetWindowTextW(hDuration, buf, 20); movie.duration = _wtoi(buf);
                GetWindowTextW(hRating, buf, 20); movie.rating = _wtof(buf);

                SendMessageW(hParent, WM_SEND_MOVIE, (WPARAM)editingMovie, (LPARAM)&movie);
                DestroyWindow(hwnd);
            }
            break;
        }

        case WM_DESTROY: {
            DeleteObject(hLabelFont);
            DeleteObject(hEditFont);
            DeleteObject(hBtnFont);
            break;
        }

        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void OpenChildWindow(HWND hParent) {
    OpenChildWindow(hParent, nullptr);
}

void OpenChildWindow(HWND hParent, Movie* movie) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = ChildProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = L"ChildWindow";
    wc.hbrBackground = CreateSolidBrush(RGB(48, 49, 52));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(
        L"ChildWindow",
        movie ? L"✏️ Редактировать фильм" : L"➕ Добавить фильм",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        150, 150, 430, 330,
        hParent, NULL, GetModuleHandleW(NULL), movie
    );

    ShowWindow(hwnd, SW_SHOW);
}