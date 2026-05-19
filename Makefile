CC = g++
NASM = nasm
CFLAGS = -c -Wall
LDFLAGS = -mwindows

all: MovieCatalog.exe MovieUtils.dll ExplicitDLL.dll

MovieCatalog.exe: main.o ChildWindow.o AsmFunctions.o
	$(CC) main.o ChildWindow.o AsmFunctions.o $(LDFLAGS) -o MovieCatalog.exe

MovieUtils.dll: MovieUtils.cpp
	$(CC) -shared -DBUILD_DLL MovieUtils.cpp -o MovieUtils.dll

ExplicitDLL.dll: ExplicitDLL.cpp
	$(CC) -shared ExplicitDLL.cpp -o ExplicitDLL.dll

AsmFunctions.o: AsmFunctions.asm
	$(NASM) -f win64 AsmFunctions.asm -o AsmFunctions.o

main.o: main.cpp Movie.h ChildWindow.h MovieUtils.h ExplicitDLL.h
	$(CC) $(CFLAGS) main.cpp -o main.o

ChildWindow.o: ChildWindow.cpp ChildWindow.h Movie.h
	$(CC) $(CFLAGS) ChildWindow.cpp -o ChildWindow.o

clean:
	del *.o *.exe *.dll

run: MovieCatalog.exe
	./MovieCatalog.exe