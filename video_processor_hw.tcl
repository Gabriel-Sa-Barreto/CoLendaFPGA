# TCL File Generated by Component Editor 18.1
# Fri Apr 24 16:34:17 BRT 2020
# DO NOT MODIFY


# 
# video_processor "video_processor" v1.0
# Gabriel Sa Barreto Alves 2020.04.24.16:34:17
# Processador de video responsavel por gerir as informa?oes que serao visualizadas em um monitor para impressao de sprite em jogos.
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module video_processor
# 
set_module_property DESCRIPTION "Processador de video responsavel por gerir as informa?oes que serao visualizadas em um monitor para impressao de sprite em jogos."
set_module_property NAME video_processor
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR "Gabriel Sa Barreto Alves"
set_module_property DISPLAY_NAME video_processor
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL video_processor
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file video_processor.v VERILOG PATH modulos/Top_level/video_processor.v TOP_LEVEL_FILE
add_fileset_file controlUnit.v VERILOG PATH modulos/ControlUnit/controlUnit.v
add_fileset_file decoderInstruction.v VERILOG PATH modulos/decoderInstruction/decoderInstruction.v
add_fileset_file demultiplexador.v VERILOG PATH modulos/demultiplexador/demultiplexador.v
add_fileset_file full_print_module.v VERILOG PATH modulos/full_print_module/full_print_module.v
add_fileset_file VGA_sync.v VERILOG PATH modulos/interfaceVGA/VGA_sync.v
add_fileset_file multiplexador.v VERILOG PATH modulos/multiplexador/multiplexador.v
add_fileset_file clock_pll.v VERILOG PATH modulos/PLL/clock_pll.v
add_fileset_file clock_pll_bb.v VERILOG PATH modulos/PLL/clock_pll_bb.v
add_fileset_file printModule.v VERILOG PATH modulos/printModule/printModule.v
add_fileset_file registerFile.v VERILOG PATH modulos/registerFile/registerFile.v
add_fileset_file sprite_line_counter.v VERILOG PATH modulos/sprite_line_counter/sprite_line_counter.v
add_fileset_file memory.v VERILOG PATH modulos/sprite_memory/memory.v
add_fileset_file memory_bb.v VERILOG PATH modulos/sprite_memory/memory_bb.v
add_fileset_file sprite_memory.v VERILOG PATH modulos/sprite_memory/sprite_memory.v


# 
# parameters
# 


# 
# display items
# 


# 
# connection point nios_custom_instruction_slave
# 
add_interface nios_custom_instruction_slave nios_custom_instruction end
set_interface_property nios_custom_instruction_slave clockCycle 0
set_interface_property nios_custom_instruction_slave operands 2
set_interface_property nios_custom_instruction_slave ENABLED true
set_interface_property nios_custom_instruction_slave EXPORT_OF ""
set_interface_property nios_custom_instruction_slave PORT_NAME_MAP ""
set_interface_property nios_custom_instruction_slave CMSIS_SVD_VARIABLES ""
set_interface_property nios_custom_instruction_slave SVD_ADDRESS_GROUP ""

add_interface_port nios_custom_instruction_slave clk_FPGA clk Input 1
add_interface_port nios_custom_instruction_slave clk_en clk_en Input 1
add_interface_port nios_custom_instruction_slave reset reset Input 1
add_interface_port nios_custom_instruction_slave dataA dataa Input 32
add_interface_port nios_custom_instruction_slave dataB datab Input 32


# 
# connection point conduit_end_R
# 
add_interface conduit_end_R conduit end
set_interface_property conduit_end_R associatedClock ""
set_interface_property conduit_end_R associatedReset ""
set_interface_property conduit_end_R ENABLED true
set_interface_property conduit_end_R EXPORT_OF ""
set_interface_property conduit_end_R PORT_NAME_MAP ""
set_interface_property conduit_end_R CMSIS_SVD_VARIABLES ""
set_interface_property conduit_end_R SVD_ADDRESS_GROUP ""

add_interface_port conduit_end_R R readdata Output 3


# 
# connection point conduit_end_G
# 
add_interface conduit_end_G conduit end
set_interface_property conduit_end_G associatedClock ""
set_interface_property conduit_end_G associatedReset ""
set_interface_property conduit_end_G ENABLED true
set_interface_property conduit_end_G EXPORT_OF ""
set_interface_property conduit_end_G PORT_NAME_MAP ""
set_interface_property conduit_end_G CMSIS_SVD_VARIABLES ""
set_interface_property conduit_end_G SVD_ADDRESS_GROUP ""

add_interface_port conduit_end_G G readdata Output 3


# 
# connection point conduit_end_B
# 
add_interface conduit_end_B conduit end
set_interface_property conduit_end_B associatedClock ""
set_interface_property conduit_end_B associatedReset ""
set_interface_property conduit_end_B ENABLED true
set_interface_property conduit_end_B EXPORT_OF ""
set_interface_property conduit_end_B PORT_NAME_MAP ""
set_interface_property conduit_end_B CMSIS_SVD_VARIABLES ""
set_interface_property conduit_end_B SVD_ADDRESS_GROUP ""

add_interface_port conduit_end_B B readdata Output 3


# 
# connection point conduit_end_hsync
# 
add_interface conduit_end_hsync conduit end
set_interface_property conduit_end_hsync associatedClock ""
set_interface_property conduit_end_hsync associatedReset ""
set_interface_property conduit_end_hsync ENABLED true
set_interface_property conduit_end_hsync EXPORT_OF ""
set_interface_property conduit_end_hsync PORT_NAME_MAP ""
set_interface_property conduit_end_hsync CMSIS_SVD_VARIABLES ""
set_interface_property conduit_end_hsync SVD_ADDRESS_GROUP ""

add_interface_port conduit_end_hsync out_hsync writeresponsevalid_n Output 1


# 
# connection point conduit_end_vsync
# 
add_interface conduit_end_vsync conduit end
set_interface_property conduit_end_vsync associatedClock ""
set_interface_property conduit_end_vsync associatedReset ""
set_interface_property conduit_end_vsync ENABLED true
set_interface_property conduit_end_vsync EXPORT_OF ""
set_interface_property conduit_end_vsync PORT_NAME_MAP ""
set_interface_property conduit_end_vsync CMSIS_SVD_VARIABLES ""
set_interface_property conduit_end_vsync SVD_ADDRESS_GROUP ""

add_interface_port conduit_end_vsync out_vsync writeresponsevalid_n Output 1


# 
# connection point conduit_printting
# 
add_interface conduit_printting conduit end
set_interface_property conduit_printting associatedClock ""
set_interface_property conduit_printting associatedReset ""
set_interface_property conduit_printting ENABLED true
set_interface_property conduit_printting EXPORT_OF ""
set_interface_property conduit_printting PORT_NAME_MAP ""
set_interface_property conduit_printting CMSIS_SVD_VARIABLES ""
set_interface_property conduit_printting SVD_ADDRESS_GROUP ""

add_interface_port conduit_printting out_printtingScreen writeresponsevalid_n Output 1

