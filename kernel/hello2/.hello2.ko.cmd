cmd_drivers/hello2/hello2.ko := aarch64-linux-gnu-ld -EL -fix-cortex-a53-843419 -r  -T /home/panzq/348dvb_v3/kernel/scripts/module-common.lds --build-id  -o drivers/hello2/hello2.ko drivers/hello2/hello2.o drivers/hello2/hello2.mod.o
