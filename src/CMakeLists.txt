add_library(qoi encoder.cpp decoder.cpp)
add_library(Qoi::qoi ALIAS qoi)

target_sources(qoi PUBLIC
    FILE_SET qoiPublic
    TYPE HEADERS
    BASE_DIRS include
    FILES
        include/qoi/qoi.h
        include/qoi/encoder.h
        include/qoi/decoder.h)

install(TARGETS qoi EXPORT QoiConfig FILE_SET qoiPublic)