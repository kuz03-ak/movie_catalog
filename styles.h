#pragma once
#include <windows.h>

// Цветовая схема (современная тёмная тема)
struct AppColors {
    static constexpr COLORREF BACKGROUND = RGB(32, 33, 36);      // Тёмно-серый фон
    static constexpr COLORREF PANEL_BG = RGB(48, 49, 52);        // Панели
    static constexpr COLORREF BUTTON_BG = RGB(66, 133, 244);     // Синий (Google Blue)
    static constexpr COLORREF BUTTON_HOVER = RGB(81, 148, 255);  // Светло-синий
    static constexpr COLORREF TEXT_PRIMARY = RGB(255, 255, 255);  // Белый текст
    static constexpr COLORREF TEXT_SECONDARY = RGB(189, 193, 198); // Светло-серый
    static constexpr COLORREF BORDER = RGB(95, 99, 104);          // Рамки
    static constexpr COLORREF SUCCESS = RGB(52, 168, 83);         // Зелёный
    static constexpr COLORREF WARNING = RGB(251, 188, 5);         // Жёлтый
};

// Функции для создания красивого интерфейса
void SetModernFont(HWND hwnd);
void CenterWindow(HWND hwnd);