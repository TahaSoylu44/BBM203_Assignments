# Derleyici ve Bayraklar (Flags)
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Çıktı dosyasının adı
TARGET = QuakeAssist

# Kaynak dosyalar
SRCS = main.cpp \
       MissionStack.cpp \
       QuakeAssistController.cpp \
       Request.cpp \
       RequestQueue.cpp \
       Team.cpp

# Obje dosyaları
OBJS = $(SRCS:.cpp=.o)

# .PHONY: Dosya ismiyle çakışmayı önler (clean veya valgrind diye dosya varsa karışmasın diye)
.PHONY: all clean valgrind gdb

all: $(TARGET)

# Ana kural: Hedef dosyayı oluştur
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# .cpp dosyalarını .o dosyalarına derle
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Valgrind Kuralı (Eğer output dosyası argümanı gerekmiyorsa son kısmı sil)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(TARGET) example_input.txt myoutput.txt

# GDB Kuralı (Eğer output dosyası argümanı gerekmiyorsa son kısmı sil)
gdb: $(TARGET)
	gdb -tui --args ./$(TARGET) example_input.txt myoutput.txt

# Temizlik kuralı
clean:
	rm -f $(OBJS) $(TARGET)