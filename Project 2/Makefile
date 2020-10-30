CXX = mpiCC -std=c++11

all: sparsemat run_test

sparsemat: sparseMat.cpp
	${CXX} -o sparse sparseMat.cpp
	
run_test:
	mpirun -n 20 ./sparse

clean:
	rm sparse
