// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9

#include <getopt.h>
#include "project1.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
using namespace std;

class Letterman {
    
public:
    
    void read_data();

    // Read and process command line arguments.
    void get_options(int argc, char** argv);
    
    bool search();

    void output();
    
    
private:
    
    vector<Dictionary> dictionary;

    size_t begin;
    string b;
    string e;
    int morph = 0;

    size_t end;

    bool stackMode = false;
    bool queueMode = false;

    // Signifies what we attribute we are sorting by.
    bool change = false;
    bool length = false;
    bool swap = false;
    string out;
};

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
 
    try {
        // Instantiate a music library.
        Letterman letterman;
        
        // Read and process the command line options.
        letterman.get_options(argc, argv);
        
        // Read in the provided file through stdin.
        letterman.read_data();
        
        // Sort the data with the provided policy and print
        // the first 'num_print' songs.
        if (letterman.search()) {
            letterman.output();
        }
    }

    // Catch runtime_errors, print the message, and exit the
    // program with a non-zero status.
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // All done!
    return 0;
}

// Read and process command line options.
void Letterman::get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;

    /*

        TODO: Add the remaining elements into the longOpts array.

    */
    // use getopt to find command line options
    struct option longOpts[] = {{ "queue", no_argument, nullptr, 'q' },
                                { "help", no_argument, nullptr, 'h' },
                                { "stack", no_argument, nullptr, 's' },
                                { "begin", required_argument, nullptr, 'b' },
                                { "output", required_argument, nullptr, 'o' },
                                { "change", no_argument, nullptr, 'c' },
                                { "length", no_argument, nullptr, 'l' },
                                { "swap", no_argument, nullptr, 'p' },
                                { "end", required_argument, nullptr, 'e' },
                                { nullptr, 0, nullptr, '\0' }};
    
    /*

        TODO: Add the remaining chars to the option string in
                the while loop conditional (currently contains "p:h").
                Options with required_argument (print) need a colon after the
                char, options with no_argument do not (help).

    */
    while ((option = getopt_long(argc, argv, "qhsb:o:clpe:", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'q':
                queueMode = true;
                break;     

            case 's':
                stackMode = true;
                break;

            case 'b': {
                string str(optarg);
                b = str;
                break;
            }

            case 'o':
                out = optarg;
                if (out != "W" && out != "M") {
                    throw std::runtime_error("Invalid output mode specified");
                }
                break;

            case 'c':
                change = true;
                break;

            case 'l':
                length = true;
                break;

            case 'p':
                swap = true;
                break;

            case 'e': {
                string str(optarg);
                e = str;
                break;
            }
                     
            case 'h':
                std::cout << "This program reads a CSV file that contains song names,\n"
                          << "the artist who wrote them, and the number of plays each song\n"
                          <<  "has on Spotify.  It then outputs the number of songs specified\n"
                          <<  "in the command line arguments (the print option), which\n"
                          <<  "defaults to 2, sorted by the option specified (one of name,\n"
                          <<  "artist, or listens).\n"
                          <<  "Usage: \'./project0\n\t[--listens | -l]\n"
                          <<                      "\t[--name | -n]\n"
                          <<                      "\t[--artist | -a]\n"
                          <<                      "\t[--print | -p] <# of songs to print>\n"
                          <<                      "\t[--help | -h]\n"
                          <<                      "\t< <CSV Music File>\'" << std::endl;
                exit(0);
        }
    }

    if (b.size() == 0) {
        throw std::runtime_error("Beginning word not specified");
    }
    if (e.size() == 0) {
        throw std::runtime_error("Ending word not specified");
    }
    if (stackMode == true && queueMode == true) {
        throw std::runtime_error("Conflicting or duplicate stack and queue specified");
    }
    if (stackMode == false && queueMode == false) {
        throw std::runtime_error("Must specify one of stack or queue");
    }
    if (change == false && length == false && swap == false) {
        throw std::runtime_error("Must specify at least one modification mode (change length swap)");
    }
    if (length == false && b.size() != e.size()) {
        throw std::runtime_error("The first and last words must have the same length when length mode is off");
    }
        
}

