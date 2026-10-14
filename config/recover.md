# Recover command

openocd -f recover.cfg -c "init; reset halt; stm32f7x mass_erase 0; exit"

# In case recover.cfg goes missing

echo 'source [find interface/stlink.cfg]
source [find target/stm32f4x.cfg]
reset_config srst_only srst_nogate
adapter speed 480' > recover.cfg

# Mass erase command 

openocd -f recover.cfg -c "init; reset halt; stm32f7x mass_erase 0; exit"

# Once completed

make flash 


