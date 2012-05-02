################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
I2C_Driver.obj: ../I2C_Driver.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430/bin/cl430" -vmsp --abi=coffabi -O0 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/I2C_test/h" --include_path="C:/ti/ccsv5/tools/compiler/msp430/include" --define=__MSP430F2274__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="I2C_Driver.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTCC_Driver.obj: ../RTCC_Driver.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430/bin/cl430" -vmsp --abi=coffabi -O0 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/I2C_test/h" --include_path="C:/ti/ccsv5/tools/compiler/msp430/include" --define=__MSP430F2274__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="RTCC_Driver.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430/bin/cl430" -vmsp --abi=coffabi -O0 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/sbobovyc/workspace_v5_1/MSP430/I2C_test/h" --include_path="C:/ti/ccsv5/tools/compiler/msp430/include" --define=__MSP430F2274__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


