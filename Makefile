
.SUFFIXES: .c .o .h .a .cpp .hpp

INCLUDEDIRS=-I./scintilla/include
CXXFLAGS= -DGTK -DSCI_LEXER -W -Wall `pkg-config --cflags gtkmm-3.0`
LEXEROBJS=$(wildcard ./scintilla/gtk/Lex*.o)

all: jabberwock

.cpp.o:
	g++ -std=c++11  $(INCLUDEDIRS) $(CXXFLAGS) -c $< -o $@

jabberwock: jabberwock.o $(LEXEROBJS) ./scintilla/bin/scintilla.a
	g++ -DGTK $^ -o $@ -lstdc++ `pkg-config --libs gtkmm-3.0 gthread-2.0 gmodule-2.0`

./scintilla/bin/scintilla.a:scintilla350.tgz
	tar zxvf scintilla350.tgz
	(cd scintilla/gtk; make GTK3=1)

scintilla350.tgz:
	wget http://prdownloads.sourceforge.net/scintilla/scintilla350.tgz


clean:
	rm -rf jabberwock *.o
