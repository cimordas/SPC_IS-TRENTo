#include "functions.h"
#include "event_CLASS.h"
#include "events_info_CLASS.h"

#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TFormula.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TThread.h"
#include "TStopwatch.h"

#include <numeric>
#include <vector>

#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
// #include <omp.h>



using namespace std;

functions::functions() {
	nevents_indicator = -1;
	X_axis_max = -1;
	boots_subdivition = 10;
	total_binned_events = 0;
 };

void functions::Set_x_axis_bins(TTree* t){	

	if ( centrality_true_false == false ){ 
		X_bins.clear();
		/* We check if the x_max is set by hand, if not we use the maximum value of the X_axis. */
		Double_t d_x;
		if (X_axis_max == -1){
			X_axis_max = t->GetMaximum(X_axis_name);
			d_x = X_axis_max / number_of_bins;
			X_axis_max = -1;
		} 
		else {
			d_x = X_axis_max / number_of_bins;
		}
		for (Int_t i = 0; i < number_of_bins + 1; i++){
			X_bins.push_back( i * d_x );
		}
	} else {
		X_bins.clear();
		//X_bins = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
		X_bins = centrality_bins;
		number_of_bins = X_bins.size() - 1;		
	}
}

void functions::Set_number_of_events(Int_t nev){
	if (nev == -1) {
		nevents_indicator = -1;
	} else {
		nevents_indicator = nev;
	}
}
void functions::Set_nevents(TTree* t){
	if (nevents_indicator == -1){
		nevents = t->GetEntries(); 
	} else {
		nevents = nevents_indicator;
	}
};

Int_t functions::bin_index(Double_t x_var){
	/*  This function check if the event belongs to the bin labeled by 'ind' */
	Int_t ind = 0;
	for (Int_t i = 0; i < number_of_bins; i++){		
		if (X_bins[i] <= x_var && x_var < X_bins[i+1]+1){
			ind = i; 
		};
	}
	return ind;
};

// vector<Double_t> functions::Get_bin_population(){ 
// 	vector<Double_t> bin_popu{};
// 	if (error_true_false){
// 		for (Int_t i = 0; i < number_of_bins; i++){
// 			bin_popu.push_back(bin_event_ID[i].size());
// 		}
// 		bin_population_vec = bin_popu;
// 	} else {
// 		if (bin_population_vec.empty()){
// 			/*  This part happens only if we start immedietly with multiplicity distirbution. Without defining any Mean values in the no error mode.*/
// 			TTree* event_tree;
// 			TFile f(file_name);
// 			f.GetObject(tree_name, event_tree);
// 			event_tree->SetBranchAddress(branch_name,  &sEvent );
// 			/*--------------------------------*/
// 			Set_x_axis_bins(event_tree);
// 			Set_nevents(event_tree);
// 			Int_t index = 0;
// 			Double_t n_ch_par = 0;
// 			bin_population_vec = vector<Double_t>(number_of_bins, 0.0);
// 			for(Int_t i = 0; i < nevents; i++){
// 				event_tree->GetEntry(i);
// 				if (X_axis_name == "nch"){
// 					n_ch_par = sEvent->Get_nch(); 
// 				} else {
// 					n_ch_par = sEvent->Get_npart();
// 				}	
// 				index = bin_index(n_ch_par);
// 				bin_population_vec[index]++;
// 			}
// 			delete event_tree;	
// 		}
// 	}
// 	return bin_population_vec;
// }

// vector<Double_t> functions::Get_bin_population_error(){
// 	if (bin_population_vec.empty()) Get_bin_population();
// 	for (auto& nk : bin_population_vec ){
// 		bin_population_error_vec.push_back(sqrt(nk*(1-nk / nevents)));
// 	}
// 	return bin_population_error_vec;
// }

void functions::Muli_or_part_binning(TTree * t){

	cout << "Binning ..." << endl;

	bin_event_ID.clear();

	Set_x_axis_bins(t);
	Set_nevents(t);
	
	/*  Initializing bin_event_ID*/
	for (Int_t bin = 0; bin < number_of_bins; bin++){
		bin_event_ID[bin] = vector<Int_t>{};
	}
	Double_t n_ch_par;
	Int_t index;
	// ========= Loop over all events ==================
	
	// TThread::Initialize();
	// int nthreads = 6;
  	// ROOT::EnableImplicitMT(nthreads);

	for(Int_t event = 0; event < nevents; event++){
		t->GetEntry(event);
		if (X_axis_name == "nch"){
			n_ch_par = sEvent->Get_nch();
		} 
		else {
			n_ch_par = sEvent->Get_npart();
		}
		index = bin_index(n_ch_par);
		bin_event_ID[index].push_back(event);
		if ( event % 1000 == 0) {cout << "binned events: " << event << endl;};
	};
	cout << "The end of binning." << endl;
};

