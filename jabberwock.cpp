#include <fstream>
#include <vector>
#include <gtkmm.h>
#include <Scintilla.h>
#include <SciLexer.h>
#define PLAT_GTK 1
#include <ScintillaWidget.h>

ScintillaObject *sci;

void on_open() {
  Gtk::FileChooserDialog dialog("select file", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  if (dialog.run() == Gtk::RESPONSE_OK) {
    std::ifstream ifs(dialog.get_filename());
    ifs.seekg(0, ifs.end);
    std::vector<char> buf(ifs.tellg());
    ifs.seekg(0, ifs.beg);
    ifs.read(buf.data(), buf.size());

    scintilla_send_message(sci, SCI_CLEARALL, SC_CP_UTF8, 0);
    scintilla_send_message(sci, SCI_INSERTTEXT, 0,
                           reinterpret_cast<sptr_t>(buf.data()));
  }
}

void on_save() {
  Gtk::FileChooserDialog dialog("select file", Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
  if (dialog.run() == Gtk::RESPONSE_OK) {
    std::vector<char> buf(scintilla_send_message(sci, SCI_GETLENGTH, 0, 0));
    scintilla_send_message(sci, SCI_GETTEXT, buf.size() + 1,
                           reinterpret_cast<sptr_t>(buf.data()));

    std::ofstream ofs(dialog.get_filename());
    ofs.write(buf.data(), buf.size());
  }
}

Glib::RefPtr<Gtk::UIManager> create_menu() {
  Glib::ustring ui_info = "<ui>"
                          "  <menubar name='Menubar'>"
                          "    <menu name='File' action='File'>"
                          "      <menuitem name='Open' action='Open'/>"
                          "      <menuitem name='Save' action='Save'/>"
                          "      <separator/>"
                          "      <menuitem name='Quit' action='Quit'/>"
                          "    </menu>"
                          "  </menubar>"
                          "  <toolbar name='Toolbar'>"
                          "    <toolitem action='Open' />"
                          "    <toolitem action='Save' />"
                          "    <toolitem action='Quit' />"
                          "  </toolbar>"
                          "</ui>";

  auto actions = Gtk::ActionGroup::create("menu");

  actions->add(Gtk::Action::create("File", "_File"));
  actions->add(Gtk::Action::create("Open", Gtk::Stock::OPEN),
               sigc::ptr_fun(on_open));
  actions->add(Gtk::Action::create("Save", Gtk::Stock::SAVE),
               sigc::ptr_fun(on_save));
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

  auto ui = create_menu();
  if (auto menubar = ui->get_widget("/Menubar")) {
    box.pack_start(*menubar, Gtk::PACK_SHRINK);
  }
  if (auto toolbar = ui->get_widget("/Toolbar")) {
    static_cast<Gtk::Toolbar *>(toolbar)->set_toolbar_style(Gtk::TOOLBAR_BOTH);
    box.pack_start(*toolbar, Gtk::PACK_SHRINK);
  }
  {
    GtkWidget *editor;

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
