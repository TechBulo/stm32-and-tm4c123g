################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
LCD12864.obj: ../LCD12864.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/driverlib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/inc" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="LCD12864.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

key.obj: ../key.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/driverlib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/inc" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="key.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/driverlib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/inc" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mdac.obj: ../mdac.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/driverlib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/inc" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="mdac.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/driverlib" --include_path="D:/software/CCS/CCS5.5.0.00077_win32/ccsv5/tivalib/inc" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


