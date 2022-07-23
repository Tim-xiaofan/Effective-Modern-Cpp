# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2010-2014 Intel Corporation

# binary name
APP = item32

# all source are stored in SRCS-y
SRCS-y := *.cpp 

# Add flag 
CPPFLAGS += -g -Wall -std=c++11

$(APP): $(SRCS-y) Makefile 
	$(CXX) $(CPPFLAGS) $(SRCS-y) -o $@


.PHONY: clean
clean:
	echo "Linux rm"
	rm -f *.o
	rm -f *.exe
	rm -f $(APP)
