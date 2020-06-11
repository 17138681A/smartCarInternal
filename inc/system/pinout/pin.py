import sys
import re
import numpy as np

count=0

for line in open(sys.argv[1]):
	match = re.search('LPUART(.+?)0x(.+?),',line)
	if match:
		match_str=match.group(1)
		module=ord(match_str[0])-48-1
		if match_str[2:4]=='RX':
			match=re.search('#define IOMUXC_GPIO_(.+?)0x(.+?), 0x(.+?), (.+?), (.+?),',line)
			match_str=match.group(1)
			for i in range(len(match_str)):
				if match_str[i:i+2].isdigit():
					print(',{')
					result='Name::kGPIO_%s,Config::MuxMode::kAlt%s,%d,%s,%s'%(match_str[:i+2],match.group(3)[0],module,match.group(4),match.group(5))
					print(result)
					print('}')
					count+=1
					break

# for line in open(sys.argv[1]):
# 	match = re.search('XBAR1(.+?)0x(.+?), (.+?), (.+?), (.+?), (.+?)',line)
# 	if match:
# 		mux = match.group(3)[2]
# 		input_reg = match.group(4)
# 		input_value = match.group(5)
# 		match=re.search('#define IOMUXC_GPIO_(.+?)_XBAR1_',line)
# 		print(',{')
# 		result='Name::kGPIO_%s,Config::MuxMode::kAlt%s,%s,%s'%(match.group(1),mux,input_reg,input_value)
# 		print(result)
# 		print('}')
# 		count+=1
# 		# match_str=match.group(1)
# 		# module=ord(match_str[0])-48-1
# 		# if match_str[2:5]=='SDA':
# 		# 	match=re.search('#define IOMUXC_GPIO_(.+?)0x(.+?), 0x(.+?), (.+?), (.+?),',line)
# 		# 	match_str=match.group(1)
# 		# 	for i in range(len(match_str)):
# 		# 		if match_str[i:i+2].isdigit():
# 		# 			print(',{')
# 		# 			result='Name::kGPIO_%s,Config::MuxMode::kAlt%s,%d,%s,%s'%(match_str[:i+2],match.group(3)[0],module,match.group(4),match.group(5))
# 		# 			print(result)
# 		# 			print('}')
# 		# 			count+=1
# 		# 			break

# for line in open(sys.argv[1]):
# 	match = re.search('LPI2C(.+?)0x(.+?),',line)
# 	if match:
# 		match_str=match.group(1)
# 		module=ord(match_str[0])-48-1
# 		if match_str[2:5]=='SDA':
# 			match=re.search('#define IOMUXC_GPIO_(.+?)0x(.+?), 0x(.+?), (.+?), (.+?),',line)
# 			match_str=match.group(1)
# 			for i in range(len(match_str)):
# 				if match_str[i:i+2].isdigit():
# 					print(',{')
# 					result='Name::kGPIO_%s,Config::MuxMode::kAlt%s,%d,%s,%s'%(match_str[:i+2],match.group(3)[0],module,match.group(4),match.group(5))
# 					print(result)
# 					print('}')
# 					count+=1
# 					break

# for line in open(sys.argv[1]):
# 	match = re.search('FLEXPWM(.+?)0x(.+?),',line)
# 	if match:
# 		match_str=match.group(1)
# 		module=ord(match_str[0])-48-1
# 		if match_str[5]=='A':
# 			channel=0
# 		elif match_str[5]=='B':
# 			channel=1
# 		elif match_str[5]=='X':
# 			channel=2
# 		else:
# 			channel=3
# 		sub_module=ord(match_str[7])-48
# 		match=re.search('#define IOMUXC_GPIO_(.+?)0x(.+?), 0x(.+?)',line)
# 		match_str=match.group(1)
# 		for i in range(len(match_str)):
# 			if match_str[i:i+2].isdigit():
# 				print(',{')
# 				result='Name::kGPIO_%s,Config::MuxMode::kAlt%s,(%d<<5|%d<<2|%d)'%(match_str[:i+2],match.group(3)[0],module,sub_module,channel)
# 				print(result)
# 				print('}')
# 				count+=1
# 				break

print(count)

# pin=np.empty((4,4,3),dtype=str)

# mod_count=int(sys.argv[2])
# sub_count=int(sys.argv[3])
# channel_count=int(sys.argv[4])

# for mod_count in range(0,4):
# 	print('{')
# 	for sub_count in range(0,4):
# 		print('{')
# 		for channel_count in range(0,3):
# 			print('{')
# 			for line in open(sys.argv[1]):
# 				match = re.search('FLEXPWM(.+?)0x(.+?),',line)
# 				if match:
# 					match_str=match.group(1)
# 					module=ord(match_str[0])-48-1
# 					if match_str[5]=='A':
# 						channel=0
# 					elif match_str[5]=='B':
# 						channel=1
# 					elif match_str[5]=='X':
# 						channel=2
# 					else:
# 						channel=3
# 					sub_module=ord(match_str[7])-48
# 					if module==mod_count and sub_count==sub_module and channel==channel_count:
# 						match=re.search('#define IOMUXC_GPIO_(.+?)0x(.+?),',line)
# 						match_str=match.group(1)
# 						for i in range(len(match_str)):
# 							if match_str[i:i+2].isdigit():
# 								result=',System::Pinout::Name::kGPIO_%s'%(match_str[:i+2])
# 								print(result)
# 								break
# 			if(channel_count==2):
# 				print('}')
# 			else:
# 				print('},')
# 		if(sub_count==3):
# 			print('}')
# 		else:
# 			print('},')
# 	if(mod_count==3):
# 		print('}')
# 	else:
# 		print('},')

# print(pin)
