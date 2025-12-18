cmd_/home/drivers/drivers/pz5/modules.order := {   echo /home/drivers/drivers/pz5/main.ko; :; } | awk '!x[$$0]++' - > /home/drivers/drivers/pz5/modules.order
