#include <gtkmm.h>
#include <Scintilla.h>
#include <SciLexer.h>
#define PLAT_GTK 1
#include <ScintillaWidget.h>

Glib::RefPtr<Gtk::UIManager> create_menu() {
  Glib::ustring ui_info = "<ui>"
                          "  <menubar name='Menubar'>"
                          "    <menu name='File' action='File'>"
                          "      <menuitem name='Open' action='Open'/>"
                          "      <separator/>"
                          "      <menuitem name='Quit' action='Quit'/>"
                          "    </menu>"
                          "  </menubar>"
                          "</ui>";

  auto actions = Gtk::ActionGroup::create("menu");

  actions->add(Gtk::Action::create("File", "_File"));
  actions->add(Gtk::Action::create("Open", Gtk::Stock::OPEN));
  actions->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
               sigc::ptr_fun(Gtk::Main::quit));

  auto ui = Gtk::UIManager::create();

  ui->insert_action_group(actions);
  ui->add_ui_from_string(ui_info);
  return ui;
}

int main(int argc, char *argv[]) {

  Gtk::Main kit(argc, argv);

  Gtk::Window window;
  window.set_default_size(600, 400);

  Gtk::Box box(Gtk::ORIENTATION_VERTICAL, 2);
  window.add(box);

  box.pack_start(*create_menu()->get_widget("/Menubar"), Gtk::PACK_SHRINK);
  {
    GtkWidget *editor;
    ScintillaObject *sci;

    editor = scintilla_new();
    sci = SCINTILLA(editor);

    scintilla_set_id(sci, 0);
    scintilla_send_message(sci, SCI_STYLECLEARALL, 0, 0);
    scintilla_send_message(sci, SCI_SETLEXER, SCLEX_CPP, 0);
    scintilla_send_message(sci, SCI_SETKEYWORDS, 0,
                           reinterpret_cast<sptr_t>("int char"));
    scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
    scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x008000);
    scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_NUMBER, 0x808000);
    scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_WORD, 0x800000);
    scintilla_send_message(sci, SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
    scintilla_send_message(sci, SCI_STYLESETBOLD, SCE_C_OPERATOR, 1);
    scintilla_send_message(sci, SCI_SETCODEPAGE, SC_CP_UTF8, 0);
    scintilla_send_message(
        sci, SCI_INSERTTEXT, 0,
        reinterpret_cast<sptr_t>("#include <iostream>\n"
                                 "\n"
                                 "int main() {\n"
                                 "    using namespace std;\n"
                                 "    cout << \"nyaocat\" << endl;\n"
                                 "}"));

    box.pack_start(*Glib::wrap(editor), true, true, 0);
  }

  window.show_all();
  Gtk::Main::run(window);
}
