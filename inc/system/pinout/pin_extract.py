import sys
import re

prev_match=''

for line in open(sys.argv[1]):
	match = re.search('#define IOMUXC_GPIO_(.+?)0x(.+?),',line)
	if match:
		match_str=match.group(1)
		module=''
		for i in range(len(match_str)):
			if match_str[i:i+2].isdigit():
				module=match_str[i+3:]
				if prev_match == match_str[:i+2]:
					print('- %s'%(module))
				else:
					prev_match=match_str[:i+2]
					print('\n**System::Pinout::Name::kGPIO_%s**'%(prev_match))
				break