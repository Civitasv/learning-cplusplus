add_library(qrcode qrcode.cc)

target_include_directories(qrcode PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)