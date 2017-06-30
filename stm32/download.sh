#########################################################################
# File Name: download.sh
# Author: Juven
# mail: yeashenlee@163.com
# Created Time: 2017年06月17日 星期六 13时40分35秒
#########################################################################
#!/bin/bash
sudo stm32flash /dev/serial0 -w main.bin -v -R -g 0 -i 23,-24,24:-23
