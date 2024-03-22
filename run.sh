#!/bin/sh

## SPC IS predictions. ##
result_Path=Generated_data_example/
config_Path=config_files/

# IS events.
##./root_trento/build/root_trento --config $config_Path'trento_config.conf'  --nevents 200000 --output-file $result_Path'PbPb_Run2_1Mevents.root' --number-of-cores 5 --target Pb --projectile Pb --long-output 1

##./calculator/build/calculate  -i $result_Path'PbPb_Run2_1Mevents.root' -f $result_Path'PbPb_Run2_1Mevents_2SPC.root' -o $config_Path'expr_2SPC.conf' -c $config_Path'calc-config.conf'
##./calculator/build/calculate  -i $result_Path'PbPb_Run2_1Mevents.root' -f $result_Path'PbPb_Run2_1Mevents_3SPC.root' -o $config_Path'expr_3SPC.conf' -c $config_Path'calc-config.conf'
##./calculator/build/calculate  -i $result_Path'PbPb_Run2_1Mevents.root' -f $result_Path'PbPb_Run2_1Mevents_4SPC.root' -o $config_Path'expr_4SPC.conf' -c $config_Path'calc-config.conf'


## SC IS predictions. ##
./calculator/build/calculate  -i $result_Path'PbPb_Run2_1Mevents.root' -f $result_Path'PbPb_Run2_1Mevents_SC.root' -o $config_Path'expr_SC.conf' -c $config_Path'calc-config.conf'