void functions::Centrality_binning(TTree *t){

	cout << "Calculate centrality ..." << endl;

	bin_event_ID.clear();

	Set_x_axis_bins(t);
	Set_nevents(t);

	Double_t n_ch_par = 0;

	for(Int_t event = 0; event < nevents; event++){
		t->GetEntry(event);
		if (X_axis_name == "nch"){
			n_ch_par = sEvent->Get_nch();
		} 
		else {
			n_ch_par = sEvent->Get_npart();
		}

		pair<Int_t, Double_t> ID_xVar;
		ID_xVar.first = event;
		ID_xVar.second = n_ch_par;
		sorted_event_ID.push_back(ID_xVar); 
		// index = bin_index(n_ch_par);
		// bin_event_ID[index].push_back(event);
		if ( event % 1000 == 0) {cout << "Sorted events: " << event << endl;};
    };
	// for(pair<Int_t, Double_t> element : sorted_event_ID){
	// 	if(element.first > 99500) cout << "event ID: " << element.first << "  mulriplicity:  " << element.second << endl;
	// }; 

		/* Before sorting the events, we check if the total number of events are divisible with 100.
		If not, we remove few events from the end of the list. With that, the events are divisible into centrality bins
		where their width are a multiplication of 1%. Since it is not sorted yet, the removed events are unbiased. */
	//cout << " ==================== nevents: " << nevents << endl;
	Int_t events_remnant = nevents % 100;
	if ( events_remnant != 0){
		sorted_event_ID.resize(sorted_event_ID.size()-events_remnant);
	}
	// for(pair<Int_t, Double_t> element : sorted_event_ID){
	// 	if(element.first > 99500) cout << "event ID: " << element.first << "  mulriplicity:  " << element.second << endl;
	// }; 
	//cout << " ==================== refinedvnevents: " << sorted_event_ID.size() << endl;
	std::sort(sorted_event_ID.begin(), sorted_event_ID.end(), [](const pair<Int_t, Double_t> &left, const pair<Int_t, Double_t> &right) {
		return left.second >= right.second;
	});
	// Int_t check = 0;
	// for(pair<Int_t, Double_t> element : sorted_event_ID){
	// 	check++;
	// 	if(check == 20000) cout << "event ID: " << element.first << "  mulriplicity:  " << element.second << endl;
	// };	



	// vector<Int_t> X_bins = {0, 10, 20, 40, 60, 80, 100};
	//vector<Double_t> centrality_weight{};
	Double_t centrality_weight;
	Int_t lower_bound = 0, upper_bound = 0;
	for (Int_t i = 0; i < X_bins.size() - 1; i++){
		centrality_weight = (X_bins[i+1] - X_bins[i] ) / 100.;
		//centrality_weight.push_back((centrality_list[i+1] - centrality_list[i] ) / 100.);
		lower_bound = upper_bound + 1;
		upper_bound += centrality_weight * sorted_event_ID.size();
		//cout << "range: " << lower_bound << "   " << upper_bound << "     " << centrality_weight <<endl;
		for (Int_t evnt = lower_bound-1; evnt < upper_bound; evnt++){
			//cout << "In bin " << i << ": " << sorted_event_ID[evnt].first << endl;
			bin_event_ID[i].push_back(  sorted_event_ID[evnt].first   );
		}
		
		sort(bin_event_ID[i].begin(), bin_event_ID[i].end()); /* ! SORTING THE FINAL LIST IS VERY IMPORTANT. It Makes final calculation much faster! */
	}
	//cout << endl;
	// for (Int_t i = 0; i < centrality_list.size() - 1; i++){
	// 	cout << centrality_bin_event_ID[i].back() << "   ";
	// }
	// cout << endl;
	cout << "The end of centrality calculation." << endl;	
};

void functions::Read_variable(){
	TString nam;
	for (pair<TString, variable_struct> element : var){
		nam = element.first;
		if (var[nam].leaf_name == "nch"){
			var[nam].value = sEvent->Get_nch();
		} else if (var[nam].leaf_name == "npart"){ 
			var[nam].value = sEvent->Get_npart();
		} else if (var[nam].leaf_name == "r"){
			var[nam].value = sEvent->Get_r(var[nam].harmonic);
		} else if (var[nam].leaf_name == "b"){
			var[nam].value = sEvent->Get_b();
		} else if (var[nam].leaf_name == "epsilonx"){
			var[nam].value = sEvent->Get_epsilonx(var[nam].harmonic);
		} else if (var[nam].leaf_name == "epsilony"){
			var[nam].value = sEvent->Get_epsilony(var[nam].harmonic);	
		} else if (var[nam].leaf_name == "epsilon"){
			var[nam].value = sqrt(pow(sEvent->Get_epsilonx(var[nam].harmonic),2)+pow(sEvent->Get_epsilony(var[nam].harmonic),2));
		} else if (var[nam].leaf_name == "phi"){
			var[nam].value =  (atan2(sEvent->Get_epsilony(var[nam].harmonic),sEvent->Get_epsilonx(var[nam].harmonic))) / var[nam].harmonic;	
		} else {	
			cout << "Parameter does not exist." << endl;
		}
	}
	/* Here, we check if the variable values fullfill the cuts. */
	
	cut_condition = true;
	for (pair<TString, Double_t> element : min_cut){
		nam = element.first;
		if(var[nam].value < element.second){
			cut_condition = cut_condition * false;
		}
	}
	for (pair<TString, Double_t> element : max_cut){
		nam = element.first;
		if(var[nam].value > element.second){
			cut_condition = cut_condition * false;
		}
	} 
};

