obj-m = arnamodule.o
#.DEFAULT_GOAL := install
install:
	@echo "$(tput setaf 0) $(tput setab 3) Making install :) $(tput sgr 0)"
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	@echo "$(tput setaf 0) $(tput setab 9) Making clean :( $(tput sgr 0)"
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

	