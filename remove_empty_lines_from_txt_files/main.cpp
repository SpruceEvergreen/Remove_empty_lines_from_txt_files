// This little program is for transforming in a certain way txt files (in this particular case
// empty lines, empty lines with tab and lines with ? symbol are removed from the file, also 
// first occurance of a comma in a line is replaced with a semicolon).
// First the user is promted to put a path for the directory where the files they wish to transform are located.
// If the directory doesn't exist, the user can create it.
// After that the program checks if the directory is empty. If it's empty, the program asks the
// user to populate it with txt files. After that the program lists all the files
// located in the chosen directory. Then the user can either transfrom all the files 
// in the current directory or choose specific files (by entering their names without .txt) to transform

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string ask_the_path();
void transform_all_files(std::vector<std::string> out_directory_content_vector);
void transform_one_file(std::vector<std::string> out_directory_content_vector, std::string path_name);

// asking the path to where the files are
std::string ask_the_path() {
    bool done = false;
    std::string path_holder {};

    do {
        int control_counter {0};
        
        std::cout << "Enter the path: ";
        
        std::getline(std::cin, path_holder);
        std::filesystem::path path_to_create = path_holder;
        if (std::filesystem::exists(path_to_create)) {
            control_counter++;
        }
        
        if (control_counter == 0) {
            std::cout << "This directory doesn't exist" << std::endl;
            std::cout << "Do you want to create this directory (Y/N)?: ";
            std::string selection {};
            std::getline(std::cin, selection);
            std::transform(selection.begin(), selection.end(), selection.begin(),
                   [](unsigned char c) { return std::toupper(c); });
            if (selection == "Y") {
                std::filesystem::create_directory(path_holder);
                done = true;
            } else if (selection == "N") {
                std::cout << "Ready for entering new path" << std::endl;
            } else {
                std::cout << "Unavailable option, try again" << std::endl;
            } 
        } else {
            std::cout << "This directory already exists" << std::endl;
            std::cout << "Do you want to transform files in this directory (Y/N)?: ";
            std::string selection {};
            std::getline(std::cin, selection);
            std::transform(selection.begin(), selection.end(), selection.begin(),
                   [](unsigned char c) { return std::toupper(c); });
            if (selection == "Y") {
                done = true;
            } else if (selection == "N") {
                std::cout << "Ready for entering new path" << std::endl;
            } else {
                std::cout << "Unavailable option, try again" << std::endl;
            }
        }

    } while (!done);
    return path_holder;
}

void transform_all_files(std::vector<std::string> out_directory_content_vector) {
    for (auto &elem : out_directory_content_vector) {
                std::string line {};
                std::fstream original_file;
                std::ofstream without_file;
                std::string name_of_out_file {elem};
                // insert _without_line to the name of the file
                auto it = name_of_out_file.find(".txt");
                name_of_out_file.insert(it, "_without_line");
                // opening text files to read from and to write to
                original_file.open(elem);
                without_file.open(name_of_out_file);
                
                // transforming data and writing it to the file
                while(getline(original_file, line)) {
                    if (line != "" && (line.find('?') == std::string::npos) && line != "	") {
                        // replacing first occurance of a comma with a semicolon
                        auto it = std::find(line.begin(), line.end(), ',');
                        *it = ';';
                        without_file << line << std::endl;
                    }
                }
                original_file.close();
                without_file.close();
            }
            std::cout << "Done!" << std::endl;
}

