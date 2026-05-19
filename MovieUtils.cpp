#define BUILD_DLL
#include "MovieUtils.h"
#include <cstring>

extern "C" {
    DLL_EXPORT void SortMoviesByYear(Movie* movies, int count) {
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
    
    DLL_EXPORT double CalculateAverageRating(Movie* movies, int count) {
        if (count == 0) return 0.0;
        double sum = 0.0;
        for (int i = 0; i < count; i++) {
            sum += movies[i].rating;
        }
        return sum / count;
    }
    
    DLL_EXPORT int FindMoviesByGenre(Movie* movies, int count, const wchar_t* genre, Movie* result) {
        int found = 0;
        for (int i = 0; i < count && found < count; i++) {
            if (wcscmp(movies[i].genre, genre) == 0) {
                result[found] = movies[i];
                found++;
            }
        }
        return found;
    }
    
    DLL_EXPORT int FindMoviesByDirector(Movie* movies, int count, const wchar_t* director, Movie* result) {
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