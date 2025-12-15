cmd_/home/drivers/drivers/pz2/modules.order := {   echo /home/drivers/drivers/pz2/main.ko; :; } | awk '!x[$$0]++' - > /home/drivers/drivers/pz2/modules.order