void transform_one_file(std::vector<std::string> out_directory_content_vector, std::string path_name, int &main_loop_counter) {
    std::string name_holder {};
    std::string name_of_file {};
    std::string name_of_out_file {};
    int transform_one_file_counter {0};
    do {
        if (transform_one_file_counter == 0) {
            std::cout << "Please, enter name of the file without .txt (or Q to quit): ";
        } else {
            std::cout << "If you want to transform another file, enter its name without .txt (or Q to quit): ";
        }
        
        std::getline(std::cin, name_holder);
        if (name_holder != "Q" && name_holder != "q") {
            name_of_file = path_name;
            name_of_file.append("\\");
            name_of_file.append(name_holder);
            name_of_out_file = name_of_file;
                    
            name_of_file.append(".txt");
            name_of_out_file.append("_without_line.txt");
            
            if (!std::filesystem::exists(name_of_file)) {
                std::cout << "File doesn't exist" << std::endl;
            } else {
            
                std::fstream original_file;
                std::ofstream without_file;
                    
                original_file.open(name_of_file);
                without_file.open(name_of_out_file);
                    
                std::string line {};
                
                // removing empty lines, tab lines and lines with ? symbol in them
                while(getline(original_file, line)) {
                    if (line != "" && (line.find('?') == std::string::npos) && line != "	") {
                        // replacing first occurance of a comma with a semicolon
                        auto it = std::find(line.begin(), line.end(), ',');
                        *it = ';';
                        without_file << line << std::endl;
                        }
                }   
                original_file.close();
                without_file.close();
                std::cout << "Done!" << std::endl;
                transform_one_file_counter++;
            }
        } else {
            std::string inner_loop_selection {};
            do {
                std::cout << "To end the program, enter E. To return to the previous menu, enter R: ";
                std::getline(std::cin, inner_loop_selection);
                if (inner_loop_selection == "E" || inner_loop_selection == "e") {
                    std::cout << "Quiting" << std::endl;
                    main_loop_counter++; 
                } else if (inner_loop_selection == "R" || inner_loop_selection == "r") {
                    std::cout << "Returning to the previous menu" << std::endl; 
                } else {
                    std::cout << "Unavailable option" << std::endl;
                }
            } while(inner_loop_selection != "R" && inner_loop_selection != "r" && inner_loop_selection != "E" && inner_loop_selection != "e");
            
        }
    } while (name_holder != "Q" && name_holder != "q");
}

int main() {
    // asking and getting the path for the directory
    std::string path_name {};
    path_name = ask_the_path();
    // checking if the directory is empty. If the directory is empty then asking to add files
    do {
        std::string selection{};
        if (std::filesystem::is_empty(path_name)) {
            std::cout << "The directory has no files. Please add files" << std::endl;
            std::cout << "After adding files, hit enter: ";
            std::getline(std::cin, selection);
            if (selection.length() != 0) {
                std::cout << "Please, try again" << std::endl;
            }
        }
    } while(std::filesystem::is_empty(path_name));
    
    // creating a text file to store the names of the files in the directory  
    std::ofstream directory_content;
    directory_content.open("directory_content.txt");
    // printing and writing to the text file the names of the files that the directory has
    std::cout << "The directory has following files: " << std::endl;
    for (const auto & entry : std::filesystem::directory_iterator(path_name)) {
        std::cout << entry.path() << std::endl;
        directory_content << entry.path() << std::endl;
    }
    // creating the vector to store file names and populating the vector reading from the file directory_content.txt
    std::cout << std::endl;
    std::fstream out_directory_content {};
    std::string out_directory_content_string;
    std::vector<std::string> out_directory_content_vector {};
    out_directory_content.open("directory_content.txt");
    while(getline(out_directory_content, out_directory_content_string)) {
        out_directory_content_vector.push_back(out_directory_content_string);
    }
    // removing quotes from the strings in the out_directory_content_vector
    for (auto &elem:  out_directory_content_vector) {
        elem.erase(std::remove(elem.begin(), elem.end(), '"'), elem.end());
    }
    // main menu loop 
    std::string answer_selection {};
    int main_loop_counter {0};
    do {
        std::cout << "Do you want to transform all the files in the current directory?(Y/N/Q): "; 
        std::getline(std::cin, answer_selection);
        if (answer_selection == "Y" || answer_selection == "y") {
            transform_all_files(out_directory_content_vector);
            main_loop_counter++;
        } else if (answer_selection == "N" || answer_selection == "n") {
            transform_one_file(out_directory_content_vector, path_name, main_loop_counter);
        } else if (answer_selection == "Q" || answer_selection == "q") {
            std::cout << "Quiting" << std::endl;
        } else {
            std::cout << "Unavailable option" << std::endl;
        }
    } while (answer_selection != "Q" && answer_selection != "q" && main_loop_counter == 0);

	return 0;
}