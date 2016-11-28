import ctypes
import sys

clib = ctypes.cdll.LoadLibrary("libfast_filter.so")

if len(sys.argv) != 13:
	sys.exit(0)
# arguments
input_img_path = sys.argv[1]
output_img_path = sys.argv[2]
width = sys.argv[3]
weights = sys.argv[4:]

print input_img_path
print output_img_path
print width
print weights

# invocking c library function
#clib.doFiltering( unsigned char* img_data, float* filter_weights,       
#                  int filter_width,        unsigned char* out_img_data )