# Makefile for FN6806 Pricing Engine
# Compiler: clang++ with C++17

CXX = clang++
CXXFLAGS = -g -Wall -Wextra -Wmost -std=c++17

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
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run with default log level
run: $(TARGET)
	cd /var/home/yangye/work/MFEg/FN6806/FN6806 && 82-system/$(TARGET)

# Run with debug logging
debug: $(TARGET)
	cd /var/home/yangye/work/MFEg/FN6806/FN6806 && 82-system/$(TARGET) debug

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

# Dependencies (header files)
main.o: main.cpp app_config.h portfolio.h risk_report.h yield_curve.h logger.h
app_config.o: app_config.cpp app_config.h logger.h
portfolio.o: portfolio.cpp portfolio.h instrument_csv.h logger.h observer/risk_observer.h
instrument_csv.o: instrument_csv.cpp instrument_csv.h instrument.h european_option.h fixed_rate_bond.h logger.h
yield_curve.o: yield_curve.cpp yield_curve.h logger.h
fixed_rate_bond.o: fixed_rate_bond.cpp fixed_rate_bond.h instrument.h yield_curve.h logger.h
european_option.o: european_option.cpp european_option.h instrument.h logger.h
risk_report.o: risk_report.cpp risk_report.h portfolio.h csv_writer.h logger.h
observer/risk_limit_checker.o: observer/risk_limit_checker.cpp observer/risk_limit_checker.h observer/risk_observer.h portfolio.h instrument.h logger.h

.PHONY: all run debug clean
