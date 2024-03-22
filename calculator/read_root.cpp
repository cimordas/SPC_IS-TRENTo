//calcuator
#include "functions.h"

// ROOT
#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TFormula.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TStopwatch.h"

//c++
#include <numeric>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <omp.h>

using namespace std;
using namespace boost::filesystem;
using namespace boost::program_options;

int main(int argc, const char* argv[]){

	TStopwatch Full_time, Run_time;
	Full_time.Start();
	////=========
	
	functions* func = new functions();
	path output_path = "", root_file_path = "";
	try{
		options_description desc{"TrENTo calculator"};

		desc.add_options()
            ("calc-config,c", value<path>()->default_value(""), "Calculator config file")
            ("number-of-bins,b", value<int>(), "Number of bins")
            ("x-axis-variable,x", value<TString>(), "X axis variable")
            ("error-estimation,e", value<bool>(),"Error estimation")
			("x-axis-centrality,t", value<bool>(),"X axis centrality")
			("centrality-bin-list,r", value<vector<double>>(),"Centrality bin list") /* NOT WORKING */
            ("error-subdivsion,s", value<int>(),"Error subdivsion")
            ("x-axis-max,m", value<float>(), "X axis max")
            ("nevents-max,n", value<int>(), "Nevents max")
            ("input-file,i", value<path>(), "Input file")
            ("output-file,f", value<path>()->default_value("result.root"), "Output file")    
            ("observables,o", value<path>(), "Observables config file");

        variables_map vm;
    
        store(parse_command_line(argc, argv, desc), vm);   
        
        /* Set the options  */
		func->Set_tree_name("trento_events");
		func->Set_branch_name("events");

        path calc_conf_path = vm["calc-config"].as<path>();
		if (!exists(calc_conf_path) && calc_conf_path != "") {
			cout << "This is calc-config path: " << calc_conf_path << endl;
			cout << "Calculator config file is not found.\n";
			return 0;
		};
		
    	func->Set_calculator_config(calc_conf_path.string());   

		if (vm.count("number-of-bins"))		func->Set_number_of_bins(vm["number-of-bins"].as<int>());
		if (vm.count("x-axis-variable"))	func->Set_X_axis_variable(vm["x-axis-variable"].as<TString>());
		if (vm.count("error-estimation"))	func->Stat_error_estimation(vm["error-estimation"].as<bool>());
		if (vm.count("x-axis-centrality"))	func->Centrality_calculation(vm["x-axis-centrality"].as<bool>());
		if (vm.count("centrality-bin-list"))func->Set_centrality_bins(vm["centrality-bin-list"].as<vector<double>>());
		if (vm.count("error-subdivsion"))	func->Set_bootstrap_division(vm["error-subdivsion"].as<int>());
		if (vm.count("x-axis-max"))		    func->Set_X_axis_max(vm["x-axis-max"].as<float>());
		if (vm.count("nevents-max"))		func->Set_number_of_events(vm["nevents-max"].as<int>());
		
		if (!vm.count("input-file")){
			cout << "The input root file must be set.\n";
		} else {
			root_file_path = vm["input-file"].as<path>();
			if (!exists(root_file_path)) {
				cout << "Input root file is not found.\n";
				return 0;
			};
			func->Set_root_file(root_file_path.string()); 
		}
		// cout << "==>  " << vm["observablest"].as<path>() << endl;
		if (!vm.count("observables")){
			cout << "The observable config file must be set.\n";
			return 0;
		} else {
			path obs_conf_path = vm["observables"].as<path>();
			// cout << "obs path: " << obs_conf_path << endl;
			if (!exists(obs_conf_path)) {
				cout << "Observables config file is not found.\n";
				return 0;
			};
			func->Set_observables_config(obs_conf_path.string()); 
		}


		output_path = vm["output-file"].as<path>();
    	// func->Save_results_to_root(output_path.string());  
		// func->Save_info_to_root(root_file_path.string(), output_path.string());
	} 
	catch (const error &ex){
        cerr << ex.what() << endl;
        return 0;
    }

	func->Fill_histograms();
	
	cout << "Hists filled. " << endl;
	
	func->Run();

	cout << "Run done. " << endl;

	func->Save_results_to_root(output_path.string());  
	func->Save_info_to_root(root_file_path.string(), output_path.string());

	Full_time.Stop();
	Full_time.Print();
}	
