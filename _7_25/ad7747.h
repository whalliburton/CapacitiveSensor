
#define R_STATUS        0
#define R_CAP_DATA_H    1
#define R_CAP_DATA_M    2
#define R_CAP_DATA_L    3
#define R_VT_DATA_H     4
#define R_VT_DATA_M     5
#define R_VT_DATA_L     6
#define R_CAP_SETUP     7
#define R_VT_SETUP      8
#define R_EXC_SETUP     9
#define R_CONFIGURATION 10
#define R_CAP_DAC_A     11
#define R_CAP_DAC_B     12
#define R_CAP_OFFSET_H  13
#define R_CAP_OFFSET_L  14
#define R_CAP_GAIN_H    15
#define R_CAP_GAIN_L    16
#define R_VOLT_GAIN_H   17
#define R_VOLT_GAIN_L   18
#define R_LAST_REGISTER 18

#define R_RESET 0xBF

const char* register_names[] = { "STATUS",
                                 "CAP_DATA_H",
                                 "CAP_DATA_M",
                                 "CAP_DATA_L",
                                 "VT_DATA_H",
                                 "VT_DATA_M",
                                 "VT_DATA_L",
                                 "CAP_SETUP",
                                 "VT_SETUP",
                                 "EXC_SETUP",
                                 "CONFIGURATION",
                                 "CAP_DAC_A",
                                 "CAP_DAC_B",
                                 "CAP_OFFSET_H",
                                 "CAP_OFFSET_L",
                                 "CAP_GAIN_H",
                                 "CAP_GAIN_L",
                                 "VOLT_GAIN_H",
                                 "VOLT_GAIN_L" };

