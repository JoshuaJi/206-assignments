import sys

def sanity_check(argv):
	if len(sys.argv) != 2:
		print "Please provide only the file name"
		sys.exit(0)

def process_file(text_file):
	word_pair_list = {};
	line = text_file.read()
	line = line.replace('-', ' ')
	line = line.replace('\n', ' ')
	line = ''.join([i for i in line if (i.isalpha() or i == ' ')])
	
	words = line.split(' ')
	words = filter(lambda word:word != '', words)
	for i in xrange(len(words)-1):
		
		word_pair = words[i] + '-' + words[i+1]
		word_pair = word_pair.strip(".,!?()\{\}[]\'\"")
		word_pair = word_pair.lower()
		
		if word_pair in word_pair_list:
			word_pair_list[word_pair] += 1
		else:
			word_pair_list[word_pair] = 1
	word_pair_list = sorted(word_pair_list.items(), key=lambda x:x[1], reverse=True)
	return word_pair_list

def print_words(word_pair_list):
	for (k,v) in word_pair_list:
		print k+':'+str(v)

def main():
	sanity_check(sys.argv);
	try:
		text_file = open(sys.argv[1]);
	except Exception as e:
		print "File provided does not exist"
		sys.exit(0)
	
	word_pair_list = process_file(text_file)
	print_words(word_pair_list)

if __name__ == '__main__':
	main()