# load input image path: loads the specified file, discards any existing
# history and makes this image the active image.

# filter filter width filter weights (same argument format from Q1): applies
# the specified filter to the previously active image. The filtered
# result becomes the new active image and is placed at the next step in
# the history. Any previous history from this point forwards must be
# truncated.

# undo: moves the active image backwards by one step in history, if this
# is possible.

# redo: moves the active image forwards by one step in history, if this is
# possible.


import ctypes
import pickle
import sys
import os

PIK = "history.pickle"
clib = ctypes.CDLL("./libfast_filter.so")
img_path = ""
current_index = 0
history = []

try:
	with open(PIK, "rb") as f:
		data = pickle.load(f)
		img_path = data[0]
		current_index = data[1]
		history = data[2]
except Exception as e:
	img_path = ""
	current_index = 0
	history = []


def load(argv):
	global img_path
	global current_index
	global history
	if len(argv) != 3:
		print "Usage load <input_image_path>"
		sys.exit(0)
	if not os.path.isfile(argv[2]):
		print "file does not exist"
		sys.exit(0)
	img_path = argv[2]
	current_index = 0
	history = []
	img_file = open(img_path, 'rb')
	result_file = open('result.bmp', 'wb')
	result_file.write(img_file.read())
	img_file.close()
	result_file.close()


def img_filter(input_file, output_file, width, weights):
	input_img_path = open(input_file, 'rb')
	input_img_data = input_img_path.read()
	output_img_data = ' ' * len(input_img_data)
	output_img_path = open(output_file, 'wb')

	CFloatArrayType = ctypes.c_float * len(weights)
	cfloat_array_instance = CFloatArrayType( *weights )

	# invocking c library function
	clib.doFiltering( ctypes.c_char_p(input_img_data), cfloat_array_instance, width, ctypes.c_char_p(output_img_data))
	output_img_path.write(output_img_data)
	input_img_path.close()
	output_img_path.close()



def do_filter(argv):
	
	number_of_args = 0
	try:
		number_of_args = 3+int(argv[0])*int(argv[0])
	except Exception as e:
		print "Usage filter <width> <weights_array>"
		sys.exit(0)

	# arguments
	if not os.path.isfile(img_path):
		print "file does not exist"
		sys.exit(0)

	for x in xrange(0, number_of_args-3):
		try:
			float(argv[x])
		except Exception as e:
			print "width and weights should be numbers"
			sys.exit(0)

	global current_index
	global history


	width = int(argv[0])
	weights = argv[1:]
	weights = map(float, weights)

	try:
		img_filter('result.bmp', 'result.bmp', width, weights)
	except Exception as e:
		img_filter(img_path, 'result.bmp', width, weights)
	

	current_index += 1
	history.append((width, weights))

def undo(argv):
	global img_path
	global current_index
	global history

	if current_index == 0:
		print "Undo is currently not available"
		sys.exit(0)

	current_index -= 1	
	output_img_path = open('result.bmp', 'wb')
	output_img_path.write(open(img_path, 'rb').read())
	output_img_path.close()

	for x in xrange(0, current_index):
		img_filter('result.bmp', 'result.bmp', history[x][0], history[x][1])

	

def redo(argv):
	global current_index

	if current_index >= len(history):
		print "Can not redo currently, you are at the latest step"
		sys.exit(0)
	img_filter('result.bmp', 'result.bmp', history[current_index][0], history[current_index][1])
	current_index += 1

def save_status(img_path, current_index, history):
	data = [img_path, current_index, history]
	with open(PIK, "wb") as f:
		pickle.dump(data, f)


def main():
	if sys.argv[1] == 'load':
		load(sys.argv)

	elif sys.argv[1] == 'filter':
		do_filter(sys.argv[2:])

	elif sys.argv[1] == 'undo':
		undo(sys.argv)

	elif sys.argv[1] == 'redo':
		redo(sys.argv)

	else:
		print "Command not found"
		sys.exit(0)

	save_status(img_path, current_index, history)

if __name__ == '__main__':
	main()
