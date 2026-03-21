#include <iostream>
#include <gtkmm/button.h>
#include <gtkmm.h>
#include <gtkmm/application.h>


class MyWindow : public Gtk::Window
{
    public:
        MyWindow() {

          loadCss();

          m_button = Gtk::Button("Test");
          m_button.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::onButtonClicked));

          m_label = Gtk::Label("Hello!");
          m_label.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_THEME);
          m_label.add_css_class("label");

          m_picture = Gtk::Picture("/home/david/Pictures/arg.png");

          m_layout_grid = Gtk::Grid();
          m_layout_grid.attach(m_button, 0, 0);
          m_layout_grid.attach(m_label, 1, 1);
          m_layout_grid.attach(m_picture, 2, 2);

          set_child(m_layout_grid);
          set_size_request(800, 600);
        }

        void onButtonClicked(){
          std::cout << "Hello\n";
        }

        void loadCss(){
          css_provider = Gtk::CssProvider::create();
          css_provider->load_from_path("main.css");
        }

        Gtk::Picture m_picture;
        Gtk::Button m_button;
        Gtk::Label m_label;
        Gtk::Grid m_layout_grid;
        Glib::RefPtr<Gtk::CssProvider> css_provider;
};

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create("org.gtkmm.example");
    return app->make_window_and_run<MyWindow>(argc, argv);
}