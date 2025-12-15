cmd_/home/drivers/drivers/pz4/modules.order := {   echo /home/drivers/drivers/pz4/main.ko; :; } | awk '!x[$$0]++' - > /home/drivers/drivers/pz4/modules.order