//==================================================//
map<TString, variable_struct> functions::Read_variable(TTree * t){
	
	map<TString, variable_struct> var_local;
	event_CLASS* sEvent_local = new event_CLASS();
	t->SetBranchAddress(branch_name,  &sEvent_local );

	TString nam;
	for (pair<TString, variable_struct> element : var){
		nam = element.first;
		var_local[nam].leaf_name = var[nam].leaf_name;
		if (var[nam].leaf_name == "nch"){
			var_local[nam].value = sEvent_local->Get_nch();
		} else if (var[nam].leaf_name == "npart"){ 
			var_local[nam].value = sEvent_local->Get_npart();
		} else if (var[nam].leaf_name == "r"){
			var_local[nam].value = sEvent_local->Get_r(var[nam].harmonic);
		} else if (var[nam].leaf_name == "b"){
			var_local[nam].value = sEvent_local->Get_b();
		} else if (var[nam].leaf_name == "epsilonx"){
			var_local[nam].value = sEvent_local->Get_epsilonx(var[nam].harmonic);
		} else if (var[nam].leaf_name == "epsilony"){
			var_local[nam].value = sEvent_local->Get_epsilony(var[nam].harmonic);	
		} else if (var[nam].leaf_name == "epsilon"){
			var_local[nam].value = sqrt(pow(sEvent_local->Get_epsilonx(var[nam].harmonic),2)+pow(sEvent_local->Get_epsilony(var[nam].harmonic),2));
		} else if (var[nam].leaf_name == "phi"){
			var_local[nam].value =  (atan2(sEvent_local->Get_epsilony(var[nam].harmonic),sEvent_local->Get_epsilonx(var[nam].harmonic))) / var[nam].harmonic;	
		} else {	
			cout << "Parameter does not exist." << endl;
		}
	}
	return var_local;
	/* Here, we check if the variable values fullfill the cuts. */
	
	// cut_condition = true;
	// for (pair<TString, Double_t> element : min_cut){
	// 	nam = element.first;
	// 	if(var[nam].value < element.second){
	// 		cut_condition = cut_condition * false;
	// 	}
	// }
	// for (pair<TString, Double_t> element : max_cut){
	// 	nam = element.first;
	// 	if(var[nam].value > element.second){
	// 		cut_condition = cut_condition * false;
	// 	}
	// } 
};

bool functions::cut_condition_func(map<TString, variable_struct> var_local){
	bool cut_cond = true;
	TString nam;
	for (pair<TString, Double_t> element : min_cut){
		nam = element.first;
		if(var_local[nam].value < element.second){
			cut_cond = cut_cond * false;
		}
	}
	for (pair<TString, Double_t> element : max_cut){
		nam = element.first;
		if(var_local[nam].value > element.second){
			cut_cond = cut_cond * false;
		}
	} 
	return cut_cond;
};
//===========================================================//

void functions::calc_mean_values(){

	TTree* t;
	//gSystem->Load(class_name);
	TFile f(file_name);
    f.GetObject(tree_name,t);
	t->SetBranchAddress(branch_name,  &sEvent );

	Set_x_axis_bins(t);
	Set_nevents(t);

	Int_t index = 0;
	Double_t n_ch_par = 0;
	TString ss;
	/* Here we initialize the function that we are going to calculate its mean value. */
	/* and initializing 'ave_vec_map'*/
	map<TString, TFormula> tf_map;	
	for (pair<TString, TString> element : function){
		ss = element.first;
		tf_map[ss] = TFormula("tf", function[ss]);	
		ave_vec_map[ss] = vector<Double_t>(number_of_bins, 0.0);
	};

	bin_population_vec = vector<Double_t>(number_of_bins, 0.0);
	
	if (!bin_population_vec.empty()){bin_population_vec = vector<Double_t>(number_of_bins, 0.0);}

	//====================  Main For Loop over ALL events ==============================	
	for(Int_t i = 0; i < nevents; i++){
		
		t->GetEntry(i);
		
		Read_variable();
		if(!cut_condition) continue; //check the cuts

		if (!(sEvent->Get_npart()))  continue;
		
		//---------------------
		if (X_axis_name == "nch"){
			n_ch_par = sEvent->Get_nch(); 
		} else {
			n_ch_par = sEvent->Get_npart();
		}	
		//---------------------
		index = bin_index(n_ch_par);
				
		bin_population_vec[index]++;

		for(pair<TString, TString> element : function){
			ss = element.first;	
			for (pair<TString, variable_struct> element : var){
				tf_map[ss].SetParameter(element.first, var[element.first].value);
			}		
			ave_vec_map[ss][index] += tf_map[ss].Eval(0);		
		}
		if (i % 100000 == 0 ) {cout << "event: " << i << endl;};
	}
	
	TString varNames;
	for(pair<TString, TString> element : function){
		varNames = "[" + element.first + "]";
		for (Int_t bin = 0; bin < number_of_bins; bin++){
			ave_vec_map[element.first][bin] = ave_vec_map[element.first][bin] / bin_population_vec[bin];
		};		
	};

	delete t;
};

