import sys

def sanity_check(argv):
	if len(sys.argv) != 2:
		print "Please provide only the file name"
		sys.exit(0)

def process_file(text_file):
	word_list = {};
	line = text_file.read()
	line = line.replace('-', ' ')
	line = ''.join([i for i in line if (i.isalpha() or i == ' ')])
	
	for word in line.split(' '):
		word = word.strip(".,!?()\{\}[]\'\"")
		word = word.lower()
		if word == '':
			continue
		if word in word_list:
			word_list[word] += 1
		else:
			word_list[word] = 1
	word_list = sorted(word_list.items(), key=lambda x:x[1], reverse=True)
	return word_list

def print_words(word_list):
	for (k,v) in word_list:
		print k+':'+str(v)

def main():
	sanity_check(sys.argv);
	try:
		text_file = open(sys.argv[1]);
	except Exception as e:
		print "File provided does not exist"
		sys.exit(0)
	
	word_list = process_file(text_file)
	print_words(word_list)

if __name__ == '__main__':
	main()