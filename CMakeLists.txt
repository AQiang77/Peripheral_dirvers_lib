idf_component_register(
    SRCS
        "src/drivers/charge/axp517/axp517.cpp"
        "port/esp_idf/src/i2c_bus.cpp"
        "port/esp_idf/src/i2c_device.cpp"
    INCLUDE_DIRS
        "include"
        "port/esp_idf/include"
    REQUIRES
        esp_driver_i2c
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)
