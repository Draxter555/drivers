cmd_/home/drivers/drivers/pz6/modules.order := {   echo /home/drivers/drivers/pz6/main.ko; :; } | awk '!x[$$0]++' - > /home/drivers/drivers/pz6/modules.order