bool Letterman::search() {
    deque<size_t> sc;
    size_t current = begin;
    int num_discovered = 1;
    while (current != end) {
        for (size_t i = 0; i < dictionary.size(); ++i) {
            if (i != current) {
                if (length && dictionary[i].discovered == false && (dictionary[current].word.size() != dictionary[i].word.size())) {
                    if ((dictionary[current].word).size() > (dictionary[i].word).size() && ((dictionary[current].word).size() - 1) == (dictionary[i].word).size()) {
                            for (size_t j = 0; j < (dictionary[current].word).size(); ++j) {
                                string w = dictionary[current].word;
                                int ind = static_cast<int>(j);
                                w.erase(w.begin() + ind);
                                if (w == dictionary[i].word && dictionary[i].word[j] != dictionary[i].word[j + 1]) {
                                    sc.push_back(i);
                                    dictionary[i].discovered = true;
                                    dictionary[i].prev = current;
                                    ++num_discovered;
                                    dictionary[i].morph_type = 'd';
                                    dictionary[i].morph_index = j;
                                    if (i == end) {
                                        return true;
                                    }
                                    break;
                                }
                            }
                    } else if ((dictionary[current].word).size() < (dictionary[i].word).size() && ((dictionary[current].word).size() + 1) == (dictionary[i].word).size()) {
                        for (size_t j = 0; j < (dictionary[i].word).size(); ++j) {
                                string w = dictionary[current].word;
                                w.insert(j, 1, (dictionary[i].word)[j]);                             
                                if (w == dictionary[i].word && dictionary[i].word[j] != dictionary[i].word[j + 1]) {
                                    sc.push_back(i);
                                    dictionary[i].discovered = true;
                                    dictionary[i].prev = current;
                                    ++num_discovered;
                                    dictionary[i].morph_type = 'i';
                                    dictionary[i].morph_index = j;
                                    dictionary[i].morph_letter = w[j];
                                    if (i == end) {
                                        return true;
                                    }
                                    break;
                                }
                            }
                        }   
                    }
                if (change && dictionary[i].discovered == false && (dictionary[current].word.size() == dictionary[i].word.size())) {
                    for (size_t j = 0; j < (dictionary[i].word).size(); ++j) {
                            string w = dictionary[current].word;
                            w[j] = (dictionary[i].word)[j];
                            if (w == dictionary[i].word) {
                                sc.push_back(i);
                                dictionary[i].discovered = true;
                                dictionary[i].prev = current;
                                ++num_discovered;
                                dictionary[i].morph_type = 'c';
                                dictionary[i].morph_letter = w[j];
                                dictionary[i].morph_index = j;
                                if (i == end) {
                                    return true;
                                }
                                break;
                        }
                    }
                }         
                if (swap && dictionary[i].discovered == false && (dictionary[i].word).size() == (dictionary[current].word).size()) {
                    for (size_t j = 0; j < (dictionary[i].word).size() - 1; ++j) {
                        string w = dictionary[current].word;
                        std::swap(w[j], w[j+1]);
                        if (w == dictionary[i].word) {
                            sc.push_back(i);
                            dictionary[i].discovered = true;
                            dictionary[i].prev = current;
                            ++num_discovered;
                            dictionary[i].morph_type = 's';
                            dictionary[i].morph_index = j;
                            if (i == end) {
                                return true;
                            }
                            break;
                        }
                    }
                }
            }
        }
        if (sc.size() == 0) {
            cout << "No solution, " << num_discovered << " words discovered." << endl;
            return false;
        }
    if (stackMode) {
        current = sc.back();
        sc.pop_back();
    }else {
        current = sc.front();
        sc.pop_front();
    }
}
return true;
}

void Letterman::output() {
    size_t curr = end;
    vector<Dictionary> d;
        if (out == "M") {
        while (curr != std::string::npos) {
        d.push_back(dictionary[curr]);
        curr = dictionary[curr].prev;
        ++morph;
    }
    cout << "Words in morph: " << morph << endl;
    for (size_t i = d.size() - 1; i != std::string::npos; --i) {
        if (d[i].morph_type == 'i') {
            cout << "i," << d[i].morph_index << "," << d[i].morph_letter << endl;
        }else if (d[i].morph_type == 'c') {
            cout << "c," << d[i].morph_index << "," << d[i].morph_letter << endl;
        }else if (d[i].morph_type == 's') {
            cout << "s," << d[i].morph_index << endl;
        }else if (d[i].morph_type == 'd') {
            cout << "d," << d[i].morph_index << endl;
        }else {
            cout << d[i].word << endl;
        }
    }
    }else {
        while (curr != std::string::npos) {
        d.push_back(dictionary[curr]);
        curr = dictionary[curr].prev;
        ++morph;
    }
    cout << "Words in morph: " << morph << endl;
    for (size_t i = d.size() - 1; i != std::string::npos; --i) {
        cout << d[i].word << endl;
    }
    }
    }


