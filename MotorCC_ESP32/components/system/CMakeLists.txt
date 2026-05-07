idf_component_register(SRCS "system.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver motor_dc encoder drv8833 env)