void functions::Run_wo_error(){
	
	obse_vec.clear();

	TString funcName;
	TFormula tf;
	Int_t number_of_means;
	TString name_of_mean;

	calc_mean_values();

 	for (pair<TString, TString> element : obs_func){
		
		funcName = element.first;
		tf = TFormula("tf", obs_func[funcName]);
		number_of_means = tf.GetNpar();
		obse_vec[funcName] = vector<double>(number_of_bins, 0.0);

		for (Int_t bin = 0; bin < number_of_bins; bin++){
			for (Int_t i = 0; i < number_of_means; i++){
				name_of_mean = tf.GetParName(i);
				tf.SetParameter(name_of_mean, (ave_vec_map[name_of_mean])[bin]);
			};
			obse_vec[funcName][bin] += tf.Eval(0);
		};
	};
};

void functions::calc_mean_values(vector<Int_t> event_ID, TTree *t){

	Int_t num_of_events = event_ID.size();
	TString ss;
	/* Here we initialize the function that we are going to calculate its mean value. */
	/* and initializing 'ave_vec_map'*/
	map<TString, TFormula> tf_map;	
	for (pair<TString, TString> element : function){
		ss = element.first;
		tf_map[ss] = TFormula("tf", function[ss]);	
		ave_map[ss] = 0.0;
	};
	//====================  Main For Loop over a subset of events ==============================	
	Int_t event;	
	for(Int_t i = 0; i < num_of_events; i++){
		
		check_nevents++;
		//cout << "Size at each sub run: " << num_of_events << endl;

		event = event_ID[i];
		t->GetEntry(event);
		Read_variable();
		if(!cut_condition) continue; //check the cuts

		if (!(sEvent->Get_npart()))  continue;
		for(pair<TString, TString> element : function){
			ss = element.first;	
			// Read_variable();
			for (pair<TString, variable_struct> element : var){
				tf_map[ss].SetParameter(element.first, var[element.first].value);
			}		
			ave_map[ss] += tf_map[ss].Eval(0);	
		}
		//tf_map[ss].Clear();
		//if (i % 10000 == 0) {cout << "event: " << i << endl; };
	}
	for(pair<TString, TString> element : function){
		if (num_of_events == 0){
			ave_map[element.first] = 0;
		} else {
			ave_map[element.first] = ave_map[element.first] / num_of_events;
		}	
	};
};

void functions::Sub_run(vector<Int_t> event_ID, TTree *t){
		
	TString funcName;
	TFormula tf;
	Int_t number_of_means;
	TString name_of_mean;
	
	calc_mean_values(event_ID, t);
	
 	for (pair<TString, TString> element : obs_func){
		funcName = element.first;
		tf = TFormula("tf", obs_func[funcName]);
		number_of_means = tf.GetNpar();
		obse[funcName] = 0.0;

		for (Int_t i = 0; i < number_of_means; i++){
			name_of_mean = tf.GetParName(i);
			tf.SetParameter(name_of_mean, ave_map[name_of_mean]);
		};
		obse[funcName] = tf.Eval(0);	
	};
}

