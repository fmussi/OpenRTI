#add_executable(ChatCC1516e_fm ChatCC1516e_fm.cpp)
#target_link_libraries(ChatCC1516e_fm rti1516e fedtime1516e OpenRTI)

add_executable(ChatCC1516e_fm_mt ChatCC1516e_fm_mt.cpp)
if(WIN32)
    target_link_libraries(ChatCC1516e_fm_mt rti1516e fedtime1516e OpenRTI)
endif()
if (UNIX AND NOT APPLE)
    target_link_libraries(ChatCC1516e_fm_mt rti1516e fedtime1516e OpenRTI pthread)
endif()

install(TARGETS ChatCC1516e_fm_mt
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(FILES Chat-evolved.xml
  DESTINATION "${OPENRTI_DATAROOTDIR}/samples")
