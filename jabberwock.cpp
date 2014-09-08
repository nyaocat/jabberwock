#include <gtk/gtk.h>

#include <Scintilla.h>
#include <SciLexer.h>
#define PLAT_GTK 1
#include <ScintillaWidget.h>

int exit_app(GtkWidget*, GdkEventAny*, gpointer) {
   gtk_main_quit();
   return 1;
}

int main(int argc, char **argv) {
   GtkWidget *app;
   GtkWidget *editor;
   ScintillaObject *sci;

   gtk_init(&argc, &argv);
   app = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   editor = scintilla_new();
   sci = SCINTILLA(editor);

   gtk_container_add(GTK_CONTAINER(app), editor);
   g_signal_connect(G_OBJECT(app), "delete_event", G_CALLBACK(exit_app), 0);
   gtk_widget_set_size_request(app, 600, 400);

   scintilla_set_id(sci, 0);

   scintilla_send_message(sci, SCI_STYLECLEARALL, 0, 0);
   scintilla_send_message(sci, SCI_SETLEXER, SCLEX_CPP, 0);
   scintilla_send_message(sci, SCI_SETKEYWORDS, 0, reinterpret_cast<sptr_t>("int char"));
   scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
   scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x008000);
   scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_NUMBER, 0x808000);
   scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_WORD, 0x800000);
   scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
   scintilla_send_message(sci, SCI_STYLESETBOLD, SCE_C_OPERATOR, 1);
   scintilla_send_message(sci, SCI_INSERTTEXT, 0,
     reinterpret_cast<sptr_t>(
       "#include <iostream>\n"
       "\n"
       "int main() {\n"
       "    using namespace std;\n"
       "    cout << \"nyaocat\" << endl;\n"
       "}"));

   gtk_widget_show_all(app);
   gtk_widget_grab_focus(GTK_WIDGET(editor));
   gtk_main();
}

