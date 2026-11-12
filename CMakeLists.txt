cmake_minimum_required(VERSION 3.20.0)

# Keep this application and its board definition outside the Zephyr tree.
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(speeder_pdu_canopen)

# Export the selected dictionary to CANopenNode's separate library target.
# This also isolates the application from legacy object-dictionary paths in
# the shared workspace module.
zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/objdict)

target_sources(app PRIVATE
	src/main.c
	src/pdu_adc.c
	src/pdu_control.c
	src/pdu_telemetry.c
	objdict/CO_OD.c
)
target_include_directories(app PRIVATE objdict)

# The shared CANopenNode module in this laptop predates conditional program
# download source registration. Add that source from the discovered module
# only for the update-capable build, without modifying the shared module.
if(CONFIG_CANOPENNODE_PROGRAM_DOWNLOAD)
	target_sources(app PRIVATE
		${ZEPHYR_CANOPENNODEZEPHYR_MODULE_DIR}/src/canopen_program.c
		${ZEPHYR_CANOPENNODEZEPHYR_MODULE_DIR}/src/canopen_leds.c
	)
endif()
