add_library(rs232 STATIC RS-232/rs232.c)

add_library(serialib STATIC serialib/lib/serialib.cpp) # Makes new library target for serialib
target_include_directories(serialib PUBLIC serialib/lib/) # Adds serialib/lib folder as include directory