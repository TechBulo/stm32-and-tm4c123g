################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --preinclude="C:/TivaWare_C_Series-2.1.0.12573/driverlib/ccs/Debug/driverlib.lib" --preinclude="C:/StellarisWare/driverlib/ccs-cm4f/Debug" --preinclude="D:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --preinclude="C:/StellarisWare" --preinclude="C:/StellarisWare/inc" --include_path="C:/TivaWare_C_Series-2.1.0.12573" --include_path="C:/TivaWare_C_Series-2.1.0.12573/driverlib" --include_path="C:/TivaWare_C_Series-2.1.0.12573/inc" --include_path="D:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --gcc --define=ccs="ccs" --define=DEBUG --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"D:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --preinclude="C:/TivaWare_C_Series-2.1.0.12573/driverlib/ccs/Debug/driverlib.lib" --preinclude="C:/StellarisWare/driverlib/ccs-cm4f/Debug" --preinclude="D:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --preinclude="C:/StellarisWare" --preinclude="C:/StellarisWare/inc" --include_path="C:/TivaWare_C_Series-2.1.0.12573" --include_path="C:/TivaWare_C_Series-2.1.0.12573/driverlib" --include_path="C:/TivaWare_C_Series-2.1.0.12573/inc" --include_path="D:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --gcc --define=ccs="ccs" --define=DEBUG --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


