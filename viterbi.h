#ifndef VITERBI_H
#define VITERBI_H
#include <unordered_map>

using namespace std;

class Viterbi {
private:
    unordered_map<int, unordered_map<string,  unordered_map<string, int> > > pi;
    unordered_map<int, unordered_map<string,  unordered_map<string, string> > > bp;
    unordered_map<string, double> q;
    unordered_map<string, unordered_map<string, double> > emision;
public:
    Viterbi();
    ~Viterbi();
    void process(string, unordered_map<string, double>, unordered_map<string, unordered_map<string, double> >, vector<string>);
    vector<string> viterbi(vector<string> words, unordered_map<string, double> q, unordered_map<string, unordered_map<string, double> > em, vector<string> v);
};

Viterbi::Viterbi() {}
Viterbi::~Viterbi() {}
void Viterbi::process(string filename, unordered_map<string, double> q, unordered_map<string, unordered_map<string, double> > em, vector<string> v) {


    string line;
    cout << filename << endl;
    ifstream in(filename, ios::in);
    ofstream out("gene_dev.p2.out", ios::out | ios::trunc);

    cout << "linea inside viterbi: " << endl;
    vector<string> words;
    while (getline(in, line)) {
        if (line == "") { // es salto de linea
            vector<string> tag_sequence = this->viterbi(words, q, em, v);
            db(tag_sequence.size());
            for (int i = 0; i < tag_sequence.size(); i++) {
                // db(tag_sequence[i]);
                out << words[i] << " " << tag_sequence[i] << endl;
            }
            out << endl;
            words.clear();
        }
        words.push_back(line);
    }
    out.close();
    in.close();
    this->q = q;
    this->emision = em;
    cout << "do_viterbi was called" << endl;
}

vector<string> Viterbi::viterbi(vector<string> words, unordered_map<string, double> q, unordered_map<string, unordered_map<string, double> > em, vector<string> v) {

    // puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    // for (int i = 0; i < v.size(); i++) {
    //     cout << v[i] << endl;
    // }
    // puts(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    //size of sentence
    int n = words.size();
    vector < vector<string> > S(n + 2, v);
    vector<string> asterick(1, "*");
    S[0] = S[1] = asterick;

    // db(S.size());
    // for (int i = 0; i < S.size(); ++i) {
    //     cout << "i = " << i << endl;
    //     for (int j = 0; j < S[i].size(); ++j) {
    //         cout << S[i][j] << "\t";
    //     }
    //     cout << endl;
    // }

    double maxw = 0, prod = 0;
    // db(words.size());
    // for (int i = 0; i < words.size(); i++)
    // {
    //     db(words[i]);
    // }
    pi.clear();
    pi[1]["*"]["*"] = 1;
    // viterbi starts here
    int contark = 0, contaru = 0, contarv = 0, contarw = 0;
    for (int k = 2; k < n + 2; k++) {
        // db(k);
        // contark++;
        for (int u = 0; u < S[k - 1].size(); u++) {
            // db(u);
            // contaru++;
            for (int v = 0; v < S[k].size(); v++) {
                // db(v);
                // contarv++;
                maxw = 0, prod = 0;
                for (int w = 0; w < S[k - 2].size(); w++) {
                    // db(w);
                    contarw++;

                    cout << "para pi:" << endl;
                    db3( k , S[k - 1][u] , S[k][v] );
                    cout << "para k - 1" << endl;
                    db3(k - 1, S[k - 2][w], S[k - 1][u]);
                    cout << "para q:" << endl;
                    cout << S[k][v] + "|" + S[k - 2][w] + " " + S[k - 1][u] << endl;
                    cout << "=====================================================================" << endl;
                    cout << "foobar = " << S[k][v] + "|" + S[k - 2][w] + " " + S[k - 1][u] << " contarw = " << contarw << endl;
                    cout << q[  S[k][v] + "|" + S[k - 2][w] + " " + S[k - 1][u] ] << endl;
                    cout << "=====================================================================" << endl;
                    prod = pi[k - 1][ S[k - 2][w] ][ S[k - 1][u] ] * q[  S[k][v] + "|" + S[k - 2][w] + " " + S[k - 1][u] ] * em[words[k - 2]][S[k][v]];

                    db2(prod, maxw);

                    if (prod > maxw) {
                        maxw = prod;
                        bp[ k ][ S[k - 1][u] ] [ S[k][v] ] = S[k - 2][w];
                        db(bp[ k ][ S[k - 1][u] ] [ S[k][v] ]);
                    }
                }
                pi[ k ][ S[k - 1][u] ] [ S[k][v] ] = maxw;
            }
        }
    }
    db(contark);
    db(contaru);
    db(contarv);
    db(contarw);
    cout << endl;
    cout << endl;
    cout << endl;

    return vector<string>();


    vector<string> seq(n + 2);
    for (int u = 0; u < S[n].size(); u++) {
        maxw = 0, prod = 0;
        for (int v = 0; v < S[n + 1].size(); v++) {
            prod = pi[n + 1][ S[n][u] ][ S[n + 1][v] ] * q[ "STOP|" + S[n][u] + " " + S[n + 1][v] ];
            if (prod > maxw) {
                maxw       = prod;
                seq[n]     = S[n][u];
                seq[n + 1] = S[n + 1][v];
            }
        }
        for (int k = n - 1; k >= 2; k--) {
            seq[k] = bp[k + 2][seq[k + 1]][seq[k + 2]];
        }

        vector<string> res(seq.begin() + 2, seq.end());
        return res;
    }
}

#endif