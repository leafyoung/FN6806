# Makefile for FN6806 Pricing Engine
# Compiler: clang++ with C++17

CXX = clang++
CXXFLAGS = -g -Wall -Wextra -Wmost -std=c++17
# -MMD -MP: generate .d files so editing a header rebuilds its users
DEPFLAGS = -MMD -MP

# Source files
SRCS = main.cpp app_config.cpp portfolio.cpp instrument_csv.cpp yield_curve.cpp fixed_rate_bond.cpp european_option.cpp risk_report.cpp observer/risk_limit_checker.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target
TARGET = pricing_engine

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# Run with default log level
run: $(TARGET)
	cd .. && 82-system/$(TARGET)

# Run with debug logging
debug: $(TARGET)
	cd .. && 82-system/$(TARGET) debug

# Clean
clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(TARGET)


-include $(OBJS:.o=.d)

.PHONY: all run debug clean
