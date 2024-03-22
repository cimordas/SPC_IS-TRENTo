//calcuator
#include "functions.h"
#include "event_CLASS.h"
#include "events_info_CLASS.h"


//c++
#include <numeric>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace boost::program_options;

int main(int argc, const char* argv[]){

    path num_root_file_path, den_root_file_path, out_root_file_path, config_file_path;

    try{
        options_description desc{"ratio"};

        desc.add_options()
            ("numerator-file,n", value<path>(), "Numerator file")
            ("Denominator-file,d", value<path>(), "Denominator file")
            ("output-file,f", value<path>()->default_value("ratio.root"), "Output file")
            ("observables-config,c", value<path>(),"Observables config");

        variables_map vm;
    
        store(parse_command_line(argc, argv, desc), vm);   

		if (!vm.count("numerator-file")){
			cout << "The numerator root file must be set.\n";
		} else {
			num_root_file_path = vm["numerator-file"].as<path>();
			if (!exists(num_root_file_path)) {
				cout << "Numerator root file is not found.\n";
				return 0;
			};
		}

        if (!vm.count("Denominator-file")){
			cout << "The denominator root file must be set.\n";
		} else {
			den_root_file_path = vm["Denominator-file"].as<path>();
			if (!exists(den_root_file_path)) {
				cout << "Denominator root file is not found.\n";
				return 0;
			};
		}

        if (!vm.count("observables-config")){
			cout << "The observables config file must be set.\n";
		} else {
			config_file_path = vm["observables-config"].as<path>();
			if (!exists(config_file_path)) {
				cout << "Observables config file is not found.\n";
				return 0;
			};
		}

        out_root_file_path = vm["output-file"].as<path>();
         
    }
    catch (const error &ex){
        cerr << ex.what() << endl;
        return 0;
    }

    TString n_file_tstring, d_file_tstring, o_file_tstring, c_file_tstring;
    n_file_tstring = num_root_file_path.string();
    d_file_tstring = den_root_file_path.string();
    o_file_tstring = out_root_file_path.string();
    c_file_tstring = config_file_path.string();

    functions* func = new functions();

    func->Save_ratios_to_root(n_file_tstring, d_file_tstring, o_file_tstring, c_file_tstring);
    func->Save_info_to_root(n_file_tstring, o_file_tstring);
}