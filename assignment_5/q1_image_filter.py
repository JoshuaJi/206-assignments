import ctypes
import sys

clib = ctypes.CDLL("./libfast_filter.so")

if len(sys.argv) != 13:
	sys.exit(0)
# arguments
input_img_path = open(sys.argv[1], 'rb').read()
output_img_data = ' ' * len(input_img_path)
print input_img_path
output_img_path = open(sys.argv[2], 'wb')
width = sys.argv[3]
weights = sys.argv[4:]
weights = map(float, weights)

CFloatArrayType = ctypes.c_float * len(weights)
cfloat_array_instance = CFloatArrayType( *weights )

# invocking c library function
clib.doFiltering( input_img_path, cfloat_array_instance, width, output_img_data)
# print output_img_data