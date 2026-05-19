#pragma once
#include <windows.h>
#include "Movie.h"

// Типы функций для явной загрузки
typedef void (*SortByYearFunc)(Movie*, int);
typedef double (*AvgRatingFunc)(Movie*, int);
typedef int (*FindByGenreFunc)(Movie*, int, const wchar_t*, Movie*);
typedef int (*FindByDirectorFunc)(Movie*, int, const wchar_t*, Movie*);