void functions::Run_with_error(){

	TTree* event_tree;	
	TFile f(file_name);
	cout << "Read from file: " << file_name << endl;
    f.GetObject(tree_name,event_tree);
	event_tree->SetBranchAddress(branch_name,  &sEvent );

	obse_vec.clear();
	obse_vec_err.clear();
	
	if ( centrality_true_false == false ){
		Muli_or_part_binning(event_tree);
	} else {
		Centrality_binning(event_tree);
	}
	
	cout << "Start running over events:" << endl;
	// vector<Int_t> subdiv_event_ID;
	// Long64_t event_vec_size, sub_nevents;
	// Int_t subdiv;
	// Double_t center = 0., variance = 0.;

	map<TString, vector<Double_t>  > obse_subset;
	for(pair<TString, TString> element : obs_func){
		obse_subset[element.first] = vector<Double_t>{};
	};		

	
	for (Int_t bin = 0; bin < number_of_bins; bin++){

		vector<Int_t> subdiv_event_ID;
		Long64_t event_vec_size, sub_nevents;
		Int_t subdiv;
		Double_t center = 0., variance = 0.;

		event_vec_size = bin_event_ID[bin].size();
		
		//cout << "Bin: " << bin << "   size: " << event_vec_size << endl;
		//cout << "Four first bin events: " << bin_event_ID[bin][0] << "  ,  "  << bin_event_ID[bin][1] << "  ,  "  << bin_event_ID[bin][2] << "  ,  " << bin_event_ID[bin][3] << endl;
		
		/*  Maybe in some bins the number of events are smaller than the value of subdivision in Bootstrap.
		In this case, we change the value of subdivision such that at each set we have exactly one event.
		Indeed the error would be teriblle but at least we do get something.*/
		if (event_vec_size >= boots_subdivition){
			subdiv = boots_subdivition;
		} else if (event_vec_size != 0){
			subdiv = event_vec_size;
		} else {
			subdiv = 1;
		};
		sub_nevents = floor(event_vec_size / subdiv);
		/* In the following, we partition the 'bin_event_ID', and calculate observables for each subset. */
		/*====================================================*/
		
		//TStopwatch Subrun_time;
	    
		for (Int_t k = 0; k < subdiv; k++){

			subdiv_event_ID = vector<Int_t>(bin_event_ID[bin].begin() + k * sub_nevents, bin_event_ID[bin].begin() + (k+1) * sub_nevents);
			//Subrun_time.Start();
			//cout << "subsize: " << subdiv_event_ID.size() << "  first, second, third element:  " << subdiv_event_ID[0] << " , " << subdiv_event_ID[1] << " , " << subdiv_event_ID[2] << endl;
			Sub_run(subdiv_event_ID, event_tree);
			//Subrun_time.Stop();	
			for(pair<TString, TString> element : obs_func){
				obse_subset[element.first].push_back(obse[element.first]);
			};		
				
		}
		
		//Subrun_time.Print();
		/*====================================================*/
		for(pair<TString, TString> element : obs_func){
			center = accumulate(obse_subset[element.first].begin(), obse_subset[element.first].end(), (Double_t)0.0 ) / subdiv;
			for (auto& theta : obse_subset[element.first]){
				variance += pow( (theta - center), 2.);
			}
			variance = variance / (subdiv-1);
			obse_vec[element.first].push_back(center);
			obse_vec_err[element.first].push_back( sqrt(variance) );
			variance = 0;
			center = 0;
		};
		/*   Reset the variables for the next bin  */
		/*=========================================*/
		obse_subset.clear();
		subdiv_event_ID.clear();
		/*=========================================*/
		
		if ( centrality_true_false == false ){
			cout << "bin: " << bin << endl;
		} else {
			cout << "Centrality bin: " << bin << endl;
		}
	};	
	delete event_tree;

	cout << "Total number of calculated events: " << check_nevents << endl;
}

void functions::Run(){
	if (error_true_false){
		Run_with_error();
	} else if (!error_true_false) {
		Run_wo_error();
	} else {
		cout << "Bad boolean input." << endl;
	}
};

void functions::Fill_histograms(){
	
	TTree* event_tree;
	TFile f(file_name);
    f.GetObject(tree_name,event_tree);
	event_tree->SetBranchAddress(branch_name,  &sEvent );

	Set_nevents(event_tree);

	map<TString, TFormula> tf_map;	
	TString ss;
	for (pair<TH1F*, TString> his_vars : hist_map){
		ss = his_vars.second;
		tf_map[ss] = TFormula("tf", hist_function[ss]);	
	};
	
	TThread::Initialize();
	int nthreads = 6;
  	ROOT::EnableImplicitMT(nthreads);

	for(Int_t i = 0; i < nevents; i++){
		
		event_tree->GetEntry(i);
		Read_variable();
		if(!cut_condition) continue; //check the cuts
		for (pair<TH1F*, TString> his_vars : hist_map){
			ss = his_vars.second;	
			// Read_variable();
			for (pair<TString, variable_struct> element : var){
				tf_map[ss].SetParameter(element.first, var[element.first].value);
			}	
			his_vars.first->Fill(tf_map[ss].Eval(0));	
		}
		if (i % 100000 == 0 ) {cout << "event: " << i << endl;};
	}
	delete event_tree;
};

TGraph *functions::ratio(Double_t* x_ax, TGraph *tg_n, TGraph *tg_d){
	TGraph *tg_ratio = new TGraphErrors();
	Int_t n_x_points = tg_n->GetN();
	if ( n_x_points != tg_d->GetN() ){
		cout << "Number of points are not equal." << endl;
		return 0;
	}

	Double_t num_var_center, num_var_error, denom_var_center, denom_var_error;
	vector<Double_t> ratio_center, ratio_error, x_err;

	for (Int_t i = 0; i < n_x_points; i++){
		num_var_center = tg_n->GetPointY(i);
		num_var_error = tg_n->GetErrorY(i);
		denom_var_center = tg_d->GetPointY(i);
		denom_var_error = tg_d->GetErrorY(i);
		x_err.push_back( tg_n->GetErrorX(i) );
		ratio_center.push_back(num_var_center / denom_var_center);
		ratio_error.push_back( abs(num_var_center / denom_var_center) * sqrt(pow( num_var_error  /  num_var_center , 2.) + pow( denom_var_error / denom_var_center  , 2.))  );
	} 
	tg_ratio = new TGraphErrors(n_x_points, x_ax, &ratio_center[0], &x_err[0], &ratio_error[0]);
	return tg_ratio;
};

