cmd_/home/drivers/drivers/pz3/modules.order := {   echo /home/drivers/drivers/pz3/main.ko; :; } | awk '!x[$$0]++' - > /home/drivers/drivers/pz3/modules.order
