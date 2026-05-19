#include <windows.h>
#include <vector>
#include <string>
#include "Movie.h"
#include "ChildWindow.h"
#include "MovieUtils.h"
#include "ExplicitDLL.h"

// Подключаем визуальные стили
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

std::vector<Movie> movieList;
HWND hListBox;
HMODULE hExplicitDLL = NULL;

SortByYearFunc pSortByYear = NULL;
AvgRatingFunc pAvgRating = NULL;
FindByGenreFunc pFindByGenre = NULL;
FindByDirectorFunc pFindByDirector = NULL;

void UpdateMovieListBox(int index) {
    if (index < 0 || index >= movieList.size()) return;
    
    std::wstring display;
    wchar_t rating[10];
    swprintf(rating, 10, L"%.1f", movieList[index].rating);
    
    display = L"🎬 " + std::wstring(movieList[index].title) + L"\n" +
              L"   👨 " + std::wstring(movieList[index].director) + L"  •  📅 " +
              std::to_wstring(movieList[index].year) + L"  •  ⏱️ " +
              std::to_wstring(movieList[index].duration) + L" мин\n" +
              L"   🎭 " + std::wstring(movieList[index].genre) + L"  •  ⭐ " + rating;

    if(SendMessageW(hListBox, LB_GETCOUNT, 0, 0) > (WPARAM)index) {
        SendMessageW(hListBox, LB_DELETESTRING, index, 0);
        SendMessageW(hListBox, LB_INSERTSTRING, index, (LPARAM)display.c_str());
    } else {
        SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)display.c_str());
    }
}

void DemonstrateImplicitDLL() {
    if (movieList.empty()) {
        MessageBoxW(NULL, L"Нет фильмов для демонстрации!", L"Неявная DLL", MB_OK);
        return;
    }
    
    double avgRating = CalculateAverageRating(movieList.data(), movieList.size());
    
    wchar_t message[512];
    swprintf(message, 512, L"Средний рейтинг всех фильмов: %.2f\n\n(функция из неявно загруженной DLL)", avgRating);
    MessageBoxW(NULL, message, L"Неявная загрузка DLL", MB_OK);
}

void DemonstrateExplicitDLL() {
    if (movieList.empty()) {
        MessageBoxW(NULL, L"Нет фильмов!", L"Explicit DLL", MB_OK);
        return;
    }

    if (!hExplicitDLL) {
        hExplicitDLL = LoadLibraryW(L"ExplicitDLL.dll");
        if (!hExplicitDLL) {
            MessageBoxW(NULL, L"Не удалось загрузить ExplicitDLL.dll!", L"Ошибка", MB_OK | MB_ICONERROR);
            return;
        }

        pSortByYear = (SortByYearFunc)GetProcAddress(hExplicitDLL, "SortMoviesByYearExplicit");
        if (!pSortByYear) {
            MessageBoxW(NULL, L"Не найдена функция SortMoviesByYearExplicit!", L"Ошибка", MB_OK | MB_ICONERROR);
            return;
        }
    }

    // Глубокое копирование!
    std::vector<Movie> sortedMovies(movieList.begin(), movieList.end());
    
    pSortByYear(sortedMovies.data(), (int)sortedMovies.size());

    std::wstring message = L"Фильмы, отсортированные по году:\n\n";
    
    for (size_t i = 0; i < sortedMovies.size() && i < 6; ++i) {
        const auto& m = sortedMovies[i];
        wchar_t line[512] = {0};
        swprintf(line, 512, L"%s (%d) — %s, рейтинг: %.1f\n", 
                 m.title, m.year, m.director, m.rating);
        message += line;
    }

    MessageBoxW(NULL, message.c_str(), L"Explicit DLL - Sort by year", MB_OK | MB_ICONINFORMATION);
}

