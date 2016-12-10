#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>

using namespace std;

int main() {
    unordered_map<string, int> map_word;
    unordered_map<string, int>::iterator it;
    ifstream in("gene.train", ios::in);
    stringstream ss;
    string word, tagword, line;
    if (in.is_open()) {
        while (getline(in, line)) {
            if (line == "") continue;
            ss.clear();
            ss << line;
            ss >> word >> tagword;
            map_word[word]++;
        }

        ofstream out("gene.out.train", ios::out | ios::trunc);
        in.clear();
        in.seekg(0, ios::beg);
        while (getline(in, line)) {
            if (line == "")  {
                out << endl;
                continue;
            }
            ss.clear();
            ss << line;
            ss >> word >> tagword;
            if (map_word[word] < 5) {
                out << "_RARE_ " << tagword << endl;
            }
            else {
                out << word << " " << tagword << endl;
            }
        }
        out.close();
        in.close();
    } else {
        cout << "no se puede abrir el archivo gene.counts" << endl;
    }

    return 0;
}