#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import cgi
import cgitb
cgitb.enable()
import ctypes
import pickle
import sys
import os

PIK = "history.pickle"
clib = ctypes.CDLL("./libfast_filter.so")
img_path = ""
current_index = 0
history = []
print "Content-Type: text/html;charset=utf-8"


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


def load(image_file):
	global img_path
	global current_index
	global history

	try:
		os.remove(img_path)
	except Exception, e:
		pass
	
	temp_file = open(image_file.filename, 'wb')
	temp_file.write(image_file.value)
	temp_file.close()

	result_file = open('result.bmp', 'wb')
	result_file.write(image_file.value)
	result_file.close()
	img_path = image_file.filename
	current_index = 0
	history = []


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

	# arguments
	if not os.path.isfile(img_path):
		print "file does not exist"
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
	

	
	if current_index < len(history):
		history[current_index] = (width, weights)	
	else:
		history.append((width, weights))
	current_index += 1

def undo():
	global img_path
	global current_index
	global history

	if current_index == 0:
		print '<script>alert("Undo is currently not available, you just load the image")</script>'
		return
	current_index -= 1	
	output_img_path = open('result.bmp', 'wb')
	output_img_path.write(open(img_path, 'rb').read())
	output_img_path.close()

	for x in xrange(0, current_index):
		img_filter('result.bmp', 'result.bmp', history[x][0], history[x][1])

	

def redo():
	global current_index

	if current_index >= len(history):
		print '<script>alert("Can not redo currently, you are at the latest step")</script>'
		return
	img_filter('result.bmp', 'result.bmp', history[current_index][0], history[current_index][1])
	current_index += 1

def save_status(img_path, current_index, history):
	data = [img_path, current_index, history]
	with open(PIK, "wb") as f:
		pickle.dump(data, f)




print """
<html>

<head>
	<link rel="stylesheet" type="text/css" href="../css/animate.css">
	<link rel="stylesheet" type="text/css" href="../css/main.css">
	<link rel="stylesheet" type="text/css" href="../css/bootstrap.min.css">

</head>

<body>
<form name="input" action="./q3_cgi_filter.py" method="post" enctype="multipart/form-data" class="animated fadeIn">
 """
form = cgi.FieldStorage()
if form.has_key('load'):
	if form.has_key('photo') and form['photo'].value != '':
		load(form['photo'])
	else:
		print '<script>alert("please upload a file before click load button")</script>'
elif form.has_key('filter'):
	weights = [form['00'].value, form['01'].value, form['02'].value, form['10'].value, form['11'].value, form['12'].value, form['20'].value, form['21'].value, form['22'].value]
	weights = map(float, weights)
	weights.insert(0, 3)
	do_filter(weights)
elif form.has_key('undo'):
	undo()
elif form.has_key('redo'):
	redo()

save_status(img_path, current_index, history)

print """



  <p>Photo to Upload: <input type="file" name="photo" style="display:inline" /></p>
  <br>
  <p class="h4">First Filter to Apply:</p>
  <div class="weights">
  <p><input type="text" name="00" value="%f"> <input type="text" name="01" value="%f"> <input type="text" name="02"  value="%f"> </p>
  <p><input type="text" name="10" value="%f"> <input type="text" name="11" value="%f"> <input type="text" name="12" value="%f"> </p>
  <p><input type="text" name="20" value="%f"> <input type="text" name="21"  value="%f"> <input type="text" name="22" value="%f"> </p>
  </div>
  <input type="submit" value="Load" name="load" class="btn btn-info">
  <input type="submit" value="Filter" name="filter" class="btn btn-info">
  <input type="submit" value="Undo" name="undo" class="btn btn-info">
  <input type="submit" value="Redo" name="redo" class="btn btn-info">


</form>
<hl>
  	<img src="./result.bmp" class="animated tada">
  </hl>
</body>
</html>

"""%(float(form['00'].value), float(form['01'].value), float(form['02'].value), float(form['10'].value), float(form['11'].value), float(form['12'].value), float(form['20'].value), float(form['21'].value), float(form['22'].value))