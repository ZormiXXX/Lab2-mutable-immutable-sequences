CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include


SRC_DIR = src
BUILD_DIR = build
INC_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

TARGET = $(BUILD_DIR)/lab2

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	@echo "Создание директории $(BUILD_DIR)..."
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	@echo "Линковка..."
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Компиляция $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Очистка..."
	rm -rf $(BUILD_DIR)

rebuild: clean all

run: $(TARGET)
	@echo "Запуск программы..."
	./$(TARGET)

test: $(TARGET)
	@echo "Запуск тестов..."
	./$(TARGET)

help:
	@echo "Доступные команды:"
	@echo "  make        - Скомпилировать проект"
	@echo "  make clean  - Удалить папку build"
	@echo "  make rebuild- Очистить и скомпилировать заново"
	@echo "  make run    - Скомпилировать и запустить"
	@echo "  make test   - Запустить тесты"
	@echo "  make help   - Показать эту справку"

-include $(OBJECTS:.o=.d)

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Генерация зависимостей для $<..."
	@$(CXX) $(CXXFLAGS) -MM -MT $(BUILD_DIR)/$*.o $< > $@

.PHONY: all clean rebuild run test help