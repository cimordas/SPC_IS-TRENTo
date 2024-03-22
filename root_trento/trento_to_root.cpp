//c++
#include <boost/program_options.hpp>
#include <stdlib.h>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <boost/filesystem.hpp>
// #include <omp.h>
// #include "ROOT/TThreadedObject.hxx"
// #include "ROOT/TSeq.hxx"
// #include "ROOT/TThreadExecutor.hxx"
// #include "ROOT/TThreadedObject.hxx"
// #include "TThread.h"
// #include "TThreadPool.h"
// #include "TThreadFactory.h"
// #include "TThreadImp.h"
// #include "TThreadSlots.h"
#include <omp.h>
//ROOT
#include "TROOT.h"  
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TString.h" 
#include "TRandom3.h"
#include "TH1F.h"
#include "TThread.h"
#include "TStopwatch.h"

//#include "TChain.h"
//Run Trento
#include "trento_options.h"
#include "events_info_CLASS.h"
// #include "event_CLASS.h"


using namespace std;
using namespace boost::filesystem;
using namespace boost::program_options;

int main(int argc, const char* argv[]){

    events_info_CLASS * info = new events_info_CLASS;
    TString out_path_tstring, file_name_tstring;
    Int_t n_cores;
    try{
        options_description desc{"TrENTo options"};

        desc.add_options()
            ("config", value<path>()->default_value(""), "Configure file")
            ("target", value<string>(), "Target")
            ("projectile", value<string>(), "Projectile")
            ("nevents", value<int>(),"Number of events")
            ("reduced-thickness,p", value<float>(),"Reduced thickness")
            ("fluctuation,k", value<float>(), "Fluctuation")
            ("nucleon-width,w", value<float>(), "Nucleon width")
            ("constit-width,v", value<float>(), "Constit width")
            ("constit-number,m", value<int>(), "Constit number")    
            ("nucleon-min-dist,d", value<float>(), "Nucleon minimum distance")
            ("cross-section,x", value<float>(), "Cross section")
            ("normalization,n", value<float>(), "Normalization")
            ("b-min", value<float>(), "b min")
            ("b-max", value<float>(), "b max")
            ("grid-max", value<float>(), "grid max")
            ("grid-step", value<float>(), "grid step")
            ("output-file", value<path>()->default_value("gen_events.root"),"Output file")
            ("long-output,l", value<Int_t>()->default_value(0),"Output harmonics up to 8")
            ("number-of-cores,c", value<Int_t>()->default_value(1),"Number of cores");

        variables_map vm;
    
        store(parse_command_line(argc, argv, desc), vm);   
        
        /* Set the options  */
        path conf_path = vm["config"].as<path>();
        cout << "Config path " << conf_path << "  exist? " << exists(conf_path) << endl;
        info->Set_from_config_file(conf_path.string()); 

        if (vm.count("target"))                 info->Set("target", vm["target"].as<string>());
        if (vm.count("projectile"))             info->Set("projectile", vm["projectile"].as<string>()); 
        if (vm.count("nevents"))                info->Set("nevents", vm["nevents"].as<int>());
        if (vm.count("reduced-thickness"))      info->Set("reduced-thickness", vm["reduced-thickness"].as<float>() ); 
        if (vm.count("fluctuation"))            info->Set("fluctuation", vm["fluctuation"].as<float>());
        if (vm.count("nucleon-width"))          info->Set("nucleon-width", vm["nucleon-width"].as<float>());
        if (vm.count("constit-number"))         info->Set("constit-number", vm["constit-number"].as<int>());
        if (vm.count("nucleon-min-dist"))       info->Set("nucleon-min-dist", vm["nucleon-min-dist"].as<float>());
        if (vm.count("cross-section"))          info->Set("cross-section", vm["cross-section"].as<float>());
        if (vm.count("normalization"))          info->Set("normalization", vm["normalization"].as<float>());
        if (vm.count("b-min"))                  info->Set("b-min", vm["b-min"].as<float>());
        if (vm.count("b-max"))                  info->Set("b-max", vm["b-max"].as<float>());
        if (vm.count("grid-max"))               info->Set("grid-max", vm["grid-max"].as<float>());
        if (vm.count("grid-step"))              info->Set("grid-step", vm["grid-step"].as<float>());
        if (vm.count("long-output"))            info->Set("long-output", vm["long-output"].as<int>());
        
        if (vm.count("constit-width")){
            if (vm["constit-number"].as<int>() == 1){
                info->Set("constit-width", vm["nucleon-width"].as<float>());
                cout << "For 'constit-number' = 1, 'constit-width' is equal to 'nucleon-width'. " << endl;
            } else {
                if (vm["constit-width"].as<float>() > vm["nucleon-width"].as<float>()) {
                    cout << "'constit-width' should be smaller than or equal to 'nucleon-width'." << endl;
                    return 0;
                }
                info->Set("constit-width", vm["constit-width"].as<float>());
            };
        }

        n_cores = vm["number-of-cores"].as<Int_t>();

        path out_path = vm["output-file"].as<path>(); // The input is a path.
        out_path_tstring = out_path.parent_path().string();
        if (out_path_tstring != "") out_path_tstring += "/";
        file_name_tstring = out_path.stem().string();   
    }
    catch (const error &ex){
        cerr << ex.what() << endl;
        return 0;
    }

    trento_options opt[n_cores];

    #ifdef _OPENMP
        cout << "OMP defined"  << endl;
    #else
        cout << "OMP not defined" << endl;
    #endif

    TThread::Initialize(); /* THIS LINE IS CRUCIAL */
    
    #pragma omp parallel num_threads(n_cores)
	{
        cout << "The thread is " << omp_get_thread_num()  << endl;
        opt[omp_get_thread_num()].Set_output_file( Form(out_path_tstring + file_name_tstring + "_%d.root", omp_get_thread_num()));
        opt[omp_get_thread_num()].Get_options(info);
        opt[omp_get_thread_num()].Run_trento(); 
    }      

    vector<TString> subfiles{};
    for (Int_t i = 0; i < n_cores; i++){
        subfiles.push_back(Form(out_path_tstring + file_name_tstring + "_%d.root", i));
    }

    opt->merge_files(subfiles, out_path_tstring + file_name_tstring + ".root");
    cout << "End of merging." << endl;
}
