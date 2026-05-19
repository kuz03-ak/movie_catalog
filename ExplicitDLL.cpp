#include <windows.h>
#include "Movie.h"
#include <cstring>

extern "C" {
    __declspec(dllexport) void SortMoviesByYearExplicit(Movie* movies, int count) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (movies[j].year > movies[j + 1].year) {
                    Movie temp = movies[j];
                    movies[j] = movies[j + 1];
                    movies[j + 1] = temp;
                }
            }
        }
    }
    
    __declspec(dllexport) double CalculateAverageRatingExplicit(Movie* movies, int count) {
        if (count == 0) return 0.0;
        double sum = 0.0;
        for (int i = 0; i < count; i++) {
            sum += movies[i].rating;
        }
        return sum / count;
    }
    
    __declspec(dllexport) int FindMoviesByGenreExplicit(Movie* movies, int count, 
                                                          const wchar_t* genre, Movie* result) {
        int found = 0;
        for (int i = 0; i < count && found < count; i++) {
            if (wcscmp(movies[i].genre, genre) == 0) {
                result[found] = movies[i];
                found++;
            }
        }
        return found;
    }
    
    __declspec(dllexport) int FindMoviesByDirectorExplicit(Movie* movies, int count,
                                                             const wchar_t* director, Movie* result) {
        int found = 0;
        for (int i = 0; i < count && found < count; i++) {
            if (wcscmp(movies[i].director, director) == 0) {
                result[found] = movies[i];
                found++;
            }
        }
        return found;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}