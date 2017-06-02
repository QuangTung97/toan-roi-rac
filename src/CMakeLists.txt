project(src)

add_library(src STATIC 
                matrix.cpp  
                overflow_time.cpp  
                path.cpp  
                position_reader.cpp  
                solver.cpp)