TGraph *functions::ratio(TGraph *tg_n, TGraph *tg_d){
	TGraph *tg_ratio = new TGraphErrors();
	Int_t n_x_points = tg_n->GetN();
	if ( n_x_points != tg_d->GetN() ){
		cout << "Number of points are not equal." << endl;
		return 0;
	}
	vector<Double_t> x_axis;
	for (Int_t i = 0; i < n_x_points; i++){
		if (tg_n->GetPointX(i) != tg_d->GetPointX(i)){
			cout << "The X value of two raios are not compatible => " << tg_n->GetPointX(i) << " Vs " << tg_d->GetPointX(i) << endl;
			return 0;
		}
		x_axis.push_back(tg_n->GetPointX(i));
	}
	Double_t num_var_center, num_var_error, denom_var_center, denom_var_error;
	vector<Double_t> ratio_center, ratio_error, x_err;

	for (Int_t i = 0; i < n_x_points; i++){
		num_var_center = tg_n->GetPointY(i);
		num_var_error = tg_n->GetErrorY(i);
		denom_var_center = tg_d->GetPointY(i);
		denom_var_error = tg_d->GetErrorY(i);
		x_err.push_back( tg_n->GetErrorX(i) );
		ratio_center.push_back(num_var_center / denom_var_center);
		ratio_error.push_back( abs(num_var_center / denom_var_center) * sqrt(pow( num_var_error  /  num_var_center , 2.) + pow( denom_var_error / denom_var_center  , 2.))  );
	} 
	tg_ratio = new TGraphErrors(n_x_points, &x_axis[0], &ratio_center[0], &x_err[0], &ratio_error[0]);
	return tg_ratio;
};


void functions::Set_calculator_config(TString s){
	string line;
	ifstream config_file(s);

	stringstream split_line;
	istream_iterator<string> begin, end;
	vector<string> vector_line;
	if (config_file.is_open()){
		while (getline(config_file, line)){
			if ((line.front() == '#' || line.length() == 0 ) || all_of(line.begin(), line.end(), [](char c){return isspace(c);})) continue;
			split_line = stringstream(line);
			begin = istream_iterator<string>(split_line);
			vector_line = vector<string>(begin, end);
			if (vector_line.size() == 2 || (vector_line.size() > 2 && vector_line[2].front() == '#')){
				bool check_list = false;
				for (pair<TString, TString> element: calculate_options){
					if (vector_line[0] == element.first){
						check_list = true;
					}
				}
				if (!check_list){
					cout << "ERROR: Calculator option " + vector_line[0] + " is not found." << endl;
					return;
				}
				calculate_options[vector_line[0]] = vector_line[1];
			} else {
				cout << "ERROR: Each line of config file should have two items: 'option name' and 'option value'." << endl;
				break;
			};
		}
	}
	
	Set_number_of_bins(stoi(calculate_options["number-of-bins"].Data()));
	Set_X_axis_variable(calculate_options["x-axis-variable"].Data());
	Stat_error_estimation(stoi(calculate_options["error-estimation"].Data()));
	Centrality_calculation(stoi(calculate_options["x-axis-centrality"].Data()));
	Set_bootstrap_division(stoi(calculate_options["error-subdivsion"].Data()));
	Set_X_axis_max(stod(calculate_options["x-axis-max"].Data()));
	Set_number_of_events(stoi(calculate_options["nevents-max"].Data()));
	Set_root_file(calculate_options["input-file"].Data());

	/* Converting input centrality list to vector<double> 
	  READING CENTRALITY LIST FROM CONFIG FILE HAS NOT BEEN IMPLEMENETED*/
	//Set_centrality_bins(stoi(calculate_options["centrality-bin-list"].Data()));

	//cout << "Wht is happenong: " << calculate_options["x-axis-max"].Data() << endl;
}

vector<vector<string>> functions::Matrix_parse_config(TString s){
	string line;
	ifstream config_file(s);

	stringstream split_line;
	istream_iterator<string> begin, end;
	vector<string> vector_line, vector_line_noHashtag;
	vector<vector<string>> text_matrix{};
	if (config_file.is_open()){
		while (getline(config_file, line)){
			if (line.front() == '#' || line.length() == 0 || all_of(line.begin(), line.end(), [](char c){return isspace(c);})) continue;
			// cout << "Lines are: " << line << endl;
			split_line = stringstream(line);
			begin = istream_iterator<string>(split_line);
			vector_line = vector<string>(begin, end);
			for (auto ind : vector_line){
				if (ind.front() == '#') break;
				vector_line_noHashtag.push_back(ind);
			}
			text_matrix.push_back(vector_line_noHashtag);
			vector_line_noHashtag = vector<string>{};
		}
	}
	return text_matrix;
};

