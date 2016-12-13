void show_pi(unordered_map<int, unordered_map<string,  unordered_map<string, int> > > pi) {
    unordered_map<int, unordered_map<string,  unordered_map<string, int> > >::iterator itum;
    unordered_map<string, unordered_map<string, int> >::iterator itum2;
    unordered_map<string, int>::iterator itum3;

    for (itum = pi.begin(); itum != pi.end(); itum++) {
        cout << "itum = " << itum->first << endl;
        for (itum2 = itum->second.begin(); itum2 != itum->second.end(); itum2++) {
            cout << "itum2 = " << itum2->first << endl;
            for (itum3 = itum2->second.begin(); itum3 != itum2->second.end(); itum3++) {
                cout << "key = " << itum3->first << " val = " << itum3->second << endl;
            }

        }
    }
}

    unordered_map<int, unordered_map<string,  unordered_map<string, int> > > pi;

    pi[1]["*"]["*"] = 12;

    cout << "show antes: " << endl;
    show_pi(pi);

    pi.clear();

    cout << "show despues: " << endl;
    show_pi(pi);
    pi[4]["*"]["*"] = 55;
    cout << "show nuevamente: " << endl;
    show_pi(pi);

    return 0;