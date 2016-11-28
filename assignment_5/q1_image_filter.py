import ctypes
import sys

clib = ctypes.CDLL("./libfast_filter.so")

if len(sys.argv) != 13:
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