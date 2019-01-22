SRCS = painless.cpp solvers/SolverFactory.cpp solvers/Maple.cpp \
       clauses/ClauseFilter.cpp clauses/ClauseFilter.cpp utils/SatUtils.cpp \
       sharing/RadarSatSharing.cpp clauses/ClauseBuffer.cpp utils/System.cpp \
       working/SequentialWorker.cpp working/Portfolio.cpp sharing/Sharer.cpp

OBJS = $(addsuffix .o, $(basename $(SRCS)))

EXEC = painless

LIBS = -lmapleCOMSPS -L../mapleCOMSPS/build/release/lib/ -lpthread -lz -lm

CC = g++

CFLAGS = -I../mapleCOMSPS -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS \
           -O3 -std=c++11

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(OBJS) $(EXEC)
