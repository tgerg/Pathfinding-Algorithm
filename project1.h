// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9

#include <vector>
#include <string>

struct Dictionary {
    std::string word;
    size_t prev = std::string::npos;
    bool discovered = false;  
    char morph_type;
    char morph_letter;
    size_t morph_index;
};