#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <unordered_map>

#define db(a) cout << #a << " = " << a << endl;

using namespace std;

int main() {

    std::regex ngram_regex(" ([1-3])-GRAM ");
    std::smatch ngram_match;

    string y, x, line, WORDTAG_N_GRAM, ngram_string;
    int times = 0, emission_count = 0, ngram_count = 0, n;
    stringstream ss;
    unordered_map<string, unordered_map<string, double> > em_parameter;
    unordered_map<string, double> count_y;
    unordered_map<string, double> contary;
    ifstream in("gene.out.counts", ios::in);

    if (in.is_open()) {
        // while (getline(in, line) && ++times < 10) {
        while (getline(in, line)) {
            // cout << line << endl;
            ss.clear();
            ss.str(line);
            ss >> emission_count >> WORDTAG_N_GRAM >> y >> x;
            if (WORDTAG_N_GRAM == "WORDTAG") {
                em_parameter[x][y] = emission_count;
                contary[y]++;
            }
            else {
                if(std::regex_search(line, ngram_match, ngram_regex)) {
                    ss.clear();
                    ss.str(ngram_match.prefix().str());
                    ss >> ngram_count;

                    // se lee en n-grama "n" pero no se usa en este caso
                    ss.clear();
                    ss.str(ngram_match[1].str());
                    ss >> n;

                    ngram_string = ngram_match.suffix().str();
                    count_y[ngram_string] = ngram_count;
                }
            }

            ofstream outtest("outtest.out");
            unordered_map<string, double>::iterator itcount;
            for (itcount = count_y.begin(); itcount != count_y.end(); itcount++) {

                if (contary[itcount->first]) {
                    outtest << "ngram_string = " << itcount->first << " ngram_count = " << itcount->second << endl;
                    outtest << "ngram_string_orig = " << itcount->first << " ngram_count = " << contary[itcount->first] << endl;
                    outtest << endl;
                }
            }
            outtest.close();
            
        }
        in.close();

        // unordered_map<string, double>::iterator     ttt;
        // for (ttt = contary.begin(); ttt != contary.end(); ttt++) {
        //     if ((ttt->first).find(' ') == string::npos)
        //         cout << "clase y = " << ttt->first << " count_y = " << ttt->second << endl;
        // }

        unordered_map<string, double>::iterator tt;
        cout << "para key _RARE_" << endl;

        for (tt = em_parameter["_RARE_"].begin(); tt != em_parameter["_RARE_"].end(); tt++) {
            cout << "clase y = " << tt->first << " value = " <<  tt->second << endl;
        }

        unordered_map<string, unordered_map<string, double> >::iterator it;

        // iterador para y
        unordered_map<string, double>::iterator itt;
        double best_value = 0;

        for (it = em_parameter.begin(); it != em_parameter.end(); it++) {
            best_value = 0;
            for (itt = it->second.begin(); itt != it->second.end(); itt++) {
                // e(x | y) = Count(y -> x) / Count(y)
                itt->second /= contary[itt->first]; 
            }
        }

        cout << "para key _RARE_ DESPUES:" << endl;

        for (tt = em_parameter["_RARE_"].begin(); tt != em_parameter["_RARE_"].end(); tt++) {
            cout << "clase y = " << tt->first << " value = " <<  tt->second << endl;
        }

        ifstream indev("gene.dev", ios::in);
        ofstream out("gene_dev.p1.out", ios::out | ios::trunc);
        
        

        if (indev.is_open()) {
            unordered_map<string, double>::iterator it;
            double best = 0;
            string best_y = "";
            while (getline(indev, line)) {
                if (line != "") {
                    best = 0;
                    best_y = "";
                    for (it = em_parameter[line].begin(); it != em_parameter[line].end(); it++) {
                        if (best < it->second) {
                            best = it->second;
                            best_y = it->first;
                        }
                    }
                    if (best_y == "") {
                        for (it = em_parameter["_RARE_"].begin(); it != em_parameter["_RARE_"].end(); it++) {
                            if (best < it->second) {
                                best = it->second;
                                best_y = it->first;
                            }
                        }
                    }
                    out << line << " " << best_y << endl;
                }
                else
                    out << endl;
            }
            indev.close();
        }
        else {
            cout << "el archivo gen.dev no está abierto" << endl;
        }
        out.close();
    } else {
        cout << "no se puede abrir el archivo gene.counts" << endl;
    }

    return 0;
}