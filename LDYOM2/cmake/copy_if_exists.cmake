# copy_if_exists.cmake — копирует SRC -> DST только если SRC существует
if(EXISTS "${SRC}")
    file(COPY "${SRC}/" DESTINATION "${DST}")
else()
    message(STATUS "Opcode nodes source not found, skipping deploy: ${SRC}")
endif()
