# INST

Generate a random sparse matrix:
    python genSparse.py

To change matrix parameters:
    OPEN:
        genSparse.py

        CHANGE lines 5 (var[n]) and lines 6 (var[m])
        n = rows
        m = columns

    RUN:
        python genSparse.py

    OPEN:
        Makefile

        CHANGE line 9 parameter to new n value
            ex. (n = 30):
                mpirun -n <n> ./sparse
                to:
                mpirun -n 30 ./sparse

TO RUN:
    make
