add_library(serialib STATIC serialib/lib/serialib.cpp) # Makes new library target for serialib
target_include_directories(serialib PUBLIC serialib/lib/) # Adds serialib/lib folder as include directory


add_subdirectory(GraphicsUtils)
