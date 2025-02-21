CC = g++ -Wall -std=c++11 
TARGET1 = main
TARGET2 = getdata
TARGET3 = RandomForest


all: $(TARGET1).cpp $(TARGET2).cpp $(TARGET3).cpp
	$(CC) -c $(TARGET1).cpp $(TARGET2).cpp $(TARGET3).cpp
	$(CC) -o $(TARGET1) $(TARGET1).o $(TARGET2).o $(TARGET3).o
	

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)
	rm -f $(TARGET1).o $(TARGET2).o $(TARGET3).o