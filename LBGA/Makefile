CXXFLAGS += -Wall -std=c++11
SRC_DIR = .

all: run

run : 
	$(CXX)  $(CXXFLAGS) \
	$(SRC_DIR)/*.cpp -I$(SRC_DIR)/* -pthread -o $@ 

clean : 
	$(RM) $(SRC_DIR)/run
