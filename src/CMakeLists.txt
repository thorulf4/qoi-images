add_library(qoi INTERFACE)
add_library(Qoi::qoi ALIAS qoi)

target_sources(qoi PUBLIC
    FILE_SET qoiPublic
    TYPE HEADERS
    BASE_DIRS include
    FILES
        include/qoi/core.h
        include/qoi/encoder.h
        include/qoi/decoder.h
        include/qoi/types.h)

install(TARGETS qoi EXPORT QoiConfig FILE_SET qoiPublic)