#include <gtk/gtk.h>

#include <Scintilla.h>
#include <SciLexer.h>
#define PLAT_GTK 1
#include <ScintillaWidget.h>

void open(GtkAction *, gpointer )
{
  g_print("未実装");
}

void quit(GtkAction *action, gpointer user_data)
{
  GObject *window = G_OBJECT(user_data);
  g_object_unref(g_object_get_data(window, "ui"));
  gtk_main_quit();
}

const gchar *menu_info =
  "<ui>"
  "  <menubar name='Menubar'>"
  "    <menu name='File' action='File'>"
  "      <menuitem name='Open' action='Open'/>"
  "      <separator/>"
  "      <menuitem name='Quit' action='Quit'/>"
  "    </menu>"
  "  </menubar>"
  "</ui>";

GtkActionEntry entries[] = {
  {"File", NULL, "_File"},
  {"Open", GTK_STOCK_OPEN, "_Open", "<control>O", "Open file", G_CALLBACK(open)},
  {"Quit", GTK_STOCK_QUIT, "_Quit", "<control>Q", "Quit this program", G_CALLBACK(quit)},
};

GtkUIManager* create_menu(GtkWidget *parent)
{
  GtkUIManager *ui;
  GtkActionGroup *actions;

  ui = gtk_ui_manager_new();
  actions = gtk_action_group_new("menu");
  gtk_action_group_add_actions(actions, entries, sizeof(entries) / sizeof(entries[0]), parent);
  gtk_ui_manager_insert_action_group(ui, actions, 0);
  gtk_ui_manager_add_ui_from_string(ui, menu_info, -1, NULL);
  gtk_window_add_accel_group(GTK_WINDOW(parent), gtk_ui_manager_get_accel_group(ui));
  return ui;
}

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

   GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
   gtk_container_add(GTK_CONTAINER(app), vbox);
   GtkUIManager *ui;
   GtkWidget *menubar;

   ui = create_menu(app);
   g_object_set_data(G_OBJECT(app), "ui", (gpointer)ui);
   menubar = gtk_ui_manager_get_widget(ui, "/Menubar");
   gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), editor, TRUE, TRUE, 0);

   gtk_widget_show_all(app);
   gtk_widget_grab_focus(GTK_WIDGET(editor));
   gtk_main();
}

