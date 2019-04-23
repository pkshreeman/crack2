# -*- indent-tabs-mode:t; -*-

# The number of random hashes to pull from the rockyou file
NUM_HASHES=100

# Which rockyou file to use
#ROCKYOU=rockyou100.txt
#ROCKYOU=rockyou1000.txt
#ROCKYOU=rockyou2000.txt
ROCKYOU=rockyou1m.txt

all: hashpass crack

# This rule links hashpass.o and md5.o along with the
# libssl and libcrypto libraries to make the executable.
hashpass: hashpass.o md5.o
	clang hashpass.o md5.o -o hashpass -l crypto

md5.o: md5.c
	clang -g -c md5.c -Wall

hashpass.o: hashpass.c
	clang -g -c hashpass.c -Wall

crack: crack.o md5.o
	clang crack.o md5.o -o crack -l crypto

crack.o: crack.c
	clang -g -c crack.c -Wall

hashes: hashpass
	shuf -n $(NUM_HASHES) $(ROCKYOU) > passwords.txt
	./hashpass < passwords.txt > hashes.txt

hashes.txt: hashes

clean:
	rm -f *.o hashpass crack hashes.txt passwords.txt

test: crack hashes.txt
	./crack hashes.txt $(ROCKYOU)

check: crack hashes.txt
	valgrind ./crack hashes.txt $(ROCKYOU)
	
check2: crack hashes.txt
	valgrind ./crack hashes.txt $(ROCKYOU) -v --leak-check=full --xtree-memory=full  --xtree-leak=yes
