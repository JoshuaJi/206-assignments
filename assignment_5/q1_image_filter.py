import ctypes
import sys

clib = ctypes.CDLL("./libfast_filter.so")

number_of_args = 0
try:
	number_of_args = 4+int(sys.argv[3])*int(sys.argv[3])
except Exception as e:
	print "Usage python q1_image_filter.py <input_file> <output_file> <width> <weights_array>"
	sys.exit(0)

if len(sys.argv) != number_of_args:
	print "Usage python q1_image_filter.py <input_file> <output_file> <width> <weights_array>"
	sys.exit(0)

for x in xrange(0, number_of_args-3):
	try:
		float(sys.argv[x + 3])
	except Exception as e:
		print "width and weights should be numbers"
		sys.exit(0)
		
# arguments
input_img_path = open(sys.argv[1], 'rb')
input_img_data = input_img_path.read()
output_img_data = ' ' * len(input_img_data)
output_img_path = open(sys.argv[2], 'wb')
width = int(sys.argv[3])
weights = sys.argv[4:]
weights = map(float, weights)

CFloatArrayType = ctypes.c_float * len(weights)
cfloat_array_instance = CFloatArrayType( *weights )

# invocking c library function
clib.doFiltering( ctypes.c_char_p(input_img_data), cfloat_array_instance, width, ctypes.c_char_p(output_img_data))
output_img_path.write(output_img_data)
input_img_path.close()
output_img_path.close()
# print output_img_data