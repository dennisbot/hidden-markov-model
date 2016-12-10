#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

int main() {
    ifstream in("gene.train", ios::in);
    string line, gene, grupo;
    stringstream ss;
    unordered_map<string, int> um;
    unordered_set<string> us_infrecuent;
    int cont = 0;
    if (in.is_open()) {
        // while (getline(in, line) && ++cont < 18) {
        while (getline(in, line)) {
            // cout << line << endl;
            ss.clear();
            ss.str(line);
            ss >> gene >> grupo;
            // cout << gene << " - " << grupo << endl;
            um[gene]++;
        }
        unordered_map<string, int>::iterator it;

        int thesize = distance(um.begin(), um.end());
        printf("the number of distint genes is : %d\n", thesize);
        for (it = um.begin(); it != um.end(); it++) {
            if (it->second < 5) us_infrecuent.insert(it->first);
        }
        printf("the number of infrecuent words is : %d\n", us_infrecuent.size());
        in.clear();
        in.seekg(0, ios::beg);
        ofstream out("gene_rare_.train", ios::out);
        while (getline(in, line)) {
            // cout << line << endl;
            ss.clear();
            ss.str(line);
            ss >> gene >> grupo;
            if (us_infrecuent.find(gene) != us_infrecuent.end()) {
                out << "_RARE_ " << grupo << endl;
            }
            else {
                out << gene << " " << grupo << endl;
            }
        }
        out.close();
        in.close();
    } else {
        cout << "no se puede abrir el archivo" << endl;
    }
    return 0;
}