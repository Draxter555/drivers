cmd_/home/drivers/drivers/lab1/modules.order := {   echo /home/drivers/drivers/lab1/main.ko; :; } | awk '!x[$$0]++' - > /home/drivers/drivers/lab1/modules.order
