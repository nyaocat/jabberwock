
.SUFFIXES: .c .o .h .a .cpp .hpp

INCLUDEDIRS=-I../scintilla/include
CXXFLAGS= -DGTK -DSCI_LEXER -W -Wall
LEXEROBJS=$(wildcard ../scintilla/gtk/Lex*.o)

all: jabberwock

.cpp.o:
	g++ -std=c++11 `pkg-config --cflags gtkmm-3.0` $(INCLUDEDIRS) $(CXXFLAGS) -c $< -o $@

jabberwock: jabberwock.o $(LEXEROBJS) ../scintilla/bin/scintilla.a
	g++ -DGTK $^ -o $@ -lstdc++ `pkg-config --libs gtkmm-3.0 gthread-2.0 gmodule-2.0`

clean:
	rm -rf jabberwock *.o
