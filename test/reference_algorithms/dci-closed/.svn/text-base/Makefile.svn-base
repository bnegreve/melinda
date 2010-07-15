CC  =g++
LFS_SUPP=
CFLAGS =`getconf LFS_CFLAGS` -Wall --pedantic -O6 -finline-functions -DREENTRANT
#CFLAGS =`getconf LFS_CFLAGS` -Wall --pedantic -DREENTRANT -g

#CFLAGS = -Wall --pedantic -finline-functions -DREENTRANT -gdwarf-2 -g3 -O3


#CC = xlc++ -q32 -O3


HEADER  = database.hh first_scan.hh second_scan.hh items.hh   \
          my_defs.hh  tid_list_macros.hh  utils.hh writer.hh \
          memory.hh \
          stat.hh transaction.hh vertical.hh  \
          third_scan.hh	 outputunmap.hh \
          fourth_scan.hh print.hh 

OBJS	= database.o fim_all.o items.o memory.o utils.o \
          stat.o first_scan.o second_scan.o third_scan.o \
          outputunmap.o fourth_scan.o print.o 

LIBS = 

ADDFLAGS =

TARGET  = multi-closed

default: $(TARGET)

multi-closed: $(OBJS) $(HEADER) 
	$(CC) $(CFLAGS) -lpthread -o $(TARGET) $(OBJS) $(LIBS)
	strip $(TARGET)

all:
	make clean && make ssn
	make clean && make sdn
	make clean && make ddn

	make clean && make ssp
	make clean && make sdp
	make clean && make ddp
	make clean && make multi-closed

# ------------------------------------------
# various policies
# ssn
ssn: ADDFLAGS = -DPARTITIONING_POLICY=4
ssn: TARGET   = ssn
ssn: $(OBJS) $(HEADER) 
	$(CC) $(CFLAGS) -lpthread -o $(TARGET) $(OBJS) $(LIBS)
	strip $(TARGET)

sdn: ADDFLAGS = -DPARTITIONING_POLICY=3
sdn: TARGET   = sdn
sdn: $(OBJS) $(HEADER) 
	$(CC) $(CFLAGS) -lpthread -o $(TARGET) $(OBJS) $(LIBS)
	strip $(TARGET)

ddn: ADDFLAGS = -DPARTITIONING_POLICY=5
ddn: TARGET   = ddn
ddn: $(OBJS) $(HEADER) 
	$(CC) $(CFLAGS) -lpthread -o $(TARGET) $(OBJS) $(LIBS)
	strip $(TARGET)

ssp: ADDFLAGS = -DPARTITIONING_POLICY=6
ssp: TARGET   = ssp
ssp: $(OBJS) $(HEADER) 
	$(CC) $(CFLAGS) -lpthread -o $(TARGET) $(OBJS) $(LIBS)
	strip $(TARGET)

sdp: ADDFLAGS = -DPARTITIONING_POLICY=7
sdp: TARGET   = sdp
sdp: $(OBJS) $(HEADER) 
	$(CC) $(CFLAGS) -lpthread -o $(TARGET) $(OBJS) $(LIBS)
	strip $(TARGET)

ddp: ADDFLAGS = -DPARTITIONING_POLICY=8
ddp: TARGET   = ddp
ddp: $(OBJS) $(HEADER) 
	$(CC) $(CFLAGS) -lpthread -o $(TARGET) $(OBJS) $(LIBS)
	strip $(TARGET)



.SUFFIXES: .o .cc

.cc.o:
	$(CC) $(CFLAGS) $(ADDFLAGS) -c $<       

depend:
	makedepend $(CFLAGS) *.cc
clean:
	rm -f *~  $(OBJS) 
remove:
	rm -f *~  $(OBJS) $(TARGET) $(TARGET).exe ssn ssn.exe sdn sdn.exe ddn ddn.exe ssp ssp.exe sdp sdp.exe ddp ddp.exe


# dependencies
# DO NOT DELETE

