//ROOT
#include "TROOT.h"  
#include "TObject.h"

//ROOT-TrENTo
#include "trento_options.h"
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

    path input_file_path, output_file_path;
    int min, max;
    TString input_file_tstr, output_file_tstr;

    try{
        options_description desc{"ratio"};

        desc.add_options()
            ("input-file-form,i", value<path>(), "Input file form")
            ("output-file,o", value<path>(), "Output file")
            ("file-index-min,l", value<int>(), "File index min")
            ("file-index-max,g", value<int>(), "File index max");;

        variables_map vm;
    
        store(parse_command_line(argc, argv, desc), vm);   

        input_file_path = vm["input-file-form"].as<path>();
        input_file_tstr = input_file_path.string();
        output_file_path = vm["output-file"].as<path>();
        output_file_tstr = output_file_path.string();
        min = vm["file-index-min"].as<int>();
        max = vm["file-index-max"].as<int>();
         
    }
    catch (const error &ex){
        cerr << ex.what() << endl;
        return 0;
    }

    vector<TString> subfiles{};
    for (Int_t i = min; i < max + 1; i++){
        subfiles.push_back(Form(input_file_tstr, i));
    }
    trento_options * opt = new trento_options;
 
    opt->merge_files(subfiles, output_file_tstr);
    cout << "End of external merging." << endl;
}