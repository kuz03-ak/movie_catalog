Remove-Item *.o, *.exe, *.dll, *.a -Force -ErrorAction SilentlyContinue
nasm -f win64 AsmFunctions.asm -o AsmFunctions.o
g++ -shared -DBUILD_DLL MovieUtils.cpp -o MovieUtils.dll "-Wl,--out-implib,libMovieUtils.a"
g++ -shared ExplicitDLL.cpp -o ExplicitDLL.dll
g++ -c main.cpp -o main.o
g++ -c ChildWindow.cpp -o ChildWindow.o
g++ main.o ChildWindow.o AsmFunctions.o -mwindows -o MovieCatalog.exe -L. -lMovieUtils
.\MovieCatalog.exe