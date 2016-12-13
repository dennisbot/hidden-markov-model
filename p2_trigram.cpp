#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <vector>
#include <set>

#define db(a) cout << #a << " = " << a << endl;
#define db2(a,b) cout << #a << " = " << a << " " << #b << " = " << b << endl;
#define db3(a,b,c) cout << #a << " = " << a << " " << #b << " = " << b << " " << #c << " = " << c << endl;

#include "viterbi.h"

using namespace std;



int main() {
    Viterbi *v = new Viterbi();
    std::regex ngram_regex(" ([1-3])-GRAM ");
    std::smatch ngram_match;

    string y, x, line, WORDTAG_N_GRAM, ngram_string;
    string yi_2, yi_1, yi;

    int times = 0, emission_count = 0, ngram_count = 0, n;
    stringstream ss;
    unordered_map<string, unordered_map<string, double> > em_parameter;
    unordered_map<string, double> count_y;
    unordered_map<string, unordered_map<string, double> > count2;
    unordered_map<string, unordered_map<string, double> > count3;
    unordered_map<string, double> q;
    set<string> S;

    ifstream in("nenas.counts", ios::in);

    int counts = 0;

    if (in.is_open()) {
        // while (getline(in, line) && ++times < 10) {
        while (getline(in, line)) {
            // cout << line << endl;
            ss.clear();
            ss.str(line);
            ss >> emission_count >> WORDTAG_N_GRAM >> y >> x;

            ofstream foo;
            if (WORDTAG_N_GRAM == "WORDTAG") {
                em_parameter[x][y] = emission_count;
                S.insert(y);
            }
            else {
                // cout << line << endl;
                if(std::regex_search(line, ngram_match, ngram_regex)) {
                    ss.clear();
                    ss.str(ngram_match.prefix().str());
                    ss >> ngram_count;

                    // se lee en n-grama "n" pero no se usa en este caso
                    ss.clear();
                    ss.str(ngram_match[1].str());
                    ss >> n;
                    ngram_string = ngram_match.suffix().str();
                    if (n == 2) { // es un bigrama
                        // q[]
                        ss.clear();
                        ss.str(ngram_string);
                        ss >> yi_2 >> yi_1;
                        // db2(yi_2, yi_1);
                        // cout << "ngram_string = " << ngram_string << endl;
                        // cout << "============================================" << endl;
                        count2[yi_2][yi_1] = ngram_count;
                    }
                    if (n == 3) { // es un trigrama
                        // q[]
                        ss.clear();
                        ss.str(ngram_string);
                        ss >> yi_2 >> yi_1 >> yi;
                        // cout << "counts#" << counts++ << endl;
                        // db3(yi_2, yi_1, yi);
                        // cout << "ngram_string = " << ngram_string << endl;
                        // cout << "============================================" << endl;

                        count3[yi_2 + " " + yi_1][yi] = ngram_count;
                    }

                    // cout << "n = " << n << " string = " << ngram_string << " line = " << line << endl;
                    count_y[ngram_string] = ngram_count;
                }
            }



            ofstream outtest("outtest.out", ios::out | ios::trunc);
            unordered_map<string, double>::iterator itcount;
            for (itcount = count_y.begin(); itcount != count_y.end(); itcount++) {
                outtest << "ngram_string = " << itcount->first << " ngram_count = " << itcount->second << endl;
            }
            outtest.close();
        }
        in.close();

        ofstream trans_emit("transmision_emision.txt", ios::out | ios::trunc);

        // calculamos q
        unordered_map<string, unordered_map<string, double> >::iterator itq;
        unordered_map<string, double>::iterator itqq;
        for (itq = count3.begin(); itq != count3.end(); itq++) {
            for (itqq = itq->second.begin(); itqq != itq->second.end(); itqq++) {
                ss.clear();
                ss.str(itq->first);
                // db(itq->first);
                // db(itqq->first + "|" + itq->first);
                // cout << "===============================" << endl;
                ss >> yi_2 >> yi_1;
                //Count(yi | yi − 2, yi − 1) / Count(yi − 2, yi − 1)
                q[itqq->first + "|" + itq->first] = itqq->second * 1. / count2[yi_2][yi_1];
            }
        }
        // mostramos valores de q

        times = 0;
        for (itqq = q.begin(); itqq != q.end(); itqq++) {
            trans_emit << ++times << ".- q[" << itqq->first << "] = " << itqq->second << endl;
        }

        unordered_map<string, unordered_map<string, double> >::iterator it;

        // iterador para y
        unordered_map<string, double>::iterator itt;
        double best_value = 0;

        for (it = em_parameter.begin(); it != em_parameter.end(); it++) {
            best_value = 0;
            for (itt = it->second.begin(); itt != it->second.end(); itt++) {
                // e(x | y) = Count(y -> x) / Count(y)
                itt->second /= count_y[itt->first];
            }
        }
        cout << endl;
        times = 0;
        for (it = em_parameter.begin(); it != em_parameter.end(); it++) {
            best_value = 0;
            for (itt = it->second.begin(); itt != it->second.end(); itt++) {
                // e(x | y) = Count(y -> x) / Count(y)
                trans_emit << ++times << ".- e(" << it->first << "|" << itt->first << ") = " << itt->second << endl;
            }
        }


        trans_emit.close();

        std::vector<string> myvec(S.begin(), S.end());

        v->process("nenas.dev", q, em_parameter, myvec);
        delete v;

    }
    return 0;
}