void DemonstrateAsmFunction() {
    if (movieList.empty()) {
        MessageBoxW(NULL, L"No movies to analyze!", L"Assembler", MB_OK);
        return;
    }
    
    // Создаём массив указателей на названия
    std::vector<const wchar_t*> titles;
    for (const auto& movie : movieList) {
        titles.push_back(movie.title);
    }
    
    // Вызываем ассемблерную функцию
    int longTitles = CountLongTitles(titles.data(), (int)titles.size(), 10);
    
    // Формируем сообщение БЕЗ конкатенации напрямую
    wchar_t message[1024];
    swprintf(message, 1024, 
             L"Movies with title longer than 10 chars: %d out of %d\n\n(assembler function x64)", 
             longTitles, (int)movieList.size());
    
    if (longTitles > 0) {
        wcscat_s(message, 1024, L"\n\nExamples:\n");
        int shown = 0;
        for (size_t i = 0; i < movieList.size() && shown < 5; i++) {
            if (wcslen(movieList[i].title) > 10) {
                wchar_t line[256];
                swprintf(line, 256, L"• %s (%d chars)\n", movieList[i].title, (int)wcslen(movieList[i].title));
                wcscat_s(message, 1024, line);
                shown++;
            }
        }
    }
    
    MessageBoxW(NULL, message, L"Assembler Function", MB_OK);
}

LRESULT CALLBACK ParentProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            // Создаём красивый список с эмодзи
            hListBox = CreateWindowW(L"LISTBOX", L"", 
                WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL | WS_BORDER,
                20, 50, 660, 280, hwnd, NULL, NULL, NULL);
            
            // Устанавливаем шрифт
            HFONT hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            SendMessageW(hListBox, WM_SETFONT, (WPARAM)hFont, TRUE);
            
            // Создаём кнопки
            CreateWindowW(L"BUTTON", L"➕ Добавить фильм", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 350, 150, 40, hwnd, (HMENU)1, NULL, NULL);
            
            CreateWindowW(L"BUTTON", L"✏️ Редактировать", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                180, 350, 150, 40, hwnd, (HMENU)2, NULL, NULL);
            
            CreateWindowW(L"BUTTON", L"📊 Неявная DLL", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                350, 350, 130, 40, hwnd, (HMENU)3, NULL, NULL);
            
            CreateWindowW(L"BUTTON", L"🔧 Явная DLL", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                490, 350, 130, 40, hwnd, (HMENU)4, NULL, NULL);
            
            CreateWindowW(L"BUTTON", L"⚡ Ассемблер", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                630, 350, 120, 40, hwnd, (HMENU)5, NULL, NULL);
            
            break;
        }
        
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(255, 255, 255));
            SetBkColor(hdcStatic, RGB(32, 33, 36));
            return (LRESULT)CreateSolidBrush(RGB(32, 33, 36));
        }
        
        case WM_COMMAND: {
            if(LOWORD(wParam) == 1) {
                OpenChildWindow(hwnd);
            } else if(LOWORD(wParam) == 2) {
                int index = SendMessageW(hListBox, LB_GETCURSEL, 0, 0);
                if(index != LB_ERR && index < movieList.size()) {
                    OpenChildWindow(hwnd, &movieList[index]);
                } else {
                    MessageBoxW(hwnd, L"Пожалуйста, выберите фильм для редактирования!", 
                               L"Внимание", MB_OK | MB_ICONINFORMATION);
                }
            } else if(LOWORD(wParam) == 3) {
                DemonstrateImplicitDLL();
            } else if(LOWORD(wParam) == 4) {
                DemonstrateExplicitDLL();
            } else if(LOWORD(wParam) == 5) {
                DemonstrateAsmFunction();
            }
            break;
        }
        
        case WM_SEND_MOVIE: {
            Movie* editingMovie = (Movie*)wParam;
            Movie* newMovie = (Movie*)lParam;

            if(editingMovie) {
                *editingMovie = *newMovie;
                int index = editingMovie - &movieList[0];
                UpdateMovieListBox(index);
            } else {
                movieList.push_back(*newMovie);
                UpdateMovieListBox(movieList.size() - 1);
            }
            break;
        }

        case WM_DESTROY: {
            if (hExplicitDLL) {
                FreeLibrary(hExplicitDLL);
            }
            PostQuitMessage(0);
            break;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int ncmdshow) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = ParentProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"ParentWindow";
    wc.hbrBackground = CreateSolidBrush(RGB(32, 33, 36));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(L"ParentWindow", L"🎬 Кинотека - Каталог фильмов",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 450,
        NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, ncmdshow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while(GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}