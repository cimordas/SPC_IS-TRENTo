#include "trento_options.h"
#include "event_CLASS.h"
#include "events_info_CLASS.h"

#include "TObject.h"
#include "TROOT.h" 
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TChain.h"

#include <iostream>
#include <fstream>
// #include <omp.h>

using namespace std;

trento_options::trento_options(){};

void trento_options::Set_command(){

    Tcmd = "./trento";

    if (options["constit-width"] == "") options["constit-width"] = options["nucleon-width"];
    Tcmd += Form(" %s %s %s", options["target"].Data(), options["projectile"].Data(), options["nevents"].Data());

    if (options["constit-width"] > options["nucleon-width"]){
        cout << "\"constit-width\" should be smaller than or equal to \"nucleon-width\". " << endl;
        return;
    }
    
    for (pair<TString, TString> element : options){
        if (element.second != "" && element.first != "target" && element.first != "projectile" && element.first != "nevents"){
            Tcmd += Form(" --%s %s", element.first.Data(), element.second.Data());     
        }
    }
    cmd = Tcmd.Data();
}

void trento_options::Run_trento(){

    TTree * event_tree = new TTree("trento_events","a simple Tree with class");
    /*===================================*/
    
    Set_command();
    cout << " Trento command:  " << cmd << endl;

    event_CLASS* sEvent = new event_CLASS();
    event_tree->Branch("events",  &sEvent );

    string data;
    FILE * stream;
    const int max_buffer = 512;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");

    stringstream line;
    istream_iterator<string> begin;
    istream_iterator<string> end;
    vector<string> token_str{};
    vector<double> token{};
    if (stream) {
        Int_t nevents = 0;
        while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL) {
            line = stringstream(buffer);
            begin = istream_iterator<string>(line);
            token_str = vector<string>(begin, end);
            
            if (token_str[0] == "#") continue; //skip the first line;
            
           // cout << line.str() << endl;
            /* Here we change the strings into double.*/
            
            for (auto& n : token_str){
                token.push_back( atof(n.c_str()) );
            }
               
            sEvent->Set_b(token[1]);
            sEvent->Set_npart(token[2]);
            sEvent->Set_nch(token[3]);   

            // cout << "nevents: " << nevents << endl;

            if (options["long-output"] == "0"){  

                sEvent->Set_epsilon(2, token[4], token[8]);
                sEvent->Set_r(2, token[12]);

                sEvent->Set_epsilon(3, token[5], token[9]);
                sEvent->Set_r(3, token[13]);

                sEvent->Set_epsilon(4, token[6], token[10]);
                sEvent->Set_r(4, token[14]);

                sEvent->Set_epsilon(5, token[7], token[11]);
                sEvent->Set_r(5, token[15]);         

            } else {

                sEvent->Set_epsilon(2, token[4], token[11]);
                sEvent->Set_r(2, token[18]);

                sEvent->Set_epsilon(3, token[5], token[12]);
                sEvent->Set_r(3, token[19]);

                sEvent->Set_epsilon(4, token[6], token[13]);
                sEvent->Set_r(4, token[20]);

                sEvent->Set_epsilon(5, token[7], token[14]);
                sEvent->Set_r(5, token[21]);  

                sEvent->Set_epsilon(6, token[8], token[15]);
                sEvent->Set_r(6, token[22]); 

                sEvent->Set_epsilon(7, token[9], token[16]);
                sEvent->Set_r(7, token[23]); 

                sEvent->Set_epsilon(8, token[10], token[17]);
                sEvent->Set_r(8, token[24]);   

            }                    
            
            event_tree->Fill();
            
            /*-------------------------*/
            token = vector<double>{};

            nevents++;
            if (nevents % 10000 == 0) cout << "event: " << nevents  << endl;     
        

        };

        unique_ptr<TFile> f(TFile::Open(out_file,"recreate"));
        
        event_tree->Write();
        //f.WriteObject(&write_info, "events_information");
        //////////////////////////////////////////////
        ///////////  How to read the info ////////////
        // events_info_CLASS *read_info;
        // f.GetObject("events_information", read_info);
        // cout << "read ===== " << read_info->Get()["nevents"] << endl;
        //////////////////////////////////////////////

        pclose(stream);
    } 
    /////////////////////////////
    /* The information of run is saved into the output file.
    The generated events have been added to the same file before.*/
    events_info_CLASS write_info;
    write_info.Set(options);
    write_info.Save_to_root_file(out_file, "UPDATE");
}

void trento_options::merge_files(vector<TString> file, TString outfile){

    /*  Here the vector of files are checked first to have all exactly 
    the same "event_information". Then files are merged.  */
    Int_t chain_size = file.size();
    TChain ch("trento_events");
    map<TString, TString> map1, map2;
    events_info_CLASS *read_info_1, *read_info_2;

    if (file.size() == 0){
        cout << "File name vector is empty." << endl;
        return;
    }
    ch.Add(file[0]);
    if (file.size() == 1){
        gSystem->Exec(TString::Format("mv %s %s", file[0].Data(), outfile.Data()));
    } else {
        for (size_t i = 0; i < file.size()-1; i++){
            unique_ptr<TFile> f1(TFile::Open(file[i],"READ"));
            unique_ptr<TFile> f2(TFile::Open(file[i+1],"READ"));
            f1->GetObject("events_information", read_info_1);
            f2->GetObject("events_information", read_info_2);
            map1 = read_info_1->Get();
            map2 = read_info_2->Get();
            for (pair<TString, TString> element : map1){
                if (map1[element.first] != map2[element.first]) {
                    cout << "There is a conflict in " << element.first << "." << endl;
                    return;
                };
            }
            ch.Add(file[i+1]);
        }
        ch.Merge(outfile);
        /* Adding the events information to the file. */
        read_info_1->Save_to_root_file(outfile, "UPDATE");
        /* Removing the subfiles */
        for (size_t i = 0; i < file.size(); i++){
            gSystem->Exec(TString::Format("rm -f %s", file[i].Data()));
        }
    }

};