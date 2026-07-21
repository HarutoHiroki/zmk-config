# Custom OLED status screen (bongo cat + label) for the Lily58.
if(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM)
  target_include_directories(app PRIVATE src)
  target_sources(app PRIVATE
    src/widgets/bongo_cat_images.c
    src/widgets/bongo_cat.c
    src/custom_status_screen.c
  )
endif()
