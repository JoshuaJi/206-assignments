import sys

# sanity chekc the number of arguments
def sanity_check(argv):
	if len(sys.argv) != 2:
		print "Please provide only the file name"
		sys.exit(0)

# take a text file and extracts the word as well as their frequency
def process_file(text_file):
	word_list = {}

	# read file and only keep alphabet and spaces
	line = text_file.read()
	line = line.replace('-', ' ')
	line = line.replace('\n', ' ')
	line = ''.join([i for i in line if (i.isalpha() or i == ' ')])
	
	# for each word, if it's not empty, add it in the dictionary
	for word in line.split(' '):
		word = word.strip(".,!?()\{\}[]\'\"")
		word = word.lower()

		if word == '':
			continue
		if word in word_list:
			word_list[word] += 1
		else:
			word_list[word] = 1

	# sort word-frequency dictionary based on how many times they showed up
	word_list = sorted(word_list.items(), key=lambda x:x[1], reverse=True)
	return word_list

# print word-frequency dictionary in a nice format
def print_words(word_list):
	for (k,v) in word_list:
		print k+':'+str(v)

def main():
	sanity_check(sys.argv);

	# open the txt file if it exists
	try:
		text_file = open(sys.argv[1]);
	except Exception as e:
		print "File provided does not exist"
		sys.exit(0)
	
	word_list = process_file(text_file)
	print_words(word_list)

if __name__ == '__main__':
	main()