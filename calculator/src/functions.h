#ifndef functions_h
#define functions_h

#include "TObject.h"
#include "TROOT.h" 
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TFormula.h"
#include "TF1.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
//#include "TCanvas.h"


#include "event_CLASS.h"
#include <iostream>
#include <vector>
// #include <omp.h>

using namespace std;

struct variable_struct{
	TString name, leaf_name;
	Double_t value;
	Int_t harmonic;
};

class functions: public TObject {
private:
	/*  Private variables: */
	/*  ================== */	
	event_CLASS* sEvent = new event_CLASS();
	
	Long64_t nevents, nevents_indicator;
	Double_t X_axis_max;
	Int_t number_of_bins, boots_subdivition, total_binned_events;
	vector<Int_t> harmonic{};
	vector<Double_t> X_bins{}, bin_population_vec{}, centrality_bins = {0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
	// vector<TH1F *> hist_list{}; /* Note that it is vector of pointers, so the input should be defined as pointer. */
	// vector<TString> hist_var{};
	vector<TString> observable_name_list{}, histogram_name_list{}, ratio_name_list{};

	TString file_name, X_axis_name, class_name, tree_name, branch_name;
	map<TString, TString> obs_func = {}, function = {}, hist_function = {}, calculate_options = {{"number-of-bins", "15"}, 
	{"x-axis-variable", "nch"}, {"error-estimation", "1"}, {"x-axis-centrality", "0"}, 
	//{"centrality-bin-list", "{0, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}}"}, 
	{"error-subdivsion","10"}, {"x-axis-max", "-1"}, {"nevents-max", "-1"}, {"input-file", ""}};
	map<TString, variable_struct> var;
	map<TString, vector<Double_t>> ave_vec_map, obse_vec, obse_vec_err;
	map<TString, Double_t> ave_map, obse, min_cut, max_cut;
	map<Int_t, vector<Int_t>> bin_event_ID, centrality_bin_event_ID; /* First element is the index of the bin, second element is a vector of event IDs blong to that bin. */
	vector<pair<Int_t, Double_t> > sorted_event_ID;/* The event ID when they are sorted based on x axis variable. */
	map<TH1F*, TString> hist_map;
	map<TString, Int_t> hist_norml_bool;

	Double_t check_nevents = 0;

	bool error_true_false = true, cut_condition = true, centrality_true_false = false;

	/*  Private functions: */
	/*  ================== */
	void Set_x_axis_bins(TTree*);
	void Muli_or_part_binning(TTree *);
	Int_t bin_index( Double_t);		
	void Read_variable();
	void calc_mean_values();
	void calc_mean_values(vector<Int_t>, TTree *);		
	void Sub_run(vector<Int_t>, TTree *);
	void Set_nevents(TTree*);
	void Run_wo_error();
	void Run_with_error();
	void Get_total_binned_events();

public:
	// functions(){
	// 	nevents_indicator = -1;
	// 	X_axis_max = -1;
	// 	boots_subdivition = 10;
	// 	total_binned_events = 0;
	// };
	functions();
	~functions() {
		delete sEvent;			
	};

	void Set_class_name(TString c_name){
		class_name = c_name;
	}
	void Set_number_of_bins(Int_t n_bins){
		number_of_bins = n_bins;
	}
	void Set_centrality_bins(vector<Double_t> cen_bins){
		centrality_bins = cen_bins;
	}	
	void Set_root_file(TString s){
		file_name = s;
	}
	void Set_tree_name(TString s){
		tree_name = s;
	}
	void Set_branch_name(TString s){
		branch_name = s;
	}				
	void Set_X_axis_variable(TString s){
		if (s == "nch" || s== "npart"){
			X_axis_name = s;	
		}
		else {
			cout << "The X axis can be 'nch' or 'npart'" << endl; 
		}
	}

	void Stat_error_estimation(bool TF){
		error_true_false = TF;
	}

	void Centrality_calculation(bool TF){
		centrality_true_false = TF;
	}

	void Set_number_of_events(Int_t);

	void Set_variable(TString nam, TString s){
		variable_struct variable;
		variable.name = nam;
		//variable.harmonic = -1;
		variable.leaf_name = s;
		// var[nam].push_back(variable);
		var[nam] = variable;
	}
	void Set_variable(TString nam, TString s, Int_t harm){
		variable_struct variable;
		variable.name = nam;
		variable.harmonic = harm;
		variable.leaf_name = s;
		var[nam] = variable;
	}
	void Set_observable(TString nam, TString s){
		//cout << "size: " << obs_func.size() << endl;
		obs_func[nam] = s;			
	}		
	void Set_function(TString nam, TString s){
		function[nam] = s;	
	}	
	void Set_hist_function(TString nam, TString s){
		hist_function[nam] = s;	
	}				
	void Set_X_axis_max(Double_t max){
		X_axis_max = max;
	}
			
	void Set_bootstrap_division(Int_t k){
		boots_subdivition = k;			
	}

	void Set_histogram(TH1F* h, TString s){
		hist_map.insert( pair<TH1F*, TString>(h,s) );
	}

	void Set_min_cuts(TString s, Double_t cut){
		min_cut.insert(pair<TString, Double_t>(s,cut));
	}

	void Set_max_cuts(TString s, Double_t cut){
		max_cut.insert(pair<TString, Double_t>(s,cut));
	}

	void Fill_histograms();

	vector<Double_t> Get_x_axis_bins(){
		if (X_bins.empty()){
			cout << "x axis bins has not been set yet." << endl;
			return vector<Double_t>();
		}
		else {
			return X_bins;	
		}
	};

	Double_t Get_x_axis_max(){
		return X_axis_max;
	}
	vector<Double_t> Get_bin_population();
	vector<Double_t> Get_bin_population_error();

	vector<Double_t> Get_x_axis_bins_center(){
		vector<Double_t> vec_x{};
		if (X_bins.empty()){
			cout << "x axis bins has not been set yet." << endl;
			return vector<Double_t>();
		}
		else {
			for (Int_t i = 0; i < number_of_bins; i++){
				vec_x.push_back( (X_bins[i] + X_bins[i+1]) / 2. );
			}
			return vec_x;	
		}
	};

	vector<Double_t> Get_observable(TString s){
		return obse_vec[s];
	};	

	vector<Double_t> Get_observable_error(TString s){
		if (obse_vec_err[s].size() == 0){
			obse_vec_err[s] = vector<Double_t>(number_of_bins, 0.0);
		}
		return obse_vec_err[s];
	};	
	
	map<TString, variable_struct> Read_variable(TTree *);
	bool cut_condition_func(map<TString, variable_struct>);
	// void Reset_observable_values(){
	// 	obse_vec.clear();
	// 	obse_vec_err.clear();
	// };			
	/*  Functions to simplify histogram plots */
	/*========================================*/
	vector<Double_t> Get_hist_bin_center(TH1F *h){
		vector<Double_t> vec;
		for (Int_t i = 0; i < h->GetNbinsX(); i++){
			vec.push_back(h->GetBinCenter(i+1));
		}
		return vec;
	};
	vector<Double_t> Get_hist_content(TH1F *h){
		vector<Double_t> vec;
		for (Int_t i = 0; i < h->GetNbinsX(); i++){
			vec.push_back(h->GetBinContent(i+1));
		}
		return vec;
	};
	vector<Double_t> Get_hist_error(TH1F *h){
		vector<Double_t> vec;
		for (Int_t i = 0; i < h->GetNbinsX(); i++){
			vec.push_back(h->GetBinError(i+1));
		}
		return vec;
	};
	/*========================================*/
	void Run();

	vector<vector<string>> Matrix_parse_config(TString);

	TGraph *ratio(Double_t*, TGraph *, TGraph *);
	TGraph *ratio(TGraph *, TGraph *);

	void Set_calculator_config(TString);

	void Set_observables_config(TString);

	void Save_results_to_root(TString);

	void Save_info_to_root(TString, TString);

	void Save_ratios_to_root(TString, TString, TString, TString);

	void Centrality_binning(TTree *);

	ClassDef(functions, 1)	
};

#endif