void Letterman::read_data() {
    
    char mode;
    std::string plays;
    size_t num_words = 0;
    string word;
    size_t x = 0;

    // Read in the first number to find the number
    // of songs in the file.
    // Also consume the new line after the integer.
    std::cin >> mode >> std::ws >> num_words >> std::ws;

    // Reserve the exact amount of memory needed
    // to fit all the songs.
    dictionary.reserve(size_t(num_words));
    size_t num_space = num_words;

    // Read to the end of the CSV file.
    if (mode == 'S') {
        while (cin >> word) {
            if (word[0] == '/') {
                std::getline(cin, word);
            }else {
                Dictionary dict;
                dict.word = word;
                if (word == b) {
                    begin = x;
                    dict.discovered = true;
                }else if (word == e) {
                    end = x;
                }
                ++x;
                dictionary.push_back(dict);
            }
        }
    }else if (mode == 'C') {
        while (cin >> word) {
            if (word[0] == '/') {
                std::getline(cin, word);
            }else {
                if (word.find('[') != std::string::npos) {
                    size_t i1 = word.find('[');
                    size_t i2 = word.find(']');
                    size_t diff = i2 - i1;
                    string ins = word.substr(i1, diff + 1);
                    word.erase(i1, diff + 1);
                    for (size_t i = 1; i < ins.size() - 1; ++i) {
                        string w = word;
                        w.insert(i1, 1, ins.at(i));
                        Dictionary dict;
                        dict.word = w;
                        if (w == b) {
                        begin = x;
                        dict.discovered = true;
                        }else if (w == e) {
                        end = x;
                        }
                        if (x == num_space) {
                            num_space += num_words;
                            dictionary.reserve(num_space);
                        }
                        ++x;
                        dictionary.push_back(dict);
                    }
                }else if (word.find('!') != std::string::npos) {
                    size_t i2 = word.find('!');
                    int ind = static_cast<int>(i2);
                    word.erase(word.begin() + ind);
                    Dictionary dict;
                    dict.word = word;
                    if (word == b) {
                    begin = x;
                    dict.discovered = true;
                }else if (word == e) {
                    end = x;
                }
                if (x == num_space) {
                            num_space += num_words;
                            dictionary.reserve(num_space);
                        }
                ++x;
                dictionary.push_back(dict);
                    char a = word.at(i2 - 2);
                    word.at(i2 - 2) = word.at(i2 - 1);
                    word.at(i2 - 1) = a;
                    Dictionary d;
                    d.word = word;
                    if (word == b) {
                    begin = x;
                    d.discovered = true;
                }else if (word == e) {
                    end = x;
                }
                if (x == num_space) {
                            num_space += num_words;
                            dictionary.reserve(num_space);
                        }
                ++x;
                dictionary.push_back(d);
                }else if (word.find('&') != std::string::npos) {
                    size_t i1 = word.find('&');
                    int ind = static_cast<int>(i1);
                    word.erase(word.begin() + ind);
                    Dictionary dict;
                    dict.word = word;
                    if (word == b) {
                    begin = x;
                    dict.discovered = true;
                }else if (word == e) {
                    end = x;
                }
                if (x == num_space) {
                            num_space += num_words;
                            dictionary.reserve(num_space);
                        }
                ++x;
                dictionary.push_back(dict);
                    string w;
                    for (size_t i = word.size() - 1; i != std::string::npos; --i) {
                        w.push_back(word.at(i));
                    }
                    Dictionary d;
                    d.word = w;
                    if (w == b) {
                    begin = x;
                    d.discovered = true;
                }else if (w == e) {
                    end = x;
                }
                if (x == num_space) {
                            num_space += num_words;
                            dictionary.reserve(num_space);
                        }
                ++x;
                dictionary.push_back(d);
                }else if (word.find('?') != std::string::npos) {
                    size_t index = word.find('?');
                    int ind = static_cast<int>(index);
                    word.erase(word.begin() + ind);
                    Dictionary dict;
                    dict.word = word;
                    if (word == b) {
                    begin = x;
                    dict.discovered = true;
                }else if (word == e) {
                    end = x;
                }
                if (x == num_space) {
                            num_space += num_words;
                            dictionary.reserve(num_space);
                        }
                ++x;
                dictionary.push_back(dict);
                    char a = word.at(index - 1);
                    word.insert(index - 1, 1, a);
                    Dictionary d;
                    d.word = word;
                    if (word == b) {
                    begin = x;
                    d.discovered = true;
                }else if (word == e) {
                    end = x;
                }
                if (x == num_space) {
                            num_space += num_words;
                            dictionary.reserve(num_space);
                        }
                ++x;
                dictionary.push_back(d);
                }else {
                    Dictionary dict;
                dict.word = word;
                if (word == b) {
                    begin = x;
                    dict.discovered = true;
                }else if (word == e) {
                    end = x;
                }
                if (x == num_space) {
                            num_space += num_words;
                            dictionary.reserve(num_space);
                        }
                ++x;
                dictionary.push_back(dict);
                }
            }
        }
        
    }
        // If we didn't read in any data, throw an error.
    if (!dictionary.size()) {
        throw std::runtime_error("No data was read in! Refer to the help option to see program usage.");
    }
    if (dictionary[begin].word != b) {
        throw std::runtime_error("Beginning word does not exist in the dictionary");
    }
    if (dictionary[end].word != e) {
        throw std::runtime_error("Ending word does not exist in the dictionary");
    }
}
