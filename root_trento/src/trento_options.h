#ifndef trento_options_h
#define trento_options_h

#include "TObject.h"
#include "TROOT.h" 
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "events_info_CLASS.h"
#include "event_CLASS.h"



#include <iostream>
using namespace std;

class trento_options: public TObject{
private:
    Int_t number_of_events;
    TString target, projectile, nevents_str, Tcmd, out_file;
    map<TString, TString> options{};
    string cmd;
    bool multicore = false;

public:
    trento_options();

    void Get_options(events_info_CLASS * inf){
        options = inf->Get();
    };
    template <typename T> void Set(TString s1, T s2);
    // void Multicore(bool b){
    //     multicore = b;
    // }
    void Set_command();

    void Set_output_file(TString s){
        out_file = s;
    }
    void Run_trento();

    void merge_files(vector<TString> file, TString outfile);

    //ClassDef(trento_options, 1)	    
};


#endif