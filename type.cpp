#include <iostream>
#include <vector>

static int currentId = 0;

class Type {
    public:
    Type(const std::string name){
        id = ++currentId;
        this->name = name;
        std::cout << "Type " << this->name << " constructed " << id << "\n";
    };
    
    ~Type(){
        resistantAgainstTypes.~vector();
        weakAgainstTypes.~vector();
        std::cout << "Type " << this->name << " destructed " << id << "\n";
    }

    bool nameMatches(std::string comparison){
            return name.compare(comparison) == 0;
    }

    std::string  name;
    int id;

    std::vector<int> resistantAgainstTypes = std::vector<int>();
    std::vector<int> weakAgainstTypes = std::vector<int>();
};