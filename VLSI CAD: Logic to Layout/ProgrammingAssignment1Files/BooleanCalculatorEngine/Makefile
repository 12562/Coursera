CXX := g++
CXXFLAGS := -g -Wall
INC := -I include

bin/BooleanCalculatorEngine: build/BooleanCalculatorEngine.o build/cmds.o build/boolean_fn_array.o build/pcn_cubes.o build/pcn_vals.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INC) -lboost_program_options 

build/BooleanCalculatorEngine.o: src/BooleanCalculatorEngine.cpp include/cmds_class.hpp include/boolean_fn_array_class.hpp include/pcn_cubes_class.hpp include/pcn_vals_class.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

build/cmds.o: src/cmds_class.cpp include/cmds_class.hpp include/boolean_fn_array_class.hpp include/pcn_cubes_class.hpp include/pcn_vals_class.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

build/boolean_fn_array.o: src/boolean_fn_array_class.cpp include/boolean_fn_array_class.hpp include/pcn_cubes_class.hpp include/pcn_vals_class.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

build/pcn_cubes.o: src/pcn_cubes_class.cpp include/pcn_cubes_class.hpp include/pcn_vals_class.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

build/pcn_vals.o: src/pcn_vals_class.cpp include/pcn_vals_class.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC)

clean:
	rm build/*.*