database.o: database.hh /usr/include/stdio.h /usr/include/features.h
database.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
database.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
database.o: /usr/include/bits/typesizes.h /usr/include/libio.h
database.o: /usr/include/_G_config.h /usr/include/wchar.h
database.o: /usr/include/bits/wchar.h /usr/include/gconv.h
database.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
database.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
database.o: /usr/include/bits/confname.h /usr/include/getopt.h transaction.hh
database.o: items.hh my_defs.hh /usr/include/string.h
fim_all.o: utils.hh /usr/include/unistd.h /usr/include/features.h
fim_all.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fim_all.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
fim_all.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
fim_all.o: /usr/include/bits/confname.h /usr/include/getopt.h
fim_all.o: /usr/include/sys/time.h /usr/include/time.h
fim_all.o: /usr/include/bits/time.h /usr/include/sys/select.h
fim_all.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
fim_all.o: /usr/include/pthread.h /usr/include/sched.h
fim_all.o: /usr/include/bits/sched.h /usr/include/signal.h
fim_all.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
fim_all.o: /usr/include/bits/sigthread.h database.hh /usr/include/stdio.h
fim_all.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
fim_all.o: /usr/include/bits/wchar.h /usr/include/gconv.h
fim_all.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fim_all.o: transaction.hh items.hh my_defs.hh /usr/include/string.h stat.hh
fim_all.o: print.hh first_scan.hh second_scan.hh third_scan.hh fourth_scan.hh
fim_all.o: outputunmap.hh memory.hh /usr/include/sys/types.h
fim_all.o: /usr/include/endian.h /usr/include/bits/endian.h
fim_all.o: /usr/include/sys/sysmacros.h
first_scan.o: first_scan.hh items.hh /usr/include/stdio.h
first_scan.o: /usr/include/features.h /usr/include/sys/cdefs.h
first_scan.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
first_scan.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
first_scan.o: /usr/include/libio.h /usr/include/_G_config.h
first_scan.o: /usr/include/wchar.h /usr/include/bits/wchar.h
first_scan.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
first_scan.o: /usr/include/bits/sys_errlist.h my_defs.hh stat.hh database.hh
first_scan.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
first_scan.o: /usr/include/bits/confname.h /usr/include/getopt.h
first_scan.o: transaction.hh /usr/include/string.h utils.hh
first_scan.o: /usr/include/sys/time.h /usr/include/time.h
first_scan.o: /usr/include/bits/time.h /usr/include/sys/select.h
first_scan.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
first_scan.o: /usr/include/pthread.h /usr/include/sched.h
first_scan.o: /usr/include/bits/sched.h /usr/include/signal.h
first_scan.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
first_scan.o: /usr/include/bits/sigthread.h print.hh
fourth_scan.o: fourth_scan.hh my_defs.hh stat.hh /usr/include/sys/time.h
fourth_scan.o: /usr/include/features.h /usr/include/sys/cdefs.h
fourth_scan.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
fourth_scan.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
fourth_scan.o: /usr/include/time.h /usr/include/bits/time.h
fourth_scan.o: /usr/include/sys/select.h /usr/include/bits/select.h
fourth_scan.o: /usr/include/bits/sigset.h /usr/include/unistd.h
fourth_scan.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
fourth_scan.o: /usr/include/getopt.h utils.hh /usr/include/pthread.h
fourth_scan.o: /usr/include/sched.h /usr/include/bits/sched.h
fourth_scan.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
fourth_scan.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
fourth_scan.o: database.hh /usr/include/stdio.h /usr/include/libio.h
fourth_scan.o: /usr/include/_G_config.h /usr/include/wchar.h
fourth_scan.o: /usr/include/bits/wchar.h /usr/include/gconv.h
fourth_scan.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fourth_scan.o: transaction.hh items.hh /usr/include/string.h print.hh
items.o: items.hh /usr/include/stdio.h /usr/include/features.h
items.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
items.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
items.o: /usr/include/bits/typesizes.h /usr/include/libio.h
items.o: /usr/include/_G_config.h /usr/include/wchar.h
items.o: /usr/include/bits/wchar.h /usr/include/gconv.h
items.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
items.o: my_defs.hh print.hh
memory.o: /usr/include/stdio.h /usr/include/features.h
memory.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
memory.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
memory.o: /usr/include/bits/typesizes.h /usr/include/libio.h
memory.o: /usr/include/_G_config.h /usr/include/wchar.h
memory.o: /usr/include/bits/wchar.h /usr/include/gconv.h
memory.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
memory.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
memory.o: /usr/include/endian.h /usr/include/bits/endian.h
memory.o: /usr/include/sys/select.h /usr/include/bits/select.h
memory.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
memory.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
memory.o: /usr/include/bits/sched.h /usr/include/alloca.h
memory.o: /usr/include/string.h memory.hh /usr/include/unistd.h
memory.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
memory.o: /usr/include/getopt.h
outputunmap.o: outputunmap.hh my_defs.hh stat.hh utils.hh
outputunmap.o: /usr/include/unistd.h /usr/include/features.h
outputunmap.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
outputunmap.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
outputunmap.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
outputunmap.o: /usr/include/bits/confname.h /usr/include/getopt.h
outputunmap.o: /usr/include/sys/time.h /usr/include/time.h
outputunmap.o: /usr/include/bits/time.h /usr/include/sys/select.h
outputunmap.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
outputunmap.o: /usr/include/pthread.h /usr/include/sched.h
outputunmap.o: /usr/include/bits/sched.h /usr/include/signal.h
outputunmap.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
outputunmap.o: /usr/include/bits/sigthread.h database.hh /usr/include/stdio.h
outputunmap.o: /usr/include/libio.h /usr/include/_G_config.h
outputunmap.o: /usr/include/wchar.h /usr/include/bits/wchar.h
outputunmap.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
outputunmap.o: /usr/include/bits/sys_errlist.h transaction.hh items.hh
outputunmap.o: /usr/include/string.h print.hh
print.o: print.hh /usr/include/stdio.h /usr/include/features.h
print.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
print.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
print.o: /usr/include/bits/typesizes.h /usr/include/libio.h
print.o: /usr/include/_G_config.h /usr/include/wchar.h
print.o: /usr/include/bits/wchar.h /usr/include/gconv.h
print.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
print.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/time.h
print.o: /usr/include/bits/sched.h /usr/include/signal.h
print.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
print.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
second_scan.o: second_scan.hh items.hh /usr/include/stdio.h
second_scan.o: /usr/include/features.h /usr/include/sys/cdefs.h
second_scan.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
second_scan.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
second_scan.o: /usr/include/libio.h /usr/include/_G_config.h
second_scan.o: /usr/include/wchar.h /usr/include/bits/wchar.h
second_scan.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
second_scan.o: /usr/include/bits/sys_errlist.h my_defs.hh stat.hh database.hh
second_scan.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
second_scan.o: /usr/include/bits/confname.h /usr/include/getopt.h
second_scan.o: transaction.hh /usr/include/string.h print.hh utils.hh
second_scan.o: /usr/include/sys/time.h /usr/include/time.h
second_scan.o: /usr/include/bits/time.h /usr/include/sys/select.h
second_scan.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
second_scan.o: /usr/include/pthread.h /usr/include/sched.h
second_scan.o: /usr/include/bits/sched.h /usr/include/signal.h
second_scan.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
second_scan.o: /usr/include/bits/sigthread.h /usr/include/math.h
second_scan.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
second_scan.o: /usr/include/bits/mathcalls.h
stat.o: utils.hh /usr/include/unistd.h /usr/include/features.h
stat.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
stat.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
stat.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
stat.o: /usr/include/bits/confname.h /usr/include/getopt.h
stat.o: /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h
stat.o: /usr/include/sys/select.h /usr/include/bits/select.h
stat.o: /usr/include/bits/sigset.h /usr/include/pthread.h
stat.o: /usr/include/sched.h /usr/include/bits/sched.h /usr/include/signal.h
stat.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
stat.o: /usr/include/bits/sigthread.h database.hh /usr/include/stdio.h
stat.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
stat.o: /usr/include/bits/wchar.h /usr/include/gconv.h
stat.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
stat.o: transaction.hh items.hh my_defs.hh /usr/include/string.h
third_scan.o: third_scan.hh items.hh /usr/include/stdio.h
third_scan.o: /usr/include/features.h /usr/include/sys/cdefs.h
third_scan.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
third_scan.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
third_scan.o: /usr/include/libio.h /usr/include/_G_config.h
third_scan.o: /usr/include/wchar.h /usr/include/bits/wchar.h
third_scan.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
third_scan.o: /usr/include/bits/sys_errlist.h my_defs.hh stat.hh
third_scan.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/time.h
third_scan.o: /usr/include/bits/sched.h /usr/include/signal.h
third_scan.o: /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h
third_scan.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
third_scan.o: database.hh /usr/include/unistd.h /usr/include/bits/posix_opt.h
third_scan.o: /usr/include/bits/confname.h /usr/include/getopt.h
third_scan.o: transaction.hh /usr/include/string.h vertical.hh
third_scan.o: tid_list_macros.hh /usr/include/stdlib.h
third_scan.o: /usr/include/sys/types.h /usr/include/endian.h
third_scan.o: /usr/include/bits/endian.h /usr/include/sys/select.h
third_scan.o: /usr/include/bits/select.h /usr/include/bits/time.h
third_scan.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h print.hh
third_scan.o: memory.hh utils.hh /usr/include/sys/time.h
utils.o: /usr/include/stdio.h /usr/include/features.h
utils.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
utils.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
utils.o: /usr/include/bits/typesizes.h /usr/include/libio.h
utils.o: /usr/include/_G_config.h /usr/include/wchar.h
utils.o: /usr/include/bits/wchar.h /usr/include/gconv.h
utils.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
utils.o: utils.hh /usr/include/unistd.h /usr/include/bits/posix_opt.h
utils.o: /usr/include/bits/confname.h /usr/include/getopt.h
utils.o: /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h
utils.o: /usr/include/sys/select.h /usr/include/bits/select.h
utils.o: /usr/include/bits/sigset.h /usr/include/pthread.h
utils.o: /usr/include/sched.h /usr/include/bits/sched.h /usr/include/signal.h
utils.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
utils.o: /usr/include/bits/sigthread.h database.hh transaction.hh items.hh
utils.o: my_defs.hh /usr/include/string.h