void functions::Set_observables_config(TString s){

	Set_variable("eps_2", "epsilon", 2);
	Set_variable("eps_3", "epsilon", 3);
	Set_variable("eps_4", "epsilon", 4);
	Set_variable("eps_5", "epsilon", 5);
	Set_variable("eps_6", "epsilon", 6);
	Set_variable("eps_7", "epsilon", 7);
	Set_variable("eps_8", "epsilon", 8);

	Set_variable("phi_2", "phi", 2);
	Set_variable("phi_3", "phi", 3);
	Set_variable("phi_4", "phi", 4);
	Set_variable("phi_5", "phi", 5);
	Set_variable("phi_6", "phi", 6);
	Set_variable("phi_7", "phi", 7);
	Set_variable("phi_8", "phi", 8);	

	Set_variable("r_2", "r", 2);
	Set_variable("r_3", "r", 3);
	Set_variable("r_4", "r", 4);
	Set_variable("r_5", "r", 5);
	Set_variable("r_6", "r", 6);
	Set_variable("r_7", "r", 7);
	Set_variable("r_8", "r", 8);

	Set_variable("n_ch", "nch");
	Set_variable("n_part", "npart");
	Set_variable("b_imp", "b");

	vector<vector<string>> text_matrix = Matrix_parse_config(s);
		
	Int_t height = text_matrix.size();
	Int_t function_begin = 0, function_end = 0, Observables_begin = 0, Observables_end = 0, Histograms_begin = 0, Histograms_end = 0, Cuts_begin = 0, Cuts_end = 0;
	for (Int_t i = 0; i < height; i++){
		// cout << text_matrix[i][0] << endl;
		if (text_matrix[i][0] == "Functions:") function_begin = i;
		if (text_matrix[i][0] == "FE") function_end = i;

		if (text_matrix[i][0] == "Observables:") Observables_begin = i;
		if (text_matrix[i][0] == "OE") Observables_end = i;		

		if (text_matrix[i][0] == "Histograms:") Histograms_begin = i;
		if (text_matrix[i][0] == "HE") Histograms_end = i;		

		if (text_matrix[i][0] == "Cuts:") Cuts_begin = i;
		if (text_matrix[i][0] == "CE") Cuts_end = i;		
	}

	for (Int_t i = Histograms_begin +1; i < Histograms_end; i++){

		Int_t line_size = text_matrix[i].size(), hist_nbins;
		TString Hist_name = text_matrix[i][0], Hist_expression = "";
		Double_t hist_min, hist_max;

		cout << "Is it bool? " << text_matrix[i][1] << endl;
		hist_norml_bool[Hist_name] = stoi(text_matrix[i][1]);
		histogram_name_list.push_back(Hist_name);
		hist_nbins = stoi(text_matrix[i][2]);
		hist_min = stod(text_matrix[i][3]);
		hist_max = stod(text_matrix[i][4]);
		
		
		
		TH1F *h_obs = new TH1F(Hist_name, Hist_name, hist_nbins, hist_min, hist_max);

		for (Int_t k = 5; k < line_size; k++){
			Hist_expression += text_matrix[i][k];
		}
		hist_function[Hist_name] = Hist_expression;
		Set_histogram(h_obs, Hist_name);
		cout << "Hist-name: " << Hist_name << "   Hist-mode: " << text_matrix[i][1] << "      Hist-Express: " << Hist_expression << endl;

	}

	for (Int_t i = Cuts_begin +1; i < Cuts_end; i++){
		for (auto ind : text_matrix[i]){
			cout << ind << " ";
		}
		cout << endl; 
	}

	for (Int_t i = Observables_begin +1; i < Observables_end; i++){
		
		Int_t line_size = text_matrix[i].size();
		TString Observable_name = text_matrix[i][0], Observable_expression = "";
		for (Int_t k = 1; k < line_size; k++){
			Observable_expression += text_matrix[i][k];
		}
		observable_name_list.push_back(Observable_name);
		Set_observable(Observable_name, Observable_expression);
		//cout << "Obs-name: " << Observable_name << "      Obs-Express: " << Observable_expression << endl;
	}	
	
	for (Int_t i = function_begin +1; i < function_end; i++){
		
		Int_t line_size = text_matrix[i].size();
		TString Func_name = text_matrix[i][0], Func_expression = "";
		for (Int_t k = 1; k < line_size; k++){
			Func_expression += text_matrix[i][k];
		}
		Set_function(Func_name, Func_expression);
		//cout << "Func-name: " << Func_name << "      Func-Express: " << Func_expression << endl;
	}	
}

