#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <time.h>
#include <cstdlib>
#include <gtkmm/button.h>
#include <gtkmm.h>
#include <gtkmm/application.h>
#include "type.cpp"

class MyWindow : public Gtk::Window
{
    public:
        MyWindow() {
            srand (time(NULL));
            loadCss();

            loadTypesFromCsv("chart.csv");

            
            
            m_button_super_effective = Gtk::Button("Super Effective!");
            m_button_super_effective.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::onSuperEffectiveClicked));

            m_button_effective = Gtk::Button("Effective!");
            m_button_effective.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::onEffectiveClicked));
            
            m_button_not_effective = Gtk::Button("Not Effective!");
            m_button_not_effective.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::onNotEffectiveClicked)); 
            
            m_button_procced = Gtk::Button("Procced!");
            m_button_procced.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::reset));


            m_label_left = Gtk::Label(m_types.at(0).name);
            m_label_left.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_THEME);
            m_label_left.add_css_class("label");
            m_label_left.add_css_class(m_types.at(0).name);

            m_label_right = Gtk::Label(m_types.at(0).name);
            m_label_right.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_THEME);
            m_label_right.add_css_class("label");
            m_label_right.add_css_class(m_types.at(0).name);

            m_label_solution = Gtk::Label("XXXXX");
            m_label_solution.get_style_context()->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_THEME);
            m_label_solution.add_css_class("label");

            

            reset();

            m_layout_grid = Gtk::Grid();

            for (int x = 0; x < 7; x++)
            {
            for (int y = 0; y < 6; y++)
            {
                auto empty_label = Gtk::Label("");
                m_layout_grid.attach(empty_label, x, y);
            }
            }

            m_layout_grid.attach(m_label_left, 1, 1, 2);
            m_layout_grid.attach(m_label_right, 4, 1, 2);

            m_layout_grid.attach(m_button_super_effective, 2, 3);
            m_layout_grid.attach(m_button_effective, 3, 4);
            m_layout_grid.attach(m_button_not_effective, 4, 3);
            m_layout_grid.attach(m_button_procced, 3, 2);
            m_layout_grid.attach(m_label_solution, 3, 3);

            m_layout_grid.set_row_homogeneous(true);
            m_layout_grid.set_column_homogeneous(true);

            set_child(m_layout_grid);
            set_size_request(800, 600);
        }

        void loadCss(){
          css_provider = Gtk::CssProvider::create();
          css_provider->load_from_path("main.css");
        }

        void onNotEffectiveClicked(){
          if(checkEffectiveness() == 0){
            std::cout << "Yeepie\n";
            m_label_solution.add_css_class("true");
          }else{
            m_label_solution.add_css_class("false");
          }
          //reset();
        }

        void onEffectiveClicked(){
          if(checkEffectiveness() == 1){
            std::cout << "Yeepie\n";
           m_label_solution.add_css_class("true");
          }else{
            m_label_solution.add_css_class("false");
          }
          //reset();
        }

        void onSuperEffectiveClicked(){
          if(checkEffectiveness() == 2){
            std::cout << "Yeepie\n";
           m_label_solution.add_css_class("true");
          }else{
            m_label_solution.add_css_class("false");
          }
          //reset();
        }

        int checkEffectiveness(){
            std::cout << "Help\n";
            std::cout << m_type_def->resistantAgainstTypes.size() << "res\n";
            std::cout << m_type_def->weakAgainstTypes.size() << "weak\n";
            for (int i = 0; i < m_type_def->resistantAgainstTypes.size(); i++)
            {
                if(m_type_def->resistantAgainstTypes.at(i) == m_type_atk->id){
                    return 0;
                }
            }
            for (int i = 0; i < m_type_def->weakAgainstTypes.size(); i++)
            {
                if(m_type_def->weakAgainstTypes.at(i) == m_type_atk->id){
                    return 2;
                }
            }
            return 1;
        }

        void reset(){
            m_label_solution.remove_css_class("false");
            m_label_solution.remove_css_class("true");
            m_type_atk = changeLabel(&m_label_left, rand() % m_types.size());
            m_type_def = changeLabel(&m_label_right, rand() % m_types.size());
        }

        Type* changeLabel(Gtk::Label *label, int id){
            
            label->remove_css_class(label->get_text());
            label->set_text(m_types.at(id).name);
            label->add_css_class(m_types.at(id).name);
            return &m_types.at(id);
        }

        void loadTypesFromCsv(std::string path){
            std::ifstream in_file;
            in_file.open(path);
            std::cout << "Is open:" << (in_file.is_open()?"true":"false")<<std::endl;
            std::string line;
            // Get the first line
            std::getline(in_file, line);
            parseFirstCsvLine(line);
            // Parse the Rest
            while(std::getline(in_file, line)) {
                std::cout << line << '\n';
                std::regex outer_delimiter(";");
                std::sregex_token_iterator outer_iterator(line.begin(), line.end(), outer_delimiter, -1);
                std::sregex_token_iterator outer_end;
                std::string type_string = *outer_iterator++;
                Type* currentType;
                for (auto it = m_types.begin(); it != m_types.end(); ++it) {
                    if (it->nameMatches(type_string))
                    {
                        currentType = &*it;
                        std::cout << "found match for "<<type_string << " Id: "<<currentType->id;
                    }
                }
                
                std::string resistant_string = *outer_iterator++;
                auto resistantIds = parseSubCsv(resistant_string);


                std::string weak_string = *outer_iterator;
                auto weakIds = parseSubCsv(weak_string);

                currentType->resistantAgainstTypes = resistantIds;
                currentType->weakAgainstTypes = weakIds;
            }
        }
        std::vector<int> parseSubCsv(std::string part){
            std::cout << part << '\n';
            std::regex delimiter(",");
            std::sregex_token_iterator regex_iterator(part.begin(), part.end(), delimiter, -1);
            std::sregex_token_iterator regex_end;
            std::vector<int> result = std::vector<int>();
            while (regex_iterator != regex_end) {
                auto type_string = *regex_iterator;
                
                for (auto it = m_types.begin(); it != m_types.end(); ++it) {
                    if (it->nameMatches(type_string))
                    {
                        result.push_back(it->id);
                        std::cout<< it->id<< ", ";
                    }
                }
                ++regex_iterator;
            }
            std::cout<<std::endl;
            return result;
        }

        void parseFirstCsvLine(std::string line){
                std::cout << line << '\n';
                std::regex delimiter(";");
                std::sregex_token_iterator iterator(line.begin(), line.end(), delimiter, -1);
                std::sregex_token_iterator end;
                while (iterator != end) {
                    m_types.push_back(Type(*iterator));
                    ++iterator;
                }
        }
        std::vector<Type> m_types = std::vector<Type>();

        Type *m_type_atk = new Type("Hello");
        Type *m_type_def = new Type("Hello");

        // Gtk::Picture m_picture;
        Gtk::Button m_button_procced;
        Gtk::Button m_button_super_effective;
        Gtk::Button m_button_effective;
        Gtk::Button m_button_not_effective;
        Gtk::Label m_label_left;
        Gtk::Label m_label_right;
        Gtk::Label m_label_solution;
        Gtk::Grid m_layout_grid;
        Glib::RefPtr<Gtk::CssProvider> css_provider;


};

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create("org.gtkmm.example");
    return app->make_window_and_run<MyWindow>(argc, argv);
}