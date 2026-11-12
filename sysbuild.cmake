# Give the MCUboot child image its own code-partition selection. The board's
# default chosen partition is slot 0 because normal application builds must
# link there when MCUboot is enabled.
if(SB_CONFIG_BOOTLOADER_MCUBOOT)
  list(APPEND mcuboot_DTC_OVERLAY_FILE
       "${CMAKE_CURRENT_LIST_DIR}/sysbuild/mcuboot.overlay")
  list(REMOVE_DUPLICATES mcuboot_DTC_OVERLAY_FILE)
  set(mcuboot_DTC_OVERLAY_FILE "${mcuboot_DTC_OVERLAY_FILE}" CACHE INTERNAL "")
endif()