void functions::Save_results_to_root(TString s){


	
	TFile *f = new TFile(s, "RECREATE");

	/*  ================  OBSERVABLES ================ */

	Int_t number_of_observables = observable_name_list.size();

	auto x_vector = Get_x_axis_bins_center();
	vector<Double_t> x_vector_err(0.0, number_of_bins);
	
	/*-----------------*/
	for (Int_t i = 0; i < number_of_observables; i++){
		auto obs_central = Get_observable(observable_name_list[i]);
		auto obs__error = Get_observable_error(observable_name_list[i]);
		auto obs_tgr  = new TGraphErrors(number_of_bins, &x_vector[0], &obs_central[0], &x_vector_err[0], &obs__error[0]);

		obs_tgr->SetTitle("; #LT" + X_axis_name + "#GT; " + observable_name_list[i] );
		obs_tgr->SetName("tgr_" + observable_name_list[i]);
		obs_tgr->GetXaxis()->SetTitleSize(0.045);
		obs_tgr->GetYaxis()->SetTitleSize(0.045);
		// gPad->SetLeftMargin(0.15);
		obs_tgr->SetMarkerStyle(8);	

		f->WriteObject(obs_tgr, observable_name_list[i]); 
	}

	/*  ================  HISTOGRAMS ================ */

	for (pair<TH1F*, TString> element : hist_map){
		
		TH1F *h_save = (TH1F *)element.first->Clone();
		if (hist_norml_bool[element.second] == 1){
			h_save->Scale(1. / h_save->Integral(),"width");
		}
		
		auto n_of_bins = h_save->GetSize() - 2; // GetSize() returns number of bins + Underflow + Overflow
		auto hist_x = Get_hist_bin_center(h_save);
		vector<Double_t> hist_x_err(hist_x.size(), 0.);
		auto hist_y = Get_hist_content(h_save);
		auto hist_y_err = Get_hist_error(h_save);

		auto hist_tgr  = new TGraphErrors(n_of_bins, &hist_x[0], &hist_y[0], &hist_x_err[0], &hist_y_err[0]);
		hist_tgr->SetTitle("; #LT" + element.second + "#GT; events"  );
		hist_tgr->SetName("hist_tgr_" + element.second);
		//gr_Ru_v22->GetXaxis()->SetRangeUser(0., 550);
		hist_tgr->GetXaxis()->SetTitleSize(0.045);
		hist_tgr->GetYaxis()->SetTitleSize(0.045);
		// gPad->SetLeftMargin(0.15);
		hist_tgr->SetMarkerStyle(8);	
		f->WriteObject(hist_tgr, element.second);

		// TCanvas * c_hist = new TCanvas(element.second, element.second, 800, 600);
		// gPad->SetLeftMargin(0.15);
		// hist_tgr->SetMarkerStyle(8);
		// hist_tgr->Draw("AP");
		// // //c_obs->Update();

		// // f->cd();
		// c_hist->SaveAs("/home/farid/Trento_analyszer_ROOT/GitHub/" + element.second + "_hist.png");

	}
	f->Close();
}

void functions::Save_ratios_to_root(TString s_n, TString s_d, TString s_r, TString s_conf){
	
	unique_ptr<TFile> fn(TFile::Open(s_n,"READ"));
	unique_ptr<TFile> fd(TFile::Open(s_d,"READ"));
	unique_ptr<TFile> fr(TFile::Open(s_r,"RECREATE"));


	vector<vector<string>> text_matrix = Matrix_parse_config(s_conf);

	Int_t height = text_matrix.size();
	Int_t ratio_begin = 0, ratio_end = 0;
	for (Int_t i = 0; i < height; i++){
		if (text_matrix[i][0] == "Ratios:") ratio_begin = i;
		if (text_matrix[i][0] == "RE") ratio_end = i;
	}

	for (Int_t i = ratio_begin +1; i < ratio_end; i++){
		for (auto & element : text_matrix[i]){
			//cout << "element: " << element << endl;
			ratio_name_list.push_back(element);
		}
	}
	// for (auto & element : ratio_name_list){
	// 	cout << "We got everything: " << element << endl;
	// }
	
	for (auto & element : ratio_name_list){

		TGraphErrors *	tgr_numerator = (TGraphErrors*)fn->Get(element);
		TGraphErrors *	tgr_denominator = (TGraphErrors*)fd->Get(element);

		auto * obs_tgr  =  ratio(tgr_numerator, tgr_denominator);
		
		cout << "xname? " << X_axis_name << endl;
		obs_tgr->SetTitle("; #LT" + X_axis_name + "#GT; " + element + "Ratio" );
		obs_tgr->SetName("tgr_" + element + "Ratio");
		//gr_Ru_v22->GetXaxis()->SetRangeUser(0., 550);
		obs_tgr->GetXaxis()->SetTitleSize(0.045);
		obs_tgr->GetYaxis()->SetTitleSize(0.045);
		
		//gPad->SetLeftMargin(0.15);
		// cout << "But...: "  << endl;
		obs_tgr->SetMarkerStyle(8);	
				
		fr->WriteObject(obs_tgr, element + "Ratio");

	}

	// auto obs_tgr  = new TGraphErrors(number_of_bins, &x_vector[0], &obs_central[0], &x_vector_err[0], &obs__error[0]);
	// obs_tgr->SetTitle("; #LT" + X_axis_name + "#GT; " + observable_name_list[i] );
	// obs_tgr->SetName("tgr_" + observable_name_list[i]);
	// //gr_Ru_v22->GetXaxis()->SetRangeUser(0., 550);
	// obs_tgr->GetXaxis()->SetTitleSize(0.045);
	// obs_tgr->GetYaxis()->SetTitleSize(0.045);
	// gPad->SetLeftMargin(0.15);
	// obs_tgr->SetMarkerStyle(8);	
	// f->WriteObject(obs_tgr, observable_name_list[i]);

	// for (TObject* keyAsObj : *fn->GetListOfKeys()){
	// 	auto key = dynamic_cast<TKey*>(keyAsObj);
	// 	if (!strcmp(key->GetClassName(), "TGraphErrors")) cout << "Yes it is" << endl; // HOW to Compare GetClassName
	// 	cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl;
	// }

};

void functions::Save_info_to_root(TString s_i, TString s_o){
	unique_ptr<TFile> f1(TFile::Open(s_i,"READ"));
	events_info_CLASS *read_info;
	f1->GetObject("events_information", read_info);
	read_info->Save_to_root_file(s_o, "UPDATE");
}



