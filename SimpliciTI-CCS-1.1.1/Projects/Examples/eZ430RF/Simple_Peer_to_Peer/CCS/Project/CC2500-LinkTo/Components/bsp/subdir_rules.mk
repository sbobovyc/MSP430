################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Components/bsp/bsp.obj: C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Components/bsp/bsp.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430/bin/cl430" --cmd_file="C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Projects/Examples/eZ430RF/Simple_Peer_to_Peer/CCS/Project/../Configuration/smpl_nwk_config.dat" --cmd_file="C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Projects/Examples/eZ430RF/Simple_Peer_to_Peer/CCS/Project/../Configuration/End_Device/smpl_config.dat"  -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430/include" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Projects/Examples/eZ430RF/Simple_Peer_to_Peer/CCS/Project/../../../../../../Components/bsp" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Projects/Examples/eZ430RF/Simple_Peer_to_Peer/CCS/Project/../../../../../../Components/bsp/boards/eZ430RF" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Projects/Examples/eZ430RF/Simple_Peer_to_Peer/CCS/Project/../../../../../../Components/bsp/drivers" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Projects/Examples/eZ430RF/Simple_Peer_to_Peer/CCS/Project/../../../../../../Components/mrfi" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Projects/Examples/eZ430RF/Simple_Peer_to_Peer/CCS/Project/../../../../../../Components/simpliciti/nwk" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/SimpliciTI-CCS-1.1.1/Projects/Examples/eZ430RF/Simple_Peer_to_Peer/CCS/Project/../../../../../../Components/simpliciti/nwk_applications" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Components/bsp/bsp.pp" --obj_directory="Components/bsp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


