#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

char* GetFilename(int argc, char** argv){
    char* filename = nullptr;
    for (int i = 1; i < argc; i++){
        if (argv[i][0] != '-' && (argv[i - 1][0] != '-' || !strncmp(argv[i - 1], "--", 2))){
            filename = argv[i];
            break;
        }
    }
    return filename;
}

int GetNumberOfLines (int argc, char** argv) {
    int lines_num = 0;
    for (int i = 1; i < argc - 1; i++){
        if (!strcmp(argv[i], "-l")) {
            if (argv[i + 1][0] == '-' || argv[i + 1] == nullptr) { 
                cerr << "User error";
                return EXIT_FAILURE;
            } else {
                lines_num = std::atoi(argv[i + 1]);
            }
        } else if (!strncmp("--lines=", argv[i], 8)) {
            lines_num = atoi(argv[i] + 8);
        }
    }

    if (lines_num <= 0) {
        cerr << "User error";
        return EXIT_FAILURE;
    }
    return lines_num;
}

bool GetTail(int argc, char** argv){
    for (int i = 1; i < argc - 1; i++){
        if (!strcmp(argv[i], "-t") || !strncmp(argv[i], "--tail", 7)){
            return true;
        }
    }
    return false;
}

char GetDelimiter (int argc, char** argv){
    for (int i = 1; i < argc - 1; i++){
        unsigned int length1 = strlen(argv[i]);
        unsigned int length2 = strlen(argv[i + 1]);
        if (!strcmp(argv[i], "-d")) {
            if (argv[i + 1][1] == '-' || argv[i + 1] == nullptr) {
                cerr << "User error";
                return EXIT_FAILURE;
            } else {
                if (length2 == 2){
                    return argv[i + 1][1];
                } else if (length2 == 1) {
                    return argv[i + 1][0];
                }
           }
        } else if (!strncmp(argv[i], "--delimiter=", 12)){
            if (length1 == 13){
                return argv[i][12];
            } else if (length1 == 14)
            {
                return argv[i][13];
            }
        }    
    }
    return 'n';
}

int main(int argc, char **argv){
    setlocale(LC_ALL, "rus");
    char* filename = GetFilename(argc, argv);
    fstream file(filename);
    
    if (!file.is_open()){
        cout << "Error: programme can't open file" << '\n';
    }else {
        char delimiter[3] = {'\\', '\n', '\0'};
        int lines = GetNumberOfLines(argc, argv);
        bool tail = GetTail(argc, argv);
        delimiter[1] = GetDelimiter(argc, argv);
        char current_delimiter;
        if (delimiter[0] == '\\') {
            switch (delimiter[1]) {
                case 'n':
                    current_delimiter = '\n';
                    break;
                case 't':
                    current_delimiter = '\t';
                    break;
                case 'r':
                    current_delimiter = '\r';
                    break;
                case 'v':
                    current_delimiter = '\v';
                    break;
                case 'b':
                    current_delimiter = '\b';
                    break;
                case '\\':
                    current_delimiter = '\\';
                    break;
                case '\'':
                    current_delimiter = '\'';
                    break;
                case '\"':
                    current_delimiter = '\"';
                    break;
                case '0':
                    current_delimiter = '\0';
                    break;
                case '?':
                    current_delimiter = '\?';
                    break;
                default:
                    current_delimiter = delimiter[1];
                    break;
            }
        } else {
            return EXIT_FAILURE;
        }

        char current_simbol;
        if (!tail){
            while (!file.eof()){
                if (!lines){
                    break;
                }
                file.get(current_simbol);
                if (current_simbol == current_delimiter){
                    lines--;
                    cout << endl;
                } else {
                    cout << current_simbol;
                }
            }
            file.close();
        } else{
            file.seekg(-1, ios::end);
            int counter = 0;
            while (file.seekg(-1, ios::cur)) {
                if (file.peek() == current_delimiter) {
                    counter++;
                }
                if (counter == lines || (!file.tellg())) {
                    file.get();
                    cout << file.rdbuf();
                    break;
                }
            }
            file.close();
        }    
    }
    file.close();

    return 0;
}