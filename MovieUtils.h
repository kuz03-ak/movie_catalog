#pragma once
#include <windows.h>
#include "Movie.h"

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

extern "C" {
    // Функции для работы с фильмами (неявная загрузка)
    DLL_EXPORT void SortMoviesByYear(Movie* movies, int count);
    DLL_EXPORT double CalculateAverageRating(Movie* movies, int count);
    DLL_EXPORT int FindMoviesByGenre(Movie* movies, int count, const wchar_t* genre, Movie* result);
    DLL_EXPORT int FindMoviesByDirector(Movie* movies, int count, const wchar_t* director, Movie* result);
    
    // Функция на ассемблере
    // Ассемблерная функция - новая сигнатура
    int CountLongTitles(const wchar_t* titles[], int count, int minLength);
}