include makeinclude

#########################

LOCAL_FILES = EVILib.o\
	EVI-D30g.o\
	EVI-D30.o\
	EVI-D31.o\
	EVI-D70g.o\
	EVI-D70.o\
	EVI-D70P.o\
	EVI-D100g.o\
	EVI-D100.o\
	EVI-D100P.o\
	EVILib.a

#########################

ALL = $(LOCAL_FILES)

main: $(ALL)

all: $(ALL)

#########################

EVILib.o: EVILib.c++ EVILib.h EVI-D30g.o EVI-D70g.o EVI-D100g.o
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D30g.o: EVI-D30g.c++ EVI-D30g.h EVI-D30.o EVI-D31.o
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D30.o: EVI-D30.c++ EVI-D30.h
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D31.o: EVI-D31.c++ EVI-D31.h
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D70g.o: EVI-D70g.c++ EVI-D70g.h EVI-D70.o EVI-D70P.o
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D70.o: EVI-D70.c++ EVI-D70.h
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D70P.o: EVI-D70P.c++ EVI-D70P.h
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D100g.o: EVI-D100g.c++ EVI-D100g.h EVI-D100.o EVI-D100P.o
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D100.o: EVI-D100.c++ EVI-D100.h
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVI-D100P.o: EVI-D100P.c++ EVI-D100P.h
	@echo Compiling $@
	$(CC) $(OPTIONS) $(INCLUDES) -o $@ -c $<

EVILib_Files = EVILib.o EVI-D30g.o EVI-D30.o EVI-D31.o EVI-D100g.o EVI-D100.o EVI-D100P.o EVI-D70g.o EVI-D70.o EVI-D70P.o
EVILib.a : $(EVILib_Files)
	@echo == making $@ ==
	$(AR) -rc $@ $(EVILib_Files);
	@echo

#########################

clean:
	-$(RM) *~

rclean:
	$(MAKE) clean
	@for files in $(LOCAL_FILES); do\
		($(RM) $$files);\
	done

clock:
	$(TOUCH) *
