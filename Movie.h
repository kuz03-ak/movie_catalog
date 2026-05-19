#pragma once
#include <windows.h>
#include <cstring>

#define WM_SEND_MOVIE (WM_USER + 1)

struct Movie {
    wchar_t title[100];
    wchar_t director[50];
    int year;
    int duration;
    double rating;
    wchar_t genre[30];

    Movie() {
        title[0] = L'\0';
        director[0] = L'\0';
        genre[0] = L'\0';
        year = 0;
        duration = 0;
        rating = 0.0;
    }

    Movie(const Movie& other) {
        wcscpy_s(title, 100, other.title);
        wcscpy_s(director, 50, other.director);
        wcscpy_s(genre, 30, other.genre);
        year = other.year;
        duration = other.duration;
        rating = other.rating;
    }

    Movie& operator=(const Movie& other) {
        if (this != &other) {
            wcscpy_s(title, 100, other.title);
            wcscpy_s(director, 50, other.director);
            wcscpy_s(genre, 30, other.genre);
            year = other.year;
            duration = other.duration;
            rating = other.rating;
        }
        return *this;
    }
};