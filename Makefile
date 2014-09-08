
.SUFFIXES: .c .o .h .a .cpp .hpp

INCLUDEDIRS=-I../scintilla/include
CXXFLAGS= -DGTK -DSCI_LEXER -W -Wall
LEXEROBJS=$(wildcard ../scintilla/gtk/Lex*.o)

all: jabberwock

.cpp.o:
	g++ `pkg-config --cflags gtk+-3.0` $(INCLUDEDIRS) $(CXXFLAGS) -c $< -o $@

jabberwock: jabberwock.o $(LEXEROBJS) ../scintilla/bin/scintilla.a
	g++ -DGTK $^ -o $@ -lstdc++ `pkg-config --libs gtk+-3.0 gthread-2.0`

clean:
	rm -rf jabberwock *.o
