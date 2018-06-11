CC=g++
CXXFLAGS=-I./inc -I../inc
LIBS=-L../../lib -lboost_system -lboost_log -lboost_log_setup -lboost_thread -lboost_date_time -lpthread
SRCS=main.cpp \
     Application.cpp \
     Config.cpp \
     Server.cpp \
     SessionInside.cpp \
     SessionOutside.cpp \
     Log.cpp

OBJS=$(SRCS:.cpp=.o)

TARGET=$(HOME)/bin/QuoteServer
$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

.cpp.o:
	$(CC) -c -Wall -DBOOST_ALL_DYN_LINK -std=c++11 $(CXXFLAGS) $<


.PHONY:clean  
clean:  
	-rm -f $(OBJS) $(TARGET)
