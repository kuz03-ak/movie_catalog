#pragma once
#include <windows.h>

// ---------------- пользовательское сообщение ----------------
#define WM_SEND_MOVIE (WM_USER + 1)

// ---------------- структура фильма ----------------
struct Movie {
    wchar_t title[100];     // Название фильма
    wchar_t director[50];   // Режиссер
    int year;               // Год выпуска
    int duration;           // Длительность в минутах
    double rating;          // Рейтинг (0.0 - 10.0)
    wchar_t genre[30];      // Жанр
};